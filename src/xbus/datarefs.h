/*  Copyright (C) 2013 VATSIM Community / contributors
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BLACKSIM_XBUS_DATAREFS_H
#define BLACKSIM_XBUS_DATAREFS_H

//! \file

#include <XPLM/XPLMDataAccess.h>
#include <vector>
#include <string>
#include <cassert>

#include "datarefs.inc"

namespace XBus
{

    //! \private
    class DataRefImpl
    {
    public:
        DataRefImpl(char const* name) : m_ref(XPLMFindDataRef(name))
        {
            //TODO warn if m_ref is NULL
        }

        template <typename T>
        void implSet(T);

        template <typename T>
        T implGet() const;

    private:
        XPLMDataRef m_ref;
    };

    //! \private
    class ArrayDataRefImpl
    {
    public:
        ArrayDataRefImpl(char const* name, size_t size) : m_ref(XPLMFindDataRef(name)), m_size(size)
        {
            //TODO warn if m_ref is NULL
        }

        template <typename T>
        void implSetAll(std::vector<T> const&);

        template <typename T>
        std::vector<T> implGetAll() const;

        template <typename T>
        void implSetAt(size_t index, T);

        template <typename T>
        T implGetAt(size_t index);

    private:
        XPLMDataRef m_ref;
        size_t const m_size;
    };

    /*!
     * Class providing access to a single X-Plane dataref
     * \tparam DataRefTraits The trait class representing the dataref.
     * See the xplane::data namespace and http://www.xsquawkbox.net/xpsdk/docs/DataRefs.html
     */
    template<class DataRefTraits>
    class DataRef : private DataRefImpl
    {
    public:
        //! Constructor
        DataRef() : DataRefImpl(DataRefTraits::name()) {}

        //! Traits type
        typedef DataRefTraits TraitsType;

        //! Dataref type
        typedef typename DataRefTraits::type DataRefType;

        //! Set the value of the dataref (if it is writable)
        void set(DataRefType d) { DataRefImpl::implSet(d); }

        //! Get the value of the dataref
        DataRefType get() const { return DataRefImpl::implGet<DataRefType>(); }
    };

    /*!
     * Class providing access to a single X-Plane array dataref
     * \tparam DataRefTraits The trait class representing the dataref.
     * See the xplane::data namespace and http://www.xsquawkbox.net/xpsdk/docs/DataRefs.html
     */
    template<class DataRefTraits>
    class ArrayDataRef : private ArrayDataRefImpl
    {
    public:
        //! Constructor
        ArrayDataRef() : ArrayDataRefImpl(DataRefTraits::name(), DataRefTraits::size) {}

        //! Traits type
        typedef DataRefTraits TraitsType;

        //! Dataref type
        typedef typename DataRefTraits::type DataRefType;

        //! Set the value of the whole array (if it is writable)
        void setAll(std::vector<DataRefType> const& a) { ArrayDataRefImpl::implSetAll(a); }

        //! Get the value of the whole array
        std::vector<DataRefType> getAll() const { return ArrayDataRefImpl::implGetAll<DataRefType>(); }

        //! Set the value of a single element (if it is writable)
        void setAt(size_t index, DataRefType d) { ArrayDataRefImpl::implSetAt(index, d); }

        //! Get the value of a single element
        DataRefType getAt(size_t index) const { return ArrayDataRefImpl::implGetAt<DataRefType>(index); }
    };

    /*!
     * Class providing access to a single X-Plane string dataref
     * \tparam DataRefTraits The trait class representing the dataref.
     * See the xplane::data namespace and http://www.xsquawkbox.net/xpsdk/docs/DataRefs.html
     */
    template<class DataRefTraits>
    class StringDataRef
    {
    public:
        //! Constructor
        StringDataRef() : m_ref(XPLMFindDataRef(DataRefTraits::name()))
        {
            //TODO warn if m_ref is NULL
        }

        //! Set the value of the whole string (if it is writable)
        void set(std::string const& s) { setSubstr(0, s); }

        //! Get the value of the whole string
        std::string get() const { return getSubstr(0, DataRefTraits::size); }

        //! Set the value of part of the string (if it is writable)
        void setSubstr(size_t offset, std::string const& s)
        { assert((s.size() + 1) <= (DataRefTraits::size - offset)); XPLMSetDatab(m_ref, s.c_str(), offset, s.size() + 1); }

        //! Get the value of part of the string
        std::string getSubstr(size_t offset, size_t size) const
        { std::string s (size, 0); XPLMGetDatab(m_ref, &s[0], (int)offset, (int)size); size = s.find(char(0)); if (size != std::string::npos) s.resize(size); return s; }

    private:
        XPLMDataRef m_ref;
    };

    template <>
    inline void DataRefImpl::implSet<int>(int d) { XPLMSetDatai(m_ref, d); }
    template <>
    inline void DataRefImpl::implSet<float>(float d) { XPLMSetDataf(m_ref, d); }
    template <>
    inline void DataRefImpl::implSet<double>(double d) { XPLMSetDatad(m_ref, d); }
    template <>
    inline int DataRefImpl::implGet<int>() const { return XPLMGetDatai(m_ref); }
    template <>
    inline float DataRefImpl::implGet<float>() const { return XPLMGetDataf(m_ref); }
    template <>
    inline double DataRefImpl::implGet<double>() const { return XPLMGetDatad(m_ref); }

    template <>
    inline void ArrayDataRefImpl::implSetAll<int>(std::vector<int> const& v) { assert(v.size() <= m_size); XPLMSetDatavi(m_ref, const_cast<int*>(&v[0]), 0, (int)v.size()); }
    template <>
    inline void ArrayDataRefImpl::implSetAll<float>(std::vector<float> const& v) { assert(v.size() <= m_size); XPLMSetDatavf(m_ref, const_cast<float*>(&v[0]), 0, (int)v.size()); }
    template <>
    inline std::vector<int> ArrayDataRefImpl::implGetAll<int>() const { std::vector<int> v (m_size); XPLMGetDatavi(m_ref, &v[0], 0, (int)m_size); return v; }
    template <>
    inline std::vector<float> ArrayDataRefImpl::implGetAll<float>() const { std::vector<float> v (m_size); XPLMGetDatavf(m_ref, &v[0], 0, (int)m_size); return v; }

    template <>
    inline void ArrayDataRefImpl::implSetAt<int>(size_t i, int d) { assert(i <= m_size); XPLMSetDatavi(m_ref, &d, (int)i, 1); }
    template <>
    inline void ArrayDataRefImpl::implSetAt<float>(size_t i, float d) { assert(i <= m_size); XPLMSetDatavf(m_ref, &d, (int)i, 1); }
    template <>
    inline int ArrayDataRefImpl::implGetAt<int>(size_t i) { assert(i <= m_size); int d; XPLMGetDatavi(m_ref, &d, (int)i, 1); return d; }
    template <>
    inline float ArrayDataRefImpl::implGetAt<float>(size_t i) { assert(i <= m_size); float d; XPLMGetDatavf(m_ref, &d, (int)i, 1); return d; }

} // namespace

#endif // guard
