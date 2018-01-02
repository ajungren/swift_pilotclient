/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_COMPONENTS_FLIGHTPLANCOMPONENT_H
#define BLACKGUI_COMPONENTS_FLIGHTPLANCOMPONENT_H

#include "blackgui/overlaymessagesframe.h"
#include "blackgui/blackguiexport.h"
#include "blackmisc/simulation/data/lastmodel.h"
#include "blackmisc/simulation/simulatedaircraft.h"
#include "blackmisc/aviation/flightplan.h"
#include "blackmisc/network/data/lastserver.h"
#include "blackmisc/network/user.h"
#include "blackmisc/datacache.h"
#include "blackmisc/identifier.h"
#include "blackmisc/statusmessagelist.h"

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QTabWidget>
#include <QtGlobal>
#include <QFileDialog>

namespace Ui { class CFlightPlanComponent; }
namespace BlackGui
{
    namespace Components
    {
        //! Flight plan widget
        class BLACKGUI_EXPORT CFlightPlanComponent : public COverlayMessagesTabWidget
        {
            Q_OBJECT

        public:
            //! Constructor
            explicit CFlightPlanComponent(QWidget *parent = nullptr);

            //! Destructor
            virtual ~CFlightPlanComponent();

            //! Login data were set
            void loginDataSet();

            //! Prefill with aircraft dara
            void fillWithFlightPlanData(const BlackMisc::Aviation::CFlightPlan &flightPlan);

            //! Get this flight plan
            BlackMisc::Aviation::CFlightPlan getFlightPlan() const;

        private:
            QScopedPointer<Ui::CFlightPlanComponent> ui;
            BlackMisc::Aviation::CFlightPlan m_flightPlan; //!< My flight plan
            BlackMisc::Simulation::CAircraftModel m_model; //!< currently used model
            BlackMisc::CIdentifier m_identifier; //!< Flightplan identifier
            BlackMisc::CDataReadOnly<BlackMisc::Simulation::Data::TLastModel> m_lastAircraftModel { this }; //!< recently used aircraft model
            BlackMisc::CDataReadOnly<BlackMisc::Network::Data::TLastServer> m_lastServer { this }; //!< recently used server (VATSIM, other)

            static constexpr int showOverlayMs = 5000;

            //! Validate, generates status messages
            BlackMisc::CStatusMessageList validateAndInitializeFlightPlan(BlackMisc::Aviation::CFlightPlan &fligtPlan);

            //! Default value for ICAO airports
            static const QString &defaultIcao() { static const QString d("ICAO"); return d; }

            //! Default value for time
            static const QString &defaultTime() { static const  QString t("00:00"); return t; }

            //! Identifier
            static const BlackMisc::CIdentifier &flightPlanIdentifier();

            //! Prefill with own aircraft data
            void prefillWithOwnAircraftData();

            //! Prefill with aircraft data
            void prefillWithAircraftData(const BlackMisc::Simulation::CSimulatedAircraft &aircraft);

            //! Prefill with user data
            void prefillWithUserData(const BlackMisc::Network::CUser &user);

            //! Set completers
            void initCompleters();

            //! File name for load/save
            QString getDefaultFilename(bool load);

            //! Call \sa buildRemarksString from combo box signal
            void currentTextChangedToBuildRemarks(const QString &text) { this->buildRemarksString(); Q_UNUSED(text); }

            //! Voice combo boxes shall display the same
            void syncVoiceComboBoxes(const QString &text);

            //! Send flightplan
            void sendFlightPlan();

            //! Reset Flightplan
            void resetFlightPlan();

            //! Load from disk
            void loadFromDisk();

            //! Save to disk
            void saveToDisk();

            //! Set SELCAL in own aircrafr
            void setSelcalInOwnAircraft();

            //! Load Flightplan
            void loadFlightPlanFromNetwork();

            //! Validate Flightplan
            void validateFlightPlan();

            //! Remarks
            void buildRemarksString();

            //! Copy over
            void copyRemarksConfirmed() { this->copyRemarks(true); }

            //! Copy over
            void copyRemarks(bool confirm = true);

            //! Show generator tab page
            void currentTabGenerator();

            //! GUI init complete
            void swiftWebDataRead();

            //! Build "H/B737/F"
            void buildPrefixIcaoSuffix();

            //! Prefix check box changed
            void prefixCheckBoxChanged();

            //! Aircraft type changed
            void aircraftTypeChanged();

            //! Something like "H/B737/F"
            QString getPrefixIcaoSuffix() const;

            //! Aircraft type as ICAO code
            BlackMisc::Aviation::CAircraftIcaoCode getAircraftIcaoCode() const;

            //! Show tab of equipment codes
            void showEquipmentCodesTab();

            //! VFR rules?
            bool isVfr() const;

            //! Override remarks message dialog
            bool overrideRemarks();

            //! Guess some FP values/setting
            void anticipateValues();
        };
    } // ns
} // ns
#endif // guard
