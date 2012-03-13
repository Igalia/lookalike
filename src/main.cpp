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

#include "lookalikemain.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MComponentCache>
#include <MExport>

M_EXPORT int main(int argc, char *argv[])
{
    MApplication *application = MComponentCache::mApplication(argc, argv, "lookalike");
    MApplicationWindow *window = MComponentCache::mApplicationWindow();
    window->setStyleName(QLatin1String("CommonApplicationWindowInverted"));
    window->setWindowTitle("Look alike");
    LookAlikeMain *main = new LookAlikeMain();
    window->show();

    // Hide the status bar
    Qt::WindowStates state = window->windowState();
    window->setWindowState(state | Qt::WindowFullScreen);
    // Disable rounded corners
    window->setRoundedCornersEnabled(false);

    int result = application->exec();

    delete main;
    delete window;
    delete application;

    return result;
}
