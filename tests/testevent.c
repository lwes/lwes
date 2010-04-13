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
#include <stdlib.h>

#include "lwes_event.h"
#include "lwes_hash.h"
#include "lwes_marshall_functions.h"

static size_t null_at = 0;
static size_t malloc_count = 0;

void *my_malloc (size_t size)
{
  void *ret = NULL;
  malloc_count++;
  if ( malloc_count != null_at )
    {
      ret = malloc (size);
    }
  return ret;
}
#define malloc my_malloc

static int hash_null = 0;

struct lwes_hash *my_lwes_hash_create (void)
{
  struct lwes_hash * my_hash = NULL;
  if ( ! hash_null )
    {
      my_hash = lwes_hash_create ();
    }
  return my_hash;
}
#define lwes_hash_create my_lwes_hash_create

static int put_null = 0;
int my_lwes_hash_put(struct lwes_hash* hash, char *key, void *value)
{
  int ret = -3;
  if ( ! put_null )
    {
      ret = lwes_hash_put (hash, key, value);
    }
  return ret;
}
#define lwes_hash_put my_lwes_hash_put

static struct lwes_event_attribute null_type;
static LWES_U_INT_16 enc = (LWES_U_INT_16)1;

static int lwes_hash_get_type_error = 0;
static int lwes_hash_get_value_error = 0;
void *
my_lwes_hash_get
  (struct lwes_hash* hash,
   const char *key)
{
  if (lwes_hash_get_type_error == 1)
    {
      null_type.type = LWES_TYPE_U_INT_16;
      null_type.value = (void*)&enc;
      return (void *)&null_type;
    }
  else if (lwes_hash_get_value_error == 1)
    {
      null_type.type = LWES_TYPE_INT_16;
      null_type.value = NULL;
      return (void *)&null_type;
    }
  else
    {
      return lwes_hash_get (hash, key);
    }
}
#define lwes_hash_get my_lwes_hash_get

#include "lwes_event.c"

#undef malloc
#undef lwes_hash_create
#undef lwes_hash_put
#undef lwes_hash_get

static LWES_BYTE ref_bytes_no_db[216] = {
  0x0b,0x54,0x79,0x70,0x65,0x43,0x68,0x65,0x63,0x6b,0x65,0x72,0x00,0x0d,0x0b,
  0x61,0x6e,0x49,0x50,0x41,0x64,0x64,0x72,0x65,0x73,0x73,0x06,0x64,0x00,0x00,
  0xe0,0x07,0x61,0x55,0x49,0x6e,0x74,0x33,0x32,0x03,0xff,0xff,0xff,0xff,0x08,
  0x61,0x42,0x6f,0x6f,0x6c,0x65,0x61,0x6e,0x09,0x01,0x08,0x61,0x42,0x6f,0x6f,
  0x6c,0x65,0x61,0x6e,0x09,0x01,0x07,0x61,0x6e,0x49,0x6e,0x74,0x31,0x36,0x02,
  0xff,0xff,0x07,0x61,0x53,0x74,0x72,0x69,0x6e,0x67,0x05,0x00,0x13,0x68,0x74,
  0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x74,0x65,0x73,0x74,0x2e,0x63,
  0x6f,0x6d,0x07,0x61,0x6e,0x49,0x6e,0x74,0x36,0x34,0x07,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0x07,0x61,0x6e,0x49,0x6e,0x74,0x33,0x32,0x04,0xff,0xff,
  0xff,0xff,0x07,0x61,0x55,0x49,0x6e,0x74,0x31,0x36,0x01,0xff,0xff,0x0d,0x61,
  0x6e,0x6f,0x74,0x68,0x65,0x72,0x55,0x49,0x6e,0x74,0x31,0x36,0x01,0xff,0xff,
  0x08,0x53,0x65,0x6e,0x64,0x65,0x72,0x49,0x50,0x06,0x01,0x00,0x00,0x7f,0x07,
  0x61,0x55,0x49,0x6e,0x74,0x36,0x34,0x08,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0x0b,0x61,0x4d,0x65,0x74,0x61,0x53,0x74,0x72,0x69,0x6e,0x67,0x05,0x00,
  0x05,0x68,0x65,0x6c,0x6c,0x6f};

static LWES_BYTE ref_bytes_db[188] = {
  0x0b,0x54,0x79,0x70,0x65,0x43,0x68,0x65,0x63,0x6b,0x65,0x72,0x00,0x0b,0x0b,
  0x61,0x6e,0x49,0x50,0x41,0x64,0x64,0x72,0x65,0x73,0x73,0x06,0x64,0x00,0x00,
  0xe0,0x07,0x61,0x55,0x49,0x6e,0x74,0x33,0x32,0x03,0xff,0xff,0xff,0xff,0x08,
  0x61,0x42,0x6f,0x6f,0x6c,0x65,0x61,0x6e,0x09,0x01,0x07,0x61,0x6e,0x49,0x6e,
  0x74,0x31,0x36,0x02,0xff,0xff,0x07,0x61,0x53,0x74,0x72,0x69,0x6e,0x67,0x05,
  0x00,0x13,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x74,0x65,
  0x73,0x74,0x2e,0x63,0x6f,0x6d,0x07,0x61,0x6e,0x49,0x6e,0x74,0x36,0x34,0x07,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x07,0x61,0x6e,0x49,0x6e,0x74,0x33,
  0x32,0x04,0xff,0xff,0xff,0xff,0x07,0x61,0x55,0x49,0x6e,0x74,0x31,0x36,0x01,
  0xff,0xff,0x08,0x53,0x65,0x6e,0x64,0x65,0x72,0x49,0x50,0x06,0x01,0x00,0x00,
  0x7f,0x07,0x61,0x55,0x49,0x6e,0x74,0x36,0x34,0x08,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0x0b,0x61,0x4d,0x65,0x74,0x61,0x53,0x74,0x72,0x69,0x6e,0x67,
  0x05,0x00,0x05,0x68,0x65,0x6c,0x6c,0x6f};

const char *esffile         = "testeventtypedb.esf";
LWES_SHORT_STRING eventname = (LWES_SHORT_STRING)"TypeChecker";
LWES_SHORT_STRING key01     = (LWES_SHORT_STRING)"aString";
LWES_LONG_STRING  value01   = (LWES_LONG_STRING)"http://www.test.com";
LWES_SHORT_STRING key02     = (LWES_SHORT_STRING)"aBoolean";
LWES_BOOLEAN      value02   = (LWES_BOOLEAN)1;
LWES_LONG_STRING  value02s  = (LWES_SHORT_STRING)"1";
LWES_SHORT_STRING key03     = (LWES_SHORT_STRING)"anIPAddress";
LWES_SHORT_STRING value03s  = (LWES_SHORT_STRING)"224.0.0.100";
LWES_SHORT_STRING key04     = (LWES_SHORT_STRING)"aUInt16";
LWES_U_INT_16     value04   = (LWES_U_INT_16)65535;
LWES_SHORT_STRING key05     = (LWES_SHORT_STRING)"anotherUInt16";
LWES_U_INT_16     value05   = (LWES_U_INT_16)65535;
LWES_SHORT_STRING key06     = (LWES_SHORT_STRING)"anInt16";
LWES_INT_16       value06   = (LWES_INT_16)-1;
LWES_SHORT_STRING key07     = (LWES_SHORT_STRING)"aUInt32";
LWES_U_INT_32     value07   = (LWES_U_INT_32)0xffffffffL;
LWES_SHORT_STRING key08     = (LWES_SHORT_STRING)"anInt32";
LWES_INT_32       value08   = (LWES_INT_32)-1;
LWES_SHORT_STRING key09     = (LWES_SHORT_STRING)"aUInt64";
LWES_U_INT_64     value09   = (LWES_U_INT_64)0xffffffffffffffffULL;
LWES_SHORT_STRING key10     = (LWES_SHORT_STRING)"anInt64";
LWES_INT_64       value10   = (LWES_INT_64)-1;
LWES_SHORT_STRING key11     = (LWES_SHORT_STRING)"aMetaString";
LWES_LONG_STRING  value11   = (LWES_LONG_STRING)"hello";
LWES_SHORT_STRING key12     = (LWES_SHORT_STRING)"SenderIP";
LWES_IP_ADDR      value12;

static void
test_event_with_db (void)
{
  struct lwes_event *event1;
  struct lwes_event *event2;
  struct lwes_event_type_db *db = NULL;
  struct lwes_event_deserialize_tmp dtmp;
  LWES_BYTE bytes[LWES_MSG_SIZE_MAX];
  int size1;
  int size2;
  int i;

  LWES_SHORT_STRING evnt_nm_o1;
  LWES_U_INT_16     nm_attr_o1;
  LWES_LONG_STRING  value01_o1;
  LWES_BOOLEAN      value02_o1;
  LWES_LONG_STRING  value02s_o1;
  LWES_IP_ADDR      value03_o1;
  LWES_U_INT_16     value04_o1;
  LWES_U_INT_16     value05_o1;
  LWES_INT_16       value06_o1;
  LWES_U_INT_32     value07_o1;
  LWES_INT_32       value08_o1;
  LWES_U_INT_64     value09_o1;
  LWES_INT_64       value10_o1;
  LWES_LONG_STRING  value11_o1;
  LWES_IP_ADDR      value12_o1;

  LWES_SHORT_STRING evnt_nm_o2;
  LWES_U_INT_16     nm_attr_o2;
  LWES_LONG_STRING  value01_o2;
  LWES_BOOLEAN      value02_o2;
  LWES_LONG_STRING  value02s_o2;
  LWES_IP_ADDR      value03_o2;
  LWES_U_INT_16     value04_o2;
  LWES_U_INT_16     value05_o2;
  LWES_INT_16       value06_o2;
  LWES_U_INT_32     value07_o2;
  LWES_INT_32       value08_o2;
  LWES_U_INT_64     value09_o2;
  LWES_INT_64       value10_o2;
  LWES_LONG_STRING  value11_o2;
  LWES_IP_ADDR      value12_o2;

  db = lwes_event_type_db_create ((char*)esffile);
  assert ( db != NULL );

  event1 = lwes_event_create (db, eventname);
  assert ( event1 != NULL );

  assert ( lwes_event_get_name ( event1, &evnt_nm_o1) == 0 );
  assert ( strcmp (evnt_nm_o1, eventname) == 0 );
  assert ( lwes_event_set_STRING           (event1, key01, value01) ==  1 );
  assert ( lwes_event_set_BOOLEAN          (event1, key02, value02) ==  2 );
  assert ( lwes_event_set_STRING           (event1, key02, value02s)== -2 );
  assert ( lwes_event_set_IP_ADDR_w_string (event1, key03, value03s)==  3 );
  assert ( lwes_event_set_U_INT_16         (event1, key04, value04) ==  4 );
  assert ( lwes_event_set_U_INT_16         (event1, key05, value05) == -1 );
  assert ( lwes_event_set_INT_16           (event1, key06, value06) ==  5 );
  assert ( lwes_event_set_U_INT_32         (event1, key07, value07) ==  6 );
  assert ( lwes_event_set_INT_32           (event1, key08, value08) ==  7 );
  assert ( lwes_event_set_U_INT_64         (event1, key09, value09) ==  8 );
  assert ( lwes_event_set_INT_64           (event1, key10, value10) ==  9 );
  assert ( lwes_event_set_STRING           (event1, key11, value11) == 10 );
  assert ( lwes_event_set_IP_ADDR          (event1, key12, value12) == 11 );
  assert ( lwes_event_get_number_of_attributes ( event1, &nm_attr_o1) == 0 );
  assert ( nm_attr_o1 == 11 );

  size1 = lwes_event_to_bytes (event1,bytes,LWES_MSG_SIZE_MAX, 0);

  /* this test may fail depending on whether the hashing function or the
   * default ordering changes
   */
  for ( i = 0 ; i < size1 ; i++ )
    {
      assert ( bytes[i] == ref_bytes_db[i] );
    }

  event2 = lwes_event_create_no_name (db);
  assert ( event2 != NULL );

  size2  = lwes_event_from_bytes (event2, bytes, size1, 0, &dtmp);

  assert ( size1 == size2 );

  assert ( lwes_event_get_name ( event2, &evnt_nm_o2) == 0 );
  assert ( strcmp (evnt_nm_o2, eventname) == 0 );
  assert ( strcmp (evnt_nm_o1, evnt_nm_o2) == 0 );
  assert ( lwes_event_get_number_of_attributes ( event2, &nm_attr_o2) == 0 );
  assert ( nm_attr_o2 == 11 );
  assert ( nm_attr_o2 == nm_attr_o1 );

  assert ( lwes_event_get_STRING   (event1, key01, &value01_o1) ==  0 );
  assert ( lwes_event_get_BOOLEAN  (event1, key02, &value02_o1) ==  0 );
  assert ( lwes_event_get_STRING   (event1, key02, &value02s_o1)==  0 );
  assert ( lwes_event_get_IP_ADDR  (event1, key03, &value03_o1) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event1, key04, &value04_o1) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event1, key05, &value05_o1) == -1 );
  assert ( lwes_event_get_INT_16   (event1, key06, &value06_o1) ==  0 );
  assert ( lwes_event_get_U_INT_32 (event1, key07, &value07_o1) ==  0 );
  assert ( lwes_event_get_INT_32   (event1, key08, &value08_o1) ==  0 );
  assert ( lwes_event_get_U_INT_64 (event1, key09, &value09_o1) ==  0 );
  assert ( lwes_event_get_INT_64   (event1, key10, &value10_o1) ==  0 );
  assert ( lwes_event_get_STRING   (event1, key11, &value11_o1) ==  0 );
  assert ( lwes_event_get_IP_ADDR  (event1, key12, &value12_o1) ==  0 );

  assert ( lwes_event_get_STRING   (event2, key01, &value01_o2) ==  0 );
  assert ( lwes_event_get_BOOLEAN  (event2, key02, &value02_o2) ==  0 );
  assert ( lwes_event_get_STRING   (event2, key02, &value02s_o2)==  0 );
  assert ( lwes_event_get_IP_ADDR  (event2, key03, &value03_o2) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event2, key04, &value04_o2) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event2, key05, &value05_o2) == -1 );
  assert ( lwes_event_get_INT_16   (event2, key06, &value06_o2) ==  0 );
  assert ( lwes_event_get_U_INT_32 (event2, key07, &value07_o2) ==  0 );
  assert ( lwes_event_get_INT_32   (event2, key08, &value08_o2) ==  0 );
  assert ( lwes_event_get_U_INT_64 (event2, key09, &value09_o2) ==  0 );
  assert ( lwes_event_get_INT_64   (event2, key10, &value10_o2) ==  0 );
  assert ( lwes_event_get_STRING   (event2, key11, &value11_o2) ==  0 );
  assert ( lwes_event_get_IP_ADDR  (event2, key12, &value12_o2) ==  0 );

  assert ( strcmp(value01_o1, value01_o2) == 0 ) ;
  assert ( value02_o1 == value02_o2 );
  assert ( strcmp(value02s_o1, value02s_o2) == 0 ) ;
  assert ( value03_o1.s_addr == value03_o2.s_addr );
  assert ( value04_o1 == value04_o2 );
  assert ( value06_o1 == value06_o2 );
  assert ( value07_o1 == value07_o2 );
  assert ( value08_o1 == value08_o2 );
  assert ( value09_o1 == value09_o2 );
  assert ( value10_o1 == value10_o2 );
  assert ( strcmp ( value11_o1, value11_o2 ) == 0 );
  assert ( value12_o1.s_addr == value12_o2.s_addr );

  lwes_event_destroy (event1);
  lwes_event_destroy (event2);
  lwes_event_type_db_destroy (db);
}

static void
test_event_without_db (void)
{
  struct lwes_event *event1;
  struct lwes_event *event2;
  struct lwes_event_deserialize_tmp dtmp;
  LWES_BYTE bytes[LWES_MSG_SIZE_MAX];
  int size1;
  int size2;
  int i;

  LWES_SHORT_STRING evnt_nm_o1;
  LWES_U_INT_16     nm_attr_o1;
  LWES_LONG_STRING  value01_o1;
  LWES_BOOLEAN      value02_o1;
  LWES_LONG_STRING  value02s_o1;
  LWES_IP_ADDR      value03_o1;
  LWES_U_INT_16     value04_o1;
  LWES_U_INT_16     value05_o1;
  LWES_INT_16       value06_o1;
  LWES_U_INT_32     value07_o1;
  LWES_INT_32       value08_o1;
  LWES_U_INT_64     value09_o1;
  LWES_INT_64       value10_o1;
  LWES_LONG_STRING  value11_o1;
  LWES_IP_ADDR      value12_o1;

  LWES_SHORT_STRING evnt_nm_o2;
  LWES_U_INT_16     nm_attr_o2;
  LWES_LONG_STRING  value01_o2;
  LWES_BOOLEAN      value02_o2;
  LWES_LONG_STRING  value02s_o2;
  LWES_IP_ADDR      value03_o2;
  LWES_U_INT_16     value04_o2;
  LWES_U_INT_16     value05_o2;
  LWES_INT_16       value06_o2;
  LWES_U_INT_32     value07_o2;
  LWES_INT_32       value08_o2;
  LWES_U_INT_64     value09_o2;
  LWES_INT_64       value10_o2;
  LWES_LONG_STRING  value11_o2;
  LWES_IP_ADDR      value12_o2;

  event1 = lwes_event_create (NULL, eventname);
  assert ( event1 != NULL );

  assert ( lwes_event_get_name ( event1, &evnt_nm_o1) == 0 );
  assert ( strcmp (evnt_nm_o1, eventname) == 0 );
  assert ( lwes_event_set_STRING           (event1, key01, value01) ==  1 );
  assert ( lwes_event_set_BOOLEAN          (event1, key02, value02) ==  2 );
  assert ( lwes_event_set_STRING           (event1, key02, value02s)==  3 );
  assert ( lwes_event_set_IP_ADDR_w_string (event1, key03, value03s)==  4 );
  assert ( lwes_event_set_U_INT_16         (event1, key04, value04) ==  5 );
  assert ( lwes_event_set_U_INT_16         (event1, key05, value05) ==  6 );
  assert ( lwes_event_set_INT_16           (event1, key06, value06) ==  7 );
  assert ( lwes_event_set_U_INT_32         (event1, key07, value07) ==  8 );
  assert ( lwes_event_set_INT_32           (event1, key08, value08) ==  9 );
  assert ( lwes_event_set_U_INT_64         (event1, key09, value09) == 10 );
  assert ( lwes_event_set_INT_64           (event1, key10, value10) == 11 );
  assert ( lwes_event_set_STRING           (event1, key11, value11) == 12 );
  assert ( lwes_event_set_IP_ADDR          (event1, key12, value12) == 13 );
  assert ( lwes_event_get_number_of_attributes ( event1, &nm_attr_o1) == 0 );
  assert ( nm_attr_o1 == 13 );

  size1 = lwes_event_to_bytes (event1,bytes,LWES_MSG_SIZE_MAX,0);

  /* this test may fail depending on whether the hashing function or the
   * default ordering changes
   */
  for ( i = 0 ; i < size1 ; i++ )
    {
      assert ( bytes[i] == ref_bytes_no_db[i] );
    }

  event2 = lwes_event_create_no_name (NULL);
  assert ( event2 != NULL );

  size2  = lwes_event_from_bytes (event2,bytes,size1,0,&dtmp);

  assert ( size1 == size2 );

  assert ( lwes_event_get_name ( event2, &evnt_nm_o2) == 0 );
  assert ( strcmp (evnt_nm_o2, eventname) == 0 );
  assert ( strcmp (evnt_nm_o1, evnt_nm_o2) == 0 );
  assert ( lwes_event_get_number_of_attributes ( event2, &nm_attr_o2) == 0 );
  assert ( nm_attr_o2 == 13 );
  assert ( nm_attr_o2 == nm_attr_o1 );

  assert ( lwes_event_get_STRING   (event1, key01, &value01_o1) ==  0 );
  assert ( lwes_event_get_BOOLEAN  (event1, key02, &value02_o1) ==  0 );
  assert ( lwes_event_get_STRING   (event1, key02, &value02s_o1)==  0 );
  assert ( lwes_event_get_IP_ADDR  (event1, key03, &value03_o1) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event1, key04, &value04_o1) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event1, key05, &value05_o1) ==  0 );
  assert ( lwes_event_get_INT_16   (event1, key06, &value06_o1) ==  0 );
  assert ( lwes_event_get_U_INT_32 (event1, key07, &value07_o1) ==  0 );
  assert ( lwes_event_get_INT_32   (event1, key08, &value08_o1) ==  0 );
  assert ( lwes_event_get_U_INT_64 (event1, key09, &value09_o1) ==  0 );
  assert ( lwes_event_get_INT_64   (event1, key10, &value10_o1) ==  0 );
  assert ( lwes_event_get_STRING   (event1, key11, &value11_o1) ==  0 );
  assert ( lwes_event_get_IP_ADDR  (event1, key12, &value12_o1) ==  0 );

  assert ( lwes_event_get_STRING   (event2, key01, &value01_o2) ==  0 );
  assert ( lwes_event_get_BOOLEAN  (event2, key02, &value02_o2) ==  0 );
  assert ( lwes_event_get_STRING   (event2, key02, &value02s_o2)==  0 );
  assert ( lwes_event_get_IP_ADDR  (event2, key03, &value03_o2) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event2, key04, &value04_o2) ==  0 );
  assert ( lwes_event_get_U_INT_16 (event2, key05, &value05_o2) ==  0 );
  assert ( lwes_event_get_INT_16   (event2, key06, &value06_o2) ==  0 );
  assert ( lwes_event_get_U_INT_32 (event2, key07, &value07_o2) ==  0 );
  assert ( lwes_event_get_INT_32   (event2, key08, &value08_o2) ==  0 );
  assert ( lwes_event_get_U_INT_64 (event2, key09, &value09_o2) ==  0 );
  assert ( lwes_event_get_INT_64   (event2, key10, &value10_o2) ==  0 );
  assert ( lwes_event_get_STRING   (event2, key11, &value11_o2) ==  0 );
  assert ( lwes_event_get_IP_ADDR  (event2, key12, &value12_o2) ==  0 );

  assert ( strcmp(value01_o1, value01_o2) == 0 ) ;
  assert ( value02_o1 == value02_o2 );
  assert ( strcmp(value02s_o1, value02s_o2) == 0 ) ;
  assert ( value03_o1.s_addr == value03_o2.s_addr );
  assert ( value04_o1 == value04_o2 );
  assert ( value05_o1 == value05_o2 );
  assert ( value06_o1 == value06_o2 );
  assert ( value07_o1 == value07_o2 );
  assert ( value08_o1 == value08_o2 );
  assert ( value09_o1 == value09_o2 );
  assert ( value10_o1 == value10_o2 );
  assert ( strcmp ( value11_o1, value11_o2 ) == 0 );
  assert ( value12_o1.s_addr == value12_o2.s_addr );

  lwes_event_destroy (event1);
  lwes_event_destroy (event2);
}

static void
test_error_and_boundary_conditions (void)
{
  struct lwes_event *event = NULL;
  LWES_U_INT_16     num_attrs;
  int i;

  LWES_U_INT_16     value_uint16;
  LWES_INT_16       value_int16;
  LWES_U_INT_32     value_uint32;
  LWES_INT_32       value_int32;
  LWES_U_INT_64     value_uint64;
  LWES_INT_64       value_int64;
  LWES_LONG_STRING  value_string;
  LWES_IP_ADDR      value_ipaddr;
  LWES_BOOLEAN      value_bool;


  /* cause malloc to fail for event creates */
  malloc_count = 0;
  null_at      = 1;
  assert (lwes_event_create (NULL, "Foo") == NULL);

  malloc_count = 0;
  null_at      = 1;
  assert (lwes_event_create_no_name (NULL) == NULL);

  /* cause lwes_hash_create to fail */
  malloc_count = 0;
  null_at      = 0;
  hash_null    = 1;
  assert (lwes_event_create_no_name (NULL) == NULL);
  assert (lwes_event_create (NULL, "Foo") == NULL);
  hash_null    = 0;

  /* cause lwes_set_name to fail in creates */
  malloc_count = 0;
  null_at      = 2;
  assert (lwes_event_create (NULL, "Foo") == NULL);

  assert ((event = lwes_event_create (NULL, "Foo")) != NULL);

  /* 1st round is malloc of attribute value failures */
  /* 2nd round is malloc of attribute name failures */
  /* 3rd round is failure to create attribute structure */
  /* 4th round is hash put failing */
  for ( i = 1 ; i < 5 ; i++ )
    {
      if ( i == 4 )
        {
          put_null = 1;
          null_at  = 0;
        }
      else
        {
          null_at      = i;
        }

      malloc_count = 0;
      assert (lwes_event_set_U_INT_16 (event, "Bar", 10) == -3);
      malloc_count = 0;
      assert (lwes_event_set_INT_16 (event, "Bar", 10) == -3);
      malloc_count = 0;
      assert (lwes_event_set_U_INT_32 (event, "Bar", 10) == -3);
      malloc_count = 0;
      assert (lwes_event_set_INT_32 (event, "Bar", 10) == -3);
      malloc_count = 0;
      assert (lwes_event_set_U_INT_64 (event, "Bar", 10) == -3);
      malloc_count = 0;
      assert (lwes_event_set_INT_64 (event, "Bar", 10) == -3);
      malloc_count = 0;
      assert (lwes_event_set_U_INT_64_w_string (event, "Bar", "feedabbadeadbeef")
              == -3);
      malloc_count = 0;
      assert (lwes_event_set_INT_64_w_string (event, "Bar", "feedabbadeadbeaf")
              == -3);
      malloc_count = 0;
      assert (lwes_event_set_STRING (event, "Bar", "feedabbadeadbeaf") == -3);
      malloc_count = 0;
      assert (lwes_event_set_IP_ADDR (event, "Bar", value12) == -3);
      malloc_count = 0;
      assert (lwes_event_set_IP_ADDR_w_string (event, "Bar", "127.0.0.1")
              == -3);
      malloc_count = 0;
      assert (lwes_event_set_BOOLEAN (event, "Bar", 1) == -3);
    }

  /* reset error counters */
  malloc_count = 0;
  null_at = 0;
  put_null = 0;

  /* few remaining set cases */
  assert (lwes_event_set_U_INT_64_w_string (event, "Bar", "zzww") == -2);
  assert (lwes_event_set_INT_64_w_string (event, "Bar", "zzww")   == -2);

  assert (lwes_event_get_number_of_attributes (event, &num_attrs) == 0);
  assert (num_attrs == 0);

  /* For the GET methods only error is not found, so search for non existent
     keys */
  assert (lwes_event_get_U_INT_16 (event, "Bar", &value_uint16) == -1);
  assert (lwes_event_get_INT_16   (event, "Bar", &value_int16 ) == -1);
  assert (lwes_event_get_U_INT_32 (event, "Bar", &value_uint32) == -1);
  assert (lwes_event_get_INT_32   (event, "Bar", &value_int32 ) == -1);
  assert (lwes_event_get_U_INT_64 (event, "Bar", &value_uint64) == -1);
  assert (lwes_event_get_INT_64   (event, "Bar", &value_int64 ) == -1);
  assert (lwes_event_get_STRING   (event, "Bar", &value_string) == -1);
  assert (lwes_event_get_IP_ADDR  (event, "Bar", &value_ipaddr) == -1);
  assert (lwes_event_get_BOOLEAN  (event, "Bar", &value_bool  ) == -1);

  assert (lwes_event_destroy (event) == 0);
}

static void
test_set_once_attributes (void)
{
  struct lwes_event *event = NULL;
  LWES_SHORT_STRING tmp_event_name;

  /* Name set in constructor */
  event = lwes_event_create (NULL, "Foo");
  assert (event != NULL);

  /* attempting to set the name once it's been set is verboten */
  assert (lwes_event_set_name (event, "Fooz") == -1);
  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp ("Foo", tmp_event_name) == 0 );

  assert (lwes_event_destroy (event) == 0);

  /* None set in constructor */
  event = lwes_event_create_no_name (NULL);
  assert (event != NULL);

  /* attempting to set the name should succeed */
  assert (lwes_event_set_name (event, "Foo") == 0);

  /* attempting to set the name once it's been set is verboten */
  assert (lwes_event_set_name (event, "Fooz") == -1);
  assert (lwes_event_get_name (event, &tmp_event_name) == 0);
  assert (strcmp ("Foo", tmp_event_name) == 0 );

  assert (lwes_event_destroy (event) == 0);
}

static void
test_null_args (void)
{
  struct lwes_event *event = NULL;
  LWES_SHORT_STRING tmp_event_name;
  LWES_U_INT_16     tmp_num_attrs;

  LWES_U_INT_16     value_uint16;
  LWES_INT_16       value_int16;
  LWES_U_INT_32     value_uint32;
  LWES_INT_32       value_int32;
  LWES_U_INT_64     value_uint64;
  LWES_INT_64       value_int64;
  LWES_LONG_STRING  value_string;
  LWES_IP_ADDR      value_ipaddr;
  LWES_BOOLEAN      value_bool;
  LWES_BYTE         tmp_buf[50];
  struct lwes_event_deserialize_tmp dtmp;

  /* only case where null args are okay is that db is allowd to be NULL */
  assert ((event = lwes_event_create_no_name (NULL)) != NULL);
  assert (lwes_event_destroy (event) == 0);

  assert (lwes_event_create (NULL, NULL) == NULL);

  /* temporary event */
  assert ((event = lwes_event_create_no_name (NULL)) != NULL);

  /* Test all the permutations of calling mutators with null args */
  assert (lwes_event_set_name (NULL, "Foo") == -1);
  assert (lwes_event_set_name (event, NULL) == -1);

  assert (lwes_event_set_U_INT_16 (NULL, "Bar", 10) == -1);
  assert (lwes_event_set_U_INT_16 (event, NULL, 10) == -1);

  assert (lwes_event_set_INT_16 (NULL, "Bar", 10) == -1);
  assert (lwes_event_set_INT_16 (event, NULL, 10) == -1);

  assert (lwes_event_set_U_INT_32 (NULL, "Bar", 10) == -1);
  assert (lwes_event_set_U_INT_32 (event, NULL, 10) == -1);

  assert (lwes_event_set_INT_32 (NULL, "Bar", 10) == -1);
  assert (lwes_event_set_INT_32 (event, NULL, 10) == -1);

  assert (lwes_event_set_U_INT_64 (NULL, "Bar", 10) == -1);
  assert (lwes_event_set_U_INT_64 (event, NULL, 10) == -1);

  assert (lwes_event_set_U_INT_64_w_string (NULL, "Bar", "feedabba") == -1);
  assert (lwes_event_set_U_INT_64_w_string (event, NULL, "feedabba") == -1);
  assert (lwes_event_set_U_INT_64_w_string (event, "Bar", NULL)      == -1);

  assert (lwes_event_set_INT_64 (NULL, "Bar", 10) == -1);
  assert (lwes_event_set_INT_64 (event, NULL, 10) == -1);

  assert (lwes_event_set_INT_64_w_string (NULL, "Bar", "feedabba") == -1);
  assert (lwes_event_set_INT_64_w_string (event, NULL, "feedabba") == -1);
  assert (lwes_event_set_INT_64_w_string (event, "Bar", NULL)      == -1);

  assert (lwes_event_set_STRING (NULL, "Bar", "feedabba") == -1);
  assert (lwes_event_set_STRING (event, NULL, "feedabba") == -1);
  assert (lwes_event_set_STRING (event, "Bar", NULL)      == -1);

  assert (lwes_event_set_IP_ADDR (NULL, "Bar", value12) == -1);
  assert (lwes_event_set_IP_ADDR (event, NULL, value12) == -1);

  assert (lwes_event_set_IP_ADDR_w_string (NULL, "Bar", "127.0.0.1") == -1);
  assert (lwes_event_set_IP_ADDR_w_string (event, NULL, "127.0.0.1") == -1);
  assert (lwes_event_set_IP_ADDR_w_string (event, "Bar", NULL)      == -1);

  assert (lwes_event_set_BOOLEAN (NULL, "Bar", 1) == -1);
  assert (lwes_event_set_BOOLEAN (event, NULL, 0) == -1);

  /* Now test all the permutations of calling accessors with null args */
  assert (lwes_event_get_name (NULL,  &tmp_event_name) == -1);
  assert (lwes_event_get_name (event, NULL)            == -1);

  assert (lwes_event_get_number_of_attributes (NULL,  &tmp_num_attrs) == -1);
  assert (lwes_event_get_number_of_attributes (event, NULL) == -1);

  assert (lwes_event_get_U_INT_16 (NULL,  "Bar", &value_uint16) == -1);
  assert (lwes_event_get_U_INT_16 (event, NULL,  &value_uint16) == -1);
  assert (lwes_event_get_U_INT_16 (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_INT_16   (NULL,  "Bar", &value_int16 ) == -1);
  assert (lwes_event_get_INT_16   (event, NULL,  &value_int16 ) == -1);
  assert (lwes_event_get_INT_16   (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_U_INT_32 (NULL,  "Bar", &value_uint32) == -1);
  assert (lwes_event_get_U_INT_32 (event, NULL,  &value_uint32) == -1);
  assert (lwes_event_get_U_INT_32 (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_INT_32   (NULL,  "Bar", &value_int32 ) == -1);
  assert (lwes_event_get_INT_32   (event, NULL,  &value_int32 ) == -1);
  assert (lwes_event_get_INT_32   (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_U_INT_64 (NULL,  "Bar", &value_uint64) == -1);
  assert (lwes_event_get_U_INT_64 (event, NULL,  &value_uint64) == -1);
  assert (lwes_event_get_U_INT_64 (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_INT_64   (NULL,  "Bar", &value_int64 ) == -1);
  assert (lwes_event_get_INT_64   (event, NULL,  &value_int64 ) == -1);
  assert (lwes_event_get_INT_64   (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_STRING   (NULL,  "Bar", &value_string) == -1);
  assert (lwes_event_get_STRING   (event, NULL,  &value_string) == -1);
  assert (lwes_event_get_STRING   (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_IP_ADDR  (NULL,  "Bar", &value_ipaddr) == -1);
  assert (lwes_event_get_IP_ADDR  (event, NULL,  &value_ipaddr) == -1);
  assert (lwes_event_get_IP_ADDR  (event, "Bar", NULL)          == -1);

  assert (lwes_event_get_BOOLEAN  (NULL,  "Bar", &value_bool)   == -1);
  assert (lwes_event_get_BOOLEAN  (event, NULL,  &value_bool)   == -1);
  assert (lwes_event_get_BOOLEAN  (event, "Bar", NULL)          == -1);

  assert (lwes_event_to_bytes (NULL,  tmp_buf, 50,  0) == -1);
  assert (lwes_event_to_bytes (event, NULL,    50,  0) == -1);
  assert (lwes_event_to_bytes (event, tmp_buf,  0,  0) == -1);
  assert (lwes_event_to_bytes (event, tmp_buf, 50, 50) == -1);

  assert (lwes_event_from_bytes (NULL,  tmp_buf, 50,  0, &dtmp) == -1);
  assert (lwes_event_from_bytes (event, NULL,    50,  0, &dtmp) == -1);
  assert (lwes_event_from_bytes (event, tmp_buf,  0,  0, &dtmp) == -1);
  assert (lwes_event_from_bytes (event, tmp_buf, 50, 50, &dtmp) == -1);
  assert (lwes_event_from_bytes (event, tmp_buf, 50, 50, NULL)  == -1);

  assert (lwes_event_get_number_of_attributes (event, &tmp_num_attrs) == 0);
  assert (tmp_num_attrs == 0);
  assert (lwes_event_destroy (event) == 0);

  assert (lwes_event_destroy (NULL) == 0);
}

static void
test_serialize_errors (void)
{
  LWES_SHORT_STRING  name  = (LWES_SHORT_STRING)"a";
  struct lwes_event *event = NULL;
  LWES_BYTE bytes[LWES_MSG_SIZE_MAX];

  /* failure at marshalling an attribute name */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_BOOLEAN (event, key02, value02) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs, then fail */
    assert (lwes_event_to_bytes (event, bytes, 4, 0) == -5);
    lwes_event_destroy (event);
  }

  /* failure at marshalling an attribute type */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_BOOLEAN (event, key02, value02) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 9 bytes for
       attribute name, then fail */
    assert (lwes_event_to_bytes (event, bytes, 13, 0) == -6);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a u_int_16 attribute value */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_U_INT_16 (event, key04, value04) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_to_bytes (event, bytes, 13, 0) == -7);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a int_16 attribute value */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_INT_16 (event, key06, value06) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_to_bytes (event, bytes, 13, 0) == -8);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a u_int_32 attribute value */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_U_INT_32 (event, key07, value07) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_to_bytes (event, bytes, 13, 0) == -9);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a int_32 attribute value */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_INT_32 (event, key08, value08) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_to_bytes (event, bytes, 13, 0) == -10);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a u_int_64 attribute value */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_U_INT_64 (event, key09, value09) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_to_bytes (event, bytes, 13, 0) == -11);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a int_64 attribute value */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_INT_64 (event, key10, value10) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_to_bytes (event, bytes, 13, 0) == -12);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a boolean attribute type */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_BOOLEAN (event, key02, value02) ==  1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 9 bytes for
       attribute name, plus 1 byte for attribute type, then fail */
    assert (lwes_event_to_bytes (event, bytes, 14, 0) == -13);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a ip address attribute type */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_IP_ADDR (event, key12, value12) == 1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 9 bytes for
       attribute name, plus 1 byte for attribute type, then fail */
    assert (lwes_event_to_bytes (event, bytes, 14, 0) == -14);
    lwes_event_destroy (event);
  }

  /* failure at marshalling a string attribute type */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_STRING (event, key11, value11) == 1);
    /* 2 bytes for name plus 2 bytes for num attrs plus 12 bytes for
       attribute name, plus 1 byte for attribute type, then fail */
    assert (lwes_event_to_bytes (event, bytes, 17, 0) == -15);
    lwes_event_destroy (event);
  }

  /* failure at marshalling number of attributes */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_to_bytes (event, bytes, 3, 0) == -16);
    lwes_event_destroy (event);
  }

  /* failure at marshalling event name */
  {
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_to_bytes (event, bytes, 1, 0) == -17);
    lwes_event_destroy (event);
  }
}

static void
test_deserialize_errors (void)
{
  LWES_SHORT_STRING  name  = (LWES_SHORT_STRING)"a";
  struct lwes_event *event = NULL;
  LWES_BYTE bytes[LWES_MSG_SIZE_MAX];

  /* failure(s) at unmarshalling a uint_16 attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_U_INT_16 (event, key04, value04) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_from_bytes (event, bytes, 13, 0, &dtmp)==-3);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-2);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling an int_16 attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_INT_16 (event, key06, value06) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_from_bytes (event, bytes, 13, 0, &dtmp)==-5);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-4);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling a uint_32 attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_U_INT_32 (event, key07, value07) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_from_bytes (event, bytes, 13, 0, &dtmp)==-7);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-6);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling an int_32 attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_INT_32 (event, key08, value08) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_from_bytes (event, bytes, 13, 0, &dtmp)==-9);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-8);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling a uint_64 attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_U_INT_64 (event, key09, value09) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_from_bytes (event, bytes, 13, 0, &dtmp)==-11);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-10);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling an int_64 attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_INT_64 (event, key10, value10) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 8 bytes for
       attribute name, plus 1 byte for attribute type then fail */
    assert (lwes_event_from_bytes (event, bytes, 13, 0, &dtmp)==-13);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-12);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling a boolean attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_BOOLEAN (event, key02, value02) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 9 bytes for
       attribute name, plus 1 byte for attribute type, then fail */
    assert (lwes_event_from_bytes (event, bytes, 14, 0, &dtmp)==-15);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-14);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling an ip_addr attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_IP_ADDR (event, key12, value12) == 1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 9 bytes for
       attribute name, plus 1 byte for attribute type, then fail */
    assert (lwes_event_from_bytes (event, bytes, 14, 0, &dtmp)==-17);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-16);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure(s) at unmarshalling a string attribute type */
  {
    struct lwes_event_deserialize_tmp dtmp;
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_STRING (event, key11, value11) == 1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* FIRST fail to unmarshall */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 12 bytes for
       attribute name, plus 1 byte for attribute type, then fail */
    assert (lwes_event_from_bytes (event, bytes, 17, 0, &dtmp)==-19);
    lwes_event_destroy (event);

    /* THEN fail to set a field */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 2;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-18);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* unmarshalling a bad type id */
  {
    struct lwes_event_deserialize_tmp dtmp;
    /* create an event with just a name */
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_BOOLEAN (event, key02, value02) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* fail to deserialize it */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* change to an unknown type */
    bytes[13] = 0x69;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp)==-20);
    lwes_event_destroy (event);
  }

  /* failure at unmarshalling the type id */
  {
    struct lwes_event_deserialize_tmp dtmp;
    /* create an event with just a name */
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_BOOLEAN (event, key02, value02) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* fail to deserialize it */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    /* 2 bytes for name plus 2 bytes for num attrs plus 9 bytes for
       attribute name, then fail */
    assert (lwes_event_from_bytes (event, bytes, 13, 0, &dtmp) == -21);
    lwes_event_destroy (event);
  }

  /* failure at unmarshalling the attribute name */
  {
    struct lwes_event_deserialize_tmp dtmp;
    /* create an event with just a name */
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_set_BOOLEAN (event, key02, value02) ==  1);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* fail to deserialize it */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    assert (lwes_event_from_bytes (event, bytes, 4, 0, &dtmp) == -22);
    lwes_event_destroy (event);
  }

  /* failure at unmarshalling the number of elements*/
  {
    struct lwes_event_deserialize_tmp dtmp;
    /* create an event with just a name */
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* fail to deserialize it */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    assert (lwes_event_from_bytes (event, bytes, 2, 0, &dtmp) == -23);
    lwes_event_destroy (event);
  }


  /* failure at setting an event name */
  {
    struct lwes_event_deserialize_tmp dtmp;
    /* create an event with just a name */
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* fail to deserialize it */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    malloc_count = 0;
    null_at = 1;
    assert (lwes_event_from_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0, &dtmp) == -24);
    malloc_count = 0;
    null_at = 0;
    lwes_event_destroy (event);
  }

  /* failure at unmarshalling an event name */
  {
    struct lwes_event_deserialize_tmp dtmp;
    /* create an event with just a name */
    assert ((event = lwes_event_create (NULL, name)) != NULL);
    assert (lwes_event_to_bytes (event, bytes, LWES_MSG_SIZE_MAX, 0) > 0);
    lwes_event_destroy (event);

    /* fail to deserialize it */
    assert ((event = lwes_event_create_no_name (NULL)) != NULL);
    assert (lwes_event_from_bytes (event, bytes, 1, 0, &dtmp) == -25);
    lwes_event_destroy (event);
  }
}

static void
test_enumeration (void)
{
  struct lwes_event *event1;
  struct lwes_event_enumeration e;

  /* create the event */
  event1 = lwes_event_create (NULL, eventname);
  assert ( event1 != NULL );

  /* set one field of each type */
  assert (lwes_event_set_U_INT_16         (event1, key04, value04) ==  1);
  assert (lwes_event_set_INT_16           (event1, key06, value06) ==  2);
  assert (lwes_event_set_U_INT_32         (event1, key07, value07) ==  3);
  assert (lwes_event_set_INT_32           (event1, key08, value08) ==  4);
  assert (lwes_event_set_STRING           (event1, key01, value01) ==  5);
  assert (lwes_event_set_IP_ADDR          (event1, key12, value12) ==  6);
  assert (lwes_event_set_INT_64           (event1, key10, value10) ==  7);
  assert (lwes_event_set_U_INT_64         (event1, key09, value09) ==  8);
  assert (lwes_event_set_BOOLEAN          (event1, key02, value02) ==  9);

  /* enumerate over the fields */
  if (lwes_event_keys (event1, &e))
    {
      LWES_CONST_SHORT_STRING key;
      LWES_TYPE type;
      int done = 0;
      while (! done)
        {
          int ret = 0;
          ret = lwes_event_enumeration_next_element (&e, &key, &type);
          switch (type)
            {
              case LWES_TYPE_U_INT_16:
                {
                  LWES_U_INT_16     value04_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_U_INT_16
                             (event1, key, &value04_o1) ==  0);
                  assert (value04_o1 == value04);
                }
                break;

              case LWES_TYPE_INT_16:
                {
                  LWES_INT_16       value06_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_INT_16
                            (event1, key, &value06_o1) ==  0);
                  assert (value06_o1 == value06);
                }
                break;

              case LWES_TYPE_U_INT_32:
                {
                  LWES_U_INT_32     value07_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_U_INT_32
                            (event1, key, &value07_o1) ==  0 );
                  assert (value07_o1 == value07);
                }
                break;

              case LWES_TYPE_INT_32:
                {
                  LWES_INT_32       value08_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_INT_32
                            (event1, key, &value08_o1) ==  0 );
                  assert (value08_o1 == value08);
                }
                break;

              case LWES_TYPE_STRING:
                {
                  LWES_LONG_STRING  value01_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_STRING
                            (event1, key, &value01_o1) ==  0);
                  assert (strcmp (value01_o1, value01) == 0) ;
                }
                break;

              case LWES_TYPE_IP_ADDR:
                {
                  LWES_IP_ADDR      value12_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_IP_ADDR
                            (event1, key, &value12_o1) ==  0);
                  assert (value12_o1.s_addr == value12.s_addr);
                }
                break;

              case LWES_TYPE_INT_64:
                {
                  LWES_INT_64       value10_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_INT_64
                            (event1, key, &value10_o1) ==  0);
                  assert (value10_o1 == value10);
                }
                break;

              case LWES_TYPE_U_INT_64:
                {
                  LWES_U_INT_64     value09_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_U_INT_64
                            (event1, key, &value09_o1) ==  0);
                  assert (value09_o1 == value09);
                }
                break;

              case LWES_TYPE_BOOLEAN:
                {
                  LWES_BOOLEAN      value02_o1;
                  assert (ret == 1);
                  assert (lwes_event_get_BOOLEAN 
                            (event1, key, &value02_o1) ==  0);
                  assert (value02_o1 == value02);
                }
                break;

              case LWES_TYPE_UNDEFINED:
                {
                  assert (ret == 0);
                  assert (key == NULL);
                  assert (type == LWES_TYPE_UNDEFINED);
                }
                break;
            }
          if (ret == 0)
            {
              done = 1;
            }
        }
    }

  /* destroy the event */
  lwes_event_destroy (event1);
}

int main (void)
{
  value12.s_addr = inet_addr ("127.0.0.1");

  test_event_with_db ();
  test_event_without_db ();
  test_set_once_attributes ();
  test_null_args ();
  test_error_and_boundary_conditions ();
  test_serialize_errors ();
  test_deserialize_errors ();
  test_enumeration ();

  return 0;
}
