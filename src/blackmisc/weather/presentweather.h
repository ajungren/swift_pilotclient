/* Copyright (C) 2015
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_WEATHER_PRESENTWEATHER_H
#define BLACKMISC_WEATHER_PRESENTWEATHER_H

#include "blackmisc/blackmiscexport.h"
#include "blackmisc/valueobject.h"
#include "blackmisc/propertyindex.h"
#include "blackmisc/blackmiscfreefunctions.h"
#include <type_traits>

namespace BlackMisc
{
    namespace Weather
    {
        /*!
         * Value object for a cloud layer
         */
        class BLACKMISC_EXPORT CPresentWeather : public CValueObject<CPresentWeather>
        {
        public:
            //! Intensity
            enum Intensity
            {
                Light,
                Moderate,
                Heavy,
                InVincinity
            };

            //! Descriptor
            enum Descriptor
            {
                None,
                Shallow,
                Patches,
                Partial,
                Drifting,
                Blowing,
                Showers,
                Thunderstorm,
                Freezing,
            };

            //! Weather Phenomenon
            enum WeatherPhenomenon
            {
                NoPhenomena         = 0,
                Drizzle             = 1 << 0,
                Rain                = 1 << 1,
                Snow                = 1 << 2,
                SnowGrains          = 1 << 3,
                IceCrystals         = 1 << 4,
                IcePellets          = 1 << 5,
                Hail                = 1 << 6,
                SnowPellets         = 1 << 7,
                Unknown             = 1 << 8,
                Mist                = 1 << 9,
                Fog                 = 1 << 10,
                Smoke               = 1 << 11,
                VolcanicAsh         = 1 << 12,
                Dust                = 1 << 13,
                Sand                = 1 << 14,
                Haze                = 1 << 15,
                DustSandWhirls      = 1 << 16,
                Squalls             = 1 << 17,
                TornadoOrWaterspout = 1 << 18,
                FunnelCloud         = 1 << 19,
                Sandstorm           = 1 << 20,
                Duststorm           = 1 << 21,
            };

            //! Properties by index
            enum ColumnIndex
            {
                IndexPresentWeather = BlackMisc::CPropertyIndex::GlobalIndexCPresentWeather,
                IndexIntensity,
                IndexDescriptor,
                IndexWeatherPhenomena
            };

            //! \copydoc BlackMisc::CValueObject::registerMetadata
            static void registerMetadata();

            //! Default constructor.
            CPresentWeather() = default;

            //! Constructor
            CPresentWeather(Intensity intensity, Descriptor descriptor, int weatherPhenomena);

            //! Set intensity
            void setIntensity(Intensity intensity) { m_intensity = intensity; }

            //! Get intensity
            Intensity getIntensity() const { return m_intensity; }

            //! Set descriptor
            void setDescriptor(Descriptor descriptor) { m_descriptor = descriptor; }

            //! Get descriptor
            Descriptor getDescriptor() const { return m_descriptor; }

            //! Set weather phenomena
            void setWeatherPhenomena(int phenomena) { m_weatherPhenomena = phenomena; }

            //! Get weather phenomenas
            int getWeatherPhenomena() const { return m_weatherPhenomena; }

            //! \copydoc BlackMisc::Mixin::Index::propertyByIndex
            CVariant propertyByIndex(const BlackMisc::CPropertyIndex &index) const;

            //! \copydoc BlackMisc::Mixin::Index::setPropertyByIndex
            void setPropertyByIndex(const CVariant &variant, const BlackMisc::CPropertyIndex &index);

            //! \copydoc BlackMisc::Mixin::String::toQString
            QString convertToQString(bool i18n = false) const;

        private:
            BLACK_ENABLE_TUPLE_CONVERSION(CPresentWeather)
            Intensity m_intensity = Moderate;
            Descriptor m_descriptor = None;
            int m_weatherPhenomena;
        };

    } // namespace
} // namespace

Q_DECLARE_METATYPE(BlackMisc::Weather::CPresentWeather)
Q_DECLARE_METATYPE(BlackMisc::Weather::CPresentWeather::Intensity)
Q_DECLARE_METATYPE(BlackMisc::Weather::CPresentWeather::Descriptor)
BLACK_DECLARE_TUPLE_CONVERSION(BlackMisc::Weather::CPresentWeather, (
                                   attr(o.m_intensity),
                                   attr(o.m_descriptor),
                                   attr(o.m_weatherPhenomena)
                               ))

#endif // guard
