/***********************************************************************
 * Copyright (C) 2013, Nanjing WFN Technology Co., Ltd 
 * Description: 
 * Others:
 * Version:          V1.0
 * Author:           Yi Jian
 * Date:             2013-09-12
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/

#ifndef _BYTEORDER_H__
#define _BYTEORDER_H__

//#include <netinet/in.h>
#include "BaseDefine.h"

namespace openflow {


#ifndef __CHECKER__
static inline uint64_t
htonll(uint64_t n)
{
    return htonl(1) == 1 ? n : ((uint64_t) htonl(n) << 32) | htonl(n >> 32);
}

static inline uint64_t
ntohll(uint64_t n)
{
    return htonl(1) == 1 ? n : ((uint64_t) ntohl(n) << 32) | ntohl(n >> 32);
}
#else
/* Making sparse happy with these functions also makes them unreadable, so
 * don't bother to show it their implementations. */
uint64_t htonll(uint64_t);
uint64_t ntohll(uint64_t);
#endif

/* These macros may substitute for htons(), htonl(), and htonll() in contexts
 * where function calls are not allowed, such as case labels.  They should not
 * be used elsewhere because all of them evaluate their argument many times. */
#if defined(__BIG_ENDIAN__) || __CHECKER__
#define CONSTANT_HTONS(VALUE) ((WPL_FORCE ACE_UINT16) ((VALUE) & 0xffff))
#define CONSTANT_HTONL(VALUE) ((WPL_FORCE ACE_UINT32) ((VALUE) & 0xffffffff))
#define CONSTANT_HTONLL(VALUE) \
        ((WPL_FORCE ACE_UINT64) ((VALUE) & UINT64_C(0xffffffffffffffff)))
#else
#define CONSTANT_HTONS(VALUE)                       \
        (((((ACE_UINT16) (VALUE)) & 0xff00) >> 8) |   \
         ((((ACE_UINT16) (VALUE)) & 0x00ff) << 8))
#define CONSTANT_HTONL(VALUE)                           \
        (((((ACE_UINT32) (VALUE)) & 0x000000ff) << 24) |  \
         ((((ACE_UINT32) (VALUE)) & 0x0000ff00) <<  8) |  \
         ((((ACE_UINT32) (VALUE)) & 0x00ff0000) >>  8) |  \
         ((((ACE_UINT32) (VALUE)) & 0xff000000) >> 24))
#define CONSTANT_HTONLL(VALUE)                                           \
        (((((ACE_UINT64) (VALUE)) & UINT64_C(0x00000000000000ff)) << 56) | \
         ((((ACE_UINT64) (VALUE)) & UINT64_C(0x000000000000ff00)) << 40) | \
         ((((ACE_UINT64) (VALUE)) & UINT64_C(0x0000000000ff0000)) << 24) | \
         ((((ACE_UINT64) (VALUE)) & UINT64_C(0x00000000ff000000)) <<  8) | \
         ((((ACE_UINT64) (VALUE)) & UINT64_C(0x000000ff00000000)) >>  8) | \
         ((((ACE_UINT64) (VALUE)) & UINT64_C(0x0000ff0000000000)) >> 24) | \
         ((((ACE_UINT64) (VALUE)) & UINT64_C(0x00ff000000000000)) >> 40) | \
         ((((ACE_UINT64) (VALUE)) & UINT64_C(0xff00000000000000)) >> 56))
#endif


inline ACE_UINT16 HostToNetwork16(ACE_UINT16 n) {
  return htons(n);
}

inline ACE_UINT32 HostToNetwork32(ACE_UINT32 n) {
  return htonl(n);
}

inline uint64_t HostToNetwork64(uint64_t n) {
  return htonll(n);
}

inline ACE_UINT16 NetworkToHost16(ACE_UINT16 n) {
  return ntohs(n);
}

inline ACE_UINT32 NetworkToHost32(ACE_UINT32 n) {
  return ntohl(n);
}

inline uint64_t NetworkToHost64(uint64_t n) {
    return ntohll(n);
}

}  // namespace openflow

#endif//_BYTEORDER_H__


