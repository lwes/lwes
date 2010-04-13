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

/*! \typedef struct in_addr LWES_IPV4
 *  \brief an ipv4 internet address, which is 4 bytes in length
 */
typedef struct in_addr     LWES_IPV4;

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

/*! \typedef float       LWES_FLOAT
 *  \brief an ieee 754 single precision floating point value
 */
typedef float              LWES_FLOAT;

/*! \typedef double      LWES_DOUBLE
 *  \brief an ieee 754 double precision floating point value
 */
typedef double             LWES_DOUBLE;

/*! \typedef char  LWES_CHAR
 *  \brief a character which is mainly used in mallocs for string types
 */
typedef char               LWES_CHAR;

/* maximum payload size for an event system message payload */
extern const size_t LWES_MSG_SIZE_MAX;

/* maximum sizes of different types of strings in the system */
#define LWES_EVENT_NAME_MAX 127
#define LWES_STRING_MAX     65535

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
//    LWES_TYPE_BYTE      = 10,  /*!< 1 byte */
//    LWES_TYPE_FLOAT     = 11,  /*!< 4 byte ieee 754 single precision float */
//    LWES_TYPE_DOUBLE    = 12,  /*!< 8 byte ieee 754 double precision flaot */
//    LWES_TYPE_IPV4      = 13,  /*!< 4 byte ipv4 address type */
//    LWES_TYPE_EVENT     = 14,  /*!< event type */
    LWES_TYPE_UNDEFINED = 255, /*!< undefined type */
} LWES_TYPE;

#ifdef __cplusplus
}
#endif 

#endif /* __LWES_TYPES_H */
