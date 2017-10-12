/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_AVIATION_AIRPORTICAOCODE_H
#define BLACKMISC_AVIATION_AIRPORTICAOCODE_H

#include "blackmisc/blackmiscexport.h"
#include "blackmisc/metaclass.h"
#include "blackmisc/propertyindex.h"
#include "blackmisc/valueobject.h"

#include <QMetaType>
#include <QString>
#include <tuple>

namespace BlackMisc
{
    namespace Aviation
    {
        //! Value object encapsulating information of airport ICAO data.
        class BLACKMISC_EXPORT CAirportIcaoCode : public CValueObject<CAirportIcaoCode>
        {
        public:
            //! Default constructor.
            CAirportIcaoCode() {}

            //! Constructor
            CAirportIcaoCode(const QString &icaoCode) : m_icaoCode(CAirportIcaoCode::unifyAirportCode(icaoCode)) {}

            //! Constructor, needed to disambiguate implicit conversion from string literal.
            CAirportIcaoCode(const char *icaoCode) : m_icaoCode(CAirportIcaoCode::unifyAirportCode(icaoCode)) {}

            //! Is empty?
            bool isEmpty() const { return this->m_icaoCode.isEmpty(); }

            //! Get code.
            const QString &asString() const { return this->m_icaoCode; }

            //! Get ICAO code
            QString getIcaoCode() const { return m_icaoCode; }

            //! Equals callsign string?
            bool equalsString(const QString &icaoCode) const;

            //! Unify code
            static QString unifyAirportCode(const QString &icaoCode);

            //! Valid ICAO designator
            static bool isValidIcaoDesignator(const QString &icaoCode);

            //! Containing numbers (normally indicator for small airfield/strip)
            static bool containsNumbers(const QString &icaoCode);

            //! \copydoc BlackMisc::Mixin::String::toQString()
            QString convertToQString(bool i18n = false) const;

            //! Compare for index
            int comparePropertyByIndex(const CPropertyIndex &index, const CAirportIcaoCode &compareValue) const;

        private:
            QString m_icaoCode;

            BLACK_METACLASS(
                CAirportIcaoCode,
                BLACK_METAMEMBER(icaoCode, 0, CaseInsensitiveComparison)
            );
        };
    } // namespace
} // namespace

Q_DECLARE_METATYPE(BlackMisc::Aviation::CAirportIcaoCode)

#endif // guard
