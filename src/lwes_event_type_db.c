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

#include "lwes_event_type_db.h"
#include "lwes_esf_parser.h"
#include "lwes_hash.h"

const LWES_U_INT_32 ATTRIBUTE_OPTIONAL =     0 ;
const LWES_U_INT_32 ATTRIBUTE_REQUIRED = (1<<0);
const LWES_U_INT_32 ATTRIBUTE_ARRAYED  = (1<<1);
const LWES_U_INT_32 ATTRIBUTE_NULLABLE = (1<<2);


struct lwes_event_type_db *
lwes_event_type_db_create
  (const char *filename)
{
  struct lwes_event_type_db *db =
     (struct lwes_event_type_db *)
       malloc (sizeof (struct lwes_event_type_db));
  if (db != NULL)
    {
      db->esf_filename[0] = '\0';
      strncat (db->esf_filename, filename, FILENAME_MAX - 1);

      db->events = lwes_hash_create ();
      if (db->events != NULL)
        {
          if (lwes_parse_esf (db, db->esf_filename) != 0)
            {
              lwes_event_type_db_destroy(db);
              db = NULL;
            }
        }
      else
        {
          free (db);
          db = NULL;
        }
    }

  return db;
}


int
lwes_event_type_db_destroy
  (struct lwes_event_type_db *db)
{
  struct lwes_hash_enumeration e;
  struct lwes_hash_enumeration e2;
  LWES_SHORT_STRING eventName = NULL;
  struct lwes_hash *attrHash  = NULL;
  LWES_SHORT_STRING attrName  = NULL;
  LWES_BYTE        *attrType  = NULL;
  if (db != NULL)
    {
      /* clear out the hash */
      if (lwes_hash_keys (db->events, &e))
        {
          while (lwes_hash_enumeration_has_more_elements (&e))
            {
              eventName = lwes_hash_enumeration_next_element (&e);
              attrHash =
                (struct lwes_hash *)lwes_hash_remove (db->events, eventName);
              if (lwes_hash_keys (attrHash, &e2))
                {
                  while (lwes_hash_enumeration_has_more_elements (&e2))
                    {
                      attrName = lwes_hash_enumeration_next_element (&e2);
                      attrType =
                        (LWES_BYTE *)lwes_hash_remove (attrHash, attrName);

                      /* free the type and the name */
                      if (attrName != NULL)
                        free (attrName);
                      if (attrType != NULL)
                        free (attrType);
                    }
                }
              lwes_hash_destroy (attrHash);

              if (eventName != NULL)
                free (eventName);
            }
        }
      lwes_hash_destroy (db->events);
      free (db);
    }

  lwes_parse_esf_destroy ();

  return 0;
}

int
lwes_event_type_db_add_event
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name)
{
  void *ret = NULL;
  struct lwes_hash *eventHash = NULL;
  /* try and allocate the key */
  LWES_SHORT_STRING eventHashKey =
    (LWES_SHORT_STRING)malloc (sizeof (LWES_CHAR)*(strlen (event_name)+1));
  if (eventHashKey == NULL)
    {
      return -3;
    }
  strcpy (eventHashKey, event_name);
  /* try and allocate the value */
  eventHash = lwes_hash_create ();
  if (eventHash == NULL)
    {
      free (eventHashKey);
      return -3;
    }

  /* try and place the key and value into the hash, if we fail free both */
  ret = lwes_hash_put (db->events, eventHashKey, eventHash);
  if ( ret != NULL )
    {
      free (eventHashKey);
      lwes_hash_destroy (eventHash);
      return -4;
    }

  return 0;
}

int
lwes_event_type_db_add_attribute_ex
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name,
   LWES_SHORT_STRING attr_name,
   LWES_SHORT_STRING type,
   LWES_INT_32 flags,
   LWES_INT_16 arr_size,
   LWES_INT_16 max_str_size)
{
  void *ret = NULL;
  struct lwes_hash *eventHash =
    (struct lwes_hash *)lwes_hash_get (db->events, event_name);
  LWES_SHORT_STRING tmpAttrName = NULL;
  struct lwes_event_field_db_attribute *tmpAttrRec = NULL;

  tmpAttrName =
      (LWES_SHORT_STRING)malloc (sizeof (LWES_CHAR)*(strlen (attr_name)+1));
  if (tmpAttrName == NULL)
    {
      return -3;
    }
  strcpy (tmpAttrName, attr_name);

  tmpAttrRec = (struct lwes_event_field_db_attribute*)
    malloc(sizeof(struct lwes_event_field_db_attribute));
  if (tmpAttrRec == NULL)
    {
      free (tmpAttrName);
      return -3;
    }


  tmpAttrRec->attr_flags = flags;
  tmpAttrRec->array_size = arr_size;
  tmpAttrRec->max_str_size = max_str_size;
  tmpAttrRec->type = lwes_string_to_type(type);
  if ((flags & ATTRIBUTE_NULLABLE) &&  tmpAttrRec->type<LWES_TYPE_U_INT_16_ARRAY)
    {
      /* turn it into a nullable array type */
      tmpAttrRec->type += (LWES_TYPE_N_U_INT_16_ARRAY-LWES_TYPE_U_INT_16);
    }
  else if (arr_size != 0 && tmpAttrRec->type<LWES_TYPE_U_INT_16_ARRAY)
    {
      /* turn it into an array type */
      tmpAttrRec->type += (LWES_TYPE_U_INT_16_ARRAY-LWES_TYPE_U_INT_16);
    }

  ret = lwes_hash_put (eventHash, tmpAttrName, tmpAttrRec);

  /* if inserting into the hash fails we should free up our memory */
  if (ret != NULL)
  {
    free (tmpAttrName);
    free (tmpAttrRec);
    return -4;
  }

  return 0;
}

int
lwes_event_type_db_add_attribute
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name,
   LWES_SHORT_STRING attr_name,
   LWES_SHORT_STRING type)
{
  return lwes_event_type_db_add_attribute_ex (db, event_name, attr_name, type, 0, 0, 0);
}

int
lwes_event_type_db_check_for_event
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name)
{
  return lwes_hash_contains_key (db->events, event_name);
}

const struct lwes_event_field_db_attribute*
lwes_event_type_db_lookup_attr
  (struct lwes_event_type_db *db,
   LWES_CONST_SHORT_STRING attr_name,
   LWES_CONST_SHORT_STRING event_name)
{
  struct lwes_hash *event =
    (struct lwes_hash *)lwes_hash_get (db->events, event_name);
  struct lwes_hash *meta_event =
    (struct lwes_hash *)lwes_hash_get (db->events, LWES_META_INFO_STRING);
  struct lwes_event_field_db_attribute *tmp_rec = NULL;

  if (event != NULL)
  {
    tmp_rec = (struct lwes_event_field_db_attribute *)
      lwes_hash_get (event, attr_name);
  }
  if (tmp_rec == NULL && meta_event != NULL)
  {
    tmp_rec = (struct lwes_event_field_db_attribute *)
      lwes_hash_get (meta_event, attr_name);
  }
  return tmp_rec;
}

int
lwes_event_type_db_check_for_attribute
  (struct lwes_event_type_db *db,
   LWES_CONST_SHORT_STRING attr_name,
   LWES_CONST_SHORT_STRING event_name)
{
  const struct lwes_event_field_db_attribute* attrRec =
    lwes_event_type_db_lookup_attr (db, attr_name, event_name);

  return (NULL != attrRec);
}

int
lwes_event_type_db_check_for_type
  (struct lwes_event_type_db *db,
   LWES_BYTE type_value,
   LWES_CONST_SHORT_STRING attr_name,
   LWES_CONST_SHORT_STRING event_name)
{
  const struct lwes_event_field_db_attribute* attrRec =
    lwes_event_type_db_lookup_attr (db, attr_name, event_name);

  return ((NULL != attrRec) && (attrRec->type == type_value));
}
