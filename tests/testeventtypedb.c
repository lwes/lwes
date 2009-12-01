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

#include <assert.h>

#include <stdlib.h>

/* wrap malloc and other functions to cause test memory problems */
void *my_malloc (size_t size);
struct lwes_hash *my_lwes_hash_create (void);
int my_lwes_hash_put(struct lwes_hash* hash, char *key, void *value);

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

#include "lwes_hash.h"

static int hash_null = 0;

struct lwes_hash *my_lwes_hash_create (void)
{
  struct lwes_hash * my_hash = NULL;
  if ( ! hash_null )
    {
      my_hash = lwes_hash_create ();
    }
  return my_hash;
}
#define lwes_hash_create my_lwes_hash_create

static int put_null = 0;
int my_lwes_hash_put(struct lwes_hash* hash, char *key, void *value)
{
  int ret = -3;
  if ( ! put_null )
    {
      ret = lwes_hash_put (hash, key, value);
    }
  return ret;
}
#define lwes_hash_put my_lwes_hash_put

#include "lwes_event_type_db.c"

#undef malloc
#undef lwes_hash_create
#undef lwes_hash_put

static void
test_db (void)
{
  struct lwes_event_type_db *db;
  const char *esffile = "testeventtypedb.esf";
  const char *nofile  = "doesntexist.esf";

  /* cause malloc to fail at initial malloc */
  null_at = 1;
  db = lwes_event_type_db_create((char*)esffile);
  assert ( db == NULL );

  /* cause malloc to fail at internal malloc */
  hash_null = 1;
  db = lwes_event_type_db_create((char*)esffile);
  assert ( db == NULL );
  hash_null = 0;

  /* try to load a non-existent files */
  db = lwes_event_type_db_create ((char *)nofile);
  assert (db == NULL);

  /* succeed this time */
  db = lwes_event_type_db_create((char*)esffile);
  assert ( db != NULL );

/*  lwes_event_type_db_to_stream(db,stdout); */

  /* this event will exist */
  assert (
    lwes_event_type_db_check_for_event( db,
                                        (LWES_SHORT_STRING)"TypeChecker") );
  /* so will this one */
  assert (
    lwes_event_type_db_check_for_event( db,
                                        (LWES_SHORT_STRING)"Empty") );

  /* this event will not */
  assert (
    ! lwes_event_type_db_check_for_event( db,
                                          (LWES_SHORT_STRING)"Foo") );

  /* these are the attributes of the event */
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aString",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_STRING_TOKEN,
                                        (LWES_SHORT_STRING)"aString",
                                        (LWES_SHORT_STRING)"TypeChecker"));

  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aBoolean",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_BOOLEAN_TOKEN,
                                        (LWES_SHORT_STRING)"aBoolean",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"anIPAddress",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_IP_ADDR_TOKEN,
                                        (LWES_SHORT_STRING)"anIPAddress",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aUInt16",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_16_TOKEN,
                                        (LWES_SHORT_STRING)"aUInt16",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"anInt16",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_INT_16_TOKEN,
                                        (LWES_SHORT_STRING)"anInt16",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aUInt32",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_32_TOKEN,
                                        (LWES_SHORT_STRING)"aUInt32",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"anInt32",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_INT_32_TOKEN,
                                        (LWES_SHORT_STRING)"anInt32",
                                        (LWES_SHORT_STRING)"TypeChecker"));

  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aUInt64",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_64_TOKEN,
                                        (LWES_SHORT_STRING)"aUInt64",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"anInt64",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_INT_64_TOKEN,
                                        (LWES_SHORT_STRING)"anInt64",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  /* now check the meta fields */
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"SenderIP",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_IP_ADDR_TOKEN,
                                        (LWES_SHORT_STRING)"SenderIP",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"SenderPort",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_16_TOKEN,
                                        (LWES_SHORT_STRING)"SenderPort",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"ReceiptTime",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_INT_64_TOKEN,
                                        (LWES_SHORT_STRING)"ReceiptTime",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"SiteID",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_16_TOKEN,
                                        (LWES_SHORT_STRING)"SiteID",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aMetaString",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_STRING_TOKEN,
                                        (LWES_SHORT_STRING)"aMetaString",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  /* also for the empty event */
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"SenderIP",
                                             (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_IP_ADDR_TOKEN,
                                        (LWES_SHORT_STRING)"SenderIP",
                                        (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"SenderPort",
                                             (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_16_TOKEN,
                                        (LWES_SHORT_STRING)"SenderPort",
                                        (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"ReceiptTime",
                                             (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_INT_64_TOKEN,
                                        (LWES_SHORT_STRING)"ReceiptTime",
                                        (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"SiteID",
                                             (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_16_TOKEN,
                                        (LWES_SHORT_STRING)"SiteID",
                                        (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aMetaString",
                                             (LWES_SHORT_STRING)"Empty"));
  assert (
    lwes_event_type_db_check_for_type ( db,
                                        LWES_STRING_TOKEN,
                                        (LWES_SHORT_STRING)"aMetaString",
                                        (LWES_SHORT_STRING)"Empty"));

  /* now make sure that unknown fields work fine */
  assert (
    ! lwes_event_type_db_check_for_attribute ( db,
                                             (LWES_SHORT_STRING)"aBar",
                                             (LWES_SHORT_STRING)"TypeChecker"));
  /* also that trying the wrong type fails */
  assert (
    ! lwes_event_type_db_check_for_type ( db,
                                        LWES_U_INT_16_TOKEN,
                                        (LWES_SHORT_STRING)"aMetaString",
                                        (LWES_SHORT_STRING)"TypeChecker"));
  assert (
    ! lwes_event_type_db_check_for_type ( db,
                                          LWES_U_INT_16_TOKEN,
                                          (LWES_SHORT_STRING)"random",
                                          (LWES_SHORT_STRING)"TypeChecker"));

  /* try some error cases with the lower level calls */

  /* fail at allocating the space for the event name */
  malloc_count = 0;
  null_at = 1;
  assert ( lwes_event_type_db_add_event ( db, (LWES_SHORT_STRING)"newEvent")
           == -3 );

  /* fail at allocating the hash for the event */
  hash_null = 1;
  assert ( lwes_event_type_db_add_event ( db, (LWES_SHORT_STRING)"newEvent")
           == -3 );
  hash_null = 0;

  /* fail at putting the event hash into the db hash */
  put_null = 1;
  assert ( lwes_event_type_db_add_event ( db, (LWES_SHORT_STRING)"newEvent")
           == -3 );
  put_null = 0;

  /* fail at allocating space for the attribute name */
  malloc_count = 0;
  null_at = 1;
  assert ( lwes_event_type_db_add_attribute ( db,
                                              (LWES_SHORT_STRING)"TypeChecker",
                                              (LWES_SHORT_STRING)"aMetaString",
                                              (LWES_SHORT_STRING)"string")
           == -3);

  /* fail at allocating space for the attribute value type */
  malloc_count = 0;
  null_at = 2;
  assert ( lwes_event_type_db_add_attribute ( db,
                                              (LWES_SHORT_STRING)"TypeChecker",
                                              (LWES_SHORT_STRING)"aMetaString",
                                              (LWES_SHORT_STRING)"string")
           == -3);

  /* fail at putting the attribute into the event hash */
  put_null = 1;
  assert ( lwes_event_type_db_add_attribute ( db,
                                              (LWES_SHORT_STRING)"TypeChecker",
                                              (LWES_SHORT_STRING)"aMetaString",
                                              (LWES_SHORT_STRING)"string")
           == -3);
  put_null = 0;

  /* finally free it all */
  lwes_event_type_db_destroy(db);
}

static void
test_2_db (void)
{
  struct lwes_event_type_db *db;
  struct lwes_event_type_db *db2;
  const char *esffile = "testeventtypedb.esf";
  const char *esffile2 = "testeventtypedb2.esf";

  /* create one */
  db = lwes_event_type_db_create ((char*)esffile);
  assert ( db != NULL );

  /* then another */
  db2 = lwes_event_type_db_create ((char*)esffile2);
  assert ( db2 != NULL );

  /* then free the first */
  lwes_event_type_db_destroy (db);

  /* then the second */
  lwes_event_type_db_destroy (db2);
}

int main(void)
{
  test_db ();
  test_2_db ();

  return 0;
}
