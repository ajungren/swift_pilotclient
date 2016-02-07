/* Copyright (C) 2015
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_NETWORK_ROLE_H
#define BLACKMISC_NETWORK_ROLE_H

#include "blackmisc/blackmiscexport.h"
#include "blackmisc/valueobject.h"
#include "blackmisc/datastore.h"
#include <QString>

namespace BlackMisc
{
    namespace Network
    {
        /*!
         * Role
         */
        class BLACKMISC_EXPORT CRole :
            public CValueObject<CRole>,
            public IDatastoreObjectWithIntegerKey
        {
        public:
            //! Properties by index
            enum ColumnIndex
            {
                IndexName = BlackMisc::CPropertyIndex::GlobalIndexCRole,
                IndexDescription
            };

            //! Constructor
            CRole() = default;

            //! Constructor
            CRole(const QString &name, const QString &description);

            //! Name
            const QString &getName() const { return m_name; }

            //! Name
            void setName(const QString &name) { m_name = name.trimmed().toUpper(); }

            //! Description
            const QString &getDescription() const { return m_description; }

            //! Description
            void setDescription(const QString &description) { m_description = description.trimmed(); }

            //! \copydoc BlackMisc::Mixin::String::toQString
            QString convertToQString(bool i18n = false) const;

            //! \copydoc BlackMisc::Mixin::Index::propertyByIndex
            CVariant propertyByIndex(const BlackMisc::CPropertyIndex &index) const;

            //! \copydoc BlackMisc::Mixin::Index::setPropertyByIndex
            void setPropertyByIndex(const CVariant &variant, const BlackMisc::CPropertyIndex &index);

            //! Role from DB JSON
            static CRole fromDatabaseJson(const QJsonObject &json);

        private:
            BLACK_ENABLE_TUPLE_CONVERSION(CRole)
            QString m_name;
            QString m_description;
        };

    } // ns
} // ns

BLACK_DECLARE_TUPLE_CONVERSION(BlackMisc::Network::CRole, (o.m_dbKey, o.m_name, o.m_description))
Q_DECLARE_METATYPE(BlackMisc::Network::CRole)

#endif // guard
