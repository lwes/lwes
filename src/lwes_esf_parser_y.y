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

int lwesparse (void *param);
int lweslex (YYSTYPE *lvalp, void *param);
void lweslexdestroy (void);
void lwesrestart (FILE *input_file);

void lwes_yyerror (const char *s, void *param);

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

event: eventname '{' attributelist '}'
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      if ( state->lastEvent != NULL )
        {
          free( state->lastEvent );
        }
      state->lastType  = LWES_TYPE_UNDEFINED;
      state->lastEvent = NULL;
    }
  | error ';'
    {
      lweserror("parser error with ';'");
    }
  | error '}'
    {
      lweserror("parser error with '}'");
    }
  ;

eventname: EVENTWORD
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      lwes_event_type_db_add_event (state->db, (LWES_SHORT_STRING) lweslval);
      state->lastEvent = strdup(lweslval);

      if(state->lastEvent == NULL )
        {
          char buffer[256];
          sprintf(buffer,"malloc problem for eventname '%s'", lweslval);
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
  | type attributename error ';'
    {
      lweserror("Did you forget a ';'?");
    }
  | type attributename error '}'
    {
      lweserror("Did you forget a semi-colon?");
    }
  ;

attributename: ATTRIBUTEWORD
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      if (state->lastType != LWES_TYPE_UNDEFINED)
        {
          lwes_event_type_db_add_attribute
            ( state->db,
              state->lastEvent,
              (LWES_SHORT_STRING) lweslval,
              state->lastType);
          state->lastType = LWES_TYPE_UNDEFINED;
        }
      else
        {
          lweserror("Bad 'type' 'attributename' pair");
        }
    }
  ;

type: YY_UINT16
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_U_INT_16;
    }
  | YY_INT16
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_INT_16;
    }
  | YY_UINT32
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_U_INT_32;
    }
  | YY_INT32
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_INT_32;
    }
  | YY_STRING
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_STRING;
    }
  | YY_IP_ADDR
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_IP_ADDR;
    }
  | YY_INT64
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_INT_64;
    }
  | YY_UINT64
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_U_INT_64;
    }
  | YY_BOOLEAN
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      state->lastType = LWES_TYPE_BOOLEAN;
    }
  | ATTRIBUTEWORD
    {
      struct lwes_parser_state *state = (struct lwes_parser_state *) param;
      char buffer[256];
      sprintf(buffer,"unknown type '%s'",lweslval); 
      lweserror(buffer);
      state->lastType = LWES_TYPE_UNDEFINED;
    }
  ;

%%

extern FILE *lwesin;

int lwes_parse_esf (struct lwes_event_type_db *database, const char *filename)
{
  FILE *fd = NULL;
  struct lwes_parser_state state;

  state.db = database;
  state.lastType = LWES_TYPE_UNDEFINED;
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

void lwes_parse_esf_destroy (void)
{
  lweslexdestroy ();
}

void lwes_yyerror (const char *s, void *param)
{
  struct lwes_parser_state *state = (struct lwes_parser_state *) param;
  fprintf(stderr,"ERROR : %s : line %d\n",s, state->lineno);
  state->errors = 1;
}
