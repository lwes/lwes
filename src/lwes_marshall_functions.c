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
      bytes[(*offset)]   = (LWES_BYTE)( (anInt & (255 << 8)) >> 8);
      bytes[(*offset)+1] = (LWES_BYTE)( (anInt & (255)) );
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
      bytes[(*offset)]   = (LWES_BYTE)( (anInt & (255 << 8)) >> 8);
      bytes[(*offset)+1] = (LWES_BYTE)( (anInt & (255)) );
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
      bytes[(*offset)]   = (LWES_BYTE) ( (anInt & (255 << 24)) >> 24);
      bytes[(*offset)+1] = (LWES_BYTE) ( (anInt & (255 << 16)) >> 16);
      bytes[(*offset)+2] = (LWES_BYTE) ( (anInt & (255 <<  8)) >>  8);
      bytes[(*offset)+3] = (LWES_BYTE) ( (anInt & (255)));
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
      bytes[(*offset)]   = (LWES_BYTE) ( (anInt & (255 << 24)) >> 24);
      bytes[(*offset)+1] = (LWES_BYTE) ( (anInt & (255 << 16)) >> 16);
      bytes[(*offset)+2] = (LWES_BYTE) ( (anInt & (255 <<  8)) >>  8);
      bytes[(*offset)+3] = (LWES_BYTE) ( (anInt & (255)));
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
      bytes[(*offset)  ] = (LWES_BYTE) ( (anInt & (0xffULL << 56)) >> 56);
      bytes[(*offset)+1] = (LWES_BYTE) ( (anInt & (0xffULL << 48)) >> 48);
      bytes[(*offset)+2] = (LWES_BYTE) ( (anInt & (0xffULL << 40)) >> 40);
      bytes[(*offset)+3] = (LWES_BYTE) ( (anInt & (0xffULL << 32)) >> 32);
      bytes[(*offset)+4] = (LWES_BYTE) ( (anInt & (0xffULL << 24)) >> 24);
      bytes[(*offset)+5] = (LWES_BYTE) ( (anInt & (0xffULL << 16)) >> 16);
      bytes[(*offset)+6] = (LWES_BYTE) ( (anInt & (0xffULL <<  8)) >>  8);
      bytes[(*offset)+7] = (LWES_BYTE) ( (anInt & (0xffULL <<  0)) >>  0);
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
      bytes[(*offset)  ] = (LWES_BYTE) ( (anInt & (0xffULL << 56)) >> 56);
      bytes[(*offset)+1] = (LWES_BYTE) ( (anInt & (0xffULL << 48)) >> 48);
      bytes[(*offset)+2] = (LWES_BYTE) ( (anInt & (0xffULL << 40)) >> 40);
      bytes[(*offset)+3] = (LWES_BYTE) ( (anInt & (0xffULL << 32)) >> 32);
      bytes[(*offset)+4] = (LWES_BYTE) ( (anInt & (0xffULL << 24)) >> 24);
      bytes[(*offset)+5] = (LWES_BYTE) ( (anInt & (0xffULL << 16)) >> 16);
      bytes[(*offset)+6] = (LWES_BYTE) ( (anInt & (0xffULL <<  8)) >>  8);
      bytes[(*offset)+7] = (LWES_BYTE) ( (anInt & (0xffULL <<  0)) >>  0);
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
      bytes[(*offset)] = (LWES_BYTE)str_length;
      (*offset)++;
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
      bytes[(*offset)]   = (LWES_BYTE)( (str_length & (255 << 8)) >> 8);
      bytes[(*offset)+1] = (LWES_BYTE)( (str_length & (255)) );
      (*offset)+=2;
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
      *anInt = (LWES_U_INT_16)( ( ( bytes[(*offset)  ] << 8 ) & (255 << 8 ))
                              | ( ( bytes[(*offset)+1] << 0 ) & (255)) );
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
      *anInt = (LWES_INT_16)( ( ( bytes[(*offset)  ] << 8 ) & (255 << 8 ))
                            | ( ( bytes[(*offset)+1] << 0 ) & (255)) );
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
      *anInt = (LWES_U_INT_32)( ( (bytes[(*offset)  ] << 24) & (255 << 24))
                              | ( (bytes[(*offset)+1] << 16) & (255 << 16))
                              | ( (bytes[(*offset)+2] << 8 ) & (255 <<  8))
                              | ( (bytes[(*offset)+3] << 0 ) & (255)) );
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
      *anInt = (LWES_INT_32)( ( (bytes[(*offset)  ] << 24) & (255 << 24))
                            | ( (bytes[(*offset)+1] << 16) & (255 << 16))
                            | ( (bytes[(*offset)+2] << 8 ) & (255 <<  8))
                            | ( (bytes[(*offset)+3] << 0 ) & (255)) );
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
      *anInt = (LWES_U_INT_64)
          ( ( ((LWES_U_INT_64)bytes[(*offset)  ] << 56) & (0xffULL << 56))
          | ( ((LWES_U_INT_64)bytes[(*offset)+1] << 48) & (0xffULL << 48))
          | ( ((LWES_U_INT_64)bytes[(*offset)+2] << 40) & (0xffULL << 40))
          | ( ((LWES_U_INT_64)bytes[(*offset)+3] << 32) & (0xffULL << 32))
          | ( ((LWES_U_INT_64)bytes[(*offset)+4] << 24) & (0xffULL << 24))
          | ( ((LWES_U_INT_64)bytes[(*offset)+5] << 16) & (0xffULL << 16))
          | ( ((LWES_U_INT_64)bytes[(*offset)+6] << 8 ) & (0xffULL <<  8))
          | ( ((LWES_U_INT_64)bytes[(*offset)+7] << 0 ) & (0xffULL <<  0)));
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
      *anInt = (LWES_INT_64)
          ( ( ((LWES_INT_64)bytes[(*offset)  ] << 56) & (0xffULL << 56))
          | ( ((LWES_INT_64)bytes[(*offset)+1] << 48) & (0xffULL << 48))
          | ( ((LWES_INT_64)bytes[(*offset)+2] << 40) & (0xffULL << 40))
          | ( ((LWES_INT_64)bytes[(*offset)+3] << 32) & (0xffULL << 32))
          | ( ((LWES_INT_64)bytes[(*offset)+4] << 24) & (0xffULL << 24))
          | ( ((LWES_INT_64)bytes[(*offset)+5] << 16) & (0xffULL << 16))
          | ( ((LWES_INT_64)bytes[(*offset)+6] << 8 ) & (0xffULL <<  8))
          | ( ((LWES_INT_64)bytes[(*offset)+7] << 0 ) & (0xffULL <<  0)));
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


int unmarshall_SHORT_STRING (LWES_SHORT_STRING aString,
                             size_t max_string_length,
                             LWES_BYTE_P bytes,
                             size_t length,
                             size_t *offset)
{
  int ret = 0;
  size_t string_length;
  size_t data_length;

  LWES_BYTE_P tmp_ptr;
  /* check for byte of length */
  if ( bytes != NULL && (length-(*offset)) >= 1)
    {
      /* get byte of length */
      string_length = (LWES_BYTE)bytes[(*offset)];
      (*offset)++;

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
  size_t string_length;
  size_t data_length;

  LWES_BYTE_P tmp_ptr;
  /* check for 2 bytes of length */
  if ( bytes != NULL && (length-(*offset)) >= 2)
    {
      string_length = (LWES_U_INT_16)
        ( ( ( bytes[(*offset)  ] << 8 ) & (255 << 8))
        | ( ( bytes[(*offset)+1] << 0 ) & (255)) );
      (*offset)+=2;

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

