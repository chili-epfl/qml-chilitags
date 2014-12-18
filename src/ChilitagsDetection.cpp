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
 * @file ChilitagsDetection.cpp
 * @brief Implementation of the QML wrapper for a Chilitags object
 * @author Ayberk Özgür
 * @author Quentin Bonnard
 * @version 1.0
 * @date 2014-10-10
 */

#include "ChilitagsDetection.h"

ChilitagsDetection::ChilitagsDetection(QQuickItem *parent) :
    QQuickItem(parent),
    chilitags(),
    thread(&chilitags)
{
    qRegisterMetaType<chilitags::Chilitags3D_<qreal>::TagPoseMap>("TagPoseMap");
    connect(&thread,SIGNAL(tagsReady(chilitags::Chilitags3D_<qreal>::TagPoseMap)),
            this,SLOT(setTags(chilitags::Chilitags3D_<qreal>::TagPoseMap)));
    thread.start();
}

ChilitagsDetection::~ChilitagsDetection()
{
    thread.stop();
}

QVariantMap ChilitagsDetection::getTags() const
{
    return tags;
}

QMatrix4x4 ChilitagsDetection::getProjectionMatrix() const
{
    cv::Mat mat = chilitags.getCameraMatrix();
    QMatrix4x4 projectionMatrix;

    projectionMatrix.fill(0);
    for (int i = 0; i<3; ++i)
        for (int j = 0; j<3; ++j)
            projectionMatrix(i,j) = mat.at<float>(i,j);
    projectionMatrix(3,2) = 1;

    return projectionMatrix;
}

void ChilitagsDetection::setDetectionTrigger(DetectionTrigger trigger)
{
    thread.setDetectionTrigger((chilitags::Chilitags::DetectionTrigger)trigger);
}

void ChilitagsDetection::setSourceImage(QVariant sourceImage)
{
    thread.presentFrame(sourceImage.value<cv::Mat>());
}

void ChilitagsDetection::setTags(chilitags::Chilitags3D_<qreal>::TagPoseMap stlTags)
{
    tags.clear();
    for (auto tag : stlTags) {
        // TODO: manual conversion from float[16] to double[16], remove this when everything in Chilitags is float
        float values[16];
        for (int i = 0; i<16; ++i) values[i] = tag.second.val[i];
        tags.insert(QString::fromStdString(tag.first), QMatrix4x4(values)); //TODO: float* cast to qreal* which is double* typedef on desktop, WTF actually happens here to make it work?
    }
    emit tagsChanged(tags);
}

void ChilitagsDetection::setTagConfigurationFile(QString tagConfigurationFile)
{
    QFile configFile(tagConfigurationFile);
    configFile.open(QFile::ReadOnly);
    QTextStream inStream(&configFile);
    chilitags.readTagConfiguration(inStream.readAll().toStdString(),false,true);
}

void ChilitagsDetection::setIMU(QObject* imu)
{
    thread.setIMU(imu);
}

void ChilitagsDetection::setPersistence(qreal persistence)
{
    chilitags.setPersistence(persistence);
}

