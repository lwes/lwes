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
 *                                                                      *
 * $Id$                                                                 *
 *======================================================================*/
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include <lwes_event.h>

const char *output = "testeventmaxsizes.out";

LWES_U_INT_16     encoding  = (LWES_U_INT_16)1;

LWES_LONG_STRING
random_ascii (int max_length)
{
  /* the length should be between 1 and max_length, not including the '\0'
   * so add 1 for the '\0'
   */
  LWES_LONG_STRING string =
    (LWES_LONG_STRING) malloc ((max_length*sizeof (LWES_CHAR))+1);
  int i;

  for (i = 0; i < max_length; i++ )
    {
      /* valid ascii is 1 - 127 */
      string[i] = (LWES_CHAR)(65 + lrand48 () % 25);
    }
  /* need to finish with a null */
  string[max_length] = '\0';
  return string;
}

static char *
test_event1 (void)
{
  struct lwes_event *event1;
  struct lwes_event_type_db *db = NULL;
  LWES_BYTE bytes[MAX_MSG_SIZE];
  int size1;
  LWES_LONG_STRING string1;
  LWES_LONG_STRING string2;
  LWES_IP_ADDR      ip_addr;
  char bigbuf[200];
  char *retbuf;
  char bigdirbuf[4048];
  char file[4100];

  getcwd (bigdirbuf, 4048);
  snprintf (file, 4100, "%s/test1.esf", bigdirbuf);

  ip_addr.s_addr = inet_addr ("127.0.0.1");

  db = lwes_event_type_db_create (file);
  assert (db != NULL);

  event1 = lwes_event_create_with_encoding (db, "Event1", encoding);
  assert ( event1 != NULL );

  /* use the max string size */
  string1 = random_ascii (10);
  string2 = random_ascii (50);
  assert (lwes_event_set_STRING   (event1, "st",       string1) == 2);
  assert (lwes_event_set_BOOLEAN  (event1, "t_bool",   1)       == 3);
  assert (lwes_event_set_INT_16   (event1, "t_int16",  1)       == 4);
  assert (lwes_event_set_U_INT_16 (event1, "t_uint16", 1)       == 5);
  assert (lwes_event_set_INT_32   (event1, "t_int32",  1)       == 6);
  assert (lwes_event_set_U_INT_32 (event1, "t_uint32", 1)       == 7);
  assert (lwes_event_set_INT_64   (event1, "t_int64",  1)       == 8);
  assert (lwes_event_set_U_INT_64 (event1, "t_uint64", 1)       == 9);
  assert (lwes_event_set_IP_ADDR  (event1, "t_ip_addr",ip_addr) == 10);
  assert (lwes_event_set_STRING   (event1, "t_string", string2) == 11);

  size1 = lwes_event_to_bytes (event1, bytes, MAX_MSG_SIZE, 0);

  sprintf (bigbuf, "Event1\t%d\n",size1);

  lwes_event_destroy (event1);
  lwes_event_type_db_destroy (db);
  free (string1);
  free (string2);

  retbuf = (char *) malloc (sizeof (char) * strlen (bigbuf) + 1);
  strcpy (retbuf, bigbuf);

  return retbuf;
}

int main (void)
{
  int status = 0;
  FILE *output;
  char *expected_output = test_event1 ();
  size_t expected_length = strlen (expected_output);
  char buffer[2048];

  memset (buffer, 0, 2048);

  /* remove a previous run if it exists */
  system ("rm -f ./test1.out");

  /* call the calculate function */
  status =
    system ("../src/lwes-calculate-max-event-size ./test1.esf > ./test1.out");
  assert (status == 0);

  /* open up the output and compare to expected output */
  output = fopen ("./test1.out", "r");
  assert (output != NULL);
  fread (buffer, expected_length, sizeof (char), output);

  assert (strcmp (expected_output, buffer) == 0);
  fclose (output);
  free (expected_output);

  return 0;
}
