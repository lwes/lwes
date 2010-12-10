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

#ifndef __LWES_TYPES_H
#define __LWES_TYPES_H

#include <sys/types.h> /* for size_t in cygwin */
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif 

/* I wanted mainpage documentation somewhere, so this is the best place
   I can think of
 */

/*! \mainpage Light Weight Event System (LWES)
 *
 * The LWES Light-Weight Event System is a framework for allowing the
 * exchange of information from many machines to many machines in a
 * controlled, platform neutral, language neutral way. The exchange
 * of information is done in a connectless fashion using multicast
 * or unicast UDP, and using self describing data so that any
 * platform or language can translate it to it's local dialect.
 *
 *  This leads to a system with the following features
 *
 *    - Fire and Forget messaging
 *    - Decoupling of senders (emitters) from receivers (listeners, journallers)
 *    - Centerless, no single point of failure
 *    - Support for many to many communication where nodes can enter
 *      and leave the system at any time.
 *    - Computer language and Hardware Platform independent 
 *
 *  The system consists of two major components
 *
 *    Emitter - a source of events, this is usually a server or process
 *              which wants to announce something
 *
 *    Listener - a sink of events which deserializes and processes events
 *               on the fly as they are received.
 *
 *  There is also a custom listener which defers deserialization until some
 *  point in the future, and simply captures and writes the events to a log.
 *  This is in the pj package, and is called a journaller.
 *
 *  This library is the 'C' implementations of the LWES.  There is also a
 *  perl xsubs wrapper, and a pure java implementation packaged separately.
 *  All should be wire format compatible.
 *
 *  For more information on developing with the LWES see
 *  http://www.lwes.org
 */

/* Now here is the documentat for this file */

/*! \file lwes_types.h
 *  \brief Definitions for all types used in lwes
 */

/*! \typedef unsigned char LWES_BYTE
 *  \brief 1 byte
 */
typedef unsigned char      LWES_BYTE;

/*! \typedef unsigned char * LWES_BYTE_P
 *  \brief Pointer to a byte, aka a byte array
 */
typedef unsigned char *    LWES_BYTE_P;

/*! \typedef unsigned short int LWES_U_INT_16
 *  \brief 2 byte unsigned integer
 */
typedef unsigned short int LWES_U_INT_16;

/*! \typedef short int LWES_INT_16
 *  \brief 2 byte signed integer
 */
typedef short int          LWES_INT_16;

/*! \typedef unsigned int LWES_U_INT_32
 *  \brief 4 byte unsigned integer
 */
typedef unsigned int       LWES_U_INT_32;

/*! \typedef int LWES_INT_32
 *  \brief 4 byte signed integer
 */
typedef int                LWES_INT_32;

/*! \typedef unsigned long long LWES_U_INT_64
 *  \brief 8 byte unsigned integer
 */
typedef unsigned long long LWES_U_INT_64;

/*! \typedef int LWES_INT_64
 *  \brief 8 byte signed integer
 */
typedef long long          LWES_INT_64;

/*! \typedef int LWES_BOOLEAN
 *  \brief boolean value should be either 0 (false) or 1 (true), this is
 *         actually stored as a 1 byte value
 */
typedef int                LWES_BOOLEAN;

/*! \typedef struct in_addr LWES_IP_ADDR 
 *  \brief an ipv4 internet address, which is 4 bytes in length
 */
typedef struct in_addr     LWES_IP_ADDR;

/*! \typedef char *             LWES_SHORT_STRING
 *  \brief a string which is at most 1 byte in length
 */
typedef char *             LWES_SHORT_STRING;

/*! \typedef char *             LWES_LONG_STRING
 *  \brief a string which is at most 2 bytes in length
 */
typedef char *             LWES_LONG_STRING;

/*! \typedef const char *  LWES_CONST_SHORT_STRING
 *  \brief a constant string which is at most 1 byte in length
 */
typedef const char *       LWES_CONST_SHORT_STRING;

/*! \typedef const char * LWES_CONST_LONG_STRING
 *  \brief a constant string which is at most 2 bytes in length
 */
typedef const char *       LWES_CONST_LONG_STRING;

/*! \typedef char  LWES_CHAR
 *  \brief a character which is mainly used in mallocs for string types
 */
typedef char               LWES_CHAR;

/* Maximums of each type in characters for a standard encoding */
/* WARNING, WARNING
   These #define's should be considered deprecated, they will be removed
   in a future version of lwes */
#define SHORT_STRING_MAX    255   /*                                          */
#define LONG_STRING_MAX     65535 /*                                          */
#define BYTE_STRING_MAX     2     /*                   00 - FF                */
#define U_INT_16_STRING_MAX 5     /*                    0 - 65535             */
#define INT_16_STRING_MAX   6     /*               -32768 - 32767             */
#define U_INT_32_STRING_MAX 10    /*                    0 - 4294967295        */
#define INT_32_STRING_MAX   11    /*          -2147483648 - 2147483647        */
#define U_INT_64_STRING_MAX 20    /*                  0 - 18446744073709551615*/
#define INT_64_STRING_MAX   20    /*-9223372036854775808 - 9223372036854775807*/
#define BOOLEAN_STRING_MAX  5     /*                 true - false             */
#define IP_ADDR_STRING_MAX  15    /*               xxx.xxx.xxx.xxx            */

#define MAX_HEADER_LENGTH   22
#define MAX_BODY_LENGTH     65513  /* 65535 - 22                              */

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
extern const size_t MAX_QUEUED_ELEMENTS;
extern const size_t MAX_MSG_SIZE;

/* TODO: these really need to go away and be replaced with the type enumeration
   below, until then they need to remain in-sync.
 */
extern const LWES_BYTE LWES_UNDEFINED_TOKEN;
extern const LWES_BYTE LWES_U_INT_16_TOKEN;
extern const LWES_BYTE LWES_INT_16_TOKEN;
extern const LWES_BYTE LWES_U_INT_32_TOKEN;
extern const LWES_BYTE LWES_INT_32_TOKEN;
extern const LWES_BYTE LWES_U_INT_64_TOKEN;
extern const LWES_BYTE LWES_INT_64_TOKEN;
extern const LWES_BYTE LWES_BOOLEAN_TOKEN;
extern const LWES_BYTE LWES_IP_ADDR_TOKEN;
extern const LWES_BYTE LWES_STRING_TOKEN;

extern const LWES_SHORT_STRING LWES_UNDEFINED_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_16_STRING;
extern const LWES_SHORT_STRING LWES_INT_16_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_32_STRING;
extern const LWES_SHORT_STRING LWES_INT_32_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_64_STRING;
extern const LWES_SHORT_STRING LWES_INT_64_STRING;
extern const LWES_SHORT_STRING LWES_BOOLEAN_STRING;
extern const LWES_SHORT_STRING LWES_IP_ADDR_STRING;
extern const LWES_SHORT_STRING LWES_STRING_STRING;

extern const LWES_SHORT_STRING LWES_META_INFO_STRING;

/*! \enum An enumeration of the types used by lwes */
typedef enum {
    LWES_TYPE_U_INT_16  = 1,   /*!< 2 byte unsigned integer type */
    LWES_TYPE_INT_16    = 2,   /*!< 2 byte signed integer type type */ 
    LWES_TYPE_U_INT_32  = 3,   /*!< 4 byte unsigned integer type */
    LWES_TYPE_INT_32    = 4,   /*!< 4 byte signed integer type */
    LWES_TYPE_STRING    = 5,   /*!< variable bytes string type */
    LWES_TYPE_IP_ADDR   = 6,   /*!< 4 byte ipv4 address type */
    LWES_TYPE_INT_64    = 7,   /*!< 8 byte signed integer type */
    LWES_TYPE_U_INT_64  = 8,   /*!< 8 byte unsigned integer type */
    LWES_TYPE_BOOLEAN   = 9,   /*!< 1 byte boolean type */
    LWES_TYPE_UNDEFINED = 255, /*!< undefined type */
} LWES_TYPE;

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_TYPES_H */
