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
  struct in_addr validated_address;
  struct in_addr validated_iface;

  /* error out on a NULL connection */
  if (conn == NULL)
    {
      return -1;
    }
  /* validate the arguments */
  if (inet_aton (address, &validated_address) == 0)
    {
      return -2;
    }
  if (iface != NULL && inet_aton (iface, &validated_iface) == 0)
    {
      return -3;
    }

  /* set up the address structure */
  memset((char *) &conn->ip_addr, 0, sizeof(conn->ip_addr));
  conn->ip_addr.sin_family      = AF_INET;
  conn->ip_addr.sin_addr        = validated_address;
  conn->ip_addr.sin_port        = htons ((short)port);
  conn->has_bound               = 0;
  conn->has_joined              = 0;
  if (IN_MULTICAST (ntohl (conn->ip_addr.sin_addr.s_addr)))
    {
      conn->is_multicast = 1;
    }
  else
    {
      conn->is_multicast = 0;
    }

  /* construct the socket */
  if ( (conn->socketfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
      return -4;
    }

  if (conn->is_multicast)
    {
      /* if the address we are emitting on is a multicast address we
         set the sockopt which uses that interface to emit on, this doesn't
         work for unicast */

      conn->mreq.imr_multiaddr = conn->ip_addr.sin_addr;
      if (iface != NULL)
        {
          conn->mreq.imr_interface = validated_iface;
          if (setsockopt(conn->socketfd, IPPROTO_IP, IP_MULTICAST_IF,
                         (char *)&validated_iface,
                         sizeof(validated_iface)) < 0 )
            {
              return -5;
            }
        }
      else
        {
          conn->mreq.imr_interface.s_addr = htonl (INADDR_ANY);
        }
    }

  /* Setting the value for SO_SNDBF , trying for 10*MAX_MSG_SIZE */
  int i;
  for ( i = 10 ; i > 0 ; i-- )
    {
      int sndbufsize = MAX_MSG_SIZE*i;
      if (setsockopt (conn->socketfd, SOL_SOCKET, SO_SNDBUF,
                      (void*)&sndbufsize,sizeof(sndbufsize)) == 0)
        {
          break;
        }
    }
  if ( i == 0 )
    {
      return -6;
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

  /* check has_joined first, as we also "join" a unicast channel, so may
     need to do cleanup here someday */
  if ( conn->has_joined )
    {
      /* if we are using a multicast address we need to drop the membership
         so that upstream routers get the message */
      if ( conn->is_multicast )
        {
          /* drop the multicast channel */
          if (setsockopt (conn->socketfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                          (void*)&(conn->mreq), sizeof(conn->mreq)) < 0 )
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

      if (conn->is_multicast)
        {
          if (getsockopt (conn->socketfd, IPPROTO_IP, IP_MULTICAST_TTL,
                          &ttl, &len) < 0)
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
      if (conn->is_multicast)
        {
          if (setsockopt (conn->socketfd, IPPROTO_IP, IP_MULTICAST_TTL,
                          (void*)&ttl, sizeof (ttl)) < 0)
            {
              return -2;
            }
        }

      return 0;
    }

  return -1;
}

int
lwes_net_get_rcvbuf
  (struct lwes_net_connection *conn)
{
  if (conn != NULL)
    {
      int rcvbufsize = 0;
      socklen_t len = sizeof (rcvbufsize);
      if (getsockopt (conn->socketfd, SOL_SOCKET, SO_RCVBUF,
                      &rcvbufsize, &len) < 0)
        {
          return -2;
        }

      return rcvbufsize;
    }

  return -1;
}

int
lwes_net_set_rcvbuf
  (struct lwes_net_connection *conn, int new_rcvbuf)
{
  if (conn != NULL)
    {
      int rcvbuf = new_rcvbuf;
      if (setsockopt (conn->socketfd, SOL_SOCKET, SO_RCVBUF,
                      &rcvbuf, sizeof (rcvbuf)) < 0)
        {
          return -2;
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
lwes_net_is_multicast
  (struct lwes_net_connection *conn)
{
  if (conn == NULL)
    {
      return 0;
    }
  return conn->is_multicast;
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
      (conn->socketfd, bytes, len, 0,
       (struct sockaddr * )&(conn->ip_addr), sizeof(conn->ip_addr));

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
  int on = 1;

  if (conn == NULL)
    {
      return -1;
    }

  if ( conn->has_bound != 1 )
    {
      /* set a socket option such that the socket can be reused */
      if ( setsockopt (conn->socketfd, SOL_SOCKET, SO_REUSEADDR,
                       (void*)&on, sizeof(on)) < 0 )
        {
          return -2;
        }
#ifdef HAVE_SO_REUSEPORT
      if (! conn->is_multicast )
        {
          /* Set port for reuse for unicast packets. */
          if ( setsockopt(conn->socketfd, SOL_SOCKET, SO_REUSEPORT,
                          &on, sizeof(on)) < 0 )
            {
              return -3;
            }
        }
#endif

      /* check to see if the rcvbuf is less than 100*MAX_MSG_SIZE, if it's
       * not just continue, as it's probably plenty big
       */
      if (lwes_net_get_rcvbuf (conn) < (int)(100*MAX_MSG_SIZE))
        {
          int i = 0;
          /* try for as big a buffer as possible, start at 100*MAX_MSG_SIZE
           * and work down, if you can't get a buffer of at least one max
           * message, error out */
          for ( i = 100 ; i > 0 ; i-- )
            {
              int rcvbufsize  = MAX_MSG_SIZE*i;
              if (lwes_net_set_rcvbuf (conn, rcvbufsize) == 0)
                {
                  break;
                }
            }
          if ( i == 0 )
            {
              return -4;
            }
        }

      /* if we are not in a multicast connection, then the address we are
         using to receive on will be wrong, so we will have to set
         it to INADDR_ANY */
      if (! conn->is_multicast)
        {
          conn->ip_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        }

      /* bind to the socket */
      if (bind (conn->socketfd,
                (struct sockaddr * )&(conn->ip_addr),
                sizeof(conn->ip_addr) ) < 0 )
        {
          return -5;
        }
      conn->has_bound = 1;

      /* add the multicast channel given */
      if (conn->is_multicast)
        {
          if (setsockopt (conn->socketfd,
                          IPPROTO_IP,
                          IP_ADD_MEMBERSHIP,
                          (void*)&(conn->mreq),
                          sizeof(conn->mreq)) < 0 )
            {
              return -6;
            }
          conn->has_joined = 1;
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
