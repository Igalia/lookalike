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


#include "facerecognitionresetter.h"

#ifdef Q_OS_LINUX
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#endif // Q_OS_LINUX

#include <QDir>


static const char* xdg_data_home = getenv("XDG_DATA_HOME");
static const char* data_root_dir = GALLERYCORE_DATA_ROOT_DIR;
static const char* data_data_dir = GALLERYCORE_DATA_DATA_DIR;
static const char* database_filename = FACE_RECOGNITION_DATABASE_FILENAME;
static const char* database_journal_filename = FACE_RECOGNITION_DATABASE_FILENAME"-journal";
static const char* core_user = GALLERYCORE_USER;
static const char* core_group = GALLERYCORE_GROUP;
static const char* regular_user = REGULAR_USER;
static const char* regular_group = REGULAR_GROUP;

FaceRecognitionResetter::FaceRecognitionResetter() :
    QObject()
{
}

void FaceRecognitionResetter::dealWithDB(int dealType) const
{
    QString infoText;
    bool success = false;

    switch (dealType) {
    case 0:
        success = deleteDB(infoText);
        break;
    case 1:
        success = protectDB(true, infoText);
        break;
    case 2:
        success = protectDB(false, infoText);
        break;
    default:
        Q_ASSERT(false);
    }
}

bool FaceRecognitionResetter::isDBCorrected() const
{
    bool result = true;


    QDir dir(xdg_data_home);
    if (!dir.exists(data_root_dir)) {
        goto clean;
    }

    dir.cd(data_root_dir);
    if (!dir.exists(data_data_dir)) {
        goto clean;
    }

    dir.cd(data_data_dir);
    if (dir.exists(database_filename)) {
        QFile::Permissions filePermissions =
            QFile::permissions(dir.absoluteFilePath(database_filename));

        result = filePermissions.testFlag(QFile::ReadOwner)
            && filePermissions.testFlag(QFile::WriteOwner);
        goto clean;
    } else {
        goto clean;
    }


clean:

    return result;
}

bool FaceRecognitionResetter::deleteDB(QString &infoText) const
{
    bool result = false;

    infoText = QString("");

    QDir dir(xdg_data_home);
    if (!dir.exists(data_root_dir)) {
        infoText.append("Directory didn't exist beforehand.");
        goto clean;
    }

    dir.cd(data_root_dir);
    if (!dir.exists(data_data_dir)) {
        infoText.append("Directory didn't exist beforehand.");
        goto clean;
    }

    dir.cd(data_data_dir);
    if (dir.exists(database_filename)) {
        if (!dir.remove(database_filename)) {
            infoText.append("Failed deleting the DB.");
            return result;
        } else {
            infoText.append("Succeeded deleting the DB.");
            result = true;
        }
    } else {
        infoText.append("DB didn't exist beforehand.");
    }

    if (dir.exists(database_journal_filename)) {
        if (!dir.remove(database_journal_filename)) {
            infoText.append("Failed deleting the DB journal.");
            result = false;
            return result;
        } else {
            infoText.append("Succeeded deleting the DB journal.");
            result = true;
        }
    } else {
        infoText.append(" DB journal didn't exist beforehand.");
    }

    dir.cdUp();
    if (!dir.rmdir(data_data_dir)) {
        infoText.append(" Failed deleting the parent directory.");
        return result;
    }

    infoText.append(" Succeeded deleting the parent directory.");


clean:

    return result;
}

bool FaceRecognitionResetter::protectDB(bool protect, QString &infoText) const
{
    bool result = false;
    struct passwd *pwd = 0;
    struct group *grd = 0;
    mode_t mode = 0;
    QFile::Permissions directoryPermissions;
    QFile::Permissions filePermissions;

    if (protect) {

#ifdef Q_OS_LINUX

        pwd = getpwnam(core_user);
        grd = getgrnam(core_group);
        mode = S_ISGID | S_IRGRP | S_IWGRP | S_IXGRP;

#endif // Q_OS_LINUX

        directoryPermissions = QFile::ReadGroup
            | QFile::WriteGroup
            | QFile::ExeGroup;
        filePermissions = QFile::ReadOwner
            | QFile::WriteOwner
            | QFile::ReadGroup
            | QFile::WriteGroup;
    } else {

#ifdef Q_OS_LINUX

        pwd = getpwnam(regular_user);
        grd = getgrnam(regular_group);
        mode = S_IRUSR | S_IWUSR | S_IXUSR
            | S_IRGRP | S_IWGRP | S_IXGRP
            | S_IROTH | S_IWOTH | S_IXOTH;

#endif // Q_OS_LINUX

        directoryPermissions = QFile::ReadOwner
            | QFile::WriteOwner
            | QFile::ExeOwner
            | QFile::ReadGroup
            | QFile::WriteGroup
            | QFile::ExeGroup
            | QFile::ReadOther
            | QFile::WriteOther
            | QFile::ExeOther;
        filePermissions = QFile::ReadOwner
            | QFile::WriteOwner
            | QFile::ReadGroup
            | QFile::WriteGroup
            | QFile::ReadOther
            | QFile::WriteOther;
    }

    infoText = QString("");

    QDir dir(xdg_data_home);
    if (!dir.exists(data_root_dir)) {
        infoText.append("Directory didn't exist beforehand.");
        goto clean;
    }

    dir.cd(data_root_dir);
    if (!dir.exists(data_data_dir)) {
        infoText.append("Directory didn't exist beforehand.");
        goto clean;
    }

#ifdef Q_OS_LINUX

    if (NULL != pwd) {
        if (NULL != grd) {
            if (0 == chown(dir.absoluteFilePath(data_data_dir).toUtf8().constData(),
                           pwd->pw_uid,
                           grd->gr_gid)) {
                infoText.append("Owner of the parent directory changed.");
            } else {
                infoText.append("Failed changing owner of the parent directory.");
                goto clean;
            }
        }
    }

    if (0 == chmod(dir.absoluteFilePath(data_data_dir).toUtf8().constData(),
                   mode)) {
        infoText.append(" Permissions of the parent directory changed.");
    } else {
        infoText.append(" Failed changing the permissions of the parent directory.");
        goto clean;
    }

#else

    if (QFile::setPermissions(dir.absoluteFilePath(data_data_dir),
                              directoryPermissions)) {
        infoText.append(" Permissions of the parent directory changed.");
    } else {
        infoText.append(" Failed changing the permissions of the parent directory.");
        goto clean;
    }

#endif // Q_OS_LINUX

    dir.cd(data_data_dir);
    if (dir.exists(database_filename)) {

#ifdef Q_OS_LINUX

        if (NULL != pwd) {
            if (NULL != grd) {
                if (0 == chown(dir.absoluteFilePath(database_filename).toUtf8().constData(),
                               pwd->pw_uid,
                               grd->gr_gid)) {
                    infoText.append(" Owner of the DB changed.");
                } else {
                    infoText.append(" Failed changing the owner of the DB.");
                    goto clean;
                }
            }
        }

#endif // Q_OS_LINUX

        if (QFile::setPermissions(dir.absoluteFilePath(database_filename),
                                  filePermissions)) {
            infoText.append(" Permissions of the DB changed.");
            result = true;
        } else {
            infoText.append(" Failed changing the permissions of the DB.");
            goto clean;
        }

    } else {
        infoText.append(" DB didn't exist beforehand.");
        goto clean;
    }

    // Journal file
    if (dir.exists(database_journal_filename)) {

#ifdef Q_OS_LINUX

        if (NULL != pwd) {
            if (NULL != grd) {
                if (0 == chown(dir.absoluteFilePath(database_journal_filename).toUtf8().constData(),
                               pwd->pw_uid,
                               grd->gr_gid)) {
                    infoText.append(" Owner of the DB journal changed.");
                } else {
                    infoText.append(" Failed changing the owner of the DB journal.");
                    goto clean;
                }
            }
        }

#endif // Q_OS_LINUX

        if (QFile::setPermissions(dir.absoluteFilePath(database_journal_filename),
                                  filePermissions)) {
            infoText.append(" Permissions of the DB journal changed.");
        } else {
            infoText.append(" Failed changing the permissions of the DB journal.");
            goto clean;
        }

    } else {
        infoText.append(" DB journal didn't exist beforehand.");
        goto clean;
    }


clean:

    return result;
}

