/*
 * This file is part of LookAlike
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Andres Gomez <agomez@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */


#ifndef FACERECOGNITIONRESETTER_H
#define FACERECOGNITIONRESETTER_H

#include <QObject>

class FaceRecognitionResetter : public QObject
{
    Q_OBJECT
public:
    explicit FaceRecognitionResetter();

public Q_SLOTS:
    void dealWithDB(int dealType) const;
    bool isDBCorrected() const;

private:
    bool deleteDB(QString &infoText) const;
    bool protectDB(bool protect, QString &infoText) const;
};

#endif // FACERECOGNITIONRESETTER_H
