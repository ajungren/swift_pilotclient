/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift Project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_COMPONENTS_SETTINGSSIMULATORCOMPONENT_H
#define BLACKGUI_COMPONENTS_SETTINGSSIMULATORCOMPONENT_H

#include "blackcore/application/applicationsettings.h"
#include "blackgui/blackguiexport.h"
#include "blackmisc/settingscache.h"
#include "blackmisc/simulation/simulatorplugininfolist.h"

#include <QFrame>
#include <QObject>
#include <QScopedPointer>
#include <QString>

namespace BlackCore { class CPluginManagerSimulator; }
namespace BlackMisc { namespace Simulation { class CSimulatorPluginInfo; } }
namespace Ui { class CSettingsSimulatorComponent; }
namespace BlackGui
{
    namespace Components
    {
        //! All simulator settings component (GUI)
        class BLACKGUI_EXPORT CSettingsSimulatorComponent : public QFrame
        {
            Q_OBJECT

        public:
            //! Constructor
            explicit CSettingsSimulatorComponent(QWidget *parent = nullptr);

            //! Destructor
            virtual ~CSettingsSimulatorComponent();

        private:
            //! Driver plugin enabled/disabled
            void pluginStateChanged(const QString &identifier, bool enabled);

            //! Apply max.aircraft
            void onApplyMaxRenderedAircraft();

            //! Apply max.distance
            void onApplyMaxRenderedDistance();

            //! Apply disable rendering
            void onApplyDisableRendering();

            //! Apply time synchronization
            void onApplyTimeSync();

            //! Clear restricted rendering
            void clearRestricedRendering();

            //! Simulator plugin changed
            void simulatorPluginChanged(const BlackMisc::Simulation::CSimulatorPluginInfo &info);

            //! Open plugin details window
            void showPluginDetails(const QString &identifier);

            //! Show plugin config
            void showPluginConfig(const QString &identifier);

            //! Select/deselect enabled/disabled plugins
            void reloadPluginConfig();

            //! Check plugins again
            void checkSimulatorPlugins();

            //! Set the GUI values
            void setGuiValues();

            //! Available plugins, auto pseudo plugin added
            BlackMisc::Simulation::CSimulatorPluginInfoList getAvailablePlugins() const;

            QScopedPointer<Ui::CSettingsSimulatorComponent> ui; //!< UI
            bool m_pluginLoaded = false; //!< plugin loaded?
            BlackCore::CPluginManagerSimulator* m_plugins = nullptr;
            BlackMisc::CSetting<BlackCore::Application::TEnabledSimulators> m_enabledSimulators { this, &CSettingsSimulatorComponent::reloadPluginConfig };
        };
    }
} // namespace

#endif // guard
