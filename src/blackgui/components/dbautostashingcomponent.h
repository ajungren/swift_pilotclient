/* Copyright (C) 2016
 * swift project Community / Contributors
 *
 * This file is part of swift Project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_COMPONENTS_DBAUTOSTASHINGCOMPONENT_H
#define BLACKGUI_COMPONENTS_DBAUTOSTASHINGCOMPONENT_H

#include "blackcore/webdataservices.h"
#include "dbmappingcomponentaware.h"
#include "blackmisc/logcategorylist.h"
#include "blackgui/views/aircraftmodelview.h"
#include <QDialog>

namespace Ui { class CDbAutoStashingComponent; }

namespace BlackGui
{
    namespace Components
    {
        /*!
         * Stashing component
         */
        class CDbAutoStashingComponent :
            public QDialog,
            public BlackMisc::Network::CWebDataServicesAware,
            public BlackGui::Components::CDbMappingComponentAware
        {
            Q_OBJECT

        public:
            //! Current state of this component
            enum State
            {
                Idle,
                Running,
                Completed
            };

            //! Constructor
            explicit CDbAutoStashingComponent(QWidget *parent = nullptr);

            //! Destructor
            ~CDbAutoStashingComponent();

            //! \copydoc BlackMisc::Network::CWebDataServicesAware::setProvider
            virtual void setProvider(BlackMisc::Network::IWebDataServicesProvider *webDataReaderProvider) override;

            //! At least run once and completed
            bool isCompleted() const { return m_state == Completed; }

        public slots:
            //! \copydoc QDialog::accept
            virtual void accept() override;

            //! \copydoc QDialog::exec
            virtual int exec() override;

            //! Show last result
            void showLastResults();

        private slots:
            //! Data have been read
            void ps_entitiesRead(BlackMisc::Network::CEntityFlags::Entity entity, BlackMisc::Network::CEntityFlags::ReadState readState, int count);

        private:
            QScopedPointer<Ui::CDbAutoStashingComponent> ui;

            //! Init the component
            void initGui();

            //! Update GUI values
            void updateGuiValues(int percent);

            //! Number of all or selected models
            int getSelectedOrAllCount() const;

            //! Model view to take models from
            const BlackGui::Views::CAircraftModelView *currentModelView() const;

            //! Model view to take models from
            BlackGui::Views::CAircraftModelView *currentModelView();

            //! Add a status message
            void addStatusMessage(const BlackMisc::CStatusMessage &msg);

            //! Add a status message for a given model (prefixed)
            void addStatusMessage(const BlackMisc::CStatusMessage &msg, const BlackMisc::Simulation::CAircraftModel &model);

            //! Try stashing selected or all models
            void tryToStashModels();

            //! Try stashing a model
            //! \param model this model can be updated with consolidated data
            //! \return true means stashing is possible
            bool tryToStashModel(BlackMisc::Simulation::CAircraftModel &model);

            //! Categories
            const BlackMisc::CLogCategoryList &categgories();

            int m_noStashed = 0;           //!< stashed models
            int m_noData = 0;              //!< not stashed because no data
            int m_noValidationFailed = 0;  //!< not stashed because validation failed
            State m_state = Idle;          //!< modus
            BlackMisc::Simulation::CAircraftModelList m_modelsToStash; //!< Models about to be stashed
        };
    } // ns
} // ns

#endif // guard
