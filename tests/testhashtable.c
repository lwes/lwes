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
#if HAVE_CONFIG_H
  #include <config.h>
#endif

#include <stdlib.h>

/* wrap malloc and other functions to cause test memory problems */
void *my_malloc (size_t size);

static size_t null_at = 0;
static size_t malloc_count = 0;

void *my_malloc (size_t size)
{
  void *ret = NULL;
  malloc_count++;
  if ( malloc_count != null_at )
    {
      ret = malloc (size);
    }
  return ret;
}

#define malloc my_malloc

#include "lwes_hash.c"

#undef malloc

#include <assert.h>

int main(void)
{
  const char *key1= "key1";
  const char *key2= "key2";
  const char *key3= "key3";
  const char *key4= "key4";
  const char *key5= "key5";
  const char *key6= "key6";
  const char *key7= "key7";
  const char *key8= "key8";
  const char *key9= "key9";
  const char *key10= "key10";
  const char *key11= "key11";
  const char *key12= "key12";
  const char *badkey1= "badkey1";
  int    value1=5;
  int    value2=10;
  int    value3=11;
  int    value4=12;
  const char * value5="value5";
  double value6=3.14159;
  const char * value7="value7";
  double value8=29.345;
  const char * value9="value9";
  double value10=56.789;
  const char * value11="value11";
  double value12=105.9;
  int   * value1_out;
  int   * value2_out;
  int   * value3_out;
  int   * value4_out;
  char ** value5_out;
  double* value6_out;
  char ** value7_out;
  double* value8_out;
  char ** value9_out;
  double* value10_out;
  char ** value11_out;
  double* value12_out;
  int   * value1_rem;
  int   * value2_rem;
  int   * value3_rem;
  int   * value4_rem;
  char ** value5_rem;
  double* value6_rem;
  char ** value7_rem;
  double* value8_rem;
  char ** value9_rem;
  double* value10_rem;
  char ** value11_rem;
  double* value12_rem;

  struct lwes_hash *hash = NULL;
  struct lwes_hash_enumeration e;

  /* for enumeration test */
  int i;
  int num_found[] = { 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0 };

  /* first test some failures of malloc */
  /* malloc in create */
  null_at = 1;
  hash = lwes_hash_create();
  assert ( hash == NULL );

  /* malloc in init */
  malloc_count = 0;
  null_at = 2;
  hash = lwes_hash_create();
  assert ( hash == NULL );

  /* successful create */
  hash = lwes_hash_create();
  assert ( hash != NULL );

  /* hash should be empty */
  assert ( lwes_hash_is_empty(hash) );
  assert ( lwes_hash_size(hash) == 0 );

  /* some boundary conditions, should return errors */
  assert ( lwes_hash_put (NULL, (char*)key1,  NULL    ) == -1 );
  assert ( lwes_hash_put (hash, NULL,         NULL    ) == -1 );

  /* hash should still be empty */
  assert ( lwes_hash_is_empty(hash) );
  assert ( lwes_hash_size(hash) == 0 );

  /* malloc failure */
  malloc_count = 0;
  null_at = 1;
  assert ( lwes_hash_put (hash, (char*)key1,  &value1 ) == -3 );

  /* hash should still be empty */
  assert ( lwes_hash_is_empty(hash) );
  assert ( lwes_hash_size(hash) == 0 );

  /* return null for unknown keys */
  assert ( lwes_hash_get (hash, (char*)key1) == NULL );
  /* return null removal of unknown key */
  assert ( lwes_hash_remove (hash, (char*)key1) == NULL );
  /* return false for unknown keys */
  assert ( lwes_hash_contains_key (hash, (char*)key1) == 0 );

  /* test enumeration with no elements */
  assert ( lwes_hash_keys (hash, &e) );
  assert ( ! lwes_hash_enumeration_has_more_elements (&e));
  assert ( lwes_hash_enumeration_next_element (&e) == NULL );

  /* add a couple of elements */
  assert ( lwes_hash_put (hash, (char*)key1,  &value1 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key2,  &value2 ) == 0 );

  /* test enumeration with just a couple of elements */
  assert (lwes_hash_keys (hash, &e));
  i = 0;
  while ( lwes_hash_enumeration_has_more_elements (&e) )
  {
    char *tmpKey = lwes_hash_enumeration_next_element (&e);
    if ( ! strcmp ( key1,  tmpKey ) )
      {
        num_found[0]++;
      }
    if ( ! strcmp ( key2,  tmpKey ) )
      {
        num_found[1]++;
      }
    i++;
  }
  assert ( i == 2 );
  for ( i = 0 ; i < 2 ; i++ )
    {
      assert ( num_found[i] == 1 );
      num_found[i] = 0;  /* reset for below */
    }

  /* put a bunch more elements */
  assert ( lwes_hash_put (hash, (char*)key3,  &value3 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key4,  &value4 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key5,  &value5 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key6,  &value6 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key7,  &value7 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key8,  &value8 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key9,  &value9 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key10, &value10) == 0 );
  assert ( lwes_hash_put (hash, (char*)key11, &value11) == 0 );
  assert ( lwes_hash_put (hash, (char*)key12, &value12) == 0 );

  assert ( !lwes_hash_is_empty(hash) );
  assert ( lwes_hash_size(hash) == 12 );

  /* lwes_hash_to_stream(hash,stdout); */

  /* now succeed */
  assert (lwes_hash_keys (hash, &e));

  /* test enumeration of entire table */
  while ( lwes_hash_enumeration_has_more_elements(&e) )
  {
    char *tmpKey = lwes_hash_enumeration_next_element(&e);
    if ( ! strcmp ( key1,  tmpKey ) ) { num_found[0]++;  }
    if ( ! strcmp ( key2,  tmpKey ) ) { num_found[1]++;  }
    if ( ! strcmp ( key3,  tmpKey ) ) { num_found[2]++;  }
    if ( ! strcmp ( key4,  tmpKey ) ) { num_found[3]++;  }
    if ( ! strcmp ( key5,  tmpKey ) ) { num_found[4]++;  }
    if ( ! strcmp ( key6,  tmpKey ) ) { num_found[5]++;  }
    if ( ! strcmp ( key7,  tmpKey ) ) { num_found[6]++;  }
    if ( ! strcmp ( key8,  tmpKey ) ) { num_found[7]++;  }
    if ( ! strcmp ( key9,  tmpKey ) ) { num_found[8]++;  }
    if ( ! strcmp ( key10, tmpKey ) ) { num_found[9]++;  }
    if ( ! strcmp ( key11, tmpKey ) ) { num_found[10]++; }
    if ( ! strcmp ( key12, tmpKey ) ) { num_found[11]++; }
  }
  /* make sure we got ALL the elements out */
  for ( i = 0 ; i < 12 ; i++ )
    {
      assert ( num_found[i] == 1 );
    }

  /* test enumeration failures */
  assert (! lwes_hash_keys (NULL, &e) );
  assert (! lwes_hash_keys (hash, NULL) );

  /* get all the values */
  value1_out = (int *)lwes_hash_get (hash, (char*)key1);
  assert ( *value1_out == value1 );
  value2_out = (int *)lwes_hash_get (hash, (char*)key2);
  assert ( *value2_out == value2 );
  value3_out = (int *)lwes_hash_get (hash, (char*)key3);
  assert ( *value3_out == value3 );
  value4_out = (int *)lwes_hash_get (hash, (char*)key4);
  assert ( *value4_out == value4 );
  value5_out = (char **)lwes_hash_get (hash, (char*)key5);
  assert ( ! strcmp ( *value5_out, value5 ) );
  value6_out = (double *)lwes_hash_get (hash, (char*)key6);
  assert ( *value6_out == value6 );
  value7_out = (char **)lwes_hash_get (hash, (char*)key7);
  assert ( ! strcmp ( *value7_out, value7 ) );
  value8_out = (double *)lwes_hash_get (hash, (char*)key8);
  assert ( *value8_out == value8 );
  value9_out = (char **)lwes_hash_get (hash, (char*)key9);
  assert ( ! strcmp ( *value9_out, value9 ) );
  value10_out = (double *)lwes_hash_get (hash, (char*)key10);
  assert ( *value10_out == value10 );
  value11_out = (char **)lwes_hash_get (hash, (char*)key11);
  assert ( ! strcmp ( *value11_out, value11 ) );
  value12_out = (double *)lwes_hash_get (hash, (char*)key12);
  assert ( *value12_out == value12 );

  assert ( ! lwes_hash_is_empty (hash) );
  assert ( lwes_hash_size (hash) == 12 );

  assert (   lwes_hash_contains_key ( hash, (char*)key6 ) );
  assert ( ! lwes_hash_contains_key ( hash, (char*)badkey1 ) );

  /* destroy without removal of all elements fails */
  assert ( lwes_hash_destroy (hash) == -1 );

  /* test removal of bad key */
  assert ( lwes_hash_remove (hash, NULL) == NULL );

  /* remove all the elements */
  value1_rem = (int *)lwes_hash_remove (hash, (char*)key1);
  assert ( *value1_rem == value1 );
  value2_rem = (int *)lwes_hash_remove (hash, (char*)key2);
  assert ( *value2_rem == value2 );
  value3_rem = (int *)lwes_hash_remove (hash, (char*)key3);
  assert ( *value3_rem == value3 );
  value4_rem = (int *)lwes_hash_remove (hash, (char*)key4);
  assert ( *value4_rem == value4 );
  value5_rem = (char **)lwes_hash_remove (hash, (char*)key5);
  assert ( ! strcmp ( *value5_rem, value5 ) );
  value6_rem = (double *)lwes_hash_remove (hash, (char*)key6);
  assert ( *value6_rem == value6 );
  value7_rem = (char **)lwes_hash_remove (hash, (char*)key7);
  assert ( ! strcmp ( *value7_rem, value7 ) );
  value8_rem = (double *)lwes_hash_remove (hash, (char*)key8);
  assert ( *value8_rem == value8 );
  value9_rem = (char **)lwes_hash_remove (hash, (char*)key9);
  assert ( ! strcmp ( *value9_rem, value9 ) );
  value10_rem = (double *)lwes_hash_remove (hash, (char*)key10);
  assert ( *value10_rem == value10 );
  value11_rem = (char **)lwes_hash_remove (hash, (char*)key11);
  assert ( ! strcmp ( *value11_rem, value11 ) );
  value12_rem = (double *)lwes_hash_remove (hash, (char*)key12);
  assert ( *value12_rem == value12 );

  assert ( lwes_hash_is_empty(hash) );

  assert ( lwes_hash_destroy(hash) == 0 );

  /* testing the overflow coding, with 1 bin, everything should be in
   * the same bin, so searching for an unknown key should return null
   */
  hash = lwes_hash_create_with_bins (1);
  assert ( hash != NULL );
  assert ( lwes_hash_put (hash, (char*)key1,  &value1 ) == 0 );
  assert ( lwes_hash_put (hash, (char*)key2,  &value2 ) == 0 );
  assert ( lwes_hash_get (hash, (char*)key3) == NULL );
  assert ( lwes_hash_remove (hash, (char*)key3) == NULL );
  assert ( lwes_hash_remove (hash, (char*)key2) != NULL );
  assert ( lwes_hash_remove (hash, (char*)key1) != NULL );
  assert ( lwes_hash_is_empty(hash) );
  assert ( lwes_hash_destroy(hash) == 0 );

  return 0;
}
