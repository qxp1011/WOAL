/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        OFPMessageUtils.h
 * Description:      OpenFlow message utilities header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-12
 *
 * History 1:
 *     Date:         2013-12-21
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/
#ifndef OFP_MESSAGE_UTILS_H
#define OFP_MESSAGE_UTILS_H

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"

#ifndef ETH_ADDR_LENTH
#define ETH_ADDR_LENTH (6)
#endif

#define ETH_TYPE_IP            0x0800
#define ETH_TYPE_ARP           0x0806
#define ETH_TYPE_VLAN_8021Q    0x8100
#define ETH_TYPE_VLAN          ETH_TYPE_VLAN_8021Q
#define ETH_TYPE_VLAN_8021AD   0x88a8
#define ETH_TYPE_IPV6          0x86dd
#define ETH_TYPE_LACP          0x8809
#define ETH_TYPE_RARP          0x8035
#define ETH_TYPE_MPLS          0x8847
#define ETH_TYPE_MPLS_MCAST    0x8848

// copied from <netinet/icmp6.h>
#define ND_ROUTER_SOLICIT           133
#define ND_ROUTER_ADVERT            134
#define ND_NEIGHBOR_SOLICIT         135
#define ND_NEIGHBOR_ADVERT          136
#define ND_REDIRECT                 137

typedef long CmResult;

/*********************** ASSERT ************************/
#ifdef CM_DISABLE_ASSERT
#undef CM_DISABLE_ASSERT
#endif

#ifdef CM_DISABLE_ASSERT
#define CM_ASSERT_RETURN(expr, rv) \
  do { \
    if (!(expr)) { \
        ACE_ERROR((LM_ERROR, ACE_TEXT("%s:%u Assert failed: %s\n"), __FILE__, __LINE__, #expr)); \
        return rv; \
    } \
  } while (0)

#define CM_ASSERT_RETURN_VOID(expr) \
do { \
    if (!(expr)) { \
        ACE_ERROR((LM_ERROR, ACE_TEXT("%s:%u Assert failed: %s\n"), __FILE__, __LINE__, #expr)); \
        return; \
    } \
} while (0)
#else
#define CM_ASSERT_RETURN(expr, rv) \
  do { \
      ACE_ASSERT((expr)); \
      if (!(expr)) { \
          ACE_ERROR((LM_ERROR, ACE_TEXT("%s:%u Assert failed: %s\n"), __FILE__, __LINE__, #expr)); \
          return rv; \
      } \
  } while (0)

#define CM_ASSERT_RETURN_VOID(expr) \
  do { \
      ACE_ASSERT((expr)); \
      if (!(expr)) { \
          ACE_ERROR((LM_ERROR, ACE_TEXT("%s:%u Assert failed: %s\n"), __FILE__, __LINE__, #expr)); \
          return; \
      } \
  } while (0)
#endif // CM_DISABLE_ASSERT

class COFPMsgMemoryMgr
{
public:
    static CmResult Copy(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_CDR::Octet &abDesUsedMemSize,
                         const ACE_CDR::Octet *apSrc, ACE_CDR::Octet abSrcSize);
    static CmResult Copy(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize,
                         const ACE_CDR::Octet *apSrc, ACE_UINT16 awSrcSize);

    /* Decode paddings with the specified length. */
    static CmResult PadStreamFrom(ACE_InputCDR &aIs, ACE_UINT16 aPadLen);
    /* Encode paddings with the specified length. */
    static CmResult PadStreamTo(ACE_OutputCDR &aOs, ACE_UINT16 aPadLen);

    /* Decode data from the specified stream. If the memory is insufficient, reallocate it. */
    static CmResult DataStreamFrom(std::allocator<ACE_CDR::Octet> &aAlloc, 
                                   ACE_InputCDR &aIs, 
                                   ACE_UINT16 awDataSize,
                                   ACE_CDR::Octet *&apDes, 
                                   ACE_UINT16 &awDesMemSize, 
                                   ACE_UINT16 &awDesUsedMemSize);
};

class COFPMsgCommonUtils
{
public:
    #define OFP_MSG_UTILS_BYTE_MAX_VALUE ((ACE_CDR::Octet)(0xff))
    /* Returns true if the 'n' bytes starting at 'p' are zeros. */
    static bool is_all_zeros(const ACE_CDR::Octet *p, size_t n);   
    
    /* Returns true if the 'n' bytes starting at 'p' are 0xff. */
    static bool is_all_ones(const ACE_CDR::Octet *p, size_t n);
    
    /* Given the wildcard bit count in the least-significant 6 of 'wcbits', returns
     * an IP netmask with a 1 in each bit that must match and a 0 in each bit that
     * is wildcarded.
     *
     * The bits in 'wcbits' are in the format used in enum ofp_flow_wildcards: 0
     * is exact match, 1 ignores the LSB, 2 ignores the 2 least-significant bits,
     * ..., 32 and higher wildcard the entire field.  This is the *opposite* of the
     * usual convention where e.g. /24 indicates that 8 bits (not 24 bits) are
     * wildcarded. */
    static ACE_UINT32 ofputil_wcbits_to_netmask(int wcbits);
    static ACE_CDR::Octet     CalcIpOneBitNum(ACE_UINT32 dwIpAddr);

    static bool     eth_addr_is_zero(const ACE_CDR::Octet ea[ETH_ADDR_LENTH]);
    static int      eth_mask_is_exact(const ACE_CDR::Octet ea[ETH_ADDR_LENTH]);

    static bool     ipv6_addr_is_zero(const ACE_CDR::Octet addr[16]);
    static int      ipv6_mask_is_exact(const ACE_CDR::Octet addr[16]);

    static bool     array_is_zero(const ACE_CDR::Octet *apMask, ACE_UINT16 awArraySize);

    //static bool     is_ip_any(FLOWKEY *key);
    static bool     eth_type_mpls(ACE_UINT16 eth_type);
};

/* Given the wildcard bit count in the least-significant 6 of 'wcbits', returns
 * an IP netmask with a 1 in each bit that must match and a 0 in each bit that
 * is wildcarded.
 *
 * The bits in 'wcbits' are in the format used in enum ofp_flow_wildcards: 0
 * is exact match, 1 ignores the LSB, 2 ignores the 2 least-significant bits,
 * ..., 32 and higher wildcard the entire field.  This is the *opposite* of the
 * usual convention where e.g. /24 indicates that 8 bits (not 24 bits) are
 * wildcarded. */
inline ACE_UINT32 COFPMsgCommonUtils::ofputil_wcbits_to_netmask(int wcbits)
{
    wcbits &= 0x3f;
    return wcbits < 32 ? (~((1u << wcbits) - 1)) : 0;
}

inline bool COFPMsgCommonUtils::eth_addr_is_zero(const ACE_CDR::Octet ea[ETH_ADDR_LENTH])
{
    return !(ea[0] | ea[1] | ea[2] | ea[3] | ea[4] | ea[5]);
}

inline int COFPMsgCommonUtils::eth_mask_is_exact(const ACE_CDR::Octet ea[ETH_ADDR_LENTH])
{
    return (ea[0] & ea[1] & ea[2] & ea[3] & ea[4] & ea[5]) == OFP_MSG_UTILS_BYTE_MAX_VALUE;
}

/*inline bool COFPMsgCommonUtils::is_ip_any(FLOWKEY *key)
{
    return ((ETH_TYPE_IP == key->eth_type)
            || (ETH_TYPE_IPV6 == key->eth_type));
}*/

inline bool COFPMsgCommonUtils::eth_type_mpls(ACE_UINT16 eth_type)
{
    return ((ETH_TYPE_MPLS == eth_type)
            || (ETH_TYPE_MPLS_MCAST == eth_type));
}

#endif


