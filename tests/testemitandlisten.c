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

#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>

#include "lwes_net_functions.h"
#include "lwes_marshall_functions.h"
#include "lwes_emitter.h"
#include "lwes_listener.h"

/* wrap malloc and other functions to cause test memory problems */

static size_t null_at = 0;
static size_t malloc_count = 0;

static void *my_malloc (size_t size)
{
  void *ret = NULL;
  malloc_count++;
  if ( malloc_count != null_at )
    {
      ret = malloc (size);
    }
  return ret;
}

static int time_past = 0;
static int time_future = 0;
static time_t my_time (time_t *t)
{
  if (time_past == 0 && time_future == 0)
    {
      return time (t);
    }
  else
    {
      if (time_past != 0)
        {
          return (time_t)(time (t) - time_past);
        }
      else
        {
          return (time_t)(time (t) + time_future);
        }
    }
}

static int lwes_net_open_error = 0;
static int
my_lwes_net_open
  (struct lwes_net_connection *conn,
   const char *address, 
   const char *iface,
   int port)
{
  if (lwes_net_open_error == 0)
    {
      return lwes_net_open (conn, address, iface, port);
    }
  return -1;
}

static int lwes_net_set_ttl_error = 0;
static int
my_lwes_net_set_ttl
  (struct lwes_net_connection *conn, int new_ttl)
{
  if (lwes_net_set_ttl_error == 0)
    {
      return lwes_net_set_ttl (conn, new_ttl);
    }
  return -1;
}

static int lwes_net_send_bytes_error = 0;
static int
my_lwes_net_send_bytes
  (struct lwes_net_connection *conn,
   LWES_BYTE_P bytes,
   size_t len)
{
  if (lwes_net_send_bytes_error == 0)
    {
      return lwes_net_send_bytes (conn, bytes, len);
    }
  return -1;
}

static int lwes_net_sendto_bytes_error = 0;
static int
my_lwes_net_sendto_bytes
  (struct lwes_net_connection *conn,
   char *address,
   char *iface,
   int port,
   LWES_BYTE_P bytes,
   size_t len)
{
  if (lwes_net_sendto_bytes_error == 0)
    {
      return lwes_net_sendto_bytes (conn, address, iface, port, bytes, len);
    }
  return -1;
}

static int lwes_net_recv_bytes_error = 0;
static int
my_lwes_net_recv_bytes
  (struct lwes_net_connection *conn,
   LWES_BYTE_P bytes,
   size_t len)
{
  if (lwes_net_recv_bytes_error == 0)
    {
      return lwes_net_recv_bytes (conn, bytes, len);
    }
  return -1;
}

static int lwes_event_to_bytes_error = 0;
static int
my_lwes_event_to_bytes
  (struct lwes_event *event,
   LWES_BYTE_P bytes,
   size_t num_bytes,
   size_t offset)
{
  if (lwes_event_to_bytes_error == 0)
    {
      return lwes_event_to_bytes (event, bytes, num_bytes, offset);
    }
  return -1;
}

static size_t marshall_U_INT_16_fail_at = 0;
static size_t marshall_U_INT_16_count = 0;

static int
my_marshall_U_INT_16
  (LWES_U_INT_16     anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset)
{
  ++marshall_U_INT_16_count;
  if ( marshall_U_INT_16_count != marshall_U_INT_16_fail_at)
    {
      return marshall_U_INT_16 (anInt, bytes, length, offset);
    }
  return 0;
}

#define malloc my_malloc
#define time my_time
#define lwes_net_open my_lwes_net_open
#define lwes_net_set_ttl my_lwes_net_set_ttl
#define lwes_net_sendto_bytes my_lwes_net_sendto_bytes
#define lwes_net_send_bytes my_lwes_net_send_bytes
#define lwes_net_recv_bytes my_lwes_net_recv_bytes
#define lwes_event_to_bytes my_lwes_event_to_bytes
#define marshall_U_INT_16 my_marshall_U_INT_16

#include "lwes_emitter.c"
#include "lwes_listener.c"

#undef malloc
#undef time
#undef lwes_net_open
#undef lwes_net_set_ttl
#undef lwes_net_sendto_bytes
#undef lwes_net_send_bytes
#undef lwes_net_recv_bytes
#undef lwes_event_to_bytes
#undef marshall_U_INT_16

const char *esffile         = "testeventtypedb.esf";
LWES_SHORT_STRING eventname = (LWES_SHORT_STRING)"TypeChecker";
LWES_SHORT_STRING key01     = (LWES_SHORT_STRING)"aString";
LWES_LONG_STRING  value01   = (LWES_LONG_STRING)"http://www.test.com";
LWES_SHORT_STRING key02     = (LWES_SHORT_STRING)"aBoolean";
LWES_BOOLEAN      value02   = (LWES_BOOLEAN)1;
LWES_SHORT_STRING key03     = (LWES_SHORT_STRING)"anIPAddress";
LWES_IP_ADDR      value03;
LWES_SHORT_STRING key04     = (LWES_SHORT_STRING)"aUInt16";
LWES_U_INT_16     value04   = (LWES_U_INT_16)65535;
LWES_SHORT_STRING key05     = (LWES_SHORT_STRING)"anInt16";
LWES_INT_16       value05   = (LWES_INT_16)-1;
LWES_SHORT_STRING key06     = (LWES_SHORT_STRING)"aUInt32";
LWES_U_INT_32     value06   = (LWES_U_INT_32)0xffffffffL;
LWES_SHORT_STRING key07     = (LWES_SHORT_STRING)"anInt32";
LWES_INT_32       value07   = (LWES_INT_32)-1;
LWES_SHORT_STRING key08     = (LWES_SHORT_STRING)"aUInt64";
LWES_U_INT_64     value08   = (LWES_U_INT_64)0xffffffffffffffffULL;
LWES_SHORT_STRING key09     = (LWES_SHORT_STRING)"anInt64";
LWES_INT_64       value09   = (LWES_INT_64)-1;

const int   mcast_port      = 12345;
const char *mcast_ip        = "224.0.0.254";
/*const char *mcast_ip        = "127.0.0.1";*/
const char *mcast_iface     = 0;
const char *mcast_iface2    = "127.0.0.1";

static void emitter     (const char *ip, const int port, const char *iface);
static void emitter_to  (const char *ip,
                         const int port,
                         const char *iface,
                         const char *n_ip,
                         const int n_port,
                         const char *n_iface);
static void listener    (const char *ip, const int port, const char *iface);
static void listener_to (const char *ip,
                         const int   port,
                         const char *iface,
                         const char *n_ip,
                         const int   n_port,
                         const char *n_iface);

static void test_emit (void)
{
  pid_t pid;
  int status;

  pid = fork ();
  assert ( pid >= 0 );

  if ( pid == 0 )
    {
      /* child listens */
      listener (mcast_ip, mcast_port, mcast_iface);
      exit (0);
    }
  else
    {
      /* parent emits */
      sleep (1);
      emitter (mcast_ip, mcast_port, mcast_iface);
      /* then waits for child */
      assert ( wait (&status) == pid );
      assert ( status == 0 );
    }
}

static void test_emitto (void)
{
  pid_t pid;
  int status;

  /* freebsd seems to bail on setsockopt for the interface if we
     have a null address, so test with two real addresses */
  char hostname[500];
  char new_iface[500];
  const int new_port = mcast_port+1;
  struct hostent *hptr;
  assert (gethostname (hostname, 500) == 0);
  assert ((hptr = gethostbyname (hostname)) != NULL);
  switch (hptr->h_addrtype)
    {
      case AF_INET:
        {
          char **pptr;
          pptr = hptr->h_addr_list;
          inet_ntop (hptr->h_addrtype, *pptr, new_iface, 500);
        }
        break;
    }

  pid = fork ();
  assert ( pid >= 0 );

  if ( pid == 0 )
    {
      /* child listens */
      listener_to (mcast_ip, mcast_port, mcast_iface,
                   mcast_ip, new_port,   new_iface);
      exit (0);
    }
  else
    {
      /* parent emits */
      sleep (1);
      emitter_to (mcast_ip, mcast_port, mcast_iface,
                  mcast_ip, new_port,   new_iface);
      /* then waits for child */
      assert ( wait (&status) == pid );
      assert ( status == 0 );
    }
}

static void test_listener_failures (void)
{
  /* open failures */
  {
    /* 1: malloc failure for listener */
    malloc_count = 0;
    null_at      = 1;
    assert (lwes_listener_create ((char *) mcast_ip,
                                  (char *) mcast_iface,
                                  (int) mcast_port) == NULL);
    /* 2: malloc failure for buffer */
    malloc_count = 0;
    null_at      = 2;
    assert (lwes_listener_create ((char *) mcast_ip,
                                  (char *) mcast_iface,
                                  (int) mcast_port) == NULL);
    /* 3: malloc failure for dtmp */
    malloc_count = 0;
    null_at      = 3;
    assert (lwes_listener_create ((char *) mcast_ip,
                                  (char *) mcast_iface,
                                  (int) mcast_port) == NULL);
    /* 4: lwes_net_open failure */
    lwes_net_open_error = 1;
    assert (lwes_listener_create ((char *) mcast_ip,
                                  (char *) mcast_iface,
                                  (int) mcast_port) == NULL);
    lwes_net_open_error = 0;
  }

  /* recv failures */
  {
    struct lwes_listener *listener;
    struct lwes_event *event;
    struct lwes_event_type_db *db = NULL;

    listener = lwes_listener_create ((char *) mcast_ip,
                                     (char *) mcast_iface,
                                     (int) mcast_port);
    assert (listener != NULL);
    event  = lwes_event_create_no_name ( db );
    assert (event != NULL);

    lwes_net_recv_bytes_error = 1;
    assert (lwes_listener_recv (listener, event) != 0);
    lwes_net_recv_bytes_error = 0;

    lwes_event_destroy(event);

    lwes_listener_destroy(listener);
  }

  /* recv_by timeout failure */
  {
    struct lwes_listener *listener;
    struct lwes_event *event;
    struct lwes_event_type_db *db = NULL;

    listener = lwes_listener_create ((char *) mcast_ip,
                                     (char *) mcast_iface,
                                     (int) mcast_port);
    assert (listener != NULL);
    event  = lwes_event_create_no_name ( db );
    assert (event != NULL);

    assert (lwes_listener_recv_by (listener, event, 2000) != 0);

    lwes_event_destroy(event);

    lwes_listener_destroy(listener);
  }

  /* add_headers failures */
  {
    struct lwes_listener *listener;
    struct lwes_emitter *emitter;
    struct lwes_event *event;
    struct lwes_event *event2;
    LWES_BYTE bytes[MAX_MSG_SIZE];
    int n;
    size_t m;

    emitter = lwes_emitter_create ((char *) mcast_ip,
                                   (char *) mcast_iface,
                                   (int) mcast_port,
                                   0,
                                   10);
    assert (emitter != NULL);

    listener = lwes_listener_create ((char *) mcast_ip,
                                     (char *) mcast_iface,
                                     (int) mcast_port);
    assert (listener != NULL);

    event  = lwes_event_create (NULL, eventname);
    assert (event != NULL);

    event2 = lwes_event_create_no_name (NULL);
    assert (event2 != NULL);

    /* recv first, so we are listening */
    assert (lwes_listener_recv_by (listener, event, 10) != 0);
    /* now emit */
    assert (lwes_emitter_emit (emitter, event) == 0);
    /* get the bytes */
    assert ((n = lwes_listener_recv_bytes_by
                   (listener, bytes, MAX_MSG_SIZE, 1000)) > 0);


    /* fail to add headers for various reasons */
    m = n;

    /*  1 2  3  4  5  6  7  8  9 10 11 12 13 14 */
    /* 08 T  y  p  e  C  h  e  c  k  e  r 00 00 */
    /* reason 1: truncated event name */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 5, &m) == -1);

    /* reason 2: truncated num attrs */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 12, &m) == -2);


    /* reason 3: error adding receipt time */

    /* 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 */
    /* 00 R  e  c  e  i  p  t  T  i  m  e  00 00 00 00 00 00 00 00 00 */

    /* first with string marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 14, &m) == -3);
    /* then with type token marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 26, &m) == -3);
    /* finally with type marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 28, &m) == -3);

    /* reason 4: error adding Sender IP */

    /* 36 37 38 39 40 41 42 43 44 45 46 47 48 49 */
    /* 00  S  e  n  d  e  r  I  P 00 00 00 00 00 */

    /* first with string marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 35, &m) == -4);
    /* then with type token marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 44, &m) == -4);
    /* finally with type marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 45, &m) == -4);

    /* reason 5: error adding Sender Port */

    /* 50 51 52 53 54 55 56 57 58 59 60 61 62 63 */
    /* 00  S  e  n  d  e  r  P  o  r  t 00 00 00 */
    /* first with string marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 49, &m) == -5);
    /* then with type token marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 60, &m) == -5);
    /* finally with type marshall */
    assert (lwes_listener_add_header_fields
              (listener, bytes, 61, &m) == -5);

    /* reason 6: error reencoding number of attributes */
    marshall_U_INT_16_fail_at = 2;
    marshall_U_INT_16_count = 0;
    assert (lwes_listener_add_header_fields
              (listener, bytes, 500, &m) == -6);

    marshall_U_INT_16_fail_at = 0;

    /* one final test for coverage get it to fail when process event calls
       add header fields */

    marshall_U_INT_16_fail_at = 2;
    marshall_U_INT_16_count = 0;

    /* emit again */
    assert (lwes_emitter_emit (emitter, event) == 0);
    /* get the bytes */
    assert ((n = lwes_listener_recv_by
                   (listener, event2, 1000)) < 0);
    marshall_U_INT_16_fail_at = 0;

    lwes_event_destroy (event);
    lwes_event_destroy (event2);
    lwes_listener_destroy (listener);
    lwes_emitter_destroy (emitter);
  }
}

static void test_event_name_peek (void)
{
  /* successful event name peek */
  {
    struct lwes_listener *listener;
    struct lwes_emitter *emitter;
    struct lwes_event *event;
    struct lwes_event *event2;
    LWES_BYTE bytes[MAX_MSG_SIZE];
    int n;
    LWES_SHORT_STRING name1 = (LWES_SHORT_STRING)"TypeChecker";
    LWES_SHORT_STRING name2 = (LWES_SHORT_STRING)"Type";
    LWES_SHORT_STRING name3 = (LWES_SHORT_STRING)"CheckerType";

    emitter = lwes_emitter_create ((char *) mcast_ip,
                                   (char *) mcast_iface,
                                   (int) mcast_port,
                                   0,
                                   10);
    assert (emitter != NULL);

    listener = lwes_listener_create ((char *) mcast_ip,
                                     (char *) mcast_iface,
                                     (int) mcast_port);
    assert (listener != NULL);

    event  = lwes_event_create (NULL, name1);
    assert (event != NULL);

    event2 = lwes_event_create_no_name (NULL);
    assert (event2 != NULL);

    /* recv first, so we are listening */
    assert (lwes_listener_recv_by (listener, event, 10) != 0);
    /* now emit */
    assert (lwes_emitter_emit (emitter, event) == 0);
    /* get the bytes */
    assert ((n = lwes_listener_recv_bytes_by
                   (listener, bytes, MAX_MSG_SIZE, 1000)) > 0);

    /* check that the exact name matches */
    assert (lwes_listener_event_has_name (bytes, n, name1) == 0);

    /* but not the subname */
    assert (lwes_listener_event_has_name (bytes, n, name2) != 0);

    /* same lengths will also fail */
    assert (lwes_listener_event_has_name (bytes, n, name3) != 0);

    lwes_event_destroy (event);
    lwes_event_destroy (event2);
    lwes_listener_destroy (listener);
    lwes_emitter_destroy (emitter);
  }

  /* failure at event name peek */
  {
    LWES_BYTE bytes[MAX_MSG_SIZE];

    /* NULL bytes */
    assert (lwes_listener_event_has_name (NULL, 10, eventname) != 0);

    /* zero length */
    assert (lwes_listener_event_has_name (bytes, 0, eventname) != 0);

    /* NULL event name */
    assert (lwes_listener_event_has_name (bytes, 10, NULL) != 0);
  }
}

static void test_emitter_failures (void)
{
  /* open failures */
  {
    /* 1: malloc failure for emitter */
    malloc_count = 0;
    null_at      = 1;
    assert (lwes_emitter_create ((char *) mcast_ip,
                                 (char *) mcast_iface,
                                 (int) mcast_port, 0, 60)
            == NULL);
    /* 2: lwes_net_open failure */
    lwes_net_open_error = 1;
    assert (lwes_emitter_create ((char *) mcast_ip,
                                 (char *) mcast_iface,
                                 (int) mcast_port, 0, 60)
            == NULL);
    lwes_net_open_error = 0;

    /* 3: lwes_net_set_ttl failure */
    lwes_net_set_ttl_error = 1;
    assert (lwes_emitter_create ((char *) mcast_ip,
                                 (char *) mcast_iface,
                                 (int) mcast_port, 0, 60)
            == NULL);
    lwes_net_set_ttl_error = 0;

    /* 4: malloc failure for msg buffer */
    malloc_count = 0;
    null_at      = 2;
    assert (lwes_emitter_create ((char *) mcast_ip,
                                 (char *) mcast_iface,
                                 (int) mcast_port, 0, 60)
            == NULL);
  }

  /* Test emit failures */
  {
    struct lwes_emitter *emitter;
    struct lwes_event *event;

    emitter = lwes_emitter_create ((char *) mcast_ip,
                                   (char *) mcast_iface,
                                   (int) mcast_port, 0, 60);
    assert (emitter != NULL);
    event  = lwes_event_create (NULL, eventname);
    assert ( event != NULL );

    lwes_event_to_bytes_error = 1;
    assert (lwes_emitter_emit (emitter, event) == -1);
    lwes_event_to_bytes_error = 0;

    lwes_net_send_bytes_error = 1;
    assert (lwes_emitter_emit (emitter, event) == -2);
    lwes_net_send_bytes_error = 0;
    lwes_event_destroy (event);
    lwes_emitter_destroy (emitter);
  }

  /* Test emitto failures */
  {
    struct lwes_emitter *emitter;
    struct lwes_event *event;

    emitter = lwes_emitter_create ((char *) mcast_ip,
                                   (char *) mcast_iface,
                                   (int) mcast_port, 0, 60);
    assert (emitter != NULL);
    event  = lwes_event_create (NULL, eventname);
    assert ( event != NULL );

    lwes_event_to_bytes_error = 1;
    assert (lwes_emitter_emitto ((char *) mcast_ip,
                                 (char *) mcast_iface,
                                 (int) mcast_port, emitter, event) == -1);
    lwes_event_to_bytes_error = 0;

    lwes_net_sendto_bytes_error = 1;
    assert (lwes_emitter_emitto ((char *) mcast_ip,
                                 (char *) mcast_iface,
                                 (int) mcast_port, emitter, event) == -2);
    lwes_net_sendto_bytes_error = 0;

    assert (lwes_emitter_emitto ((char *) mcast_ip,
                                 (char *) mcast_iface,
                                 (int) mcast_port, NULL, event) == -1);

    lwes_event_destroy (event);
    lwes_emitter_destroy (emitter);
  }

  /* emitter shouldn't be able to emit an event with no name */
  {
    struct lwes_emitter *emitter;
    struct lwes_event *event;

    emitter = lwes_emitter_create ((char *) mcast_ip,
                                   (char *) mcast_iface,
                                   (int) mcast_port,
                                   0,
                                   10);
    assert (emitter != NULL);

    event  = lwes_event_create_no_name (NULL);
    assert (event != NULL);

    /* recv first, so we are listening */
    assert (lwes_emitter_emit (emitter, event) < 0);
    lwes_event_destroy (event);
    lwes_emitter_destroy (emitter);
  }
}

static void listener (const char *ip, const int port, const char *iface)
{
  struct lwes_listener *listener = NULL;
  struct lwes_event *event;
  struct lwes_event_type_db *db = NULL;

  LWES_SHORT_STRING tmp_event_name;
  LWES_LONG_STRING  value01_o;
  LWES_BOOLEAN      value02_o;
  LWES_IP_ADDR      value03_o;
  LWES_U_INT_16     value04_o;
  LWES_INT_16       value05_o;
  LWES_U_INT_32     value06_o;
  LWES_INT_32       value07_o;
  LWES_U_INT_64     value08_o;
  LWES_INT_64       value09_o;
  int ret;
  int i;

/*  db = lwes_event_type_db_create ( (char*)esffile );
 *  assert ( db != NULL );
 */
  listener = lwes_listener_create ((char *) ip,
                                   (char *) iface,
                                   (int) port);
  assert ( listener != NULL );
  event  = lwes_event_create_no_name ( db );
  assert ( event != NULL );

  assert (lwes_listener_recv (listener, event) > 0);

  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp (tmp_event_name, eventname) == 0);

  assert ( lwes_event_get_STRING   (event, key01, &value01_o) ==  0 );
  assert ( lwes_event_get_BOOLEAN  (event, key02, &value02_o) ==  0 );
  assert ( lwes_event_get_IP_ADDR  (event, key03, &value03_o) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event, key04, &value04_o) ==  0 );
  assert ( lwes_event_get_INT_16   (event, key05, &value05_o) ==  0 );
  assert ( lwes_event_get_U_INT_32 (event, key06, &value06_o) ==  0 );
  assert ( lwes_event_get_INT_32   (event, key07, &value07_o) ==  0 );
  assert ( lwes_event_get_U_INT_64 (event, key08, &value08_o) ==  0 );
  assert ( lwes_event_get_INT_64   (event, key09, &value09_o) ==  0 );

  assert ( ! strcmp(value01, value01_o) ) ;
  assert ( value02 == value02_o );
  assert ( value03.s_addr == value03_o.s_addr );
  assert ( value04 == value04_o );
  assert ( value05 == value05_o );
  assert ( value06 == value06_o );
  assert ( value07 == value07_o );
  assert ( value08 == value08_o );
  assert ( value09 == value09_o );
  lwes_event_destroy(event);

  /* next we should see a startup event */
  event  = lwes_event_create_no_name ( db );
  assert ( event != NULL );
  ret = lwes_listener_recv_by (listener, event, 10000);
  assert ( ret > 0 );

  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp (tmp_event_name, "System::Startup") == 0);

  lwes_event_destroy(event);

  /* then two events as before */
  for (i = 0 ; i < 2 ; i++)
    {
      event  = lwes_event_create_no_name ( db );
      assert ( event != NULL );
      ret = lwes_listener_recv (listener, event);
      assert ( ret > 0 );

      assert (lwes_event_get_name (event, &tmp_event_name) == 0);
      assert (strcmp (tmp_event_name, eventname) == 0);

      assert ( lwes_event_get_STRING   (event, key01, &value01_o) ==  0 );
      assert ( lwes_event_get_BOOLEAN  (event, key02, &value02_o) ==  0 );
      assert ( lwes_event_get_IP_ADDR  (event, key03, &value03_o) ==  0 );
      assert ( lwes_event_get_U_INT_16 (event, key04, &value04_o) ==  0 );
      assert ( lwes_event_get_INT_16   (event, key05, &value05_o) ==  0 );
      assert ( lwes_event_get_U_INT_32 (event, key06, &value06_o) ==  0 );
      assert ( lwes_event_get_INT_32   (event, key07, &value07_o) ==  0 );
      assert ( lwes_event_get_U_INT_64 (event, key08, &value08_o) ==  0 );
      assert ( lwes_event_get_INT_64   (event, key09, &value09_o) ==  0 );

      assert ( ! strcmp(value01, value01_o) ) ;
      assert ( value02 == value02_o );
      assert ( value03.s_addr == value03_o.s_addr );
      assert ( value04 == value04_o );
      assert ( value05 == value05_o );
      assert ( value06 == value06_o );
      assert ( value07 == value07_o );
      assert ( value08 == value08_o );
      assert ( value09 == value09_o );
      lwes_event_destroy(event);
    }

  /* followed by a heartbeat */
  event  = lwes_event_create_no_name ( db );
  assert ( event != NULL );
  ret = lwes_listener_recv (listener, event);
  assert ( ret > 0 );

  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp (tmp_event_name, "System::Heartbeat") == 0);
  /* check that the counts are correct */
  assert (lwes_event_get_INT_64 (event, "seq", &value09_o) == 0);
  assert (value09_o == 1);
  assert (lwes_event_get_INT_64 (event, "count", &value09_o) == 0);
  assert (value09_o == 2);
  assert (lwes_event_get_INT_64 (event, "total", &value09_o) == 0);
  assert (value09_o == 2);
  assert (lwes_event_get_INT_16 (event, "freq", &value05_o) == 0);
  assert (value05_o == 10);

  lwes_event_destroy(event);

  /* and finally a shutdown */
  event  = lwes_event_create_no_name ( db );
  assert ( event != NULL );
  ret = lwes_listener_recv (listener, event);
  assert ( ret > 0 );

  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp (tmp_event_name, "System::Shutdown") == 0);
  assert (lwes_event_get_INT_64 (event, "seq", &value09_o) == 0);
  assert (value09_o == 1);
  assert (lwes_event_get_INT_64 (event, "count", &value09_o) == 0);
  assert (value09_o == 0);
  assert (lwes_event_get_INT_64 (event, "total", &value09_o) == 0);
  assert (value09_o == 2);
  assert (lwes_event_get_INT_16 (event, "freq", &value05_o) == 0);
  assert (value05_o == 32767);

  lwes_event_destroy(event);

  lwes_listener_destroy(listener);
/*  lwes_event_type_db_destroy(db); */
}

static void listener_to (const char *ip,
                         const int   port,
                         const char *iface,
                         const char *n_ip,
                         const int   n_port,
                         const char *n_iface)
{
  struct lwes_listener *listener = NULL;
  struct lwes_listener *o_listener = NULL;
  struct lwes_event *event;
  struct lwes_event_type_db *db = NULL;

  LWES_SHORT_STRING tmp_event_name;
  LWES_LONG_STRING  value01_o;
  LWES_BOOLEAN      value02_o;
  LWES_IP_ADDR      value03_o;
  LWES_U_INT_16     value04_o;
  LWES_INT_16       value05_o;
  LWES_U_INT_32     value06_o;
  LWES_INT_32       value07_o;
  LWES_U_INT_64     value08_o;
  LWES_INT_64       value09_o;
  int i;

  assert ((listener = lwes_listener_create ((char *) n_ip,
                                   (char *) n_iface,
                                   (int)    n_port)) != NULL);
  assert ((o_listener = lwes_listener_create ((char *) ip,
                                     (char *) iface,
                                     (int)    port)) != NULL);

  /* first we should see a Startup event on the old channel */
  assert ((event  = lwes_event_create_no_name (db)) != NULL);
  assert (lwes_listener_recv (o_listener, event) > 0);
  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp (tmp_event_name, "System::Startup") == 0);
  lwes_event_destroy(event);

  /* then we should see events on the new channel */
  assert ((event  = lwes_event_create_no_name (db)) != NULL);
  assert (lwes_listener_recv (listener, event) > 0);

  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp (tmp_event_name, eventname) == 0);

  assert (lwes_event_get_STRING   (event, key01, &value01_o) ==  0);
  assert (lwes_event_get_BOOLEAN  (event, key02, &value02_o) ==  0);
  assert (lwes_event_get_IP_ADDR  (event, key03, &value03_o) ==  0);
  assert (lwes_event_get_U_INT_16 (event, key04, &value04_o) ==  0);
  assert (lwes_event_get_INT_16   (event, key05, &value05_o) ==  0);
  assert (lwes_event_get_U_INT_32 (event, key06, &value06_o) ==  0);
  assert (lwes_event_get_INT_32   (event, key07, &value07_o) ==  0);
  assert (lwes_event_get_U_INT_64 (event, key08, &value08_o) ==  0);
  assert (lwes_event_get_INT_64   (event, key09, &value09_o) ==  0);

  assert (! strcmp(value01, value01_o)) ;
  assert (value02 == value02_o);
  assert (value03.s_addr == value03_o.s_addr);
  assert (value04 == value04_o);
  assert (value05 == value05_o);
  assert (value06 == value06_o);
  assert (value07 == value07_o);
  assert (value08 == value08_o);
  assert (value09 == value09_o);
  lwes_event_destroy (event);

  /* then two events as before */
  for (i = 0 ; i < 2 ; i++)
    {
      assert ((event  = lwes_event_create_no_name (db)) != NULL);
      assert (lwes_listener_recv (listener, event) > 0);

      assert (lwes_event_get_name (event, &tmp_event_name) == 0);
      assert (strcmp (tmp_event_name, eventname) == 0);

      assert ( lwes_event_get_STRING   (event, key01, &value01_o) ==  0 );
      assert ( lwes_event_get_BOOLEAN  (event, key02, &value02_o) ==  0 );
      assert ( lwes_event_get_IP_ADDR  (event, key03, &value03_o) ==  0 );
      assert ( lwes_event_get_U_INT_16 (event, key04, &value04_o) ==  0 );
      assert ( lwes_event_get_INT_16   (event, key05, &value05_o) ==  0 );
      assert ( lwes_event_get_U_INT_32 (event, key06, &value06_o) ==  0 );
      assert ( lwes_event_get_INT_32   (event, key07, &value07_o) ==  0 );
      assert ( lwes_event_get_U_INT_64 (event, key08, &value08_o) ==  0 );
      assert ( lwes_event_get_INT_64   (event, key09, &value09_o) ==  0 );

      assert ( ! strcmp(value01, value01_o) ) ;
      assert ( value02 == value02_o );
      assert ( value03.s_addr == value03_o.s_addr );
      assert ( value04 == value04_o );
      assert ( value05 == value05_o );
      assert ( value06 == value06_o );
      assert ( value07 == value07_o );
      assert ( value08 == value08_o );
      assert ( value09 == value09_o );
      lwes_event_destroy(event);
    }

  /* and finally a shutdown on the old channel */
  assert ((event  = lwes_event_create_no_name (db)) != NULL);
  assert (lwes_listener_recv (o_listener, event) > 0);
  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp (tmp_event_name, "System::Shutdown") == 0);
  assert (lwes_event_get_INT_64 (event, "seq", &value09_o) == 0);
  assert (value09_o == 0);
  assert (lwes_event_get_INT_64 (event, "count", &value09_o) == 0);
  assert (value09_o == 0);
  assert (lwes_event_get_INT_64 (event, "total", &value09_o) == 0);
  assert (value09_o == 0);
  /* we test the weird time running backward case here */
  assert (lwes_event_get_INT_16 (event, "freq", &value05_o) == 0);
  assert (value05_o == 0);
  lwes_event_destroy (event);
  lwes_listener_destroy (listener);
  lwes_listener_destroy (o_listener);
}

static void emitter    (const char *ip, const int port, const char *iface)
{
  struct lwes_emitter *emitter;
  struct lwes_event *event;
  struct lwes_event_type_db *db = NULL;

/*  db = lwes_event_type_db_create ( (char*)esffile );
 * assert ( db != NULL );
 */

  /* test null safety */
  assert ( lwes_emitter_emit(NULL, NULL) == -1 );

  /* emit without heartbeat */
  emitter = lwes_emitter_create (ip, iface, port, 0, 60);
  assert ( emitter != NULL );
  assert (lwes_net_get_ttl (&(emitter->connection)) == 3);
  event  = lwes_event_create (db, eventname);
  assert ( event != NULL );

  assert ( lwes_event_set_STRING   (event, key01, value01) == 1 );
  assert ( lwes_event_set_BOOLEAN  (event, key02, value02) == 2 );
  assert ( lwes_event_set_IP_ADDR  (event, key03, value03) == 3 );
  assert ( lwes_event_set_U_INT_16 (event, key04, value04) == 4 );
  assert ( lwes_event_set_INT_16   (event, key05, value05) == 5 );
  assert ( lwes_event_set_U_INT_32 (event, key06, value06) == 6 );
  assert ( lwes_event_set_INT_32   (event, key07, value07) == 7 );
  assert ( lwes_event_set_U_INT_64 (event, key08, value08) == 8 );
  assert ( lwes_event_set_INT_64   (event, key09, value09) == 9 );

  assert ( lwes_emitter_emit(emitter,event) == 0 );

  lwes_event_destroy(event);
  lwes_emitter_destroy(emitter);
/*  lwes_event_type_db_destroy(db); */

  /* emit with heartbeat */
  emitter = lwes_emitter_create (ip, iface, port, 1, 2);
  assert ( emitter != NULL );
  event  = lwes_event_create (db, eventname);
  assert ( event != NULL );

  assert ( lwes_event_set_STRING   (event, key01, value01) == 1 );
  assert ( lwes_event_set_BOOLEAN  (event, key02, value02) == 2 );
  assert ( lwes_event_set_IP_ADDR  (event, key03, value03) == 3 );
  assert ( lwes_event_set_U_INT_16 (event, key04, value04) == 4 );
  assert ( lwes_event_set_INT_16   (event, key05, value05) == 5 );
  assert ( lwes_event_set_U_INT_32 (event, key06, value06) == 6 );
  assert ( lwes_event_set_INT_32   (event, key07, value07) == 7 );
  assert ( lwes_event_set_U_INT_64 (event, key08, value08) == 8 );
  assert ( lwes_event_set_INT_64   (event, key09, value09) == 9 );

  /* emit one */
  assert ( lwes_emitter_emit(emitter,event) == 0 );

  /* then pretend it's the future so we get a heartbeat */
  time_future = 10;
  assert ( lwes_emitter_emit(emitter,event) == 0 );
  time_future = 0;

  lwes_event_destroy(event);

  /* pretend its really really far in the future to see that freq is
     capped */
  time_future = 100000;
  lwes_emitter_destroy(emitter);
  time_future = 0;
}

static void emitter_to  (const char *ip,
                         const int port,
                         const char *iface,
                         const char *n_ip,
                         const int n_port,
                         const char *n_iface)
{
  struct lwes_emitter *emitter;
  struct lwes_event *event;
  struct lwes_event_type_db *db = NULL;

/* db = lwes_event_type_db_create ( (char*)esffile );
 * assert ( db != NULL );
 */

  /* emit without heartbeat */
  emitter = lwes_emitter_create (ip, iface, port, 0, 60);
  assert (emitter != NULL);
  assert (lwes_net_get_ttl (&(emitter->connection)) == 3);
  event  = lwes_event_create (db, eventname);
  assert (event != NULL);

  assert (lwes_event_set_STRING   (event, key01, value01) == 1);
  assert (lwes_event_set_BOOLEAN  (event, key02, value02) == 2 );
  assert (lwes_event_set_IP_ADDR  (event, key03, value03) == 3);
  assert (lwes_event_set_U_INT_16 (event, key04, value04) == 4);
  assert (lwes_event_set_INT_16   (event, key05, value05) == 5);
  assert (lwes_event_set_U_INT_32 (event, key06, value06) == 6);
  assert (lwes_event_set_INT_32   (event, key07, value07) == 7);
  assert (lwes_event_set_U_INT_64 (event, key08, value08) == 8);
  assert (lwes_event_set_INT_64   (event, key09, value09) == 9);

  assert (lwes_emitter_emitto ((char *) n_ip,
                               (char *) n_iface,
                               (int)    n_port,
                               emitter,
                               event) == 0);

  lwes_event_destroy(event);
  lwes_emitter_destroy(emitter);
/*  lwes_event_type_db_destroy(db); */

  /* emit with heartbeat */
  emitter = lwes_emitter_create (ip, iface, port, 1, 2);
  assert (emitter != NULL);
  event  = lwes_event_create (db, eventname);
  assert (event != NULL);

  assert (lwes_event_set_STRING   (event, key01, value01) == 1);
  assert (lwes_event_set_BOOLEAN  (event, key02, value02) == 2);
  assert (lwes_event_set_IP_ADDR  (event, key03, value03) == 3);
  assert (lwes_event_set_U_INT_16 (event, key04, value04) == 4);
  assert (lwes_event_set_INT_16   (event, key05, value05) == 5);
  assert (lwes_event_set_U_INT_32 (event, key06, value06) == 6);
  assert (lwes_event_set_INT_32   (event, key07, value07) == 7);
  assert (lwes_event_set_U_INT_64 (event, key08, value08) == 8);
  assert (lwes_event_set_INT_64   (event, key09, value09) == 9);

  assert (lwes_emitter_emitto ((char *) n_ip,
                               (char *) n_iface,
                               (int)    n_port,
                               emitter,
                               event) == 0);

  /* pretend its the future */
  time_future = 10;

  assert (lwes_emitter_emitto ((char *) n_ip,
                               (char *) n_iface,
                               (int)    n_port,
                               emitter,
                               event) == 0);

  lwes_event_destroy (event);

  /* pretend its the past to test the other edge case for freqency */
  time_past = 2000;
  lwes_emitter_destroy (emitter);
  time_past = 0;
}

int main (void)
{
  /* set this one here since we need to call a function */
  value03.s_addr = inet_addr("224.0.0.100");

  test_event_name_peek ();
  test_listener_failures ();
  test_emitter_failures ();

  test_emit ();
  test_emitto ();

  return 0;
}

