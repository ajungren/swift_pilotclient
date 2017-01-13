/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift Project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_LOGINCOMPONENT_H
#define BLACKGUI_LOGINCOMPONENT_H

#include "blackcore/vatsim/vatsimsettings.h"
#include "blackcore/data/vatsimsetup.h"
#include "blackgui/blackguiexport.h"
#include "blackgui/settings/guisettings.h"
#include "blackmisc/simulation/simulatedaircraft.h"
#include "blackmisc/network/entityflags.h"
#include "blackmisc/network/server.h"
#include "blackmisc/network/user.h"
#include "blackmisc/digestsignal.h"
#include "blackmisc/aviation/callsign.h"
#include "blackmisc/settingscache.h"
#include "blackmisc/datacache.h"

#include <QFrame>
#include <QObject>
#include <QScopedPointer>
#include <QString>

class QTimer;
class QWidget;

namespace BlackMisc
{
    namespace Simulation
    {
        class CAircraftModel;
        class CSimulatedAircraft;
    }
}
namespace Ui { class CLoginComponent; }
namespace BlackGui
{
    namespace Components
    {
        class CDbQuickMappingWizard;

        /*!
         * Login component to flight network
         */
        class BLACKGUI_EXPORT CLoginComponent : public QFrame
        {
            Q_OBJECT

        public:
            //! Log categories
            static const BlackMisc::CLogCategoryList &getLogCategories();

            //! Constructor
            explicit CLoginComponent(QWidget *parent = nullptr);

            //! Automatically popup
            void setAutoPopupWizad(bool autoPopup);

            //! Destructor
            virtual ~CLoginComponent();

        signals:
            //! Login
            void loginOrLogoffSuccessful();

            //! Cancelled
            void loginOrLogoffCancelled();

            //! Request network settings
            void requestNetworkSettings();

            //! Relevant login data changed
            //! \private normally loginDataChangedDigest will be used
            void loginDataChanged();

            //! Relevant login data changed (digest version)
            void loginDataChangedDigest();

        public slots:
            //! Main info area changed
            void mainInfoAreaChanged(const QWidget *currentWidget);

        private slots:
            //! Login cancelled
            void ps_loginCancelled();

            //! Login requested
            void ps_toggleNetworkConnection();

            //! VATSIM data file was loaded
            void ps_onWebServiceDataRead(int entity, int stateInt, int number);

            //! Validate aircaft
            bool ps_validateAircraftValues();

            //! Validate VATSIM credentials
            bool ps_validateVatsimValues();

            //! Aircraft ICAO code has been changed
            void ps_changedAircraftIcao(const BlackMisc::Aviation::CAircraftIcaoCode &icao);

            //! Airline ICAO code has been changed
            void ps_changedAirlineIcao(const BlackMisc::Aviation::CAirlineIcaoCode &icao);

            //! Settings have been changed
            void ps_reloadSettings();

            //! Logoff countdown
            void ps_logoffCountdown();

            //! Reverse lookup model
            void ps_reverseLookupAircraftModel();

            //! Simulator model has been changed
            void ps_simulatorModelChanged(const BlackMisc::Simulation::CAircraftModel &model);

            //! Launch mapping wizard
            void ps_mappingWizard();

        private:
            //! GUI aircraft values, formatted
            struct CGuiAircraftValues
            {
                BlackMisc::Aviation::CCallsign         ownCallsign;
                BlackMisc::Aviation::CAircraftIcaoCode ownAircraftIcao;
                BlackMisc::Aviation::CAirlineIcaoCode  ownAirlineIcao;
                QString ownAircraftCombinedType;
                QString ownAircraftSimulatorModel;
            };

            //! VATSIM login data
            struct CVatsimValues
            {
                QString vatsimId;
                QString vatsimPassword;
                QString vatsimRealName;
                QString vatsimHomeAirport;
            };

            // -------------- values from GUI -----------------

            //! Values from GUI
            CGuiAircraftValues getAircraftValuesFromGui() const;

            //! Values from GUI
            CVatsimValues getVatsimValuesFromGui() const;

            //! User from VATSIM data
            BlackMisc::Network::CUser getUserFromVatsimGuiValues() const;

            //! Callsign from GUI
            BlackMisc::Aviation::CCallsign getCallsignFromGui() const;

            //! Set ICAO values
            //! \return changed values
            bool setGuiIcaoValues(const BlackMisc::Simulation::CAircraftModel &model, bool onlyIfEmpty);

            // -------------- values from GUI -----------------

            //! Update own callsign (own aircraft from what is set in the GUI)
            //! \return changed?
            bool updateOwnAircraftCallsignAndPilotFromGuiValues();

            //! Update own ICAO values (own aircraft from what is set in the GUI)
            //! \return changed?
            bool updateOwnAircaftIcaoValuesFromGuiValues();

            // -------------- others -----------------

            //! Own model and ICAO data for GUI and own aircraft
            void setOwnModelAndIcaoValues();

            //! Selected server (VATSIM)
            BlackMisc::Network::CServer getCurrentVatsimServer() const;

            //! Selected server (others)
            BlackMisc::Network::CServer getCurrentOtherServer() const;

            //! Set OK button string
            void setOkButtonString(bool connected);

            //! Show/hide elements as appropriate
            void setGuiVisibility(bool connected);

            //! Logoff countdown
            void startLogoffTimerCountdown();

            //! Completers
            void initCompleters(BlackMisc::Network::CEntityFlags::Entity entity);

            //! Highlight model field according to model data
            void highlightModelField(const BlackMisc::Simulation::CAircraftModel &model = {});

            //! Load from settings
            void loadRememberedVatsimData();

            //! Get a prefill model
            BlackMisc::Simulation::CAircraftModel getPrefillModel() const;

            QScopedPointer<Ui::CLoginComponent> ui;
            QScopedPointer<CDbQuickMappingWizard> m_mappingWizard;
            BlackMisc::CDigestSignal m_changedLoginDataDigestSignal { this, &CLoginComponent::loginDataChanged, &CLoginComponent::loginDataChangedDigest, 1500, 10 };
            bool m_autoPopupWizard = false; //!< automatically popup wizard if mapping is needed
            bool m_visible = false; //!< is this component selected?
            const int LogoffIntervalSeconds = 20; //!< time before logoff
            QTimer *m_logoffCountdownTimer { nullptr }; //!< timer used logoff countdown
            BlackMisc::CSettingReadOnly<BlackCore::Vatsim::TTrafficServers> m_otherTrafficNetworkServers { this, &CLoginComponent::ps_reloadSettings };
            BlackMisc::CSetting<BlackGui::Settings::TOwnAircraftModel> m_currentAircraftModel { this }; //!< current settings of aircraft
            BlackMisc::CData<BlackCore::Data::TVatsimCurrentServer> m_currentVatsimServer { this }; //!< cache for current VATSIM server
        };
    } // namespace
} // namespace

#endif // guard
