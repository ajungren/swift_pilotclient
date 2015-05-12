/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

#include "blackmisc/aviation/transponder.h"
#include "blackmisc/propertyindex.h"
#include "blackmisc/blackmiscfreefunctions.h"
#include "blackmisc/variant.h"

namespace BlackMisc
{
    namespace Aviation
    {

        bool CTransponder::validValues() const
        {
            if (this->isDefaultValue()) return true; // special case
            return CTransponder::isValidTransponderCode(this->m_transponderCode);
        }

        bool CTransponder::isInNormalSendingMode() const
        {
            switch (this->m_transponderMode)
            {
            case ModeA:
            case ModeC:
            case ModeMil1:
            case ModeMil2:
            case ModeMil3:
            case ModeMil4:
            case ModeMil5:
                return true;
            case StateIdent:
            case StateStandby:
            default:
                return false;
            }
        }

        QString CTransponder::convertToQString(bool /* i18n */) const
        {
            return this->getTransponderCodeFormatted().append(" ").append(this->getModeAsString());
        }

        CTransponder::TransponderMode CTransponder::modeFromString(const QString &modeString)
        {
            if (modeString.startsWith("I", Qt::CaseInsensitive)) return StateIdent;
            if (modeString.startsWith("S", Qt::CaseInsensitive)) return StateStandby;
            if (modeString.startsWith("Mode C", Qt::CaseInsensitive)) return ModeC;
            if (modeString.startsWith("C", Qt::CaseInsensitive)) return ModeC;
            if (modeString.startsWith("Mode S", Qt::CaseInsensitive)) return ModeS;
            if (modeString.contains("Mode 1", Qt::CaseInsensitive)) return ModeMil1;
            if (modeString.contains("Mode 2", Qt::CaseInsensitive)) return ModeMil2;
            if (modeString.contains("Mode 3", Qt::CaseInsensitive)) return ModeMil3;
            if (modeString.contains("Mode 4", Qt::CaseInsensitive)) return ModeMil4;
            if (modeString.contains("Mode 5", Qt::CaseInsensitive)) return ModeMil5;
            return StateStandby;
        }

        QString CTransponder::getTransponderCodeFormatted() const
        {
            QString f("0000");
            f = f.append(QString::number(this->m_transponderCode));
            return f.right(4);
        }

        QString CTransponder::getTransponderCodeAndModeFormatted() const
        {
            QString s = this->getTransponderCodeFormatted();
            s.append(' ').append(this->getModeAsString());
            return s;
        }

        void CTransponder::setTransponderCode(const QString &transponderCode)
        {
            if (CTransponder::isValidTransponderCode(transponderCode))
            {
                bool ok;
                this->setTransponderCode(transponderCode.toInt(&ok));
            }
            else
            {
                Q_ASSERT_X(false, "CTransponder::setTransponderCode", "illegal transponder value");
            }
        }

        bool CTransponder::isValidTransponderCode(const QString &transponderCode)
        {
            if (transponderCode.isEmpty() || transponderCode.length() > 4) return false;
            bool number;
            qint32 tc = transponderCode.toInt(&number);
            if (!number) return false;
            if (tc < 0 || tc > 7777) return false;
            QRegExp rx("[0-7]{1,4}");
            return rx.exactMatch(transponderCode);
        }

        bool CTransponder::isValidTransponderCode(qint32 transponderCode)
        {
            if (transponderCode < 0 || transponderCode > 7777) return false;
            return CTransponder::isValidTransponderCode(QString::number(transponderCode));
        }

        const QString &CTransponder::modeAsString(CTransponder::TransponderMode mode)
        {
            static QString m;
            switch (mode)
            {
            case StateIdent:
                m = "Ident";
                break;
            case StateStandby:
                m = "Standby";
                break;
            case ModeC:
                m = "Mode C";
                break;
            case ModeS:
                m = "Mode S";
                break;
            case ModeMil1:
                m = "Mil.Mode 1";
                break;
            case ModeMil2:
                m = "Mil.Mode 2";
                break;
            case ModeMil3:
                m = "Mil.Mode 3";
                break;
            case ModeMil4:
                m = "Mil.Mode 4";
                break;
            case ModeMil5:
                m = "Mil.Mode 5";
                break;
            default:
                qFatal("Illegal Transponder Mode");
            }
            return m;
        }

        CVariant CTransponder::propertyByIndex(const CPropertyIndex &index) const
        {
            if (index.isMyself()) { return this->toCVariant(); }
            ColumnIndex i = index.frontCasted<ColumnIndex>();
            switch (i)
            {
            case IndexMode:
                return CVariant::from(static_cast<int>(this->getTransponderMode()));
            case IndexModeAsString:
                return CVariant::from(this->getModeAsString());
            case IndexTransponderCode:
                return CVariant::from(this->getTransponderCode());
            case IndexTransponderCodeFormatted:
                return CVariant::from(this->getTransponderCodeFormatted());
            case IndexTransponderCodeAndModeFormatted:
                return CVariant::from(this->getTransponderCodeAndModeFormatted());
            default:
                break;
            }

            Q_ASSERT_X(false, "CTransponder", "index unknown");
            QString m = QString("no property, index ").append(index.toQString());
            return CVariant::fromValue(m);
        }

        void CTransponder::setPropertyByIndex(const CVariant &variant, const CPropertyIndex &index)
        {
            if (index.isMyself())
            {
                this->convertFromCVariant(variant);
                return;
            }
            ColumnIndex i = index.frontCasted<ColumnIndex>();
            switch (i)
            {
            case IndexMode:
                this->setTransponderMode(static_cast<TransponderMode>(variant.toInt()));
                break;
            case IndexModeAsString:
                this->setTransponderMode(modeFromString(variant.toQString()));
                break;
            case IndexTransponderCode:
            case IndexTransponderCodeFormatted:
                if (variant.canConvert<int>())
                {
                    this->setTransponderCode(variant.toInt());
                }
                else
                {
                    this->setTransponderCode(variant.toQString());
                }
                break;
            case IndexTransponderCodeAndModeFormatted:
            default:
                Q_ASSERT_X(false, "CTransponder", "index unknown");
                break;
            }
        }

    } // namespace
} // namespace
