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

