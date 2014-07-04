/* Copyright (C) 2013 VATSIM Community / authors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "simulatorinfolist.h"

namespace BlackSim
{

    CSimulatorInfoList::CSimulatorInfoList() { }

    bool CSimulatorInfoList::supportsSimulator(const CSimulatorInfo &info)
    {
        return this->contains(info);
    }

    QStringList CSimulatorInfoList::toStringList(bool i18n) const
    {
        return this->transform([i18n](const CSimulatorInfo &info) { return info.toQString(i18n); });
    }

    void CSimulatorInfoList::registerMetadata()
    {
        qRegisterMetaType<BlackMisc::CSequence<CSimulatorInfo>>();
        qDBusRegisterMetaType<BlackMisc::CSequence<CSimulatorInfo>>();
        qRegisterMetaType<BlackMisc::CCollection<CSimulatorInfo>>();
        qDBusRegisterMetaType<BlackMisc::CCollection<CSimulatorInfo>>();
        qRegisterMetaType<CSimulatorInfoList>();
        qDBusRegisterMetaType<CSimulatorInfoList>();
    }

} // namespace BlackSim
