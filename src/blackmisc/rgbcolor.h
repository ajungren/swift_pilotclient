/* Copyright (C) 2015
 * Swift Project Community / Contributors
 *
 * This file is part of Swift Project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_RGBCOLOR_H
#define BLACKMISC_RGBCOLOR_H

#include "blackmiscexport.h"
#include "valueobject.h"
#include <QColor>

namespace BlackMisc
{
    /*!
     * Color
     */
    class BLACKMISC_EXPORT CRgbColor : public CValueObject<CRgbColor>
    {
    public:
        //! Properties by index
        enum ColumnIndex
        {
            IndexRed = BlackMisc::CPropertyIndex::GlobalIndexCRgbColor,
            IndexGreen,
            IndexBlue,
            IndexWebHex
        };

        //! Constructor
        CRgbColor() = default;

        //! Constructor (hex or color name)
        CRgbColor(const QString &color, bool isName = false);

        //! Constructor
        CRgbColor(int r, int g, int b);

        //! Constructor from QColor
        CRgbColor(const QColor &color);

        //! To QColor
        QColor toQColor() const;

        //! Set by QColor
        bool setQColor(const QColor &color);

        //! Red
        int red() const;

        //! Red 0..1
        double normalizedRed() const;

        //! Red as hex
        QString redHex(int digits = 2) const;

        //! Green
        int green() const;

        //! Green 0..1
        double normalizedGreen() const;

        //! Green as hex
        QString greenHex(int digits = 2) const;

        //! Blue
        int blue() const;

        //! Blue 0..1
        double normalizedBlue() const;

        //! Blue as hex
        QString blueHex(int digits = 2) const;

        //! Hex value
        QString hex(bool withHash = false) const;

        //! Hex or color name
        void setByString(const QString &color, bool isName = false);

        //! Valid?
        bool isValid() const;

        //! Color distance [0..1]
        //! http://stackoverflow.com/questions/4754506/color-similarity-distance-in-rgba-color-space/8796867#8796867
        double colorDistance(const CRgbColor &color) const;

        //! Mark as invalid
        void setInvalid();

        //! Representing icon
        CIcon toIcon() const;

        //! \copydoc BlackMisc::Mixin::String::toQString
        QString convertToQString(bool i18n = false) const;

        //! \copydoc BlackMisc::Mixin::Index::propertyByIndex
        CVariant propertyByIndex(const BlackMisc::CPropertyIndex &index) const;

        //! \copydoc BlackMisc::Mixin::Index::setPropertyByIndex
        void setPropertyByIndex(const CVariant &variant, const BlackMisc::CPropertyIndex &index);

        //! Compare for index
        int comparePropertyByIndex(const CRgbColor &compareValue, const CPropertyIndex &index) const;

    private:
        BLACK_ENABLE_TUPLE_CONVERSION(CRgbColor)
        int m_r = -1;
        int m_g = -1;
        int m_b = -1;

        //! Color range 255/4095/65535
        double colorRange() const;

        static QString intToHex(int h, int digits = 2);
    };
} // namespace

Q_DECLARE_METATYPE(BlackMisc::CRgbColor)
BLACK_DECLARE_TUPLE_CONVERSION(BlackMisc::CRgbColor, (
                                   attr(o.m_r),
                                   attr(o.m_g),
                                   attr(o.m_b)
                               ))

#endif // guard
