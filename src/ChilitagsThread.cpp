/*
 * Copyright (C) 2014 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file ChilitagsThread.cpp
 * @brief Runs Chilitags3D.estimate() in a separate thread
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-10-14
 */

#include "ChilitagsThread.h"

//*****************************************************************************
// ChilitagsTask implementation
//*****************************************************************************

ChilitagsTask::ChilitagsTask(chilitags::Chilitags3D_<qreal>* chilitags)
{
    this->chilitags = chilitags;

    cvCamDeltaR(0) = 1;
    cvCamDeltaR(1) = 0;
    cvCamDeltaR(2) = 0;
    cvCamDeltaR(3) = 0;

    cvCamDeltaT(0) = 0;
    cvCamDeltaT(1) = 0;
    cvCamDeltaT(2) = 0;
}

ChilitagsTask::~ChilitagsTask()
{
}

void ChilitagsTask::presentFrame(cv::Mat frame){
    frameLock.lock();
    switch(state){

        //Invalid state, do not present
        case NONE:
            break;

        //Currently processing, copy buffer over the next frame
        case BUSY:
            nextFrame = frame.clone(); //TODO: We're screwed if this thing does not memcpy the buffer behind the curtains
            nextFrameAvailable = true;
            break;

        //Currently waiting, do not copy buffer but present directly
        case WAITING_FOR_FRAME:
            nextFrame = frame;
            nextFrameAvailable = true;
            nextFrameCond.wakeAll();
            break;
    }
    frameLock.unlock();
}

void ChilitagsTask::stop()
{
    running = false;
    nextFrameCond.wakeAll();
}

void ChilitagsTask::doWork()
{
    running = true;

#ifdef QT_DEBUG
    QElapsedTimer timer;
    float fps = 0.0f;
    int millisElapsed = 0;
    int millis;
    timer.start();
#endif

    frameLock.lock();
    while(running){

        //Process frame if possible
        if(nextFrameAvailable){

            //Signal that we're consuming the next frame now
            nextFrameAvailable = false;
            state = BUSY;

            //Unlock the lock so that we can present a new frame while getting IMU data and estimating
            frameLock.unlock();

            //Get angular displacement
            QMetaObject::invokeMethod(imu, "getAngularDisplacement", Qt::BlockingQueuedConnection,
                    Q_RETURN_ARG(QQuaternion, camDeltaR));
            cvCamDeltaR(0) = camDeltaR.scalar();
            cvCamDeltaR(1) = camDeltaR.x();
            cvCamDeltaR(2) = camDeltaR.y();
            cvCamDeltaR(3) = camDeltaR.z();

            //Get linear displacement
            QMetaObject::invokeMethod(imu, "getLinearDisplacement", Qt::BlockingQueuedConnection,
                    Q_RETURN_ARG(QVector3D, camDeltaT));
            cvCamDeltaT(0) = camDeltaT.x()*1000.0f; //Convert to millimeters
            cvCamDeltaT(1) = camDeltaT.y()*1000.0f; //Convert to millimeters
            cvCamDeltaT(2) = camDeltaT.z()*1000.0f; //Convert to millimeters

            //Reset displacement calculations for next iteration
            QMetaObject::invokeMethod(imu, "resetDisplacement", Qt::BlockingQueuedConnection);

            auto tags = chilitags->estimate(nextFrame, chilitags::Chilitags::ASYNC_DETECT_PERIODICALLY, cvCamDeltaR, cvCamDeltaT);
            frameLock.lock();

            emit tagsReady(tags);
        }

        //Wait for next frame
        state = WAITING_FOR_FRAME;
        //frameLock.unlock() is performed by wait below
        nextFrameCond.wait(&frameLock);
        //frameLock.lock() is performed by wait above

#ifdef QT_DEBUG
        millis = (int)timer.restart();
        millisElapsed += millis;
        fps = FPS_RATE*fps + (1.0f - FPS_RATE)*(1000.0f/millis);
        if(millisElapsed >= FPS_PRINT_PERIOD){
            qDebug("Chilitags is running at %f FPS",fps);
            millisElapsed = 0;
        }
#endif
    }
    state = NONE;
    frameLock.unlock();
}

void ChilitagsTask::setIMU(QObject* imu)
{
    this->imu = imu;
}

//*****************************************************************************
// ChilitagsThread implementation
//*****************************************************************************

ChilitagsThread::ChilitagsThread(chilitags::Chilitags3D_<qreal>* chilitags)
{
    task = new ChilitagsTask(chilitags);
    task->setIMU(imu);
    task->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), task, SLOT(doWork()));
    connect(
            task, SIGNAL(tagsReady(chilitags::Chilitags3D_<qreal>::TagPoseMap)),
            this, SIGNAL(tagsReady(chilitags::Chilitags3D_<qreal>::TagPoseMap)));
}

ChilitagsThread::~ChilitagsThread()
{
    stop();
    delete task;
}

void ChilitagsThread::start()
{
    workerThread.start();
}

void ChilitagsThread::stop()
{
    if(task != nullptr)
        task->stop();
    workerThread.quit();
    workerThread.wait();
}

void ChilitagsThread::presentFrame(cv::Mat frame)
{
    task->presentFrame(frame);
}

void ChilitagsThread::setIMU(QObject* imu)
{
    this->imu = imu;
    if(task != nullptr)
        task->setIMU(imu);
}

