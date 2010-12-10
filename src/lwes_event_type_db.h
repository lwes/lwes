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

#ifndef __LWES_EVENT_TYPE_DB
#define __LWES_EVENT_TYPE_DB

#include "lwes_types.h"

#include <stdio.h>   /* for FILENAME_MAX */
#include <stdlib.h>  /* for malloc */
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file lwes_event_type_db.h
 *  \brief Functions for an event specification file database
 *
 *  This is used in cases where you want to restrict the events allowed
 *  to be emitted or listened to to a particular esf file
 */

/*! \struct lwes_event_type_db_attribute lwes_event_type_db.h
 *  \brief The attributes stored in the database
 */
struct lwes_event_type_db_attribute
{
  /*! An event */
  struct lwes_event *event;
};

/*! \struct lwes_event_type_db lwes_event_type_db.h
 *  \brief The data base itself
 */
struct lwes_event_type_db
{
  /*! holds the file name which describes the database (aka, the esf file) */
  char esf_filename[FILENAME_MAX];
  /*! holds a hash of event descriptions by the event name
      for events which are described in the esf file */
  struct lwes_hash *events;
};

/*! \brief Creates the memory for the event_type_db.
 *  
 *  This creates memory which should be freed with lwes_event_type_db_destroy
 *
 *  \param[in] filename the path to the file containing the esf description
 *
 *  \see lwes_event_type_db_destroy
 *
 *  \return the newly created db on success, NULL on failure
 */
struct lwes_event_type_db *
lwes_event_type_db_create
  (const char *filename);

/*! \brief Cleanup the memory for the event_type_db.
 *
 *  This frees the memory created by lwes_event_type_db_create.
 *
 *  \param[in] db the db to free
 *
 *  \return 0 on success, a negative number on failure
 */
int
lwes_event_type_db_destroy
  (struct lwes_event_type_db *db);

/*! \brief Add an an event name to the database
 *
 * \param[in] db the db to add the event name to
 * \param[in] event_name the name of an event
 *
 * \return 0 if the add is successful, a negative number on failure
 */
int
lwes_event_type_db_add_event
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name);

/*! \brief Add an attribute name to an event name of the database
 *
 * \param[in] db the db to add the attr_name into
 * \param[in] event_name the name of an event
 * \param[in] attr_name the name of an attribute
 * \param[in] type the type of the attribute
 *
 * \return 0 if the add is successful, a negative number on failure
 */
int
lwes_event_type_db_add_attribute
  (struct lwes_event_type_db *db,
   LWES_SHORT_STRING event_name,
   LWES_SHORT_STRING attr_name,
   LWES_SHORT_STRING type);

/*! \brief Check for an event in the database
 *
 *  \param[in] db the db to check for the event in
 *  \param[in] event_name the name of an event
 *
 *  \return 1 if the event is in the db, 0 if it is not
 */
int
lwes_event_type_db_check_for_event
  (struct lwes_event_type_db *db, 
   LWES_SHORT_STRING event_name);

/*! \brief Check for an attribute in an event in the database
 *
 *  \param[in] db the db to check
 *  \param[in] attr_name the attribute name to check for
 *  \param[in] event_name the event name to check in
 *
 *  \return 1 if the attribute is in the event in the db, 0 if it is not
 */
int
lwes_event_type_db_check_for_attribute
  (struct lwes_event_type_db *db, 
   LWES_CONST_SHORT_STRING attr_name,
   LWES_CONST_SHORT_STRING event_name);

/*! \brief Check the type of an attribute in an event in the db
 *
 *  \param[in] db the db to check
 *  \param[in] type_value the type to check against
 *  \param[in] attr_name the attribute name to check for
 *  \param[in] event_name the event name to check in
 *
 *  \return 1 if the attribute in the event in the db is of the asked for type,
 *          0 if it is not of the specified type
 */
int
lwes_event_type_db_check_for_type
  (struct lwes_event_type_db *db, 
   LWES_BYTE type_value,
   LWES_CONST_SHORT_STRING attr_name,
   LWES_CONST_SHORT_STRING event_name);

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_EVENT_TYPE_DB */
