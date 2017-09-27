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
#include <stdio.h>
#include <stdint.h>

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
 *  http://lwes.github.io
 */

/* Now here is the documentation for this file */

/*! \file lwes_types.h
 *  \brief Definitions for all types used in lwes
 */

/*! \typedef unsigned char LWES_BYTE
 *  \brief 1 byte
 */
typedef uint8_t            LWES_BYTE;

/*! \typedef unsigned char * LWES_BYTE_P
 *  \brief Pointer to a byte, aka a byte array
 */
typedef uint8_t *          LWES_BYTE_P;

/*! \typedef unsigned short int LWES_U_INT_16
 *  \brief 2 byte unsigned integer
 */
typedef uint16_t           LWES_U_INT_16;

/*! \typedef short int LWES_INT_16
 *  \brief 2 byte signed integer
 */
typedef int16_t            LWES_INT_16;

/*! \typedef unsigned int LWES_U_INT_32
 *  \brief 4 byte unsigned integer
 */
typedef uint32_t           LWES_U_INT_32;

/*! \typedef int LWES_INT_32
 *  \brief 4 byte signed integer
 */
typedef int32_t            LWES_INT_32;

/*! \typedef unsigned long long LWES_U_INT_64
 *  \brief 8 byte unsigned integer
 */
typedef uint64_t           LWES_U_INT_64;

/*! \typedef int LWES_INT_64
 *  \brief 8 byte signed integer
 */
typedef int64_t            LWES_INT_64;

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
 *  \brief a string with length that can be stored in at most 1 byte
 */
typedef char *             LWES_SHORT_STRING;

/*! \typedef char *             LWES_LONG_STRING
 *  \brief a string with length that can be stored in at most 2 bytes
 */
typedef char *             LWES_LONG_STRING;

/*! \typedef const char *  LWES_CONST_SHORT_STRING
 *  \brief a constant string with length that can be stored in at most 1 byte
 */
typedef const char *       LWES_CONST_SHORT_STRING;

/*! \typedef const char * LWES_CONST_LONG_STRING
 *  \brief a constant string with length that can be stored in at most 1 byte
 */
typedef const char *       LWES_CONST_LONG_STRING;

/*! \typedef char  LWES_CHAR
 *  \brief a character which is mainly used in mallocs for string types
 */
typedef char               LWES_CHAR;

/*! \typedef int LWES_FLOAT
 *  \brief 4 byte single-precision floating-point value
 */
typedef float              LWES_FLOAT;

/*! \typedef int LWES_DOUBLE
 *  \brief 8 byte single-precision floating-point value
 */
typedef double             LWES_DOUBLE;

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


extern const LWES_SHORT_STRING LWES_UNDEFINED_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_16_STRING;
extern const LWES_SHORT_STRING LWES_INT_16_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_32_STRING;
extern const LWES_SHORT_STRING LWES_INT_32_STRING;
extern const LWES_SHORT_STRING LWES_STRING_STRING;
extern const LWES_SHORT_STRING LWES_IP_ADDR_STRING;
extern const LWES_SHORT_STRING LWES_INT_64_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_64_STRING;
extern const LWES_SHORT_STRING LWES_BOOLEAN_STRING;
extern const LWES_SHORT_STRING LWES_BYTE_STRING;
extern const LWES_SHORT_STRING LWES_FLOAT_STRING;
extern const LWES_SHORT_STRING LWES_DOUBLE_STRING;
extern const LWES_SHORT_STRING LWES_LONG_STRING_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_16_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_INT_16_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_32_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_INT_32_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_INT_64_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_U_INT_64_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_STRING_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_IP_ADDR_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_BOOLEAN_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_BYTE_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_FLOAT_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_DOUBLE_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_U_INT_16_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_INT_16_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_U_INT_32_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_INT_32_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_STRING_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_INT_64_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_U_INT_64_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_BOOLEAN_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_BYTE_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_FLOAT_ARRAY_STRING;
extern const LWES_SHORT_STRING LWES_N_DOUBLE_ARRAY_STRING;


extern const LWES_SHORT_STRING LWES_META_INFO_STRING;

/*! \enum An enumeration of the types used by lwes */
typedef enum {
    LWES_TYPE_U_INT_16          = 1,   /*!< 2 byte unsigned integer type */
    LWES_TYPE_INT_16            = 2,   /*!< 2 byte signed integer type type */
    LWES_TYPE_U_INT_32          = 3,   /*!< 4 byte unsigned integer type */
    LWES_TYPE_INT_32            = 4,   /*!< 4 byte signed integer type */
    LWES_TYPE_STRING            = 5,   /*!< variable bytes string type */
    LWES_TYPE_IP_ADDR           = 6,   /*!< 4 byte ipv4 address type */
    LWES_TYPE_INT_64            = 7,   /*!< 8 byte signed integer type */
    LWES_TYPE_U_INT_64          = 8,   /*!< 8 byte unsigned integer type */
    LWES_TYPE_BOOLEAN           = 9,   /*!< 1 byte boolean type */
    LWES_TYPE_BYTE              = 10,  /*!< 1 byte byte type */
    LWES_TYPE_FLOAT             = 11,  /*!< 4 byte floating point type */
    LWES_TYPE_DOUBLE            = 12,  /*!< 8 byte double-precision floating point type */
    /* linear array types */
    LWES_TYPE_U_INT_16_ARRAY    = 129,
    LWES_TYPE_INT_16_ARRAY      = 130,
    LWES_TYPE_U_INT_32_ARRAY    = 131,
    LWES_TYPE_INT_32_ARRAY      = 132,
    LWES_TYPE_STRING_ARRAY      = 133,
    LWES_TYPE_IP_ADDR_ARRAY     = 134,
    LWES_TYPE_INT_64_ARRAY      = 135,
    LWES_TYPE_U_INT_64_ARRAY    = 136,
    LWES_TYPE_BOOLEAN_ARRAY     = 137,
    LWES_TYPE_BYTE_ARRAY        = 138,
    LWES_TYPE_FLOAT_ARRAY       = 139,
    LWES_TYPE_DOUBLE_ARRAY      = 140,
    /* "nullable" (sparse) array types */
//    LWES_TYPE_N_U_INT_16_ARRAY  = 141,
//    LWES_TYPE_N_INT_16_ARRAY    = 142,
//    LWES_TYPE_N_U_INT_32_ARRAY  = 143,
//    LWES_TYPE_N_INT_32_ARRAY    = 144,
//    LWES_TYPE_N_STRING_ARRAY    = 145,
//    LWES_TYPE_N_INT_64_ARRAY    = 147,
//    LWES_TYPE_N_U_INT_64_ARRAY  = 148,
//    LWES_TYPE_N_BOOLEAN_ARRAY   = 149,
//    LWES_TYPE_N_BYTE_ARRAY      = 150,
//    LWES_TYPE_N_FLOAT_ARRAY     = 151,
//    LWES_TYPE_N_DOUBLE_ARRAY    = 152,
    LWES_TYPE_UNDEFINED         = 255, /*!< undefined type */
} LWES_TYPE;

LWES_CONST_SHORT_STRING
lwes_type_to_string
  (LWES_TYPE type);

LWES_TYPE
lwes_string_to_type
  (LWES_SHORT_STRING type_str);

LWES_BOOLEAN
lwes_type_is_array
  (LWES_TYPE typ);

LWES_BOOLEAN
lwes_type_is_nullable_array
  (LWES_TYPE typ);

LWES_TYPE
lwes_array_type_to_base
  (LWES_TYPE typ);

/* returns the unit-size for type typ
 * NOTE: for strings it returns the pointer-size */
int
lwes_type_to_size
  (LWES_TYPE type);

int
lwes_typed_value_to_stream
  (LWES_TYPE type,
   void* value,
   FILE *stream);

int
lwes_typed_array_to_stream
  (LWES_TYPE type,
   void* value,
   int size,
   FILE *stream);

struct lwes_event_attribute;

int
lwes_event_attribute_to_stream
  (struct lwes_event_attribute *attribute,
   FILE *stream);

#ifdef __cplusplus
}
#endif

#endif /* __LWES_TYPES_H */
