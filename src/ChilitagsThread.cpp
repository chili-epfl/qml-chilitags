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
 * @author Lorenzo Lucignano (QVideoFilterRunnable implementation)
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

            //Unlock the lock so that we can present a new frame while it's estimating
            frameLock.unlock();
            auto tags = chilitags->estimate(nextFrame);
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

//*****************************************************************************
// ChilitagsThread implementation
//*****************************************************************************

ChilitagsThread::ChilitagsThread(chilitags::Chilitags3D_<qreal>* chilitags)
{
    task = new ChilitagsTask(chilitags);
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
    if(task != NULL){
        task->stop();
    }
    workerThread.quit();
    workerThread.wait();
}


QVideoFrame ChilitagsThread::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags){
    auto pixelFormat=surfaceFormat.pixelFormat();
    auto handle=input->handleType();
    //This case
    switch (handle) {
    case QAbstractVideoBuffer::NoHandle: ///< Mainly desktop configuration
        if(input->map(QAbstractVideoBuffer::ReadOnly)){
            cv::Mat mat;
            if( QVideoFrame::imageFormatFromPixelFormat(pixelFormat)!=QImage::Format_Invalid ){
                QImage img( input->bits(),
                            input->width(),
                            input->height(),
                            input->bytesPerLine(),
                            QVideoFrame::imageFormatFromPixelFormat(pixelFormat));
                img=img.convertToFormat(QImage::Format_Grayscale8);
                mat=cv::Mat(img.height(), img.width(), CV_8UC1,
                            const_cast<uchar*>(img.constBits()), img.bytesPerLine()).clone();
                task->presentFrame(mat);
            }
            else if (pixelFormat == QVideoFrame::PixelFormat::Format_YUV420P ||
                     pixelFormat == QVideoFrame::PixelFormat::Format_NV21 ||
                     pixelFormat == QVideoFrame::PixelFormat::Format_NV12
                     ) {
                mat=cv::Mat(input->height(), input->width(),CV_8UC1 ,
                            const_cast<uchar*>(input->bits())).clone();
                task->presentFrame(mat);
            }
            else{
                qWarning()<<" Handle: NoHandle, unsupported pixel format:"<<pixelFormat;
            }
            input->unmap();
        }
        else{
            qWarning()<<"Cannot map video buffer";
        }
        break;
    case QAbstractVideoBuffer::GLTextureHandle:
        if ( pixelFormat == QVideoFrame::Format_BGR32){
            QImage img(input->width(),input->height(),QImage::Format_RGBA8888);
            GLuint textureId = input->handle().value<GLuint>();
            QOpenGLContext *ctx = QOpenGLContext::currentContext();
            QOpenGLFunctions *f = ctx->functions();
            //GLuint fbo;
            if(fbo==0)
                f->glGenFramebuffers(1, &fbo);
            GLuint prevFbo;
            f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
            f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
            f->glPixelStorei(GL_PACK_ALIGNMENT,4);
            f->glReadPixels(0, 0, input->width(), input->height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
            f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
            cv::Mat mat=cv::Mat(img.height(), img.width(), CV_8UC4,
                                const_cast<uchar*>(img.constBits()), img.bytesPerLine()).clone();
            task->presentFrame(mat);
        }
        else{
            qWarning()<<"Handle: GLTextureHandle, unsupported pixel format:"<<pixelFormat;
        }
        break;
    default:
        qWarning()<<"Unsupported Video Frame Handle:"<<handle;
        break;
    }

    return *input;
}

