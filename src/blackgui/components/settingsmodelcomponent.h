/* Copyright (C) 2016
 * swift project Community / Contributors
 *
 * This file is part of swift Project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_COMPONENTS_SETTINGSMODELCOMPONENT_H
#define BLACKGUI_COMPONENTS_SETTINGSMODELCOMPONENT_H

#include "blackgui/settings/guisettings.h"
#include <QFrame>

namespace Ui { class CSettingsModelComponent; }
namespace BlackGui
{
    namespace Components
    {
        /*!
         * Settings UI for model matching/mapping
         */
        class CSettingsModelComponent : public QFrame
        {
            Q_OBJECT

        public:
            //! Constructor
            explicit CSettingsModelComponent(QWidget *parent = nullptr);

            //! Destructor
            virtual ~CSettingsModelComponent();

            //! Interval, -1 for disabled
            int getBackgroundUpdaterIntervallSecs() const;

        private:
            QScopedPointer<Ui::CSettingsModelComponent> ui;
            BlackMisc::CSetting<BlackGui::Settings::TBackgroundConsolidation> m_consolidationSetting { this, &CSettingsModelComponent::cacheChanged }; //!< consolidation time

            //! Consolidation time entered
            void consolidationEntered();

            //! Cache has been changed
            void cacheChanged();
        };
    } // ns
} // ns

#endif // guard
