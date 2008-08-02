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
#ifndef __LWES_EMITTER_H
#define __LWES_EMITTER_H

#include "lwes_types.h"
#include "lwes_net_functions.h"
#include "lwes_event.h"

#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file lwes_emitter.h
 *  \brief Functions for emitting LWES events
 */

/*! \struct lwes_emitter lwes_emitter.h
 *  \brief Emits LWES events
 */
struct lwes_emitter
{
  /*! multicast channel to emit to */
  struct lwes_net_connection connection;
  /*! buffer for serialization of events */
  LWES_BYTE_P buffer;
  /*! count of total events sent */
  LWES_INT_64 count;
  /*! count of events sent since last heartbeat event */
  LWES_INT_64 count_since_last_beat;
  /*! sequence number of heartbeats */
  LWES_INT_64 sequence;
  /*! frequency in seconds of heartbeats */
  LWES_INT_16 frequency;
  /*! boolean for whether or not to emit heartbeats */
  LWES_BOOLEAN emitHeartbeat;
  /*! time of last heartbeat */
  time_t last_beat_time;
};

/*! \brief Create an Emitter
 *
 *  \param[in] address        The multicast ip address as a dotted quad string
 *                            of the channel to emit to.
 *  \param[in] iface          The dotted quad ip address of the interface to
 *                            send messages on, can be NULL to use default.
 *  \param[in] port           The port of the channel to emit to.
 *  \param[in] emit_heartbeat Set to 1 to emit heartbeats, set to 0 to not
 *                            emit heartbeats.
 *  \param[in] freq           Number of seconds between heartbeats.
 *
 *  \see lwes_emitter_destroy
 *
 *  \return A newly created emitter, use lwes_emitter_destroy to free
 */
struct lwes_emitter *
lwes_emitter_create
  (LWES_CONST_SHORT_STRING address,
   LWES_CONST_SHORT_STRING iface,
   LWES_U_INT_32 port,
   LWES_BOOLEAN emit_heartbeat,
   LWES_INT_16 freq);

/*! \brief Create an Emitter with a TTL
 *
 *  \param[in] address        The multicast ip address as a dotted quad string
 *                            of the channel to emit to.
 *  \param[in] iface          The dotted quad ip address of the interface to
 *                            send messages on, can be NULL to use default.
 *  \param[in] port           The port of the channel to emit to.
 *  \param[in] emit_heartbeat Set to 1 to emit heartbeats, set to 0 to not
 *                            emit heartbeats.
 *  \param[in] freq           Number of seconds between heartbeats.
 *  \param[in] ttl            The ttl to use for emitted events
 *
 *  \see lwes_emitter_destroy
 *
 *  \return A newly created emitter, use lwes_emitter_destroy to free
 */
struct lwes_emitter *
lwes_emitter_create_with_ttl
  (LWES_CONST_SHORT_STRING address,
   LWES_CONST_SHORT_STRING iface,
   LWES_U_INT_32 port,
   LWES_BOOLEAN emit_heartbeat,
   LWES_INT_16 freq,
   LWES_U_INT_32 ttl);

/*! \brief Emit an event to the multicast channel defined in the emitter
 *
 *  \param[in] emitter The emitter to emit to
 *  \param[in] event   The event to emit
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_emitter_emit
  (struct lwes_emitter *emitter,
   struct lwes_event *event);

/*! \brief Emit an event to a different multicast channel
 *
 *  \param[in] address The multicast ip address as a dotted quad string
 *                     of the channel to emit to.
 *  \param[in] iface   The dotted quad ip address of the interface to
 *                     send messages on, can be NULL to use default.
 *  \param[in] port    The port of the channel to emit to.
 *  \param[in] emitter The emitter to use for holding the send buffer
 *  \param[in] event   The event to emit
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_emitter_emitto
  (LWES_SHORT_STRING address,
   LWES_SHORT_STRING iface,
   LWES_U_INT_32 port,
   struct lwes_emitter *emitter,
   struct lwes_event *event);

/*! \brief Emit bytes to a multicast channel
 *
 * Use this in re-emitter's so that you don't have to deserialize and
 * reserialize, NOTE: this will not result in statistics being incremented
 *
 *  \param[in] emitter The emitter to emit to
 *  \param[in] bytes   The bytes to emit
 *  \param[in] length  The number of bytes to emit
 *
 *  \return the number of bytes sent on success, a negative number on failure
 */
int
lwes_emitter_emit_bytes
  (struct lwes_emitter *emitter,
   LWES_BYTE_P bytes,
   size_t length);

/*! \brief Destroy an Emitter
 *
 * \param[in] emitter The emitter to destroy by freeing all of it's used
 *                    memory.
 *
 * \return 0 on success, negative number on failure
 */
int
lwes_emitter_destroy
  (struct lwes_emitter *emitter);

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_EMITTER_H */
