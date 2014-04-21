/* Copyright (C) 2013 VATSIM Community / contributors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BLACKCORE_CONTEXTSIMULATOR_IMPL_H
#define BLACKCORE_CONTEXTSIMULATOR_IMPL_H

#include "blackcore/context_simulator.h"
#include "blackcore/context_network.h"
#include "blackcore/simulator.h"

#include "blacksim/simulatorinfo.h"
#include "blacksim/simulatorinfolist.h"

#include <QTimer>
#include <QDir>

namespace BlackCore
{
    /*!
     * \brief Network simulator concrete implementation
     */
    class CContextSimulator : public IContextSimulator
    {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", BLACKCORE_CONTEXTSIMULATOR_INTERFACENAME)
        friend class CRuntime;

    public:
        //! \brief Destructor
        virtual ~CContextSimulator();

    public slots:

        //! \copydoc IContextSimulator::getSimulatorPluginList()
        virtual BlackSim::CSimulatorInfoList getAvailableSimulatorPlugins() const override;
		
        //! \copydoc IContextSimulator::isConnected()
        virtual bool isConnected() const override;

        //! \brief Can we connect?
        virtual bool canConnect() override;

        //! \brief Connect to simulator
        virtual bool connectTo() override;

        //! \brief Disconnect from simulator
        virtual bool disconnectFrom() override;

        //! \copydoc IContextSimulator::getOwnAircraft()
        virtual BlackMisc::Aviation::CAircraft getOwnAircraft() const override;

        //! \copydoc IContextSimulator::getSimulatorInfo()
        virtual BlackSim::CSimulatorInfo getSimulatorInfo() const override;

        //! \copydoc IContextSimulator::loadSimulatorPlugin()
        virtual bool loadSimulatorPlugin (const BlackSim::CSimulatorInfo &simulatorInfo) override;

        //! \copydoc IContextSimulator::unloadSimulatorPlugin()
        virtual void unloadSimulatorPlugin () override;

    protected:
        //! \brief Constructor
        CContextSimulator(CRuntimeConfig::ContextMode, CRuntime *runtime);

        //! Register myself in DBus
        CContextSimulator *registerWithDBus(CDBusServer *server)
        {
            if (!server || this->m_mode != CRuntimeConfig::LocalInDbusServer) return this;
            server->addObject(CContextSimulator::ObjectPath(), this);
            return this;
        }

    private slots:
        //! \copydoc IContextSimulator::updateOwnAircraft()
        virtual void updateOwnAircraft();

        //! Set new connection status
        void setConnectionStatus(bool value);

    private:
        //! \brief find and catalog all simulator plugins
        void findSimulatorPlugins();

        BlackMisc::Aviation::CAircraft m_ownAircraft;
        BlackCore::ISimulator *m_simulator;
        QTimer *m_updateTimer;
        QDir m_pluginsDir;
        QSet<ISimulatorFactory*> m_simulatorFactories;
    };

} // namespace BlackCore

#endif // guard
