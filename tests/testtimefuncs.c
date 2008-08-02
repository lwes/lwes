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
 *                                                                      *
 * $Id$                                                                 *
 *======================================================================*/
#if HAVE_CONFIG_H
  #include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "lwes_time_functions.h"
#include "lwes_time_functions.c"

int main(void)
{
  struct timeval tv1;
  /* a known timeval and it's equivalent ULL representation */
  struct timeval known_time        = { 1095303071, 455000 };
  unsigned long long known_time_ll = 1095303071455ULL;
  int i = 0;

  /* make sure convertUnixLongLongTimeToTimeval gives us back what we expect */
  convertUnixLongLongTimeToTimeval(known_time_ll, &tv1);
  assert ( known_time.tv_sec == tv1.tv_sec );
  assert ( known_time.tv_usec == tv1.tv_usec);

  /* check for currentTimeMillisLongLong
   *
   * since I can't be sure that I won't hit a second boundary, I'll test
   * 100 times.
   */
  for ( i = 0; i < 100 ; i++ )
    {
      struct timeval itv1 = { 0, 0 };
      struct timeval itv2 = { 0, 0 };
      unsigned long long current_time = 0LL;

      /* get a reference time */
      gettimeofday(&itv1,0);

      /* get it as a long long */
      current_time = currentTimeMillisLongLong ();

      /* convert to a timeval */
      convertUnixLongLongTimeToTimeval (current_time, &itv2);

      /* compare to the time from gettimeofday, since we are internally
       * calling gettime of day this should be very close, at most you
       * may go over a second boundary so allow 1 second difference
       */
      assert ( (itv2.tv_sec - itv1.tv_sec) == 0
               || ( itv2.tv_sec - itv1.tv_sec) == 1 );
    }

  return 0;
}
