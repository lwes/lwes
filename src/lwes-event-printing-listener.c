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

#include "lwes_listener.h"

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <inttypes.h>

/* prototypes */
static void signal_handler(int sig);

/* global variable used to indicate what signal (if any) has been caught */
static volatile int done = 0;

static const char help[] =
  "lwes-event-printing-listener [options]"                             "\n"
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
  "    -h"                                                             "\n"
  "         show this message"                                         "\n"
  ""                                                                   "\n"
  "  arguments are specified as -option value or -optionvalue"         "\n"
  ""                                                                   "\n";


static int
lwes_event_to_stream
  (struct lwes_event *event,
   FILE *stream)
{
  struct lwes_event_attribute *tmp;
  struct lwes_hash_enumeration e;

  fprintf (stream, "%s", event->eventName);
  fprintf (stream,"[");
  fflush (stream);
  lwes_typed_value_to_stream (LWES_TYPE_U_INT_16, (void*)&event->number_of_attributes, stream);
  fprintf (stream,"]");
  fflush (stream);
  fprintf (stream,"\n");
  fflush (stream);
  fprintf (stream,"{\n");
  fflush (stream);

  if (lwes_hash_keys (event->attributes, &e))
    {
      while (lwes_hash_enumeration_has_more_elements (&e))
        {
          LWES_SHORT_STRING tmpAttrName =
            lwes_hash_enumeration_next_element (&e);

          tmp =
            (struct lwes_event_attribute *)lwes_hash_get (event->attributes,
                                                          tmpAttrName);

          fprintf (stream,"\t");
          fflush (stream);
          fprintf (stream, "%s", tmpAttrName);
          fflush (stream);
          fprintf (stream," = ");
          fflush (stream);
          lwes_event_attribute_to_stream (tmp, stream);
          fflush (stream);
          fprintf (stream,";\n");
          fflush (stream);
        }
    }
  fprintf (stream,"}\n");
  fflush (stream);
  return 0;
}

int main (int   argc,
          char *argv[])
{
  const char *mcast_ip    = "224.1.1.11";
  const char *mcast_iface = NULL;
  int         mcast_port  = 12345;

  sigset_t fullset;
  struct sigaction act;

  struct lwes_listener * listener;

  opterr = 0;
  while (1)
    {
      char c = getopt (argc, argv, "m:p:i:h");

      if (c == -1)
        {
          break;
        }

      switch (c)
        {
          case 'm':
            mcast_ip = optarg;

            break;

          case 'p':
            mcast_port = atoi(optarg);

            break;

          case 'h':
            fprintf (stderr, "%s", help);

            return 1;

          case 'i':
            mcast_iface = optarg;

            break;

          default:
            fprintf (stderr,
                     "error: unrecognized command line option -%c\n", 
                     optopt);

            return 1;
        }
    }

  sigfillset (&fullset);
  sigprocmask (SIG_SETMASK, &fullset, NULL);

  memset (&act, 0, sizeof (act));
  act.sa_handler = signal_handler;
  sigfillset (&act.sa_mask);

  sigaction (SIGINT, &act, NULL);
  sigaction (SIGTERM, &act, NULL);
  sigaction (SIGPIPE, &act, NULL);

  sigdelset (&fullset, SIGINT);
  sigdelset (&fullset, SIGTERM);
  sigdelset (&fullset, SIGPIPE);

  sigprocmask (SIG_SETMASK, &fullset, NULL);

  listener = lwes_listener_create ( (LWES_SHORT_STRING) mcast_ip,
                                    (LWES_SHORT_STRING) mcast_iface,
                                    (LWES_U_INT_32)     mcast_port );

  while ( ! done )
    {
      struct lwes_event *event = lwes_event_create_no_name ( NULL );

      if ( event != NULL )
        {
          int ret = lwes_listener_recv ( listener, event);
          if ( ret > 0 )
            {
              lwes_event_to_stream (event, stdout);
            }
        }
      lwes_event_destroy (event);
    }

  lwes_listener_destroy (listener);

  return 0;
}

static void signal_handler(int sig)
{
  (void)sig; /* appease compiler */
  done = 1;
}
