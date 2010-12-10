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
#include "lwes_event.h"
#include "lwes_time_functions.h"
#include "lwes_marshall_functions.h"

struct lwes_listener *
lwes_listener_create
  (LWES_SHORT_STRING address,
   LWES_SHORT_STRING iface,
   LWES_U_INT_32 port)
{
  struct lwes_listener *listener =
    (struct lwes_listener *) malloc (sizeof (struct lwes_listener));

  if ( listener == NULL )
    {
      return NULL;
    }

  /* I originally was allocating on the stack, but that fails on freebsd,
     so I'll malloc and free this here */
  listener->buffer = (LWES_BYTE_P) malloc (sizeof (LWES_BYTE)*MAX_MSG_SIZE);

  if ( listener->buffer == NULL )
    {
      if ( listener != NULL )
        free (listener);
      return NULL;
    }

  listener->dtmp =
    (struct lwes_event_deserialize_tmp *)
      malloc (sizeof (struct lwes_event_deserialize_tmp));
  if ( listener->dtmp == NULL )
    {
      if ( listener->buffer != NULL )
        free (listener->buffer);
      if ( listener != NULL )
        free (listener);
      return NULL;
    }

  if ( (lwes_net_open (&(listener->connection),address,iface,port) != 0) ||
       (lwes_net_recv_bind (&(listener->connection)) != 0) )
    {
      if ( listener->dtmp != NULL )
        free (listener->dtmp);
      if ( listener->buffer != NULL )
        free (listener->buffer);
      if ( listener != NULL )
        free (listener);
      return NULL;
    }

  return listener;
}

int
lwes_listener_add_header_fields
  (struct lwes_listener *listener,
   LWES_BYTE_P bytes,
   size_t max,
   size_t *len)
{
  size_t n = *len;
  size_t offset_to_num_attrs;
  size_t tmp_offset;
  LWES_U_INT_16 num_attrs;
  LWES_INT_64 receipt_time;
  LWES_IP_ADDR sender_ip;
  LWES_U_INT_16 sender_port;

  /* grab some information from the packet and add it to the event */
  receipt_time = currentTimeMillisLongLong();
  sender_ip    = listener->connection.sender_ip_addr.sin_addr;
  sender_port  = ntohs (listener->connection.sender_ip_addr.sin_port);

  /* deserialize the event name */
  offset_to_num_attrs = 0;
  if (unmarshall_SHORT_STRING (listener->dtmp->tmp_string,
                               (SHORT_STRING_MAX+1),
                               bytes,
                               max,
                               &offset_to_num_attrs) == 0)
    {
      return -1;
    }

  /* keep track of offset of the number of attributes since
     we will change it */
  tmp_offset = offset_to_num_attrs;
  if (unmarshall_U_INT_16     (&num_attrs,
                               bytes,
                               max,
                               &tmp_offset) == 0)
    {
      return -2;
    }

  /* add receipt time to the event */
  if (   marshall_SHORT_STRING   ((LWES_SHORT_STRING)"ReceiptTime",
                                  bytes,
                                  max,
                                  &n) == 0
      || marshall_BYTE           (LWES_INT_64_TOKEN,
                                  bytes,
                                  max,
                                  &n) == 0
      || marshall_INT_64         (receipt_time,
                                  bytes,
                                  max,
                                  &n) == 0)
    {
      return -3;
    }
  ++num_attrs;

  /* add sender ip to the event */
  if (   marshall_SHORT_STRING   ((LWES_SHORT_STRING)"SenderIP",
                                  bytes,
                                  max,
                                  &n) == 0
      || marshall_BYTE           (LWES_IP_ADDR_TOKEN,
                                  bytes,
                                  max,
                                  &n) == 0
      ||  marshall_IP_ADDR       (sender_ip,
                                  bytes,
                                  max,
                                  &n) == 0)
    {
      return -4;
    }
  ++num_attrs;

  /* add sender port to the event */
  if (    marshall_SHORT_STRING   ((LWES_SHORT_STRING)"SenderPort",
                                   bytes,
                                   max,
                                   &n) == 0
       || marshall_BYTE           (LWES_U_INT_16_TOKEN,
                                   bytes,
                                   max,
                                   &n) == 0
       || marshall_U_INT_16       (sender_port,
                                   bytes,
                                   max,
                                   &n) == 0)
    {
      return -5;
    }
  ++num_attrs;

  /* finally put the new number of attributes into the appropriate place */
  if (marshall_U_INT_16      (num_attrs,
                              bytes,
                              max,
                              &offset_to_num_attrs) == 0)
    {
      return -6;
    }

  *len = n;
  return 0;
}

int
lwes_listener_event_has_name
  (LWES_BYTE_P bytes,
   size_t len,
   LWES_CONST_SHORT_STRING name)
{
  size_t serialized_name_len;

  /* NULL bytes is an error */
  if (bytes == NULL)
    {
      return -1;
    }

  /* minimum length is 1 byte of length and one character, so length
     should be at least 2 */
  if (len < 2)
    {
      return -1;
    }

  /* event name must be non null */
  if (name == NULL)
    {
      return -1;
    }

  /* first byte is the length */
  serialized_name_len = (LWES_BYTE)bytes[0];

  /* compare the lengths of the serialized and passed in names */
  if (serialized_name_len != strlen (name))
    {
      return -1;
    }

  /* finally compare the names themselves */

  /* name is offset by one byte */
  return memcmp ((const unsigned char *)&(bytes[1]), name, strlen (name));
}


/* FIXME: Make private in next major release of lwes */
int
lwes_listener_recv_process_event
  (struct lwes_listener *listener,
   struct lwes_event *event,
   int len)
{
  size_t n = len;
  int ret;
  if ((ret = lwes_listener_add_header_fields (listener,
                                              listener->buffer,
                                              MAX_MSG_SIZE,
                                              &n)) < 0)
    {
      return ret;
    }

   return lwes_event_from_bytes
            (event, listener->buffer, n, 0, listener->dtmp );
}

int
lwes_listener_recv
  (struct lwes_listener *listener,
   struct lwes_event *event)
{
  int n;

  if ( (n = lwes_listener_recv_bytes (listener,
                                      listener->buffer,
                                      MAX_MSG_SIZE)) < 0 )
    {
      return n;
    }

  return lwes_listener_recv_process_event (listener, event, n);
}

int
lwes_listener_recv_by
  (struct lwes_listener *listener,
   struct lwes_event *event,
   unsigned int timeout_ms)
{
  int n;

  if ( (n = lwes_listener_recv_bytes_by (listener,
                                         listener->buffer,
                                         MAX_MSG_SIZE,
                                         timeout_ms)) < 0 )
    {
      return n;
    }

  return lwes_listener_recv_process_event (listener, event, n);
}


int
lwes_listener_recv_bytes
  (struct lwes_listener *listener,
   LWES_BYTE_P bytes,
   size_t max)
{
  int n = 0;

  if ((n = lwes_net_recv_bytes (&(listener->connection),
                                bytes,
                                max)) < 0 )
    {
      return -2;
    }

  return n;
}

int
lwes_listener_recv_bytes_by
  (struct lwes_listener *listener,
   LWES_BYTE_P bytes,
   size_t max,
   unsigned int timeout_ms)
{
  int n = 0;

  if ((n = lwes_net_recv_bytes_by (&(listener->connection),
                                   bytes,
                                   max,
                                   timeout_ms)) < 0 )
    {
      return -2;
    }

  return n;
}


int
lwes_listener_destroy
  (struct lwes_listener *listener)
{
  int ret = 0;

  ret = lwes_net_close (&(listener->connection));

  if ( listener->buffer != NULL )
    free (listener->buffer);
  if ( listener->dtmp != NULL )
    free (listener->dtmp);
  if ( listener != NULL )
    free (listener);

  return ret;
}
