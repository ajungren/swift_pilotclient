/* Copyright (C) 2013
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

#include "plugin.h"
#include "service.h"
#include "traffic.h"
#include "weather.h"
#include "utils.h"
#include "XPLM/XPLMProcessing.h"
#include <cmath>
#include <functional>
#include <thread>

namespace {
    inline std::string xswiftbusServiceName() {
        return std::string("org.swift-project.xswiftbus");
    }
}

namespace XSwiftBus
{

    CPlugin::CPlugin()
        : m_dbusConnection(std::make_shared<CDBusConnection>()), m_menu(CMenu::mainMenu().subMenu("XSwiftBus"))
    {
        m_startServerMenuItem = m_menu.item("Start XSwiftBus", [this]{ startServer(CDBusConnection::SessionBus); });
        m_showHideLabelsMenuItem = m_menu.item("Show/Hide Aircraft Labels", [this]
        {
            m_traffic->setDrawingLabels(!m_traffic->isDrawingLabels());
        });
        m_showHideLabelsMenuItem.setEnabled(false);
        m_messageWindowSubMenu = m_menu.subMenu("Message Window");
        m_toggleMessageWindowMenuItem = m_messageWindowSubMenu.item("Show/Hide", [this]
        {
            m_service->toggleMessageBoxVisibility();
        });
        m_toggleMessageWindowMenuItem.setEnabled(false);
        m_popupMessageWindowMenuItem = m_messageWindowSubMenu.checkableItem("Pop up Window on new Nessage", true, [this] (bool checked)
        {
            m_popupMessageWindowMenuItem.setChecked(!checked);
            m_service->setPopupMessageWindow(!checked);
        });
        m_popupMessageWindowMenuItem.setEnabled(false);
        m_disappearMessageWindowMenuItem = m_messageWindowSubMenu.checkableItem("Hide Message Window after 5s", true, [this] (bool checked)
        {
            m_disappearMessageWindowMenuItem.setChecked(!checked);
            m_service->setDisappearMessageWindow(!checked);
        });
        m_disappearMessageWindowMenuItem.setEnabled(false);
        m_planeViewSubMenu = m_menu.subMenu("Follow Plane View");
        planeViewOwnAircraftMenuItem = m_planeViewSubMenu.item("Own Aircraft", [this] { switchToOwnAircraftView(); });

        readConfig();

        /*m_dbusThread = std::thread([this]()
        {
            while(!m_shouldStop)
            {
                m_dbusConnection->runBlockingEventLoop();
            }
        });*/

        XPLMRegisterFlightLoopCallback(flightLoopCallback, -1, this);
    }

    CPlugin::~CPlugin()
    {
        XPLMUnregisterFlightLoopCallback(flightLoopCallback, this);
        m_dbusConnection->close();
        m_shouldStop = true;
        if (m_dbusThread.joinable()) { m_dbusThread.join(); }
    }

    void CPlugin::readConfig()
    {
        initXPlanePath();
        auto configFilePath = g_xplanePath + "Resources" + g_sep + "plugins" + g_sep + "xswiftbus" + g_sep + "xswiftbus.conf";
        m_pluginConfig.setFilePath(configFilePath);
        m_pluginConfig.parse();
        m_pluginConfig.print();
    }

    void CPlugin::startServer(CDBusConnection::BusType bus)
    {
        (void) bus;
        // for (auto &item : m_startServerMenuItems) { item.setEnabled(false); }
        m_startServerMenuItem.setEnabled(false);

        m_service = std::make_unique<CService>();
        m_traffic = std::make_unique<CTraffic>();
        m_weather = std::make_unique<CWeather>();

        m_traffic->setPlaneViewMenu(m_planeViewSubMenu);

        m_showHideLabelsMenuItem.setEnabled(true);
        m_toggleMessageWindowMenuItem.setEnabled(true);
        m_popupMessageWindowMenuItem.setEnabled(true);
        m_disappearMessageWindowMenuItem.setEnabled(true);

        if (m_pluginConfig.getDBusMode() == CConfig::DBusP2P)
        {
            m_dbusP2PServer = std::make_unique<CDBusServer>();

            // FIXME: make listen address configurable
            std::string listenAddress = "tcp:host=" + m_pluginConfig.getDBusAddress() + ",port=" + std::to_string(m_pluginConfig.getDBusPort());
            m_dbusP2PServer->listen(listenAddress);
            m_dbusP2PServer->setDispatcher(&m_dbusDispatcher);

            m_dbusP2PServer->setNewConnectionFunc([this](const std::shared_ptr<CDBusConnection> &conn)
            {
                m_dbusConnection = conn;
                m_dbusConnection->setDispatcher(&m_dbusDispatcher);
                m_service->setDBusConnection(m_dbusConnection);
                m_service->registerDBusObjectPath(m_service->InterfaceName(), m_service->ObjectPath());
                m_traffic->setDBusConnection(m_dbusConnection);
                m_traffic->registerDBusObjectPath(m_traffic->InterfaceName(), m_traffic->ObjectPath());
                m_weather->setDBusConnection(m_dbusConnection);
                m_weather->registerDBusObjectPath(m_weather->InterfaceName(), m_weather->ObjectPath());
            });
        }
        else
        {
            // Todo: retry if it fails
            bool success = m_dbusConnection->connect(CDBusConnection::SessionBus);

            if (!success)
            {
                // Print error
                return;
            }

            m_dbusConnection->setDispatcher(&m_dbusDispatcher);
            m_dbusConnection->requestName(xswiftbusServiceName());

            m_service->setDBusConnection(m_dbusConnection);
            m_service->registerDBusObjectPath(m_service->InterfaceName(), m_service->ObjectPath());
            m_traffic->setDBusConnection(m_dbusConnection);
            m_traffic->registerDBusObjectPath(m_traffic->InterfaceName(), m_traffic->ObjectPath());
            m_weather->setDBusConnection(m_dbusConnection);
            m_weather->registerDBusObjectPath(m_weather->InterfaceName(), m_weather->ObjectPath());
        }

        INFO_LOG("XSwiftBus started.");
    }

    void CPlugin::switchToOwnAircraftView()
    {
        /* This is the hotkey callback.  First we simulate a joystick press and
         * release to put us in 'free view 1'.  This guarantees that no panels
         * are showing and we are an external view. */
        XPLMCommandButtonPress(xplm_joy_v_fr1);
        XPLMCommandButtonRelease(xplm_joy_v_fr1);

        /* Now we control the camera until the view changes. */
        XPLMControlCamera(xplm_ControlCameraUntilViewChanges, orbitOwnAircraftFunc, this);
    }

    void CPlugin::onAircraftModelChanged()
    {
        if (m_service)
        {
            m_service->onAircraftModelChanged();
        }
    }

    void CPlugin::onAircraftRepositioned()
    {
        if (m_service)
        {
            m_service->updateAirportsInRange();
        }
    }

    float CPlugin::flightLoopCallback(float, float, int, void *refcon)
    {
        auto *plugin = static_cast<CPlugin *>(refcon);
        plugin->m_dbusDispatcher.runOnce();
        if (plugin->m_service) { plugin->m_service->process(); }
        if (plugin->m_weather) { plugin->m_weather->process(); }
        if (plugin->m_traffic) { plugin->m_traffic->process(); }
        return -1;
    }

    int CPlugin::orbitOwnAircraftFunc(XPLMCameraPosition_t *cameraPosition, int isLosingControl, void *refcon)
    {
        auto *plugin = static_cast<CPlugin *>(refcon);

        if (isLosingControl == 1) { return 0; }

        if (cameraPosition)
        {
            int w, h, x, y;
            // First get the screen size and mouse location. We will use this to decide
            // what part of the orbit we are in. The mouse will move us up-down and around.
            // fixme: In a future update, change the orbit only while right mouse button is pressed.
            XPLMGetScreenSize(&w, &h);
            XPLMGetMouseLocation(&x, &y);
            double heading = 360.0 * static_cast<double>(x) / static_cast<double>(w);
            double pitch = 20.0 * ((static_cast<double>(y) / static_cast<double>(h)) * 2.0 - 1.0);

            // Now calculate where the camera should be positioned to be 200
            // meters from the plane and pointing at the plane at the pitch and
            // heading we wanted above.
            static const double PI = std::acos(-1);
            double dx = -50.0 * sin(heading * PI / 180.0);
            double dz = 50.0 * cos(heading * PI / 180.0);
            double dy = -50.0 * tan(pitch * PI / 180.0);

            double lx, ly, lz;

            lx = plugin->m_ownAircraftPositionX.get();
            ly = plugin->m_ownAircraftPositionY.get();
            lz = plugin->m_ownAircraftPositionZ.get();

            // Fill out the camera position info.
            cameraPosition->x = static_cast<float>(lx + dx);
            cameraPosition->y = static_cast<float>(ly + dy);
            cameraPosition->z = static_cast<float>(lz + dz);
            cameraPosition->pitch = static_cast<float>(pitch);
            cameraPosition->heading = static_cast<float>(heading);
            cameraPosition->roll = 0;
        }

        // Return 1 to indicate we want to keep controlling the camera.
        return 1;
    }
}
