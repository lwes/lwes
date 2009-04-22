%{
/*
 * This is the lexical analyser for the Event Specification file
 */

#define YYSTYPE const char*

#include "lwes_esf_parser.h"
#include "lwes_esf_parser_y.h"  /* token codes from the parser */

#define YY_NO_UNPUT

#undef YY_DECL
#define YY_DECL int lweslex( YYSTYPE *lvalp, void *param)

int lweslex(YYSTYPE *lvalp, void *param);
void lweslexdestroy (void);

void
lweslexdestroy
  (void)
{
/* FIXME: This doesn't work with multiple parsers, I think I need to
   experiment with reentrancy */
  lwes_delete_buffer (YY_CURRENT_BUFFER);
}


%}

%option noyywrap

%%

\n              { ((struct lwes_parser_state *) param)->lineno++; }
uint16          { 
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_UINT16);
                }
int16           {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_INT16);
                }
uint32          {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_UINT32);
                }
int32           {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_INT32);
                }
string          {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_STRING);
                }
ip_addr         {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_IP_ADDR);
                }
int64           {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_INT64);
                }
uint64          {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_UINT64);
                }
boolean         {
                  *lvalp = (YYSTYPE)lwestext;
                  return(YY_BOOLEAN);
                }
[a-zA-Z0-9_:]+  {
                   *lvalp = (YYSTYPE)lwestext;
                  if (((struct lwes_parser_state *) param)->in_event)
                    return(ATTRIBUTEWORD);
                  else
                    return(EVENTWORD);
                }
"{"             {
                  ((struct lwes_parser_state *) param)->in_event = 1;
                  return '{';
                }
"}"             {
                  ((struct lwes_parser_state *) param)->in_event = 0;
                  return '}';
                }
";"             {
                  return ';';
                }
"#"[^\n]*       /* eat up one-line comments */
.               ;
%%
