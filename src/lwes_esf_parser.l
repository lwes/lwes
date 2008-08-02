%{
/*
 * This is the lexical analyser for the Event Specification file
 */

#define YYSTYPE const char*

#include "lwes_esf_parser_y.h"  /* token codes from the parser */

#define YY_NO_UNPUT

int inEvent = 0;
extern int lineno;
int lweslex(void);
void lweslexdestroy (void);
YYSTYPE lweslval;

void
lweslexdestroy
  (void)
{
/* FIXME: This doesn't work with multiple parsers, I think I need to
   experiment with reentrancy */
/*  lwes_delete_buffer (YY_CURRENT_BUFFER); */
}


%}

%option noyywrap

%%

\n              { lineno++; }
uint16          { lweslval = (YYSTYPE)lwestext;
                  return(YY_UINT16);
                }
int16           { lweslval = (YYSTYPE)lwestext;
                  return(YY_INT16);
                }
uint32          { lweslval = (YYSTYPE)lwestext;
                  return(YY_UINT32);
                }
int32           { lweslval = (YYSTYPE)lwestext;
                  return(YY_INT32);
                }
string          { lweslval = (YYSTYPE)lwestext;
                  return(YY_STRING);
                }
ip_addr         { lweslval = (YYSTYPE)lwestext;
                  return(YY_IP_ADDR);
                }
int64           { lweslval = (YYSTYPE)lwestext;
                  return(YY_INT64);
                }
uint64          { lweslval = (YYSTYPE)lwestext;
                  return(YY_UINT64);
                }
boolean         { lweslval = (YYSTYPE)lwestext;
                  return(YY_BOOLEAN);
                }
[a-zA-Z0-9_:]+   { lweslval = (YYSTYPE)lwestext;
                  if (inEvent)
                    return(ATTRIBUTEWORD);
                  else
                    return(EVENTWORD);
                }
"{"             {
                  inEvent = 1;
                  return '{';
                }
"}"             {
                  inEvent = 0;
                  return '}';
                }
";"             {
                  return ';';
                }
"#"[^\n]*       /* eat up one-line comments */
.               ;
%%
