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

#ifndef __LWES_MARSHAL_FUNCTIONS_H
#define __LWES_MARSHAL_FUNCTIONS_H

#include <ctype.h>
#include <stdio.h>

#include "lwes_types.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file lwes_marshall_functions.h
 *  \brief Functions for marshalling LWES data types to/from byte arrays
 */

/*! \brief Marshall a byte into a byte array
 *
 * Attempt to marshall aByte into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] aByte the byte to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_BYTE
  (LWES_BYTE         aByte,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a 16 bit unsigned int into a byte array
 *
 * Attempt to marshall anInt into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] anInt the LWES_U_INT_16 to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_U_INT_16
  (LWES_U_INT_16     anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a 16 bit int into a byte array
 *
 * Attempt to marshall anInt into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] anInt the LWES_INT_16 to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_INT_16
  (LWES_INT_16       anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a 32 bit unsigned int into a byte array
 *
 * Attempt to marshall anInt into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] anInt the LWES_U_INT_32 to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_U_INT_32
  (LWES_U_INT_32     anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a 32 bit int into a byte array
 *
 * Attempt to marshall anInt into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] anInt the LWES_INT_32 to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_INT_32
  (LWES_INT_32       anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a 64 bit unsigned int into a byte array
 *
 * Attempt to marshall anInt into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] anInt the LWES_U_INT_64 to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_U_INT_64
  (LWES_U_INT_64     anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a 64 bit int into a byte array
 *
 * Attempt to marshall anInt into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] anInt the LWES_INT_64 to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_INT_64
  (LWES_INT_64       anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a boolean into a byte array
 *
 * Attempt to marshall aBoolean into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] aBoolean the LWES_BOOLEAN to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_BOOLEAN
  (LWES_BOOLEAN      aBoolean,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall an ip4v address into a byte array
 *
 * Attempt to marshall ipAddress into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] ipAddress the LWES_IP_ADDR to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_IP_ADDR
  (LWES_IP_ADDR      ipAddress,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a short string into a byte array
 *
 * Attempt to marshall aString into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] aString the LWES_SHORT_STRING to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_SHORT_STRING
  (LWES_SHORT_STRING aString,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Marshall a long string into a byte array
 *
 * Attempt to marshall aString into the given byte array at the given
 * offset if there is enough total space.  The length should be the total
 * length of the array.
 *
 * The offset will be modified by the number of bytes set, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * space to write this.
 *
 *  \param[in] aString the LWES_LONG_STRING to write into the array
 *  \param[in] bytes the byte array to write into
 *  \param[in] length total length of the array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes written on success.
 */
int
marshall_LONG_STRING
  (LWES_LONG_STRING  aString,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t            *offset);

/*! \brief Unmarshall a byte from a byte array
 *
 * Attempt to unmarshall aByte from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] aByte the byte to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_BYTE
  (LWES_BYTE *       aByte,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a 16 bit unsigned int from a byte array
 *
 * Attempt to unmarshall anInt from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] anInt the LWES_U_INT_16 to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_U_INT_16
  (LWES_U_INT_16 *   anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a 16 bit int from a byte array
 *
 * Attempt to unmarshall anInt from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] anInt the LWES_INT_16 to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_INT_16
  (LWES_INT_16 *     anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a 32 bit unsigned int from a byte array
 *
 * Attempt to unmarshall anInt from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] anInt the LWES_U_INT_32 to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_U_INT_32
  (LWES_U_INT_32 *   anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a 32 bit int from a byte array
 *
 * Attempt to unmarshall anInt from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] anInt the LWES_INT_32 to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_INT_32
  (LWES_INT_32 *     anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a 64 bit unsigned int from a byte array
 *
 * Attempt to unmarshall anInt from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] anInt the LWES_U_INT_64 to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_U_INT_64
  (LWES_U_INT_64 *   anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a 64 bit int from a byte array
 *
 * Attempt to unmarshall anInt from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] anInt the LWES_INT_64 to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_INT_64
  (LWES_INT_64 *     anInt,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a boolean from a byte array
 *
 * Attempt to unmarshall aBoolean from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] aBoolean the LWES_BOOLEAN to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_BOOLEAN
  (LWES_BOOLEAN *    aBoolean,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall an ipv4 address from a byte array
 *
 * Attempt to unmarshall ipAddress from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] ipAddress the LWES_IP_ADDR to read from the array and write into
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_IP_ADDR
  (LWES_IP_ADDR *    ipAddress,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a short string from a byte array
 *
 * Attempt to unmarshall aString from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The max_string_length is the maximum length of the passed in string.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] aString the LWES_SHORT_STRING to read from the array
 *                      and write into
 *  \param[in] max_string_length the total length of aString, this will not
 *                               be overflowed
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_SHORT_STRING
  (LWES_SHORT_STRING aString,
   size_t            max_string_length,
   LWES_BYTE_P       bytes,
   size_t            length,
   size_t *          offset);

/*! \brief Unmarshall a long string from a byte array
 *
 * Attempt to unmarshall aString from the given byte array at the
 * given offset, without overflowing the bound.  If the bound would be
 * overflown, an error is returned.
 *
 * The max_string_length is the maximum length of the passed in string.
 *
 * The length should be the total length of the array.
 *
 * The offset will be modified by the number of bytes consumed, and that value
 * will be returned.  Thus a value of 0 is an error, meaning not enough
 * bytes to fill out the given type.
 *
 *  \param[out] aString the LWES_LONG_STRING to read from the array
 *                      and write into
 *  \param[in] max_string_length the total length of aString, this will not
 *                               be overflowed
 *  \param[in] bytes the byte array to read from
 *  \param[in] length total length of the byte array
 *  \param[in,out] offset the offset into the array, then the new offset
 *
 *  \return 0 on error, the number of bytes consumed on success.
 */
int
unmarshall_LONG_STRING
  (LWES_LONG_STRING aString,
   size_t           max_string_length,
   LWES_BYTE_P      bytes,
   size_t           length,
   size_t *         offset);

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_MARSHALL_FUNCTIONS_H */
