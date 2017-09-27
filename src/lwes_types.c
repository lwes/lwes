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

#define TYPE_TO_STR(typ) \
  case LWES_TYPE_##typ:       return LWES_##typ##_STRING; \
  case LWES_TYPE_##typ##_ARRAY: return LWES_##typ##_ARRAY_STRING;

LWES_CONST_SHORT_STRING
lwes_type_to_string
  (LWES_TYPE type)
{
  switch (type) {
    TYPE_TO_STR(U_INT_16)
    TYPE_TO_STR(INT_16)
    TYPE_TO_STR(U_INT_32)
    TYPE_TO_STR(INT_32)
    TYPE_TO_STR(U_INT_64)
    TYPE_TO_STR(INT_64)
    TYPE_TO_STR(BOOLEAN)
    TYPE_TO_STR(IP_ADDR)
    TYPE_TO_STR(STRING)
    TYPE_TO_STR(BYTE)
    TYPE_TO_STR(FLOAT)
    TYPE_TO_STR(DOUBLE)
    default: return LWES_UNDEFINED_STRING;
  }
}

#define STR_TO_TYPE(typ) \
  if (0 == strcmp(LWES_##typ##_STRING, type)) { return LWES_TYPE_##typ; } \
  if (0 == strcmp(LWES_##typ##_ARRAY_STRING, type)) { return LWES_TYPE_##typ##_ARRAY; }

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

/* NOTE: this intentionally returns the unit-size for array types */
#define TYPE_TO_SIZE(typ) \
  case LWES_TYPE_##typ:         return sizeof(LWES_##typ); \
  case LWES_TYPE_##typ##_ARRAY: return sizeof(LWES_##typ);

int
lwes_type_to_size
  (LWES_TYPE type)
{
  switch (type) {
    TYPE_TO_SIZE(U_INT_16)
    TYPE_TO_SIZE(INT_16)
    TYPE_TO_SIZE(U_INT_32)
    TYPE_TO_SIZE(INT_32)
    TYPE_TO_SIZE(U_INT_64)
    TYPE_TO_SIZE(INT_64)
    TYPE_TO_SIZE(BOOLEAN)
    TYPE_TO_SIZE(IP_ADDR)
    TYPE_TO_SIZE(BYTE)
    TYPE_TO_SIZE(FLOAT)
    TYPE_TO_SIZE(DOUBLE)
    case LWES_TYPE_STRING       : return sizeof(LWES_SHORT_STRING);
    case LWES_TYPE_STRING_ARRAY : return sizeof(LWES_SHORT_STRING);
    default: return 0;
  }
}

LWES_BOOLEAN
lwes_type_is_array(LWES_TYPE typ)
{
  return ((typ!= LWES_TYPE_UNDEFINED) &&
          (typ >= LWES_TYPE_U_INT_16_ARRAY) )
          ? TRUE : FALSE;
}

LWES_BOOLEAN
lwes_type_is_nullable_array(LWES_TYPE typ)
{
  // TODO
  (void)typ; return FALSE;
  //return ((typ!= LWES_TYPE_UNDEFINED) &&
  //        (typ >= LWES_TYPE_N_U_INT_16_ARRAY)
  //        ? TRUE : FALSE;
}

LWES_TYPE
lwes_array_type_to_base(LWES_TYPE typ)
{
  if (!lwes_type_is_array(typ)) { return LWES_TYPE_UNDEFINED; }
  /* TODO deal with nullable arrays */
  return (typ-LWES_TYPE_U_INT_16_ARRAY)+LWES_TYPE_U_INT_16;
}

int
lwes_typed_value_to_stream
  (LWES_TYPE type,
   void* value,
   FILE *stream)
{
  void* v = value;

  if (lwes_type_is_array(type)) {
    /* can't process it without an array size */
    return 0;
  }

  switch(type) {
    case LWES_TYPE_U_INT_16: return fprintf(stream,"%hu",     *(LWES_U_INT_16*)v);
    case LWES_TYPE_INT_16:   return fprintf(stream,"%hd",     *(LWES_INT_16*)v);
    case LWES_TYPE_U_INT_32: return fprintf(stream,"%u",      *(LWES_U_INT_32*)v);
    case LWES_TYPE_INT_32:   return fprintf(stream,"%d",      *(LWES_INT_32*)v);
    case LWES_TYPE_U_INT_64: return fprintf(stream,"%"PRIu64, *(LWES_U_INT_64*)v);
    case LWES_TYPE_INT_64:   return fprintf(stream,"%"PRId64, *(LWES_INT_64*)v);
    case LWES_TYPE_BOOLEAN:  return fprintf(stream,"%s",      (1==*(LWES_BOOLEAN*)v)?"true":"false");
    case LWES_TYPE_IP_ADDR:  return fprintf(stream,"%s",      inet_ntoa(*(LWES_IP_ADDR *)v));
    case LWES_TYPE_STRING:   return fprintf(stream,"\"%s\"",  (LWES_LONG_STRING)v);
    default: return 0; //fprintf(stream, "<UNKNOWN_FIELD_TYPE>");
  }
  return 0;
}

int
lwes_typed_array_to_stream
  (LWES_TYPE type,
   void* value,
   int size,
   FILE *stream)
{
  char* v = value;
  int i, skip;
  int ret=0;
  LWES_TYPE baseType;

  if (!lwes_type_is_array(type)) {
    return 0;
  }
  baseType = lwes_array_type_to_base(type);
  skip = lwes_type_to_size(type);
  fprintf(stream,"[ ");
    if (LWES_TYPE_STRING == baseType)
      {
        for (i=0; i<size; ++i)
          {
            ret += lwes_typed_value_to_stream(baseType, *(char**)(v+(i*skip)), stream);
            if (i<size-1)
              { fprintf(stream,", "); }
          }
      }
    else
      {
        for (i=0; i<size; ++i)
          {
            ret += lwes_typed_value_to_stream(baseType, v+(i*skip), stream);
            if (i<size-1)
              { fprintf(stream,", "); }
          }
      }
  fprintf(stream," ]");
  return ret;
}


int
lwes_event_attribute_to_stream
  (struct lwes_event_attribute *attribute,
   FILE *stream)
{
  void* val = attribute->value;
  if (lwes_type_is_array(attribute->type))
    {
      return lwes_typed_array_to_stream(attribute->type, val, attribute->array_len, stream);
    }
  else
    {
      return lwes_typed_value_to_stream(attribute->type, val, stream);
    }
}
