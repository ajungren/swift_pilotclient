/* Copyright (C) 2015
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKCORE_CONTEXT_CONTEXTSIMULATOR_EMPTY_H
#define BLACKCORE_CONTEXT_CONTEXTSIMULATOR_EMPTY_H

#include "blackcoreexport.h"
#include "contextsimulator.h"
#include "blackmisc/logmessage.h"

namespace BlackCore
{
    namespace Context
    {
        //! Empty context, used during shutdown/initialization
        class BLACKCORE_EXPORT CContextSimulatorEmpty : public IContextSimulator
        {
            Q_OBJECT

        public:
            //! Constructor
            CContextSimulatorEmpty(CCoreFacade *runtime) : IContextSimulator(CCoreFacadeConfig::NotUsed, runtime) {}

        public slots:
            //! \copydoc IContextSimulator::getSimulatorPluginInfo
            virtual BlackMisc::Simulation::CSimulatorPluginInfo getSimulatorPluginInfo() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CSimulatorPluginInfo();
            }

            //! \copydoc IContextSimulator::getAvailableSimulatorPlugins
            virtual BlackMisc::Simulation::CSimulatorPluginInfoList getAvailableSimulatorPlugins() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CSimulatorPluginInfoList();
            }

            //! \copydoc IContextSimulator::startSimulatorPlugin
            virtual bool startSimulatorPlugin(const BlackMisc::Simulation::CSimulatorPluginInfo &simulatorInfo) override
            {
                Q_UNUSED(simulatorInfo);
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc IContextSimulator::checkListeners
            virtual int checkListeners() override
            {
                return 0;
            }

            //! \copydoc IContextSimulator::getSimulatorStatus
            virtual int getSimulatorStatus() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return 0;
            }

            //! \copydoc IContextSimulator::stopSimulatorPlugin
            virtual void stopSimulatorPlugin(const BlackMisc::Simulation::CSimulatorPluginInfo &simulatorInfo) override
            {
                Q_UNUSED(simulatorInfo);
                logEmptyContextWarning(Q_FUNC_INFO);
            }

            //! \copydoc IContextSimulator::getAirportsInRange
            virtual BlackMisc::Aviation::CAirportList getAirportsInRange(bool recalculatePosition) const override
            {
                Q_UNUSED(recalculatePosition);
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Aviation::CAirportList();
            }

            //! \copydoc IContextSimulator::getModelSet
            virtual BlackMisc::Simulation::CAircraftModelList getModelSet() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CAircraftModelList();
            }

            //! \copydoc IContextSimulator::simulatorsWithInitializedModelSet
            virtual BlackMisc::Simulation::CSimulatorInfo simulatorsWithInitializedModelSet() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CSimulatorInfo();
            }

            //! \copydoc IContextSimulator::verifyPrerequisites
            virtual BlackMisc::CStatusMessageList verifyPrerequisites() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::CStatusMessageList();
            }

            //! \copydoc IContextSimulator::getModelSetLoaderSimulator
            virtual BlackMisc::Simulation::CSimulatorInfo getModelSetLoaderSimulator() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CSimulatorInfo();
            }

            //! \copydoc IContextSimulator::setModelSetLoaderSimulator
            virtual void setModelSetLoaderSimulator(const BlackMisc::Simulation::CSimulatorInfo &simulator) override
            {
                Q_UNUSED(simulator);
                logEmptyContextWarning(Q_FUNC_INFO);
            }

            //! \copydoc IContextSimulator::getModelSetStrings
            virtual QStringList getModelSetStrings() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return QStringList();
            }

            //! \copydoc IContextSimulator::getModelSetCompleterStrings
            virtual QStringList getModelSetCompleterStrings(bool sorted) const override
            {
                Q_UNUSED(sorted);
                logEmptyContextWarning(Q_FUNC_INFO);
                return QStringList();
            }

            //! \copydoc IContextSimulator::isKnownModel
            virtual bool isKnownModel(const QString &modelstring) const override
            {
                Q_UNUSED(modelstring);
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc IContextSimulator::getModelSetModelsStartingWith
            virtual BlackMisc::Simulation::CAircraftModelList getModelSetModelsStartingWith(const QString &modelString) const override
            {
                Q_UNUSED(modelString);
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CAircraftModelList();
            }

            //! \copydoc IContextSimulator::getModelSetCount
            virtual int getModelSetCount() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return 0;
            }

            //! \copydoc IContextSimulator::getSimulatorInternals
            virtual BlackMisc::Simulation::CSimulatorInternals getSimulatorInternals() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CSimulatorInternals();
            }

            //! \copydoc IContextSimulator::setTimeSynchronization
            virtual bool setTimeSynchronization(bool enable, const BlackMisc::PhysicalQuantities::CTime &offset) override
            {
                Q_UNUSED(enable);
                Q_UNUSED(offset);
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc IContextSimulator::isTimeSynchronized
            virtual bool isTimeSynchronized() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc ISimulator::getInterpolationSetupGlobal
            virtual BlackMisc::Simulation::CInterpolationAndRenderingSetupGlobal getInterpolationAndRenderingSetupGlobal() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CInterpolationAndRenderingSetupGlobal();
            }

            //! \copydoc ISimulator::getInterpolationSetupsPerCallsign
            virtual BlackMisc::Simulation::CInterpolationSetupList getInterpolationAndRenderingSetupsPerCallsign() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CInterpolationSetupList();
            }

            //! \copydoc ISimulator::getInterpolationSetupPerCallsignOrDefault
            virtual BlackMisc::Simulation::CInterpolationAndRenderingSetupPerCallsign getInterpolationAndRenderingSetupPerCallsignOrDefault(const BlackMisc::Aviation::CCallsign &callsign) const override
            {
                Q_UNUSED(callsign);
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::Simulation::CInterpolationAndRenderingSetupPerCallsign();
            }

            //! \copydoc BlackMisc::Simulation::IInterpolationSetupProvider::setInterpolationSetupGlobal
            virtual void setInterpolationAndRenderingSetupGlobal(const BlackMisc::Simulation::CInterpolationAndRenderingSetupGlobal &setup) override
            {
                Q_UNUSED(setup);
                logEmptyContextWarning(Q_FUNC_INFO);
            }

            //! \copydoc BlackMisc::Simulation::IInterpolationSetupProvider::setInterpolationSetupsPerCallsign
            virtual bool setInterpolationAndRenderingSetupsPerCallsign(const BlackMisc::Simulation::CInterpolationSetupList &setups, bool ignoreSameAsGlobal) override
            {
                Q_UNUSED(setups);
                Q_UNUSED(ignoreSameAsGlobal);
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc IContextSimulator::getInterpolationMessages
            virtual BlackMisc::CStatusMessageList getInterpolationMessages(const BlackMisc::Aviation::CCallsign &callsign) const override
            {
                Q_UNUSED(callsign);
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::CStatusMessageList();
            }

            //! \copydoc IContextSimulator::getTimeSynchronizationOffset
            virtual BlackMisc::PhysicalQuantities::CTime getTimeSynchronizationOffset() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::PhysicalQuantities::CTime();
            }

            //! \copydoc IContextSimulator::iconForModel
            virtual BlackMisc::CPixmap iconForModel(const QString &modelString) const override
            {
                Q_UNUSED(modelString);
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::CPixmap();
            }

            //! \copydoc IContextSimulator::highlightAircraft
            virtual void highlightAircraft(const BlackMisc::Simulation::CSimulatedAircraft &aircraftToHighlight, bool enableHighlight, const BlackMisc::PhysicalQuantities::CTime &displayTime) override
            {
                Q_UNUSED(aircraftToHighlight);
                Q_UNUSED(enableHighlight);
                Q_UNUSED(displayTime);
                logEmptyContextWarning(Q_FUNC_INFO);
            }

            //! \copydoc IContextSimulator::followAircraft
            virtual bool followAircraft(const BlackMisc::Aviation::CCallsign &callsign) override
            {
                Q_UNUSED(callsign);
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc IContextSimulator::resetToModelMatchingAircraft
            virtual bool resetToModelMatchingAircraft(const BlackMisc::Aviation::CCallsign &callsign) override
            {
                Q_UNUSED(callsign);
                return false;
            }


            //! \copydoc IContextSimulator::doMatchingsAgain
            virtual int doMatchingsAgain() override
            {
                return 0;
            }

            //! \copydoc IContextSimulator::doMatchingAgain
            virtual bool doMatchingAgain(const BlackMisc::Aviation::CCallsign &callsign) override
            {
                Q_UNUSED(callsign);
                return false;
            }

            //! \copydoc IContextSimulator::setWeatherActivated
            virtual void setWeatherActivated(bool activated) override
            {
                Q_UNUSED(activated);
                logEmptyContextWarning(Q_FUNC_INFO);
            }

            //! \copydoc IContextSimulator::requestWeatherGrid
            virtual void requestWeatherGrid(const BlackMisc::Weather::CWeatherGrid &weatherGrid, const BlackMisc::CIdentifier &identifier) override
            {
                Q_UNUSED(weatherGrid);
                Q_UNUSED(identifier);
                logEmptyContextWarning(Q_FUNC_INFO);
            }

            //! \copydoc IContextSimulator::getMatchingMessages
            virtual BlackMisc::CStatusMessageList getMatchingMessages(const BlackMisc::Aviation::CCallsign &callsign) const override
            {
                Q_UNUSED(callsign);
                logEmptyContextWarning(Q_FUNC_INFO);
                return BlackMisc::CStatusMessageList();
            }

            //! \copydoc IContextSimulator::enableMatchingMessages
            virtual void enableMatchingMessages(bool enable) override
            {
                Q_UNUSED(enable);
                logEmptyContextWarning(Q_FUNC_INFO);
            }

            //! \copydoc IContextSimulator::isMatchingMessagesEnabled
            virtual bool isMatchingMessagesEnabled() const override
            {
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc IContextSimulator::parseCommandLine
            virtual bool parseCommandLine(const QString &commandLine, const BlackMisc::CIdentifier &originator) override
            {
                Q_UNUSED(commandLine);
                Q_UNUSED(originator);
                logEmptyContextWarning(Q_FUNC_INFO);
                return false;
            }

            //! \copydoc IContextSimulator::getCurrentMatchingStatistics
            virtual BlackMisc::Simulation::CMatchingStatistics getCurrentMatchingStatistics(bool missingOnly) const override
            {
                Q_UNUSED(missingOnly);
                return BlackMisc::Simulation::CMatchingStatistics();
            }

            //! \copydoc IContextSimulator::setMatchingSetup
            virtual void setMatchingSetup(const BlackMisc::Simulation::CAircraftMatcherSetup &setup) override
            {
                Q_UNUSED(setup);
            }

            //! \copydoc IContextSimulator::setMatchingSetup
            virtual BlackMisc::Simulation::CAircraftMatcherSetup getMatchingSetup() const override
            {
                return BlackMisc::Simulation::CAircraftMatcherSetup();
            }

            //! \copydoc IContextSimulator::copyFsxTerrainProbe
            virtual BlackMisc::CStatusMessageList copyFsxTerrainProbe(const BlackMisc::Simulation::CSimulatorInfo &simulator) override
            {
                Q_UNUSED(simulator);
                return BlackMisc::CStatusMessageList();
            }
        };
    } // namespace
} // namespace

#endif // guard
