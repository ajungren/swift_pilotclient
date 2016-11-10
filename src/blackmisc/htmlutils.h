/* Copyright (C) 2016
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_HTMLUTILS_H
#define BLACKMISC_HTMLUTILS_H

#include "blackmisc/blackmiscexport.h"


//! Free functions in BlackMisc
namespace BlackMisc
{
    //! Container to standard HTML table
    template <class Obj, class Container> QString toHtmlTable(const CPropertyIndexList &indexes, const Container &container)
    {
        if (indexes.isEmpty() || container.isEmpty()) { return ""; }
        QString html;
        for (const Obj &obj : container)
        {
            QString rowHtml;
            for (const CPropertyIndex &index : indexes)
            {
                rowHtml += "<td>" + obj.propertyByIndex(index).toQString(true) + "</td>";
            }
            html += "<tr>" + rowHtml + "</tr>";
        }
        return "<table>" + html + "</table>";
    }
} // ns

#endif // guard
