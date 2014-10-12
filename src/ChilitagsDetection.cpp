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
    QQuickItem(parent)
{}

ChilitagsDetection::~ChilitagsDetection()
{}

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
            projectionMatrix(i,j) = mat.at<double>(i,j);
    projectionMatrix(3,2) = 1;

    return projectionMatrix;
}

void ChilitagsDetection::setSourceImage(QVariant sourceImage)
{
    cv::Mat sourceMat = sourceImage.value<cv::Mat>();

    //TODO: MOVE THIS TO ITS OWN THREAD!!!!!!!!!!!!!
    //{
    auto stlTags = chilitags.estimate(sourceMat);
    tags.clear();
    for (auto tag : stlTags) {
        // TODO: manual conversion from float[16] to double[16], remove this when everything in Chilitags is float
        float values[16];
        for (int i = 0; i<16; ++i) values[i] = tag.second.val[i];
        tags.insert(QString::fromStdString(tag.first), QMatrix4x4(values)); //TODO: float* cast to qreal* which is double* typedef on desktop, WTF actually happens here to make it work?
    }
    emit tagsChanged(tags);
    //}

}

