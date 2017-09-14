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

#include "lwes_types.h"
#include "lwes_event.h"

#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inttypes.h>

/* maximum datagram size for UDP is 64K minus IP layer overhead which is
   20 bytes for IP header, and 8 bytes for UDP header, so this value
   should be

   65535 - 28 = 65507
 */
const size_t MAX_MSG_SIZE = 65507;
const size_t MAX_QUEUED_ELEMENTS = 10000;


const LWES_SHORT_STRING LWES_UNDEFINED_STRING         = (LWES_SHORT_STRING)"undef";
const LWES_SHORT_STRING LWES_U_INT_16_STRING          = (LWES_SHORT_STRING)"uint16";
const LWES_SHORT_STRING LWES_INT_16_STRING            = (LWES_SHORT_STRING)"int16";
const LWES_SHORT_STRING LWES_U_INT_32_STRING          = (LWES_SHORT_STRING)"uint32";
const LWES_SHORT_STRING LWES_INT_32_STRING            = (LWES_SHORT_STRING)"int32";
const LWES_SHORT_STRING LWES_STRING_STRING            = (LWES_SHORT_STRING)"string";
const LWES_SHORT_STRING LWES_IP_ADDR_STRING           = (LWES_SHORT_STRING)"ip_addr";
const LWES_SHORT_STRING LWES_INT_64_STRING            = (LWES_SHORT_STRING)"int64";
const LWES_SHORT_STRING LWES_U_INT_64_STRING          = (LWES_SHORT_STRING)"uint64";
const LWES_SHORT_STRING LWES_BOOLEAN_STRING           = (LWES_SHORT_STRING)"boolean";
const LWES_SHORT_STRING LWES_BYTE_STRING              = (LWES_SHORT_STRING)"byte";
const LWES_SHORT_STRING LWES_FLOAT_STRING             = (LWES_SHORT_STRING)"float";
const LWES_SHORT_STRING LWES_DOUBLE_STRING            = (LWES_SHORT_STRING)"double";
const LWES_SHORT_STRING LWES_LONG_STRING_STRING       = (LWES_SHORT_STRING)"long_string";
const LWES_SHORT_STRING LWES_U_INT_16_ARRAY_STRING    = (LWES_SHORT_STRING)"uint16_array";
const LWES_SHORT_STRING LWES_INT_16_ARRAY_STRING      = (LWES_SHORT_STRING)"int16_array";
const LWES_SHORT_STRING LWES_U_INT_32_ARRAY_STRING    = (LWES_SHORT_STRING)"uint32_array";
const LWES_SHORT_STRING LWES_INT_32_ARRAY_STRING      = (LWES_SHORT_STRING)"int32_array";
const LWES_SHORT_STRING LWES_INT_64_ARRAY_STRING      = (LWES_SHORT_STRING)"int64_array";
const LWES_SHORT_STRING LWES_U_INT_64_ARRAY_STRING    = (LWES_SHORT_STRING)"uint64_array";
const LWES_SHORT_STRING LWES_STRING_ARRAY_STRING      = (LWES_SHORT_STRING)"string_array";
const LWES_SHORT_STRING LWES_IP_ADDR_ARRAY_STRING     = (LWES_SHORT_STRING)"ip_addr_array";
const LWES_SHORT_STRING LWES_BOOLEAN_ARRAY_STRING     = (LWES_SHORT_STRING)"boolean_array";
const LWES_SHORT_STRING LWES_BYTE_ARRAY_STRING        = (LWES_SHORT_STRING)"byte_array";
const LWES_SHORT_STRING LWES_FLOAT_ARRAY_STRING       = (LWES_SHORT_STRING)"float_array";
const LWES_SHORT_STRING LWES_DOUBLE_ARRAY_STRING      = (LWES_SHORT_STRING)"double_array";
const LWES_SHORT_STRING LWES_N_U_INT_16_ARRAY_STRING  = (LWES_SHORT_STRING)"nullable_uint16_array";
const LWES_SHORT_STRING LWES_N_INT_16_ARRAY_STRING    = (LWES_SHORT_STRING)"nullable_int16_array";
const LWES_SHORT_STRING LWES_N_U_INT_32_ARRAY_STRING  = (LWES_SHORT_STRING)"nullable_uint32_array";
const LWES_SHORT_STRING LWES_N_INT_32_ARRAY_STRING    = (LWES_SHORT_STRING)"nullable_int32_array";
const LWES_SHORT_STRING LWES_N_STRING_ARRAY_STRING    = (LWES_SHORT_STRING)"nullable_string_array";
const LWES_SHORT_STRING LWES_N_INT_64_ARRAY_STRING    = (LWES_SHORT_STRING)"nullable_int64_array";
const LWES_SHORT_STRING LWES_N_U_INT_64_ARRAY_STRING  = (LWES_SHORT_STRING)"nullable_uint64_array";
const LWES_SHORT_STRING LWES_N_BOOLEAN_ARRAY_STRING   = (LWES_SHORT_STRING)"nullable_boolean_array";
const LWES_SHORT_STRING LWES_N_BYTE_ARRAY_STRING      = (LWES_SHORT_STRING)"nullable_byte_array";
const LWES_SHORT_STRING LWES_N_FLOAT_ARRAY_STRING     = (LWES_SHORT_STRING)"nullable_float_array";
const LWES_SHORT_STRING LWES_N_DOUBLE_ARRAY_STRING    = (LWES_SHORT_STRING)"nullable_double_array";


const LWES_SHORT_STRING LWES_META_INFO_STRING=(LWES_SHORT_STRING)"MetaEventInfo";

LWES_CONST_SHORT_STRING
lwes_type_to_string
  (LWES_TYPE type)
{
  switch (type) {
    case LWES_TYPE_U_INT_16: return LWES_U_INT_16_STRING;
    case LWES_TYPE_INT_16:   return LWES_INT_16_STRING;
    case LWES_TYPE_U_INT_32: return LWES_U_INT_32_STRING;
    case LWES_TYPE_INT_32:   return LWES_INT_32_STRING;
    case LWES_TYPE_U_INT_64: return LWES_U_INT_64_STRING;
    case LWES_TYPE_INT_64:   return LWES_INT_64_STRING;
    case LWES_TYPE_BOOLEAN:  return LWES_BOOLEAN_STRING;
    case LWES_TYPE_IP_ADDR:  return LWES_IP_ADDR_STRING;
    case LWES_TYPE_STRING:   return LWES_STRING_STRING;
    case LWES_TYPE_BYTE:     return LWES_BYTE_STRING;
    case LWES_TYPE_FLOAT:    return LWES_FLOAT_STRING;
    case LWES_TYPE_DOUBLE:   return LWES_DOUBLE_STRING;
    default: return LWES_UNDEFINED_STRING;
  }
}

#define STR_TO_TYPE(typ) if (0 == strcmp(LWES_##typ##_STRING, type)) { return LWES_TYPE_##typ; }

LWES_TYPE
lwes_string_to_type
  (LWES_SHORT_STRING type)
{
  STR_TO_TYPE(U_INT_16)
  STR_TO_TYPE(INT_16)
  STR_TO_TYPE(U_INT_32)
  STR_TO_TYPE(INT_32)
  STR_TO_TYPE(U_INT_64)
  STR_TO_TYPE(INT_64)
  STR_TO_TYPE(BOOLEAN)
  STR_TO_TYPE(IP_ADDR)
  STR_TO_TYPE(STRING)
  STR_TO_TYPE(BYTE)
  STR_TO_TYPE(FLOAT)
  STR_TO_TYPE(DOUBLE)
  return LWES_TYPE_UNDEFINED;
}


int
lwes_typed_value_to_stream
  (LWES_TYPE type,
   void* value,
   FILE *stream)
{
  void* v = value;
  switch(type) {
    case LWES_TYPE_U_INT_16: return fprintf(stream,"%hu",     *(LWES_U_INT_16*)v);
    case LWES_TYPE_INT_16:   return fprintf(stream,"%hd",     *(LWES_INT_16*)v);
    case LWES_TYPE_U_INT_32: return fprintf(stream,"%u",      *(LWES_U_INT_32*)v);
    case LWES_TYPE_INT_32:   return fprintf(stream,"%d",      *(LWES_INT_32*)v);
    case LWES_TYPE_U_INT_64: return fprintf(stream,"%"PRIu64, *(LWES_U_INT_64*)v);
    case LWES_TYPE_INT_64:   return fprintf(stream,"%"PRId64, *(LWES_INT_64*)v);
    case LWES_TYPE_BOOLEAN:  return fprintf(stream,"%s",      (1==*(LWES_BOOLEAN*)v)?"true":"false");
    case LWES_TYPE_IP_ADDR:  return fprintf(stream,"%s",      inet_ntoa(*(LWES_IP_ADDR *)v));
    case LWES_TYPE_STRING:   return fprintf(stream,"%s",      (LWES_LONG_STRING)v);
    default: return 0; //fprintf(stream, "<UNKNOWN_FIELD_TYPE>");
  }
  return 0;
}

int
lwes_event_attribute_to_stream
  (struct lwes_event_attribute *attribute,
   FILE *stream)
{
  void* val = attribute->value;
  return lwes_typed_value_to_stream(attribute->type, val, stream);
}
