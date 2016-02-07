/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_AVIATION_SELCAL_H
#define BLACKMISC_AVIATION_SELCAL_H

#include "blackmisc/blackmiscexport.h"
#include "blackmisc/pq/frequency.h"
#include "blackmisc/valueobject.h"

namespace BlackMisc
{
    namespace Aviation
    {
        /*!
         * Value object for SELCAL.
         * \see http://en.wikipedia.org/wiki/SELCAL
         * \see http://www.asri.aero/our-services/selcal/ User Guide
         */
        class BLACKMISC_EXPORT CSelcal : public CValueObject<CSelcal>
        {
        public:
            //! Default constructor.
            CSelcal() = default;

            //! Constructor.
            CSelcal(const QString &code) : m_code(code.trimmed().toUpper()) {}

            /*!
             * Constructor.
             * Needed to disambiguate implicit conversion from string literal.
             */
            CSelcal(const char *code) : m_code(QString(code).trimmed().toUpper()) {}

            //! Is valid?
            bool isValid() const { return isValidCode(this->m_code); }

            //! Get SELCAL code
            const QString &getCode() const { return this->m_code; }

            /*!
             * List of 4 frequencies, if list is empty SELCAL code is not valid
             * \return  either 4 frequencies, or empty list
             */
            QList<BlackMisc::PhysicalQuantities::CFrequency> getFrequencies() const;

            //! Equals given string
            bool equalsString(const QString &code) const;

            //! Valid SELCAL characters
            static const QString &validCharacters();

            //! Is given character a valid SELCAL characer?
            static bool isValidCharacter(QChar c);

            //! Valid SELCAL code?
            static bool isValidCode(const QString &code);

            //! Audio frequency for character
            static const BlackMisc::PhysicalQuantities::CFrequency &audioFrequencyEquivalent(QChar c);

            //! All valid code pairs: AB, AC, AD ...
            static const QStringList &codePairs();

            //! \copydoc BlackMisc::Mixin::String::toQString
            QString convertToQString(bool i18n = false) const;

        private:
            BLACK_ENABLE_TUPLE_CONVERSION(CSelcal)
            QString m_code;
            static QList<BlackMisc::PhysicalQuantities::CFrequency> frequencyEquivalents;
            static QStringList allCodePairs;
        };
    } // namespace
} // namespace

BLACK_DECLARE_TUPLE_CONVERSION(BlackMisc::Aviation::CSelcal, (o.m_code))
Q_DECLARE_METATYPE(BlackMisc::Aviation::CSelcal)

#endif // guard
