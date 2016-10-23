/* Copyright (C) 2013
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKCORE_CONTEXTSIMULATOR_PROXY_H
#define BLACKCORE_CONTEXTSIMULATOR_PROXY_H

#include "blackcore/blackcoreexport.h"
#include "blackcore/context/contextsimulator.h"
#include "blackcore/corefacadeconfig.h"
#include "blackmisc/aviation/airportlist.h"
#include "blackmisc/pixmap.h"
#include "blackmisc/pq/length.h"
#include "blackmisc/pq/time.h"
#include "blackmisc/simulation/aircraftmodellist.h"
#include "blackmisc/simulation/simulatorplugininfo.h"
#include "blackmisc/simulation/simulatorplugininfolist.h"
#include "blackmisc/simulation/simulatorsetup.h"

#include <QObject>
#include <QString>

class QDBusConnection;
namespace BlackMisc
{
    class CGenericDBusInterface;
    namespace Simulation { class CSimulatedAircraft; }
}

namespace BlackCore
{
    class CCoreFacade;
    namespace Context
    {
        //! DBus proxy for Simulator Context
        //! \ingroup dbus
        class BLACKCORE_EXPORT CContextSimulatorProxy : public IContextSimulator
        {
            Q_OBJECT
            friend class IContextSimulator;

        public:
            //! Destructor
            ~CContextSimulatorProxy() {}

        public slots:
            //! \name Interface overrides
            //! @{
            virtual BlackMisc::Simulation::CSimulatorPluginInfo getSimulatorPluginInfo() const override;
            virtual BlackMisc::Simulation::CSimulatorPluginInfoList getAvailableSimulatorPlugins() const override;
            virtual bool startSimulatorPlugin(const BlackMisc::Simulation::CSimulatorPluginInfo &simulatorInfo) override;
            virtual void stopSimulatorPlugin(const BlackMisc::Simulation::CSimulatorPluginInfo &simulatorInfo) override;
            virtual int getSimulatorStatus() const override;
            virtual BlackMisc::Aviation::CAirportList getAirportsInRange() const override;
            virtual BlackMisc::Simulation::CAircraftModelList getModelSet() const override;
            virtual QStringList getModelSetStrings() const override;
            virtual BlackMisc::Simulation::CAircraftModelList getModelSetModelsStartingWith(const QString modelString) const override;
            virtual int getModelSetCount() const override;
            virtual BlackMisc::Simulation::CSimulatorSetup getSimulatorSetup() const override;
            virtual bool setTimeSynchronization(bool enable, const BlackMisc::PhysicalQuantities::CTime &offset) override;
            virtual bool isTimeSynchronized() const override;
            virtual int getMaxRenderedAircraft() const override;
            virtual void setMaxRenderedAircraft(int number) override;
            virtual void setMaxRenderedDistance(const BlackMisc::PhysicalQuantities::CLength &distance) override;
            virtual void deleteAllRenderingRestrictions() override;
            virtual bool isRenderingRestricted() const override;
            virtual bool isRenderingEnabled() const override;
            virtual BlackMisc::PhysicalQuantities::CLength getMaxRenderedDistance() const override;
            virtual BlackMisc::PhysicalQuantities::CLength getRenderedDistanceBoundary() const override;
            virtual QString getRenderRestrictionText() const override;
            virtual BlackMisc::PhysicalQuantities::CTime getTimeSynchronizationOffset() const override;
            virtual BlackMisc::CPixmap iconForModel(const QString &modelString) const override;
            virtual void highlightAircraft(const BlackMisc::Simulation::CSimulatedAircraft &aircraftToHighlight, bool enableHighlight, const BlackMisc::PhysicalQuantities::CTime &displayTime) override;
            virtual void requestWeatherGrid(const BlackMisc::Weather::CWeatherGrid &weatherGrid, const BlackMisc::CIdentifier &identifier) override;
            virtual void enableDebugMessages(bool driver, bool interpolator) override;
            virtual BlackMisc::CStatusMessageList getMatchingMessages(const BlackMisc::Aviation::CCallsign &callsign) const;
            virtual bool isMatchingMessagesEnabled() const;
            virtual void enableMatchingMessages(bool enabled);
            //! @}

        private:
            BlackMisc::CGenericDBusInterface *m_dBusInterface;

            //! Relay connection signals to local signals
            void relaySignals(const QString &serviceName, QDBusConnection &connection);

        protected:
            //! Constructor
            CContextSimulatorProxy(CCoreFacadeConfig::ContextMode mode, CCoreFacade *runtime) : IContextSimulator(mode, runtime), m_dBusInterface(0) {}

            //! DBus version constructor
            CContextSimulatorProxy(const QString &serviceName, QDBusConnection &connection, CCoreFacadeConfig::ContextMode mode, CCoreFacade *runtime);
        };
    } // ns
} // ns
#endif // guard
