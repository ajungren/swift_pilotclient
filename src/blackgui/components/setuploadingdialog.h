/* Copyright (C) 2017
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_COMPONENTS_SETUPLOADINGDIALOG_H
#define BLACKGUI_COMPONENTS_SETUPLOADINGDIALOG_H

#include "blackmisc/statusmessagelist.h"
#include <QDialog>

namespace Ui { class CSetupLoadingDialog; }
namespace BlackGui
{
    namespace Components
    {
        /**
         * Setup dialog, if something goes wrong
         */
        class CSetupLoadingDialog : public QDialog
        {
            Q_OBJECT

        public:
            //! Ctor
            explicit CSetupLoadingDialog(QWidget *parent = nullptr);

            //! Ctor with messages
            CSetupLoadingDialog(const BlackMisc::CStatusMessageList &msgs, QWidget *parent = nullptr);

            //! Dtor
            virtual ~CSetupLoadingDialog();

        private:
            QScopedPointer<Ui::CSetupLoadingDialog> ui;

            //! Set info fields
            void displayBootstrapUrls();

            //! Display bootstrap URL
            void displayCmdBoostrapUrl();

            //! Display global setup
            void displayGlobalSetup();

            //! Try again without explicit bootstrap URL
            void tryAgainWithoutBootstrapUrl();
        };
    } // ns
} // ns

#endif // guard
