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

/* prototypes */
static void signal_handler(int sig);

/* global variable used to indicate what signal (if any) has been caught */
static volatile int done = 0;

static const char help[] =
  "lwes-filter-listener [options]"                                     "\n"
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
  "    -e [comma separated list]"                                      "\n"
  "       The list of events to print out."                            "\n"
  ""                                                                   "\n"
  "    -a [comma separated k=v pairs]"                                 "\n"
  "       Key=value pairs to check before printing the event."         "\n"
  ""                                                                   "\n"
  "    -h"                                                             "\n"
  "         show this message"                                         "\n"
  ""                                                                   "\n"
  "  arguments are specified as -option value or -optionvalue"         "\n"
  ""                                                                   "\n";

int
lwes_U_INT_16_to_stream
  (LWES_U_INT_16 a_uint16,
   FILE *stream)
{
  return fprintf (stream,"%hu",a_uint16);
}

int
lwes_INT_16_to_stream
  (LWES_INT_16 an_int16,
   FILE *stream)
{
  return fprintf (stream,"%hd",an_int16);
}

int
lwes_U_INT_32_to_stream
  (LWES_U_INT_32 a_uint32,
   FILE *stream)
{
  return fprintf (stream,"%u",a_uint32);
}

int
lwes_INT_32_to_stream
  (LWES_INT_32 an_int32,
   FILE *stream)
{
  return fprintf (stream,"%d",an_int32);
}

int
lwes_U_INT_64_to_stream
  (LWES_U_INT_64 a_uint64,
   FILE *stream)
{
  return fprintf (stream,"%llu",a_uint64);
}

int
lwes_INT_64_to_stream
  (LWES_INT_64 an_int64,
   FILE *stream)
{
  return fprintf (stream,"%lld",an_int64);
}

int
lwes_BOOLEAN_to_stream
  (LWES_BOOLEAN a_boolean,
   FILE *stream)
{
  return fprintf (stream,"%s",((a_boolean==1)?"true":"false"));
}

int
lwes_IP_ADDR_to_stream
  (LWES_IP_ADDR an_ipaddr,
   FILE *stream)
{
  return fprintf (stream,"%s",inet_ntoa (an_ipaddr));
}

int
lwes_SHORT_STRING_to_stream
  (LWES_SHORT_STRING a_string,
   FILE *stream)
{
  return fprintf (stream,"%s",a_string);
}

int
lwes_LONG_STRING_to_stream
  (LWES_LONG_STRING a_string,
   FILE *stream)
{
  return fprintf (stream,"%s",a_string);
}

int
lwes_event_attribute_to_stream
  (struct lwes_event_attribute *attribute,
   FILE *stream)
{
  if (attribute->type == LWES_U_INT_16_TOKEN)
  {
    lwes_U_INT_16_to_stream (*((LWES_U_INT_16 *)attribute->value),stream);
  }
  else if (attribute->type == LWES_INT_16_TOKEN)
  {
    lwes_INT_16_to_stream (*((LWES_INT_16 *)attribute->value),stream);
  }
  else if (attribute->type == LWES_U_INT_32_TOKEN)
  {
    lwes_U_INT_32_to_stream (*((LWES_U_INT_32 *)attribute->value),stream);
  }
  else if (attribute->type == LWES_INT_32_TOKEN)
  {
    lwes_INT_32_to_stream (*((LWES_INT_32 *)attribute->value),stream);
  }
  else if (attribute->type == LWES_U_INT_64_TOKEN)
  {
    lwes_U_INT_64_to_stream (*((LWES_U_INT_64 *)attribute->value),stream);
  }
  else if (attribute->type == LWES_INT_64_TOKEN)
  {
    lwes_INT_64_to_stream (*((LWES_INT_64 *)attribute->value),stream);
  }
  else if (attribute->type == LWES_BOOLEAN_TOKEN)
  {
    lwes_BOOLEAN_to_stream (*((LWES_BOOLEAN *)attribute->value),stream);
  }
  else if (attribute->type == LWES_IP_ADDR_TOKEN)
  {
    lwes_IP_ADDR_to_stream (*((LWES_IP_ADDR *)attribute->value),stream);
  }
  else if (attribute->type == LWES_STRING_TOKEN)
  {
    lwes_LONG_STRING_to_stream ((LWES_LONG_STRING)attribute->value,stream);
  }
  else
  {
    /* should really do something here, but not sure what */
  }
  return 0;
}

int
lwes_event_to_stream
  (struct lwes_event *event,
   FILE *stream,
   const char *event_name)
{
  struct lwes_event_attribute *tmp;
  struct lwes_hash_enumeration e;

  if (event_name != NULL &&
      strcmp(event->eventName, event_name) != 0) {
    return 0;
  }

  lwes_SHORT_STRING_to_stream (event->eventName,stream);
  fprintf (stream,"[");
  fflush (stream);
  lwes_U_INT_16_to_stream (event->number_of_attributes,stream);
  fprintf (stream,"]");
  fflush (stream);
  fprintf (stream," {");
  fflush (stream);

  if (lwes_hash_keys (event->attributes, &e)) {
      while (lwes_hash_enumeration_has_more_elements (&e)) {
          LWES_SHORT_STRING tmpAttrName =
            lwes_hash_enumeration_next_element (&e);

          tmp =
            (struct lwes_event_attribute *)lwes_hash_get (event->attributes,
                                                          tmpAttrName);

          lwes_SHORT_STRING_to_stream (tmpAttrName,stream);
          fflush (stream);
          fprintf (stream," = ");
          fflush (stream);
          lwes_event_attribute_to_stream (tmp,stream);
          fflush (stream);
          fprintf (stream,";");
          fflush (stream);
        }
    }
  fprintf (stream,"}\n");
  fflush (stream);
  return 0;
}

int main (int   argc, char *argv[]) {

  const char *mcast_ip    = "224.1.1.11";
  const char *mcast_iface = NULL;
  int         mcast_port  = 12345;
  const char *event_name = NULL;
  const char *attr_list = NULL;

  sigset_t fullset;
  struct sigaction act;

  struct lwes_listener * listener;

  opterr = 0;
  while (1) {
    char c = getopt (argc, argv, "m:p:i:e:a:h");

    if (c == -1) {
      break;
    }

    switch (c) {
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

      case 'e':
        event_name = optarg;
        break;

      case 'a':
        attr_list = optarg;
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

  listener = lwes_listener_create (
      (LWES_SHORT_STRING) mcast_ip,
      (LWES_SHORT_STRING) mcast_iface,
      (LWES_U_INT_32)     mcast_port);

  while ( ! done ) {
    struct lwes_event *event = lwes_event_create_no_name ( NULL );

    if ( event != NULL ) {
      int ret = lwes_listener_recv ( listener, event);
      if ( ret > 0 ) {
        lwes_event_to_stream (event, stdout, event_name);
      }
    }
    lwes_event_destroy (event);
  }

  lwes_listener_destroy (listener);

  return 0;
}

static void signal_handler(int sig) {
  (void)sig; /* appease compiler */
  done = 1;
}
