/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift Project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project:
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_ICONS_H
#define BLACKMISC_ICONS_H

#include "blackmiscexport.h"
#include <QPixmap>
#include <QMap>

namespace BlackMisc
{
    //! Standard icons
    class BLACKMISC_EXPORT CIcons
    {
    public:

        //! Constructor, use class static only
        CIcons() = delete;

        //! Index for each icon, allows to send them via DBus, efficiently store them, etc.
        enum IconIndex
        {
            // !! keep indexes alphabetically sorted and in sync with CIconList
            ApplicationAircraft = 0,
            ApplicationAircraftIcao,
            ApplicationAirlineIcao,
            ApplicationAtc,
            ApplicationAudio,
            ApplicationCockpit,
            ApplicationCountries,
            ApplicationDatabase,
            ApplicationDbStash,
            ApplicationDistributors,
            ApplicationFlightPlan,
            ApplicationLiveries,
            ApplicationLog,
            ApplicationMappings,
            ApplicationModels,
            ApplicationSettings,
            ApplicationSimulator,
            ApplicationTextMessages,
            ApplicationWeather,
            AviationAttitudeIndicator,
            AviationAtis,
            AviationMetar,
            GeoPosition,
            NetworkCapabilityTextOnly,
            NetworkCapabilityUnknown,
            NetworkCapabilityVoice,
            NetworkCapabilityVoiceBackground,
            NetworkCapabilityVoiceReceiveOnly,
            NetworkCapabilityVoiceReceiveOnlyBackground,
            NetworkRoleApproach,
            NetworkRoleC1,
            NetworkRoleC3,
            NetworkRoleCenter,
            NetworkRoleDelivery,
            NetworkRoleFss,
            NetworkRoleGround,
            NetworkRoleI1,
            NetworkRoleI3,
            NetworkRoleMnt,
            NetworkRoleObs,
            NetworkRolePilot,
            NetworkRoleS1,
            NetworkRoleS2,
            NetworkRoleS3,
            NetworkRoleSup,
            NetworkRoleTower,
            NetworkRoleUnknown,
            NetworkVatsimLogo,
            NetworkVatsimLogoWhite,
            NotSet,
            StandardIconAppAircrafts16,
            StandardIconAppAtc16,
            StandardIconAppFlightPlan16,
            StandardIconAppLog16,
            StandardIconAppMappings16,
            StandardIconAppSettings16,
            StandardIconAppSimulator16,
            StandardIconAppTextMessages16,
            StandardIconAppUsers16,
            StandardIconAppWeather16,
            StandardIconArrowMediumEast16,
            StandardIconArrowMediumNorth16,
            StandardIconArrowMediumSouth16,
            StandardIconArrowMediumWest16,
            StandardIconClose16,
            StandardIconColor16,
            StandardIconColorPicker16,
            StandardIconColorSwatch16,
            StandardIconCross16,
            StandardIconCrossCircle16,
            StandardIconCrossSmall16,
            StandardIconCrossWhite16,
            StandardIconDatabase16,
            StandardIconDatabaseAdd16,
            StandardIconDatabaseConnect16,
            StandardIconDatabaseDelete16,
            StandardIconDatabaseEdit16,
            StandardIconDatabaseError16,
            StandardIconDatabaseKey16,
            StandardIconDatabaseTable16,
            StandardIconDelete16,
            StandardIconDockBottom16,
            StandardIconDockTop16,
            StandardIconDragAndDrop16,
            StandardIconEmpty,
            StandardIconEmpty16,
            StandardIconError16,
            StandardIconFloatAll16,
            StandardIconFloatOne16,
            StandardIconGlobe16,
            StandardIconHeadingOne16,
            StandardIconInfo16,
            StandardIconJoystick16,
            StandardIconLockClosed16,
            StandardIconLockOpen16,
            StandardIconMonitorError16,
            StandardIconPaperPlane16,
            StandardIconPaintCan16,
            StandardIconPlugin16,
            StandardIconRadar16,
            StandardIconRadio16,
            StandardIconRefresh16,
            StandardIconResize16,
            StandardIconResizeHorizontal16,
            StandardIconResizeVertical16,
            StandardIconSpeakerNetwork16,
            StandardIconStatusBar16,
            StandardIconTableRelationship16,
            StandardIconTableSheet16,
            StandardIconText16,
            StandardIconTick16,
            StandardIconTickRed16,
            StandardIconTickSmall16,
            StandardIconTickWhite16,
            StandardIconUnknown16,
            StandardIconUser16,
            StandardIconUsers16,
            StandardIconViewMulticolumn16,
            StandardIconViewTile16,
            StandardIconVolumeHigh16,
            StandardIconVolumeLow16,
            StandardIconVolumeMuted16,
            StandardIconWarning16,
            StandardIconWeatherCloudy16,
            StandardIconWrench16,
            Swift16,
            Swift24,
            Swift48,
            SwiftDatabase16,
            SwiftDatabase24,
            SwiftDatabase48,
            SwiftNova16,
            SwiftNova24,
            SwiftNova48,

            // ---------------------------
            // Icon is not from resources, but generated at runtime
            // ---------------------------
            IconIsGenerated,

            // ---------------------------
            // Icon is not from resources, but a file
            // ---------------------------
            IconIsFile
        };

        // -------------------------------------------------------------
        // Standard pixmaps
        // -------------------------------------------------------------

        //! Info
        static const QPixmap &info16();

        //! Warning
        static const QPixmap &warning16();

        //! Error
        static const QPixmap &error16();

        //! Close
        static const QPixmap &close16();

        //! Color
        static const QPixmap &color16();

        //! Color picker
        static const QPixmap &colorPicker16();

        //! Color swatch
        static const QPixmap &colorSwatch16();

        //! Resize
        static const QPixmap &resize16();

        //! Resize horizontally
        static const QPixmap &resizeHorizontal16();

        //! Resize vertically
        static const QPixmap &resizeVertical16();

        //! Refresh
        static const QPixmap &refresh16();

        //! Text
        static const QPixmap &text16();

        //! Globe
        static const QPixmap &globe16();

        //! Heading 1
        static const QPixmap &headingOne16();

        //! User
        static const QPixmap &user16();

        //! Users
        static const QPixmap &users16();

        //! View multicolumn
        static const QPixmap &viewMultiColumn();

        //! View tile
        static const QPixmap &viewTile();

        //! Tick
        static const QPixmap &tick16();

        //! Tick
        static const QPixmap &tickRed16();

        //! Tick
        static const QPixmap &tickWhite16();

        //! Tick
        static const QPixmap &tickSmall16();

        //! Cross
        static const QPixmap &cross16();

        //! Cross
        static const QPixmap &crossWhite16();

        //! Cross
        static const QPixmap &crossCircle16();

        //! Cross
        static const QPixmap &crossSmall16();

        //! Delete
        static const QPixmap &delete16();

        //! Radio
        static const QPixmap &radio16();

        //! Speaker (network)
        static const QPixmap &speakerNetwork16();

        //! Joystick
        static const QPixmap &joystick16();

        //! Lock
        static const QPixmap &lockOpen16();

        //! Lock
        static const QPixmap &lockClosed16();

        //! Paint can
        static const QPixmap &paintCan16();

        //! Plugin
        static const QPixmap &plugin16();

        //! Docking
        static const QPixmap &dockTop16();

        //! Docking
        static const QPixmap &dockBottom16();

        //! Drag and drop
        static const QPixmap &dragAndDrop16();

        //! Float all
        static const QPixmap &floatAll16();

        //! Float all
        static const QPixmap &floatOne16();

        //! Sound volume high
        static const QPixmap &volumeHigh16();

        //! Sound volume low
        static const QPixmap &volumeLow16();

        //! Sound volume muted
        static const QPixmap &volumeMuted16();

        //! Wrench
        static const QPixmap &wrench16();

        //! Radar
        static const QPixmap &radar16();

        //! Radar
        static const QPixmap &tableRelationship16();

        //! Paper plane
        static const QPixmap &paperPlane16();

        //! Table sheet
        static const QPixmap &tableSheet16();

        //! Clouds
        static const QPixmap &weatherCloudy16();

        //! Monitor error
        static const QPixmap &monitorError16();

        //! Status bar
        static const QPixmap &statusBar16();

        //! Unknown
        static const QPixmap &unknown16();

        //! Empty icon
        static const QPixmap &empty();

        //! Empty icon
        static const QPixmap &empty16();

        //! Arrow
        static const QPixmap &arrowMediumNorth16();

        //! Arrow
        static const QPixmap &arrowMediumSouth16();

        //! Arrow
        static const QPixmap &arrowMediumEast16();

        //! Arrow
        static const QPixmap &arrowMediumWest16();

        // --------------------------------------------------
        // -- Database
        // --------------------------------------------------

        //! Database
        static const QPixmap &database16();

        //! Database add
        static const QPixmap &databaseAdd16();

        //! Database connect
        static const QPixmap &databaseConnect16();

        //! Database delete
        static const QPixmap &databaseDelete16();

        //! Database edit
        static const QPixmap &databaseEdit16();

        //! Database error
        static const QPixmap &databaseError16();

        //! Database key
        static const QPixmap &databaseKey16();

        //! Database table
        static const QPixmap &databaseTable16();

        // --------------------------------------------------
        // -- Preloader icons
        // --------------------------------------------------

        //! Preloader
        static const QPixmap &preloader64();

        //! Preloader
        static const QPixmap &preloader32();

        //! Preloader
        static const QPixmap &preloader16();

        // --------------------------------------------------
        // -- swift
        // --------------------------------------------------

        //! swift icon
        static const QPixmap &swift16();

        //! swift icon
        static const QPixmap &swift24();

        //! swift icon
        static const QPixmap &swift48();

        //! swift icon / database
        static const QPixmap &swiftDatabase16();

        //! swift icon / database
        static const QPixmap &swiftDatabase24();

        //! swift icon / database
        static const QPixmap &swiftDatabase48();

        //! swift icon (nova)
        static const QPixmap &swiftNova16();

        //! swift icon (nova)
        static const QPixmap &swiftNova24();

        //! swift icon (nova)
        static const QPixmap &swiftNova48();

        // --------------------------------------------------
        // -- application icons
        // --------------------------------------------------

        //! Application weather
        static const QPixmap &appWeather16();

        //! Application settings
        static const QPixmap &appSettings16();

        //! Application users
        static const QPixmap &appUsers16();

        //! Application flight plan
        static const QPixmap &appFlightPlan16();

        //! Application cockpit
        static const QPixmap &appCockpit16();

        //! Application simulator
        static const QPixmap &appSimulator16();

        //! Application text messages
        static const QPixmap &appTextMessages16();

        //! Application ATC
        static const QPixmap &appAtc16();

        //! Application aircrafts
        static const QPixmap &appAircraft16();

        //! Application mappings
        static const QPixmap &appMappings16();

        //! Application log/status messages
        static const QPixmap &appLog16();

        //! Application audio
        static const QPixmap &appAudio16();

        //! Voice rooms
        static const QPixmap &appVoiceRooms16();

        //! App.database
        static const QPixmap &appDatabase16();

        //! Aircraft ICAO
        static const QPixmap &appAircraftIcao16();

        //! Airline ICAO
        static const QPixmap &appAirlineIcao16();

        //! Liveries
        static const QPixmap &appLiveries16();

        //! Models
        static const QPixmap &appModels16();

        //! Countries
        static const QPixmap &appCountries16();

        //! Distributors
        static const QPixmap &appDistributors16();

        //! Stash
        static const QPixmap &appDbStash16();

        // -------------------------------------------------------------
        // Network and aviation
        // -------------------------------------------------------------

        //! C1
        static const QPixmap &roleC1();

        //! C2
        static const QPixmap &roleC2();

        //! C3
        static const QPixmap &roleC3();

        //! S1
        static const QPixmap &roleS1();

        //! S2
        static const QPixmap &roleS2();

        //! S3
        static const QPixmap &roleS3();

        //! I1
        static const QPixmap &roleI1();

        //! I3
        static const QPixmap &roleI3();

        //! MNT
        static const QPixmap &roleMnt();

        //! OBS
        static const QPixmap &roleObs();

        //! SUP
        static const QPixmap &roleSup();

        //! Pilot
        static const QPixmap &rolePilot();

        //! Approach
        static const QPixmap &roleApproach();

        //! Ground
        static const QPixmap &roleGround();

        //! Delivery
        static const QPixmap &roleDelivery();

        //! FSS (flight service staion)
        static const QPixmap &roleFss();

        //! Tower
        static const QPixmap &roleTower();

        //! Center
        static const QPixmap &roleCenter();

        //! Unknown
        static const QPixmap &roleUnknown();

        //! VATSIM logo
        static const QPixmap &vatsimLogo16();

        //! VATSIM logo
        static const QPixmap &vatsimLogo32();

        //! VATSIM logo
        static const QPixmap &vatsimLogoWhite16();

        //! VATSIM logo
        static const QPixmap &vatsimLogoWhite32();

        //! Full voice capability
        static const QPixmap &capabilityVoice();

        //! Full voice capability
        static const QPixmap &capabilityVoiceBackground();

        //! Voice receive only
        static const QPixmap &capabilityVoiceReceiveOnly();

        //! Voice receive only
        static const QPixmap &capabilityVoiceReceiveOnlyBackground();

        //! Text only
        static const QPixmap &capabilityTextOnly();

        //! Text only
        static const QPixmap &capabilityUnknown();

        //! Attitude indicator
        static const QPixmap &attitudeIndicator16();

        //! METAR
        static const QPixmap &metar();

        //! ATIS
        static const QPixmap &atis();

        //! Geo position
        static const QPixmap &geoPosition16();

        // -------------------------------------------------------------
        // By index
        // -------------------------------------------------------------

        //! Pixmap by given index
        static const QPixmap &pixmapByIndex(IconIndex index);

        //! Pixmap by given index rotated
        static QPixmap pixmapByIndex(IconIndex index, int rotateDegrees);

        // -------------------------------------------------------------
        // By file from swift resource directory
        // -------------------------------------------------------------

        //! Pixmap by given index
        static const QPixmap &pixmapByResourceFileName(const QString &fileName);

        // -------------------------------------------------------------
        // Utility functions
        // -------------------------------------------------------------

        //! Change color of resource
        static QPixmap changeResourceBackgroundColor(const QString &resource, Qt::GlobalColor backgroundColor);

        //! Change color of icon
        static QIcon changeIconBackgroundColor(const QIcon &icon, Qt::GlobalColor backgroundColor, const QSize &targetsize);

        //! Change image background color
        static QImage changeImageBackgroundColor(const QImage &imgSource, Qt::GlobalColor backgroundColor);

        //! Rotate by pixmap
        static QPixmap rotate(int rotateDegrees, const QPixmap &original);

    private:
        //! File cache for the loaded
        static QMap<QString, QPixmap> &getResourceFileCache();
    };
}
#endif // guard
