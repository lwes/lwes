%{
/*
 * This is the parser for the Event Specification file
 */
#include <stdio.h>

#if HAVE_CONFIG_H
  #include <config.h>
#endif

#define YYSTYPE const char*

#include "lwes_esf_parser.h"

int lwesparse(void);
int lweslex(void);
void lweslexdestroy (void);
void lweserror(const char *s);

static struct lwes_event_type_db *db;
static char *lastType;
static char *lastEvent;
%}

%token YY_UINT16 YY_INT16 YY_UINT32 YY_INT32 YY_INT64 YY_UINT64 YY_BOOLEAN YY_STRING YY_IP_ADDR EVENTWORD ATTRIBUTEWORD

%%
eventlist: event
    | eventlist event
    ;

event: eventname '{' attributelist '}'  {
                                          if ( lastType != NULL )
                                            free(lastType);
                                          if ( lastEvent != NULL )
                                            free(lastEvent);
                                          lastType  = NULL;
                                          lastEvent = NULL;
                                        }
    | error ';'   { lweserror("parser error with ';'"); }
    | error '}'   { lweserror("parser error with '}'"); }
    ;

eventname: EVENTWORD  {
                        lwes_event_type_db_add_event
                          (db,
                           (LWES_SHORT_STRING)lweslval);
                        lastEvent = strdup (lweslval);

                        if ( lastEvent == NULL ) {
                          char buffer[256];
                          sprintf(buffer,"malloc problem for eventname '%s'",
                                  lweslval);
                          lweserror(buffer);
                        }
                      }
    ;

attributelist: attribute
    |   attributelist attribute
    ;

attribute: type attributename ';'
    | type attributename error ';'  { lweserror("Did you forget a ';'?"); }
    | type attributename error '}'  { lweserror("Did you forget a semi-colon?"); }
    ;

attributename: ATTRIBUTEWORD {
                               if (lastType != NULL)
                                 {
                                   lwes_event_type_db_add_attribute
                                     (db,
                                      (LWES_SHORT_STRING)lastEvent,
                                      (LWES_SHORT_STRING)lweslval,
                                      (LWES_SHORT_STRING)lastType);
                                   free(lastType);
                                   lastType = NULL;
                                 }
                               else
                                 {
                                   lweserror("Bad 'type' 'attributename' pair");
                                 }
                             }
    ;

type: YY_UINT16  {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);

                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_INT16   {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);

                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_UINT32  {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);
                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_INT32   {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);
                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_STRING  {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);
                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_IP_ADDR {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);
                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_INT64   {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);
                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_UINT64  {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);
                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | YY_BOOLEAN {
                   /* allocate a string, and copy the type value into it */
                   lastType = strdup (lweslval);
                   if ( lastType == NULL )
                     {
                       char buffer[256];
                       sprintf(buffer,"malloc problem for type '%s'", lweslval);
                       lweserror(buffer);
                     }
                 }
    | ATTRIBUTEWORD { char buffer[256];
                      sprintf(buffer,"unknown type '%s'",lweslval); 
                      lweserror(buffer);
                      lastType = NULL;
                    }
    ;

%%

extern FILE *lwesin;

int lineno = 1;
int errors = 0;

int
lwes_parse_esf
  (struct lwes_event_type_db *database,
   const char *filename)
{
  FILE *fd = NULL;

  db=database;

  /* reset errors and lineno */
  errors = 0;
  lineno = 1;

  /* open the file */
  fd = fopen (filename, "r");

  if ( fd != NULL )
  {
    lwesin = fd;
    lwesparse();
    fclose (fd);
  }
  else
  {
    fprintf (stderr,"ERROR: No such file : \"%s\"\n",filename);
  }

  if (!errors)
    return 1;

  return 0;
}

void
lwes_parse_esf_destroy
  (void)
{
  lweslexdestroy ();
}

void lweserror(const char *s)
{
  fprintf(stderr,"ERROR : %s : line %d\n",s,lineno);
  errors = 1;
}
