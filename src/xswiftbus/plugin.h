/* Copyright (C) 2013
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

#ifndef BLACKSIM_XSWIFTBUS_PLUGIN_H
#define BLACKSIM_XSWIFTBUS_PLUGIN_H

//! \file

/*!
 * \namespace XSwiftBus
 * Plugin loaded by X-Plane which publishes a DBus service
 */

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "dbusconnection.h"
#include "dbusdispatcher.h"
#include "dbusserver.h"
#include "datarefs.h"
#include "XPLM/XPLMCamera.h"
#include "menus.h"
#include "config.h"
#include <memory>
#include <thread>

namespace XSwiftBus
{
    class CService;
    class CTraffic;
    class CWeather;

    /*!
     * Main plugin class
     */
    class CPlugin
    {
    public:
        //! Constructor
        CPlugin();

        //! Destructor
        ~CPlugin();

        //! Called by XPluginReceiveMessage when the model is changed
        void onAircraftModelChanged();

        //! Called by XPluginReceiveMessage when the aircraft is positioned at an airport
        void onAircraftRepositioned();

    private:
        CConfig m_pluginConfig;
        CDBusDispatcher m_dbusDispatcher;
        std::unique_ptr<CDBusServer> m_dbusP2PServer;
        std::shared_ptr<CDBusConnection> m_dbusConnection;
        std::unique_ptr<CService> m_service;
        std::unique_ptr<CTraffic> m_traffic;
        std::unique_ptr<CWeather> m_weather;
        CMenu m_menu;
        CMenuItem m_startServerMenuItem;
        CMenuItem m_showHideLabelsMenuItem;
        CMenu m_messageWindowSubMenu;
        CMenuItem m_toggleMessageWindowMenuItem;
        CMenuItem m_popupMessageWindowMenuItem;
        CMenuItem m_disappearMessageWindowMenuItem;
        CMenu m_planeViewSubMenu;
        CMenuItem planeViewOwnAircraftMenuItem;

        DataRef<xplane::data::sim::flightmodel::position::local_x> m_ownAircraftPositionX;
        DataRef<xplane::data::sim::flightmodel::position::local_y> m_ownAircraftPositionY;
        DataRef<xplane::data::sim::flightmodel::position::local_z> m_ownAircraftPositionZ;

        std::thread m_dbusThread;
        bool m_shouldStop = false;

        void readConfig();
        void startServer(CDBusConnection::BusType bus);
        void switchToOwnAircraftView();

        static float flightLoopCallback(float, float, int, void *refcon);
        static int orbitOwnAircraftFunc(XPLMCameraPosition_t *cameraPosition, int isLosingControl, void *refcon);
    };
}

#endif // guard
