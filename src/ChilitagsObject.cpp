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
 * @brief Implementation of a Chilitags object that corresponds to e.g a tag
 * @author Ayberk Özgür
 * @author Quentin Bonnard
 * @version 1.0
 * @date 2014-10-10
 */

#include "ChilitagsObject.h"

ChilitagsObject::ChilitagsObject(QQuickItem* parent) :
    QQuickItem(parent)
{
    //FIXME: the parentChanged signal is the only reason
    // that we need ChilitagsObject to be a QQuickItem
    // ... maybe there is a better way
    //connect(this, &ChilitagsDetection::parentChanged, this, &ChilitagsObject::changeParent);
    if(parent)
        connect((ChilitagsDetection*) parent, &ChilitagsDetection::tagsChanged, this, &ChilitagsObject::updateTag);
}

QString ChilitagsObject::getName() const
{
    return name;
}

void ChilitagsObject::setName(const QString& name)
{
    this->name = name;
}

bool ChilitagsObject::getVisible() const
{
    return visible;
}

QMatrix4x4 ChilitagsObject::getTransform() const
{
    return transform;
}

void ChilitagsObject::updateTag(const QVariantMap& tags)
{
    auto me = tags.find(name);
    bool wasVisible = visible;
    visible = (me != tags.end());
    if(wasVisible != visible)
        emit visibilityChanged();
    if(visible) {
        transform = me->value<QMatrix4x4>();
        emit transformChanged();
    }
}

void ChilitagsObject::changeParent(QQuickItem *parent)
{
    //FIXME: we probably need to disconnect the previous parent, don't we ?
    if(parent)
        connect((ChilitagsDetection*) parent, &ChilitagsDetection::tagsChanged, this, &ChilitagsObject::updateTag);
}

