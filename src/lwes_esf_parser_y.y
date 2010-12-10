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

%{
/*
 * This is the parser for the Event Specification file
 */
#include <stdio.h>

#if HAVE_CONFIG_H
  #include <config.h>
#endif

#define YYSTYPE const char*
#define YYLEX_PARAM param
#define YYPARSE_PARAM param

#include "lwes_esf_parser.h"

int lwesparse(void *param);
int lweslex(YYSTYPE *lvalp, void *param);
void lweslexdestroy (void);
void lwesrestart(FILE *input_file);

void lwes_yyerror(const char *s, void *param);

/* since yyerror() only supports the msg param, we 'fool'
   C into sending the void * pointer along with the message */
#define lweserror(msg) lwes_yyerror(msg, param)

%}

%pure_parser

%token YY_UINT16 YY_INT16 YY_UINT32 YY_INT32 YY_INT64 YY_UINT64 YY_BOOLEAN YY_STRING YY_IP_ADDR EVENTWORD ATTRIBUTEWORD

%%
eventlist: event
    | eventlist event
    ;

event: eventname '{' attributelist '}'  {
            if ( ((struct lwes_parser_state *) param)->lastType != NULL )
              free( ((struct lwes_parser_state *) param)->lastType );
            if ( ((struct lwes_parser_state *) param)->lastEvent != NULL )
              free( ((struct lwes_parser_state *) param)->lastEvent );
            ((struct lwes_parser_state *) param)->lastType = NULL;
            ((struct lwes_parser_state *) param)->lastEvent = NULL;
                                        }
    | error ';'   { lweserror("parser error with ';'"); }
    | error '}'   { lweserror("parser error with '}'"); }
    ;

eventname: EVENTWORD  {
          lwes_event_type_db_add_event(((struct lwes_parser_state *) param)->db,
                                       (LWES_SHORT_STRING)lweslval);
          ((struct lwes_parser_state *) param)->lastEvent = strdup(lweslval);

          if(((struct lwes_parser_state *) param)->lastEvent == NULL ) {
                        char buffer[256];
                        sprintf(buffer,"malloc problem for eventname '%s'",
                                lweslval);
                        lweserror(buffer);
          }
                      }
  
    ;

attributelist:
      /* empty */
    | attribute
    | attributelist attribute
    ;

attribute: type attributename ';'
    | type attributename error ';'  { lweserror("Did you forget a ';'?"); }
    | type attributename error '}'  { lweserror("Did you forget a semi-colon?"); }
    ;

attributename: ATTRIBUTEWORD {
        if (((struct lwes_parser_state *) param)->lastType != NULL)
        {
          lwes_event_type_db_add_attribute
            (((struct lwes_parser_state *) param)->db,
            (LWES_SHORT_STRING) ((struct lwes_parser_state *) param)->lastEvent,
            (LWES_SHORT_STRING) lweslval,
            (LWES_SHORT_STRING) ((struct lwes_parser_state *) param)->lastType);
            free(((struct lwes_parser_state *) param)->lastType);
            ((struct lwes_parser_state *) param)->lastType = NULL;
         }
         else
         {
         lweserror("Bad 'type' 'attributename' pair");
         }
                             }
    ;

type: YY_UINT16  {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);

          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_INT16   {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);

          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_UINT32  {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);
          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_INT32   {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);
          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_STRING  {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);
          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_IP_ADDR {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);
          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_INT64   {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);
          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_UINT64  {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);
          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | YY_BOOLEAN {
          /* allocate a string, and copy the type value into it */
          ((struct lwes_parser_state *) param)->lastType = strdup(lweslval);
          if ( ((struct lwes_parser_state *) param)->lastType == NULL )
          {
            char buffer[256];
            sprintf(buffer,"malloc problem for type '%s'", lweslval);
            lweserror(buffer);
          }
                 }
    | ATTRIBUTEWORD { char buffer[256];
                      sprintf(buffer,"unknown type '%s'",lweslval); 
                      lweserror(buffer);
                      ((struct lwes_parser_state *) param)->lastType = NULL;
                    }
    ;

%%

extern FILE *lwesin;

int
lwes_parse_esf
  (struct lwes_event_type_db *database,
   const char *filename)
{
  FILE *fd = NULL;
  struct lwes_parser_state state;

  state.db = database;
  state.lastType = NULL;
  state.lastEvent = NULL;
  state.lineno = 1;
  state.in_event = 0;
  state.errors = 0;

  /* open the file */
  fd = fopen (filename, "r");

  if ( fd != NULL )
  {
    lwesin = fd;
    lwesrestart(lwesin);
    lwesparse((void *) &state);
    fclose (fd);
  }
  else
  {
    fprintf (stderr,"ERROR: No such file : \"%s\"\n",filename);
    state.errors++;
  }

  if (state.errors)
    return 1;

  return 0;
}

void
lwes_parse_esf_destroy
  (void)
{
  lweslexdestroy ();
}

void lwes_yyerror(const char *s, void *param)
{
  fprintf(stderr,"ERROR : %s : line %d\n",s, ((struct lwes_parser_state *) param)->lineno);
  ((struct lwes_parser_state *) param)->errors = 1;
}
