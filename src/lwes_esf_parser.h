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

#ifndef __LWES_ESF_PARSER_H
#define __LWES_ESF_PARSER_H

#if HAVE_CONFIG_H
  #include <config.h>
#endif

#include "lwes_event_type_db.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define lweslval _lweslval

/* define a structure to control the parser and lexer */

struct lwes_parser_state
{
  struct lwes_event_type_db *db;
  char *lastType;
  char *lastEvent;
  int lineno;
  int in_event;
  int errors;
};

extern int
lwes_parse_esf
  (struct lwes_event_type_db *database,
   const char *file_name);

extern void
lwes_parse_esf_destroy
  (void);

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_ESF_PARSER_H */

