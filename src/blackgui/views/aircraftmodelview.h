/* Copyright (C) 2013
 * swift project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

//! \file

#ifndef BLACKGUI_AIRCRAFTMODELVIEW_H
#define BLACKGUI_AIRCRAFTMODELVIEW_H

#include "blackgui/blackguiexport.h"
#include "blackgui/filters/aircraftmodelfilterdialog.h"
#include "blackgui/models/aircraftmodellistmodel.h"
#include "viewdbobjects.h"

namespace BlackGui
{
    namespace Views
    {
        //! Aircraft view
        class BLACKGUI_EXPORT CAircraftModelView :
            public CViewWithDbObjects<Models::CAircraftModelListModel, BlackMisc::Simulation::CAircraftModelList, BlackMisc::Simulation::CAircraftModel, int>
        {
            Q_OBJECT

        public:
            //! Constructor
            explicit CAircraftModelView(QWidget *parent = nullptr);

            //! Set display mode
            void setAircraftModelMode(Models::CAircraftModelListModel::AircraftModelMode mode);

            //! Apply to selected objects
            int applyToSelected(const BlackMisc::Aviation::CLivery &livery);

            //! Apply to selected objects
            int applyToSelected(const BlackMisc::Aviation::CAircraftIcaoCode &icao);

            //! Apply to selected objects
            int applyToSelected(const BlackMisc::Simulation::CDistributor &distributor);

            //! Apply to selected objects
            int applyToSelected(const BlackMisc::CPropertyIndexVariantMap &vm);

            //! Has any models to stash and it is allowed to stash
            bool hasSelectedModelsToStash() const;

            //! Add the technically supported metatypes allowed for drag and drop
            void setImplementedMetaTypeIds();

            //! Add my own filter dialog
            void addFilterDialog();

            //! Filter dialog if any
            BlackGui::Filters::CAircraftModelFilterDialog *getFilterDialog() const;

            //! Remove models with model strings
            int removeModelsWithModelString(const QStringList &modelStrings, Qt::CaseSensitivity sensitivity = Qt::CaseInsensitive);

            //! Remove models with model strings
            int removeModelsWithModelString(const BlackMisc::Simulation::CAircraftModelList &models, Qt::CaseSensitivity sensitivity = Qt::CaseInsensitive);

            //! \copydoc BlackGui::Models::CAircraftModelListModel::setHighlightModelStrings(const QStringList &)
            void setHighlightModelStrings(const QStringList &highlightModels);

            //! \copydoc BlackGui::Models::CAircraftModelListModel::setHighlightModelStrings(bool)
            void setHighlightModelStrings(bool highlight);

            //! \copydoc BlackGui::Models::CAircraftModelListModel::setHighlightModelStringsColor
            void setHighlightModelStringsColor(const QBrush &brush);

            bool highlightModelsStrings() const;
            //! \copydoc BlackGui::Models::CAircraftModelListModel::highlightModelStrings

        signals:
            //! Request to stash if applicable
            void requestStash(const BlackMisc::Simulation::CAircraftModelList &models);

            //! Highligh stashed models has been toggled
            void toggledHighlightStashedModels();

            //! Request further handling of drops I cannot handle on my own
            void requestHandlingOfStashDrop(const BlackMisc::Aviation::CAirlineIcaoCode &airlineIcao);

        protected:
            //! \copydoc QTableView::dropEvent
            virtual void dropEvent(QDropEvent *event) override;

            //! \copydoc CViewBaseNonTemplate::customMenu
            virtual void customMenu(QMenu &menu) const override;

        private slots:
            //! Highlight stashed models
            void ps_toggleHighlightStashedModels();

            //! Toggle highlight invalid models
            void ps_toogleHighlightInvalidModels();

            //! Toggle if stashing unselects
            void ps_stashingClearsSelection();

            //! Stash shortcut pressed
            void ps_requestStash();

        private:
            bool m_stashingClearsSelection = true; //!< stashing unselects
        };
    } // ns
} // ns
#endif // guard
