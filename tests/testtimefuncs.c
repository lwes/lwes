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
