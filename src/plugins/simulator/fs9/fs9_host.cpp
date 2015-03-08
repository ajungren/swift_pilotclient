/* Copyright (C) 2014
 * swift project community / contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "blacksimplugin_freefunctions.h"
#include "fs9_host.h"
#include "multiplayer_packet_parser.h"
#include "multiplayer_packets.h"
#include "blackmisc/project.h"
#include "blackmisc/logmessage.h"
#include <QScopedArrayPointer>
#include <QVector>

using namespace BlackMisc;

namespace BlackSimPlugin
{
    namespace Fs9
    {
        CFs9Host::CFs9Host(QObject *owner) :
            CDirectPlayPeer(owner, CProject::systemNameAndVersion())
        {
        }

        QString CFs9Host::getHostAddress()
        {
            QString address;
            if (m_hostStatus == Hosting)
            {
                DWORD dwNumAddresses = 0;

                HRESULT hr;
                QVector<LPDIRECTPLAY8ADDRESS> addresses(dwNumAddresses);
                m_directPlayPeer->GetLocalHostAddresses(addresses.data(), &dwNumAddresses, 0);
                addresses.resize(dwNumAddresses);
                ZeroMemory( addresses.data(), dwNumAddresses * sizeof(LPDIRECTPLAY8ADDRESS) );
                if (FAILED (hr = m_directPlayPeer->GetLocalHostAddresses(addresses.data(), &dwNumAddresses, 0)))
                {
                    printDirectPlayError(hr);
                    return address;
                }

                char url[250];
                DWORD size = 250;
                addresses[0]->GetURLA(url, &size);
                address = QString(url);

                for (uint ii = 0; ii < dwNumAddresses; ++ii)
                {
                    LPDIRECTPLAY8ADDRESS pAddress = addresses[ii];
                    if (pAddress)
                    {
                        pAddress->Release();
                    }
                }
            }
            return address;
        }

        void CFs9Host::sendTextMessage(const QString &textMessage)
        {
            MPChatText mpChatText;
            mpChatText.chat_data = textMessage;
            QByteArray message;
            MultiPlayerPacketParser::writeType(message, CFs9Sdk::MPCHAT_PACKET_ID_CHAT_TEXT_SEND);
            MultiPlayerPacketParser::writeSize(message, mpChatText.chat_data.size() + 1);
            message = MultiPlayerPacketParser::writeMessage(message, mpChatText);
            qDebug() << "Message:" << textMessage;
            sendMessage(message);
        }

        void CFs9Host::initialize()
        {
            initDirectPlay();
            createHostAddress();
            startHosting(CProject::systemNameAndVersion(), m_callsign);
        }

        void CFs9Host::cleanup()
        {
            stopHosting();
        }

        HRESULT CFs9Host::startHosting(const QString &session, const QString &callsign)
        {
            HRESULT hr = S_OK;

            if (m_hostStatus == Hosting) return hr;

            DPN_APPLICATION_DESC dpAppDesc;

            QScopedArrayPointer<wchar_t> wszSession(new wchar_t[session.size() + 1]);
            QScopedArrayPointer<wchar_t> wszPlayername(new wchar_t[callsign.size() + 1]);

            session.toWCharArray(wszSession.data());
            wszSession[session.size()] = 0;
            callsign.toWCharArray(wszPlayername.data());
            wszPlayername[callsign.size()] = 0;

            PLAYER_INFO_STRUCT playerInfo;
            ZeroMemory(&playerInfo, sizeof(PLAYER_INFO_STRUCT));
            strcpy(playerInfo.szAircraft, "Boeing 737-400");


            playerInfo.dwFlags = PLAYER_INFO_STRUCT::PARAMS_RECV | PLAYER_INFO_STRUCT::PARAMS_SEND;

            // Prepare and set the player information structure.
            DPN_PLAYER_INFO player;
            ZeroMemory(&player, sizeof(DPN_PLAYER_INFO));
            player.dwSize = sizeof(DPN_PLAYER_INFO);
            player.pvData = &playerInfo;
            player.dwDataSize = sizeof(PLAYER_INFO_STRUCT);
            player.dwInfoFlags = DPNINFO_NAME | DPNINFO_DATA;
            player.pwszName = wszPlayername.data();
            if (FAILED(hr = m_directPlayPeer->SetPeerInfo(&player, nullptr, nullptr, DPNSETPEERINFO_SYNC)))
            {
                printDirectPlayError(hr);
                return hr;
            }

            // Now set up the Application Description
            ZeroMemory(&dpAppDesc, sizeof(DPN_APPLICATION_DESC));
            dpAppDesc.dwSize = sizeof(DPN_APPLICATION_DESC);
            dpAppDesc.guidApplication = CFs9Sdk::guid();
            dpAppDesc.pwszSessionName = wszSession.data();

            // We are now ready to host the app
            if (FAILED(hr = m_directPlayPeer->Host(&dpAppDesc,              // AppDesc
                                                   &m_deviceAddress, 1,       // Device Address
                                                   nullptr,
                                                   nullptr,                          // Reserved
                                                   nullptr,                          // Player Context
                                                   0)))                              // dwFlags
            {
                printDirectPlayError(hr);
                return hr;
            }
            else
            {
                BlackMisc::CLogMessage(this).info("Hosting successfully started");
                m_hostStatus = Hosting;
            }

            emit statusChanged(m_hostStatus);
            return hr;
        }

        HRESULT CFs9Host::stopHosting()
        {
            HRESULT hr = S_OK;

            if (m_hostStatus == Terminated) return hr;

            BlackMisc::CLogMessage(this).info("Hosting terminated!");
            if (FAILED(hr = m_directPlayPeer->TerminateSession(nullptr, 0, 0)))
            {
                return printDirectPlayError(hr);
            }

            if (FAILED(hr = m_directPlayPeer->Close(0)))
            {
                return printDirectPlayError(hr);
            }

            m_hostStatus = Terminated;

            emit statusChanged(m_hostStatus);
            return hr;
        }
    }
}
