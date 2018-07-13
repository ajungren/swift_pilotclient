/* Copyright (C) 2017
 * swift project Community / Contributors
 *
 * This file is part of swift Project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

#include "updateinfodialog.h"
#include "ui_updateinfodialog.h"
#include "blackgui/guiapplication.h"
#include <QPushButton>
#include <QDesktopServices>

using namespace BlackMisc::Db;

namespace BlackGui
{
    namespace Components
    {
        CUpdateInfoDialog::CUpdateInfoDialog(QWidget *parent) :
            QDialog(parent),
            ui(new Ui::CUpdateInfoDialog)
        {
            ui->setupUi(this);
            ui->bb_UpdateInfolDialog->button(QDialogButtonBox::Ok)->setText(" Download and install ");
            ui->cb_DontShowAgain->setChecked(!m_setting.get());
            this->selectionChanged();
            connect(ui->comp_UpdateInfo, &CUpdateInfoComponent::selectionChanged, this, &CUpdateInfoDialog::selectionChanged);
            connect(ui->cb_DontShowAgain, &QCheckBox::toggled, this, &CUpdateInfoDialog::onDontShowAgain);
        }

        CUpdateInfoDialog::~CUpdateInfoDialog()
        { }

        bool CUpdateInfoDialog::isNewVersionAvailable() const
        {
            const bool newVersion = ui->comp_UpdateInfo->isNewPilotClientVersionAvailable();
            return newVersion;
        }

        int CUpdateInfoDialog::exec()
        {
            const int r = QDialog::exec();
            if (r != QDialog::Accepted) { return r; }
            if (!ui->comp_UpdateInfo->isNewPilotClientVersionAvailable()) { return QDialog::Rejected; }
            const CDistribution distribution = ui->comp_UpdateInfo->getCurrentDistribution();
            if (!distribution.hasDownloadUrls()) { return QDialog::Rejected; }

            // in future, start download and close application
            // for now, just open URL
            QDesktopServices::openUrl(distribution.getDownloadUrls().getRandomUrl());
            return QDialog::Rejected;
        }

        bool CUpdateInfoDialog::event(QEvent *event)
        {
            if (CGuiApplication::triggerShowHelp(this, event)) { return true; }
            return QDialog::event(event);
        }

        void CUpdateInfoDialog::onDontShowAgain(bool dontShowAgain)
        {
            m_setting.setAndSave(!dontShowAgain);
        }

        void CUpdateInfoDialog::selectionChanged()
        {
            const bool nv = ui->comp_UpdateInfo->isNewPilotClientVersionAvailable();
            ui->bb_UpdateInfolDialog->button(QDialogButtonBox::Ok)->setVisible(nv);
        }
    } // ns
} // ns
