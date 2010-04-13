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
#include "lwes_marshall_functions.h"

#include <string.h>

#ifndef htonll
#ifdef _BIG_ENDIAN
#define htonll(x)   (x)
#define ntohll(x)   (x)
#else
#define htonll(x)   ((((uint64_t)htonl(x)) << 32) + htonl(x >> 32))
#define ntohll(x)   ((((uint64_t)ntohl(x)) << 32) + ntohl(x >> 32))
#endif
#endif

const LWES_BYTE NULL_CHAR = (LWES_BYTE)'\0';

int marshall_BYTE         (LWES_BYTE         aByte,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 1 )
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
  if ( bytes != NULL && (length-(*offset)) >= 2 )
    {
      LWES_U_INT_16 newInt = htons (anInt);
      memcpy (bytes+(*offset), &newInt, 2);
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
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 2 )
    {
      LWES_INT_16 newInt = htons (anInt);
      memcpy (bytes+(*offset), &newInt, 2);
      (*offset) += 2;
      ret = 2;
    }
  return ret;
}

int marshall_U_INT_32     (LWES_U_INT_32     anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      LWES_U_INT_32 newInt = htonl (anInt);
      memcpy (bytes+(*offset), &newInt, 4);
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
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      LWES_U_INT_32 newInt = htonl (anInt);
      memcpy (bytes+(*offset), &newInt, 4);
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int marshall_U_INT_64     (LWES_U_INT_64     anInt,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 8 )
    {
      LWES_U_INT_64 newInt = htonll (anInt);
      memcpy (bytes+(*offset), &newInt, 8);
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
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 8 )
    {
      LWES_INT_64 newInt = htonll (anInt);
      memcpy (bytes+(*offset), &newInt, 8);
      (*offset) += 8;
      ret = 8;
    }
  return ret;
}

int marshall_BOOLEAN      (LWES_BOOLEAN      aBoolean,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 1 )
    {
      bytes[(*offset)] = aBoolean;
      (*offset)++;
      ret = 1;
    }
  return ret;
}

int marshall_IP_ADDR      (LWES_IP_ADDR      ipAddress,
                           LWES_BYTE_P       bytes,
                           size_t            length,
                           size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      bytes[(*offset)+3]=
        (LWES_BYTE) ( (htonl(ipAddress.s_addr) & (255 << 24)) >> 24);
      bytes[(*offset)+2]=
        (LWES_BYTE) ( (htonl(ipAddress.s_addr) & (255 << 16)) >> 16);
      bytes[(*offset)+1]=
        (LWES_BYTE) ( (htonl(ipAddress.s_addr) & (255 <<  8)) >>  8);
      bytes[(*offset)  ]=
        (LWES_BYTE) ( (htonl(ipAddress.s_addr) & (255)));
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int marshall_IPV4      (LWES_IP_ADDR      ipAddress,
                        LWES_BYTE_P       bytes,
                        size_t            length,
                        size_t            *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      memcpy (bytes+(*offset), &ipAddress.s_addr, 4);
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

  if ( bytes != NULL
       && str_length < 255 && str_length > 0
       && (length-(*offset)) >= (str_length+1) )
    {
      marshall_BYTE (str_length, bytes, length, offset);

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
       && (length-(*offset)) >= (str_length+2) )
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
  if ( bytes != NULL && (length-(*offset)) >= 1 )
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
  if ( bytes != NULL && (length-(*offset)) >= 2 )
    {
      LWES_U_INT_16 newInt;
      memcpy (&newInt, bytes+(*offset), 2);
      *anInt = ntohs (newInt);
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
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 2 )
    {
      LWES_INT_16 newInt;
      memcpy (&newInt, bytes+(*offset), 2);
      *anInt = ntohs (newInt);
      (*offset) += 2;
      ret = 2;
    }
  return ret;
}

int unmarshall_U_INT_32     (LWES_U_INT_32 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      LWES_U_INT_32 newInt;
      memcpy (&newInt, bytes+(*offset), 4);
      *anInt = ntohl (newInt);
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
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      LWES_INT_32 newInt;
      memcpy (&newInt, bytes+(*offset), 4);
      *anInt = ntohl (newInt);
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int unmarshall_U_INT_64     (LWES_U_INT_64 *anInt,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 8 )
    {
      LWES_U_INT_64 newInt;
      memcpy (&newInt, bytes+(*offset), 8);
      *anInt = ntohll (newInt);
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
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 8 )
    {
      LWES_INT_64 newInt;
      memcpy (&newInt, bytes+(*offset), 8);
      *anInt = ntohll (newInt);
      (*offset) += 8;
      ret = 8;
    }
  return ret;
}

int unmarshall_BOOLEAN      (LWES_BOOLEAN *aBoolean,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 1 )
    {
      *aBoolean = bytes[(*offset)];
      (*offset)++;
      ret = 1;
    }
  return ret;
}

int unmarshall_IP_ADDR      (LWES_IP_ADDR *ipAddress,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      ipAddress->s_addr =
                    ntohl( ( ( bytes[(*offset)+3] << 24 ) & (255 << 24))
                         | ( ( bytes[(*offset)+2] << 16 ) & (255 << 16))
                         | ( ( bytes[(*offset)+1] <<  8 ) & (255 <<  8))
                         | ( ( bytes[(*offset)  ] <<  0 ) & (255)));
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int unmarshall_IPV4 (LWES_IPV4 *ipAddress,
                     LWES_BYTE_P bytes,
                     size_t length,
                     size_t *offset)
{
  int ret = 0;
  if ( bytes != NULL && (length-(*offset)) >= 4 )
    {
      memcpy (&ipAddress->s_addr, bytes+(*offset), 4);
      (*offset) += 4;
      ret = 4;
    }
  return ret;
}

int unmarshall_SHORT_STRING (LWES_SHORT_STRING aString,
                             size_t max_string_length,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  LWES_BYTE string_length;
  size_t    data_length;

  LWES_BYTE_P tmp_ptr;
  /* check for byte of length */
  if ( bytes != NULL && (length-(*offset)) >= 1)
    {
      /* get byte of length */
      unmarshall_BYTE (&string_length, bytes, length, offset);

      /* keep track of actual number of bytes in case we truncate */
      data_length = string_length;

      /* truncate string length if necessary */
      if ( string_length > (max_string_length-1) )
      {
        string_length = max_string_length - 1;
      }

      if ( (length-(*offset)) >= string_length )
        {
          tmp_ptr = &(bytes[(*offset)]);

          /* skip full length event if we are truncating */
          (*offset) += data_length;

          memcpy (aString,tmp_ptr,string_length);

          aString[string_length] = NULL_CHAR;
          ret = data_length + 1;
        }
      else
        {
          /* we reset the offset by the length byte */
          (*offset)--;
        }
    }
  return ret;
}

int unmarshall_LONG_STRING  (LWES_LONG_STRING aString,
                             size_t max_string_length,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  LWES_U_INT_16 string_length;
  size_t data_length;

  LWES_BYTE_P tmp_ptr;
  /* check for 2 bytes of length */
  if ( bytes != NULL && (length-(*offset)) >= 2)
    {
      unmarshall_U_INT_16 (&string_length, bytes, length, offset);

      /* keep track of actual number of bytes in case we truncate */
      data_length = string_length;

      /* truncate string length if necessary */
      if ( string_length > (max_string_length-1) )
      {
        string_length = max_string_length - 1;
      }

      if ( (length-(*offset)) >= string_length )
        {
          tmp_ptr = &(bytes[(*offset)]);

          /* skip full length event if we are truncating */
          (*offset) += data_length;

          memcpy (aString,tmp_ptr,string_length);

          aString[string_length] = NULL_CHAR;
          ret = data_length + 2;
        }
      else
        {
          /* we reset the offset by the length byte */
          (*offset) -= 2;
        }
    }
  return ret;
}

