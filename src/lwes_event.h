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

#ifndef __LWES_EVENT_FUNCTIONS_H
#define __LWES_EVENT_FUNCTIONS_H

#include "lwes_types.h"
#include "lwes_hash.h"
#include "lwes_event_type_db.h"

#include <stdio.h>

#define LWES_ENCODING "enc"

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file lwes_event.h
 *  \brief Functions for dealing with LWES events
 */

/*! \struct lwes_event_deserialize_tmp lwes_event.h
 *  \brief Storage for use when deserializing strings from events
 */
struct lwes_event_deserialize_tmp
{
  /*! a LWES_SHORT_STRING of max size for use when deserializing short
   *  strings */
  LWES_CHAR     tmp_string[SHORT_STRING_MAX+1];
  /*! a LWES_LONG_STRING of max size for use when deserializing long
   *  strings */
  LWES_CHAR     tmp_string_long[LONG_STRING_MAX+1];
};

/*! \struct lwes_event lwes_event.h
 *  \brief Structure representing an event
 */
struct lwes_event
{
  /*! Name of the event */
  LWES_SHORT_STRING            eventName;
  /*! Number of attributes that have been set in the event */
  LWES_U_INT_16                number_of_attributes;
  /*! DB used for validating this event */
  struct lwes_event_type_db *  type_db;
  /*! The attributes which have been set in the event.  This is a hash
   *   keyed by attribute name with a value of struct lwes_event_attribute
   */
  struct lwes_hash *           attributes;
};

/*! \struct lwes_event_attribute lwes_event.h
 *  \brief Structure representing an attribute
 */
struct lwes_event_attribute
{
  /*! The type of the attribute */
  LWES_BYTE         type;
  /*! The value of the attribute */
  void             *value;
};

/*! \struct lwes_event_enumeration lwes_event.h
 *  \brief Structure for enumerating over the values in the event
 */
struct lwes_event_enumeration
{
  /*! enumeration for the underlying hash */
  struct lwes_hash_enumeration hash_enum;
};
 
/*! \brief Create the memory for an event with no name
 *
 *  This is used when deserializing an event from a byte array
 *
 * \param[in] db the event type db to use for this object, if NULL, disable type
 *               checking.
 *
 * \return the newly allocated event or NULL if an error occurred
 */
struct lwes_event *
lwes_event_create_no_name
  (struct lwes_event_type_db *db);

/*! \brief Create the memory for an event
 *
 * This allocate's space for name, so you don't have to dynamically
 * allocate the memory for it.
 *
 * \param[in] name the name of the event
 * \param[in] db the event type db to use for this object, if NULL, disable type
 *            checking.
 *
 * \return the newly allocated event or NULL if an error occurred
 */
struct lwes_event *
lwes_event_create
  (struct lwes_event_type_db *db,
   LWES_CONST_SHORT_STRING name);

/*! \brief Create the memory for an event with encoding
 *
 * This allocate's space for name, so you don't have to dynamically
 * allocate the memory for it
 *
 * \param[in] name the name of the event
 * \param[in] db the event type db to use for this object, if NULL, disable type
 *            checking.
 * \param[in] encoding the character encoding used for strings in this object
 *
 * \return the newly allocated event or NULL if an error occurred
 */
struct lwes_event *
lwes_event_create_with_encoding
  (struct lwes_event_type_db *db,
   LWES_CONST_SHORT_STRING name,
   LWES_INT_16 encoding);

/*! \brief Cleanup the memory for an event 
 *
 * \param[in] event the event to free
 *
 * \return 0 on success, a negative number on failure
 */
int
lwes_event_destroy
  (struct lwes_event *event);

/*! \brief Set the name of the event
 *
 *  Usually only used when lwes_event_create_no_name is used.
 *  This sets the name of the event, clears the memory of previous
 *  settings, allocates new memory as necessary.
 *
 *  \param[in] event the event to set the name of
 *  \param[in] name the new name of the event
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_set_name
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name);

/*! \brief Set the encoding of the event
 *
 *  Note the encoding is just advisory, it is up to the user of the library
 *  to insure the encoding of strings matches what is placed here
 *
 *  \param[in] event the event to set the encoding of
 *  \param[in] encoding the new encoding
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_set_encoding
  (struct lwes_event *event,
   LWES_INT_16 encoding);

/*! \brief Add a LWES_U_INT_16 attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_U_INT_16
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_U_INT_16 value);

/*! \brief Add a LWES_INT_16 attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_INT_16
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_INT_16   value);

/*! \brief Add a LWES_U_INT_32 attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_U_INT_32
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_U_INT_32 value);

/*! \brief Add a LWES_INT_32 attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_INT_32
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_INT_32   value);

/*! \brief Add a LWES_U_INT_64 attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_U_INT_64
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_U_INT_64 value);

/*! \brief Add a LWES_U_INT_64 attribute to the event with a string value
 *
 *  Internally converts the string to an LWES_U_INT_64
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the string value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_U_INT_64_w_string
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_CONST_SHORT_STRING value);

/*! \brief Add a LWES_INT_64 attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_INT_64
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_INT_64   value);

/*! \brief Add a LWES_INT_64 attribute to the event with a string value
 *
 *  Internally converts the string to an LWES_INT_64
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the string value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_INT_64_w_string
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_CONST_SHORT_STRING value);

/*! \brief Add an LWES_LONG_STRING attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_STRING
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_CONST_LONG_STRING  value);

/*! \brief Add an LWES_IP_ADDR attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_IP_ADDR
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_IP_ADDR      value);

/*! \brief Add a LWES_IP_ADDR attribute to the event with a string value
 *
 *  The string should be of the form xxx.xxx.xxx.xxx, in other words dotted
 *  quad notation.
 *
 *  Internally converts the string from a dotted quad string
 *  to an LWES_IP_ADDR.
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the string value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_IP_ADDR_w_string
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_CONST_SHORT_STRING value);

/*! \brief Add an LWES_BOOLEAN attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return the new number of attributes on succes, a negative number on
 *          failure.
 */
int
lwes_event_set_BOOLEAN
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_BOOLEAN      value);

/*! \brief Get the name of the event
 *
 *  \param[in] event the event to get the name of
 *  \param[in] name the name of the event, this should be treated as const
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_name
  (struct lwes_event *event,
   LWES_SHORT_STRING *name);

/*! \brief Get the number of attributes in the event
 *
 *  \param[in] event the event to get the number of attributes from
 *  \param[out] number the number of attributes
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_number_of_attributes
  (struct lwes_event *event, 
   LWES_U_INT_16 *number);

/*! \brief Get the encoding of the event
 *
 *  Note the encoding is just advisory, it is up to the user of the library
 *  to insure the encoding of strings matches what is placed here
 *
 *  \param[in] event the event to get the encoding from
 *  \param[out] encoding the encoding of the event
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_encoding
  (struct lwes_event *event,
   LWES_INT_16       *encoding);

/*! \brief Get an LWES_U_INT_16 attribute from the event
 *
 *  \param[in] event the event to get the attribute from
 *  \param[in] name the name of the attribute
 *  \param[out] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_U_INT_16
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_U_INT_16     *value);

/*! \brief Get an LWES_INT_16 attribute from the event
 *
 *  \param[in] event the event to get the attribute from
 *  \param[in] name the name of the attribute
 *  \param[out] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_INT_16
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_INT_16       *value);

/*! \brief Get an LWES_U_INT_32 attribute from the event
 *
 *  \param[in] event the event to get the attribute from
 *  \param[in] name the name of the attribute
 *  \param[out] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_U_INT_32
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_U_INT_32     *value);

/*! \brief Get an LWES_INT_32 attribute from the event
 *
 *  \param[in] event the event to get the attribute from
 *  \param[in] name the name of the attribute
 *  \param[out] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_INT_32
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_INT_32       *value);

/*! \brief Get an LWES_U_INT_64 attribute from the event
 *
 *  \param[in] event the event to get the attribute from
 *  \param[in] name the name of the attribute
 *  \param[out] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_U_INT_64
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_U_INT_64     *value);

/*! \brief Get an LWES_INT_64 attribute from the event
 *
 *  \param[in] event the event to get the attribute from
 *  \param[in] name the name of the attribute
 *  \param[out] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_INT_64
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_INT_64       *value);

/*! \brief Get an LWES_LONG_STRING attribute to the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute, this should be treated
 *                   as const
 *
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_STRING
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_LONG_STRING  *value);

/*! \brief Get an LWES_IP_ADDR attribute from the event
 *
 *  \param[in] event the event to add the attribute to
 *  \param[in] name the name of the attribute
 *  \param[in] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_IP_ADDR
  (struct lwes_event       *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_IP_ADDR            *value);

/*! \brief Get an LWES_BOOLEAN attribute from the event
 *
 *  \param[in] event the event to get the attribute from
 *  \param[in] name the name of the attribute
 *  \param[out] value the value of the attribute
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_get_BOOLEAN
  (struct lwes_event       *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_BOOLEAN            *value);

/*! \brief Serialize an event
 
   Serialization format is 

   <pre>
    1 byte event name length ( 1 < b <= 255 )
    b bytes
    2 bytes num_attributes ( 0 < n < 65535, 65535 is a theoretical limit )
    foreach attribute
      1 byte attribute name length ( 1 < a <= 255 )
      a bytes of attribute name
      1 byte type id
      n bytes data

   We want to determine the maximum possible number of attributes
   ( the real upper bound on n above ).

   So we need to minimize all the fields,

   Event name     = 1 byte of length + 1 byte of data = 2 bytes
   Num Attributes = 2 bytes

   So the minimum is 4 bytes, that leaves 65531 to play with

   For attributes,

    minimum attribute length =
        (minimum attr name length                     = 1 byte)
      + (minimum attr name value, given minimum length= 1 byte)
      + (type id length                               = 1 byte)
      + (minimum size of attribute);

    Where

    minimum size boolean attribute = 1 byte
    minimum size uint16  attribute = 2 bytes
    minimum size int16   attribute = 2 bytes
    minimum size uint32  attribute = 4 bytes
    minimum size int32   attribute = 4 bytes
    minimum size uint64  attribute = 8 bytes
    minimum size int64   attribute = 8 bytes
    minimum size ipaddr  attribute = 4 bytes
    minimum size string  attribute = 2 bytes length + 1 byte data = 3 bytes

    so

    minimum boolean attribute =  4 byte
    minimum uint16  attribute =  5 bytes
    minimum int16   attribute =  5 bytes
    minimum uint32  attribute =  7 bytes
    minimum int32   attribute =  7 bytes
    minimum uint64  attribute = 11 bytes
    minimum int64   attribute = 11 bytes
    minimum ipaddr  attribute =  7 bytes
    minimum string  attribute =  6 bytes

    If attribute names are allowed to be duplicated for a type, then the
    maximum number of attributes is

      floor ( (65535 - 4) / 4) = 16382

    However, attribute names are not allowed to be duplicated for any
    one type, so that puts a cap of 254, 1 non-zero byte attribute
    names ( 1 -> 255 ) for a single type, this means the actual number
    of attributes is actually less.

    To maximally pack attributes we would start with the smallest
    in terms of number of bytes, then work our way upward calculating
    the number of attributes of a certain attribute name length of
    the smallest current type, so to start the chain

    4 bytes overhead                  =    4

    1 byte attribute booleans 254*4   = 1016

    2 byte attribute booleans 254*5   = 1270
    1 byte attribute uint16   254*5   = 1270
    1 byte attribute int16    254*5   = 1270

    1 byte attribute string   254*6   = 1524
    3 byte attribute booleans 254*6   = 1524
    2 byte attribute uint16   254*6   = 1524
    2 byte attribute int16    254*6   = 1524

    1 byte attribute uint32   254*7   = 1778
    1 byte attribute int32    254*7   = 1778
    2 byte attribute string   254*7   = 1778
    4 byte attribute booleans 254*7   = 1778
    3 byte attribute uint16   254*7   = 1778
    3 byte attribute int16    254*7   = 1778
    1 byte attribute ipaddr   254*7   = 1778

    2 byte attribute uint32   254*8   = 2032
    2 byte attribute int32    254*8   = 2032
    3 byte attribute string   254*8   = 2032
    5 byte attribute booleans 254*8   = 2032
    4 byte attribute uint16   254*8   = 2032
    4 byte attribute int16    254*8   = 2032
    2 byte attribute ipaddr   254*8   = 2032

    3 byte attribute uint32   254*9   = 2286
    3 byte attribute int32    254*9   = 2286
    4 byte attribute string   254*9   = 2286
    6 byte attribute booleans 254*9   = 2286
    5 byte attribute uint16   254*9   = 2286
    5 byte attribute int16    254*9   = 2286
    3 byte attribute ipaddr   254*9   = 2286

    4 byte attribute uint32   254*10  = 2540
    4 byte attribute int32    254*10  = 2540
    5 byte attribute string   254*10  = 2540
    7 byte attribute booleans 254*10  = 2540

    6 byte attribute uint16   177*10  = 1770

    7 bytes left over                 =    7

    total attrs = 33 * 254 + 177 = 8559
    </pre>

    \param[in] event the event to serialize
    \param[in] bytes the byte array to serialize into
    \param[in] num_bytes the size of the byte array
    \param[in] offset the offset into the array to start serializing at

    \return The number of bytes written to the array on success,
             a negative number on failure
*/
int
lwes_event_to_bytes
  (struct lwes_event *event,
   LWES_BYTE_P bytes,
   size_t num_bytes,
   size_t offset);

/*! \brief Deserialize an event

    \param[in] event the event to deserialize into
    \param[in] bytes the byte array to serialize into
    \param[in] num_bytes the size of the byte array
    \param[in] offset the offset into the array to start serializing at
    \param[in] dtmp some temporary space to deserialize strings into

    \return The number of bytes read from the array on success,
             a negative number on failure
*/
int
lwes_event_from_bytes
  (struct lwes_event *event,
   LWES_BYTE_P bytes,
   size_t num_bytes,
   size_t offset,
   struct lwes_event_deserialize_tmp *dtmp);

/*! \brief Start an enumeration over the keys of the event
 *
 *  The pattern for enumerating is as follows
 *
 *  struct lwes_event_enumeration e;
 *
 *  if (lwes_event_keys (event, &e))
 *    {
 *      LWES_CONST_SHORT_STRING key;
 *      LWES_TYPE type;
 *      while (lwes_event_enumeration_next_element (&e, &key, &type))
 *        {
 *           switch (type)
 *            {
 *              case LWES_TYPE_U_INT_16:
 *                {
 *                  LWES_U_INT_16     value;
 *                  lwes_event_get_U_INT_16 (event, key, &value);
 *                }
 *                break;
 *              // ... cases for rest of types
 *            }
 *        }
 *    }
 *
 *  \param[in] event the event to enumerate over
 *  \param[in] enumeration keeps track of the enumeration
 *
 *  \return 1 if it's possible to enumerate over this event, 0 if it is not.
 */
int
lwes_event_keys
  (struct lwes_event * event,
   struct lwes_event_enumeration *enumeration);

/*! \brief Get the next element from the event
 *
 *  \param[in] enumeration keeps track of the enumeration
 *  \param[out] key the key for the next element of the event
 *  \param[out] type the type of the next element of the event
 *
 *  \return 1 if it's possible to enumerate over this event, 0 if it is not.
 */
int
lwes_event_enumeration_next_element
  (struct lwes_event_enumeration *enumeration,
   LWES_CONST_SHORT_STRING *key,
   LWES_TYPE *type); 

#ifdef __cplusplus
}
#endif 
#endif /* __LWES_EVENT_FUNCTIONS_H */
