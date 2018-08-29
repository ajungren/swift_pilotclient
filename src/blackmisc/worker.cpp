/* Copyright (C) 2014
 * swift Project Community / Contributors
 *
 * This file is part of swift project. It is subject to the license terms in the LICENSE file found in the top-level
 * directory of this distribution and at http://www.swift-project.org/license.html. No part of swift project,
 * including this file, may be copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE file.
 */

#include "blackmisc/threadutils.h"
#include "blackmisc/worker.h"
#include "blackmisc/verify.h"

#include <future>
#include <QTimer>
#include <QPointer>
#include <QCoreApplication>

#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

namespace BlackMisc
{
    void CRegularThread::run()
    {
#ifdef Q_OS_WIN32
        m_handle = GetCurrentThread();
        QThread::run();
        m_handle = nullptr;
#else
        QThread::run();
#endif
    }

    CRegularThread::~CRegularThread()
    {
#ifdef Q_OS_WIN32
        auto handle = m_handle.load();
        if (handle)
        {
            const auto status = WaitForSingleObject(handle, 0);
            if (isRunning())
            {
                switch (status)
                {
                default:
                case WAIT_FAILED: qWarning() << "Thread" << objectName() << "unspecified error"; break;
                case WAIT_OBJECT_0: qWarning() << "Thread" << objectName() << "unsafely terminated by program shutdown"; break;
                case WAIT_TIMEOUT: break;
                }
            }
        }
#endif
        quit();

        const qint64 beforeWait = QDateTime::currentMSecsSinceEpoch();
        wait(30 * 1000); //! \todo KB 2017-10 temp workaround: in T145 this will be fixed, sometimes (very rarely) hanging here during shutdown
        const qint64 delta = QDateTime::currentMSecsSinceEpoch() - beforeWait;
        BLACK_VERIFY_X(delta < 30 * 1000, Q_FUNC_INFO, "Wait timeout");
        Q_UNUSED(delta);
    }

    CWorker *CWorker::fromTaskImpl(QObject *owner, const QString &name, int typeId, std::function<CVariant()> task)
    {
        auto *worker = new CWorker(task);
        emit worker->aboutToStart();
        worker->setStarted();
        auto *thread = new CRegularThread(owner);

        if (typeId != QMetaType::Void) { worker->m_result = CVariant(typeId, nullptr); }

        const QString ownerName = owner->objectName().isEmpty() ? owner->metaObject()->className() : owner->objectName();
        thread->setObjectName(ownerName + ":" + name);
        worker->setObjectName(name);

        worker->moveToThread(thread);
        const bool s = QMetaObject::invokeMethod(worker, "ps_runTask");
        Q_ASSERT_X(s, Q_FUNC_INFO, "cannot invoke");
        Q_UNUSED(s);
        thread->start();
        return worker;
    }

    void CWorker::ps_runTask()
    {
        m_result = m_task();

        this->setFinished();

        auto *ownThread = thread();
        this->moveToThread(ownThread->thread()); // move worker back to the thread which constructed it, so there is no race on deletion

        //! \todo KB 2018-97 new syntax not yet supported on Jenkins QMetaObject::invokeMethod(ownThread, &CWorker::deleteLater)
        QMetaObject::invokeMethod(ownThread, "deleteLater");
        QMetaObject::invokeMethod(this, "deleteLater");
    }

    const CLogCategoryList &CWorkerBase::getLogCategories()
    {
        static const BlackMisc::CLogCategoryList cats { CLogCategory::worker() };
        return cats;
    }

    void CWorkerBase::waitForFinished() noexcept
    {
        std::promise<void> promise;
        then([ & ] { promise.set_value(); });
        promise.get_future().wait();
    }

    void CWorkerBase::abandon() noexcept
    {
        thread()->requestInterruption();
        quit();
    }

    void CWorkerBase::abandonAndWait() noexcept
    {
        thread()->requestInterruption();
        quitAndWait();
    }

    bool CWorkerBase::isAbandoned() const
    {
        return thread()->isInterruptionRequested();
    }

    CContinuousWorker::CContinuousWorker(QObject *owner, const QString &name) :
        CIdentifiable(name),
        m_owner(owner), m_name(name)
    {
        Q_ASSERT_X(!name.isEmpty(), Q_FUNC_INFO, "Empty name");
        this->setObjectName(m_name);
        m_updateTimer.setObjectName(m_name + ":timer");
    }

    void CContinuousWorker::start(QThread::Priority priority)
    {
        BLACK_VERIFY_X(!hasStarted(), Q_FUNC_INFO, "Tried to start a worker that was already started");
        if (hasStarted()) { return; }

        // avoid message "QObject: Cannot create children for a parent that is in a different thread"
        Q_ASSERT_X(CThreadUtils::isCurrentThreadObjectThread(m_owner), Q_FUNC_INFO, "Needs to be started in owner thread");
        emit this->aboutToStart();
        setStarted();
        auto *thread = new CRegularThread(m_owner);

        Q_ASSERT(m_owner); // must not be null, see (9) https://dev.vatsim-germany.org/issues/402
        if (m_owner)
        {
            const QString ownerName = m_owner->objectName().isEmpty() ? m_owner->metaObject()->className() : m_owner->objectName();
            thread->setObjectName(ownerName + ": " + m_name);
        }

        moveToThread(thread);
        connect(thread, &QThread::started, this, &CContinuousWorker::initialize);
        connect(thread, &QThread::finished, &m_updateTimer, &QTimer::stop);
        connect(thread, &QThread::finished, this, &CContinuousWorker::cleanup);
        connect(thread, &QThread::finished, this, &CContinuousWorker::finish);
        thread->start(priority);
    }

    void CContinuousWorker::quit() noexcept
    {
        this->setEnabled(false);

        // already in different thread? otherwise return
        if (CThreadUtils::isApplicationThreadObjectThread(this)) { return; }

        // remark: cannot stop timer here, as I am normally not in the correct thread
        thread()->quit();
    }

    void CContinuousWorker::quitAndWait() noexcept
    {
        this->quit();

        // already in application (main) thread? => return
        if (CThreadUtils::isApplicationThreadObjectThread(this)) { return; }

        // called by own thread, will deadlock, return
        if (CThreadUtils::isCurrentThreadObjectThread(this)) { return; }
        QThread *ownThread = thread();

        const qint64 beforeWait = QDateTime::currentMSecsSinceEpoch();
        ownThread->wait(30 * 1000); //! \todo KB 2017-10 temp workaround: in T145 this will be fixed, sometimes (very rarely) hanging here during shutdown
        const qint64 delta = QDateTime::currentMSecsSinceEpoch() - beforeWait;
        BLACK_VERIFY_X(delta < 30 * 1000, Q_FUNC_INFO, "Wait timeout");
        Q_UNUSED(delta);
    }

    void CContinuousWorker::startUpdating(int updateTimeSecs)
    {
        Q_ASSERT_X(hasStarted(), Q_FUNC_INFO, "Worker not yet started");
        if (!CThreadUtils::isCurrentThreadObjectThread(this))
        {
            // shift in correct thread
            if (!this->isFinished())
            {
                QPointer<CContinuousWorker> myself(this);
                QTimer::singleShot(0, this, [ = ]
                {
                    if (!myself) { return; }
                    if (this->isFinished()) { return; }
                    this->startUpdating(updateTimeSecs);
                });
            }
            return;
        }

        // here in correct timer thread
        if (updateTimeSecs < 0)
        {
            this->setEnabled(false);
            m_updateTimer.stop();
        }
        else
        {
            this->setEnabled(true);
            m_updateTimer.start(1000 * updateTimeSecs);
        }
    }

    void CContinuousWorker::finish()
    {
        this->setFinished();

        QPointer<QThread> pOwnThreadBeforeMove(this->thread()); // thread before moved back
        QPointer<CContinuousWorker> myself(this);

        if (!m_owner || (pOwnThreadBeforeMove.data() != m_owner->thread()))
        {
            if (m_owner)
            {
                // we have a owner, and the owner thread is not the same as the worker thread
                // move worker back to the thread which constructed it, so there is no race on deletion
                this->moveToThread(m_owner->thread());
                Q_ASSERT_X(this->thread() == m_owner->thread(), Q_FUNC_INFO, "Thread mismatch owner");
            }
            else if (qApp && qApp->thread())
            {
                // no owner, we move to main thread
                this->moveToThread(qApp->thread());
                Q_ASSERT_X(this->thread() == qApp->thread(), Q_FUNC_INFO, "Thread mismatch qApp");
            }

            // if thread still exists and is not the main thread, delete it
            if (pOwnThreadBeforeMove && !CThreadUtils::isApplicationThread(pOwnThreadBeforeMove.data()))
            {
                // delete the original "background" thread
                QTimer::singleShot(0, pOwnThreadBeforeMove.data(), [ = ]
                {
                    if (pOwnThreadBeforeMove) { pOwnThreadBeforeMove->deleteLater(); }
                });
            }
        }
        else
        {
            // getting here would mean we have a owner and the owner thread is the same as the work thread
            const QString msg = QString("Owner thread: '%1' own thread: '%2'").arg(CThreadUtils::threadInfo(m_owner->thread()), CThreadUtils::threadInfo(pOwnThreadBeforeMove.data()));
            BLACK_VERIFY_X(false, Q_FUNC_INFO, msg.toLatin1().constData());
        }

        if (myself)
        {
            // if worker still exists delete it
            QTimer::singleShot(0, myself.data(), [ = ]
            {
                if (myself) { myself->deleteLater(); }
            });
        }
    }
} // ns
