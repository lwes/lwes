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

#include "lwes_time_functions.h"

#if HAVE_CONFIG_H
  #include <config.h>
#endif

#if HAVE_EXTERNAL_GETTIMEOFDAY
#include EXTERNAL_GETTIMEOFDAY_HEADER
#endif

#ifndef GETTIMEOFDAY
#define GETTIMEOFDAY(t,tz) gettimeofday(t,tz)
#endif


LWES_INT_64 currentTimeMillisLongLong(void)
{
  struct timeval t;

  /* gettimeofday should ALWAYS return a value since the only possible error
   * according to the FreeBSD and Linux man pages is EFAULT if you access
   * invalid memory, this memory is on the stack so better be valid or we
   * have bigger problems
   */
  GETTIMEOFDAY(&t,0);

  return ((((LWES_INT_64)t.tv_sec)*((LWES_INT_64)1000)) +
                             (LWES_INT_64)(t.tv_usec/1000));
}

void convertUnixLongLongTimeToTimeval(LWES_INT_64 timestamp, struct timeval *t)
{
  t->tv_sec = (long)(timestamp/1000);
  t->tv_usec = (long)((timestamp%1000)*1000);
}

