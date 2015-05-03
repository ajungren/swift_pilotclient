/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_GEO_LATITUDE_H
#define BLACKMISC_GEO_LATITUDE_H

#include "blackmisc/blackmiscexport.h"
#include <QtCore/qmath.h>
#include "blackmisc/geo/earthangle.h"

namespace BlackMisc
{
    namespace Geo
    {

        //! Latitude
        class BLACKMISC_EXPORT CLatitude :
            public CEarthAngle<CLatitude>,
            public Mixin::MetaType<CLatitude>,
            public Mixin::String<CLatitude>,
            public Mixin::DBusOperators<CLatitude>
        {
        public:
            //! Base type
            using base_type = CEarthAngle<CLatitude>;

            using Mixin::MetaType<CLatitude>::registerMetadata;
            using Mixin::MetaType<CLatitude>::getMetaTypeId;
            using Mixin::MetaType<CLatitude>::isA;
            using Mixin::MetaType<CLatitude>::toCVariant;
            using Mixin::MetaType<CLatitude>::toQVariant;
            using Mixin::MetaType<CLatitude>::convertFromCVariant;
            using Mixin::MetaType<CLatitude>::convertFromQVariant;
            using Mixin::String<CLatitude>::toQString;
            using Mixin::String<CLatitude>::toFormattedQString;
            using Mixin::String<CLatitude>::toStdString;
            using Mixin::String<CLatitude>::stringForStreaming;

            //! \copydoc CValueObject::convertToQString
            QString convertToQString(bool i18n = false) const
            {
                QString s(CEarthAngle::convertToQString(i18n));
                if (!this->isZeroEpsilonConsidered())
                {
                    s.append(this->isNegativeWithEpsilonConsidered() ? " S" : " N");
                }
                return s;
            }

            //! Default constructor
            CLatitude() = default;

            //! Constructor
            explicit CLatitude(const BlackMisc::PhysicalQuantities::CAngle &angle) : CEarthAngle(angle) {}

            //! Init by double value
            CLatitude(double value, const BlackMisc::PhysicalQuantities::CAngleUnit &unit) : CEarthAngle(value, unit) {}
        };

    }
}

Q_DECLARE_METATYPE(BlackMisc::Geo::CLatitude)

#endif // guard
