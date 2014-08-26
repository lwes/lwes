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
 * This is the lexical analyser for the Event Specification file
 */

#define YYSTYPE const char*

#include "lwes_esf_parser.h"
#include "lwes_esf_parser_y.h"  /* token codes from the parser */

#define YY_NO_UNPUT 1
#define YY_NO_INPUT 1

#undef YY_DECL
#define YY_DECL int lweslex( YYSTYPE *lvalp, void *param)

/* this fixes a problem in flex where compiling with -Wall and FORTIFY_SOURCE
   fails since the ECHO macro ignores the output of fwrite().  
   So we assign the output to a static variable but don't use it so 
   the compile suceeds */
/*
static int __lwesfwriteout;
#define ECHO _fwout = fwrite( yytext, yyleng, 1, yyout )
*/

/* function prototypes */
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

%option noyywrap nounput

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
