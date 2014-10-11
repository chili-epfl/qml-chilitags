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
 * @file ChilitagsObject.cpp
 * @brief Chilitags object that corresponds to a real world object, e.g a tag
 * @author Ayberk Özgür
 * @author Quentin Bonnard
 * @version 1.0
 * @date 2014-10-10
 */

#ifndef ChilitagsObject_H
#define ChilitagsObject_H

#include <QQuickItem>
#include <QMatrix4x4>

#include "ChilitagsDetection.h"

/**
 * @brief Describes a Chilitags object that corresponds to a real world object that possibly carries an AR tag
 */
class ChilitagsObject : public QQuickItem
{
Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(bool visible READ getVisible NOTIFY visibilityChanged)
    Q_PROPERTY(QMatrix4x4 transform READ getTransform NOTIFY transformChanged)

public:

    /**
     * @brief Creates a new QML Chilitags object
     *
     * @param parent Parent of this QML item
     */
    explicit ChilitagsObject(QQuickItem* parent = 0);

    /**
     * @brief Gets the name of this Chilitags object, e.g "tag_0"
     *
     * @return Name of this tag
     */
    QString getName() const;

    /**
     * @brief Sets the name of this Chilitags object, e.g "tag_0"
     *
     * @param name New name of this Chilitags object
     */
    void setName(const QString& name);

    /**
     * @brief Gets whether this Chilitags object is seen in the camera image
     *
     * @return Whether this Chilitags object is seen in the camera image
     */
    bool getVisible() const;

    /**
     * @brief Gets the pose of this Chilitags object with respect to the camera
     *
     * @return Pose of this Chilitags object with respect to the camera
     */
    QMatrix4x4 getTransform() const;

signals:

    /**
     * @brief Emitted when this Chilitags object enters or exits the scene
     *
     * @param visible Whether this Chilitags object is visible now
     */
    void visibilityChanged(bool visible);

    /**
     * @brief Emitted when the pose of this Chilitags object changes
     *
     * @param transform New pose of this Chilitags object
     */
    void transformChanged(QMatrix4x4 transform);

public slots:

    /**
     * @brief Updates the status of this tag
     *
     * @param tags The new list of detections, maps QString's to QMatrix4x4's
     */
    void updateTag(const QVariantMap& tags);

    /**
     * @brief Sets a new parent to this Chilitags object
     *
     * @param parent The new parent, must be ChilitagsDetection
     */
    void changeParent(QQuickItem* parent);

private:

    QString name;           ///< Name of this Chilitags object, e.g "tag_0"
    bool visible;           ///< Whether this object is currently visible in the scene
    QMatrix4x4 transform;   ///< Pose of this object with respect to the camera
};

#endif
