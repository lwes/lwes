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

#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* for inet_addr below */


#include "lwes_marshall_functions.h"

static size_t null_at = 0;
static size_t malloc_count = 0;

static
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

#include "lwes_marshall_functions.c"
#include "lwes_types.c"

#undef malloc


//static LWES_BYTE marshal_data[72] = {
static LWES_BYTE marshal_data[84] = {
    0xef,0x01,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0x4b,0x3c,0x4b,0x20, 0x42,0xf9,0xf5,0xdc,0x14,0x4d,0x80,0x00,
    0x00,0x0b,
    0x66,0x6f,0x6f,0x20,0x62,0x61,0x72,0x20,0x62,0x61,0x7a,0x00,0x00,0x09,0x65,0x76,
    0x65,0x6e,0x74,0x4e,0x61,0x6d,0x65,0x08,0x61,0x74,0x74,0x72,0x4e,0x61,0x6d,0x65,
    0x01,0x00,0x00,0x7f,0x01,0x00,0x00,0x7f};

static LWES_BYTE endian_data[72] = {
    0xef,0x01,0xab,0xcd,0xd7,0xfb,0xde,0xad,0xbe,0xef,0xff,0xfe,0x78,0x6b,0xfe,0xed,
    0xab,0xba,0xde,0xad,0xbe,0xef,0xff,0xff,0xff,0xff,0xff,0x67,0x68,0x8b,0x00,0x0b,
    0x66,0x6f,0x6f,0x20,0x62,0x61,0x72,0x20,0x62,0x61,0x7a,0x00,0x00,0x09,0x65,0x76,
    0x65,0x6e,0x74,0x4e,0x61,0x6d,0x65,0x08,0x61,0x74,0x74,0x72,0x4e,0x61,0x6d,0x65,
    0x01,0x00,0x00,0x7f,0x01,0x00,0x00,0x7f};

static LWES_BYTE array_data[] = {
    /* uint16_t array length 3: 123, 5432, 54321 */
    0x81,   0x00, 0x03,
      0x00, 0x7b,
      0x15, 0x38,
      0xd4, 0x31,
    /* string array length 2: "foo", "BarBar" */
    0x85,   0x00, 0x02,
      0x00, 0x03,  0x66, 0x6f, 0x6f,
      0x00, 0x06,  0x42, 0x61, 0x72, 0x42, 0x61, 0x72
    /* TODO the other array types */
};

static LWES_BYTE nullable_data[] = {
    /* uint16_t array length 9: 123, null, 5432, null, 54321, null, null, null, null */
    0x8d,   0x00, 0x09,   0x15, 0x00,
      0x00, 0x7b,
      0x15, 0x38,
      0xd4, 0x31,
    /* string array length 5: null, "foo", null, "BarBar", null */
    0x91,   0x00, 0x05,  0x0a,
      0x00, 0x03,  0x66, 0x6f, 0x6f,
      0x00, 0x06,  0x42, 0x61, 0x72, 0x42, 0x61, 0x72
    /* TODO the other array types */
};


static LWES_BYTE scratch[512];

static LWES_BYTE bad_short_string1[1]  = {0x01};
static LWES_BYTE bad_long_string1 [2]  = {0x00, 0x01};

static void test_typefuncs() 
{
  assert(LWES_TYPE_UNDEFINED == lwes_string_to_type(LWES_UNDEFINED_STRING));

  assert(0 == strcmp(LWES_UNDEFINED_STRING, lwes_type_to_string(LWES_TYPE_UNDEFINED)));
  assert(0 == strcmp(LWES_BYTE_STRING,     lwes_type_to_string(LWES_TYPE_BYTE)));
  assert(0 == strcmp(LWES_BOOLEAN_STRING,  lwes_type_to_string(LWES_TYPE_BOOLEAN)));
  assert(0 == strcmp(LWES_INT_16_STRING,   lwes_type_to_string(LWES_TYPE_INT_16)));
  assert(0 == strcmp(LWES_U_INT_16_STRING, lwes_type_to_string(LWES_TYPE_U_INT_16)));
  assert(0 == strcmp(LWES_INT_32_STRING,   lwes_type_to_string(LWES_TYPE_INT_32)));
  assert(0 == strcmp(LWES_U_INT_32_STRING, lwes_type_to_string(LWES_TYPE_U_INT_32)));
  assert(0 == strcmp(LWES_INT_64_STRING,   lwes_type_to_string(LWES_TYPE_INT_64)));
  assert(0 == strcmp(LWES_U_INT_64_STRING, lwes_type_to_string(LWES_TYPE_U_INT_64)));
  assert(0 == strcmp(LWES_FLOAT_STRING,    lwes_type_to_string(LWES_TYPE_FLOAT)));
  assert(0 == strcmp(LWES_DOUBLE_STRING,   lwes_type_to_string(LWES_TYPE_DOUBLE)));
  assert(0 == strcmp(LWES_IP_ADDR_STRING,  lwes_type_to_string(LWES_TYPE_IP_ADDR)));
  assert(0 == strcmp(LWES_STRING_STRING,   lwes_type_to_string(LWES_TYPE_STRING)));

  assert(0 == strcmp(LWES_BYTE_ARRAY_STRING,     lwes_type_to_string(LWES_TYPE_BYTE_ARRAY)));
  assert(0 == strcmp(LWES_BOOLEAN_ARRAY_STRING,  lwes_type_to_string(LWES_TYPE_BOOLEAN_ARRAY)));
  assert(0 == strcmp(LWES_INT_16_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_INT_16_ARRAY)));
  assert(0 == strcmp(LWES_U_INT_16_ARRAY_STRING, lwes_type_to_string(LWES_TYPE_U_INT_16_ARRAY)));
  assert(0 == strcmp(LWES_INT_32_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_INT_32_ARRAY)));
  assert(0 == strcmp(LWES_U_INT_32_ARRAY_STRING, lwes_type_to_string(LWES_TYPE_U_INT_32_ARRAY)));
  assert(0 == strcmp(LWES_INT_64_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_INT_64_ARRAY)));
  assert(0 == strcmp(LWES_U_INT_64_ARRAY_STRING, lwes_type_to_string(LWES_TYPE_U_INT_64_ARRAY)));
  assert(0 == strcmp(LWES_FLOAT_ARRAY_STRING,    lwes_type_to_string(LWES_TYPE_FLOAT_ARRAY)));
  assert(0 == strcmp(LWES_DOUBLE_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_DOUBLE_ARRAY)));
  assert(0 == strcmp(LWES_IP_ADDR_ARRAY_STRING,  lwes_type_to_string(LWES_TYPE_IP_ADDR_ARRAY)));
  assert(0 == strcmp(LWES_STRING_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_STRING_ARRAY)));

  assert(0 == strcmp(LWES_N_BYTE_ARRAY_STRING,     lwes_type_to_string(LWES_TYPE_N_BYTE_ARRAY)));
  assert(0 == strcmp(LWES_N_BOOLEAN_ARRAY_STRING,  lwes_type_to_string(LWES_TYPE_N_BOOLEAN_ARRAY)));
  assert(0 == strcmp(LWES_N_INT_16_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_N_INT_16_ARRAY)));
  assert(0 == strcmp(LWES_N_U_INT_16_ARRAY_STRING, lwes_type_to_string(LWES_TYPE_N_U_INT_16_ARRAY)));
  assert(0 == strcmp(LWES_N_INT_32_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_N_INT_32_ARRAY)));
  assert(0 == strcmp(LWES_N_U_INT_32_ARRAY_STRING, lwes_type_to_string(LWES_TYPE_N_U_INT_32_ARRAY)));
  assert(0 == strcmp(LWES_N_INT_64_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_N_INT_64_ARRAY)));
  assert(0 == strcmp(LWES_N_U_INT_64_ARRAY_STRING, lwes_type_to_string(LWES_TYPE_N_U_INT_64_ARRAY)));
  assert(0 == strcmp(LWES_N_FLOAT_ARRAY_STRING,    lwes_type_to_string(LWES_TYPE_N_FLOAT_ARRAY)));
  assert(0 == strcmp(LWES_N_DOUBLE_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_N_DOUBLE_ARRAY)));
  /*assert(0 == strcmp(LWES_N_IP_ADDR_ARRAY_STRING,  lwes_type_to_string(LWES_TYPE_N_IP_ADDR_ARRAY)));*/
  assert(0 == strcmp(LWES_N_STRING_ARRAY_STRING,   lwes_type_to_string(LWES_TYPE_N_STRING_ARRAY)));


  assert(0 == lwes_type_to_size(LWES_TYPE_UNDEFINED));
  assert(sizeof(LWES_BYTE)     == lwes_type_to_size(LWES_TYPE_BYTE));
  assert(sizeof(LWES_BOOLEAN)  == lwes_type_to_size(LWES_TYPE_BOOLEAN));
  assert(sizeof(LWES_INT_16)   == lwes_type_to_size(LWES_TYPE_INT_16));
  assert(sizeof(LWES_U_INT_16) == lwes_type_to_size(LWES_TYPE_U_INT_16));
  assert(sizeof(LWES_INT_32)   == lwes_type_to_size(LWES_TYPE_INT_32));
  assert(sizeof(LWES_U_INT_32) == lwes_type_to_size(LWES_TYPE_U_INT_32));
  assert(sizeof(LWES_INT_64)   == lwes_type_to_size(LWES_TYPE_INT_64));
  assert(sizeof(LWES_U_INT_64) == lwes_type_to_size(LWES_TYPE_U_INT_64));
  assert(sizeof(LWES_FLOAT)    == lwes_type_to_size(LWES_TYPE_FLOAT));
  assert(sizeof(LWES_DOUBLE)   == lwes_type_to_size(LWES_TYPE_DOUBLE));
  assert(sizeof(LWES_IP_ADDR)  == lwes_type_to_size(LWES_TYPE_IP_ADDR));
  assert(sizeof(LWES_SHORT_STRING) == lwes_type_to_size(LWES_TYPE_STRING));

  assert(sizeof(LWES_BYTE)     == lwes_type_to_size(LWES_TYPE_BYTE_ARRAY));
  assert(sizeof(LWES_BOOLEAN)  == lwes_type_to_size(LWES_TYPE_BOOLEAN_ARRAY));
  assert(sizeof(LWES_INT_16)   == lwes_type_to_size(LWES_TYPE_INT_16_ARRAY));
  assert(sizeof(LWES_U_INT_16) == lwes_type_to_size(LWES_TYPE_U_INT_16_ARRAY));
  assert(sizeof(LWES_INT_32)   == lwes_type_to_size(LWES_TYPE_INT_32_ARRAY));
  assert(sizeof(LWES_U_INT_32) == lwes_type_to_size(LWES_TYPE_U_INT_32_ARRAY));
  assert(sizeof(LWES_INT_64)   == lwes_type_to_size(LWES_TYPE_INT_64_ARRAY));
  assert(sizeof(LWES_U_INT_64) == lwes_type_to_size(LWES_TYPE_U_INT_64_ARRAY));
  assert(sizeof(LWES_FLOAT)    == lwes_type_to_size(LWES_TYPE_FLOAT_ARRAY));
  assert(sizeof(LWES_DOUBLE)   == lwes_type_to_size(LWES_TYPE_DOUBLE_ARRAY));
  assert(sizeof(LWES_IP_ADDR)  == lwes_type_to_size(LWES_TYPE_IP_ADDR_ARRAY));
  assert(sizeof(LWES_SHORT_STRING) == lwes_type_to_size(LWES_TYPE_STRING_ARRAY));

  assert(sizeof(LWES_BYTE)     == lwes_type_to_size(LWES_TYPE_N_BYTE_ARRAY));
  assert(sizeof(LWES_BOOLEAN)  == lwes_type_to_size(LWES_TYPE_N_BOOLEAN_ARRAY));
  assert(sizeof(LWES_INT_16)   == lwes_type_to_size(LWES_TYPE_N_INT_16_ARRAY));
  assert(sizeof(LWES_U_INT_16) == lwes_type_to_size(LWES_TYPE_N_U_INT_16_ARRAY));
  assert(sizeof(LWES_INT_32)   == lwes_type_to_size(LWES_TYPE_N_INT_32_ARRAY));
  assert(sizeof(LWES_U_INT_32) == lwes_type_to_size(LWES_TYPE_N_U_INT_32_ARRAY));
  assert(sizeof(LWES_INT_64)   == lwes_type_to_size(LWES_TYPE_N_INT_64_ARRAY));
  assert(sizeof(LWES_U_INT_64) == lwes_type_to_size(LWES_TYPE_N_U_INT_64_ARRAY));
  assert(sizeof(LWES_FLOAT)    == lwes_type_to_size(LWES_TYPE_N_FLOAT_ARRAY));
  assert(sizeof(LWES_DOUBLE)   == lwes_type_to_size(LWES_TYPE_N_DOUBLE_ARRAY));
  assert(sizeof(LWES_IP_ADDR)  == lwes_type_to_size(LWES_TYPE_N_IP_ADDR_ARRAY));
  assert(sizeof(LWES_SHORT_STRING) == lwes_type_to_size(LWES_TYPE_N_STRING_ARRAY));

  assert(LWES_TYPE_UNDEFINED == lwes_array_type_to_base(LWES_TYPE_INT_16));
  assert(LWES_TYPE_U_INT_16 == lwes_array_type_to_base(LWES_TYPE_U_INT_16_ARRAY));
  assert(LWES_TYPE_U_INT_16 == lwes_array_type_to_base(LWES_TYPE_N_U_INT_16_ARRAY));
  assert(LWES_TYPE_STRING   == lwes_array_type_to_base(LWES_TYPE_STRING_ARRAY));
  assert(LWES_TYPE_STRING   == lwes_array_type_to_base(LWES_TYPE_N_STRING_ARRAY));
  assert(LWES_TYPE_DOUBLE   == lwes_array_type_to_base(LWES_TYPE_DOUBLE_ARRAY));
  assert(LWES_TYPE_DOUBLE   == lwes_array_type_to_base(LWES_TYPE_N_DOUBLE_ARRAY));

  assert( 0 == lwes_typed_value_to_stream(LWES_TYPE_U_INT_32_ARRAY, NULL, NULL));
  assert( 0 == lwes_typed_value_to_stream(LWES_TYPE_UNDEFINED, NULL, NULL));
  assert( 0 == lwes_typed_array_to_stream(LWES_TYPE_UNDEFINED, NULL, 0, NULL));
}

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
  LWES_FLOAT         aFloat        = 123.4E+5;
  LWES_DOUBLE        aDouble       = 456.7E+12;
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
  LWES_FLOAT         aFloat_1;
  LWES_DOUBLE        aDouble_1;
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
  size_t     offset = 0;
  size_t     offsetw = 0;
  size_t     offset_save = 0;
  size_t     short_string_offset1 = 0;
  size_t     short_string_offset2 = 0;
  unsigned int i=0;
  struct lwes_event_attribute attr;

  test_typefuncs();

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
  assert ( ! marshall_FLOAT        (aFloat,        NULL, num_bytes, &offset) );
  assert ( ! marshall_DOUBLE       (aDouble,       NULL, num_bytes, &offset) );
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
  assert ( ! unmarshall_FLOAT        (&aFloat_1,           NULL, 500, &offset));
  assert ( ! unmarshall_DOUBLE       (&aDouble_1,          NULL, 500, &offset));
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
  assert ( ! marshall_FLOAT        (aFloat,        bytes, num_bytes, &offset) );
  assert ( ! marshall_DOUBLE       (aDouble,       bytes, num_bytes, &offset) );
  assert ( ! marshall_LONG_STRING  (aString,       bytes, num_bytes, &offset) );
  assert ( ! marshall_LONG_STRING  (anEmptyString, bytes, num_bytes, &offset) );
  assert ( ! marshall_SHORT_STRING (eventName,     bytes, num_bytes, &offset) );
  assert ( ! marshall_SHORT_STRING (attrName,      bytes, num_bytes, &offset) );
  assert ( ! marshall_IP_ADDR      (ip_addr,       bytes, num_bytes, &offset) );
  assert ( ! marshall_IP_ADDR      (ip_addr2,      bytes, num_bytes, &offset) );

  assert ( ! unmarshall_BYTE         (&aByte_1,    bytes, num_bytes, &offset));
  assert ( ! unmarshall_BOOLEAN      (&aBool_1,    bytes, num_bytes, &offset));
  assert ( ! unmarshall_U_INT_16     (&uint16_1,   bytes, num_bytes, &offset));
  assert ( ! unmarshall_INT_16       (&int16_1,    bytes, num_bytes, &offset));
  assert ( ! unmarshall_U_INT_32     (&uint32_1,   bytes, num_bytes, &offset));
  assert ( ! unmarshall_INT_32       (&int32_1,    bytes, num_bytes, &offset));
  assert ( ! unmarshall_U_INT_64     (&uint64_1,   bytes, num_bytes, &offset));
  assert ( ! unmarshall_INT_64       (&int64_1,    bytes, num_bytes, &offset));
  /* invalid string length-specifier (in bits) */
  assert ( ! unmarshall_string (aString_1, 500, 3, bytes, num_bytes, &offset));
  assert ( ! unmarshall_LONG_STRING  (aString_1,      500,bytes, num_bytes, &offset));
  assert ( ! unmarshall_LONG_STRING  (anEmptyString_1,500,bytes, num_bytes, &offset));
  assert ( ! unmarshall_SHORT_STRING (eventName_1,    500,bytes, num_bytes, &offset));
  assert ( ! unmarshall_SHORT_STRING (attrName_1,     500,bytes, num_bytes, &offset));
  assert ( ! unmarshall_IP_ADDR      (&ip_addr_1,  bytes, num_bytes, &offset));
  assert ( ! unmarshall_IP_ADDR      (&ip_addr2_1, bytes, num_bytes, &offset));


  assert ( offset == num_bytes );
  for ( i = 0 ; i < num_bytes ; i++ )
    {
      assert (bytes[i] == 0x00 );
    }

  /* actually marshall something now */
  offset = 0;
  assert (   marshall_BYTE         (aByte,         bytes, num_bytes, &offset) );
  assert (   marshall_BOOLEAN      (aBool,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_16     (uint16,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_16       (int16,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_32     (uint32,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_32       (int32,         bytes, num_bytes, &offset) );
  assert (   marshall_U_INT_64     (uint64,        bytes, num_bytes, &offset) );
  assert (   marshall_INT_64       (int64,         bytes, num_bytes, &offset) );
  assert (   marshall_FLOAT        (aFloat,        bytes, num_bytes, &offset) );
  assert (   marshall_DOUBLE       (aDouble,       bytes, num_bytes, &offset) );
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
  assert (unmarshall_FLOAT        (&aFloat_1,            bytes, 500, &offset));
  assert (unmarshall_DOUBLE       (&aDouble_1,           bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (aString_1,       500, bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (anEmptyString_1, 500, bytes, 500, &offset));
  short_string_offset1 = offset;
  assert (unmarshall_SHORT_STRING (eventName_1,     500, bytes, 500, &offset));
  short_string_offset2 = offset;
  assert (unmarshall_SHORT_STRING (NULL,            0,   bytes, 500, &short_string_offset1));
  assert (short_string_offset1 == short_string_offset2);
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
  assert ( aFloat == aFloat_1 );
  assert ( aDouble == aDouble_1 );
  assert ( strcmp(aString,aString_1) == 0 );
  assert ( strcmp(anEmptyString,anEmptyString_1) == 0 );
  assert ( strcmp(eventName,eventName_1) == 0 );
  assert ( strcmp(attrName,attrName_1) == 0 );
  assert ( ip_addr.s_addr == ip_addr_1.s_addr );
  assert ( ip_addr2.s_addr == ip_addr2_1.s_addr );

  /* mashall via generic function */
  offset = 0;
  assert ( ! marshall_generic( LWES_TYPE_UNDEFINED, &aByte,       bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_BYTE,     &aByte,          bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_BOOLEAN,  &aBool,          bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_U_INT_16, &uint16,         bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_INT_16,   &int16,          bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_U_INT_32, &uint32,         bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_INT_32,   &int32,          bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_U_INT_64, &uint64,         bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_INT_64,   &int64,          bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_FLOAT,    &aFloat,         bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_DOUBLE,   &aDouble,        bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_STRING,   &aString,        bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_STRING,   &anEmptyString,  bytes, num_bytes, &offset) );
  assert (!marshall_generic( LWES_TYPE_STRING,   &aTooLongString, bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_IP_ADDR,  &ip_addr,        bytes, num_bytes, &offset) );
  assert ( marshall_generic( LWES_TYPE_IP_ADDR,  &ip_addr2,       bytes, num_bytes, &offset) );
  offset = 0;
  assert (unmarshall_BYTE         (&aByte_1,             bytes, 500, &offset));
  assert (unmarshall_BOOLEAN      (&aBool_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_16     (&uint16_1,            bytes, 500, &offset));
  assert (unmarshall_INT_16       (&int16_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_32     (&uint32_1,            bytes, 500, &offset));
  assert (unmarshall_INT_32       (&int32_1,             bytes, 500, &offset));
  assert (unmarshall_U_INT_64     (&uint64_1,            bytes, 500, &offset));
  assert (unmarshall_INT_64       (&int64_1,             bytes, 500, &offset));
  assert (unmarshall_FLOAT        (&aFloat_1,            bytes, 500, &offset));
  assert (unmarshall_DOUBLE       (&aDouble_1,           bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (aString_1,       500, bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (anEmptyString_1, 500, bytes, 500, &offset));
  assert (unmarshall_SHORT_STRING (NULL,            0,   bytes, 500, &short_string_offset1));
  assert (unmarshall_IP_ADDR      (&ip_addr_1,           bytes, 500, &offset));
  assert (unmarshall_IP_ADDR      (&ip_addr2_1,          bytes, 500, &offset));
  assert ( aByte == aByte_1 );
  assert ( aBool == aBool_1 );
  assert ( uint16 == uint16_1 );
  assert ( int16 == int16_1 );
  assert ( uint32 == uint32_1 );
  assert ( int32 == int32_1 );
  assert ( uint64 == uint64_1 );
  assert ( aFloat == aFloat_1 );
  assert ( aDouble == aDouble_1 );
  assert ( strcmp(aString,aString_1) == 0 );
  assert ( strcmp(anEmptyString,anEmptyString_1) == 0 );
  assert ( ip_addr.s_addr == ip_addr_1.s_addr );
  assert ( ip_addr2.s_addr == ip_addr2_1.s_addr );
  offset = 0;
  assert (!unmarshall_generic(LWES_TYPE_UNDEFINED, &aByte_1,           bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_BYTE,     &aByte_1,             bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_BOOLEAN,  &aBool_1,             bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_U_INT_16, &uint16_1,            bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_INT_16,   &int16_1,             bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_U_INT_32, &uint32_1,            bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_INT_32,   &int32_1,             bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_U_INT_64, &uint64_1,            bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_INT_64,   &int64_1,             bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_FLOAT,    &aFloat_1,            bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_DOUBLE,   &aDouble_1,           bytes, 500, &offset));
  //assert (unmarshall_generic(LWES_TYPE_STRING,   &aString_1,       500, bytes, 500, &offset));
  //assert (unmarshall_generic(LWES_TYPE_STRING,   &anEmptyString_1, 500, bytes, 500, &offset));
  //assert (unmarshall_generic(LWES_TYPE_STRING,   NULL,            0,   bytes, 500, &short_string_offset1));
  assert (unmarshall_LONG_STRING  (aString_1,       500, bytes, 500, &offset));
  assert (unmarshall_LONG_STRING  (anEmptyString_1, 500, bytes, 500, &offset));
  assert (unmarshall_SHORT_STRING (NULL,            0,   bytes, 500, &short_string_offset1));
  assert (unmarshall_generic(LWES_TYPE_IP_ADDR,     &ip_addr_1,           bytes, 500, &offset));
  assert (unmarshall_generic(LWES_TYPE_IP_ADDR,     &ip_addr2_1,          bytes, 500, &offset));
  assert ( aByte == aByte_1 );
  assert ( aBool == aBool_1 );
  assert ( uint16 == uint16_1 );
  assert ( int16 == int16_1 );
  assert ( uint32 == uint32_1 );
  assert ( int32 == int32_1 );
  assert ( uint64 == uint64_1 );
  assert ( aFloat == aFloat_1 );
  assert ( aDouble == aDouble_1 );
  assert ( strcmp(aString,aString_1) == 0 );
  assert ( strcmp(anEmptyString,anEmptyString_1) == 0 );
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

  /* Test array code */
  offset = 0;
  attr.value = NULL;
  attr.type = LWES_TYPE_U_INT_16_ARRAY;
  assert( ! marshall_array_attribute (NULL, scratch, sizeof(scratch), &offset) );
  assert( ! marshall_array_attribute (&attr, scratch, offset, &offset) );

  offset = 0;
  assert( unmarshall_BYTE (&attr.type, array_data, sizeof(array_data), &offset) );
  assert( attr.type == LWES_TYPE_U_INT_16_ARRAY );
  assert( unmarshall_array_attribute (&attr, array_data, sizeof(array_data), &offset) );
  assert( attr.array_len == 3 );
  assert( ((LWES_U_INT_16*)attr.value)[0] ==   123 );
  assert( ((LWES_U_INT_16*)attr.value)[1] ==  5432 );
  assert( ((LWES_U_INT_16*)attr.value)[2] == 54321 );
  free(attr.value);
  attr.value = NULL;

  assert( unmarshall_BYTE (&attr.type, array_data, sizeof(array_data), &offset) );
  assert( attr.type == LWES_TYPE_STRING_ARRAY );
  assert( ! unmarshall_array_attribute (NULL, array_data, offset, &offset) );
  assert( ! unmarshall_array_attribute (&attr, array_data, offset, &offset) );
  assert( unmarshall_array_attribute (&attr, array_data, sizeof(array_data), &offset) );
  assert( attr.array_len == 2 );
  assert( 0 == strcmp ("foo", ((LWES_SHORT_STRING*)attr.value)[0]) );
  assert( 0 == strcmp ("BarBar", ((LWES_SHORT_STRING*)attr.value)[1]) );
  free(attr.value);
  attr.value = NULL;

  /* mixed failure cases */
  offset = 0;
  attr.type = LWES_TYPE_STRING_ARRAY;
  attr.array_len = 1;
  attr.value = &aString;
  assert( marshall_array_attribute (&attr, scratch, sizeof(scratch), &offset) );
  attr.value = NULL;
  offset = 0;
  assert( ! unmarshall_array_attribute (&attr, scratch, 6, &offset) );
  free(attr.value);
  attr.value = NULL;

  offset = 0;
  attr.type = LWES_TYPE_U_INT_16_ARRAY;
  attr.array_len = 1;
  attr.value = &uint16;
  assert( marshall_array_attribute (&attr, scratch, sizeof(scratch), &offset) );
  attr.value = NULL;
  offset = 0;
  assert( ! unmarshall_array_attribute (&attr, scratch, 3, &offset) );
  free(attr.value);
  attr.value = NULL;

  attr.value = &uint16;
  assert( ! marshall_array_attribute (&attr, scratch, 3, &offset) );
  assert( ! marshall_array_attribute (&attr, scratch, 4, &offset) );
  attr.value = NULL;

  /* nullable array generic */
  offset = 0;
  assert( unmarshall_BYTE (&attr.type, nullable_data, sizeof(nullable_data), &offset) );
  assert( attr.type == LWES_TYPE_N_U_INT_16_ARRAY );
  offset_save = offset;
  /* fail read */
  for (i=0; i<8; ++i)
    {
      offset = offset_save;
      assert(!unmarshall_array_attribute (&attr, nullable_data, i, &offset) );
    }
  offset = offset_save;
  /* good read */
  assert( unmarshall_array_attribute (&attr, nullable_data, sizeof(nullable_data), &offset) );
  offset_save = offset;
  assert( attr.array_len == 9 );
  assert( ((LWES_U_INT_16**)attr.value)[0] );
  assert(!((LWES_U_INT_16**)attr.value)[1] );
  assert( ((LWES_U_INT_16**)attr.value)[2] );
  assert(!((LWES_U_INT_16**)attr.value)[3] );
  assert( ((LWES_U_INT_16**)attr.value)[4] );
  assert(!((LWES_U_INT_16**)attr.value)[5] );
  assert(!((LWES_U_INT_16**)attr.value)[6] );
  assert(!((LWES_U_INT_16**)attr.value)[7] );
  assert(!((LWES_U_INT_16**)attr.value)[8] );
  assert( *(((LWES_U_INT_16**)attr.value)[0]) ==   123 );
  assert( *(((LWES_U_INT_16**)attr.value)[2]) ==   5432 );
  assert( *(((LWES_U_INT_16**)attr.value)[4]) ==   54321 );
  /* write it back out */
  offsetw=0;
  assert( marshall_BYTE (attr.type, bytes, num_bytes, &offsetw) );
  assert( marshall_array_attribute (&attr, bytes, num_bytes, &offsetw) );
  free(attr.value);
  attr.value = NULL;
  /* write with errors */
  for (i=0; i<8; ++i)
    {
      offset = 0;
      assert(!marshall_array_attribute (&attr, bytes2, 0, &offset) );
    }
  free(attr.value);
  attr.value = NULL;

  /* nullable array string */
  offset = offset_save;
  assert( unmarshall_BYTE (&attr.type, nullable_data, sizeof(nullable_data), &offset) );
  assert( attr.type == LWES_TYPE_N_STRING_ARRAY );
  /* fail read */
  offset_save = offset;
  for (i=0; i<23; ++i)
    {
      offset = offset_save;
      assert(!unmarshall_array_attribute (&attr, nullable_data, i, &offset) );
    }

  /* fail malloc */
  null_at=1;
  malloc_count=0;
  assert( !unmarshall_array_attribute (&attr, nullable_data, sizeof(nullable_data), &offset) );
  null_at=0;
  malloc_count=0;


  /* good read */
  offset = offset_save;
  assert( unmarshall_array_attribute (&attr, nullable_data, sizeof(nullable_data), &offset) );
  assert( attr.array_len == 5 );
  assert(!((LWES_LONG_STRING*)attr.value)[0] );
  assert( ((LWES_LONG_STRING*)attr.value)[1] );
  assert(!((LWES_LONG_STRING*)attr.value)[2] );
  assert( ((LWES_LONG_STRING*)attr.value)[3] );
  assert(!((LWES_LONG_STRING*)attr.value)[4] );
  assert(0 == strcmp("foo", ((LWES_LONG_STRING*)attr.value)[1]) );
  assert(0 == strcmp("BarBar", ((LWES_LONG_STRING*)attr.value)[3]) );
  /* failed write */
  offset_save = offsetw;
  for (i=0; i<23; ++i)
    {
      offsetw = offset_save;
      assert(!marshall_array_attribute (&attr, bytes2, i, &offsetw) );
    }
  offsetw = offset_save;
  /* write it back out */
  offsetw = offset_save;
  assert( marshall_BYTE (attr.type, bytes, num_bytes, &offsetw) );
  assert( marshall_array_attribute (&attr, bytes, num_bytes, &offsetw) );
  free(attr.value);
  attr.value = NULL;

  for (i = 0; i < offsetw; i++)
    {
      assert(nullable_data[i] == bytes[i]);
    }

  free(attr.value);
  attr.value = NULL;

  return 0;
}
