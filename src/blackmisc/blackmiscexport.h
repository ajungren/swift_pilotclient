/* Copyright (C) 2014
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKMISC_MACROS_H
#define BLACKMISC_MACROS_H

#include <QtGlobal>

/*!
 * \def BLACKMISC_EXPORT
 * BlackMisc Export Macro
 */
#ifndef WITH_STATIC
#  if defined(BUILD_BLACKMISC_LIB)
#    define BLACKMISC_EXPORT Q_DECL_EXPORT
#  else
#    define BLACKMISC_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define BLACKMISC_EXPORT
#endif

/*!
 * \def BLACKMISC_EXPORT_TEMPLATE
 * BlackMisc Template Export Macro
 */
#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
#  define BLACKMISC_EXPORT_TEMPLATE BLACKMISC_EXPORT
#else
#  define BLACKMISC_EXPORT_TEMPLATE
#endif


//! \cond
// helper define
#define BLACK_STRINGIFY_X(v) #v
//! \endcond

/*!
 * \def BLACK_STRINGIFY
 * Value in quotes (normally to be used then with QString)
 */
#define BLACK_STRINGIFY(v) BLACK_STRINGIFY_X(v)

#endif // guard
