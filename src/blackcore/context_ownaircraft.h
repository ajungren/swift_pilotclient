/* Copyright (C) 2013
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKCORE_CONTEXTOWNAIRCRAFT_H
#define BLACKCORE_CONTEXTOWNAIRCRAFT_H

#include "blackcore/context.h"
#include "blackmisc/avaircraft.h"
#include "blackmisc/avatcstation.h"
#include "blackmisc/voiceroomlist.h"
#include "blackmisc/avselcal.h"

//! \addtogroup dbus
//! @{

//! DBus interface for context
#define BLACKCORE_CONTEXTOWNAIRCRAFT_INTERFACENAME "net.vatsim.PilotClient.BlackCore.ContextOwnAircraft"

//! DBus object path for context
#define BLACKCORE_CONTEXTOWNAIRCRAFT_OBJECTPATH "/OwnAircraft"

//! @}

namespace BlackCore
{

    //! \brief Own context proxy
    //! \ingroup dbus
    class IContextOwnAircraft : public CContext
    {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", BLACKCORE_CONTEXTOWNAIRCRAFT_INTERFACENAME)

    public:
        //! DBus interface name
        static const QString &InterfaceName()
        {
            static QString s(BLACKCORE_CONTEXTOWNAIRCRAFT_INTERFACENAME);
            return s;
        }

        //! DBus object path
        static const QString &ObjectPath()
        {
            static QString s(BLACKCORE_CONTEXTOWNAIRCRAFT_OBJECTPATH);
            return s;
        }

        //! \copydoc CContext::getPathAndContextId()
        virtual QString getPathAndContextId() const { return this->buildPathAndContextId(ObjectPath()); }

        //! Factory method
        static IContextOwnAircraft *create(CRuntime *parent, CRuntimeConfig::ContextMode mode, CDBusServer *server, QDBusConnection &conn);

        //! Destructor
        virtual ~IContextOwnAircraft() {}

    signals:
        //! Aircraft changed
        //! \remarks local only
        void changedAircraft(const BlackMisc::Aviation::CAircraft &aircraft, const QString &originator);

        //! Aircraft situation update
        //! \remarks local only
        void changedAircraftSituation(const BlackMisc::Aviation::CAircraft &aircraft, const QString &originator);

        //! Aircraft position update
        //! \remarks local only
        void changedAircraftPosition(const BlackMisc::Aviation::CAircraft &aircraft, const QString &originator);

        //! Aircraft cockpit update
        //! \remarks DBus and local
        void changedAircraftCockpit(const BlackMisc::Aviation::CAircraft &aircraft, const QString &originator);

        //! Changed SELCAL code
        //! \remarks DBus and local
        void changedSelcal(const BlackMisc::Aviation::CSelcal &selcal, const QString &originator);

    public slots:

        //! Get own aircraft
        virtual BlackMisc::Aviation::CAircraft getOwnAircraft() const = 0;

        //! Own aircraft
        //! \todo remove "own", left over from past
        virtual void updateOwnAircraft(const BlackMisc::Aviation::CAircraft &aircraft, const QString &originator) = 0;

        //! Own position, be aware height is terrain height
        //! \todo remove "own", left over from past
        virtual bool updateOwnPosition(const BlackMisc::Geo::CCoordinateGeodetic &position, const BlackMisc::Aviation::CAltitude &altitude, const QString &originator) = 0;

        //! Complete situation update
        //! \todo remove "own", left over from past
        virtual bool updateOwnSituation(const BlackMisc::Aviation::CAircraftSituation &situation, const QString &originator) = 0;

        //! Update own cockpit
        //! \todo remove "own", left over from past
        virtual bool updateOwnCockpit(const BlackMisc::Aviation::CComSystem &com1, const BlackMisc::Aviation::CComSystem &com2, const BlackMisc::Aviation::CTransponder &transponder, const QString &originator) = 0;

        //! Set current pilot
        virtual bool updatePilot(const BlackMisc::Network::CUser &pilot, const QString &originator) = 0;

        //! Set ICAO data
        virtual bool updateIcaoData(const BlackMisc::Aviation::CAircraftIcao &icaoData, const QString &originator) = 0;

        //! Set callsign
        virtual bool updateCallsign(const BlackMisc::Aviation::CCallsign &callsign, const QString &originator) = 0;

        //! Own SELCAL code
        virtual bool updateSelcal(const BlackMisc::Aviation::CSelcal &selcal, const QString &originator) = 0;

        //! Output volumes,  volumes 0..100
        virtual void setAudioOutputVolumes(int outputVolumeCom1, int outputVolumeCom2) = 0;

        //! Set individual voice rooms (overrides voice rooms)
        //! \remarks Empty string "" disables voice room override
        //! \sa enableAutomaticVoiceRoomResolution
        virtual void setAudioVoiceRoomOverrideUrls(const QString &voiceRoom1Url, const QString &voiceRoom2Url) = 0;

        //! Automatic voice room resolution for frequencies
        virtual void enableAutomaticVoiceRoomResolution(bool enable) = 0;

        //! Parse command line
        virtual bool parseCommandLine(const QString &commandLine) = 0;

    protected:
        //! Constructor
        IContextOwnAircraft(CRuntimeConfig::ContextMode mode, CRuntime *runtime) : CContext(mode, runtime) {}
    };
}

#endif // guard
