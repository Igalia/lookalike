/*
 * This file is part of LookALike
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: agomez@igalia.com
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

#include <QtCore/QCoreApplication>
#include "mytest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyTest *t = new MyTest();
    QImage i("/home/jasuarez/MyDocs/12020001.jpg");
    t->connect(t->recognizer, SIGNAL(facesDetected(QString,QImage,QSize,QList<QRect>)), t, SLOT(onFacesDetected(QString,QImage,QSize,QList<QRect>)));
    t->connect(t->recognizer, SIGNAL(facesRecognized(QString,QImage,QList<XQFaceRegion>)), t, SLOT(onFacesRecognized(QString,QImage,QList<XQFaceRegion>)));
    //t->recognizer->detectFaces("t1", i);
    t->recognizer->processFaces("urn:uuid:15fb1a3d-cfe7-ebb7-a110-bdc0c6968c8f", i);
    return a.exec();

    XQFaceGroup group = t->db->faceGroup("Jota");
    QAbstractItemModel *m = group.faces();
    int s = m->rowCount();
    qDebug() << s;
    QAbstractItemModel *m2 = t->db->faceGroups(XQFaceDatabase::UnnamedGroup);
    QString f = m2->index(0,0,QModelIndex()).data().toString();
    qDebug() << f;
    group = t->db->faceGroup(f);
    qDebug() << group.faceId();
    m = group.faces();
    f = m->index(0,0,QModelIndex()).data().toString();
    XQFaceRegion region = group.faceRegion(f);
    qDebug() << region.sourceId();
    qDebug() << region.faceIds().size();
    qDebug() << region.faceIds().at(0);
    s = m2->rowCount();
    qDebug() << s;
    return a.exec();
}
