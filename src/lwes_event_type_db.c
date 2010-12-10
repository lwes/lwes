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
      strcat (db->esf_filename, filename);

      db->events = lwes_hash_create ();
      if (db->events != NULL)
        {
          if (lwes_parse_esf (db, db->esf_filename) != 0)
            {
              free (db);
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
                  attrType = (LWES_BYTE *)lwes_hash_remove (attrHash, attrName);

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
  if (db != NULL)
    free (db);

  lwes_parse_esf_destroy ();

  return 0;
}

int
lwes_event_type_db_add_event
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name)
{
  int ret = 0;
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
  if ( ret < 0 )
    {
      free (eventHashKey);
      lwes_hash_destroy (eventHash);
    }

  return ret;
}

int
lwes_event_type_db_add_attribute
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name,
   LWES_SHORT_STRING attr_name,
   LWES_SHORT_STRING type)
{
  int ret;
  struct lwes_hash *eventHash =
    (struct lwes_hash *)lwes_hash_get (db->events, event_name);
  LWES_SHORT_STRING tmpAttrName = NULL;
  LWES_BYTE        *tmpAttrType = NULL;

  tmpAttrName =
      (LWES_SHORT_STRING)malloc (sizeof (LWES_CHAR)*(strlen (attr_name)+1));
  if (tmpAttrName == NULL)
    {
      return -3;
    }
  strcpy (tmpAttrName, attr_name);

  tmpAttrType = (LWES_BYTE *)malloc (sizeof (LWES_BYTE));
  if (tmpAttrType == NULL)
    {
      free (tmpAttrName);
      return -3;
    }

  if (strcmp (type, LWES_U_INT_16_STRING) == 0)
    {
      *tmpAttrType = LWES_U_INT_16_TOKEN;
    }
  else if (strcmp (type, LWES_INT_16_STRING) == 0)
    {
      *tmpAttrType = LWES_INT_16_TOKEN;
    }
  else if (strcmp (type, LWES_U_INT_32_STRING) == 0)
    {
      *tmpAttrType = LWES_U_INT_32_TOKEN;
    }
  else if (strcmp (type, LWES_INT_32_STRING) == 0)
    {
      *tmpAttrType = LWES_INT_32_TOKEN;
    }
  else if (strcmp (type, LWES_U_INT_64_STRING) == 0)
    {
      *tmpAttrType = LWES_U_INT_64_TOKEN;
    }
  else if (strcmp (type, LWES_INT_64_STRING) == 0)
    {
      *tmpAttrType = LWES_INT_64_TOKEN;
    }
  else if (strcmp (type, LWES_BOOLEAN_STRING) == 0)
    {
      *tmpAttrType = LWES_BOOLEAN_TOKEN;
    }
  else if (strcmp (type, LWES_IP_ADDR_STRING) == 0)
    {
      *tmpAttrType = LWES_IP_ADDR_TOKEN;
    }
  else if (strcmp (type, LWES_STRING_STRING) == 0)
    {
      *tmpAttrType = LWES_STRING_TOKEN;
    }

  ret = lwes_hash_put (eventHash, tmpAttrName, tmpAttrType);

  /* if inserting into the hash fails we should free up our memory */
  if (ret < 0)
  {
    free (tmpAttrName);
    free (tmpAttrType);
  }

  return ret;
}

int
lwes_event_type_db_check_for_event
  (struct lwes_event_type_db *db, 
   LWES_SHORT_STRING event_name)
{
  return lwes_hash_contains_key (db->events, event_name);
}

int
lwes_event_type_db_check_for_attribute
  (struct lwes_event_type_db *db, 
   LWES_CONST_SHORT_STRING attr_name,
   LWES_CONST_SHORT_STRING event_name)
{
  struct lwes_hash *event = 
    (struct lwes_hash *)lwes_hash_get (db->events, event_name);
  struct lwes_hash *meta_event = 
    (struct lwes_hash *)lwes_hash_get (db->events, LWES_META_INFO_STRING);
  int metaContainsAttribute = 0;
  int eventContainsAttribute = 0;
  if (event != NULL)
  {
    eventContainsAttribute = lwes_hash_contains_key (event, attr_name);
  }
  if (meta_event != NULL)
  {
    metaContainsAttribute = lwes_hash_contains_key (meta_event, attr_name);
  }
  return (metaContainsAttribute || eventContainsAttribute);
}

int
lwes_event_type_db_check_for_type
  (struct lwes_event_type_db *db, 
   LWES_BYTE type_value,
   LWES_CONST_SHORT_STRING attr_name,
   LWES_CONST_SHORT_STRING event_name)
{
  struct lwes_hash *event =
    (struct lwes_hash *)lwes_hash_get (db->events, event_name);
  struct lwes_hash *meta_event =
    (struct lwes_hash *)lwes_hash_get (db->events, LWES_META_INFO_STRING);
  LWES_BYTE *tmp_type = NULL;

  if (event != NULL)
  {
    tmp_type = (LWES_BYTE *)lwes_hash_get (event, attr_name);
  }
  if (tmp_type == NULL && meta_event != NULL)
  {
    tmp_type = (LWES_BYTE *)lwes_hash_get (meta_event, attr_name);
  }
  if (tmp_type == NULL)
  {
    return 0;
  }

  return ((*tmp_type)==type_value);
}
