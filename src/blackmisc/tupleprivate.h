/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

#ifndef BLACKMISC_TUPLE_PRIVATE_H
#define BLACKMISC_TUPLE_PRIVATE_H

#include "integersequence.h"
#include <QtGlobal>
#include <QDBusArgument>
#include <QHash>
#include <QJsonObject>
#include <QDateTime>
#include <QString>
#include <QJsonArray>
#include <QDebug>
#include <tuple>
#include <type_traits>
#include <functional>

namespace BlackMisc
{
    class CEmpty;
    template <class> class TupleConverter;
    template <class Derived, class Base = CEmpty> class CValueObject;

    namespace Private
    {

        // Inhibit doxygen warnings about missing documentation
        //! \cond PRIVATE

        // To allow CValueObject policy classes to use the tuple system
        class EncapsulationBreaker
        {
        protected:
            template <class T>
            static auto toMetaTuple(T &o)
            {
                return TupleConverter<std::decay_t<T>>::toMetaTuple(o);
            }
        };

        // Using SFINAE to help detect missing BLACK_ENABLE_TUPLE_CONVERSION macro in static_assert
        std::false_type hasEnabledTupleConversionHelper(...);
        template <class T>
        typename T::EnabledTupleConversion hasEnabledTupleConversionHelper(T *);
        template <class T>
        struct HasEnabledTupleConversion
        {
            using type = decltype(hasEnabledTupleConversionHelper(static_cast<T *>(nullptr)));
            static constexpr bool value = type::value;
        };

        // Using tag dispatch to select which implementation of compare() to use
        template <class T>
        int compareHelper(const T &a, const T &b, std::true_type isCValueObjectTag)
        {
            Q_UNUSED(isCValueObjectTag);
            return compare(a, b);
        }
        template <class T>
        int compareHelper(const T &a, const T &b, std::false_type isCValueObjectTag)
        {
            Q_UNUSED(isCValueObjectTag);
            if (a < b) { return -1; }
            if (a > b) { return 1; }
            return 0;
        }

        // Create an index_sequence containing indices which match a given predicate.
        template <class P, size_t I, size_t C, bool B = false, size_t I2 = 0xDeadBeef, size_t... Is>
        struct GenSequenceOnPredicate;
        template <class P, size_t I, size_t C, size_t I2, size_t... Is>
        struct GenSequenceOnPredicate<P, I, C, true, I2, Is...>
        {
            static constexpr bool test = P::template test<I>::value;
            using type = typename GenSequenceOnPredicate<P, I + 1, C, test, I, Is..., I2>::type;
        };
        template <class P, size_t I, size_t C, size_t I2, size_t... Is>
        struct GenSequenceOnPredicate<P, I, C, false, I2, Is...>
        {
            static constexpr bool test = P::template test<I>::value;
            using type = typename GenSequenceOnPredicate<P, I + 1, C, test, I, Is...>::type;
        };
        template <class P, size_t C, size_t I2, size_t... Is>
        struct GenSequenceOnPredicate<P, C, C, true, I2, Is...>
        {
            using type = index_sequence<Is..., I2>;
        };
        template <class P, size_t C, size_t I2, size_t... Is>
        struct GenSequenceOnPredicate<P, C, C, false, I2, Is...>
        {
            using type = index_sequence<Is...>;
        };
        template <class P>
        using make_index_sequence_if = typename GenSequenceOnPredicate<P, 0, std::tuple_size<typename P::tuple_type>::value>::type;

        // Predicates used with make_index_sequence_if.
        template <qint64 F, class Tu>
        struct FlagPresent
        {
            using tuple_type = Tu;
            template <size_t I>
            struct test : std::integral_constant<bool, bool(std::tuple_element_t<I, Tu>::flags & F)> {};
        };
        template <qint64 F, class Tu>
        struct FlagMissing
        {
            using tuple_type = Tu;
            template <size_t I>
            struct test : std::integral_constant<bool, ! bool(std::tuple_element_t<I, Tu>::flags & F)> {};
        };

        // Combine make_index_sequence_if with predicates to get the indices of tuple elements with certain flags.
        template <qint64 F, class Tu>
        auto findFlaggedIndices(Tu &&) -> make_index_sequence_if<FlagPresent<F, std::decay_t<Tu>>>
        {
            return {};
        }
        template <qint64 F, class Tu>
        auto skipFlaggedIndices(Tu &&) -> make_index_sequence_if<FlagMissing<F, std::decay_t<Tu>>>
        {
            return {};
        }

        // CRTP base class for Attribute, to select appropriate method of comparison.
        template <class Derived, bool AlwaysEqual, bool CaseInsensitive>
        struct AttributeComparable;
        template <class Derived, bool CaseInsensitive>
        struct AttributeComparable<Derived, true, CaseInsensitive>
        {
            friend int compare(const Derived &, const Derived &) { return 0; }
            friend bool operator ==(const Derived &, const Derived &) { return true; }
            friend bool operator !=(const Derived &, const Derived &) { return false; }
            friend bool operator <(const Derived &, const Derived &) { return false; }
            friend bool operator <=(const Derived &, const Derived &) { return true; }
            friend bool operator >(const Derived &, const Derived &) { return false; }
            friend bool operator >=(const Derived &, const Derived &) { return true; }
        };
        template <class Derived>
        struct AttributeComparable<Derived, false, false>
        {
            template <class T> using isCValueObject = typename std::is_base_of<CEmpty, T>::type; // historical use case, may need tweaking in future
            friend int compare(const Derived &a, const Derived &b) { return compareHelper(a.m_obj, b.m_obj, isCValueObject<Derived>()); }
            friend bool operator ==(const Derived &a, const Derived &b) { return a.m_obj == b.m_obj; }
            friend bool operator !=(const Derived &a, const Derived &b) { return a.m_obj != b.m_obj; }
            friend bool operator <(const Derived &a, const Derived &b) { return a.m_obj < b.m_obj; }
            friend bool operator <=(const Derived &a, const Derived &b) { return a.m_obj <= b.m_obj; }
            friend bool operator >(const Derived &a, const Derived &b) { return a.m_obj > b.m_obj; }
            friend bool operator >=(const Derived &a, const Derived &b) { return a.m_obj >= b.m_obj; }
        };
        template <class Derived>
        struct AttributeComparable<Derived, false, true>
        {
            friend int compare(const Derived &a, const Derived &b) { return a.m_obj.compare(b.m_obj, Qt::CaseInsensitive); }
            friend bool operator ==(const Derived &a, const Derived &b) { return a.m_obj.compare(b.m_obj, Qt::CaseInsensitive) == 0; }
            friend bool operator !=(const Derived &a, const Derived &b) { return a.m_obj.compare(b.m_obj, Qt::CaseInsensitive) != 0; }
            friend bool operator <(const Derived &a, const Derived &b) { return a.m_obj.compare(b.m_obj, Qt::CaseInsensitive) < 0; }
            friend bool operator <=(const Derived &a, const Derived &b) { return a.m_obj.compare(b.m_obj, Qt::CaseInsensitive) <= 0; }
            friend bool operator >(const Derived &a, const Derived &b) { return a.m_obj.compare(b.m_obj, Qt::CaseInsensitive) > 0; }
            friend bool operator >=(const Derived &a, const Derived &b) { return a.m_obj.compare(b.m_obj, Qt::CaseInsensitive) >= 0; }
        };

        // A tuple element with attached metadata.
        template <class T, qint64 Flags = 0>
        struct Attribute : public AttributeComparable<Attribute<T, Flags>, bool(Flags & DisabledForComparison), bool(Flags & CaseInsensitiveComparison)>
        {
            using type = T;
            static constexpr qint64 flags = Flags;

            Attribute(T &obj, QString jsonName = {}) : m_obj(obj), m_jsonName(jsonName) {}
            void extend(QString jsonName) { if (m_jsonName.isEmpty()) m_jsonName = jsonName; }
            T &m_obj;
            QString m_jsonName;
        };

        // Helpers used in tie(), tieMeta(), and elsewhere, which arrange for the correct types to be passed to std::make_tuple.
        // See http://en.cppreference.com/w/cpp/utility/tuple/make_tuple
        //     "For each Ti in Types..., the corresponding type Vi in Vtypes... is std::decay<Ti>::type
        //     unless application of std::decay results in std::reference_wrapper<X>, in which case the deduced type is X&."
        template <class T>
        std::reference_wrapper<T> tieHelper(T &obj)
        {
            return obj;
        }
        template <class T, qint64 Flags>
        std::reference_wrapper<T> tieHelper(Attribute<T, Flags> attr)
        {
            return attr.m_obj;
        }
        template <class T>
        Attribute<T> tieMetaHelper(T &obj)
        {
            return obj;
        }
        template <class T, qint64 Flags>
        Attribute<T, Flags> tieMetaHelper(Attribute<T, Flags> attr)
        {
            return attr;
        }

        // Compile-time assert for functions which require a meta tuple
        template <class Tu>
        void assertMeta(const Tu &) { static_assert(std::is_void<Tu>::value, "Function expected a meta tuple, got a value tuple"); }
        template <class... Ts, qint64... Fs>
        void assertMeta(const std::tuple<Attribute<Ts, Fs>...> &) {}

        // Convert a meta tuple to a value tuple
        template <class Tu, size_t... Is>
        auto stripMeta(Tu &&tu, index_sequence<Is...>)
        {
            return std::make_tuple(tieHelper(std::get<Is>(std::forward<Tu>(tu)))...);
        }

        // Convert a value tuple to a meta tuple with default metadata
        template <class Tu, size_t... Is>
        auto recoverMeta(Tu &&tu, index_sequence<Is...>)
        {
            return std::make_tuple(tieMetaHelper(std::get<Is>(std::forward<Tu>(tu)))...);
        }

        // Fill in any incomplete metadata in a meta tuple, from an appropriate source
        template <class Tu, size_t... Is>
        void extendMeta(Tu &&tu, const QStringList &jsonNames, index_sequence<Is...>)
        {
            [](...){}((std::get<Is>(std::forward<Tu>(tu)).extend(jsonNames[Is]), 0)...);
        }

        // Applying operations to all elements in a tuple, using index_sequence for clean recursion
        class TupleHelper
        {
        public:
            template <class Tu, size_t... Is>
            static int compare_(const Tu &a, const Tu &b, index_sequence<Is...>) // underscore to avoid hiding the name "compare" in other scopes
            {
                return compareImpl(std::make_pair(std::get<Is>(a), std::get<Is>(b))...);
            }

            template <class Tu, size_t... Is>
            static QDBusArgument &marshall(QDBusArgument &arg, const Tu &tu, index_sequence<Is...>)
            {
                marshallImpl(arg, std::get<Is>(tu)...);
                return arg;
            }

            template <class Tu, size_t... Is>
            static const QDBusArgument &unmarshall(const QDBusArgument &arg, Tu &tu, index_sequence<Is...>)
            {
                unmarshallImpl(arg, std::get<Is>(tu)...);
                return arg;
            }

            template <class Tu, size_t... Is>
            static QDebug debug(QDebug dbg, const Tu &tu, index_sequence<Is...>)
            {
                debugImpl(dbg, std::get<Is>(tu)...);
                return dbg;
            }

            template <class Tu, size_t... Is>
            static uint hash(const Tu &tu, index_sequence<Is...>)
            {
                return hashImpl(qHash(std::get<Is>(tu))...);
            }

            template <class Tu, size_t... Is>
            static void serializeJson(QJsonObject &json, const QStringList &names, const Tu &tu, index_sequence<Is...>)
            {
                serializeJsonImpl(json, std::make_pair(names[Is], get_ref<Is>(tu))...);
            }

            template <class Tu, size_t... Is>
            static void deserializeJson(const QJsonObject &json, const QStringList &names, Tu &tu, index_sequence<Is...>)
            {
                deserializeJsonImpl(json, std::make_pair(names[Is], get_ref<Is>(tu))...);
            }

            template <class Tu, size_t... Is>
            static void serializeJson(QJsonObject &json, const Tu &tu, index_sequence<Is...>)
            {
                serializeJsonImpl(json, std::make_pair(std::get<Is>(tu).m_jsonName, get_ref<Is>(tu))...);
                Q_UNUSED(tu); // avoid compiler warning when all attributes are JSON disabled
            }

            template <class Tu, size_t... Is>
            static void deserializeJson(const QJsonObject &json, Tu &tu, index_sequence<Is...>)
            {
                deserializeJsonImpl(json, std::make_pair(std::get<Is>(tu).m_jsonName, get_ref<Is>(tu))...);
                Q_UNUSED(tu); // avoid compiler warning when all attributes are JSON disabled
            }

        private:
            template <size_t I, class Tu>
            static auto get_ref(Tu &&tu)
            {
                return tieHelper(std::get<I>(std::forward<Tu>(tu)));
            }

            static int compareImpl() { return 0; }
            template <class T, class... Ts>
            static int compareImpl(const std::pair<T, T> &head, const Ts &... tail)
            {
                int result = compare(head.first, head.second);
                if (result) return result;
                return compareImpl(tail...);
            }

            static void marshallImpl(QDBusArgument &) {}
            template <class T, class... Ts>
            static void marshallImpl(QDBusArgument &arg, const T &head, const Ts &... tail)
            {
                marshallHelper(arg, head, 0);
                marshallImpl(arg, tail...);
            }
            template <class T, std::enable_if_t<std::is_base_of<CEmpty, T>::value, int> = 0>
            static void marshallHelper(QDBusArgument &arg, const T &val, int) { static_cast<const typename T::CValueObject &>(val).marshallToDbus(arg); }
            template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
            static void marshallHelper(QDBusArgument &arg, const T &val, int) { arg << static_cast<int>(val); }
            template <class T>
            static void marshallHelper(QDBusArgument &arg, const T &val, ...) { arg << val; }

            static void unmarshallImpl(const QDBusArgument &) {}
            template <class T, class... Ts>
            static void unmarshallImpl(const QDBusArgument &arg, T &head, Ts &... tail)
            {
                unmarshallHelper(arg, head, 0);
                unmarshallImpl(arg, tail...);
            }
            template <class T, std::enable_if_t<std::is_base_of<CEmpty, T>::value, int> = 0>
            static void unmarshallHelper(const QDBusArgument &arg, T &val, int) { static_cast<typename T::CValueObject &>(val).unmarshallFromDbus(arg); }
            template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
            static void unmarshallHelper(const QDBusArgument &arg, T &val, int) { int i; arg >> i; val = static_cast<T>(i); }
            template <class T>
            static void unmarshallHelper(const QDBusArgument &arg, T &val, ...) { arg >> val; }

            static void debugImpl(QDebug) {}
            template <class T, class... Ts>
            static void debugImpl(QDebug dbg, const T &head, const Ts &... tail)
            {
                dbg << head;
                debugImpl(dbg, tail...);
            }

            static void serializeJsonImpl(QJsonObject &) {}
            template <class T, class... Ts>
            static void serializeJsonImpl(QJsonObject &json, std::pair<QString, T> head, Ts... tail)
            {
                json << head;
                serializeJsonImpl(json, tail...);
            }

            static void deserializeJsonImpl(const QJsonObject &) {}
            template <class T, class... Ts>
            static void deserializeJsonImpl(const QJsonObject &json, std::pair<QString, T> head, Ts... tail)
            {
                json.value(head.first) >> head.second;
                deserializeJsonImpl(json, tail...);
            }

            static uint hashImpl() { return 0; }
            template <class... Ts>
            static uint hashImpl(uint head, Ts... tail) { return head ^ hashImpl(tail...); }
        };

        //! \endcond

    } // namespace Private

} // namespace BlackMisc

#endif // guard
