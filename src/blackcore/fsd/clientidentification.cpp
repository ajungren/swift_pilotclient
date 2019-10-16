/* Copyright (C) 2019
 * swift project community / contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution. No part of swift project, including this file, may be copied, modified, propagated,
 * or distributed except according to the terms contained in the LICENSE file.
 */

#include "clientidentification.h"
#include "blackmisc/logmessage.h"

namespace BlackCore
{
    namespace Fsd
    {
        ClientIdentification::ClientIdentification() : MessageBase()
        { }

        ClientIdentification::ClientIdentification(const QString &sender, quint16 clientId, const QString &clientName, int clientVersionMajor,
                int clientVersionMinor, const QString &userCid, const QString &sysUid, const QString &initialChallenge)
            : MessageBase(sender, "SERVER"),
              m_clientId(clientId), m_clientName(clientName),
              m_clientVersionMajor(clientVersionMajor), m_clientVersionMinor(clientVersionMinor),
              m_userCid(userCid), m_sysUid(sysUid),
              m_initialChallenge(initialChallenge)
        { }

        QStringList ClientIdentification::toTokens() const
        {
            auto tokens = QStringList {};
            tokens.push_back(m_sender);
            tokens.push_back(m_receiver);
            tokens.push_back(QString::number(m_clientId, 16));
            tokens.push_back(m_clientName);
            tokens.push_back(QString::number(m_clientVersionMajor));
            tokens.push_back(QString::number(m_clientVersionMinor));
            tokens.push_back(m_userCid);
            tokens.push_back(m_sysUid);
            tokens.push_back(m_initialChallenge);
            return tokens;
        }

        ClientIdentification ClientIdentification::fromTokens(const QStringList &tokens)
        {
            if (tokens.size() < 8)
            {
                BlackMisc::CLogMessage(static_cast<ClientIdentification *>(nullptr)).warning(u"Wrong number of arguments.");
                return {};
            }

            ClientIdentification packet(tokens[0], tokens[2].toUShort(nullptr, 16), tokens[3], tokens[4].toInt(), tokens[5].toInt(),
                                        tokens[6], tokens[7], tokens.size() > 8 ? tokens[8] : QString());
            return packet;
        }
    }
}
