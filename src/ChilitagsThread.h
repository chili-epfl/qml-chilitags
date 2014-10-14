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
 * @file ChilitagsThread.h
 * @brief Runs Chilitags3D.estimate() in a separate thread
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2014-10-14
 */

#ifndef CHILITAGSTHREAD_H
#define CHILITAGSTHREAD_H

#include<QDebug>
#include<QThread>
#include<QObject>
#include<QElapsedTimer>
#include<QMutex>
#include<QWaitCondition>

#include<opencv2/core.hpp>

#include<chilitags/chilitags.hpp>

#include<string>
#include<map>

typedef std::map<std::string,cv::Matx44d> Str2TransformMap;
Q_DECLARE_METATYPE(Str2TransformMap)

/**
 * @brief Object that contains the Chilitags3D.estimate() loop and its parameters
 */
class ChilitagsTask : public QObject{
Q_OBJECT

public:

    /**
     * @brief Creates a new Chilitags task
     *
     * @param chilitags Tag detector, must be already initialized
     */
    ChilitagsTask(chilitags::Chilitags3D* chilitags);

    /**
     * @brief Destroys this camera access task, does not touch the camera or the videoFrame
     */
    virtual ~ChilitagsTask();

    /**
     * @brief Presents a new frame to be processed by Chilitags
     *
     * If the thread is waiting for a frame, presents the frame directly.
     * If the thread is busy, copies the frame as the next frame to be processed.
     * Either way, does not busy wait and returns immediately.
     *
     * @param frame The new frame
     */
    void presentFrame(cv::Mat frame);

    /**
     * @brief Asks for the main loop to stop
     */
    void stop();

public slots:

    /**
     * @brief Continuously gets data from the camera
     */
    void doWork();

signals:

    /**
     * @brief Emitted when Chilitags3D.estimate() returns some tags
     */
    void tagsReady(Str2TransformMap tags);

private:

#ifdef QT_DEBUG
    const float FPS_RATE = 0.9f;            ///< Rate of using the older FPS estimates
    const int FPS_PRINT_PERIOD = 500;       ///< Period of printing the FPS estimate, in milliseconds
#endif

    /**
     * @brief Thread's possible states
     */
    enum State{
        NONE,               ///< Invalid state, new frames will not be presented
        BUSY,               ///< Currently busy with Chilitags3D.estimate()
        WAITING_FOR_FRAME   ///< Waiting for a new frame
    };

    chilitags::Chilitags3D* chilitags;  ///< Tag detector
    QMutex frameLock;                   ///< Mutex that locks the frame transaction
    QWaitCondition nextFrameCond;       ///< Condition to wait on until the next frame arrives
    State state = NONE;                 ///< Thread's current state

    //TODO: PREALLOCATE A BUFFER FOR THIS!!!!!
    cv::Mat nextFrame;                  ///< The frame that will be processed next

    bool nextFrameAvailable = false;    ///< Whether the next frame is ready and in place
    bool running = false;               ///< Whether the should keep running, we don't need a mutex for this
};

/**
 * @brief Object that starts and stops the loop that calls Chilitags3D.estimate()
 */
class ChilitagsThread : public QObject{
Q_OBJECT

public:

    /**
     * @brief Creates a new Chilitags thread controller
     *
     * @param chilitags Tag detector, must be already initialized
     */
    ChilitagsThread(chilitags::Chilitags3D* chilitags);

    /**
     * @brief Destroys this Chilitags thread controller
     */
    virtual ~ChilitagsThread();

    /**
     * @brief Starts the Chilitags loop
     */
    void start();

    /**
     * @brief Asks the Chilitags loop to stop and waits until it stops
     */
    void stop();

public slots:

    /**
     * @brief Presents a new frame to be processed by Chilitags
     *
     * If the thread is waiting for a frame, presents the frame directly.
     * If the thread is busy, copies the frame as the next frame to be processed.
     * Either way, does not busy wait and returns immediately.
     *
     * @param frame The new frame
     */
    void presentFrame(cv::Mat frame);

signals:

    /**
     * @brief Emitted when Chilitags3D.estimate() returns some tags
     */
    void tagsReady(Str2TransformMap tags);

private:

    QThread workerThread;               ///< The thread that Chilitags will work in
    ChilitagsTask* task = NULL;         ///< The loop method and parameter container
};

#endif /* CHILITAGSTHREAD_H */

