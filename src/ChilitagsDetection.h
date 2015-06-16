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
 * @file ChilitagsDetection.h
 * @brief QML wrapper for a Chilitags object
 * @author Ayberk Özgür
 * @author Quentin Bonnard
 * @author Lorenzo Lucignano (QAbstractVideoFilter implementation)
 * @version 1.0
 * @date 2014-10-10
 */

#ifndef CHILITAGSDETECTION_H
#define CHILITAGSDETECTION_H

#include<QQuickItem>
#include<QMatrix4x4>
#include<QMetaType>
#include <QVideoFilterRunnable>
#include <QOpenGLFramebufferObjectFormat>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QWindow>
#include <QOpenGLFramebufferObject>

#include <chilitags/chilitags.hpp>
#include "ChilitagsObject.h"
#include "ChilitagsThread.h"

Q_DECLARE_METATYPE(cv::Mat)

/**
 * @brief QML wrapper for a Chilitags object
 */
class ChilitagsDetection : public QAbstractVideoFilter{
Q_OBJECT
    Q_PROPERTY(QVariantMap tags READ getTags NOTIFY tagsChanged) //TODO: would a QMap<QString,QMatrix> do ?
    Q_PROPERTY(QMatrix4x4 projectionMatrix READ getProjectionMatrix NOTIFY projectionMatrixChanged)
    Q_PROPERTY(QString tagConfigurationFile WRITE setTagConfigurationFile)
    Q_PROPERTY(QQmlListProperty<ChilitagsObject> chiliobjects READ chiliobjects)
public:

    /**
     * @brief Creates a new Chilitags detector
     *
     * @param parent Parent of this QML item
     */
    explicit ChilitagsDetection(QQuickItem *parent = 0);

    /**
     * @brief Destroys this Chilitags detector
     */
    virtual ~ChilitagsDetection();

    /**
     * @brief Gets the detected tags
     *
     * @return Most recent detected tags
     */
    QVariantMap getTags() const;

    /**
     * @brief Gets the transform matrix that projects the pose onto the camera image
     *
     * @return Transform matrix that project the pose onto the camera image
     */
    QMatrix4x4 getProjectionMatrix() const;

    /**
     * @brief Reads a Chilitags tag configuration from the given qrc file
     *
     * @param tagConfigurationFile The qrc file, must begin with :/
     */
    void setTagConfigurationFile(QString tagConfigurationFile);

    //TODO: setDefaultTagSize
    //TODO: setFilter
    //TODO: setCalibration
    //TODO: setDetectionPeriod
    //TODO: setPerformance
    //TODO: setCornerRefinement
    //TODO: setMinInputWidth
    //TODO: We should not be exposing setConfig directly, the config is the QML "subtree" that is the children of ChilitagsDetection itself

    /**
     * @brief Factory function to create a new instance of a QVideoFilterRunnable.
     *        https://doc-snapshots.qt.io/qt5-dev/qabstractvideofilter.html
     *
     * @param
     */
    QVideoFilterRunnable* createFilterRunnable();
    QQmlListProperty<ChilitagsObject> chiliobjects();


signals:

    /**
     * @brief Emitted when a new image is processed
     *
     * @param tags The new set of tags
     */
    void tagsChanged(QVariantMap tags);

    void projectionMatrixChanged();

private slots:

    void setTags(chilitags::Chilitags3D_<qreal>::TagPoseMap tags);

private:

    QVariantMap tags;                   ///< Set of most recent tags and their poses wrt the camera, maps QString's to QMatrix4x4's

    QList<ChilitagsObject*> m_chiliobjects;
    //TODO: Recreate this with new size -- is it necessary?
    chilitags::Chilitags3D_<qreal> chilitags;   ///< The tag detector

};

#endif // CHILITAGSDETECTION_H
