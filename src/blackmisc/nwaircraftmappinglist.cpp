/* Copyright (C) 2013 VATSIM Community / authors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nwaircraftmappinglist.h"
#include "predicates.h"

using namespace BlackMisc::Network;
using namespace BlackMisc::Aviation;

namespace BlackMisc
{
    namespace Network
    {
        /*
         * Empty constructor
         */
        CAircraftMappingList::CAircraftMappingList() { }

        /*
         * Construct from base class object
         */
        CAircraftMappingList::CAircraftMappingList(const CSequence<CAircraftMapping> &other) :
            CSequence<CAircraftMapping>(other)
        { }

        CAircraftMappingList CAircraftMappingList::findByIcaoCodeWildcard(const CAircraftIcao &searchIcao) const
        {
            return this->findBy([ = ](const CAircraftMapping &mapping)
            {
                return mapping.matchesWildcardIcao(searchIcao);
            });
        }

        CAircraftMappingList CAircraftMappingList::findByIcaoCodeExact(const CAircraftIcao &searchIcao) const
        {
            return this->findBy(&CAircraftMapping::getIcao, searchIcao);
        }

        CAircraftMappingList CAircraftMappingList::findByModelString(const QString modelString, Qt::CaseSensitivity sensitivity) const
        {
            return this->findBy([ = ](const CAircraftMapping &mapping)
            {
                return mapping.matchesModelString(modelString, sensitivity);
            });
        }

        /*
         * Register metadata
         */
        void CAircraftMappingList::registerMetadata()
        {
            qRegisterMetaType<BlackMisc::CSequence<CAircraftMapping>>();
            qDBusRegisterMetaType<BlackMisc::CSequence<CAircraftMapping>>();
            qRegisterMetaType<BlackMisc::CCollection<CAircraftMapping>>();
            qDBusRegisterMetaType<BlackMisc::CCollection<CAircraftMapping>>();
            qRegisterMetaType<CAircraftMappingList>();
            qDBusRegisterMetaType<CAircraftMappingList>();
        }

    } // namespace
} // namespace
