/*======================================================================*
 * Copyright (C) 2008 Light Weight Event System                         *
 * All rights reserved.                                                 *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,                   *
 * Boston, MA 02110-1301 USA.                                           *
 *======================================================================*/
#ifndef __TIME_FUNCTIONS_H
#define __TIME_FUNCTIONS_H

#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include "lwes_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file lwes_time_functions.h
 *  \brief Functions for getting times in milliseconds since epoch
 */

/*! \brief Get time in milliseconds
 *
 * LWES started in Java, and in Java we use milliseconds since epoch for times.
 * So here is a convenience methods for getting the current time in
 * milliseconds since epoch
 *
 * \return the time since epoch in milliseconds
 */
LWES_INT_64
currentTimeMillisLongLong
  (void);

/*! \brief Convert to timeval
 * 
 * Converting an LWES_INT_64 to a struct timeval.
 * This is useful for printing the time.
 *
 * \param[in] timestamp a timestamp from currentTimeMillisLongLong ()
 * \param[out] t a struct timeval structure with the value from timestamp
 */
void
convertUnixLongLongTimeToTimeval
  (LWES_INT_64 timestamp,
   struct timeval *t);

#ifdef __cplusplus
}
#endif 

#endif /* __TIME_FUNCTIONS_H */
