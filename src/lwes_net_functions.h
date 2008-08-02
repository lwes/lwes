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
#ifndef __LWES_NET_FUNCTIONS_H
#define __LWES_NET_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "lwes_types.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file lwes_net_functions.h
 *  \brief Functions for dealing with multicast channels
 */

/*! \struct lwes_net_connection lwes_net_functions.h
 *  \brief   IP Multicast Channel object
 */
struct lwes_net_connection 
{
  /*! low level socket descriptor */
  int socketfd;

  /*! inet address for the multicast channel */
  struct sockaddr_in mcast_addr;

  /*! mcast struct */
  struct ip_mreq     mreq;

  /*! inet address for the sender of this packet */
  struct sockaddr_in sender_ip_addr;

  /*! used to stored the size of the above structure */
  socklen_t sender_ip_socket_size;

  /*! boolean, will be TRUE if we have joined a multicast group.
      we only join a multicast group if we are receiving packets */
  int hasJoined;
};

/*! \brief Open a multicast channel
 *
 *  \param[in] conn the multicast channel object to hole this connection
 *  \param[in] address the multicast IP address of the channel to connect to
 *  \param[in] iface the IP address of the network interface this channel shoul
 *                   be connected to
 *  \param[in] port the multicast port of the channel to connect to
 *
 *  \return 0 on success, a negative number on error
 */
int
lwes_net_open
  (struct lwes_net_connection *conn,
   const char *address, 
   const char *iface,
   int port);

/*! \brief Close a multicast channel
 *
 *  \param[in] conn the multicast channel object containing the
 *                  connection to close
 *
 *  \return 0 on success, a negative number on error
 */
int
lwes_net_close
  (struct lwes_net_connection *conn);

/*! \brief Get the current Time-To-Live on a multicast channel
 *
 *  \param[in] conn the multicast channel to get the ttl for
 *
 *  \returns the current ttl value on success, a negative number on error
 */
int
lwes_net_get_ttl
  (struct lwes_net_connection *conn);

/*! \brief Set a new Time-To-Live on a multicast channel
 *
 *  \param[in] conn the multicast channel to get the ttl for
 *  \param[in] new_ttl the new ttl to set for outgoing packets
 *
 *  \return 0 on success, a negative number on error
 */
int
lwes_net_set_ttl
  (struct lwes_net_connection *conn, int new_ttl);

/*! \brief Get the socket file descriptor for the multicast channel
 *
 *  \param[in] conn the multicast channel to get the socket file descriptor for
 *
 *  \return the file descriptor on success, a negative number on error
 */
int
lwes_net_get_sock_fd
  (struct lwes_net_connection *conn);

/*! \brief Send bytes to the multicast channel
 *
 *  \param[in] conn the multicast channel to send bytes to
 *  \param[in] bytes an array of bytes to send out on the channel
 *  \param[in] len the number of bytes from bytes to send out on the channel
 *
 *  \return the number of bytes sent on success, a negative number on failure
 */
int
lwes_net_send_bytes
  (struct lwes_net_connection *conn,
   LWES_BYTE_P bytes,
   size_t len);

/*! \brief Send bytes to a different multicast channel
 *
 *  This can be used to send bytes out over an alternate channel, this will
 *  result in the creation and destruction of a struct lwes_net_connection
 *  internally.
 *
 *  \param[in] conn the multicast channel to ignore since it's not used in this
 *                  call
 *  \param[in] address the multicast IP address of the channel to connect to
 *  \param[in] iface the IP address of the network interface this channel shoul
 *                   be connected to
 *  \param[in] port the multicast port of the channel to connect to
 *  \param[in] bytes the bytes to send to the alternate channel
 *  \param[in] len the number of bytes to send to the alternate channel
 *
 *  \return the number of bytes sent on success, a negative number on failure
 */
int
lwes_net_sendto_bytes
  (struct lwes_net_connection *conn,
   char *address,
   char *iface,
   int port,
   LWES_BYTE_P bytes,
   size_t len);

/*! \brief Bind to the multicast channel
 *
 *  Used to actually bind to a channel, this should only be called if one
 *  plans to receive bytes from the channel
 *
 *  \param[in] conn the multicast channel to bind to
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_net_recv_bind
  (struct lwes_net_connection *conn);

/*! \brief Receive bytes from the multicast channel in blocking mode
 *
 *  This calls lwes_net_recv_bind internally.
 *
 *  \param[in] conn the multicast channel to receive bytes from
 *  \param[out] bytes the byte array to fill out
 *  \param[in] len the size of the byte array to fill out
 *
 *  \return the number of bytes received on success, a negative number on
 *          failure
 */
int
lwes_net_recv_bytes
  (struct lwes_net_connection *conn, 
   LWES_BYTE_P bytes,
   size_t len);

/*! \brief Receive bytes from the multicast channel with a timeout
 *
 *  This calls lwes_net_recv_bind internally.
 *
 *  \param[in] conn the multicast channel to receive bytes from
 *  \param[out] bytes the byte array to fill out
 *  \param[in] len the size of the byte array to fill out
 *  \param[in] timeout_ms the maximum time to block on this call
 *
 *  \return the number of bytes received on success, a negative number on
 *          failure
 */
int
lwes_net_recv_bytes_by
  (struct lwes_net_connection *conn, 
   LWES_BYTE_P bytes,
   size_t len,
   unsigned int timeout_ms);

#ifdef __cplusplus
}
#endif 

#endif /* __NET_FUNCTIONS_H */
