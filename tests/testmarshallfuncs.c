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
#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* for inet_addr below */


#include "lwes_marshall_functions.h"
#include "lwes_marshall_functions.c"

static LWES_BYTE marshal_data[72] = {
    0xef,0x01,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0x00,0x0b,0x66,0x6f,0x6f,0x20,0x62,0x61,0x72,0x20,0x62,0x61,0x7a,0x00,0x00,
    0x09,0x65,0x76,0x65,0x6e,0x74,0x4e,0x61,0x6d,0x65,0x08,0x61,0x74,0x74,0x72,
    0x4e,0x61,0x6d,0x65,0x01,0x00,0x00,0x7f,0x01,0x00,0x00,0x7f};

static LWES_BYTE endian_data[72] = {
    0xef,0x01,0xab,0xcd,0xd7,0xfb,0xde,0xad,0xbe,0xef,0xff,0xfe,0x78,0x6b,0xfe,
    0xed,0xab,0xba,0xde,0xad,0xbe,0xef,0xff,0xff,0xff,0xff,0xff,0x67,0x68,0x8b,
    0x00,0x0b,0x66,0x6f,0x6f,0x20,0x62,0x61,0x72,0x20,0x62,0x61,0x7a,0x00,0x00,
    0x09,0x65,0x76,0x65,0x6e,0x74,0x4e,0x61,0x6d,0x65,0x08,0x61,0x74,0x74,0x72,
    0x4e,0x61,0x6d,0x65,0x01,0x00,0x00,0x7f,0x01,0x00,0x00,0x7f};

static LWES_BYTE bad_short_string1[1]  = {0x01};
static LWES_BYTE bad_long_string1 [2]  = {0x00, 0x01};

int main(void)
{
  LWES_BYTE          aByte         = (LWES_BYTE)0xef;
  LWES_BOOLEAN       aBool         = 1;
  LWES_U_INT_16      uint16        = 0xffff;
  LWES_INT_16        int16         = -1;
  LWES_U_INT_32      uint32        = 0xffffffff;
  LWES_INT_32        int32         = -1;
  LWES_U_INT_64      uint64        = 0xffffffffffffffffULL;
  LWES_INT_64        int64         = -1;
  LWES_LONG_STRING   aString       = (LWES_LONG_STRING)"foo bar baz";
  LWES_LONG_STRING   truncedString = (LWES_LONG_STRING)"foo ";
  LWES_LONG_STRING   anEmptyString = (LWES_LONG_STRING)"";
  LWES_LONG_STRING   aTooLongString;
  LWES_SHORT_STRING  eventName     = (LWES_SHORT_STRING)"eventName";
  LWES_SHORT_STRING  attrName      = (LWES_SHORT_STRING)"attrName";
  LWES_SHORT_STRING  truncedAttr   = (LWES_SHORT_STRING)"attr";
  LWES_SHORT_STRING  too_short     = (LWES_SHORT_STRING)"";
  LWES_SHORT_STRING  too_big       = (LWES_SHORT_STRING)"12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "12345678901234567890"
                                                        "123456789012345";
  LWES_IP_ADDR       ip_addr;
  LWES_IP_ADDR       ip_addr2;

  LWES_BYTE          aByte_1;
  LWES_BOOLEAN       aBool_1;
  LWES_U_INT_16      uint16_1;
  LWES_INT_16        int16_1;
  LWES_U_INT_32      uint32_1;
  LWES_INT_32        int32_1;
  LWES_U_INT_64      uint64_1;
  LWES_INT_64        int64_1;
  LWES_LONG_STRING   aString_1 = NULL;
  LWES_LONG_STRING   anEmptyString_1 = NULL;
  LWES_SHORT_STRING  eventName_1 = NULL;
  LWES_SHORT_STRING  attrName_1 = NULL;
  LWES_IP_ADDR       ip_addr_1;
  LWES_IP_ADDR       ip_addr2_1;

  /* to place serialized bit into */
  size_t     num_bytes = 500;
  LWES_BYTE  bytes[num_bytes];
  LWES_BYTE  bytes2[num_bytes];
  size_t     small_string_length = 5;
  LWES_CHAR  small_string[small_string_length];
  size_t     long_too_long_bytes = 65536;
  size_t offset = 0;
  unsigned int i=0;

  aTooLongString = (LWES_LONG_STRING)
    malloc(sizeof(LWES_CHAR)*(long_too_long_bytes+1));

  for ( i = 0 ; i < long_too_long_bytes; i++ )
    {
      aTooLongString[i] = (LWES_CHAR)(1 + lrand48 () % 126);
    }
  aTooLongString[long_too_long_bytes] = '\0';

  ip_addr.s_addr = inet_addr("127.0.0.1");
  ip_addr2.s_addr = inet_addr("127.0.0.1");

  for (i = 0; i < num_bytes; i++ )
    {
      bytes[i]  = 0x00;
      bytes2[i] = 0x00;
    }

  /* sanity check 1, passing NULL bytes should return all falses, and have no
     effect on offset */
  assert ( ! marshall_BYTE         (aByte,         NULL, num_bytes, &offset) );
  assert ( ! marshall_BOOLEAN      (aBool,         NULL, num_bytes, &offset) );
  assert ( ! marshall_U_INT_16     (uint16,        NULL, num_bytes, &offset) );
  assert ( ! marshall_INT_16       (int16,         NULL, num_bytes, &offset) );
  assert ( ! marshall_U_INT_32     (uint32,        NULL, num_bytes, &offset) );
  assert ( ! marshall_INT_32       (int32,         NULL, num_bytes, &offset) );
  assert ( ! marshall_U_INT_64     (uint64,        NULL, num_bytes, &offset) );
  assert ( ! marshall_INT_64       (int64,         NULL, num_bytes, &offset) );
  assert ( ! marshall_LONG_STRING  (aString,       NULL, num_bytes, &offset) );
  assert ( ! marshall_LONG_STRING  (anEmptyString, NULL, num_bytes, &offset) );
  assert ( ! marshall_SHORT_STRING (eventName,     NULL, num_bytes, &offset) );
  assert ( ! marshall_SHORT_STRING (attrName,      NULL, num_bytes, &offset) );
  assert ( ! marshall_IP_ADDR      (ip_addr,       NULL, num_bytes, &offset) );
  assert ( ! marshall_IP_ADDR      (ip_addr2,      NULL, num_bytes, &offset) );
  assert ( ! unmarshall_BYTE         (&aByte_1,            NULL, 500, &offset));
  assert ( ! unmarshall_BOOLEAN      (&aBool_1,            NULL, 500, &offset));
  assert ( ! unmarshall_U_INT_16     (&uint16_1,           NULL, 500, &offset));
  assert ( ! unmarshall_INT_16       (&int16_1,            NULL, 500, &offset));
  assert ( ! unmarshall_U_INT_32     (&uint32_1,           NULL, 500, &offset));
  assert ( ! unmarshall_INT_32       (&int32_1,            NULL, 500, &offset));
  assert ( ! unmarshall_U_INT_64     (&uint64_1,           NULL, 500, &offset));
  assert ( ! unmarshall_INT_64       (&int64_1,            NULL, 500, &offset));
  assert ( ! unmarshall_LONG_STRING  (aString_1,      500, NULL, 500, &offset));
  assert ( ! unmarshall_LONG_STRING  (anEmptyString_1,500, NULL, 500, &offset));
  assert ( ! unmarshall_SHORT_STRING (eventName_1,    500, NULL, 500, &offset));
  assert ( ! unmarshall_SHORT_STRING (attrName_1,     500, NULL, 500, &offset));
  assert ( ! unmarshall_IP_ADDR      (&ip_addr_1,          NULL, 500, &offset));
  assert ( ! unmarshall_IP_ADDR      (&ip_addr2_1,         NULL, 500, &offset));
  assert ( offset == 0 );

  /* sanity check 2, an offset equal to the length should result in all falses,
     and should have no effect on offset */
  offset = num_bytes;
  assert ( ! marshall_BYTE         (aByte,         bytes, num_bytes, &offset) );
  assert ( ! marshall_BOOLEAN      (aBool,         bytes, num_bytes, &offset) );
  assert ( ! marshall_U_INT_16     (uint16,        bytes, num_bytes, &offset) );
  assert ( ! marshall_INT_16       (int16,         bytes, num_bytes, &offset) );
  assert ( ! marshall_U_INT_32     (uint32,        bytes, num_bytes, &offset) );
  assert ( ! marshall_INT_32       (int32,         bytes, num_bytes, &offset) );
  assert ( ! marshall_U_INT_64     (uint64,        bytes, num_bytes, &offset) );
  assert ( ! marshall_INT_64       (int64,         bytes, num_bytes, &offset) );
  assert ( ! marshall_LONG_STRING  (aString,       bytes, num_bytes, &offset) );
  assert ( ! marshall_LONG_STRING  (anEmptyString, bytes, num_bytes, &offset) );
  assert ( ! marshall_SHORT_STRING (eventName,     bytes, num_bytes, &offset) );
  assert ( ! marshall_SHORT_STRING (attrName,      bytes, num_bytes, &offset) );
  assert ( ! marshall_IP_ADDR      (ip_addr,       bytes, num_bytes, &offset) );
  assert ( ! marshall_IP_ADDR      (ip_addr2,      bytes, num_bytes, &offset) );

  assert ( ! unmarshall_BYTE         (&aByte_1,           bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_BOOLEAN      (&aBool_1,           bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_U_INT_16     (&uint16_1,          bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_INT_16       (&int16_1,           bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_U_INT_32     (&uint32_1,          bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_INT_32       (&int32_1,           bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_U_INT_64     (&uint64_1,          bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_INT_64       (&int64_1,           bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_LONG_STRING  (aString_1,      500,bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_LONG_STRING  (anEmptyString_1,500,bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_SHORT_STRING (eventName_1,    500,bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_SHORT_STRING (attrName_1,     500,bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_IP_ADDR      (&ip_addr_1,         bytes,
                                      num_bytes, &offset));
  assert ( ! unmarshall_IP_ADDR      (&ip_addr2_1,        bytes,
                                      num_bytes, &offset));

  assert ( offset == num_bytes );
  for ( i = 0 ; i < num_bytes ; i++ )
    {
      assert (bytes[i] == 0x00 );
    }

  /* actually mashall something now */
  offset = 0;
  assert (   marshall_BYTE         (aByte,         bytes, num_bytes, &offset) );
  assert (   marshall_BOOLEAN      (aBool,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_16     (uint16,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_16       (int16,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_32     (uint32,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_32       (int32,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_64     (uint64,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_64       (int64,         bytes, num_bytes, &offset) );
  assert (   marshall_LONG_STRING  (aString,       bytes, num_bytes, &offset) );
  assert (   marshall_LONG_STRING  (anEmptyString, bytes, num_bytes, &offset) );
  assert ( ! marshall_LONG_STRING  (aTooLongString,bytes, num_bytes, &offset) );
  assert (   marshall_SHORT_STRING (eventName,     bytes, num_bytes, &offset) );
  assert (   marshall_SHORT_STRING (attrName,      bytes, num_bytes, &offset) );
  assert (   marshall_IP_ADDR      (ip_addr,       bytes, num_bytes, &offset) );
  assert (   marshall_IP_ADDR      (ip_addr2,      bytes, num_bytes, &offset) );

  /* short strings are not allowed to be empty */
  assert ( ! marshall_SHORT_STRING (too_short,     bytes, num_bytes, &offset) );
  /* string is too big to fit in a short string slot */
  assert ( ! marshall_SHORT_STRING (too_big,       bytes, num_bytes, &offset) );

  for ( i = 0 ; i < offset ; i++ )
    {
      assert ( marshal_data[i] == bytes[i] );
    }

  aString_1   = (LWES_LONG_STRING)malloc(sizeof(LWES_CHAR)*500);
  for (i=0; i< 500 ; i++ ) aString_1[i] = (char)0xf3;

  anEmptyString_1   = (LWES_LONG_STRING)malloc(sizeof(LWES_CHAR)*500);
  for (i=0; i< 500 ; i++ ) anEmptyString_1[i] = (char)0xf3;

  eventName_1 = (LWES_SHORT_STRING)malloc(sizeof(LWES_CHAR)*500);
  for (i=0; i< 500 ; i++ ) eventName_1[i] = (char)0xf3;

  attrName_1  = (LWES_SHORT_STRING)malloc(sizeof(LWES_CHAR)*500);
  for (i=0; i< 500 ; i++ ) attrName_1[i] = (char)0xf3;

  offset = 0;
  assert (unmarshall_BYTE         (&aByte_1,             bytes, 500, &offset));
  assert (unmarshall_BOOLEAN      (&aBool_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_16     (&uint16_1,            bytes, 500, &offset));
  assert (unmarshall_INT_16       (&int16_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_32     (&uint32_1,            bytes, 500, &offset));
  assert (unmarshall_INT_32       (&int32_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_64     (&uint64_1,            bytes, 500, &offset));
  assert (unmarshall_INT_64       (&int64_1,             bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (aString_1,       500, bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (anEmptyString_1, 500, bytes, 500, &offset));
  assert (unmarshall_SHORT_STRING (eventName_1,     500, bytes, 500, &offset));
  assert (unmarshall_SHORT_STRING (attrName_1,      500, bytes, 500, &offset));
  assert (unmarshall_IP_ADDR      (&ip_addr_1,           bytes, 500, &offset));
  assert (unmarshall_IP_ADDR      (&ip_addr2_1,          bytes, 500, &offset));

  assert ( aByte == aByte_1 );
  assert ( aBool == aBool_1 );
  assert ( uint16 == uint16_1 );
  assert ( int16 == int16_1 );
  assert ( uint32 == uint32_1 );
  assert ( int32 == int32_1 );
  assert ( uint64 == uint64_1 );
  assert ( int64 == int64_1 );
  assert ( strcmp(aString,aString_1) == 0 );
  assert ( strcmp(anEmptyString,anEmptyString_1) == 0 );
  assert ( strcmp(eventName,eventName_1) == 0 );
  assert ( strcmp(attrName,attrName_1) == 0 );
  assert ( ip_addr.s_addr == ip_addr_1.s_addr );
  assert ( ip_addr2.s_addr == ip_addr2_1.s_addr );

  /* try something other than palindromic data to make sure we're not
   * doing something endian specific
   */
  aByte     = (LWES_BYTE)0xef;
  aBool     = 1;
  uint16    = 0xabcd;
  int16     = -10245;
  uint32    = 0xdeadbeef;
  int32     = -100245;
  uint64    = 0xfeedabbadeadbeefULL;
  int64     = -10000245;

  for ( i=0; i< num_bytes; i++ ) bytes[i] = 0x00;

  offset = 0;
  assert (   marshall_BYTE         (aByte,         bytes, num_bytes, &offset) );
  assert (   marshall_BOOLEAN      (aBool,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_16     (uint16,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_16       (int16,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_32     (uint32,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_32       (int32,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_64     (uint64,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_64       (int64,         bytes, num_bytes, &offset) );
  assert (   marshall_LONG_STRING  (aString,       bytes, num_bytes, &offset) );
  assert (   marshall_LONG_STRING  (anEmptyString, bytes, num_bytes, &offset) );
  assert (   marshall_SHORT_STRING (eventName,     bytes, num_bytes, &offset) );
  assert (   marshall_SHORT_STRING (attrName,      bytes, num_bytes, &offset) );
  assert (   marshall_IP_ADDR      (ip_addr,       bytes, num_bytes, &offset) );
  assert (   marshall_IP_ADDR      (ip_addr2,      bytes, num_bytes, &offset) );
  /* string is too big to fit in a short string slot */
  assert ( ! marshall_SHORT_STRING (too_big,       bytes, num_bytes, &offset) );

  for ( i = 0 ; i < offset ; i++ )
    {
      assert ( endian_data[i] == bytes[i] );
    }

  for (i=0; i< 500 ; i++ ) aString_1[i]       = (char)0xf3;
  for (i=0; i< 500 ; i++ ) anEmptyString_1[i] = (char)0xf3;
  for (i=0; i< 500 ; i++ ) eventName_1[i]     = (char)0xf3;
  for (i=0; i< 500 ; i++ ) attrName_1[i]      = (char)0xf3;

  offset = 0;
  assert (unmarshall_BYTE         (&aByte_1,             bytes, 500, &offset));
  assert (unmarshall_BOOLEAN      (&aBool_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_16     (&uint16_1,            bytes, 500, &offset));
  assert (unmarshall_INT_16       (&int16_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_32     (&uint32_1,            bytes, 500, &offset));
  assert (unmarshall_INT_32       (&int32_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_64     (&uint64_1,            bytes, 500, &offset));
  assert (unmarshall_INT_64       (&int64_1,             bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (aString_1,       500, bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (anEmptyString_1, 500, bytes, 500, &offset));
  assert (unmarshall_SHORT_STRING (eventName_1,     500, bytes, 500, &offset));
  assert (unmarshall_SHORT_STRING (attrName_1,      500, bytes, 500, &offset));
  assert (unmarshall_IP_ADDR      (&ip_addr_1,           bytes, 500, &offset));
  assert (unmarshall_IP_ADDR      (&ip_addr2_1,          bytes, 500, &offset));

  assert ( aByte == aByte_1 );
  assert ( aBool == aBool_1 );
  assert ( uint16 == uint16_1 );
  assert ( int16 == int16_1 );
  assert ( uint32 == uint32_1 );
  assert ( int32 == int32_1 );
  assert ( uint64 == uint64_1 );
  assert ( int64 == int64_1 );
  assert ( strcmp(aString,aString_1) == 0 );
  assert ( strcmp(anEmptyString,anEmptyString_1) == 0 );
  assert ( strcmp(eventName,eventName_1) == 0 );
  assert ( strcmp(attrName,attrName_1) == 0 );
  assert ( ip_addr.s_addr == ip_addr_1.s_addr );
  assert ( ip_addr2.s_addr == ip_addr2_1.s_addr );

  for ( i=0; i< 500 ; i++ ) aString_1[i]   = (char)0xfe;
  for ( i=0; i< 500 ; i++ ) eventName_1[i] = (char)0xfe;

  /* exercise some boundary conditions, broken strings */
  offset = 0;
  assert ( ! unmarshall_SHORT_STRING ( eventName_1, 500,
                                       bad_short_string1, 1, &offset) );
  assert ( offset == 0 );

  for ( i = 0 ; i < 500 ; i++ )
    {
      assert ( eventName_1[i] == (char)0xfe );
    }

  offset = 0;
  assert ( ! unmarshall_LONG_STRING ( aString_1, 500,
                                      bad_long_string1, 2, &offset) );
  assert ( offset == 0 );

  for ( i = 0 ; i < 500 ; i++ )
    {
      assert ( aString_1[i] == (char)0xfe );
    }

  /* exercise NULL strings */
  offset = 0;
  assert (marshall_SHORT_STRING (NULL, bytes, 500, &offset) == 0);
  assert (offset == 0);

  assert (marshall_LONG_STRING (NULL, bytes, 500, &offset) == 0);
  assert (offset == 0);

  /* test the string truncation code */

  offset = 0;
  assert ( marshall_LONG_STRING  (aString,       bytes2, num_bytes, &offset) );
  assert ( marshall_SHORT_STRING (attrName,      bytes2, num_bytes, &offset) );

  offset = 0;
  assert ( unmarshall_LONG_STRING (small_string, small_string_length,
                                   bytes2, num_bytes, &offset) );
  assert ( strcmp ( small_string, truncedString ) == 0 );

  assert ( unmarshall_SHORT_STRING (small_string, small_string_length,
                                    bytes2, num_bytes, &offset) );
  assert ( strcmp ( small_string, truncedAttr ) == 0 );


  free (aTooLongString);
  free (aString_1);
  free (anEmptyString_1);
  free (eventName_1);
  free (attrName_1);
  return 0;
}
