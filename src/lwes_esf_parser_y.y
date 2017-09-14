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

void duplicate_lex_string (void* param, char* *dest, const char* str, const char* label);
void lwes_add_type_to_state(void* param, const char* type);
void lwes_yyerror(void *param, const char *s);

/* since (bison 2.x) yyerror() only supports the msg param, we 'fool'
   C into sending the void * pointer along with the message */
/*
#define lweserror(msg) lwes_yyerror(param, msg)
*/
#define lweserror lwes_yyerror

%}

%pure-parser
%parse-param {void* param}
%lex-param {void *param}

%token YY_UINT16 YY_INT16 YY_UINT32 YY_INT32 YY_INT64 YY_UINT64 YY_BOOLEAN YY_STRING YY_IP_ADDR YY_BYTE YY_FLOAT YY_DOUBLE 
%token YY_REQUIRED YY_OPTIONAL YY_NULLABLE
%token EVENTWORD ATTRIBUTEWORD ATTRIBUTESIZE BADSIZE INVALID

%%

eventlist: event
    | eventlist event
    ;

event: check eventname check '{' check attributelist check '}'  {
            if ( ((struct lwes_parser_state *) param)->lastType != NULL )
              free( ((struct lwes_parser_state *) param)->lastType );
            if ( ((struct lwes_parser_state *) param)->lastEvent != NULL )
              free( ((struct lwes_parser_state *) param)->lastEvent );
            ((struct lwes_parser_state *) param)->lastType = NULL;
            ((struct lwes_parser_state *) param)->lastEvent = NULL;
                                        }
    | error ';'   { lwes_yyerror(param, "parser error with ';'"); }
    | error '}'   { lwes_yyerror(param, "parser error with '}'"); }
    ;

eventname: EVENTWORD  {
          struct lwes_parser_state* state = (struct lwes_parser_state*)param;
          lwes_event_type_db_add_event(state->db, (LWES_SHORT_STRING)lweslval);
          duplicate_lex_string(param, &(state->lastEvent), lweslval, "eventname");
                      }
    ;

attributelist:
      /* empty */
    | attribute
    | attributelist attribute
    ;

attribute: check type check attributename check stringspec check arrayspec check ';' {
        {
          struct lwes_parser_state* state = (struct lwes_parser_state *) param;
          if (state->lastType != NULL)
          {
            /* TODO complain if strMaxSize is set and lastType is not a string */
            /* fprintf(stderr, "Adding event:[%s] type:[%s] field:[%s] flags [0x%x] arraySize:[%d] stringLimit[%d]\n", state->lastEvent, state->lastType, state->lastField, state->flags, state->arrayTypeSize, state->strMaxSize); */
            lwes_event_type_db_add_attribute_ex
              (((struct lwes_parser_state *) param)->db,
               (LWES_SHORT_STRING) state->lastEvent,
               (LWES_SHORT_STRING) state->lastField,
               (LWES_SHORT_STRING) state->lastType,
               (LWES_U_INT_32) state->flags,
               (LWES_INT_16) state->arrayTypeSize,
               (LWES_INT_16) state->strMaxSize
              );
              free(state->lastType);
              state->lastType = NULL;
              free(state->lastField);
              state->lastField = NULL;
              state->flags = 0;
              state->arrayTypeSize = 0;
              state->strMaxSize = 0;
           }
           else
           {
           lwes_yyerror(param, "Bad 'type' 'attributename' pair");
           }
        }
    }
/*
    | type attributename error ';'  { lwes_yyerror(param, "Did you forget a ';'?"); }
    | type attributename error '}'  { lwes_yyerror(param, "Did you forget a semi-colon?"); }
*/
    ;

stringspec:
      /* empty */
    |  '(' check str_size check ')'
    ;

arrayspec:
      /* empty */
    |  '[' check ']' 
           { ((struct lwes_parser_state *) param)->arrayTypeSize = -1; }
    |  '[' check count check ']' { }
    ;

count: ATTRIBUTESIZE { /* No-op */ }
    | BADSIZE  { lwes_yyerror(param, "Invalid Array Size"); }
    ;

str_size: ATTRIBUTESIZE { /* No-op */ }
    | BADSIZE  { lwes_yyerror(param, "Invalid Array Size"); }
    ;

attributename: ATTRIBUTEWORD {
          struct lwes_parser_state* state = (struct lwes_parser_state*)param;
          if (NULL == ((struct lwes_parser_state *) param)->lastType)
            {
            lwes_yyerror(param, "Bad 'type' 'attributename' pair");
            }
          if ( ((struct lwes_parser_state *) param)->lastField != NULL )
            free( ((struct lwes_parser_state *) param)->lastField );
          duplicate_lex_string(param, &(state->lastField), lweslval, "fieldname");
                             }
    ;

check:
      /* empty */
    |  INVALID { lwes_yyerror(param, "Invalid characters"); }
    ;

type: modifier type
    | YY_UINT16  { lwes_add_type_to_state(param, lweslval); }
    | YY_INT16   { lwes_add_type_to_state(param, lweslval); }
    | YY_UINT32  { lwes_add_type_to_state(param, lweslval); }
    | YY_INT32   { lwes_add_type_to_state(param, lweslval); }
    | YY_STRING  { lwes_add_type_to_state(param, lweslval); }
    | YY_IP_ADDR { lwes_add_type_to_state(param, lweslval); }
    | YY_INT64   { lwes_add_type_to_state(param, lweslval); }
    | YY_UINT64  { lwes_add_type_to_state(param, lweslval); }
    | YY_BOOLEAN { lwes_add_type_to_state(param, lweslval); }
    | YY_BYTE    { lwes_add_type_to_state(param, lweslval); }
    | YY_FLOAT   { lwes_add_type_to_state(param, lweslval); }
    | YY_DOUBLE  { lwes_add_type_to_state(param, lweslval); }
    | ATTRIBUTEWORD { char buffer[256];
                      sprintf(buffer,"unknown type '%s'",lweslval);
                      lwes_yyerror(param, buffer);
                      ((struct lwes_parser_state *) param)->lastType = NULL;
                    }
    ;


modifier: 
    | YY_REQUIRED
        { ((struct lwes_parser_state *) param)->flags |= ATTRIBUTE_REQUIRED; }
    | YY_OPTIONAL
        { ((struct lwes_parser_state *) param)->flags |= ATTRIBUTE_OPTIONAL; }
    | YY_NULLABLE
        { ((struct lwes_parser_state *) param)->flags |= ATTRIBUTE_NULLABLE; }
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
  state.lastField = NULL;
  state.flags = 0;
  state.arrayTypeSize = 0;
  state.strMaxSize = 0;
  state.lineno = 1;
  state.in_event = 0;
  state.in_array = 0;
  state.in_str_size = 0;
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

void
duplicate_lex_string
  (void* param, char* *dest, const char* str, const char* label)
{
  /* allocate a string, and copy the type value into it */
  *dest = strdup(str);
  if ( *dest == NULL )
  {
    char buffer[256];
    sprintf(buffer,"malloc problem for %s '%s'", label, str);
    lwes_yyerror(param, buffer);
  }
}

void
lwes_add_type_to_state
  (void* param, const char* type)
{
  struct lwes_parser_state* state = (struct lwes_parser_state*)param;
  duplicate_lex_string(param, &(state->lastType), type, "type");
}

/*void lwes_yyerror(const char *s, void *param)*/
void lwes_yyerror(void *param, const char *s)
{
  fprintf(stderr,"ERROR : %s : line %d\n",s, ((struct lwes_parser_state *) param)->lineno);
  ((struct lwes_parser_state *) param)->errors = 1;
}
