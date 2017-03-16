/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_SIMULATION_FSX_SIMCONNECTUTILITIES_H
#define BLACKMISC_SIMULATION_FSX_SIMCONNECTUTILITIES_H

#include "blackmisc/aviation/aircraftlights.h"
#include "blackmisc/weather/gridpoint.h"
#include "blackmisc/blackmiscexport.h"
#include "blackmisc/windllutils.h"

#include <QMetaType>
#include <QObject>
#include <QString>
#include <QSettings>
#include <QSharedPointer>

// Apart from the below definitions, the following code is OS independent,
// though it does not make sense to be used on non WIN machines.
// But it allows such parts to compile on all platforms.
#ifdef Q_OS_WIN
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
#   include <windows.h>
#else
using DWORD = unsigned long; //!< Fake Windows DWORD
#endif

namespace BlackMisc
{
    namespace Simulation
    {
        namespace Fsx
        {
            //! Utilities for SimConnect
            class BLACKMISC_EXPORT CSimConnectUtilities : public QObject
            {
                Q_OBJECT
                Q_ENUMS(SIMCONNECT_EXCEPTION)
                Q_ENUMS(SIMCONNECT_SURFACE)

            public:
                //! Filename of the file
                static const QString &simConnectFilename();

                //! Path to local config file
                static QString getLocalSimConnectCfgFilename();

                //! Path to local config file
                static bool hasLocalSimConnectCfgFilename();

                //! The simconnect.cfg as settings (or nullptr settings if no such file)
                static QSharedPointer<QSettings> simConnectFileAsSettings(const QString &fileName = getLocalSimConnectCfgFilename());

                //! IP address from settings (of simconnect.cfg), "" if not available
                static QString ipAddress(const QSettings *simConnectSettings);

                //! IP port from settings (of simconnect.cfg), -1 if not available
                static int ipPort(const QSettings *simConnectSettings);

                //! Content for FSX simconnect.cfg file
                //! \param ip IP address of FSX
                //! \param port Port of FSX (e.g. 500)
                //! \return content for simconnect.cfg
                static QString simConnectCfg(const QString &ip, int port = 500);

                //! Create a FSX simconnect.cfg file
                //! \param fileName and path
                //! \param ip IP address of FSX
                //! \param port Port of FSX (e.g. 500)
                //! \return success
                static bool writeSimConnectCfg(const QString &fileName, const QString &ip, int port = 500);

                //! Resolve SimConnect exception (based on Qt metadata).
                //! \param id enum element
                //! \return enum element's name
                static const QString simConnectExceptionToString(const DWORD id);

                //! Resolve SimConnect surface (based on Qt metadata).
                //! \param type enum element
                //! \param beautify remove "_"
                //! \return
                static const QString simConnectSurfaceTypeToString(const DWORD type, bool beautify = true);

                //! SimConnect surfaces.
                //! \sa http://msdn.microsoft.com/en-us/library/cc526981.aspx#AircraftFlightInstrumentationData
                enum SIMCONNECT_SURFACE
                {
                    Concrete,
                    Grass,
                    Water,
                    Grass_bumpy,
                    Asphalt,
                    Short_grass,
                    Long_grass,
                    Hard_turf,
                    Snow,
                    Ice,
                    Urban,
                    Forest,
                    Dirt,
                    Coral,
                    Gravel,
                    Oil_treated,
                    Steel_mats,
                    Bituminus,
                    Brick,
                    Macadam,
                    Planks,
                    Sand,
                    Shale,
                    Tarmac,
                    Wright_flyer_track
                };

                //! SimConnect exceptions.
                enum SIMCONNECT_EXCEPTION
                {
                    SIMCONNECT_EXCEPTION_NONE,
                    SIMCONNECT_EXCEPTION_ERROR,
                    SIMCONNECT_EXCEPTION_SIZE_MISMATCH,
                    SIMCONNECT_EXCEPTION_UNRECOGNIZED_ID,
                    SIMCONNECT_EXCEPTION_UNOPENED,
                    SIMCONNECT_EXCEPTION_VERSION_MISMATCH,
                    SIMCONNECT_EXCEPTION_TOO_MANY_GROUPS,
                    SIMCONNECT_EXCEPTION_NAME_UNRECOGNIZED,
                    SIMCONNECT_EXCEPTION_TOO_MANY_EVENT_NAMES,
                    SIMCONNECT_EXCEPTION_EVENT_ID_DUPLICATE,
                    SIMCONNECT_EXCEPTION_TOO_MANY_MAPS,
                    SIMCONNECT_EXCEPTION_TOO_MANY_OBJECTS,
                    SIMCONNECT_EXCEPTION_TOO_MANY_REQUESTS,
                    SIMCONNECT_EXCEPTION_WEATHER_INVALID_PORT,
                    SIMCONNECT_EXCEPTION_WEATHER_INVALID_METAR,
                    SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_GET_OBSERVATION,
                    SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_CREATE_STATION,
                    SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_REMOVE_STATION,
                    SIMCONNECT_EXCEPTION_INVALID_DATA_TYPE,
                    SIMCONNECT_EXCEPTION_INVALID_DATA_SIZE,
                    SIMCONNECT_EXCEPTION_DATA_ERROR,
                    SIMCONNECT_EXCEPTION_INVALID_ARRAY,
                    SIMCONNECT_EXCEPTION_CREATE_OBJECT_FAILED,
                    SIMCONNECT_EXCEPTION_LOAD_FLIGHTPLAN_FAILED,
                    SIMCONNECT_EXCEPTION_OPERATION_INVALID_FOR_OBJECT_TYPE,
                    SIMCONNECT_EXCEPTION_ILLEGAL_OPERATION,
                    SIMCONNECT_EXCEPTION_ALREADY_SUBSCRIBED,
                    SIMCONNECT_EXCEPTION_INVALID_ENUM,
                    SIMCONNECT_EXCEPTION_DEFINITION_ERROR,
                    SIMCONNECT_EXCEPTION_DUPLICATE_ID,
                    SIMCONNECT_EXCEPTION_DATUM_ID,
                    SIMCONNECT_EXCEPTION_OUT_OF_BOUNDS,
                    SIMCONNECT_EXCEPTION_ALREADY_CREATED,
                    SIMCONNECT_EXCEPTION_OBJECT_OUTSIDE_REALITY_BUBBLE,
                    SIMCONNECT_EXCEPTION_OBJECT_CONTAINER,
                    SIMCONNECT_EXCEPTION_OBJECT_AI,
                    SIMCONNECT_EXCEPTION_OBJECT_ATC,
                    SIMCONNECT_EXCEPTION_OBJECT_SCHEDULE
                };

                //! Lights for FSX/P3D "LIGHT ON STATES"
                //! \sa http://www.prepar3d.com/SDKv2/LearningCenter/utilities/variables/simulation_variables.html
                enum LIGHT_STATES
                {
                    Nav         = 0x0001,
                    Beacon      = 0x0002,
                    Landing     = 0x0004,
                    Taxi        = 0x0008,
                    Strobe      = 0x0010,
                    Panel       = 0x0020,
                    Recognition = 0x0040,
                    Wing        = 0x0080,
                    Logo        = 0x0100,
                    Cabin       = 0x0200
                };

                //! Lights to states
                static int lightsToLightStates(const BlackMisc::Aviation::CAircraftLights &lights);

                //! Converts the weather at gridPoint to a SimConnect METAR string
                static QString convertToSimConnectMetar(const BlackMisc::Weather::CGridPoint &gridPoint);

                //! Get info about SimConnect DLL
                static BlackMisc::CWinDllUtils::DLLInfo simConnectDllInfo();

                //! Register metadata
                static void registerMetadata();

            private:
                //!
                //! Resolve enum value to its cleartext (based on Qt metadata).
                //! \param id enum element
                //! \param enumName name of the resolved enum
                //! \return enum element's name
                static const QString resolveEnumToString(const DWORD id, const char *enumName);

                static QString windsToSimConnectMetar(const BlackMisc::Weather::CWindLayerList &windLayers);
                static QString visibilitiesToSimConnectMetar(const BlackMisc::Weather::CVisibilityLayerList &visibilityLayers);
                static QString cloudsToSimConnectMetar(const BlackMisc::Weather::CCloudLayerList &cloudLayers);
                static QString temperaturesToSimConnectMetar(const BlackMisc::Weather::CTemperatureLayerList &temperatureLayers);

                //! Hidden constructor
                CSimConnectUtilities();
            };
        } // namespace
    } // namespace
} // namespace

Q_DECLARE_METATYPE(BlackMisc::Simulation::Fsx::CSimConnectUtilities::SIMCONNECT_EXCEPTION)
Q_DECLARE_METATYPE(BlackMisc::Simulation::Fsx::CSimConnectUtilities::SIMCONNECT_SURFACE)

#endif // guard
