/* Copyright (C) 2013 VATSIM Community / authors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*!
    \file
*/

#ifndef BLACKMISC_SEQUENCE_H
#define BLACKMISC_SEQUENCE_H

#include "iterator.h"
#include "containerbase.h"
#include <QScopedPointer>
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <utility>

namespace BlackMisc
{

    /*!
     * \brief Generic type-erased sequential container with value semantics.
     * \tparam T the type of elements contained.
     *
     * Can take any suitable container class as its implementation at runtime.
     */
    template <class T>
    class CSequence : public CContainerBase<CSequence, T>
    {
    public:
        //! \brief STL compatibility
        //! @{
        typedef T key_type;
        typedef T value_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef typename Iterators::ConstBidirectionalIterator<T> const_iterator;
        typedef typename Iterators::BidirectionalIterator<T> iterator;
        typedef ptrdiff_t difference_type;
        typedef int size_type;
        //! @}

        /*!
         * \brief Default constructor.
         */
        CSequence() : m_pimpl(new Pimpl<QList<T>>(QList<T>())) {}

        /*!
         * \brief Constructor.
         * \tparam C Becomes the sequence's implementation type.
         * \param c Initial value for the sequence; typically empty, but could contain elements.
         */
        template <class C> CSequence(C c) : m_pimpl(new Pimpl<C>(std::move(c))) {}

        /*!
         * \brief Copy constructor.
         * \param other
         */
        CSequence(const CSequence &other) : m_pimpl(other.pimpl() ? other.pimpl()->clone() : nullptr) {}

        /*!
         * \brief Move constructor.
         * \param other
         */
        CSequence(CSequence &&other) : m_pimpl(other.m_pimpl.take()) {}

        /*!
         * \brief Assignment.
         * \tparam C Becomes the sequence's new implementation type.
         * \param c New value for the sequence; typically empty, but could contain elements.
         */
        template <class C> CSequence &operator =(C c) { m_pimpl.reset(new Pimpl<C>(std::move(c))); return *this; }

        /*!
         * \brief Copy assignment.
         * \param other
         * \return
         */
        CSequence &operator =(const CSequence &other) { m_pimpl.reset(other.pimpl() ? other.pimpl()->clone() : nullptr); return *this; }

        /*!
         * \brief Move assignment.
         * \param other
         * \return
         */
        CSequence &operator =(CSequence &&other) { m_pimpl.reset(other.m_pimpl.take()); return *this; }

        /*!
         * \brief Change the implementation type but keep all the same elements, by copying them into the new implementation.
         * \tparam C Becomes the sequence's new implementation type.
         */
        template <class C> void changeImpl(C = C()) { CSequence c = C(); for (auto i = cbegin(); i != cend(); ++i) c.push_back(*i); *this = std::move(c); }

        /*!
         * \brief Like changeImpl, but uses the implementation type of another sequence.
         * \param other
         * \pre The other sequence must be initialized.
         */
        void useImplOf(const CSequence &other) { PimplPtr p = other.pimpl()->cloneEmpty(); for (auto i = cbegin(); i != cend(); ++i) p->push_back(*i); m_pimpl.reset(p.take()); }

        /*!
         * \brief Returns iterator at the beginning of the sequence.
         * \return
         */
        iterator begin() { return pimpl() ? pimpl()->begin() : iterator(); }

        /*!
         * \brief Returns iterator at the beginning of the sequence.
         * \return
         */
        const_iterator begin() const { return pimpl() ? pimpl()->begin() : const_iterator(); }

        /*!
         * \brief Returns iterator at the beginning of the sequence.
         * \return
         */
        const_iterator cbegin() const { return pimpl() ? pimpl()->cbegin() : const_iterator(); }

        /*!
         * \brief Returns iterator one past the end of the sequence.
         * \return
         */
        iterator end() { return pimpl() ? pimpl()->end() : iterator(); }

        /*!
         * \brief Returns iterator one past the end of the sequence.
         * \return
         */
        const_iterator end() const { return pimpl() ? pimpl()->end() : const_iterator(); }

        /*!
         * \brief Returns iterator one past the end of the sequence.
         * \return
         */
        const_iterator cend() const { return pimpl() ? pimpl()->cend() : const_iterator(); }

        /*!
         * \brief Swap this sequence with another.
         * \param other
         */
        void swap(CSequence &other) { m_pimpl.swap(other.m_pimpl); }

        /*!
         * \brief Access an element by its index.
         * \param index
         * \return
         * \pre The sequence must be initialized and the index in bounds.
         */
        reference operator [](size_type index) { Q_ASSERT(pimpl()); return pimpl()->operator [](index); }

        /*!
         * \brief Access an element by its index.
         * \param index
         * \return
         * \pre The sequence must be initialized and the index in bounds.
         */
        const_reference operator [](size_type index) const { Q_ASSERT(pimpl()); return pimpl()->operator [](index); }

        /*!
         * \brief Access the first element.
         * \return
         * \pre The sequence must not be empty.
         */
        reference front() { Q_ASSERT(!empty()); return pimpl()->front(); }

        /*!
         * \brief Access the first element.
         * \return
         * \pre The sequence must not be empty.
         */
        const_reference front() const { Q_ASSERT(!empty()); return pimpl()->front(); }

        /*!
         * \brief Access the last element.
         * \return
         * \pre The sequence must not be empty.
         */
        reference back() { Q_ASSERT(!empty()); return pimpl()->back(); }

        /*!
         * \brief Access the last element.
         * \return
         * \pre The sequence must not be empty.
         */
        const_reference back() const { Q_ASSERT(!empty()); return pimpl()->back(); }

        /*!
         * \brief Returns number of elements in the sequence.
         * \return
         */
        size_type size() const { return pimpl() ? pimpl()->size() : 0; }

        /*!
         * \brief Returns true if the sequence is empty.
         * \return
         */
        bool empty() const { return pimpl() ? pimpl()->empty() : true; }

        /*!
         * \brief Synonym for empty.
         * \return
         */
        bool isEmpty() const { return empty(); }

        /*!
         * \brief Removes all elements in the sequence.
         */
        void clear() { if (pimpl()) pimpl()->clear(); }

        /*!
         * \brief Inserts an element into the sequence.
         * \param before
         * \param value
         * \return An iterator to the position where value was inserted.
         * \pre The sequence must be initialized.
         */
        iterator insert(iterator before, const T &value) { Q_ASSERT(pimpl()); return pimpl()->insert(before, value); }

        /*!
         * \brief Appends an element at the end of the sequence.
         * \param value
         * \pre The sequence must be initialized.
         */
        void push_back(const T &value) { Q_ASSERT(pimpl()); pimpl()->push_back(value); }

        /*!
         * \brief Synonym for push_back.
         * \param value
         * \pre The sequence must be initialized.
         */
        void insert(const T &value) { push_back(value); }

        /*!
         * \brief Removes an element at the end of the sequence.
         * \pre The sequence must contain at least one element.
         */
        void pop_back() { Q_ASSERT(!empty()); pimpl()->pop_back(); }

        /*!
         * \brief Remove the element pointed to by the given iterator.
         * \param pos
         * \return An iterator to the position of the next element after the one removed.
         * \pre The sequence must be initialized.
         */
        iterator erase(iterator pos) { Q_ASSERT(pimpl()); return pimpl()->erase(pos); }

        /*!
         * \brief Remove the range of elements between two iterators.
         * \param it1
         * \param it2
         * \return An iterator to the position of the next element after the one removed.
         * \pre The sequence must be initialized.
         */
        iterator erase(iterator it1, iterator it2) { Q_ASSERT(pimpl()); return pimpl()->erase(it1, it2); }

        /*!
         * \brief Test for equality.
         * \param other
         * \return
         * \todo Improve inefficient implementation.
         */
        bool operator ==(const CSequence &other) const { return (empty() && other.empty()) ? true : (size() != other.size() ? false : *pimpl() == *other.pimpl()); }

        /*!
         * \brief Test for inequality.
         * \param other
         * \return
         * \todo Improve inefficient implementation.
         */
        bool operator !=(const CSequence &other) const { return !(*this == other); }

    private:
        class PimplBase
        {
        public:
            virtual ~PimplBase() {}
            virtual PimplBase *clone() const = 0;
            virtual PimplBase *cloneEmpty() const = 0;
            virtual iterator begin() = 0;
            virtual const_iterator begin() const = 0;
            virtual const_iterator cbegin() const = 0;
            virtual iterator end() = 0;
            virtual const_iterator end() const = 0;
            virtual const_iterator cend() const = 0;
            virtual reference operator [](size_type index) = 0;
            virtual const_reference operator [](size_type index) const = 0;
            virtual reference front() = 0;
            virtual const_reference front() const = 0;
            virtual reference back() = 0;
            virtual const_reference back() const = 0;
            virtual size_type size() const = 0;
            virtual bool empty() const = 0;
            virtual void clear() = 0;
            virtual iterator insert(iterator pos, const T &value) = 0;
            virtual void push_back(const T &value) = 0;
            virtual void pop_back() = 0;
            virtual iterator erase(iterator pos) = 0;
            virtual iterator erase(iterator it1, iterator it2) = 0;
            virtual bool operator ==(const PimplBase &other) const = 0;
        };

        template <class C> class Pimpl : public PimplBase
        {
        public:
            static_assert(std::is_same<T, typename C::value_type>::value, "CSequence must be initialized from a container with the same value_type.");
            Pimpl(C &&c) : m_impl(std::move(c)) {}
            PimplBase *clone() const { return new Pimpl(*this); }
            PimplBase *cloneEmpty() const { return new Pimpl(C()); }
            iterator begin() { return m_impl.begin(); }
            const_iterator begin() const { return m_impl.cbegin(); }
            const_iterator cbegin() const { return m_impl.cbegin(); }
            iterator end() { return m_impl.end(); }
            const_iterator end() const { return m_impl.cend(); }
            const_iterator cend() const { return m_impl.cend(); }
            reference operator [](size_type index) { return m_impl[index]; }
            const_reference operator [](size_type index) const { return m_impl[index]; }
            reference front() { return m_impl.front(); }
            const_reference front() const { return m_impl.front(); }
            reference back() { return m_impl.back(); }
            const_reference back() const { return m_impl.back(); }
            size_type size() const { return m_impl.size(); }
            bool empty() const { return m_impl.empty(); }
            void clear() { m_impl.clear(); }
            iterator insert(iterator pos, const T &value) { return m_impl.insert(*static_cast<const typename C::iterator*>(pos.getImpl()), value); }
            void push_back(const T &value) { m_impl.push_back(value); }
            void pop_back() { m_impl.pop_back(); }
            iterator erase(iterator pos) { return m_impl.erase(*static_cast<const typename C::iterator*>(pos.getImpl())); }
            iterator erase(iterator it1, iterator it2) { return m_impl.erase(*static_cast<const typename C::iterator*>(it1.getImpl(), it2.getImpl())); }
            bool operator ==(const PimplBase &other) const { Pimpl copy = C(); for (auto i = other.cbegin(); i != other.cend(); ++i) copy.push_back(*i); return m_impl == copy.m_impl; }
        private:
            C m_impl;
        };

        typedef QScopedPointer<PimplBase> PimplPtr;
        PimplPtr m_pimpl;

        // using these methods to access m_pimpl.data() eases the cognitive burden of correctly forwarding const
        PimplBase *pimpl() { return m_pimpl.data(); }
        const PimplBase *pimpl() const { return m_pimpl.data(); }
    };

} //namespace BlackMisc

Q_DECLARE_METATYPE(BlackMisc::CSequence<int>)
Q_DECLARE_METATYPE(BlackMisc::CSequence<uint>)
Q_DECLARE_METATYPE(BlackMisc::CSequence<qlonglong>)
Q_DECLARE_METATYPE(BlackMisc::CSequence<qulonglong>)
Q_DECLARE_METATYPE(BlackMisc::CSequence<double>)

#endif //BLACKMISC_SEQUENCE_H