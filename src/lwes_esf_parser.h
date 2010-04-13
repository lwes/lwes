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
  LWES_TYPE         lastType;
  LWES_SHORT_STRING lastEvent;
  int               lineno;
  int               in_event;
  int               errors;
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
