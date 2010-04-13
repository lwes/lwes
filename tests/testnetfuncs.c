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
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>

#include "lwes_types.h"
#include "lwes_net_functions.h"

/* #define DEBUG 1 */

/*=====================================================================*
 * Substituted functions to test failures of system and library calls  *
 * as well as for testing just the code represented by this file       *
 *=====================================================================*/

# define GETSOCKOPT_NO_ERROR 0xf0f0f0f0
static int getsockopt_error_when = GETSOCKOPT_NO_ERROR;
int
my_getsockopt
  (int s,
   int level,
   int optname,
   void *optval,
   socklen_t *optlen)
{
  /* error on specific optnames, so if it's not the optname we want error
     on make the actual call */
  if (getsockopt_error_when != optname)
    {
      return getsockopt (s, level, optname, optval, optlen);
    }
  return -1;
}

# define SETSOCKOPT_NO_ERROR 0xf0f0f0f0
static int setsockopt_error_when       = SETSOCKOPT_NO_ERROR;
int
my_setsockopt
  (int s,
   int level,
   int  optname,
   const void  *optval,
   socklen_t optlen)
{
  /* error on specific optnames, so if it's not the optname we want error
     on make the actual call */
  if (setsockopt_error_when != optname)
    {
      return setsockopt (s, level, optname, optval, optlen);
    }
  return -1;
}

static int socket_error = 0;
int
my_socket
  (int domain,
   int type,
   int protocol)
{
  if (socket_error == 0)
    {
      return socket (domain, type, protocol);
    }
  return -1;
}

static int bind_error = 0;
int
my_bind
  (int sockfd,
   struct sockaddr *my_addr,
   socklen_t addrlen)
{
  if (bind_error == 0)
    {
      return bind (sockfd, my_addr, addrlen);
    }
  return -1;

}

static int sendto_error = 0;
ssize_t
my_sendto
  (int  s,
   const void *buf,
   size_t len,
   int flags,
   const struct sockaddr *to,
   socklen_t tolen)
{
  if (sendto_error == 0)
    {
      return sendto (s, buf, len, flags, to, tolen);
    }
  return -1;
}

#define getsockopt my_getsockopt
#define setsockopt my_setsockopt
#define socket my_socket
#define bind my_bind
#define sendto my_sendto

/*=====================================================================*
 * The actual file we are testing is included here                     *
 *=====================================================================*/
#include "lwes_net_functions.c"

#undef getsockopt
#undef setsockopt
#undef socket
#undef bind
#undef sendto

const int    mcast_port      = 12345;
const char * mcast_ip        = "224.0.0.254";
const char * mcast_iface     = 0;
const char * mcast_iface2    = "127.0.0.1";
unsigned int num_to_send     = 10;

static void sender    (const char *ip, const int port, const char *iface);
static void sender_to (const char *ip, const int port, const char *iface);
static void receiver  (const char *ip, const int port, const char *iface);
static void receiver_with_big_timeout
  (const char *ip, const int port, const char *iface);
static void receiver_with_small_timeout
  (const char *ip, const int port, const char *iface);

void test_ttl_functions (void)
{
  struct lwes_net_connection connection;
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == 0);

  /* test success of get */
  assert (lwes_net_get_ttl (&connection) == 1);

  /* now test failure of getsockopt in get */
  getsockopt_error_when = IP_MULTICAST_TTL;
  assert (lwes_net_get_ttl (&connection) < 0);
  getsockopt_error_when = GETSOCKOPT_NO_ERROR;

  /* test success of set */
  assert (lwes_net_set_ttl (&connection, 3) == 0);
  /* check success of set */
  assert (lwes_net_get_ttl (&connection) == 3);

  /* now test failure of setsockopt in set */
  setsockopt_error_when = IP_MULTICAST_TTL;
  assert (lwes_net_set_ttl (&connection,5) < 0);
  setsockopt_error_when = GETSOCKOPT_NO_ERROR;
  assert (lwes_net_get_ttl (&connection) == 3);
}

void test_socket_function_failures (void)
{
  struct lwes_net_connection connection;
  LWES_BYTE buffer[500];

  /* socket system call failure */
  socket_error = 1;
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == -2);
  socket_error = 0;

  /* setsockopt of IP_MULTICAST_IF failure */
  setsockopt_error_when = IP_MULTICAST_IF;
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface2,
                         (int)mcast_port) == -3);
  setsockopt_error_when = SETSOCKOPT_NO_ERROR;

  /* setsockopt of SO_SNDBUF failure */
  setsockopt_error_when = SO_SNDBUF;
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface2,
                         (int)mcast_port) == -4);
  setsockopt_error_when = SETSOCKOPT_NO_ERROR;

  /* test failures in recv_bind */

  /* need a successful open */
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == 0);

  /* First setsockopt SO_REUSEADDR failure */
  setsockopt_error_when = SO_REUSEADDR;
  assert (lwes_net_recv_bind (&connection) == -2);
  setsockopt_error_when = SETSOCKOPT_NO_ERROR;
  
  /* Second setsockopt SO_RCVBUF failure */
  setsockopt_error_when = SO_RCVBUF;
  assert (lwes_net_recv_bind (&connection) == -3);
  setsockopt_error_when = SETSOCKOPT_NO_ERROR;
  
  /* Third bind () failure */
  bind_error = 1;
  assert (lwes_net_recv_bind (&connection) == -4);
  bind_error = 0;

  /* Fourth IP_ADD_MEMBERSHIP failure */
  setsockopt_error_when = IP_ADD_MEMBERSHIP;
  assert (lwes_net_recv_bind (&connection) == -5);
  setsockopt_error_when = SETSOCKOPT_NO_ERROR;

  assert (lwes_net_close (&connection) == 0);

  /* successful so we can test close failure */
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == 0);

  assert (lwes_net_recv_bind (&connection) == 0);

  setsockopt_error_when = IP_DROP_MEMBERSHIP;
  assert (lwes_net_close (&connection) == -2);
  setsockopt_error_when = SETSOCKOPT_NO_ERROR;

  assert (lwes_net_close (&connection) == 0);

  /* Also want to test bind failures in recv functions */
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == 0);
  bind_error = 1;
  assert (lwes_net_recv_bytes (&connection, buffer, 500) == -4);
  assert (lwes_net_recv_bytes_by (&connection, buffer, 500, 10000) == -4);
  bind_error = 0;

  assert (lwes_net_close (&connection) == 0);
}

void test_null_args (void)
{
  LWES_BYTE buffer[500];
  unsigned int i;
  struct lwes_net_connection connection;

  /* initialize to nothing */
  for (i = 0; i < 500; i++)
    buffer[i]=(LWES_BYTE)0;

  /* now set 45 bytes */
  for (i = 0; i < 45; i++)
    {
      buffer[i]=(LWES_BYTE)i;
    }


  /* test NULL connection to all calls */

  assert (lwes_net_open (NULL, 
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == -1);
  assert (lwes_net_close (NULL) == -1);

  assert (lwes_net_get_ttl (NULL) == -1);

  assert (lwes_net_set_ttl (NULL, 5) == -1);

  assert (lwes_net_get_sock_fd (NULL) == -1);

  assert (lwes_net_send_bytes (NULL, buffer, 45) == -1);
  assert (lwes_net_send_bytes (&connection, NULL, 45) == -1);

  assert (lwes_net_sendto_bytes (NULL,
                                 (char*)mcast_ip,
                                 (char*)mcast_iface,
                                 (int)mcast_port,
                                 buffer,
                                 45) == -1);

  assert (lwes_net_sendto_bytes (&connection,
                                 (char*)mcast_ip,
                                 (char*)mcast_iface,
                                 (int)mcast_port,
                                 NULL,
                                 45) == -1);

  assert (lwes_net_recv_bind (NULL) == -1);

  assert (lwes_net_recv_bytes (NULL, buffer, 500) == -1);
  assert (lwes_net_recv_bytes (&connection, NULL, 500) == -1);

  assert (lwes_net_recv_bytes_by (NULL, buffer, 500, 10000) == -1);
  assert (lwes_net_recv_bytes_by (&connection, NULL, 500, 10000) == -1);
}

static void
test_sendto_bytes_failures (void)
{
  LWES_BYTE buffer[500];
  struct lwes_net_connection connection;
  unsigned int i;
  int new_port = mcast_port+1;

  /* freebsd seems to bail on setsockopt for the interface if we
     have a null address, so test with two real addresses */
  char hostname[500];
  char ipaddr[500];
  struct hostent *hptr;
  assert (gethostname (hostname, 500) == 0);
  assert ((hptr = gethostbyname (hostname)) != NULL);
  switch (hptr->h_addrtype)
    {
      case AF_INET:
        {
          char **pptr;
          pptr = hptr->h_addr_list;
          inet_ntop (hptr->h_addrtype, *pptr, ipaddr, 500);
        }
        break;
    }

  /* initialize to nothing */
  for (i = 0; i < 500; i++)
    buffer[i]=(LWES_BYTE)0;

  /* now set 45 bytes */
  for (i = 0; i < 45; i++)
    {
      buffer[i]=(LWES_BYTE)i;
    }

  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == 0);

  /* lwes_net_open failure */
  socket_error = 1;
  assert (lwes_net_sendto_bytes (&connection,
                                 (char*)mcast_ip,
                                 (char*)ipaddr,
                                 (int)new_port,
                                 buffer,45) == -2);
  socket_error = 0;

  /* lwes_net_send_bytes failure */
  sendto_error = 1;
  assert (lwes_net_sendto_bytes (&connection,
                                 (char*)mcast_ip,
                                 (char*)ipaddr,
                                 (int)new_port,
                                 buffer,45) == -3);
  sendto_error = 0;

  assert (lwes_net_close (&connection) == 0);
}

void test_sendto (void)
{
  pid_t pid;
  int status;
  int tmp_port = mcast_port+1;
  pid = fork ();
  assert (pid >= 0);

  if ( pid == 0 )
    {
      /* child receives */
      receiver (mcast_ip, tmp_port, mcast_iface2);
      exit (0);
    }
  else
    {
      /* parent sends */
      sleep (1);
      sender_to (mcast_ip, mcast_port, mcast_iface2);
      /* then waits for child */
      assert ( wait (&status) == pid );
    }
}

void test_sendto_unicast (void)
{
  pid_t pid;
  int status;
  int tmp_port = mcast_port+1;
  pid = fork ();
  assert (pid >= 0);

  if ( pid == 0 )
    {
      /* child receives */
      receiver (mcast_iface2, tmp_port, mcast_iface2);
      exit (0);
    }
  else
    {
      /* parent sends */
      sleep (1);
      sender_to (mcast_iface2, mcast_port, mcast_iface2);
      /* then waits for child */
      assert ( wait (&status) == pid );
    }
}

void test_wo_interface (void)
{
  pid_t pid;
  int status;
  pid = fork ();
  assert (pid >= 0);

  if ( pid == 0 )
    {
      /* child receives */
      receiver (mcast_ip, mcast_port, mcast_iface);
      exit (0);
    }
  else
    {
      /* parent sends */
      sleep (1);
      sender (mcast_ip, mcast_port, mcast_iface);
      /* then waits for child */
      assert ( wait (&status) == pid );
    }
}

void test_w_interface  (void)
{
  pid_t pid;
  int status;
  pid = fork ();
  assert ( pid >= 0 );

  if ( pid == 0 )
    {
      /* child receives */
      receiver (mcast_ip, mcast_port, mcast_iface2);
      exit (0);
    }
  else
    {
      /* parent sends */
      sleep (1);
      sender (mcast_ip, mcast_port, mcast_iface2);
      /* then waits for child */
      assert ( wait (&status) == pid );
    }
}

void test_w_timeout_good (void)
{
  pid_t pid;
  int status;
  pid = fork ();
  assert ( pid >= 0 );

  if ( pid == 0 )
    {
      /* child receives */
      receiver_with_big_timeout (mcast_ip, mcast_port, mcast_iface);
      exit (0);
    }
  else
    {
      /* parent sends */
      sleep (1);
      sender (mcast_ip, mcast_port, mcast_iface);
      /* then waits for child */
      assert ( wait (&status) == pid );
    }
}

void test_w_timeout_bad (void)
{
  pid_t pid;
  int status;
  pid = fork ();
  assert ( pid >= 0 );

  if ( pid == 0 )
    {
      /* child receives */
      receiver_with_small_timeout (mcast_ip, mcast_port, mcast_iface);
      exit (0);
    }
  else
    {
      /* parent sends */
      sleep (3);
      sender (mcast_ip, mcast_port, mcast_iface);
      /* then waits for child */
      assert ( wait (&status) == pid );
    }
}

static void
sender (const char *ip, const int port, const char *iface)
{
  LWES_BYTE buffer[500];
  struct lwes_net_connection connection;
  unsigned int i;

  /* initialize to nothing */
  for (i = 0; i < 500; i++)
    buffer[i]=(LWES_BYTE)0;

  /* now set 45 bytes */
  for (i = 0; i < 45; i++)
    {
      buffer[i]=(LWES_BYTE)i;
    }

  assert ( lwes_net_open (&connection,
                          (char*)ip,
                          (char*)iface,
                          (int)port) == 0 );

  for ( i = 0 ; i < num_to_send; i++ )
    {
      assert ( lwes_net_send_bytes (&connection,buffer,45) > 0 );
    }

  lwes_net_close (&connection);
}

static void
sender_to (const char *ip, const int port, const char *iface)
{
  LWES_BYTE buffer[500];
  struct lwes_net_connection connection;
  unsigned int i;
  int new_port = port+1;

  /* freebsd seems to bail on setsockopt for the interface if we
     have a null address, so test with two real addresses */
  char hostname[500];
  char ipaddr[500];
  struct hostent *hptr;
  assert (gethostname (hostname, 500) == 0);
  assert ((hptr = gethostbyname (hostname)) != NULL);
  switch (hptr->h_addrtype)
    {
      case AF_INET:
        {
          char **pptr;
          pptr = hptr->h_addr_list;
          inet_ntop (hptr->h_addrtype, *pptr, ipaddr, 500);
        }
        break;
    }

  /* initialize to nothing */
  for (i = 0; i < 500; i++)
    buffer[i]=(LWES_BYTE)0;

  /* now set 45 bytes */
  for (i = 0; i < 45; i++)
    {
      buffer[i]=(LWES_BYTE)i;
    }

  /* open up on the null interface, but sendto a different interface */
  assert ( lwes_net_open (&connection,
                          (char*)ip,
                          (char*)ipaddr,
                          (int)port) == 0 );

  for ( i = 0 ; i < num_to_send; i++ )
    {
      assert (lwes_net_sendto_bytes (&connection,
                                     (char*)ip,
                                     (char*)iface,
                                     (int)new_port,
                                     buffer,45) > 0);
    }

  lwes_net_close (&connection);
}

static void
receiver (const char *ip, const int port, const char *iface)
{
  LWES_BYTE buffer[500];
  struct lwes_net_connection connection;
  unsigned int i;

  for (i = 0; i < 500; i++)
    buffer[i]=(LWES_BYTE)0;

  assert ( lwes_net_open (&connection,
                          (char*)ip,
                          (char*)iface,
                          (int)port) == 0 );

  for ( i = 0 ; i < num_to_send ; i++ )
    {
      int j;
      int n;
      assert ((n = lwes_net_recv_bytes (&connection,buffer,500)) > 0 );
      assert ( n == 45 );
      for ( j = 0 ; j < 45 ; j++ )
        {
          assert ( buffer[j] == j );
        }
    }

  lwes_net_close (&connection);
}

static void
receiver_with_big_timeout
  (const char *ip, const int port, const char *iface)
{
  LWES_BYTE buffer[500];
  struct lwes_net_connection connection;
  unsigned int i;
  unsigned int timeout_ms = 10000;

  for (i = 0; i < 500; i++)
    buffer[i]=(LWES_BYTE)0;

  assert (lwes_net_open (&connection,
                         (char*)ip,
                         (char*)iface,
                         (int)port) == 0);

  for ( i = 0 ; i < num_to_send ; i++ )
    {
      int j;
      int n;
      assert ((n = lwes_net_recv_bytes_by (&connection,
                                           buffer,
                                           500,
                                           timeout_ms)) > 0 );
      assert ( n == 45 );
      for ( j = 0 ; j < 45 ; j++ )
        {
          assert ( buffer[j] == j );
        }
    }

  lwes_net_close (&connection);
}

static void
receiver_with_small_timeout
  (const char *ip, const int port, const char *iface)
{
  LWES_BYTE buffer[500];
  struct lwes_net_connection connection;
  unsigned int i;
  unsigned int timeout_ms = 1;
  int n;

  for (i = 0; i < 500; i++)
    buffer[i]=(LWES_BYTE)0;

  assert (lwes_net_open (&connection,
                         (char*)ip,
                         (char*)iface,
                         (int)port) == 0);

  sleep (1);

  assert ((n = lwes_net_recv_bytes_by (&connection,
                                       buffer,
                                       LWES_MSG_SIZE_MAX,
                                       timeout_ms)) < 0);

  lwes_net_close (&connection);
}

static void
test_get_sock_fd (void)
{
  struct lwes_net_connection connection;
  assert (lwes_net_open (&connection,
                         (char*)mcast_ip,
                         (char*)mcast_iface,
                         (int)mcast_port) == 0);

  assert (lwes_net_get_sock_fd (&connection) == connection.socketfd);

  lwes_net_close (&connection);
}

static void 
test_large_send ()
{
  struct lwes_net_connection connection;
  unsigned int i;
  LWES_BYTE buffer [LWES_MSG_SIZE_MAX];

  for (i=0; i < LWES_MSG_SIZE_MAX; i++)
    {
      buffer[i] = (LWES_BYTE) i;
    }

  assert ( lwes_net_open (&connection,
                          (char *)mcast_ip,
                          (char *)mcast_iface,
                          (int)mcast_port)
           == 0 );
  assert ( lwes_net_send_bytes (&connection, buffer, LWES_MSG_SIZE_MAX)  > 0); 
  lwes_net_close (&connection);  
}


int main (void)
{

  printf
    ("WARNING -\n");
  printf
    ("WARNING - this test may block forever if you have iptables or\n");
  printf
    ("WARNING - similar firewall software on your computer.  \n");
  printf
    ("WARNING - allow UDP traffic on port %d to fix it.\n", mcast_port);
  printf
    ("WARNING -\n");


#if DEBUG
  printf ("test_wo_interface\n");
#endif
  test_wo_interface ();

#if DEBUG
  printf ("test_w_interface\n");
#endif
  test_w_interface ();

#if DEBUG
  printf ("test_w_timeout_good\n");
#endif
  test_w_timeout_good ();

#if DEBUG
  printf ("test_w_timeout_bad\n");
#endif
  test_w_timeout_bad ();

#if DEBUG
  printf ("test_sendto\n");
#endif
  test_sendto ();

#if DEBUG
  printf ("test_sendto_unicast\n");
#endif
  test_sendto_unicast ();

#if DEBUG
  printf ("test_socket_function_failures\n");
#endif
  test_socket_function_failures ();

#if DEBUG
  printf ("test_ttl_functions\n");
#endif
  test_ttl_functions ();

#if DEBUG
  printf ("test_null_args\n");
#endif
  test_null_args ();

#if DEBUG
  printf ("test_get_sock_fd\n");
#endif
  test_get_sock_fd ();

#if DEBUG
  printf ("test_sendto_bytes_failures\n");
#endif
  test_sendto_bytes_failures ();

#if DEBUG
  printf ("test_large_send\n");
#endif
  test_large_send ();

  return 0;
}


