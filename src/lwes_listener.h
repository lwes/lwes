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

#ifndef __LWES_LISTENER_H
#define __LWES_LISTENER_H

#include "lwes_types.h"
#include "lwes_net_functions.h"
#include "lwes_event.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file lwes_listener.h
 *  \brief Functions for listening to LWES events
 */

/*! \struct lwes_listener lwes_listener.h
 *  \brief Listens for LWES events
 */
struct lwes_listener
{
  /*! multicast channel to listen on */
  struct lwes_net_connection connection;
  /*! this is some temporary space for deserializing strings */
  struct lwes_event_deserialize_tmp *dtmp;
  /*! this is a temporary buffer for the packet from the socket */
  LWES_BYTE_P buffer;
};

/*! \brief Create a Listener
 *
 *  \param[in] address The multicast ip address as a dotted quad string
 *                     of the channel to listen on.
 *  \param[in] iface   The dotted quad ip address of the interface to
 *                     receive messages on, can be NULL to use default.
 *  \param[in] port    The port of the channel to listen on.
 *
 *  \see lwes_listener_destroy
 *
 *  \return A newly created listener, use lwes_listener_destroy to free
 */
struct lwes_listener *
lwes_listener_create
  (LWES_SHORT_STRING address,
   LWES_SHORT_STRING iface,
   LWES_U_INT_32 port);

/*! \brief Copy some date from the UDP packet into the event
 *
 *  This will add the following fields to the event as additional attributes
 *    - SenderIP    - the ip address of the sender of the event
 *    - SenderPort  - the port of the sender of the event
 *    - ReceiptTime - a timestamp of receipt time, as milliseconds since epoch
 *
 *  This should be called immediately after one of
 *    - lwes_listener_recv
 *    - lwes_listener_recv_by
 *    - lwes_listener_recv_bytes
 *    - lwes_listener_recv_bytes_by
 *  in order for the sender ip and port to be accurate
 *
 *  \param[in] listener The listener which just received the event
 *  \param[in] bytes    The serialized event to add the attributes to
 *  \param[in] max      The maximum size of the serialzed event
 *  \param[in,out] len  The current size of the serialized event
 *
 *  \see lwes_listener_recv
 *  \see lwes_listener_recv_by
 *  \see lwes_listener_recv_bytes
 *  \see lwes_listener_recv_bytes_by
 *
 *  \return 0 upon success, a negative number upon failure
 */
int
lwes_listener_add_header_fields
  (struct lwes_listener *listener,
   LWES_BYTE_P bytes,
   size_t max,
   size_t *len);

/*! \brief Determine if a serialized event is of a given type
 *
 *  This function will compare the name in a serialized event against a
 *  given name, if they match 0 is returned, if not a non-zero value is
 *  returned
 *
 *  \param[in] bytes   The serialized event
 *  \param[in] len     The current size of the serialized event
 *  \param[in] name    The name to check
 *
 *  \return 0 if the serialized event is of type name, non-zero if it is
 *          not.
 */
int
lwes_listener_event_has_name
  (LWES_BYTE_P bytes,
   size_t len,
   LWES_CONST_SHORT_STRING name);

/*! \brief This adds headers and deserializes the event
 *
 * This is actually a private function, which will be removed next major
 * version bump of lwes.
 *
 * \param[in] listener the listener which just received the event
 * \param[out] event the event object to deserialize into
 * \param[in] len the size of the serialized event waiting in the listener
 *
 * \return 0 on success, a negative number on failure
 */
int
lwes_listener_recv_process_event
  (struct lwes_listener *listener,
   struct lwes_event *event,
   int len);

/*! \brief Receive an event from the listener in a blocking manner
 *
 *  \param[in] listener the listener to receive the event from
 *  \param[out] event the event to fill out
 *
 *  \return the number of bytes read on success, a negative number on failure
 */
int
lwes_listener_recv
  (struct lwes_listener *listener,
   struct lwes_event *event);

/*! \brief Receive an event from the listener with a timeout
 *
 *  \param[in] listener the listener to receive the event from
 *  \param[out] event the event to fill out
 *  \param[in] timeout_ms the maximum amount of time to wait for an event
 *
 *  \return the number of bytes read on success, a negative number on failure
 */
int
lwes_listener_recv_by
  (struct lwes_listener *listener,
   struct lwes_event *event,
   unsigned int timeout_ms);

/*! \brief Receive bytes from the listener in a blocking manner
 *
 *  This returns the raw bytes and returns them, it will not add header fields
 *  nor deserialize the event.  This is useful if you want to journal without
 *  the cost of deserialization.
 *
 *  \param[in] listener the listener to receive the bytes from
 *  \param[out] bytes the byte array to write into
 *  \param[in]  max the maximum number of bytes to write into the bytes array
 *
 *  \return the number of bytes read on success, a negative number on failure
 */
int
lwes_listener_recv_bytes
  (struct lwes_listener *listener,
   LWES_BYTE_P bytes,
   size_t max);

/*! \brief Receive bytes from the listener with a timeout
 *
 *  This returns the raw bytes and returns them, it will not add header fields
 *  nor deserialize the event.  This is useful if you want to journal without
 *  the cost of deserialization.
 *
 *  \param[in] listener the listener to receive the bytes from
 *  \param[out] bytes the byte array to write into
 *  \param[in] max the maximum number of bytes to write into the bytes array
 *  \param[in] timeout_ms the maximum amount of time to wait for bytes
 *
 *  \return the number of bytes read on success, a negative number on failure
 */
int
lwes_listener_recv_bytes_by
  (struct lwes_listener *listener,
   LWES_BYTE_P bytes,
   size_t max,
   unsigned int timeout_ms);

/*! \brief Destroy a Listener
 *
 * \param[in] listener The listener to destroy by freeing all of it's used
 *                     memory.
 *
 * \return 0 on success, negative number on failure
 */
int
lwes_listener_destroy
  (struct lwes_listener *listener);

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_LISTENER_H */
