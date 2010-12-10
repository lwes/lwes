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

#include "lwes_net_functions.h"

#include <string.h>
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

int
lwes_net_open
  (struct lwes_net_connection *conn,
   const char *address, 
   const char *iface,
   int port)
{
  int i;
  int arg;

  /* error out on a NULL connection */
  if (conn == NULL)
    {
      return -1;
    }

  /* set up the address structure */
  memset((char *) &conn->mcast_addr, 0, sizeof(conn->mcast_addr));
  conn->mcast_addr.sin_family      = AF_INET;
  conn->mcast_addr.sin_addr.s_addr = inet_addr (address);
  conn->mcast_addr.sin_port        = htons ((short)port);
  conn->hasJoined                  = 0;

  /* and the multicast structure (which may not be used if this is a unicast
     connection) */
  conn->mreq.imr_multiaddr = conn->mcast_addr.sin_addr;
  if ( iface == NULL || strcmp("",iface) == 0 )
    {
      conn->mreq.imr_interface.s_addr = htonl (INADDR_ANY);
    }
  else
    {
      conn->mreq.imr_interface.s_addr = inet_addr (iface);
    }

  /* construct the socket */
  if ( (conn->socketfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
      return -2;
    }

  /* if the address we are emitting on is a multicast address we
     set the sockopt which uses that interface to emit on, this doesn't
     work for unicast */
  if (IN_MULTICAST (ntohl (conn->mcast_addr.sin_addr.s_addr)))
    {
      if (iface)
        {
          struct in_addr mcastAddr;

          mcastAddr.s_addr = inet_addr (iface);
          if (setsockopt(conn->socketfd, IPPROTO_IP, IP_MULTICAST_IF,
                            (char *)&mcastAddr, sizeof(mcastAddr)) < 0 )
            {
              return -3;
            }
        }
    }

  /* Setting the value for SO_SNDBF , trying for 10*MAX_MSG_SIZE */
  for ( i = 10 ; i > 0 ; i-- )
    {
      arg = MAX_MSG_SIZE*i;
      if (setsockopt (conn->socketfd, SOL_SOCKET, SO_SNDBUF,
                      (void*)&arg,sizeof(arg)) == 0)
        {
          break;
        }
    }
  if ( i == 0 )
    {
      return -4;
    }

  /* set the size for use below */
  conn->sender_ip_socket_size = (socklen_t)sizeof(conn->sender_ip_addr);

  return 0;
}

int
lwes_net_close
  (struct lwes_net_connection *conn)
{
  if (conn == NULL)
    {
      return -1;
    }

  /* check hasJoined first, as we also "join" a unicast channel, so may
     need to do cleanup here someday */
  if ( conn->hasJoined == 1 )
    {
      /* if we are using a multicast address we need to drop the membership
         so that upstream routers get the message */
      if (IN_MULTICAST (ntohl (conn->mcast_addr.sin_addr.s_addr)))
        {
          /* drop the multicast channel */
          if (setsockopt
                (conn->socketfd,
                 IPPROTO_IP,
                 IP_DROP_MEMBERSHIP,
                 (void*)&(conn->mreq),
                 sizeof(conn->mreq)) < 0 )
          {
            return -2;
          }
        }
    }

  return close(conn->socketfd);
}

int
lwes_net_get_ttl
  (struct lwes_net_connection *conn)
{
  if (conn != NULL)
    {
      unsigned char ttl = 0;
      socklen_t     len;

      len = sizeof (ttl);

      if (IN_MULTICAST (ntohl (conn->mcast_addr.sin_addr.s_addr)))
        {
          if (getsockopt (conn->socketfd,
                          IPPROTO_IP,
                          IP_MULTICAST_TTL,
                          &ttl,
                          &len) < 0)
            {
              return -2;
            }
        }

      return ttl;
    }

  return -1;
}

int
lwes_net_set_ttl
  (struct lwes_net_connection *conn, int new_ttl)
{
  if (conn != NULL)
    {
      unsigned char ttl;

      ttl = new_ttl;
      if (IN_MULTICAST (ntohl (conn->mcast_addr.sin_addr.s_addr)))
        {
          if (setsockopt (conn->socketfd,
                          IPPROTO_IP,
                          IP_MULTICAST_TTL,
                          (void*)&ttl,
                          sizeof (ttl)) < 0)
            {
              return -2;
            }
        }

      return 0;
    }

  return -1;
}

int
lwes_net_get_sock_fd
  (struct lwes_net_connection *conn)
{
  if (conn == NULL)
    {
      return -1;
    }
  return conn->socketfd;
}

int
lwes_net_send_bytes
  (struct lwes_net_connection *conn,
   LWES_BYTE_P bytes,
   size_t len)
{
  int size = 0;

  if (conn == NULL || bytes == NULL)
    {
      return -1;
    }

  size =
    sendto
      (conn->socketfd,
       bytes,
       len,
       0, 
       (struct sockaddr * )&(conn->mcast_addr),
       sizeof(conn->mcast_addr));

  return size;
}

int
lwes_net_sendto_bytes
  (struct lwes_net_connection *conn,
   char *address,
   char *iface,
   int port,
   LWES_BYTE_P bytes,
   size_t len)
{
  struct lwes_net_connection new_conn;
  int size;

  if (conn == NULL || bytes == NULL)
    {
      return -1;
    }

  if (lwes_net_open (&new_conn, address, iface, port) < 0)
    {
      return -2;
    }

  if ((size = lwes_net_send_bytes (&new_conn, bytes, len)) < 0)
    {
      return -3;
    }

  /* There's not much we can do about a close error, and it would mask a
     successful send, so just ignore a bad return value */
  (void) lwes_net_close (&new_conn);

  return size;
}

int
lwes_net_recv_bind
  (struct lwes_net_connection *conn)
{
  int arg = -1;

  if (conn == NULL)
    {
      return -1;
    }

  if ( conn->hasJoined != 1 )
    {
      int i = 0;

      /* set a socket option such that the socket can be reused */
      if ( setsockopt (conn->socketfd,
                       SOL_SOCKET,
                       SO_REUSEADDR,
                       (void*)&arg,
                       sizeof(arg)) < 0 )
        {
          return -2;
        }

      /* try for as big a buffer as possible, start at 100*MAX_MSG_SIZE
       * and work down, if you can't get a buffer of at least one max
       * message, error out */
      for ( i = 100 ; i > 0 ; i-- )
        {
          arg = MAX_MSG_SIZE*i;
          if (setsockopt(conn->socketfd, SOL_SOCKET, SO_RCVBUF,
                         (void*)&arg,sizeof(arg)) == 0)
            {
              break;
            }
        }
      if ( i == 0 )
        {
          return -3;
        }

      /* if we are not in a multicast connection, then the address we are
         using to receive on will be wrong, so we will have to set
         it to INADDR_ANY */
      if (!IN_MULTICAST (ntohl (conn->mcast_addr.sin_addr.s_addr)))
        {
          conn->mcast_addr.sin_addr.s_addr = htonl (INADDR_ANY);
          conn->hasJoined = 1;
        }

      /* bind to the socket */
      if (bind (conn->socketfd,
                (struct sockaddr * )&(conn->mcast_addr),
                sizeof(conn->mcast_addr) ) < 0 )
        {
          return -4;
        }

      /* add the multicast channel given */
      if (IN_MULTICAST (ntohl (conn->mcast_addr.sin_addr.s_addr)))
        {
          if (setsockopt (conn->socketfd,
                          IPPROTO_IP,
                          IP_ADD_MEMBERSHIP,
                          (void*)&(conn->mreq),
                          sizeof(conn->mreq)) < 0 )
            {
              return -5;
            }
          conn->hasJoined = 1;
        }
    }

  return 0;
}

int
lwes_net_recv_bytes
  (struct lwes_net_connection *conn,
   LWES_BYTE_P bytes,
   size_t len)
{
  int ret = 0;
  int flags = 0;

  if (conn == NULL || bytes == NULL)
    {
      return -1;
    }

  if ( (ret = lwes_net_recv_bind (conn)) < 0) 
    {
      return ret;
    }

  ret = recvfrom (conn->socketfd,
                  bytes,
                  len,
                  flags,
                  (struct sockaddr *)&(conn->sender_ip_addr),
                  (socklen_t *)&(conn->sender_ip_socket_size));
  return ret;
}

int
lwes_net_recv_bytes_by
  (struct lwes_net_connection *conn, 
   LWES_BYTE_P bytes,
   size_t len,
   unsigned int timeout_ms)
{
  int ret = 0;
  int flags = 0;
  struct timeval timeout;
  fd_set read_sel;

  if (conn == NULL || bytes == NULL)
    {
      return -1;
    }

  if ((ret = lwes_net_recv_bind (conn)) < 0)
    {
      return ret;
    }

  timeout.tv_sec=timeout_ms/1000;
  timeout.tv_usec=(timeout_ms%1000)*1000;

  FD_ZERO(&read_sel);
  FD_SET(conn->socketfd, &read_sel);

  /* Just wait once, as we *should* get the packet as a chunk */
  ret = select (conn->socketfd+1, &read_sel, NULL, NULL, &timeout);
  if (ret <= 0)
    {
      return -2;
    }

  ret = recvfrom (conn->socketfd,
                  bytes,
                  len,
                  flags,
                  (struct sockaddr *)&(conn->sender_ip_addr),
                  (socklen_t *)&(conn->sender_ip_socket_size));
  return ret;
}
