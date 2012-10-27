/*
 * This file is part of LookAlike
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

#ifndef ENFORCE_H
#define ENFORCE_H

#include <QtGlobal>
#include <stdlib.h>

#ifndef ENFORCE
# if defined(QT_NO_DEBUG) || defined(UNIT_TEST)
#  define ENFORCE(expr) expr
# else /* !QT_NO_DEBUG  && !UNIT_TEST*/
#  define ENFORCE(expr) \
    do { \
        bool success = (expr); \
        if (!success) { \
            qt_assert(#expr, __FILE__, __LINE__); \
        } \
    } while (0)
# endif /* QT_NO_DEBUG || UNIT_TEST*/
#endif /* !ENFORCE */


#define TEMPLATE_RETURN_VAL_IF_FAIL(expr, printer, afterprint) \
    do { \
        bool success = (expr); \
        if (!success) { \
            printer ("%s:%u: Check failed: %s", __FILE__, __LINE__, #expr); \
            afterprint; \
        } \
    } while (0)


#ifndef RETURN_IF_FAIL
# if defined(QT_NO_DEBUG) || defined(UNIT_TEST)
#  define RETURN_IF_FAIL(expr) \
          TEMPLATE_RETURN_VAL_IF_FAIL(expr, qWarning, ((void)0))
# else /* !QT_NO_DEBUG  && !UNIT_TEST*/
#  define RETURN_IF_FAIL(expr) \
          TEMPLATE_RETURN_VAL_IF_FAIL(expr, qCritical, ::abort())
# endif /* QT_NO_DEBUG || UNIT_TEST*/
#endif /* !RETURN_IF_FAIL */

#ifndef RETURN_VAL_IF_FAIL
# if defined(QT_NO_DEBUG) || defined(UNIT_TEST)
#  define RETURN_VAL_IF_FAIL(expr, value) \
          TEMPLATE_RETURN_VAL_IF_FAIL(expr, qWarning, return (value))
# else /* !QT_NO_DEBUG && !UNIT_TEST */
#  define RETURN_VAL_IF_FAIL(expr, value) \
          TEMPLATE_RETURN_VAL_IF_FAIL(expr, qCritical, ::abort())
# endif /* QT_NO_DEBUG || UNIT_TEST*/
#endif /* !RETURN_IF_FAIL */

#endif /* ENFORCE_H */
