/* Copyright (C) 2017
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution. No part of swift project, including this file, may be copied, modified, propagated,
 * or distributed except according to the terms contained in the LICENSE file.
 */

//! \file

#ifndef BLACKSIMPLUGIN_SIMULATOR_P3DFACTORY_H
#define BLACKSIMPLUGIN_SIMULATOR_P3DFACTORY_H

#include "blackcore/simulator.h"
#include "blackmisc/simulation/simulatorplugininfo.h"

#include <QObject>
#include <QtPlugin>

namespace BlackSimPlugin::P3D
{
    //! Factory implementation to create CSimulatorP3D instances
    class CSimulatorP3DFactory :
        public QObject,
        public BlackCore::ISimulatorFactory
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.swift-project.blackcore.simulatorinterface" FILE "simulatorp3d.json")
        Q_INTERFACES(BlackCore::ISimulatorFactory)

    public:
        //! \copydoc BlackCore::ISimulatorFactory::create
        virtual BlackCore::ISimulator *create(const BlackMisc::Simulation::CSimulatorPluginInfo &info,
                                                BlackMisc::Simulation::IOwnAircraftProvider *ownAircraftProvider,
                                                BlackMisc::Simulation::IRemoteAircraftProvider *remoteAircraftProvider,
                                                BlackMisc::Weather::IWeatherGridProvider *weatherGridProvider,
                                                BlackMisc::Network::IClientProvider *clientProvider) override;

        //! \copydoc BlackCore::ISimulatorFactory::createListener
        virtual BlackCore::ISimulatorListener *createListener(const BlackMisc::Simulation::CSimulatorPluginInfo &info) override;
    };
} // namespace

#endif // guard
