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

#include "lwes_emitter.h"

/*************************************************************************
  PRIVATE API prototypes, shouldn't be called by a user of the library.
 *************************************************************************/
int
lwes_emitter_emit_event
  (struct lwes_emitter *emitter,
   struct lwes_event *event);

int
lwes_emitter_collect_statistics
  (struct lwes_emitter *emitter);

void lwes_emitter_calculate_and_send_statistics
  (struct lwes_emitter *emitter,
   struct lwes_event *stats_event,
   time_t current_time);

/*************************************************************************
  PUBLIC API
 *************************************************************************/
struct lwes_emitter *
lwes_emitter_create
  (LWES_CONST_SHORT_STRING address,
   LWES_CONST_SHORT_STRING iface,
   LWES_U_INT_32 port,
   LWES_BOOLEAN emit_heartbeat,
   LWES_INT_16 freq)
{
  return lwes_emitter_create_with_ttl (address,
                                       iface,
                                       port,
                                       emit_heartbeat,
                                       freq,
                                       3);
}

struct lwes_emitter *
lwes_emitter_create_with_ttl
  (LWES_CONST_SHORT_STRING address,
   LWES_CONST_SHORT_STRING iface,
   LWES_U_INT_32 port,
   LWES_BOOLEAN emit_heartbeat,
   LWES_INT_16 freq,
   LWES_U_INT_32 ttl)
{
  struct lwes_event* tmp_event;
  struct lwes_emitter* emitter =
    (struct lwes_emitter *) malloc (sizeof (struct lwes_emitter));

  if (emitter == NULL)
    {
      return NULL;
    }

  if (lwes_net_open (&(emitter->connection),address,iface,port) < 0)
    {
      free (emitter);
      return NULL;
    }

  if (lwes_net_set_ttl (&(emitter->connection), ttl) == -1)
    {
      free (emitter);
      return NULL;
    }

  emitter->buffer = (LWES_BYTE_P) malloc (sizeof (LWES_BYTE)*MAX_MSG_SIZE);
  if (emitter->buffer == NULL)
    {
      free(emitter);
      return NULL;
    }

  emitter->count = 0;
  emitter->count_since_last_beat = 0;
  emitter->sequence = 0;
  emitter->frequency = freq;
  emitter->emitHeartbeat = emit_heartbeat;

  /* Send an event saying we are starting up */
  if (emitter->emitHeartbeat)
    {
      tmp_event = lwes_event_create (NULL,(LWES_SHORT_STRING)"System::Startup");
      if ( tmp_event != NULL )
        {
          emitter->last_beat_time = time (NULL);
          lwes_emitter_emit_event (emitter,tmp_event);
          lwes_event_destroy (tmp_event);
        }
    }

  return emitter;
}

int
lwes_emitter_emit
  (struct lwes_emitter *emitter,
   struct lwes_event *event)
{
  int error=0;

  if(emitter == NULL)
  {
    return -1;
  }

  /* Send an event */
  error = lwes_emitter_emit_event (emitter,event);

  lwes_emitter_collect_statistics (emitter);

  return error;
}

int
lwes_emitter_emitto
  (LWES_SHORT_STRING address,
   LWES_SHORT_STRING iface,
   LWES_U_INT_32 port,
   struct lwes_emitter *emitter,
   struct lwes_event *event)
{
  int size;

  if(emitter == NULL)
  {
    return -1;
  }

  /* Send an event */
  if ((size = lwes_event_to_bytes (event,emitter->buffer,MAX_MSG_SIZE,0)) < 0)
    {
      return -1;
    }

  if (lwes_net_sendto_bytes (&(emitter->connection),
                             address,
                             iface,
                             port,
                             emitter->buffer,size) < 0 )
  {
    return -2;
  }

  return 0;
}

int
lwes_emitter_destroy
  (struct lwes_emitter *emitter)
{
  /* Send an event saying we are shutting down */
  int ret = 0;

  if (emitter != NULL)
    {
      if (emitter->emitHeartbeat)
        {
          struct lwes_event *tmp_event =
            lwes_event_create(NULL,(LWES_SHORT_STRING)"System::Shutdown");
          time_t current_time = time (NULL);

          lwes_emitter_calculate_and_send_statistics (emitter,
                                                      tmp_event,
                                                      current_time);
        }

      /* shutdown the network, use the return code here for library users */
      ret = lwes_net_close (&(emitter->connection));

      /* free our memory */
      if ( emitter->buffer != NULL )
        {
          free(emitter->buffer);
        }
      free(emitter);
   }

  return ret;
}

int
lwes_emitter_emit_bytes
  (struct lwes_emitter *emitter,
   LWES_BYTE_P bytes,
   size_t length)
{
  return lwes_net_send_bytes (&(emitter->connection), bytes ,length);
}

/*************************************************************************
  PRIVATE API
 *************************************************************************/
int
lwes_emitter_emit_event
  (struct lwes_emitter *emitter,
   struct lwes_event *event)
{
  int size;

  if ((size = lwes_event_to_bytes (event,emitter->buffer,MAX_MSG_SIZE,0)) < 0)
  {
    return -1;
  }

  if (lwes_emitter_emit_bytes (emitter, emitter->buffer, size) == -1)
  {
    return -2;
  }

  return 0;
}

void lwes_emitter_calculate_and_send_statistics
  (struct lwes_emitter *emitter,
   struct lwes_event *stats_event,
   time_t current_time)
{
  if (stats_event != NULL)
    {
      LWES_INT_16 frequency_this_period;
      time_t tmp = (current_time - emitter->last_beat_time);
      if ( tmp > 32767 )
        {
           frequency_this_period=32767;
        }
      else if ( tmp < 0 )
        {
          frequency_this_period=0;
        }
      else
        {
          frequency_this_period=(LWES_INT_16)tmp;
        }

      lwes_event_set_INT_16(stats_event,(LWES_SHORT_STRING)"freq",
                            frequency_this_period);
      lwes_event_set_INT_64(stats_event,(LWES_SHORT_STRING)"seq",
                            emitter->sequence);
      lwes_event_set_INT_64(stats_event,(LWES_SHORT_STRING)"count",
                            emitter->count_since_last_beat);
      lwes_event_set_INT_64(stats_event,(LWES_SHORT_STRING)"total",
                            emitter->count);
      lwes_emitter_emit_event(emitter,stats_event);
      lwes_event_destroy(stats_event);
    }
}

int
lwes_emitter_collect_statistics
  (struct lwes_emitter *emitter)
{
  time_t current_time = time (NULL);

  /* Count it */
  emitter->count++;
  emitter->count_since_last_beat++;

  /* Send a heartbeat event */
  if ( emitter->emitHeartbeat 
       && (current_time - emitter->last_beat_time) >= emitter->frequency )
    {
      struct lwes_event *tmp_event =
        lwes_event_create (NULL,(LWES_SHORT_STRING)"System::Heartbeat");

      if (tmp_event != NULL)
        {
          emitter->sequence++;
          lwes_emitter_calculate_and_send_statistics (emitter,
                                                      tmp_event,
                                                      current_time);
          emitter->last_beat_time = current_time;
          emitter->count_since_last_beat = 0;
        }
    }
  return 0;
}
