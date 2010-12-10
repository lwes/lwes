/*======================================================================*
 * Copyright (c) 2008, Yahoo! Inc. All rights reserved.                 *
 *                                                                      *
 * Licensed under the New BSD License (the "License"); you may not use  *
 * this file except in compliance with the License.  Unless required    *
 * by applicable law or agreed to in writing, software distributed      *
 * under the License is distributed on an "AS IS" BASIS, WITHOUT        *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     *
 * See the License for the specific language governing permissions and  *
 * limitations under the License. See accompanying LICENSE file.        *
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
