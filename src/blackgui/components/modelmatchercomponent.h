/* Copyright (C) 2016
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_COMPONENT_MODELMATCHERCOMPONENT_H
#define BLACKGUI_COMPONENT_MODELMATCHERCOMPONENT_H

#include "blackcore/aircraftmatcher.h"
#include "blackgui/blackguiexport.h"
#include "blackmisc/network/entityflags.h"
#include "blackmisc/simulation/aircraftmodel.h"
#include "blackmisc/simulation/aircraftmodelsetloader.h"
#include "blackmisc/simulation/simulatedaircraft.h"
#include "blackmisc/simulation/simulatorinfo.h"

#include <QFrame>
#include <QObject>
#include <QScopedPointer>

class QWidget;

namespace Ui { class CModelMatcherComponent; }
namespace BlackGui
{
    namespace Components
    {
        /*!
         * Model matcher testing and configuration
         */
        class BLACKGUI_EXPORT CModelMatcherComponent : public QFrame
        {
            Q_OBJECT

        public:
            //! Constructor
            explicit CModelMatcherComponent(QWidget *parent = nullptr);

            //! Destructor
            virtual ~CModelMatcherComponent();

        public slots:
            //! Tab (where this component is embedded) has been changed
            void tabIndexChanged(int index);

        private:
            //! Simulator switched
            void onSimulatorChanged(const BlackMisc::Simulation::CSimulatorInfo &simulator);

            //! Cache changed
            void onCacheChanged(BlackMisc::Simulation::CSimulatorInfo &simulator);

            //! Web data have been read
            void onWebDataRed(BlackMisc::Network::CEntityFlags::Entity entity, BlackMisc::Network::CEntityFlags::ReadState state, int number);

            //! Run the matcher
            void testModelMatching();

            //! Reverse lookup
            void reverseLookup();

            //! Init
            void redisplay();

            //! Pseudo aircraft created from entries
            BlackMisc::Simulation::CSimulatedAircraft createAircraft() const;

            //! Pseudo default aircraft
            BlackMisc::Simulation::CAircraftModel defaultModel() const;

            QScopedPointer<Ui::CModelMatcherComponent>     ui;
            BlackMisc::Simulation::CAircraftModelSetLoader m_modelSetLoader { this };
            BlackCore::CAircraftMatcher                    m_matcher { BlackCore::CAircraftMatcher::All, this };
        };
    } // ns
} // ns

#endif // guard
