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

#include "lwes_hash.h"

#include <string.h>

/*************************************************************************
  PRIVATE API Prototypes, shouldn't be called outside of this file
 *************************************************************************/
int
lwes_hash
  (const char *key);

int
lwes_hash_init
  (struct lwes_hash *hash, int bins);

/*************************************************************************
  PUBLIC API
 *************************************************************************/
struct lwes_hash *
lwes_hash_create
  (void)
{
  return lwes_hash_create_with_bins (10);
}

struct lwes_hash *
lwes_hash_create_with_bins
  (int total)
{
  struct lwes_hash *hash =
    (struct lwes_hash *) malloc (sizeof (struct lwes_hash));
  if ( hash != NULL )
    {
      if ( lwes_hash_init (hash, total) == -3 )
        {
          free (hash);
          hash = NULL;
        }
    }
  return hash;
}

int
lwes_hash_destroy
  (struct lwes_hash* hash)
{
  int ret = -1;
  /* force all entries to have been removed
   * TODO: This is a possible memory leak if callers don't check return values
   */
  if ( hash->assigned_entries == 0 )
    {
      free (hash->bins);
      free (hash);
      ret = 0;
    }
  return ret;
}

int
lwes_hash_is_empty
  (struct lwes_hash* hash)
{
  return (hash->assigned_entries==0);
}

int
lwes_hash_size
  (struct lwes_hash* hash)
{
  return hash->assigned_entries;
}

int
lwes_hash_put
  (struct lwes_hash* hash,
   char *key,
   void *value)
{
  int index;
  struct lwes_hash_element *bin         = NULL;
  struct lwes_hash_element *new_element = NULL;

  if ( key == NULL || hash == NULL )
    {
      return -1;
    }

  new_element =
    (struct lwes_hash_element *) malloc (sizeof (struct lwes_hash_element));
  if ( new_element == NULL )
    {
      return -3;
    }
  new_element->key   = key;
  new_element->value = value;
  new_element->next  = NULL;

  index = lwes_hash(key)%hash->total_bins;

  if ( hash->bins[index] == NULL )
    {
      hash->bins[index] = (void *)new_element;
    }
  else
    {
      bin = (struct lwes_hash_element *)hash->bins[index];
      while ( bin->next != NULL )
        {
          bin = bin->next;
        }
      bin->next     = new_element;
    }
  hash->assigned_entries++;

  return 0;
}

void *
lwes_hash_get
  (struct lwes_hash* hash,
   const char *key)
{
  int index;
  struct lwes_hash_element *bin      = NULL;
  struct lwes_hash_element *searcher = NULL;

  index = lwes_hash (key) % hash->total_bins;
  bin = hash->bins[index];
  if ( bin == NULL )
    return NULL;
  searcher=bin;
  while ( searcher != NULL && strcmp (searcher->key, key) != 0 )
    {
      searcher = searcher->next; 
    }
  if ( searcher == NULL )
    return NULL;

  return searcher->value;
}

void *
lwes_hash_remove
  (struct lwes_hash* hash,
   const char *key)
{
  int index;
  void *return_value;
  struct lwes_hash_element *head     = NULL;
  struct lwes_hash_element *searcher = NULL;
  struct lwes_hash_element *prev     = NULL;

  if ( key == NULL )
    {
      return NULL;
    }
  index = lwes_hash (key) % hash->total_bins;
  head = (struct lwes_hash_element *)hash->bins[index];
  if ( head == NULL )
    {
      return NULL;
    }
  searcher=head;
  prev=head;
  while ( searcher != NULL && strcmp (searcher->key,key) != 0 )
    {
      prev     = searcher;
      searcher = searcher->next; 
    }
  /* it's not in the list, so don't do anything, return NULL; */
  if ( searcher == NULL )
    return NULL;

  /* else, set the previous next equal to the searcher next */
  if ( searcher == head )
    {
      hash->bins[index] = (void *)searcher->next;
    }
  else
    {
      prev->next = searcher->next;
    }
  searcher->next = NULL;
  return_value = searcher->value;
  free (searcher);

  hash->assigned_entries--;
  return return_value;
}

int lwes_hash_contains_key(struct lwes_hash* hash, const char *key)
{
  int index;
  struct lwes_hash_element *bin      = NULL;
  struct lwes_hash_element *searcher = NULL;

  index = lwes_hash (key) % hash->total_bins;
  bin = hash->bins[index];

  if ( bin == NULL )
    return 0;

  searcher=bin;

  while ( searcher != NULL && strcmp (searcher->key,key) != 0 )
    {
      searcher = searcher->next; 
    }

  if ( searcher == NULL )
    return 0;

  return 1;
}

int
lwes_hash_keys
  (struct lwes_hash * hash,
   struct lwes_hash_enumeration *enumeration)
{
  if (hash == NULL || enumeration == NULL)
    {
      return 0;
    }

  enumeration->index = 0;
  enumeration->elements_given = 0;
  enumeration->size_at_start = lwes_hash_size (hash);
  enumeration->enum_hash = hash;
  /* set the current to the first entry of the first list */
  enumeration->current   =
    (struct lwes_hash_element *)
       enumeration->enum_hash->bins[enumeration->index];

  return 1;
}

int
lwes_hash_enumeration_has_more_elements
  (struct lwes_hash_enumeration *enumeration)
{
  int ret;
  ret = (enumeration->elements_given != enumeration->size_at_start);
  return ret;
}

char *
lwes_hash_enumeration_next_element
  (struct lwes_hash_enumeration *enumeration)
{
  struct lwes_hash_element *prev;

  /* if the current value pointed at is null (this will most often happen
   * the first time this is called, or the next value in this list is null,
   * increment the index and check again.
   */
  while ( enumeration->current == NULL )
  {
    enumeration->index++;
    /* takes care of the case where we ask beyond the end */
    if ( enumeration->index >= enumeration->enum_hash->total_bins )
    {
      return NULL;
    }
    enumeration->current = enumeration->enum_hash->bins[enumeration->index];
  }

  prev = enumeration->current;
  enumeration->current = enumeration->current->next;
  enumeration->elements_given++;
  return prev->key;
}

/*************************************************************************
  PRIVATE API, shouldn't be called by a user of the library.
 *************************************************************************/
int
lwes_hash
  (const char *key)
{
  int sum = 0;
  int i;
  for ( i = 0; key[i] != '\0'; i++ )
    sum += ((int)key[i])*((int)key[i]);
  return sum;
}

int
lwes_hash_init
  (struct lwes_hash *hash,
   int total)
{
  int i;
  int ret = -3;
  hash->total_bins       = total;
  hash->assigned_entries = 0;
  hash->bins             =
    (void **)malloc(sizeof(void *) * hash->total_bins);
  if ( hash->bins != NULL )
    {
      for ( i = 0; i < hash->total_bins; i++)
      {
        hash->bins[i] = NULL;
      }
      ret = 0;
    }
  return ret;
}

