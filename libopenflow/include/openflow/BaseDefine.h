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

#ifndef __BASE_DEFINES_H__
#define __BASE_DEFINES_H__
//#include <linux/types.h>
//#include <sys/types.h>
#include <stdint.h>

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <queue>
#include <strstream>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <functional>
#include <assert.h>
#include <limits.h>
#include "ace/Basic_Types.h"

using namespace std;

#define VERSION "0.0.0"

namespace openflow {

template<class T> inline T wpl_min(T a, T b) { return (a > b) ? b : a; }
template<class T> inline T wpl_max(T a, T b) { return (a < b) ? b : a; }
#define WPL_BIT_ENABLED(dword, bit) (((dword) & (bit)) != 0)
#define WPL_BIT_DISABLED(dword, bit) (((dword) & (bit)) == 0)
#define WPL_BIT_CMP_MASK(dword, bit, mask) (((dword) & (bit)) == mask)
#define WPL_SET_BITS(dword, bits) (dword |= (bits))
#define WPL_CLR_BITS(dword, bits) (dword &= ~(bits))


#if defined (WPL_DEBUG)
    #define WPL_ASSERT assert
#else
    #define WPL_ASSERT(x) (void)0
#endif

#define WF_ARRAY_SIZE(x) (static_cast<int>((sizeof(x)/sizeof(x[0]))))


#define strnicmp(x,y,n) strncasecmp(x,y,n)
#define stricmp(x,y) strcasecmp(x,y)

#define uint32 uint32_t
#define uint16 uint16_t
#define uint8 uint8_t

#define WPL_BIT_ENABLED(dword, bit) (((dword) & (bit)) != 0)
#define WPL_BIT_DISABLED(dword, bit) (((dword) & (bit)) == 0)
#define WPL_BIT_CMP_MASK(dword, bit, mask) (((dword) & (bit)) == mask)
#define WPL_SET_BITS(dword, bits) (dword |= (bits))
#define WPL_CLR_BITS(dword, bits) (dword &= ~(bits))

#ifdef __CHECKER__
#define WPL_BITWISE __attribute__((bitwise))
#define WPL_FORCE __attribute__((force))
#else
#define WPL_BITWISE
#define WPL_FORCE
#endif

/* The ovs_be<N> types indicate that an object is in big-endian, not
 * native-endian, byte order.  They are otherwise equivalent to uint<N>_t.
 *
 * We bootstrap these from the Linux __be<N> types.  If we instead define our
 * own independently then __be<N> and ovs_be<N> become mutually
 * incompatible. */
//typedef __be16 wpl_be16;
//typedef __be32 wpl_be32;
//typedef __be64 wpl_be64;

/* Netlink and OpenFlow both contain 64-bit values that are only guaranteed to
 * be aligned on 32-bit boundaries.  These types help.
 *
 * lib/unaligned.h has helper functions for accessing these. */

/* A 64-bit value, in host byte order, that is only aligned on a 32-bit
 * boundary.  */
typedef struct {
#ifdef __BIG_ENDIAN__
        ACE_UINT32 hi, lo;
#else
        ACE_UINT32 lo, hi;
#endif
} wpl_32aligned_u64;

/* A 64-bit value, in network byte order, that is only aligned on a 32-bit
 * boundary. */
typedef struct {
        ACE_UINT32 hi, lo;
} wpl_32aligned_be64;

typedef struct {
#ifdef __BIG_ENDIAN__
        ACE_UINT16 hi, lo;
#else
        ACE_UINT16 lo, hi;
#endif
} wpl_16aligned_u32;

/* A 32-bit value, in network byte order, that is only aligned on a 16-bit
 * boundary. */
typedef struct {
        ACE_UINT16 hi, lo;
} wpl_16aligned_be32;


} // namespace openflow

#define TYPE_IS_INTEGER(TYPE) ((TYPE) 1.5 == (TYPE) 1)
#define TYPE_IS_SIGNED(TYPE) ((TYPE) 1 > (TYPE) -1)
#define TYPE_VALUE_BITS(TYPE) (sizeof(TYPE) * CHAR_BIT - TYPE_IS_SIGNED(TYPE))
#define TYPE_MINIMUM(TYPE) (TYPE_IS_SIGNED(TYPE) \
                            ? ~(TYPE)0 << TYPE_VALUE_BITS(TYPE) \
                            : 0)
#define TYPE_MAXIMUM(TYPE) (TYPE_IS_SIGNED(TYPE) \
                            ? ~(~(TYPE)0 << TYPE_VALUE_BITS(TYPE)) \
                            : (TYPE)-1)

/* Number of decimal digits required to format an integer of the given TYPE.
 * Includes space for a sign, if TYPE is signed, but not for a null
 * terminator.
 *
 * The value is an overestimate. */
#define INT_STRLEN(TYPE) (TYPE_IS_SIGNED(TYPE) + TYPE_VALUE_BITS(TYPE) / 3 + 1)


#endif/*__BASE_DEFINES_H__*/

