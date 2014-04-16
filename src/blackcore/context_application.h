/* Copyright (C) 2013 VATSIM Community / authors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BLACKCORE_CONTEXTAPPLICATION_H
#define BLACKCORE_CONTEXTAPPLICATION_H

#include "blackcore/context.h"
#include "blackmisc/statusmessagelist.h"
#include <QObject>

#define BLACKCORE_CONTEXTAPPLICATION_INTERFACENAME "net.vatsim.PilotClient.BlackCore.ContextApplication"
#define BLACKCORE_CONTEXTAPPLICATION_OBJECTPATH "/Application"

namespace BlackCore
{
    /*!
     * \brief Application context interface
     */
    class IContextApplication : public CContext
    {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", BLACKCORE_CONTEXTAPPLICATION_INTERFACENAME)

    protected:
        //! Constructor
        IContextApplication(CRuntimeConfig::ContextMode mode, CRuntime *runtime);

    public:

        //! What output to redirect
        enum RedirectionLevel : uint
        {
            RedirectNone,
            RedirectAllOutput,
            RedirectWarningAndAbove,
            RedirectError
        };

        //! Components
        enum Component : uint
        {
            ComponentGui,
            ComponentCore
        };

        //! What a component does
        enum Actions : uint
        {
            ActionStarts,
            ActionStops
        };

        //! Service name
        static const QString &InterfaceName()
        {
            static QString s(BLACKCORE_CONTEXTAPPLICATION_INTERFACENAME);
            return s;
        }

        //! Service path
        static const QString &ObjectPath()
        {
            static QString s(BLACKCORE_CONTEXTAPPLICATION_OBJECTPATH);
            return s;
        }

        //! Destructor
        virtual ~IContextApplication() {}

        //! Output redirection (redirect my output)
        RedirectionLevel getOutputRedirectionLevel() const { return this->m_outputRedirectionLevel; }

        //! Output redirection (redirect my output)
        void setOutputRedirectionLevel(RedirectionLevel redirectionLevel) { this->m_outputRedirectionLevel = redirectionLevel; }

        //! Redirected output generated by others
        RedirectionLevel getStreamingForRedirectedOutputLevel() const { return this->m_redirectedOutputRedirectionLevel; }

        //! Redirected output generated by others
        void setStreamingForRedirectedOutputLevel(RedirectionLevel redirectionLevel) ;

        //! Process event, cross thread messages
        bool event(QEvent *event) override;

        //! Reset output redirection
        static void resetOutputRedirection();

    signals:
        //! \brief Status message
        //! \remarks not to be called directly, use IContextApplication::sendStatusMessage
        void statusMessage(const BlackMisc::CStatusMessage &message);

        //! Send status messages
        //! \remarks not to be called directly, use IContextApplication::sendStatusMessages
        void statusMessages(const BlackMisc::CStatusMessageList &messages);

        //! Redirect output streams as by qDebug(), qWarning(), qCritical()
        //! \remarks context mode is an important means to avoid infinite redirect loops
        //!          never output redirected stream messages from the same context again
        void redirectedOutput(const BlackMisc::CStatusMessage &message, qint64 contextId);

        //! A component changes
        void componentChanged(uint component, uint action);

    public slots:

        //! \brief Ping a token, used to check if application is alive
        virtual qint64 ping(qint64 token) const = 0;

        //! Status message
        virtual void sendStatusMessage(const BlackMisc::CStatusMessage &message) = 0;

        //! Send status messages
        virtual void sendStatusMessages(const BlackMisc::CStatusMessageList &messages) = 0;

        //! A component has changed its state
        virtual void notifyAboutComponentChange(uint component, uint action) = 0;

        //! Remote enabled version of writing a text file
        virtual bool writeToFile(const QString &fileName, const QString &content) = 0;

        //!  Remote enabled version of reading a text file
        virtual QString readFromFile(const QString &fileName) = 0;

        //!  Remote enabled version of deleting a file
        virtual bool removeFile(const QString &fileName) = 0;

    private:
        //! All contexts, used with messageHandler
        static QList<IContextApplication *> s_contexts;

        //! Previous message handler
        static QtMessageHandler s_oldHandler;

        //! Message handler, handles one individual context
        void messageHandler(QtMsgType type, const QMessageLogContext &messageContext, const QString &messsage);

        //! Handle output dispatch, handles all contexts
        static void messageHandlerDispatch(QtMsgType type, const QMessageLogContext &messageContext, const QString &message);

        RedirectionLevel m_outputRedirectionLevel; //!< enable / disable my output
        RedirectionLevel m_redirectedOutputRedirectionLevel; //!< enable / disable others output

    private slots:
        //! Re-stream the redirected output
        void streamRedirectedOutput(const BlackMisc::CStatusMessage &message, qint64 contextId);

    };
}

#endif // guard
