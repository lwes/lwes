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
#include "lwes_types.h"

/* maximum datagram size for UDP is 64K minus IP layer overhead which is
   20 bytes for IP header, and 8 bytes for UDP header, so this value
   should be

   65535 - 28 = 65507
 */
const size_t MAX_MSG_SIZE = 65507;
const size_t MAX_QUEUED_ELEMENTS = 10000;

/* TODO: these really need to go away and be replaced with the type enumeration
   in the header, until then they need to remain in-sync.
 */
const LWES_BYTE LWES_UNDEFINED_TOKEN= (LWES_BYTE)0xff;
const LWES_BYTE LWES_U_INT_16_TOKEN = (LWES_BYTE)0x01;
const LWES_BYTE LWES_INT_16_TOKEN   = (LWES_BYTE)0x02;
const LWES_BYTE LWES_U_INT_32_TOKEN = (LWES_BYTE)0x03;
const LWES_BYTE LWES_INT_32_TOKEN   = (LWES_BYTE)0x04;
const LWES_BYTE LWES_STRING_TOKEN   = (LWES_BYTE)0x05;
const LWES_BYTE LWES_IP_ADDR_TOKEN  = (LWES_BYTE)0x06;
const LWES_BYTE LWES_INT_64_TOKEN   = (LWES_BYTE)0x07;
const LWES_BYTE LWES_U_INT_64_TOKEN = (LWES_BYTE)0x08;
const LWES_BYTE LWES_BOOLEAN_TOKEN  = (LWES_BYTE)0x09;

const LWES_SHORT_STRING LWES_UNDEFINED_STRING=(LWES_SHORT_STRING)"undef";
const LWES_SHORT_STRING LWES_U_INT_16_STRING =(LWES_SHORT_STRING)"uint16";
const LWES_SHORT_STRING LWES_INT_16_STRING   =(LWES_SHORT_STRING)"int16";
const LWES_SHORT_STRING LWES_U_INT_32_STRING =(LWES_SHORT_STRING)"uint32";
const LWES_SHORT_STRING LWES_INT_32_STRING   =(LWES_SHORT_STRING)"int32";
const LWES_SHORT_STRING LWES_STRING_STRING   =(LWES_SHORT_STRING)"string";
const LWES_SHORT_STRING LWES_IP_ADDR_STRING  =(LWES_SHORT_STRING)"ip_addr";
const LWES_SHORT_STRING LWES_INT_64_STRING   =(LWES_SHORT_STRING)"int64";
const LWES_SHORT_STRING LWES_U_INT_64_STRING =(LWES_SHORT_STRING)"uint64";
const LWES_SHORT_STRING LWES_BOOLEAN_STRING  =(LWES_SHORT_STRING)"boolean";


const LWES_SHORT_STRING LWES_META_INFO_STRING=(LWES_SHORT_STRING)"MetaEventInfo";
