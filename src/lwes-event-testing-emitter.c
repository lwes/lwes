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

#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "lwes_emitter.h"
#include "lwes_time_functions.h"

static const char help[] =
  "lwes-event-testing-emitter [options]"                               "\n"
  ""                                                                   "\n"
  "  where options are:"                                               "\n"
  ""                                                                   "\n"
  "    -m [one argument]"                                              "\n"
  "       The multicast ip address to listen on."                      "\n"
  "       (default: 224.1.1.11)"                                       "\n"
  ""                                                                   "\n"
  "    -p [one argument]"                                              "\n"
  "       The ip port to listen on."                                   "\n"
  "       (default: 12345)"                                            "\n"
  ""                                                                   "\n"
  "    -i [one argument]"                                              "\n"
  "       The interface to listen on."                                 "\n"
  "       (default: 0.0.0.0)"                                          "\n"
  ""                                                                   "\n"
  "    -n [one argument]"                                              "\n"
  "       The number of events per second to emit."                    "\n"
  "       (default: 1)"                                                "\n"
  ""                                                                   "\n"
  "    -s [one argument]"                                              "\n"
  "       The number of seconds to emit for."                          "\n"
  "       (default: 1)"                                                "\n"
  ""                                                                   "\n"
  "    -x [one argument]"                                              "\n"
  "       The number of extra pad bytes to add to each event."         "\n"
  "       (default: 0)"                                                "\n"
  ""                                                                   "\n"
  "    -b [one argument]"                                              "\n"
  "       The amount of seconds to break for between event bursts."    "\n"
  "       (default: 0)"                                                "\n"
  ""                                                                   "\n"
  "    -e"                                                             "\n"
  "       Attempt to emit events evenly throughout each second."       "\n"
  ""                                                                   "\n"
  "    -h"                                                             "\n"
  "       show this message"                                           "\n"
  ""                                                                   "\n"
  "  arguments are specified as -option value or -optionvalue"         "\n"
  ""                                                                   "\n";

int main (int   argc,
          char *argv[])
{
  const char *mcast_ip    = "224.1.1.11";
  const char *mcast_iface = NULL;
  int         mcast_port  = 12345;
  int         number      = 1;
  int         pad         = 0;
  int         seconds     = 1;
  int         pause       = 0;
  int         even        = 0;

  struct lwes_emitter * emitter;
  struct lwes_event *event;
  char *pad_string;

  /* turn off error messages, I'll handle them */
  opterr = 0;
  while (1)
    {
      char c = getopt (argc, argv, "m:p:i:n:s:x:b:eh");

      if (c == -1)
        {
          break;
        }

      switch (c)
        {
          case 'b':
            pause = atoi(optarg);

            break;

          case 'm':
            mcast_ip = optarg;

            break;

          case 'p':
            mcast_port = atoi(optarg);

            break;

          case 'n':
            number = atoi(optarg);

            break;

          case 's':
            seconds = atoi(optarg);
            break;

          case 'e':
            even = 1;
            break;

          case 'h':
            fprintf (stderr, "%s", help);

            return 1;

          case 'i':
            mcast_iface = optarg;

            break;

          case 'x':
            pad = atoi(optarg);
            break;

          default:
            fprintf (stderr,
                     "error: unrecognized command line option -%c\n", 
                     optopt);

            return 1;
        }
    }

  pad_string = NULL;
  if (pad > 0)
    {
      pad_string = malloc(pad+1);
      if (pad_string == NULL)
        {
          fprintf (stderr,
                   "Unable to allocate %d bytes for pad string\n",
                   pad);
          exit(1);
        }
      else
        {
          memset(pad_string, 'X', pad);
          pad_string[pad] = '\0';
        }
    }

  emitter = lwes_emitter_create ( (LWES_SHORT_STRING) mcast_ip,
                                  (LWES_SHORT_STRING) mcast_iface,
                                  (LWES_U_INT_32)     mcast_port,
                                  0,
                                  10 );

  assert (emitter != NULL);

  {
    int i,s,n;
    LWES_INT_64 start  = 0LL;
    LWES_INT_64 stop   = 0LL;

    for (s = 0, n = 0; s < seconds; ++s)
      {
        start = currentTimeMillisLongLong ();
        stop  = start;
        for (i = 0; i < number; ++i, ++n)
          {
            event  = lwes_event_create (NULL, "MyEvent");
            assert (event != NULL);

            assert
              (lwes_event_set_STRING   (event, "field", "hello world") == 1 );
            assert
              (lwes_event_set_INT_32   (event, "count", i) == 2);
            assert
              (lwes_event_set_INT_32   (event, "run", s) == 3);
            assert
              (lwes_event_set_STRING (event, "prog_id","12345") == 4);
            assert
              (lwes_event_set_INT_16 (event,"num", 2) == 5);
            assert
              (lwes_event_set_STRING (event,"k0", "a-key.count") == 6);
            assert
              (lwes_event_set_INT_16 (event,"v0", 1) == 7);
            assert
              (lwes_event_set_STRING (event,"k1", "b-key.count") == 8);
            assert
              (lwes_event_set_INT_16 (event,"v1", 2) == 9);
            assert
              (lwes_event_set_U_INT_64(event, "global_count", n) == 10);
            assert
              (lwes_event_set_INT_64 (event,"SentTime", stop) == 11);
            if (pad>0) {
              assert(lwes_event_set_STRING (event,"pad",pad_string) == 12);
            }

            assert (lwes_emitter_emit (emitter, event) == 0);

            lwes_event_destroy(event);
            stop = currentTimeMillisLongLong ();

            /* if we are going over our total time, bail out */
            if ((stop - start) >= 1000)
              {
                printf ("Was only able to emit %7d in 1 sec\n",i);
                break;
              }
            
            /* if we are attempting to send events evenly throughout
            the second, consider pausing briefly to get back on
            schedule */
            if (even)
              {
                const int delay_ms = (i*1000/number) - (stop-start);
                if (delay_ms > 0)
                  {
                    usleep (delay_ms*1000);
                  }
              }
          }
        {
          const int delay_ms = (1000 - (stop - start))*1000;
          if (delay_ms > 0) usleep (delay_ms);
        }
        {
          char timebuff[20];
          /* HH/MM/SS DD/MM/YYYY  */
          /* 12345678901234567890 */
          time_t start_time = time (NULL);

          strftime (timebuff, 20, "%H:%M:%S %d/%m/%Y", localtime (&start_time));

          printf ("%s : %7d\n", timebuff, i);
        }

        sleep (pause);
      }
    lwes_emitter_destroy(emitter);
    if (pad_string!=NULL) free(pad_string);
  }

  return 0;
}
