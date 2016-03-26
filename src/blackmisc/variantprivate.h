/* Copyright (C) 2014
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_VARIANT_PRIVATE_H
#define BLACKMISC_VARIANT_PRIVATE_H

#include "blackmisc/blackmiscexport.h"
#include "blackmisc/inheritancetraits.h"
#include <QString>
#include <QMetaType>
#include <QDBusMetaType>
#include <QDBusArgument>
#include <QJsonObject>
#include <stdexcept>

namespace BlackMisc
{
    class CEmpty;
    class CVariant;
    class CPropertyIndex;
    class CIcon;

    template <typename T>
    void registerMetaValueType();

    //! Checked version from QVariant
    template <class T> void setFromQVariant(T *value, const QVariant &variant)
    {
        if (variant.canConvert<T>())
        {
            (*value) = variant.value<T>();
        }
        else
        {
            const QString m = QString("Target type: %1 Variant type: %2 %3 %4").
                              arg(qMetaTypeId<T>()).
                              arg(static_cast<int>(variant.type())).arg(variant.userType()).arg(variant.typeName());
            Q_ASSERT_X(false, "setFromQVariant", m.toLocal8Bit().constData());
            Q_UNUSED(m);
        }
    }

    namespace Private
    {
        //! \private Needed so we can copy forward-declared CVariant.
        void assign(CVariant &, const CVariant &);

        //! \private Needed so we can copy forward-declared CIcon.
        void assign(CIcon &, const CIcon &);

        //! \private Abstract base class representing the set of operations supported by a particular value type.
        struct BLACKMISC_EXPORT IValueObjectMetaInfo
        {
            virtual ~IValueObjectMetaInfo() = default;
            virtual QString toQString(const void *object, bool i18n) const = 0;
            virtual QJsonObject toJson(const void *object) const = 0;
            virtual void convertFromJson(const QJsonObject &json, void *object) const = 0;
            virtual void unmarshall(const QDBusArgument &arg, void *object) const = 0;
            virtual uint getValueHash(const void *object) const = 0;
            virtual int getMetaTypeId() const = 0;
            virtual const void *upCastTo(const void *object, int metaTypeId) const = 0;
            virtual int compareImpl(const void *lhs, const void *rhs) const = 0;
            virtual void setPropertyByIndex(void *object, const CVariant &variant, const CPropertyIndex &index) const = 0;
            virtual void propertyByIndex(const void *object, CVariant &o_variant, const BlackMisc::CPropertyIndex &index) const = 0;
            virtual QString propertyByIndexAsString(const void *object, const CPropertyIndex &index, bool i18n) const = 0;
            virtual bool equalsPropertyByIndex(const void *object, const CVariant &compareValue, const CPropertyIndex &index) const = 0;
            virtual void toIcon(const void *object, CIcon &o_icon) const = 0;
        };

        //! \private Exception to signal that an unsupported operation was requested.
        class CVariantException : public std::invalid_argument
        {
        public:
            template <class T>
            CVariantException(const T &, const QString &opName) : CVariantException(qMetaTypeId<T>(), opName) {}

            CVariantException(int typeId, const QString &opName) : std::invalid_argument((blurb(typeId, opName)).toStdString()), m_operationName(opName) {}

            const QString &operationName() const { return m_operationName; }

            ~CVariantException() {}

        private:
            QString m_operationName;

            static QString blurb(int typeId, const QString &operationName)
            {
                return QString("CVariant requested unsupported operation of contained ") + QMetaType::typeName(typeId) + " object: " + operationName;
            }
        };

        //! \private Helper using expression SFINAE to select the correct method implementations.
        struct CValueObjectMetaInfoHelper
        {
            template <typename T>
            static QJsonObject toJson(const T &object, decltype(static_cast<void>(object.toJson()), 0)) { return object.toJson(); }
            template <typename T>
            static QJsonObject toJson(const T &object, ...) { throw CVariantException(object, "toJson"); }

            template <typename T>
            static void convertFromJson(const QJsonObject &json, T &object, decltype(static_cast<void>(object.convertFromJson(json)), 0)) { object.convertFromJson(json); }
            template <typename T>
            static void convertFromJson(const QJsonObject &, T &object, ...) { throw CVariantException(object, "convertFromJson"); }

            template <typename T>
            static uint getValueHash(const T &object, decltype(static_cast<void>(qHash(object)), 0)) { return qHash(object); }
            template <typename T>
            static uint getValueHash(const T &object, ...) { throw CVariantException(object, "getValueHash"); }

            template <typename T>
            static int compareImpl(const T &lhs, const T &rhs, decltype(static_cast<void>(compare(lhs, rhs)), 0)) { return compare(lhs, rhs); }
            template <typename T>
            static int compareImpl(const T &lhs, const T&, ...) { throw CVariantException(lhs, "compare"); }

            template <typename T>
            static void setPropertyByIndex(T &object, const CVariant &variant, const CPropertyIndex &index, decltype(static_cast<void>(object.setPropertyByIndex(variant, index)), 0)) { object.setPropertyByIndex(variant, index); }
            template <typename T>
            static void setPropertyByIndex(T &object, const CVariant &, const CPropertyIndex &, ...) { throw CVariantException(object, "setPropertyByIndex"); }

            template <typename T>
            static void propertyByIndex(CVariant &o_variant, const T &object, const CPropertyIndex &index, decltype(static_cast<void>(object.propertyByIndex(index)), 0)) { assign(o_variant, object.propertyByIndex(index)); }
            template <typename T>
            static void propertyByIndex(CVariant &, const T &object, const CPropertyIndex &, ...) { throw CVariantException(object, "propertyByIndex"); }

            template <typename T>
            static QString propertyByIndexAsString(const T &object, const CPropertyIndex &index, bool i18n, decltype(static_cast<void>(object.propertyByIndexAsString(index, i18n)), 0)) { return object.propertyByIndexAsString(index, i18n); }
            template <typename T>
            static QString propertyByIndexAsString(const T &object, const CPropertyIndex &, bool, ...) { throw CVariantException(object, "propertyByIndexAsString"); }

            template <typename T>
            static bool equalsPropertyByIndex(const T &object, const CVariant &variant, const CPropertyIndex &index, decltype(static_cast<void>(object.equalsPropertyByIndex(variant, index)), 0)) { return object.equalsPropertyByIndex(variant, index); }
            template <typename T>
            static bool equalsPropertyByIndex(const T &object, const CVariant &, const CPropertyIndex &, ...) { throw CVariantException(object, "equalsPropertyByIndex"); }

            template <typename T>
            static void toIcon(const T &object, CIcon &o_icon, std::enable_if_t<! std::is_same<T, CVariant>::value, decltype(static_cast<void>(object.toIcon()), 0)>) { assign(o_icon, object.toIcon()); }
            template <typename T>
            static void toIcon(const T &object, CIcon &, ...) { throw CVariantException(object, "toIcon"); }
        };

        //! \private Implementation of IValueObjectMetaInfo representing the set of operations supported by T.
        template <typename T>
        struct CValueObjectMetaInfo : public IValueObjectMetaInfo
        {
            virtual QString toQString(const void *object, bool i18n) const override
            {
                return cast(object).toQString(i18n);
            }
            virtual QJsonObject toJson(const void *object) const override
            {
                return CValueObjectMetaInfoHelper::toJson(cast(object), 0);
            }
            virtual void convertFromJson(const QJsonObject &json, void *object) const override
            {
                CValueObjectMetaInfoHelper::convertFromJson(json, cast(object), 0);
            }
            virtual void unmarshall(const QDBusArgument &arg, void *object) const override
            {
                arg >> cast(object);
            }
            virtual uint getValueHash(const void *object) const override
            {
                return CValueObjectMetaInfoHelper::getValueHash(cast(object), 0);
            }
            virtual int getMetaTypeId() const override
            {
                return maybeGetMetaTypeId(std::integral_constant<bool, QMetaTypeId<T>::Defined> {});
            }
            virtual const void *upCastTo(const void *object, int metaTypeId) const override
            {
                const auto base = static_cast<const void *>(static_cast<const MetaBaseOfT<T> *>(&cast(object)));
                return metaTypeId == getMetaTypeId() ? object : CValueObjectMetaInfo<MetaBaseOfT<T>> {} .upCastTo(base, metaTypeId);
            }
            virtual int compareImpl(const void *lhs, const void *rhs) const override
            {
                return CValueObjectMetaInfoHelper::compareImpl(cast(lhs), cast(rhs), 0);
            }
            virtual void setPropertyByIndex(void *object, const CVariant &variant, const CPropertyIndex &index) const override
            {
                CValueObjectMetaInfoHelper::setPropertyByIndex(cast(object), variant, index, 0);
            }
            virtual void propertyByIndex(const void *object, CVariant &o_variant, const BlackMisc::CPropertyIndex &index) const override
            {
                CValueObjectMetaInfoHelper::propertyByIndex(o_variant, cast(object), index, 0);
            }
            virtual QString propertyByIndexAsString(const void *object, const CPropertyIndex &index, bool i18n) const override
            {
                return CValueObjectMetaInfoHelper::propertyByIndexAsString(cast(object), index, i18n, 0);
            }
            virtual bool equalsPropertyByIndex(const void *object, const CVariant &compareValue, const CPropertyIndex &index) const override
            {
                return CValueObjectMetaInfoHelper::equalsPropertyByIndex(cast(object), compareValue, index, 0);
            }
            virtual void toIcon(const void *object, CIcon &o_icon) const override
            {
                CValueObjectMetaInfoHelper::toIcon(cast(object), o_icon, 0);
            }

            static const T &cast(const void *object) { return *static_cast<const T *>(object); }
            static T &cast(void *object) { return *static_cast<T *>(object); }

            static int maybeGetMetaTypeId(std::true_type) { return qMetaTypeId<T>(); }
            static int maybeGetMetaTypeId(std::false_type) { return QMetaType::UnknownType; }
        };

        //! \private Explicit specialization for the terminating case of the recursive CValueObjectMetaInfo::upCastTo.
        template <>
        struct CValueObjectMetaInfo<void>
        {
            const void *upCastTo(const void *, int) const
            {
                return nullptr;
            }
        };

        //! \private Getter to obtain the IValueObjectMetaInfo which was stored by BlackMisc::registerMetaValueType.
        IValueObjectMetaInfo *getValueObjectMetaInfo(int typeId);

        //! \private Getter to obtain the IValueObjectMetaInfo which was stored by BlackMisc::registerMetaValueType.
        IValueObjectMetaInfo *getValueObjectMetaInfo(const QVariant &);

        //! \private Getter to obtain the IValueObjectMetaInfo which was stored by BlackMisc::registerMetaValueType.
        template <typename T>
        IValueObjectMetaInfo *getValueObjectMetaInfo() { return getValueObjectMetaInfo(qMetaTypeId<T>()); }

        //! \cond PRIVATE
        template <typename T, bool IsRegisteredMetaType /* = true */>
        struct MetaTypeHelperImpl
        {
            static constexpr int maybeGetMetaTypeId() { return qMetaTypeId<T>(); }
            static void maybeRegisterMetaType() { qRegisterMetaType<T>(); qDBusRegisterMetaType<T>(); registerMetaValueType<T>(); }
            static QVariant maybeToQVariant(const T &obj) { return QVariant::fromValue(obj); }
            static void maybeConvertFromQVariant(T &obj, const QVariant &var) { BlackMisc::setFromQVariant(&obj, var); }
        };

        template <typename T>
        struct MetaTypeHelperImpl<T, /* IsRegisteredMetaType = */ false>
        {
            static constexpr int maybeGetMetaTypeId() { return QMetaType::UnknownType; }
            static void maybeRegisterMetaType() {}
            static QVariant maybeToQVariant(const T &) { return {}; }
            static void maybeConvertFromQVariant(T &, const QVariant &) {}
        };

        template <typename T>
        using MetaTypeHelper = MetaTypeHelperImpl<T, QMetaTypeId<T>::Defined>;
        //! \endcond
    }
}

Q_DECLARE_METATYPE(BlackMisc::Private::IValueObjectMetaInfo *)

#endif
