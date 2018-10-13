/* Copyright (C) 2018
* swift project Community / Contributors
*
* This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
* directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
* including this file, may be copied, modified, propagated, or distributed except according to the terms
* contained in the LICENSE file.
*/

#include "blackmisc/crashsettings.h"
#include "blackmisc/stringutils.h"
#include "blackmisc/comparefunctions.h"

#include <QStringBuilder>

namespace BlackMisc
{
    namespace Settings
    {
        CCrashSettings::CCrashSettings() {}

        QString CCrashSettings::convertToQString(bool i18n) const
        {
            Q_UNUSED(i18n);
            return QStringLiteral("{ %1, %2 }").arg(boolToYesNo(this->isEnabled()), boolToYesNo(this->withPrivacyInfo()));
        }

        CVariant CCrashSettings::propertyByIndex(const CPropertyIndex &index) const
        {
            if (index.isMyself()) { return CVariant::from(*this); }
            const ColumnIndex i = index.frontCasted<ColumnIndex>();
            switch (i)
            {
            case IndexEnabled: return CVariant::fromValue(this->isEnabled());
            case IndexPrivateInfo: return CVariant::fromValue(this->withPrivacyInfo());
            default: break;
            }
            return CValueObject::propertyByIndex(index);
        }

        void CCrashSettings::setPropertyByIndex(const CPropertyIndex &index, const CVariant &variant)
        {
            if (index.isMyself()) { (*this) = variant.to<CCrashSettings>(); return; }
            const ColumnIndex i = index.frontCasted<ColumnIndex>();
            switch (i)
            {
            case IndexEnabled: this->setEnabled(variant.toBool()); break;
            case IndexPrivateInfo: this->setPrivacyInfo(variant.toBool()); break;
            default: CValueObject::setPropertyByIndex(index, variant); break;
            }
        }

        int CCrashSettings::comparePropertyByIndex(const CPropertyIndex &index, const CCrashSettings &compareValue) const
        {
            if (index.isMyself()) { return this->convertToQString().compare(compareValue.convertToQString()); }
            const ColumnIndex i = index.frontCasted<ColumnIndex>();
            switch (i)
            {
            case IndexEnabled: return Compare::compare(this->isEnabled(), compareValue.isEnabled());
            case IndexPrivateInfo: return Compare::compare(this->withPrivacyInfo(), compareValue.withPrivacyInfo());
            default: return CValueObject::comparePropertyByIndex(index.copyFrontRemoved(), compareValue);
            }
        }
    } // ns
} // ns
