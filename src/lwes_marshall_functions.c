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

#include "lwes_marshall_functions.h"

#include <string.h>

const LWES_BYTE NULL_CHAR = (LWES_BYTE)'\0';

int marshall_BYTE         (LWES_BYTE         aByte,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 1 )
    {
      bytes[(*offset)] = aByte;
      (*offset)++;
      ret = 1;
    }
  return ret;
}

int marshall_U_INT_16     (LWES_U_INT_16     anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 2 )
    {
      bytes[(*offset)+0] = (LWES_BYTE) ((anInt >>  8) & 0xffU);
      bytes[(*offset)+1] = (LWES_BYTE) ((anInt >>  0) & 0xffU);
      (*offset) += 2;
      ret = 2;
    }
  return ret;
}

int marshall_INT_16       (LWES_INT_16       anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  return marshall_U_INT_16((LWES_U_INT_16)anInt, bytes, length, offset);
}

int marshall_U_INT_32     (LWES_U_INT_32     anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 4 )
    {
      bytes[(*offset)+0] = (LWES_BYTE) ((anInt >> 24) & 0xffU);
      bytes[(*offset)+1] = (LWES_BYTE) ((anInt >> 16) & 0xffU);
      bytes[(*offset)+2] = (LWES_BYTE) ((anInt >>  8) & 0xffU);
      bytes[(*offset)+3] = (LWES_BYTE) ((anInt >>  0) & 0xffU);
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int marshall_INT_32       (LWES_INT_32       anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  return marshall_U_INT_32((LWES_U_INT_32)anInt, bytes, length, offset);
}

int marshall_U_INT_64     (LWES_U_INT_64     anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 8 )
    {
      bytes[(*offset)+0] = (LWES_BYTE) ((anInt >> 56) & 0xffU);
      bytes[(*offset)+1] = (LWES_BYTE) ((anInt >> 48) & 0xffU);
      bytes[(*offset)+2] = (LWES_BYTE) ((anInt >> 40) & 0xffU);
      bytes[(*offset)+3] = (LWES_BYTE) ((anInt >> 32) & 0xffU);
      bytes[(*offset)+4] = (LWES_BYTE) ((anInt >> 24) & 0xffU);
      bytes[(*offset)+5] = (LWES_BYTE) ((anInt >> 16) & 0xffU);
      bytes[(*offset)+6] = (LWES_BYTE) ((anInt >>  8) & 0xffU);
      bytes[(*offset)+7] = (LWES_BYTE) ((anInt >>  0) & 0xffU);
      (*offset) += 8;
      ret = 8;
    }
  return ret;
}

int marshall_INT_64       (LWES_INT_64       anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  return marshall_U_INT_64((LWES_U_INT_64)anInt, bytes, length, offset);
}

int marshall_BOOLEAN      (LWES_BOOLEAN      aBoolean,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  return marshall_BYTE((LWES_BYTE)aBoolean, bytes, length, offset);
}

int marshall_FLOAT        (LWES_FLOAT        val,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  return marshall_U_INT_32(*((LWES_U_INT_32*)&val), bytes, length, offset);
}

int marshall_DOUBLE       (LWES_DOUBLE       val,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  return marshall_U_INT_64(*((LWES_U_INT_64*)&val), bytes, length, offset);
}

int marshall_IP_ADDR      (LWES_IP_ADDR      ipAddress,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 4 )
    {
      uint32_t addr = htonl(ipAddress.s_addr);
      bytes[(*offset)+3]= (LWES_BYTE) ((addr >> 24) & 0xffU);
      bytes[(*offset)+2]= (LWES_BYTE) ((addr >> 16) & 0xffU);
      bytes[(*offset)+1]= (LWES_BYTE) ((addr >>  8) & 0xffU);
      bytes[(*offset)  ]= (LWES_BYTE) ((addr >>  0) & 0xffU);
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int marshall_SHORT_STRING (LWES_SHORT_STRING aString,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  LWES_BYTE_P tmp_ptr;
  size_t str_length;

  /* null string is an error so return 0 */
  if (aString == NULL)
    {
      return ret;
    }

  /* need to length to insure it's in bounds */
  str_length  = strlen (aString);

  /* if length - (*offset) was negative without the cast it would be an
   * unsigned comparison which was wrong and would result in writing over
   * the end of the array
   */
  if ( bytes != NULL
       && str_length < 255 && str_length > 0
       && ((int)length-(int)(*offset)) >= ((int)str_length+1) )
    {
      marshall_BYTE(str_length, bytes, length, offset);
      tmp_ptr = &(bytes[(*offset)]);
      memcpy(tmp_ptr, aString, str_length);
      (*offset) += str_length;
      ret = (str_length+1);
    }
  return ret;
}

int marshall_LONG_STRING  (LWES_LONG_STRING  aString,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  LWES_BYTE_P tmp_ptr;
  size_t str_length;

   /* null string is an error so return 0 */
  if (aString == NULL)
    {
      return ret;
    }

  /* need to length to insure it's in bounds */
  str_length  = strlen (aString);

  /* since long strings are used as values and an empty string is technically
     a valid value, we'll allow zero length strings. */
  if ( bytes != NULL
       && str_length < 65535
       && ((int)length-(int)(*offset)) >= ((int)str_length+2) )
    {
      marshall_U_INT_16 (str_length, bytes, length, offset);
      tmp_ptr = &(bytes[(*offset)]);
      memcpy(tmp_ptr, aString, str_length);
      (*offset) += str_length;
      ret = (str_length+2);
    }
  return ret;
}

int unmarshall_BYTE         (LWES_BYTE *aByte,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 1 )
    {
      *aByte = bytes[(*offset)];
      (*offset)++;
      ret = 1;
    }
  return ret;
}

int unmarshall_U_INT_16     (LWES_U_INT_16 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 2 )
    {
      *anInt = (LWES_U_INT_16)( ( bytes[(*offset)+0] << 8 )
                              | ( bytes[(*offset)+1] << 0 ) );
      (*offset) += 2;
      ret = 2;
    }
  return ret;
}

int unmarshall_INT_16       (LWES_INT_16 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  return unmarshall_U_INT_16((LWES_U_INT_16*)anInt, bytes, length, offset);
}

int unmarshall_U_INT_32     (LWES_U_INT_32 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 4 )
    {
      *anInt = (LWES_U_INT_32)( ( bytes[(*offset)  ] << 24)
                              | ( bytes[(*offset)+1] << 16)
                              | ( bytes[(*offset)+2] << 8 )
                              | ( bytes[(*offset)+3] << 0 ) );
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int unmarshall_INT_32       (LWES_INT_32 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  return unmarshall_U_INT_32((LWES_U_INT_32*)anInt, bytes, length, offset);
}

int unmarshall_U_INT_64     (LWES_U_INT_64 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 8 )
    {
      *anInt = (LWES_U_INT_64)
          ( ((LWES_U_INT_64)bytes[(*offset)  ] << 56)
          | ((LWES_U_INT_64)bytes[(*offset)+1] << 48)
          | ((LWES_U_INT_64)bytes[(*offset)+2] << 40)
          | ((LWES_U_INT_64)bytes[(*offset)+3] << 32)
          | ((LWES_U_INT_64)bytes[(*offset)+4] << 24)
          | ((LWES_U_INT_64)bytes[(*offset)+5] << 16)
          | ((LWES_U_INT_64)bytes[(*offset)+6] << 8 )
          | ((LWES_U_INT_64)bytes[(*offset)+7] << 0 ));
      (*offset) += 8;
      ret = 8;
    }
  return ret;
}

int unmarshall_INT_64       (LWES_INT_64 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  return unmarshall_U_INT_64((LWES_U_INT_64*)anInt, bytes, length, offset);
}

int unmarshall_BOOLEAN      (LWES_BOOLEAN *aBoolean,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = unmarshall_BYTE((LWES_BYTE*)aBoolean, bytes, length, offset);
  if (1 == ret)
    {
      *aBoolean &= 0xff;
    }
  return ret;
}

int unmarshall_FLOAT       (LWES_FLOAT *val,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  return unmarshall_U_INT_32((LWES_U_INT_32*)val, bytes, length, offset);
}

int unmarshall_DOUBLE       (LWES_DOUBLE *val,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  return unmarshall_U_INT_64((LWES_U_INT_64*)val, bytes, length, offset);
}

int unmarshall_IP_ADDR      (LWES_IP_ADDR *ipAddress,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && ((int)length-(int)(*offset)) >= 4 )
    {
      ipAddress->s_addr =
                    ntohl( ( bytes[(*offset)+3] << 24 )
                         | ( bytes[(*offset)+2] << 16 )
                         | ( bytes[(*offset)+1] <<  8 )
                         | ( bytes[(*offset)  ] <<  0 ));
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int unmarshall_string (LWES_LONG_STRING aString,
                       size_t max_string_length,
                       int string_size_bits,
                       LWES_BYTE_P bytes,
                       size_t length,
                       size_t *offset)
{
  int ret = 0;
  size_t string_length;
  size_t data_length;
  size_t old_offset = *offset;

  LWES_BYTE_P tmp_ptr;
  if (!bytes)
    {
      return 0;
    }

  if (8 == string_size_bits)
    {
      LWES_BYTE sz;
      if (!unmarshall_BYTE(&sz, bytes, length, offset))
        {
          return 0;
        }
      string_length = sz;
    }
  else if (16 == string_size_bits)
    {
      LWES_U_INT_16 sz;
      if (!unmarshall_U_INT_16(&sz, bytes, length, offset))
        {
          return 0;
        }
      string_length = sz;
    }
  else
    {
      return 0;
    }
  /* keep track of actual number of bytes in case we truncate */
  data_length = string_length;

  /* truncate string length if necessary */
  if ( string_length > (max_string_length-1) )
  {
    string_length = max_string_length - 1;
  }

  if ( ((int)length-(int)(*offset)) >= (int)string_length )
    {
      tmp_ptr = &(bytes[(*offset)]);

      /* skip full length event if we are truncating */
      (*offset) += data_length;

      /* if aString is null, we are just calculating an offset but not
       * copying out data
       */
      if (aString != NULL)
        {
          memcpy (aString,tmp_ptr,string_length);
          aString[string_length] = NULL_CHAR;
        }

      ret = data_length + 2;
    }
  else
    {
      /* we reset the offset back */
      (*offset) = old_offset;
    }
  return ret;
}


int unmarshall_SHORT_STRING (LWES_SHORT_STRING aString,
                             size_t max_string_length,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  return unmarshall_string (aString, max_string_length, 8, bytes, length, offset);
}

int unmarshall_LONG_STRING  (LWES_LONG_STRING aString,
                             size_t max_string_length,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  return unmarshall_string (aString, max_string_length, 16, bytes, length, offset);
}

#define TYPED_MARSHALL(typ)                      \
  case LWES_TYPE_##typ:                          \
    return marshall_##typ                        \
      (*(LWES_##typ *)val, bytes, length, offset);

int
marshall_generic
  (LWES_BYTE      type,
   void*          val,
   LWES_BYTE_P    bytes,
   size_t         length,
   size_t *       offset)
{
  switch (type) {
    TYPED_MARSHALL(U_INT_16)
    TYPED_MARSHALL(INT_16)
    TYPED_MARSHALL(U_INT_32)
    TYPED_MARSHALL(INT_32)
    TYPED_MARSHALL(U_INT_64)
    TYPED_MARSHALL(INT_64)
    TYPED_MARSHALL(BOOLEAN)
    TYPED_MARSHALL(IP_ADDR)
    TYPED_MARSHALL(BYTE)
    TYPED_MARSHALL(FLOAT)
    TYPED_MARSHALL(DOUBLE)
    /* NOTE: SHORT vs LONG and a different signature makes this mildly problematic: */
    case LWES_TYPE_STRING: return marshall_LONG_STRING
        (*(LWES_LONG_STRING *)val, bytes, length, offset);
    default: return 0;
  }
}

#define TYPED_UNMARSHALL(typ)                    \
  case LWES_TYPE_##typ:                          \
    return unmarshall_##typ                      \
      ((LWES_##typ *)val, bytes, length, offset);

int
unmarshall_generic
  (LWES_BYTE      type,
   void*          val,
   LWES_BYTE_P    bytes,
   size_t         length,
   size_t *       offset)
{
  switch (type) {
    TYPED_UNMARSHALL(U_INT_16)
    TYPED_UNMARSHALL(INT_16)
    TYPED_UNMARSHALL(U_INT_32)
    TYPED_UNMARSHALL(INT_32)
    TYPED_UNMARSHALL(U_INT_64)
    TYPED_UNMARSHALL(INT_64)
    TYPED_UNMARSHALL(BOOLEAN)
    TYPED_UNMARSHALL(IP_ADDR)
    TYPED_UNMARSHALL(BYTE)
    TYPED_UNMARSHALL(FLOAT)
    TYPED_UNMARSHALL(DOUBLE)
    /* NOTE: SHORT vs LONG and a different signature makes this mildly problematic: */
    /* case LWES_TYPE_STRING: return unmarshall_LONG_STRING
        ((LWES_LONG_STRING *)val, bytes, length, offset); */
    default: return 0;
  }
}


int
marshall_array_attribute
  (struct lwes_event_attribute* attr,
   LWES_BYTE_P     bytes,
   size_t          length,
   size_t*         offset)
{
  int i, delta, w, used=0;
  LWES_BYTE type, baseType;
  char* array;
  if (!attr || !offset || !lwes_type_is_array(attr->type))
    { return 0; }
  w = marshall_U_INT_16(attr->array_len, bytes, length, offset);
  if (!w)
    { return 0; }
  used += w;
  array = (char*) attr->value;
  type = attr->type;
  baseType = lwes_array_type_to_base(type);
  delta = lwes_type_to_size(baseType);
  /* TODO bit array and skips for nullable-arrays, or separate function */
  for (i=0; i<attr->array_len; ++i)
    {
      w = marshall_generic(baseType, array+i*delta, bytes, length, offset);
      if (!w)
        { 
          return 0; 
        }
      used += w;
    }
  return used;
}

int
calculate_array_byte_size
  (LWES_BYTE       type,
   LWES_U_INT_16   array_len,
   LWES_BYTE_P     bytes,
   size_t          length,
   size_t          offset)
{
  LWES_BYTE baseType = lwes_array_type_to_base(type);
  if (baseType == LWES_TYPE_STRING)
    {
      int i, total = 0;
      LWES_U_INT_16 cur;
      /* skip over the actual strings, but add up the sizes */
      for (i=0; i<array_len; ++i) {
        if (!unmarshall_U_INT_16(&cur, bytes, length, &offset))
          { return -1; }
        total += cur+1; /* add space for trailing NULL */
        offset += cur;
      }
      total += lwes_type_to_size(baseType) * array_len;
      return total;
    }
  else if (lwes_type_is_nullable_array(type))
    {
      // TODO discount by number of null elements, add base-array, and bit-vector size
      return -1;
    }
  else
    {
      /* Normal arrays, are just unit_size * num_elements */
      return lwes_type_to_size(baseType) * array_len;
    }
}

int
unmarshall_array_attribute
  (struct lwes_event_attribute* attr,
   LWES_BYTE_P     bytes,
   size_t          length,
   size_t*         offset)
{
  int i, used=0, r, delta, alloc_size;
  LWES_BYTE baseType;

  if (!attr || !offset)
    {
      return 0;
    }
  r = unmarshall_U_INT_16(&(attr->array_len), bytes, length, offset);
  alloc_size = calculate_array_byte_size(attr->type, attr->array_len, bytes, length, *offset);
  if (alloc_size <= 1)
    {
      return 0;
    }
  used += r;
  attr->value = (void*)malloc(alloc_size);
  if (!attr->value)
    {
      return 0;
    }

  baseType = lwes_array_type_to_base(attr->type);
  delta = lwes_type_to_size(baseType);
  if (LWES_TYPE_STRING == baseType)
    {
      // TODO handle nullable
      int arrayPtrBytes = delta*attr->array_len;
      int left = alloc_size - arrayPtrBytes;
      char* *curPtr = attr->value;
      char* curStr = ((char*)attr->value) + arrayPtrBytes;
      for (i=0; i<attr->array_len; ++i)
        {
          r = unmarshall_LONG_STRING(curStr, left, bytes, length, offset);
          if (!r)
            {
              free(attr->value);
              attr->value = NULL;
              return 0;
            }
          curPtr[i] = curStr;
         /* leave off the 2 bytes for (LONG_) string size, but include trailing null */
          curStr += (r-2+1);
          left -= (r-2+1);
          used += r;
        }
    }
  else
    {
      char* cur = attr->value;
      // TODO handle nullable
      for (i=0; i<attr->array_len; ++i)
        {
          r = unmarshall_generic(baseType, cur, bytes, length, offset);
          if (!r)
            {
              free(attr->value);
              attr->value = NULL;
              return 0;
            }
          cur += delta;
          used += r;
        }
    }
  return used;
}

