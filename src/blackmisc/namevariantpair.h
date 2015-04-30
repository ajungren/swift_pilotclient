/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_NAMEVARIANTPAIR_H
#define BLACKMISC_NAMEVARIANTPAIR_H

#include "blackmiscexport.h"
#include "variant.h"
#include "valueobject.h"
#include "icon.h"

namespace BlackMisc
{
    //! Value / variant pair
    class BLACKMISC_EXPORT CNameVariantPair : public CValueObject<CNameVariantPair>
    {
    public:
        //! Properties by index
        enum ColumnIndex
        {
            IndexName = BlackMisc::CPropertyIndex::GlobalIndexCNameVariantPair,
            IndexVariant
        };

        //! Default constructor.
        CNameVariantPair() = default;

        //! Constructor.
        CNameVariantPair(const QString &name, const CVariant &variant, const CIcon &icon = CIcon());

        //! Get name.
        const QString &getName() const { return m_name; }

        //! Get variant.
        CVariant getVariant() const { return m_variant; }

        //! Set name.
        void setName(const QString &name) { this->m_name = name; }

        //! Name available?
        bool hasName() const { return !this->m_name.isEmpty(); }

        //! Set variant.
        void setVariant(const CVariant &variant) { m_variant = variant; }

        //! \copydoc CValueObject::toIcon()
        virtual BlackMisc::CIcon toIcon() const override;

        //! Has icon
        bool hasIcon() const;

        //! \copydoc CValueObject::propertyByIndex
        virtual CVariant propertyByIndex(const BlackMisc::CPropertyIndex &index) const override;

        //! \copydoc CValueObject::setPropertyByIndex
        virtual void setPropertyByIndex(const CVariant &variant, const BlackMisc::CPropertyIndex &index) override;

        //! \copydoc CValueObject::convertToQString()
        virtual QString convertToQString(bool i18n = false) const override;

    private:
        BLACK_ENABLE_TUPLE_CONVERSION(CNameVariantPair)
        QString  m_name;
        CVariant m_variant;
        CIcon    m_icon; //!< optional icon
    };
} // namespace

BLACK_DECLARE_TUPLE_CONVERSION(BlackMisc::CNameVariantPair, (o.m_name, o.m_variant, o.m_icon))
Q_DECLARE_METATYPE(BlackMisc::CNameVariantPair)

#endif // guard
