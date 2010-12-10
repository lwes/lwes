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

#ifndef __LWES_HASH_H
#define __LWES_HASH_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file lwes_hash.h
 *  \brief Functions for dealing with the hash which is in the event
 */

/*! \struct lwes_hash lwes_hash.h
 *  \brief Structure containing a hashtable, used to store key value
 *         pairs in the event.  This is opaque in case of future extension.
 */
struct lwes_hash
{
  void **bins;
  int total_bins;
  int assigned_entries;
};

struct lwes_hash_element
{
  char * key;
  void * value;
  struct lwes_hash_element *next;
};

/*! \struct lwes_hash_enumeration lwes_hash.h
 *  \brief Structure for enumerating over the hash.  This is opaque in 
 *         case of future extension.
 */
struct lwes_hash_enumeration
{
  int index;
  int elements_given;
  int size_at_start;  /* in case we are using this enumeration to remove all the
                         elements in the hash */
  struct lwes_hash *enum_hash;
  struct lwes_hash_element *current;
};

/*! \brief Create the memory for a hashtable
 *
 *  Creates the memory for a hash, use lwes_hash_destroy to free memory.
 *
 *  \see lwes_hash_destroy
 *  \return a newly allocated hash or NULL if an error occured
 */
struct lwes_hash *
lwes_hash_create
  (void);

struct lwes_hash *
lwes_hash_create_with_bins
  (int total_bins);

int
lwes_hash_destroy
  (struct lwes_hash* hash);

int
lwes_hash_is_empty
  (struct lwes_hash* hash);

int
lwes_hash_size
  (struct lwes_hash* hash);

int
lwes_hash_rehash
  (struct lwes_hash* hash);

int
lwes_hash_put
  (struct lwes_hash* hash,
   char *key,
   void *value);

void *
lwes_hash_get
  (struct lwes_hash* hash,
   const char *key);

void *
lwes_hash_remove
  (struct lwes_hash* hash,
   const char *key);

int
lwes_hash_contains_key
  (struct lwes_hash* hash,
   const char *key);

int
lwes_hash_keys
  (struct lwes_hash * hash,
   struct lwes_hash_enumeration *enumeration);

int
lwes_hash_enumeration_has_more_elements
  (struct lwes_hash_enumeration *enumeration);

char *
lwes_hash_enumeration_next_element
  (struct lwes_hash_enumeration *enumeration);

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_HASH_H */
