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
 * @file ChilitagsPlugin.cpp
 * @brief Implementation of the object that exposes the CVCamera plugin components as QML objects
 * @author Ayberk Özgür
 * @author Quentin Bonnard
 * @version 1.0
 * @date 2014-10-10
 */

#include "ChilitagsPlugin.h"

void ChilitagsPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<ChilitagsDetection>(uri, 1, 0, "Chilitags");
    qmlRegisterType<ChilitagsObject>(uri, 1, 0, "ChilitagsObject");
    qmlRegisterType<MatrixTransform>(uri, 1, 0, "MatrixTransform");
#ifdef QT_3D_LIB
    qmlRegisterType<MatrixTransform3D>(uri, 1, 0, "MatrixTransform3D");
#endif
}

