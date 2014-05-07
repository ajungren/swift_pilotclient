/* Copyright (C) 2013 VATSIM Community / authors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BLACKCORE_VATSIMDATAFILEREADER_H
#define BLACKCORE_VATSIMDATAFILEREADER_H

#include "blackmisc/avatcstationlist.h"
#include "blackmisc/avaircraftlist.h"
#include "blackmisc/nwserverlist.h"
#include "blackmisc/nwuserlist.h"
#include "blackmisc/avcallsignlist.h"

#include <QObject>
#include <QTimer>
#include <QNetworkReply>

namespace BlackCore
{
    /*!
     * Read bookings from VATSIM
     */
    class CVatsimDataFileReader : public QObject
    {
        Q_OBJECT

    public:
        //! Constructor
        explicit CVatsimDataFileReader(const QStringList &urls, QObject *parent = nullptr);

        //! Update timestamp
        QDateTime getUpdateTimestamp() const { return this->m_updateTimestamp; }

        //! Read / re-read bookings
        void read();

        /*!
         * \brief Set the update time
         * \param updatePeriodMs 0 stops the timer
         */
        void setInterval(int updatePeriodMs);

        //! Get the timer interval (ms)
        int interval() const { return this->m_updateTimer->interval();}

        //! Get aircrafts
        const BlackMisc::Aviation::CAircraftList &getAircrafts() { return this->m_aircrafts; }

        //! Get aircrafts
        const BlackMisc::Aviation::CAtcStationList &getAtcStations() { return this->m_atcStations; }

        //! Get all voice servers
        const BlackMisc::Network::CServerList &getVoiceServers() { return this->m_voiceServers; }

        //! Users for callsign(s)
        BlackMisc::Network::CUserList getUsersForCallsigns(const BlackMisc::Aviation::CCallsignList &callsigns);

        //! User for callsign
        BlackMisc::Network::CUserList getUsersForCallsign(const BlackMisc::Aviation::CCallsign &callsign);

        //! Controllers for callsigns
        BlackMisc::Network::CUserList getControllersForCallsigns(const BlackMisc::Aviation::CCallsignList &callsigns);

        //! Controllers for callsign
        BlackMisc::Network::CUserList getControllersForCallsign(const BlackMisc::Aviation::CCallsign &callsign);

        //! Users for callsigns
        BlackMisc::Network::CUserList getPilotsForCallsigns(const BlackMisc::Aviation::CCallsignList &callsigns);

        //! Users for callsign
        BlackMisc::Network::CUserList getPilotsForCallsign(const BlackMisc::Aviation::CCallsign &callsign);

    private slots:
        //! Data have been read
        void loadFinished(QNetworkReply *nwReply);

    private:
        QStringList m_serviceUrls; /*!< URL of the service */
        int m_currentUrlIndex;
        QNetworkAccessManager *m_networkManager;
        QDateTime m_updateTimestamp;
        QTimer *m_updateTimer;
        BlackMisc::Network::CServerList m_voiceServers;
        BlackMisc::Aviation::CAtcStationList m_atcStations;
        BlackMisc::Aviation::CAircraftList m_aircrafts;
        static const QMap<QString, QString> clientPartsToMap(const QString &currentLine, const QStringList &clientSectionAttributes);

        //! Section in file
        enum Section
        {
            SectionNone,
            SectionVoiceServer,
            SectionClients,
            SectionGeneral
        };

    signals:
        //! Data have been read
        void dataRead();
    };
}

#endif // guard
