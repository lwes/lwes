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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <errno.h>

#include "lwes_event.h"
#include "lwes_hash.h"
#include "lwes_marshall_functions.h"

/*************************************************************************
  PRIVATE API prototypes, shouldn't be called by a user of the library.
 *************************************************************************/

/* Create the memory for an event attribute */
static struct lwes_event_attribute *
lwes_event_attribute_create (LWES_BYTE       attrType,
                             void*           attrValue,
                             LWES_U_INT_16   arrayLen);

static void
lwes_event_attribute_destroy 
  (struct lwes_event_attribute* attr);

static int
lwes_event_add_attr
  (struct lwes_event*           event,
   LWES_CONST_SHORT_STRING      attrNameIn,
   struct lwes_event_attribute* attribute);

static int
lwes_event_add
  (struct lwes_event*       event,
   LWES_CONST_SHORT_STRING  attrNameIn,
   LWES_BYTE                attrType,
   void*                    attrValue,
   LWES_U_INT_16            arrayLen);

static int
lwes_event_set_generic
  (struct lwes_event*       event,
   LWES_CONST_SHORT_STRING  attrName,
   LWES_BYTE                attrType,
   int                      attrSize,
   void*                    attrValue);

static int
lwes_event_get_generic
  (struct lwes_event*       event,
   LWES_CONST_SHORT_STRING  attrName,
   LWES_BYTE                attrType,
   int                      attrSize,
   void*                    attrValue);

int
lwes_INT_64_from_hex_string
  (const char *buffer,
   LWES_INT_64* an_int64);

int
lwes_U_INT_64_from_hex_string
  (const char *buffer,
   LWES_U_INT_64* a_uint64);

/*************************************************************************
  PUBLIC API
 *************************************************************************/
struct lwes_event *
lwes_event_create_no_name
  (struct lwes_event_type_db *db)
{
  struct lwes_event *event =
     (struct lwes_event *)malloc (sizeof (struct lwes_event));

  if (event == NULL)
    {
      return NULL;
    }

  event->eventName            = NULL;
  event->number_of_attributes = 0;
  event->type_db              = db;
  event->attributes           = lwes_hash_create ();
  if (event->attributes == NULL)
    {
      free(event);
      return NULL;
    }

  return event;
}

/* PUBLIC : Create the memory for an event */
struct lwes_event *
lwes_event_create
  (struct lwes_event_type_db *db,
   LWES_CONST_SHORT_STRING name)
{
  struct lwes_event *event;

  if (name == NULL)
    {
      return NULL;
    }

  event = (struct lwes_event *)malloc (sizeof (struct lwes_event));

  if (event == NULL)
    {
      return NULL;
    }

  event->eventName            = NULL;
  event->number_of_attributes = 0;
  event->type_db              = db;
  event->attributes           = lwes_hash_create ();
  if (event->attributes == NULL)
    {
      free(event);
      return NULL;
    }

  if (lwes_event_set_name (event,name) < 0)
    {
      /* Having problems, bail and return NULL */
      lwes_hash_destroy (event->attributes);
      free (event);
      return NULL;
    }

  return event;
}

struct lwes_event *
lwes_event_create_with_encoding
  (struct lwes_event_type_db *db,
   LWES_CONST_SHORT_STRING name,
   LWES_INT_16 encoding)
{
  struct lwes_event *event;

  if (name == NULL)
    {
      return NULL;
    }

  event = (struct lwes_event *)malloc(sizeof(struct lwes_event));

  if (event == NULL)
    {
      return NULL;
    }

  event->eventName            = NULL;
  event->number_of_attributes = 0;
  event->type_db              = db;
  event->attributes           = lwes_hash_create ();

  if (event->attributes == NULL)
    {
      free (event);
      return NULL;
    }

  if (lwes_event_set_name(event,name) < 0)
    {
      /* Having memory problems, bail */
      lwes_hash_destroy (event->attributes);
      free (event);
      return NULL;
    }

  if (lwes_event_set_encoding (event,encoding) < 0)
    {
      /* problem setting encoding, free up memory and bail */
      lwes_hash_destroy (event->attributes);
      if (event->eventName != NULL)
        {
          free (event->eventName);
        }
      free (event);
      return NULL;
    }

  return event;
}

int
lwes_event_set_name
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name)
{
  if (event == NULL || name == NULL || event->eventName != NULL)
    {
      return -1;
    }

  event->eventName =
    (LWES_SHORT_STRING) malloc (sizeof (LWES_CHAR)*(strlen (name)+1));

  if (event->eventName == NULL)
    {
      return -3;
    }

  strcpy (event->eventName,name);

  return 0;
}

int
lwes_event_set_encoding
  (struct lwes_event *event,
   LWES_INT_16 encoding)
{
  LWES_INT_16 tmp_encoding;

  if (event == NULL)
    {
      return -1;
    }

  if ( lwes_event_get_INT_16 (event, LWES_ENCODING, &tmp_encoding) == -1 )
    {
      return lwes_event_set_INT_16 (event, LWES_ENCODING, encoding);
    }
  return -1;
}


int
lwes_event_get_name
  (struct lwes_event *event, LWES_SHORT_STRING *name)
{
  if (event == NULL || name == NULL)
    {
      return -1;
    }
  *name = event->eventName;
  return 0;
}

int
lwes_event_get_number_of_attributes
  (struct lwes_event *event,
   LWES_U_INT_16 *number)
{
  if (event == NULL || number == NULL)
    {
      return -1;
    }
  *number = event->number_of_attributes;
  return 0;
}

int
lwes_event_get_encoding
  (struct lwes_event *event,
   LWES_INT_16 *encoding)
{
  if (event == NULL || encoding == NULL)
    {
      return -1;
    }

  return lwes_event_get_INT_16 (event,
                                (LWES_SHORT_STRING)LWES_ENCODING,
                                encoding);
}

/* PUBLIC : Cleanup the memory for an event */
int
lwes_event_destroy
  (struct lwes_event *event)
{
  struct lwes_event_attribute *tmp = NULL;
  struct lwes_hash_enumeration e;

  if (event == NULL)
    {
      return 0;
    }

  /* free the parts of the event */
  if (event->eventName != NULL)
    {
      free(event->eventName);
    }

  /* clear out the hash */
  if (lwes_hash_keys (event->attributes, &e))
    {
      while (lwes_hash_enumeration_has_more_elements (&e))
        {
          LWES_SHORT_STRING tmpAttrName =
            lwes_hash_enumeration_next_element (&e);
          tmp =
            (struct lwes_event_attribute *)lwes_hash_remove (event->attributes,
                                                             tmpAttrName);
          /* free the attribute name and value*/
          if (tmpAttrName != NULL)
            {
              free(tmpAttrName);
            }
          /* free the attribute itself*/
          lwes_event_attribute_destroy(tmp);
        }
    }

  /* free the now empty hash */
  lwes_hash_destroy (event->attributes);

  /* finally free the event structure */
  free (event);

  return 0;
}



#define TYPED_EVENT_FIELD_TO_BYTES(typ, fail_ret) \
  else if (tmp->type == LWES_TYPE_##typ)          \
  {                                               \
    if (!marshall_##typ                           \
                (*((LWES_##typ *)tmp->value),     \
                bytes, num_bytes, &tmpOffset))    \
      { ret = fail_ret; }                         \
  }                                               \
  else if (tmp->type == LWES_TYPE_##typ##_ARRAY)  \
  {                                               \
    if (!marshall_array_attribute                 \
          (tmp, bytes, num_bytes, &tmpOffset))    \
      { ret = fail_ret-100; }                     \
  }                                               \
  else if (tmp->type == LWES_TYPE_N_##typ##_ARRAY)\
  {                                               \
    if (!marshall_array_attribute                 \
          (tmp, bytes, num_bytes, &tmpOffset))    \
      { ret = fail_ret-150; }                     \
  }


/* PUBLIC : serialize the event and put it into a byte array */
int
lwes_event_to_bytes
  (struct lwes_event *event,
   LWES_BYTE_P bytes,
   size_t num_bytes,
   size_t offset)
{
  struct lwes_event_attribute *tmp;
  struct lwes_event_attribute *encodingAttr;
  size_t tmpOffset = offset;
  struct lwes_hash_enumeration e;
  int ret = 0;

  if (   event == NULL
      || bytes == NULL
      || num_bytes == 0
      || offset >= num_bytes)
    {
      return -1;
    }

  /* start with the event name */
  if (marshall_SHORT_STRING (event->eventName,
                              bytes,
                              num_bytes,
                              &tmpOffset))
    {
      /* then the number of attributes */
      if (marshall_U_INT_16     (event->number_of_attributes,
                                  bytes,
                                  num_bytes,
                                  &tmpOffset))
        {
          /* handle encoding first if it is set */
          encodingAttr =
            (struct lwes_event_attribute *)
              lwes_hash_get (event->attributes,
                             (LWES_SHORT_STRING)LWES_ENCODING);

          if (encodingAttr)
            {
              void* encodingValue = encodingAttr->value;
              LWES_BYTE encodingType = encodingAttr->type;
              if (encodingValue)
                {
                  if (encodingType == LWES_TYPE_INT_16)
                    {
                      if (   0 == marshall_SHORT_STRING
                                    ((LWES_SHORT_STRING)LWES_ENCODING,
                                     bytes, num_bytes, &tmpOffset)
                          || 0 == marshall_BYTE
                                    (encodingType,
                                     bytes, num_bytes, &tmpOffset)
                          || 0 == marshall_INT_16
                                    (*((LWES_INT_16 *)encodingValue),
                                     bytes, num_bytes, &tmpOffset) ) {
                          return -2;
                        }
                    }
                  else
                    {
                      return -3;
                    }
                }
              else
                {
                  return -4;
                }
            }

          /* now iterate over all the other values in the hash */
          if (lwes_hash_keys (event->attributes, &e))
            {
              while (lwes_hash_enumeration_has_more_elements (&e) && ret == 0)
                {
                  LWES_SHORT_STRING tmpAttrName =
                    lwes_hash_enumeration_next_element (&e);

                  /* skip encoding as we've dealt with it above */
                  if (! strcmp(tmpAttrName, LWES_ENCODING))
                    {
                      continue;
                    }

                  tmp =
                    (struct lwes_event_attribute *)
                      lwes_hash_get (event->attributes, tmpAttrName);

                  if (!marshall_SHORT_STRING (tmpAttrName,
                                             bytes, num_bytes, &tmpOffset))
                    {
                      ret = -5;
                    }
                  else
                    {
                      if (!marshall_BYTE (tmp->type,
                                         bytes, num_bytes, &tmpOffset))
                        {
                          ret = -6;
                        }
                      else
                        {
                          if (tmp->type == LWES_TYPE_UNDEFINED)
                            {
                            }
                          TYPED_EVENT_FIELD_TO_BYTES(U_INT_16, -7)
                          TYPED_EVENT_FIELD_TO_BYTES(INT_16,   -8)
                          TYPED_EVENT_FIELD_TO_BYTES(U_INT_32, -9)
                          TYPED_EVENT_FIELD_TO_BYTES(INT_32,   -10)
                          TYPED_EVENT_FIELD_TO_BYTES(U_INT_64, -11)
                          TYPED_EVENT_FIELD_TO_BYTES(INT_64,   -12)
                          TYPED_EVENT_FIELD_TO_BYTES(BOOLEAN,  -13)
                          TYPED_EVENT_FIELD_TO_BYTES(IP_ADDR,  -14)
                          TYPED_EVENT_FIELD_TO_BYTES(BYTE,     -18)
                          TYPED_EVENT_FIELD_TO_BYTES(FLOAT,    -19)
                          TYPED_EVENT_FIELD_TO_BYTES(DOUBLE,   -20)
                          else if (tmp->type == LWES_TYPE_STRING)
                            {
                              if (!marshall_LONG_STRING ((LWES_LONG_STRING)tmp->value,
                                                        bytes, num_bytes, &tmpOffset)) {
                                  ret = -15;
                                }
                            }
                          else if ((tmp->type == LWES_TYPE_STRING_ARRAY) ||
                                   (tmp->type == LWES_TYPE_N_STRING_ARRAY))
                            {
                              if (!marshall_array_attribute
                                    (tmp, bytes, num_bytes, &tmpOffset))
                                { ret = -115; }
                            }
                          else
                            {
                              /* should never be reached, but if it does,
                               * there's some sort of  corruption with this
                               * attribute of the event, so skip it */
                            }
                        }
                    }
                }
            }
        }
      else
        {
          ret = -16;
        }
    }
  else
    {
      ret = -17;
    }

  return ((ret < 0) ? ret : (int)(tmpOffset-offset));
}

/* PUBLIC : add common headers to a serialized event */
int
lwes_event_add_headers
  (LWES_BYTE_P bytes,
   size_t max,
   size_t *len,
   LWES_INT_64 receipt_time,
   LWES_IP_ADDR sender_ip,
   LWES_U_INT_16 sender_port)
{
  size_t n = *len;
  size_t offset_to_num_attrs;
  size_t tmp_offset;
  LWES_U_INT_16 num_attrs;
  LWES_SHORT_STRING receipt_time_str = (LWES_SHORT_STRING)"ReceiptTime";
  size_t receipt_time_len = strlen (receipt_time_str);
  LWES_SHORT_STRING sender_ip_str = (LWES_SHORT_STRING)"SenderIP";
  size_t sender_ip_len = strlen (sender_ip_str);
  LWES_SHORT_STRING sender_port_str = (LWES_SHORT_STRING)"SenderPort";
  size_t sender_port_len = strlen (sender_port_str);

  /* The following fields will be added at the end of the event, iff they
   * are not already there
   *   int64   ReceiptTime =    1 (short string length)
   *                         + 11 (length of string)
   *                         +  1 (length of type byte)
   *                         +  8 (length of int64)
   *                       = 21
   *   ip_addr SenderIp    = 1 + 8 + 1 + 4 = 14
   *   uint16  SenderPort  = 1 + 10 + 1 + 2 = 14
   */

  /* First we check to see if these fields have already been added by *
   * calculating the offsets to the keys whose order is determined below
   */
  size_t sender_port_offset =
    sender_port_len + sizeof(LWES_BYTE) + sizeof(LWES_U_INT_16);
  size_t sender_ip_offset =
    sender_port_offset + sizeof(LWES_BYTE) /* short string length of port key */
    + sender_ip_len + sizeof(LWES_BYTE) + sizeof(LWES_IP_ADDR);
  size_t receipt_time_offset =
    sender_ip_offset + sizeof(LWES_BYTE)  /* short string length of ip key*/
    + receipt_time_len + sizeof(LWES_BYTE) + sizeof(LWES_INT_64);

  /* then comparing to the keys themselves */
  if (
      /* if the longest offset backwards puts us outside the boundaries
       * of the serialized event */
      ((int)n-(int)receipt_time_offset < 0)
      ||
      (
        /* the longest offset backwards does not match */
        memcmp ((const unsigned char *)&bytes[n-receipt_time_offset],
                  receipt_time_str,
                  receipt_time_len) != 0
        &&
        memcmp ((const unsigned char *)&bytes[n-sender_ip_offset],
                sender_ip_str,
                sender_ip_len) != 0
        &&
        memcmp ((const unsigned char *)&bytes[n-sender_port_offset],
                  sender_port_str,
                  sender_port_len) != 0
      )
    )
    {
      /* The values don't exist at the end of the event so add them */

      /* deserialize the event name to get the offset
       * for the number of attributes */
      offset_to_num_attrs = 0;
      if (unmarshall_SHORT_STRING (NULL,
                                   0,
                                   bytes,
                                   max,
                                   &offset_to_num_attrs) == 0)
        {
          return -1;
        }

      /* keep track of offset of the number of attributes since
         we will change it */
      tmp_offset = offset_to_num_attrs;
      if (unmarshall_U_INT_16     (&num_attrs,
                                   bytes,
                                   max,
                                   &tmp_offset) == 0)
        {
          return -2;
        }

      /* add receipt time to the event */
      if (   marshall_SHORT_STRING   (receipt_time_str,
                                      bytes,
                                      max,
                                      &n) == 0
          || marshall_BYTE           (LWES_TYPE_INT_64,
                                      bytes,
                                      max,
                                      &n) == 0
          || marshall_INT_64         (receipt_time,
                                      bytes,
                                      max,
                                      &n) == 0)
        {
          return -3;
        }
      ++num_attrs;

      /* add sender ip to the event */
      if (   marshall_SHORT_STRING   (sender_ip_str,
                                      bytes,
                                      max,
                                      &n) == 0
          || marshall_BYTE           (LWES_TYPE_IP_ADDR,
                                      bytes,
                                      max,
                                      &n) == 0
          ||  marshall_IP_ADDR       (sender_ip,
                                      bytes,
                                      max,
                                      &n) == 0)
        {
          return -4;
        }
      ++num_attrs;

      /* add sender port to the event */
      if (    marshall_SHORT_STRING   (sender_port_str,
                                       bytes,
                                       max,
                                       &n) == 0
           || marshall_BYTE           (LWES_TYPE_U_INT_16,
                                       bytes,
                                       max,
                                       &n) == 0
           || marshall_U_INT_16       (sender_port,
                                       bytes,
                                       max,
                                       &n) == 0)
        {
          return -5;
        }
      ++num_attrs;

      /* finally put the new number of attributes into the appropriate place */
      if (marshall_U_INT_16      (num_attrs,
                                  bytes,
                                  max,
                                  &offset_to_num_attrs) == 0)
        {
          return -6;
        }

      *len = n;
    }
  return 0;
}

#define TYPED_BYTES_TO_EVENT_FIELD(typ,set_fail_ret,mar_fail_ret)    \
  else if (tmp_byte == LWES_TYPE_##typ)                              \
    {                                                                \
      LWES_##typ var;                                                \
      if (!unmarshall_##typ                                          \
            (&var, bytes, num_bytes, &tmpOffset))                    \
        { return mar_fail_ret; }                                     \
      if (0 > lwes_event_set_##typ                                   \
            (event, tmp_short_str, var))                             \
        { return set_fail_ret; }                                     \
    }                                                                \
  else if (tmp_byte == LWES_TYPE_##typ##_ARRAY)                      \
    {                                                                \
      struct lwes_event_attribute* attr;                             \
      attr = lwes_event_attribute_create(tmp_byte, NULL, 0);         \
      if (!unmarshall_array_attribute                                \
            (attr, bytes, num_bytes, &tmpOffset))                    \
        {                                                            \
          lwes_event_attribute_destroy(attr);                        \
          return mar_fail_ret-100;                                   \
        }                                                            \
      if (0 > lwes_event_add_attr                                    \
            (event, tmp_short_str, attr))                            \
        {                                                            \
          free(attr->value);                                         \
          free(attr);                                                \
          return set_fail_ret;                                       \
        }                                                            \
    }                                                                \
  else if (tmp_byte == LWES_TYPE_N_##typ##_ARRAY)                    \
    {                                                                \
      struct lwes_event_attribute* attr;                             \
      attr = lwes_event_attribute_create(tmp_byte, NULL, 0);         \
      if (!unmarshall_array_attribute                                \
            (attr, bytes, num_bytes, &tmpOffset))                    \
        {                                                            \
          lwes_event_attribute_destroy(attr);                        \
          return mar_fail_ret-150;                                   \
        }                                                            \
      if (0 > lwes_event_add_attr                                    \
            (event, tmp_short_str, attr))                            \
        {                                                            \
          free(attr->value);                                         \
          free(attr);                                                \
          return set_fail_ret;                                       \
        }                                                            \
    }

/* PUBLIC : deserialize the event from a byte array and into an event */
int
lwes_event_from_bytes_lax
  (struct lwes_event *event,
   LWES_U_INT_16 *expected,
   LWES_BYTE_P bytes,
   size_t num_bytes,
   size_t offset,
   struct lwes_event_deserialize_tmp *dtmp)
{
  size_t tmpOffset = offset;

  LWES_BYTE         tmp_byte;
  LWES_U_INT_16     tmp_uint16;
  LWES_SHORT_STRING tmp_short_str;
  LWES_LONG_STRING  tmp_long_str;

  if (   event == NULL
      || bytes == NULL
      || num_bytes == 0
      || offset >= num_bytes
      || dtmp == NULL)
    {
      return -1;
    }

  tmp_short_str = dtmp->tmp_string;
  tmp_long_str  = dtmp->tmp_string_long;

  /* unmarshall the event name */
  if (!unmarshall_SHORT_STRING 
        (tmp_short_str, (SHORT_STRING_MAX+1),
         bytes, num_bytes, &tmpOffset))
     {
       return -25;
     }
  /* copies the data out of tmp_short_str */
  if (lwes_event_set_name (event, tmp_short_str) != 0)
    {
      return -24;
    }
  /* unmarshall the number of elements */
  if (!unmarshall_U_INT_16 (&tmp_uint16,bytes,num_bytes,&tmpOffset))
    {
      return -23;
    }
  if (expected)
    { *expected = tmp_uint16; }

  /* this used to be based on number of attributes, but that
   * is technically less flexible and unnecessary, so switching
   * to pure byte offset based finish */
  while (tmpOffset != num_bytes)
    {
      /* unmarshall the attribute name */
      if (unmarshall_SHORT_STRING (tmp_short_str,
                                    (SHORT_STRING_MAX+1),
                                    bytes,
                                    num_bytes,
                                    &tmpOffset))
        {
          /* unmarshall the type id */
          if (unmarshall_BYTE         (&tmp_byte,
                                        bytes,
                                        num_bytes,
                                        &tmpOffset))
            {
              if (tmp_byte == LWES_TYPE_UNDEFINED)
                {
                }
              TYPED_BYTES_TO_EVENT_FIELD(U_INT_16, -2,  -3)
              TYPED_BYTES_TO_EVENT_FIELD(INT_16,   -4,  -5)
              TYPED_BYTES_TO_EVENT_FIELD(U_INT_32, -6,  -7)
              TYPED_BYTES_TO_EVENT_FIELD(INT_32,   -8,  -9)
              TYPED_BYTES_TO_EVENT_FIELD(U_INT_64, -10, -11)
              TYPED_BYTES_TO_EVENT_FIELD(INT_64,   -12, -13)
              TYPED_BYTES_TO_EVENT_FIELD(BOOLEAN,  -14, -15)
              TYPED_BYTES_TO_EVENT_FIELD(IP_ADDR,  -16, -17)
              TYPED_BYTES_TO_EVENT_FIELD(BYTE,     -26, -27)
              TYPED_BYTES_TO_EVENT_FIELD(FLOAT,    -28, -29)
              TYPED_BYTES_TO_EVENT_FIELD(DOUBLE,   -30, -31)
              else if (tmp_byte == LWES_TYPE_STRING)
                {
                  if (unmarshall_LONG_STRING 
                        (tmp_long_str, (LONG_STRING_MAX+1), 
                         bytes, num_bytes, &tmpOffset))
                    {
                      if (0 > lwes_event_set_STRING 
                          (event, tmp_short_str, tmp_long_str))
                        {
                          return -18;
                        }
                    }
                  else
                    {
                      return -19;
                    }
                }
              else if ((tmp_byte == LWES_TYPE_STRING_ARRAY) ||
                       (tmp_byte == LWES_TYPE_N_STRING_ARRAY))
                {
                  struct lwes_event_attribute* attr;
                  attr = lwes_event_attribute_create(tmp_byte, NULL, 0);
                  if (!unmarshall_array_attribute
                        (attr, bytes, num_bytes, &tmpOffset))
                    {
                      lwes_event_attribute_destroy(attr);
                      return -119;
                    }
                  if (0 > lwes_event_add_attr
                        (event, tmp_short_str, attr))
                    {
                      lwes_event_attribute_destroy(attr);
                      return -118;
                    }
                }
              else
                {
                  return -20;
                }
            }
          else
            {
              return -21;
            }
        }
      else
        {
          return -22;
        }
    }

  return (int)(tmpOffset-offset);
}

/* PUBLIC : deserialize the event from a byte array and into an event, *strict* */
int
lwes_event_from_bytes
  (struct lwes_event *event,
   LWES_BYTE_P bytes,
   size_t num_bytes,
   size_t offset,
   struct lwes_event_deserialize_tmp *dtmp)
{
  LWES_U_INT_16 expected=0;
  int ret = lwes_event_from_bytes_lax(event, &expected, bytes, num_bytes, offset, dtmp);
  if (ret < 0)
    { return ret; }

  /* strict attribute count enforcement */
  if (expected != event->number_of_attributes)
    { 
      return -50; 
    }

  return ret;
}

/* ACCESSOR METHODS */


static int
lwes_event_set_generic
  (struct lwes_event*       event,
   LWES_CONST_SHORT_STRING  attrName,
   LWES_BYTE                attrType,
   int                      attrSize,
   void*                    attrValue)
{
  int ret = 0;
  char *attrCopy;

  if (event == NULL || attrName == NULL || attrValue == NULL)
    {
      return -1;
    }

  attrCopy = (char *)malloc (attrSize);

  if (attrCopy == NULL)
    {
      return -3;
    }
  memcpy(attrCopy, attrValue, attrSize);

  ret = lwes_event_add (event, attrName, attrType, attrCopy, 0);
  if (ret < 0)
    {
      free (attrCopy);
    }
  return ret;
}

static int
lwes_event_get_generic
  (struct lwes_event*       event,
   LWES_CONST_SHORT_STRING  attrName,
   LWES_BYTE                attrType,
   int                      attrSize,
   void*                    attrValue)
{
  struct lwes_event_attribute *tmp;
  if (event == NULL || attrName == NULL || attrValue == NULL)
    {
      return -1;
    }
  tmp = (struct lwes_event_attribute *)lwes_hash_get (event->attributes, attrName);
  if (tmp != NULL && tmp->type == attrType)
    {
      memcpy(attrValue, tmp->value, attrSize);
      return 0;
    }
  return -1;
}

int
lwes_event_set_array
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING attrName,
   LWES_TYPE type,
   LWES_U_INT_16 arr_length,
   void *arr)
{
  int i, ret = 0;
  char *attrCopy;

  if (event == NULL || attrName == NULL || arr == NULL)
    {
      return -1;
    }
  if (!lwes_type_is_array(type))
    {
      return -1;
    }

  LWES_TYPE baseType = lwes_array_type_to_base(type);
  int baseSize = arr_length * lwes_type_to_size(type);
  int attrSize = baseSize;
  if (LWES_TYPE_STRING == baseType)
    {
      /* Sum up the size of all the strings, to allocate in one block */
      LWES_CONST_SHORT_STRING *str = (LWES_CONST_SHORT_STRING*)arr;
      for (i=0; i< arr_length; ++i)
        {
          attrSize += 1 + (str[i] ? strlen(str[i]) : 0);
        }
    }

  attrCopy = (char *)malloc (attrSize);
  if (attrCopy == NULL)
    {
      return -3;
    }

  if (LWES_TYPE_STRING == baseType)
    {
      LWES_CONST_SHORT_STRING *str = (LWES_CONST_SHORT_STRING*)arr;
      LWES_SHORT_STRING *strOut    = ((LWES_SHORT_STRING*)(void*)attrCopy);
      char * cur = ((char*)attrCopy) + baseSize;
      for (i=0; i< arr_length; ++i)
        {
          if (str[i])
            {
              int sz = 1 + strlen(str[i]);
              memcpy(cur, str[i], sz);
              strOut[i]=cur;
              cur += sz;
            }
        }
    }
  else
    {
      memcpy(attrCopy, arr, attrSize);
    }

  ret = lwes_event_add (event, attrName, type, attrCopy, arr_length);
  if (ret < 0)
    {
      free (attrCopy);
    }
  return ret;
}

/* NOTE: the returned array should be treated as const, */
/* as it references the internal event data. */
int
lwes_event_get_array
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_TYPE type,
   LWES_U_INT_16 *arr_length,
   void **arr)
{
  struct lwes_event_attribute *tmp;

  if (event == NULL || name == NULL || arr == NULL)
    {
      return -1;
    }
  if (!lwes_type_is_array(type))
    {
      return -1;
    }

  tmp = (struct lwes_event_attribute *)lwes_hash_get (event->attributes, name);

  if (tmp != NULL && tmp->type == type)
    {
      *arr = (char*)tmp->value;
      *arr_length = tmp->array_len;
      return 0;
    }
  return -1;
}

int
lwes_event_set_nullable_array
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_TYPE type,
   LWES_U_INT_16 arr_length,
   void *arr)
{
  int i, ret = 0;
  LWES_BYTE* tmp = NULL;
  LWES_BYTE* dataOut = NULL;
  LWES_BYTE** pointersIn = arr;
  LWES_BYTE** pointers = NULL;
  LWES_TYPE baseType = lwes_array_type_to_base(type);
  int delta = lwes_type_to_size(baseType);
  int baseSize = arr_length * sizeof(char*);
  int attrSize = baseSize;

  if (event == NULL || name == NULL || arr == NULL)
    {
      return -1;
    }
  if (!lwes_type_is_nullable_array(type))
    {
      return -1;
    }

  /* Sum up the size of all non-null elements, to allocate in one block */
  if (LWES_TYPE_STRING == baseType)
    {
      for (i=0; i< arr_length; ++i)
        {
          attrSize += (pointersIn[i] ? strlen((char*)pointersIn[i])+1 : 0);
        }
    }
  else
    {
      for (i=0; i<arr_length; ++i)
        {
          attrSize += (pointersIn[i] ? delta : 0);
        }
    }

  tmp = (LWES_BYTE*)malloc (attrSize);
  if (tmp == NULL)
    {
      return -3;
    }

  pointers = (LWES_BYTE**)(void*)tmp;
  dataOut = tmp + baseSize;

  for (i=0; i< arr_length; ++i)
    {
      if (pointersIn[i])
        {
          int sz = delta;
          if (LWES_TYPE_STRING == baseType)
            {
              sz = 1 + strlen((char*)pointersIn[i]);
            }
          memcpy(dataOut, pointersIn[i], sz);
          pointers[i]=dataOut;
          dataOut += sz;
        }
      else
        {
          pointers[i]=NULL;
        }
    }

  ret = lwes_event_add (event, name, type, tmp, arr_length);
  if (ret < 0)
    {
      free (tmp);
    }
  return ret;
}

int
lwes_event_get_nullable_array
  (struct lwes_event *event,
   LWES_CONST_SHORT_STRING name,
   LWES_TYPE type,
   LWES_U_INT_16 *arr_length,
   void **arr)
{
  struct lwes_event_attribute *tmp;

  if (event == NULL || name == NULL || arr == NULL)
    {
      return -1;
    }
  if (!lwes_type_is_nullable_array(type))
    {
      return -1;
    }

  tmp = (struct lwes_event_attribute *)lwes_hash_get (event->attributes, name);

  if (tmp != NULL && tmp->type == type)
    {
      *arr = (char*)tmp->value;
      *arr_length = tmp->array_len;
      return 0;
    }
  return -1;
}


#define MAKE_ACCESSORS(typ)                             \
int lwes_event_set_##typ                                \
   (struct lwes_event      *event,                      \
    LWES_CONST_SHORT_STRING attrName,                   \
    LWES_##typ              value)                      \
{                                                       \
  return lwes_event_set_generic(event, attrName,        \
                                LWES_TYPE_##typ,        \
                                sizeof(value), &value); \
}                                                       \
                                                        \
int lwes_event_get_##typ                                \
   (struct lwes_event       *event,                     \
    LWES_CONST_SHORT_STRING  name,                      \
    LWES_##typ              *value)                     \
{                                                       \
  return lwes_event_get_generic(event, name,            \
                                LWES_TYPE_##typ,        \
                                sizeof(*value), value); \
}                                                       \
                                                        \
int lwes_event_set_##typ##_ARRAY                        \
  (struct lwes_event       *event,                      \
   LWES_CONST_SHORT_STRING  name,                       \
   LWES_U_INT_16  length,                               \
   LWES_##typ  *value)                                  \
{                                                       \
  return lwes_event_set_array(event, name,              \
      LWES_TYPE_##typ##_ARRAY, length, (void*)value);   \
}                                                       \
                                                        \
int lwes_event_get_##typ##_ARRAY                        \
  (struct lwes_event       *event,                      \
   LWES_CONST_SHORT_STRING  name,                       \
   LWES_U_INT_16 *length,                               \
   LWES_##typ **value)                                  \
{                                                       \
  return lwes_event_get_array(event, name,              \
      LWES_TYPE_##typ##_ARRAY, length, (void*)value);   \
}                                                       \
                                                        \
int lwes_event_set_N_##typ##_ARRAY                      \
  (struct lwes_event       *event,                      \
   LWES_CONST_SHORT_STRING  name,                       \
   LWES_U_INT_16  length,                               \
   LWES_##typ  **value)                                 \
{                                                       \
  return lwes_event_set_nullable_array(event, name,     \
      LWES_TYPE_N_##typ##_ARRAY, length, (void*)value); \
}                                                       \
                                                        \
int lwes_event_get_N_##typ##_ARRAY                      \
  (struct lwes_event       *event,                      \
   LWES_CONST_SHORT_STRING  name,                       \
   LWES_U_INT_16 *length,                               \
   LWES_##typ* **value)                                 \
{                                                       \
  return lwes_event_get_nullable_array(event, name,     \
      LWES_TYPE_N_##typ##_ARRAY, length, (void*)value); \
}


MAKE_ACCESSORS(U_INT_16)
MAKE_ACCESSORS(INT_16)
MAKE_ACCESSORS(U_INT_32)
MAKE_ACCESSORS(INT_32)
MAKE_ACCESSORS(U_INT_64)
MAKE_ACCESSORS(INT_64)
MAKE_ACCESSORS(BOOLEAN)
MAKE_ACCESSORS(IP_ADDR)
MAKE_ACCESSORS(BYTE)
MAKE_ACCESSORS(FLOAT)
MAKE_ACCESSORS(DOUBLE)




int lwes_event_set_STRING_ARRAY
  (struct lwes_event       *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_U_INT_16  length,
   LWES_STRING  *value)
{
  return lwes_event_set_array(event, name,
      LWES_TYPE_STRING_ARRAY, length, (void*)value);
}

int lwes_event_get_STRING_ARRAY
  (struct lwes_event       *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_U_INT_16 *length,
   LWES_STRING **value)
{
  return lwes_event_get_array(event, name,
      LWES_TYPE_STRING_ARRAY, length, (void*)value);
}

int lwes_event_set_N_STRING_ARRAY
  (struct lwes_event       *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_U_INT_16  length,
   LWES_STRING  *value)
{
  return lwes_event_set_nullable_array(event, name,
      LWES_TYPE_N_STRING_ARRAY, length, (void*)value);
}

int lwes_event_get_N_STRING_ARRAY
  (struct lwes_event       *event,
   LWES_CONST_SHORT_STRING  name,
   LWES_U_INT_16 *length,
   LWES_STRING **value)
{
  return lwes_event_get_nullable_array(event, name,
      LWES_TYPE_N_STRING_ARRAY, length, (void*)value);
}



int lwes_event_set_U_INT_64_w_string (struct lwes_event *     event,
                                      LWES_CONST_SHORT_STRING attrName,
                                      LWES_CONST_SHORT_STRING uint64_string)
{
  LWES_INT_64 u_int_64;
  if (event == NULL || attrName == NULL || uint64_string == NULL)
    {
      return -1;
    }
  if (lwes_INT_64_from_hex_string (uint64_string, &u_int_64) < 0)
    {
      return -2;
    }
  return lwes_event_set_U_INT_64 (event, attrName, u_int_64);
}

int lwes_event_set_INT_64_w_string (struct lwes_event       *event,
                                    LWES_CONST_SHORT_STRING  attrName,
                                    LWES_CONST_SHORT_STRING  int64_string)
{
  LWES_INT_64 int_64;

  if (event == NULL || attrName == NULL || int64_string == NULL)
    {
      return -1;
    }

  if (lwes_INT_64_from_hex_string (int64_string,&int_64) < 0)
    {
      return -2;
    }
  return lwes_event_set_INT_64 (event, attrName, int_64);
}

int lwes_event_set_STRING (struct lwes_event *       event,
                           LWES_CONST_SHORT_STRING   attrName,
                           LWES_CONST_LONG_STRING    value)
{
  int size = (NULL == value) ? 0 : sizeof (LWES_CHAR)*(strlen (value)+1);
  return lwes_event_set_generic(event, attrName, LWES_TYPE_STRING, size, (char*)value);
}

int lwes_event_get_STRING (struct lwes_event       *event,
                           LWES_CONST_SHORT_STRING  name,
                           LWES_LONG_STRING        *value)
{
  struct lwes_event_attribute *tmp;

  if (event == NULL || name == NULL || value == NULL)
    {
      return -1;
    }

  tmp = (struct lwes_event_attribute *)lwes_hash_get (event->attributes, name);

  if (tmp != NULL && tmp->type == LWES_TYPE_STRING)
    {
      *value = (((LWES_LONG_STRING)tmp->value));
      return 0;
    }
  return -1;
}


int lwes_event_set_IP_ADDR_w_string (struct lwes_event *       event,
                                     LWES_CONST_SHORT_STRING   attrName,
                                     LWES_CONST_SHORT_STRING  value)
{
  int ret = 0;
  struct in_addr *attrValue;

  if (event == NULL || attrName == NULL || value == NULL)
    {
      return -1;
    }

  attrValue = (LWES_IP_ADDR *)malloc (sizeof (LWES_IP_ADDR));
  if (attrValue == NULL)
    {
      return -3;
    }
  attrValue->s_addr = inet_addr (value);

  ret = lwes_event_add (event, attrName, LWES_TYPE_IP_ADDR, attrValue, 0);
  if (ret < 0)
    {
      free (attrValue);
    }
  return ret;
}


/*************************************************************************
  PRIVATE API
 *************************************************************************/
/* Create the memory for an attribute */
static struct lwes_event_attribute *
lwes_event_attribute_create (LWES_BYTE       attrType,
                             void*           attrValue,
                             LWES_U_INT_16   arrayLen)
{
  struct lwes_event_attribute *attribute =
    (struct lwes_event_attribute *)
      malloc (sizeof (struct lwes_event_attribute));

  if (attribute == NULL)
    {
      return NULL;
    }

  attribute->type  = attrType;
  attribute->value = attrValue;
  attribute->array_len = arrayLen;

  return attribute;
}

static void
lwes_event_attribute_destroy 
  (struct lwes_event_attribute* attr)
{
  if (attr->value)
    { 
      free(attr->value);
      attr->value = NULL;
    }
  free(attr);
}


static int
lwes_event_add_attr
  (struct lwes_event*           event,
   LWES_CONST_SHORT_STRING      attrNameIn,
   struct lwes_event_attribute* attribute)
{
  struct lwes_event_attribute* attribute_out = NULL;
  LWES_SHORT_STRING attrName  = NULL;
  void* ret = NULL;

  /* check against the event db */
  if (event->type_db != NULL
       && lwes_event_type_db_check_for_attribute (event->type_db,
                                                  attrNameIn,
                                                  event->eventName) == 0)
    {
      return -1;
    }
  if (event->type_db != NULL
       && lwes_event_type_db_check_for_type (event->type_db,
                                             attribute->type,
                                             attrNameIn,
                                             event->eventName) == 0)
    {
      return -2;
    }

  /* copy the attribute name */
  attrName =
      (LWES_SHORT_STRING) malloc( sizeof(LWES_CHAR)*(strlen (attrNameIn)+1));
  if (attrName == NULL)
    {
      return -3;
    }
  attrName[0] = '\0';
  strcat (attrName,attrNameIn);

  /* Try and put something into the hash */
  ret = lwes_hash_put (event->attributes, attrName, attribute);

  /* if put returns the given attribute there was a failure of some sort, so free
   * memory and return -4
   */
  if (ret == attribute)
    {
      free (attrName);
      return -4;
    }
  else if (ret != NULL)
    {
      attribute_out = ret;
      /* in this case we replaced the old value and it returned it, so free up the
       * old value and the key (since we reused the old key)
       */
      if (attribute_out->value != NULL)
        {
          free (attribute_out->value);
        }
      free (attribute_out);
      free (attrName);
    }
  else
    {
      /* we successfully added a new attribute, so increment the number of attributes */
      event->number_of_attributes++;
    }

  return 0;
}

/* add an attribute to an event */
static int
lwes_event_add (struct lwes_event*       event,
                LWES_CONST_SHORT_STRING  attrNameIn,
                LWES_BYTE                attrType,
                void*                    attrValue,
                LWES_U_INT_16            arrayLen)
{
  struct lwes_event_attribute* attribute = NULL;
  int ret                                = 0;

  /* create the attribute */
  attribute = lwes_event_attribute_create (attrType, attrValue, arrayLen);
  if (attribute == NULL)
    {
      return -3;
    }

  ret = lwes_event_add_attr (event, attrNameIn, attribute);

  if (ret)
    {
      free (attribute);
      return ret;
    }

  return event->number_of_attributes;
}

int
lwes_U_INT_64_from_hex_string
  (const char *buffer,
   LWES_U_INT_64* a_uint64)
{
  char *endptr;

  /* manpage for strtoull suggests setting errno to zero then checking
   * for error afterwards
   */
  errno = 0;
  *a_uint64 = strtoull (buffer, &endptr, 16);

  /* it's considered an error if it overflows (ie, errno is ERANGE)
   * or if the entire string is not used (ie, in_length is greater than
   * out_length)
   */
  if (errno == ERANGE
      || ((int)strlen (buffer) > (int)(endptr - buffer)))
    {
      return -1;
    }

  return 0;
}

int
lwes_INT_64_from_hex_string
  (const char *buffer,
   LWES_INT_64* an_int64)
{
  /*
   * totally bogus, but on freebsd strtoll for hex can not reach the
   * maximum hex value of 0xffffffffffffffff, so instead I use
   * strtoull which does reach the upper range, and then cast
   */
  LWES_U_INT_64 a_uint64;
  int ret;

  ret = lwes_U_INT_64_from_hex_string (buffer, &a_uint64);

  if (ret < 0)
    {
      return ret;
    }

  *an_int64 = (LWES_INT_64)a_uint64;

  return 0;
}

int
lwes_event_keys
  (struct lwes_event * event,
   struct lwes_event_enumeration *enumeration)
{
  return lwes_hash_keys (event->attributes, &(enumeration->hash_enum));
}

int
lwes_event_enumeration_next_element
  (struct lwes_event_enumeration *enumeration,
   LWES_CONST_SHORT_STRING *key,
   LWES_TYPE *type)
{
  LWES_SHORT_STRING tmpAttrName =
     lwes_hash_enumeration_next_element (&(enumeration->hash_enum));
  (*key) = NULL;
  (*type) = LWES_TYPE_UNDEFINED;

  if ( tmpAttrName != NULL)
    {
      struct lwes_event_attribute *tmp =
         (struct lwes_event_attribute *)
           lwes_hash_get (enumeration->hash_enum.enum_hash, (tmpAttrName));
      if (tmp != NULL)
        {
          (*type) = (LWES_TYPE) (tmp->type);
          (*key) = tmpAttrName;
          return 1;
        }
    }
  return 0;
}


