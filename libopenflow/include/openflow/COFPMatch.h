/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMatch.h
 * Description:      OpenFlow match header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFP_MATCH_H
#define COFP_MATCH_H

#include "IOFPMsgParser.h"
#include "OFPMessageUtils.h"
#include "openflow-common.h"
//#include "COpenFlowMatch.h"

#if 0
// Interface for transforming between COFPXXMatch and OpenFlowMatch.
class IOFPMatch
{
public:
    typedef struct in6_addr ofp_in6_addr;
    #define OFP_MATCH_VLAN_PCP_MASK     ((uint8_t)0x07)
    #define OFP_MATCH_IP_DSCP_MASK      ((uint8_t)0x3f)
    #define OFP_MATCH_MPLS_LABEL_MASK   ((uint32_t)0xfffff)
    #define OFP_MATCH_MPLS_TC_MASK      ((uint8_t)0x07)
    #define OFP_MATCH_MPLS_BOS_MASK     ((uint8_t)0x1)
    #define OFP_MATCH_PBB_ISID_MASK     ((uint32_t)0xffffff)
    #define OFP_MATCH_IPV6_EXTHDR_MASK  ((uint16_t)0x01ff)
    #define OFP_MATCH_IPV6_FLABEL_MASK  ((uint32_t)0xfffff)
    
    virtual ~IOFPMatch()    {};

    virtual CmResult ToOpenFlowMatch(COpenFlowMatch &aMatch)             = 0;
    virtual CmResult FromOpenFlowMatch(const COpenFlowMatch &aMatch)     = 0;
};
#endif

class COFP10Match : public IOFPMsgParser//, public IOFPMatch
{
public:
    /* Fields to match against flows */
    struct ofp_match 
    {
        ACE_UINT32 wildcards; /* Wildcard fields. */
        ACE_UINT16 in_port; /* Input switch port. */
        ACE_CDR::Octet dl_src[OFP_ETH_ALEN]; /* Ethernet source address. */
        ACE_CDR::Octet dl_dst[OFP_ETH_ALEN]; /* Ethernet destination address. */
        ACE_UINT16 dl_vlan; /* Input VLAN id. */
        ACE_CDR::Octet dl_vlan_pcp; /* Input VLAN priority. */
        ACE_CDR::Octet pad1[1]; /* Align to 64-bits */
        ACE_UINT16 dl_type; /* Ethernet frame type. */
        ACE_CDR::Octet nw_tos; /* IP ToS (actually DSCP field, 6 bits). */
        ACE_CDR::Octet nw_proto; /* IP protocol or lower 8 bits of  * ARP opcode. */
        ACE_CDR::Octet pad2[2]; /* Align to 64-bits */
        ACE_UINT32 nw_src; /* IP source address. */
        ACE_UINT32 nw_dst; /* IP destination address. */
        ACE_UINT16 tp_src; /* TCP/UDP source port. */
        ACE_UINT16 tp_dst; /* TCP/UDP destination port. */
    };

    static_assert(sizeof(ofp_match)== 40, "COFP10Match size is not what we expect for");

    /* Flow wildcards. */
    enum ofp_flow_wildcards {
        OFPFW_IN_PORT  = 1 << 0, /* Switch input port. */
        OFPFW_DL_VLAN  = 1 << 1, /* VLAN id. */
        OFPFW_DL_SRC   = 1 << 2, /* Ethernet source address. */
        OFPFW_DL_DST   = 1 << 3, /* Ethernet destination address. */
        OFPFW_DL_TYPE  = 1 << 4, /* Ethernet frame type. */
        OFPFW_NW_PROTO = 1 << 5, /* IP protocol. */
        OFPFW_TP_SRC   = 1 << 6, /* TCP/UDP source port. */
        OFPFW_TP_DST   = 1 << 7, /* TCP/UDP destination port. */
        /* IP source address wildcard bit count. 0 is exact match, 1 ignores the
         * LSB, 2 ignores the 2 least-significant bits, ..., 32 and higher wildcard
         * the entire field. This is the *opposite* of the usual convention where
         * e.g. /24 indicates that 8 bits (not 24 bits) are wildcarded. */
        OFPFW_NW_SRC_SHIFT = 8,
        OFPFW_NW_SRC_BITS  = 6,
        OFPFW_NW_SRC_MASK  = ((1 << OFPFW_NW_SRC_BITS) - 1) << OFPFW_NW_SRC_SHIFT,
        OFPFW_NW_SRC_ALL   = 32 << OFPFW_NW_SRC_SHIFT,
        /* IP destination address wildcard bit count. Same format as source. */
        OFPFW_NW_DST_SHIFT = 14,
        OFPFW_NW_DST_BITS  = 6,
        OFPFW_NW_DST_MASK  = ((1 << OFPFW_NW_DST_BITS) - 1) << OFPFW_NW_DST_SHIFT,
        OFPFW_NW_DST_ALL   = 32 << OFPFW_NW_DST_SHIFT,
        OFPFW_DL_VLAN_PCP  = 1 << 20, /* VLAN priority. */
        OFPFW_NW_TOS       = 1 << 21, /* IP ToS (DSCP field, 6 bits). */
        /* Wildcard all fields. */
        OFPFW_ALL          = ((1 << 22) - 1)
    };

public:
    COFP10Match()
    {
        ::memset(&m_tMember, 0, sizeof(m_tMember));
    }
    COFP10Match(struct ofp_match &aMatch)
    {
        ::memcpy(&m_tMember, &aMatch, sizeof(m_tMember));
    }
    virtual ~COFP10Match(){}

    ACE_UINT32 GetWildcards() const
    {
        return m_tMember.wildcards;
    }
    ACE_UINT16 GetInPort() const
    {
        return m_tMember.in_port;
    }
    const ACE_CDR::Octet *GetDlSrc() const
    {
        return m_tMember.dl_src;
    }
    const ACE_CDR::Octet *GetDlDst() const
    {
        return m_tMember.dl_dst;
    }
    ACE_UINT16 GetDlVlan() const
    {
        return m_tMember.dl_vlan;
    }
    ACE_CDR::Octet GetDlVlanPcp() const
    {
        return m_tMember.dl_vlan_pcp;
    }
    ACE_UINT16 GetDlType() const
    {
        return m_tMember.dl_type;
    }
    ACE_CDR::Octet GetNwTos() const
    {
        return m_tMember.nw_tos;
    }
    ACE_CDR::Octet GetNwProto() const
    {
        return m_tMember.nw_proto;
    }
    ACE_UINT32 GetNwSrc() const
    {
        return m_tMember.nw_src;
    }
    ACE_UINT32 GetNwDst() const
    {
        return m_tMember.nw_dst;
    }
    ACE_UINT16 GetTpSrc() const
    {
        return m_tMember.tp_src;
    }
    ACE_UINT16 GetTpDst() const
    {
        return m_tMember.tp_dst;
    }

    void SetWildcards(ACE_UINT32 adwWildcards)
    {
        m_tMember.wildcards = adwWildcards;
    }
    void SetInPort(ACE_UINT16 awInPort)
    {
        m_tMember.in_port = awInPort;
    }
    void SetDlSrc(const ACE_CDR::Octet aDlSrc[OFP_ETH_ALEN], const ACE_CDR::Octet aDlSrcMask[OFP_ETH_ALEN]);
    void SetDlSrc(const ACE_CDR::Octet aDlSrc[OFP_ETH_ALEN])
    {
        ::memcpy(m_tMember.dl_src, aDlSrc, OFP_ETH_ALEN);
    }
    void SetDLSrc(string &mac);

    void SetDlDst(const ACE_CDR::Octet aDlDst[OFP_ETH_ALEN], const ACE_CDR::Octet aDlDstMask[OFP_ETH_ALEN]);
    void SetDlDst(const ACE_CDR::Octet aDlDst[OFP_ETH_ALEN])
    {
        ::memcpy(m_tMember.dl_dst, aDlDst, OFP_ETH_ALEN);
    }
    void SetDlDst(string &mac);
    
    void SetDlVlan(ACE_UINT16 awVlan)
    {
        m_tMember.dl_vlan = awVlan;
    }
    void SetDlVlanPcp(ACE_CDR::Octet abVlanPcp)
    {
        m_tMember.dl_vlan_pcp = abVlanPcp;
    }
    void SetDlType(ACE_UINT16 awDlType)
    {
        m_tMember.dl_type = awDlType;
    }
    void SetNwTos(ACE_CDR::Octet abNwTos)
    {
        m_tMember.nw_tos = abNwTos;
    }
    void SetNwProto(ACE_CDR::Octet abNwProto)
    {
        m_tMember.nw_proto = abNwProto;
    }
    void SetNwSrc(ACE_UINT32 adwNwSrc)
    {
        m_tMember.nw_src = adwNwSrc;
    }
    void SetNwDst(ACE_UINT32 adwNwDst)
    {
        m_tMember.nw_dst = adwNwDst;
    }
    void SetTpSrc(ACE_UINT16 awTpSrc)
    {
        m_tMember.tp_src = awTpSrc;
    }
    void SetTpDst(ACE_UINT16 awTpDst)
    {
        m_tMember.tp_dst = awTpDst;
    }
    
    bool operator==(COFP10Match &aMatch)
    {
        return Compare(aMatch);
    }
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return sizeof(m_tMember);
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    #if 0
    // for interface IOFPMatch
    virtual CmResult ToOpenFlowMatch(COpenFlowMatch &aMatch);
    virtual CmResult FromOpenFlowMatch(const COpenFlowMatch &aMatch);
    #endif
    
private:
    bool Compare(COFP10Match &aMatch);
    //void ofputil_wildcard_from_ofpfw10(uint32_t ofpfw, COpenFlowMatchMask &wc);

private:
    struct ofp_match m_tMember;
};


class COFP13OxmTlv;

#define OFP13_MATCH_HEADLEN             (2 * sizeof(ACE_UINT16))
#define OFP13_MATCH_OXMTLV_HEADLEN      (4)

#define OFP_ETHTYPE_IPV4_PROT           (0x0800)
#define OFP_ETHTYPE_IPV6_PROT           (0x86dd)
#define OFP_ETHTYPE_ARP_PROT            (0x0806)
/* Nortel Provider Backbone Transport protocols,
   refer to http://standards.ieee.org/develop/regauth/ethertype/eth.txt */
#define OFP_ETHTYPE_NPBTP_PROT          (0x88e7)

#define OFP_TCP_TYPE                    (6)
#define OFP_UDP_TYPE                    (17)
#define OFP_SCTP_TYPE                   (132)
#define OFP_ICMPV4_TYPE                 (1)
#define OFP_ICMPV6_TYPE                 (58)

#define OFP_ONEBYTE_SIZE                (8)
#define OFP_SIZEOF_BYTE                 (sizeof(ACE_CDR::Octet))
#define OFP_SIZEOF_WORD16               (sizeof(ACE_UINT16))
#define OFP_SIZEOF_WORD32               (sizeof(ACE_UINT32))
#define OFP_SIZEOF_WORD64               (sizeof(ACE_UINT64))

// The mask in the OXM payload is as long as the value. So the total length is a double.
#define OFP_NUM_OF_COPIES_WITH_MASK     (2)

#define OXM_OF_IN_PORT_BITS             (32)
#define OXM_OF_IN_PHY_PORT_BITS         (32)
#define OXM_OF_METADATA_BITS            (64)
#define OXM_OF_ETH_ADDR_BITS            (48)
#define OXM_OF_ETH_TYPE_BITS            (16)
#define OXM_OF_ETH_TYPE_BYTES           (OFP_SIZEOF_WORD16)
#define OXM_OF_VLAN_VID_BITS            (13)
#define OXM_OF_VLAN_VID_ROUND_BYTE      (OFP_SIZEOF_WORD16)
#define OXM_OF_VLAN_PCP_BITS            (3)
#define OXM_OF_VLAN_PCP_ROUND_BYTE      (OFP_SIZEOF_BYTE)
#define OXM_OF_IP_DSCP_ROUND_BYTE       (OFP_SIZEOF_BYTE)
#define OXM_OF_IP_ECN_ROUND_BYTE        (OFP_SIZEOF_BYTE)
#define OXM_OF_IP_PROTO_BITS            (8)
#define OXM_OF_IPV4_ADDR_BITS           (32)
#define OXM_OF_IPV6_ADDR_BITS           (128)
#define OXM_OF_TCP_PORT_BITS            (16)
#define OXM_OF_UDP_PORT_BITS            (16)
#define OXM_OF_SCTP_PORT_BITS           (16)
#define OXM_OF_ICMPV4_BITS              (8)
#define OXM_OF_ICMPV6_BITS              (8)
#define OXM_OF_ARP_OP_BITS              (8)
#define OXM_OF_ARP_PA_BITS              (32)
#define OXM_OF_ARP_HA_BITS              (48)
#define OXM_OF_IPV6_FLABEL_BITS         (20)
#define OXM_OF_IPV6_FLABEL_ROUND_BYTE   (OFP_SIZEOF_WORD32)
#define OXM_OF_IPV6_ND_LL_BITS          (48)
#define OXM_OF_MPLS_LABEL_BITS          (20)
#define OXM_OF_MPLS_LABEL_ROUND_BYTES   (OFP_SIZEOF_WORD32)
#define OXM_OF_PBB_ISID_BITS            (24)
#define OXM_OF_PBB_ISID_ROUND_BYTES     (OFP_SIZEOF_WORD32)
#define OXM_OF_TUNNEL_ID_BITS           (64)
#define OXM_OF_IPV6_EXTHDR_BITS         (9)
#define OXM_OF_IPV6_EXTHDR_ROUND_BYTES  (OFP_SIZEOF_WORD16)


#define OFP_BYTE_MAX_VALUE              ((ACE_CDR::Octet)(0xff))
#define OFP_WORD16_MAX_VALUE            ((ACE_UINT16)(0xffff))
#define OFP_WORD32_MAX_VALUE            ((ACE_UINT32)(0xffffffff))
#define OFP_WORD64_MAX_VALUE            ((ACE_UINT64)(0xffffffffffffffff))

/* Fields to match against flows. 
    See A.2.3.1 Flow Match Header, openflow-spec-v1.3.1. 
   Notice: 
   Before setting the data member of COFP13Match objects, you should call COFP13Match(length) or 
SetMatchLen(length)
   to allocate memory for the field list. 
   NB: While setting, you should ensure pre-requisite are satisfied in Table~11: Match fields details. */
class COFP13Match : public IOFPMsgParser//, public IOFPMatch
{
public:
    /* The match type indicates the match structure (set of fields that compose the
        * match) in use. The match type is placed in the type field at the beginning
        * of all match structures. The "OpenFlow Extensible Match" type corresponds
        * to OXM TLV format described below and must be supported by all OpenFlow
        * switches. Extensions that define other match types may be published on the
        * ONF wiki. Support for extensions is optional.
        */
    enum ofp_match_type {
        OFPMT_STANDARD = 0, /* Deprecated. */
        OFPMT_OXM      = 1, /* OpenFlow Extensible Match */
    };

    /* OXM Class IDs.
    * The high order bit differentiate reserved classes from member classes.
    * Classes 0x0000 to 0x7FFF are member classes, allocated by ONF.
    * Classes 0x8000 to 0xFFFE are reserved classes, reserved for standardisation.
    */
    enum ofp_oxm_class {
        OFPXMC_NXM_0          = 0x0000, /* Backward compatibility with NXM */
        OFPXMC_NXM_1          = 0x0001, /* Backward compatibility with NXM */
        OFPXMC_OPENFLOW_BASIC = 0x8000, /* Basic class for OpenFlow */
        OFPXMC_EXPERIMENTER   = 0xFFFF, /* Experimenter class */
    };

    /* OXM Flow match field types for OpenFlow basic class. */
    enum oxm_ofb_match_fields {
        OFPXMT_OFB_IN_PORT        = 0, /* Switch input port. */
        OFPXMT_OFB_IN_PHY_PORT    = 1, /* Switch physical input port. */
        OFPXMT_OFB_METADATA       = 2, /* Metadata passed between tables. */
        OFPXMT_OFB_ETH_DST        = 3, /* Ethernet destination address. */
        OFPXMT_OFB_ETH_SRC        = 4, /* Ethernet source address. */
        OFPXMT_OFB_ETH_TYPE       = 5, /* Ethernet frame type. */
        OFPXMT_OFB_VLAN_VID       = 6, /* VLAN id. */
        OFPXMT_OFB_VLAN_PCP       = 7, /* VLAN priority. */
        OFPXMT_OFB_IP_DSCP        = 8, /* IP DSCP (6 bits in ToS field). */
        OFPXMT_OFB_IP_ECN         = 9, /* IP ECN (2 bits in ToS field). */
        OFPXMT_OFB_IP_PROTO       = 10, /* IP protocol. */
        OFPXMT_OFB_IPV4_SRC       = 11, /* IPv4 source address. */
        OFPXMT_OFB_IPV4_DST       = 12, /* IPv4 destination address. */
        OFPXMT_OFB_TCP_SRC        = 13, /* TCP source port. */
        OFPXMT_OFB_TCP_DST        = 14, /* TCP destination port. */
        OFPXMT_OFB_UDP_SRC        = 15, /* UDP source port. */
        OFPXMT_OFB_UDP_DST        = 16, /* UDP destination port. */
        OFPXMT_OFB_SCTP_SRC       = 17, /* SCTP source port. */
        OFPXMT_OFB_SCTP_DST       = 18, /* SCTP destination port. */
        OFPXMT_OFB_ICMPV4_TYPE    = 19, /* ICMP type. */
        OFPXMT_OFB_ICMPV4_CODE    = 20, /* ICMP code. */
        OFPXMT_OFB_ARP_OP         = 21, /* ARP opcode. */
        OFPXMT_OFB_ARP_SPA        = 22, /* ARP source IPv4 address. */
        OFPXMT_OFB_ARP_TPA        = 23, /* ARP target IPv4 address. */
        OFPXMT_OFB_ARP_SHA        = 24, /* ARP source hardware address. */
        OFPXMT_OFB_ARP_THA        = 25, /* ARP target hardware address. */
        OFPXMT_OFB_IPV6_SRC       = 26, /* IPv6 source address. */
        OFPXMT_OFB_IPV6_DST       = 27, /* IPv6 destination address. */
        OFPXMT_OFB_IPV6_FLABEL    = 28, /* IPv6 Flow Label */
        OFPXMT_OFB_ICMPV6_TYPE    = 29, /* ICMPv6 type. */
        OFPXMT_OFB_ICMPV6_CODE    = 30, /* ICMPv6 code. */
        OFPXMT_OFB_IPV6_ND_TARGET = 31, /* Target address for ND. */
        OFPXMT_OFB_IPV6_ND_SLL    = 32, /* Source link-layer for ND. */
        OFPXMT_OFB_IPV6_ND_TLL    = 33, /* Target link-layer for ND. */
        OFPXMT_OFB_MPLS_LABEL     = 34, /* MPLS label. */
        OFPXMT_OFB_MPLS_TC        = 35, /* MPLS TC. */
        OFPXMT_OFB_MPLS_BOS       = 36, /* MPLS BoS bit. */
        OFPXMT_OFB_PBB_ISID       = 37, /* PBB I-SID. */
        OFPXMT_OFB_TUNNEL_ID      = 38, /* Logical Port Metadata. */
        OFPXMT_OFB_IPV6_EXTHDR    = 39, /* IPv6 Extension Header pseudo-field */
    };

    /* special values dened for OFPXMT_OFB_VLAN_VID:
     * The VLAN id is 12-bits, so we can use the entire 16 bits to indicate
     * special conditions.
     */
    enum ofp_vlan_id {
        OFPVID_PRESENT = 0x1000, /* Bit that indicate that a VLAN id is set */
        OFPVID_NONE    = 0x0000, /* No VLAN id was set. */
    };

    /* Bit definitions for IPv6 Extension Header pseudo-field. 
       The field OXM_OF_IPV6_EXTHDR is a pseudo field that indicates the presence of various IPv6 extension
       headers in the packet header. The IPv6 extension header bits are combined together in the elds
       OXM_OF_IPV6_EXTHDR, and those bits can have the following values: */
    enum ofp_ipv6exthdr_flags {
        OFPIEH_NONEXT = 1 << 0, /* "No next header" encountered. */
        OFPIEH_ESP    = 1 << 1, /* Encrypted Sec Payload header present. */
        OFPIEH_AUTH   = 1 << 2, /* Authentication header present. */
        OFPIEH_DEST   = 1 << 3, /* 1 or 2 dest headers present. */
        OFPIEH_FRAG   = 1 << 4, /* Fragment header present. */
        OFPIEH_ROUTER = 1 << 5, /* Router header present. */
        OFPIEH_HOP    = 1 << 6, /* Hop-by-hop header present. */
        OFPIEH_UNREP  = 1 << 7, /* Unexpected repeats encountered. */
        OFPIEH_UNSEQ  = 1 << 8, /* Unexpected sequencing encountered. */
    };

    /* Header for OXM experimenter match fields. */
    struct ofp_oxm_experimenter_header {
        ACE_UINT32 oxm_header; /* oxm_class = OFPXMC_EXPERIMENTER */
        ACE_UINT32 experimenter; /* Experimenter ID which takes the same form as in struct ofp_experimenter_header. */
    };

    typedef enum ofp_ip_type {
        OFP_MATCH_IPV4 = 1 << 0,
        OFP_MATCH_IPV6 = 1 << 1, 
    } ofpIpType;

    COFP13Match(ACE_UINT16 awType = OFPMT_OXM);
    virtual ~COFP13Match();
    COFP13Match &operator=(COFP13Match &aMatch);
    
public:
    inline ACE_UINT16 GetMatchType()                                const;
    /* Total length of the OXM TLV.*/
    inline ACE_UINT32 GetMatchOxmFieldListLen()                     const;

    // Check whether pre-requisite are satisfied in Table~11: Match fields details.
    // Notice: 
    // 1. currently, we check the prerequisites by iterating the previous fields. This is of high time cost if there 
    // are many matches with many fields. If it's a hot-spot, we will reconstruct the code as nx_pull_raw() does in OVS V1.10.0.
    bool IsMeetPrerequisite(const std::list<COFP13OxmTlv *> &aMatchFieldList, 
                            const COFP13OxmTlv *apOxmTlv);

    // for m_matchFieldList
    // aList should meet the pre-requisite in Table~11: Match fields details.
    CmResult AddOxmTlvList(std::list<COFP13OxmTlv *> &aList);
    CmResult AppendOxmTlv(COFP13OxmTlv *apOxmTlv);
    CmResult GetOxmTlvList(std::list<COFP13OxmTlv *> &aList);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    #if 0
    // for interface IOFPMatch
    /* The following restriction, of OpenFlow Spec 1.3.0, is implemented in the function:
       Any given oxm_type may appear in an OpenFlow match at most once, otherwise the switch must generate an error (
       see 6.4). */
    virtual CmResult ToOpenFlowMatch(COpenFlowMatch &aMatch);

    /* The logic of our function refered to nx_put_raw, from ovs, 2013-08-13.
    static int
    nx_put_raw(struct ofpbuf *b, bool oxm, const struct match *match,
               ovs_be64 cookie, ovs_be64 cookie_mask)
    */
    virtual CmResult FromOpenFlowMatch(const COpenFlowMatch &aMatch);
    #endif
    
protected:
    ACE_UINT16 m_wType;   /* One of OFPMT_* */
    ACE_UINT16 m_wLength; /* Length of ofp_match (excluding padding) */
    /* Followed by:
        * - Exactly (length - 4) (possibly 0) bytes containing OXM TLVs, then
        * - Exactly ((length + 7)/8*8 - length) (between 0 and 7) bytes of
        * all-zero bytes
        * In summary, ofp_match is padded as needed, to make its overall size
        * a multiple of 8, to preserve alignement in structures using it.
        */
    //BYTE m_bOxmFields[OFP13_MATCH_OXMTLV_HEADLEN]; /* OXMs start here - Make compiler happy */
    
    std::list<COFP13OxmTlv *> m_matchFieldList;

private:
    #if 0
    CmResult FillMetaData(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv);
    void FillEthDst(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv);
    void FillEthSrc(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv);
    void FillEthAddr(uint8_t *apAddr, uint8_t *apAddrMask, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv);
    CmResult FillVlanVid(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    
    /* Sets the VLAN VID that 'match' matches to 'vid', which is interpreted as an
     * OpenFlow 1.2 "vlan_vid" value, that is, the low 13 bits of 'vlan_tci' (VID
     * plus CFI). */
    void     match_set_vlan_vid(COpenFlowMatch &aMatch, WORD16 awVid);
    
    /* Sets the VLAN VID that 'flow' matches to 'vid', which is interpreted as an
     * OpenFlow 1.2 "vlan_vid" value, that is, the low 13 bits of 'vlan_tci' (VID
     * plus CFI), with the corresponding 'mask'. */
    void     match_set_vlan_vid_masked(COpenFlowMatch &aMatch, WORD16 awVid, WORD16 awMask);
    
    /* Sets the VLAN VID that 'flow' matches to 'vid', which is interpreted as an
     * OpenFlow 1.2 "vlan_vid" value, that is, the low 13 bits of 'vlan_tci' (VID
     * plus CFI). */
    void     match_key_set_vlan_vid(COpenFlowMatch &aMatch, WORD16 awVid);
    
    /* Modifies 'match' so that the VLAN VID is wildcarded.  If the PCP is already
     * wildcarded, then 'match' will match a packet regardless of whether it has an
     * 802.1Q header or not. */
    void     match_set_any_vid(COpenFlowMatch &aMatch);
    
    void     match_set_dl_tci_masked(COpenFlowMatch &aMatch, WORD16 awTci, WORD16 awMask);
    
    /* Modifies 'match' so that it matches only packets with an 802.1Q header whose
     * PCP equals the low 3 bits of 'dl_vlan_pcp'. */
    void match_set_dl_vlan_pcp(COpenFlowMatch &aMatch, uint8_t abDlVlanPcp);
    
    /* Sets the VLAN PCP that 'flow' matches to 'pcp', which should be in the
     * range 0...7.
     *
     * This function has no effect on the VLAN ID that 'flow' matches.
     *
     * After calling this function, 'flow' will not match packets without a VLAN
     * header. */
    void flow_set_vlan_pcp(COpenFlowMatch &aMatch, uint8_t pcp);

    void match_set_nw_dscp(COpenFlowMatch &aMatch, uint8_t abNwDscp);
    void match_set_nw_ecn(COpenFlowMatch &aMatch,  uint8_t abNwEcn);

    void FillIPV4Src(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillIPV4Dst(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillIPV4Addr(uint32_t &anAddr, uint32_t &anAddrMask, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillArpSpa(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillArpTpa(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillArpSha(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillArpTha(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillIpv6NdSll(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillIpv6NdTll(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    
    void FillIPV6Addr(ofp_in6_addr &aIpv6AddrDst, ofp_in6_addr &aIpv6AddrMaskDst,
                      const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillIPV6Addr(uint8_t aIpv6AddrDst[16], uint8_t aIpv6AddrMaskDst[16],
                      const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);

    void match_set_ipv6_addr_masked(ofp_in6_addr &aIpv6AddrDst, ofp_in6_addr &aIpv6AddrMaskDst, 
                                    const ofp_in6_addr &aIpv6AddrSrc, const ofp_in6_addr &aIpv6AddrMaskSrc);
    void match_set_ipv6_addr_masked(uint8_t aIpv6AddrDst[16], uint8_t aIpv6AddrMaskDst[16], 
                                    const uint8_t aIpv6AddrSrc[16], const uint8_t aIpv6AddrMaskSrc[16]);
    void ipv6_addr_bitand(const ofp_in6_addr &aIpv6AddrSrc, const ofp_in6_addr &aIpv6AddrMaskSrc, 
                          ofp_in6_addr &aIpv6AddrDst);
    void ipv6_addr_bitand(const uint8_t aIpv6AddrSrc[16], const uint8_t aIpv6AddrMaskSrc[16], 
                          uint8_t aIpv6AddrDst[16]);
    void FillIPV6FLabel(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillPbbIsid(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillIpv6Exthdr(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);
    void FillTunnelId(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv);

    CmResult AppendEthAddrOxm(BYTE abOxmField, const uint8_t aAddrVal[ETH_ADDR_LENTH], 
                              const uint8_t aAddrMask[ETH_ADDR_LENTH]);
    CmResult AppendIpv6AddrOxm(BYTE abOxmField, const uint8_t aAddrVal[16], const uint8_t aAddrMask[16]);

    template<typename T> CmResult AppendOxmWithMask(BYTE abFieldType, T abPayload, T abMask, T abFullMask);

    CmResult AppendArrayOxmWithoutMask(BYTE abFieldType, const BYTE *apPayload, const BYTE *apMask, WORD16 awArraySize);

    // aFullMask: expected full mask, e.g: for vlan_vid, its aFullMask is (VLAN_VID_MASK | VLAN_CFI).
    template<typename T> CmResult AppendOxmWithoutMask(BYTE abFieldType, T abPayload, T abMask, T abFullMask);

    CmResult AppendIpOxm(ofpIpType aIpType, const COpenFlowMatch &aMatch);
    #endif
    template<typename T> bool CheckPrevOxm(const std::list<COFP13OxmTlv *> aMatchFieldList, 
                                                   ACE_CDR::Octet abExpectType, 
                                                   T aExpectVal1, 
                                                   T aExpectVal2 = 0);
};


inline ACE_UINT16 COFP13Match::GetMatchType()                       const
{
    return m_wType;
}


/* Total length of the OXM TLV.*/
inline ACE_UINT32 COFP13Match::GetMatchOxmFieldListLen()            const
{
    return (m_wLength - OFP13_MATCH_HEADLEN);
}

#if 0
inline void COFP13Match::FillEthDst(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv)
{
    FillEthAddr(aMatch.m_match.key.dl_dst, aMatch.m_match.mask.dl_dst, apOxmTlv);
}


inline void COFP13Match::FillEthSrc(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv)
{
    FillEthAddr(aMatch.m_match.key.dl_src, aMatch.m_match.mask.dl_src, apOxmTlv);
}


/* Sets the VLAN VID that 'match' matches to 'vid', which is interpreted as an
 * OpenFlow 1.2 "vlan_vid" value, that is, the low 13 bits of 'vlan_tci' (VID
 * plus CFI). */
inline void COFP13Match::match_set_vlan_vid(COpenFlowMatch &aMatch, WORD16 aVid)
{
    match_set_vlan_vid_masked(aMatch, aVid, VLAN_VID_MASK | VLAN_CFI);
}


inline void COFP13Match::FillIPV4Src(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillIPV4Addr(aMatch.m_match.key.ipv4_src, aMatch.m_match.mask.ipv4_src, apOxmTlv);
}


inline void COFP13Match::FillIPV4Dst(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillIPV4Addr(aMatch.m_match.key.ipv4_dst, aMatch.m_match.mask.ipv4_dst, apOxmTlv);
}


inline void COFP13Match::FillArpSpa(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillIPV4Addr(aMatch.m_match.key.arp_spa, aMatch.m_match.mask.arp_spa, apOxmTlv);
}


inline void COFP13Match::FillArpTpa(COpenFlowMatch &aMatch, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillIPV4Addr(aMatch.m_match.key.arp_tpa, aMatch.m_match.mask.arp_tpa, apOxmTlv);
}


inline void COFP13Match::FillArpSha(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillEthAddr(aMatch.m_match.key.arp_sha, aMatch.m_match.mask.arp_sha, apOxmTlv);
}


inline void COFP13Match::FillArpTha(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillEthAddr(aMatch.m_match.key.arp_tha, aMatch.m_match.mask.arp_tha, apOxmTlv);
}


inline void COFP13Match::FillIpv6NdSll(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillEthAddr(aMatch.m_match.key.ipv6_nd_sll, aMatch.m_match.mask.ipv6_nd_sll, apOxmTlv);
}


inline void COFP13Match::FillIpv6NdTll(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    FillEthAddr(aMatch.m_match.key.ipv6_nd_tll, aMatch.m_match.mask.ipv6_nd_tll, apOxmTlv);
}
#endif

/* Each OXM TLV is 5 to 259 (inclusive) bytes long. */
#define OFP13_MATCH_OXMTLV_MINLEN  (5)
#define OFP13_MATCH_OXMTLV_MAXLEN  (259)

/* NB: the following is NOT implemented.
 * Set Field actions, A.2.5 Action Structures
 * The match of the ow entry must contain the OXM prerequisite
 * curresponding to the field to be set (see A.2.3.6), otherwise an error must be generated (see 6.4).
 * format: OXM0, OXM1, etc. And every OXM is of the following structure,
 *   |<--------------XML TLV header---------------------->|<---payload---->|
 *  31                                         1615             9  8 7                 0
 *   |              oxm_class               | |oxm_field |HM| oxm_length|****************|
 */   
class COFP13OxmTlv : public IOFPMsgParser
{
public:
    COFP13OxmTlv(
        ACE_CDR::Octet         abPayloadLen = 0,
        const ACE_CDR::Octet  *apPayload    = NULL,
        ACE_CDR::Octet         abOxmFiled   = 0,
        ACE_CDR::Octet         abOxmHasMask = false,
        ACE_UINT16       awOxmClass   = COFP13Match::OFPXMC_OPENFLOW_BASIC);
    virtual ~COFP13OxmTlv();

    ACE_UINT16      GetOxmClass()                                   const;
    ACE_CDR::Octet        GetOxmField()                                   const;
    bool        GetHasMask()                                    const;
    ACE_CDR::Octet        GetOxmLen()                                     const;
    const ACE_CDR::Octet *GetPayload()                                    const;

    void SetOxmClass(ACE_UINT16 awClass);
    void SetOxmField(ACE_CDR::Octet abField);
    void SetHasMask(bool abHasMask = false);

    // The sizes of data pointed to by apPayload and apPayloadMask are both abPayloadLen.
    CmResult SetPayload(const ACE_CDR::Octet *apPayload, ACE_CDR::Octet abPayloadLen, const ACE_CDR::Octet *apPayloadMask = NULL);

    friend CmResult COFPMsgMemoryMgr::Copy(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, 
                                                ACE_CDR::Octet &abDesUsedMemSize, const ACE_CDR::Octet *apSrc, ACE_CDR::Octet abSrcSize);

    static CmResult DecodeOxmTlv(ACE_InputCDR &is, COFP13OxmTlv * &apOxmTlv);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    bool isHardwareAddr(ACE_CDR::Octet abField)                           const;
    bool isIPv6Addr(ACE_CDR::Octet abField)                               const;
    CmResult ConvertPayloadByteOrder();    

private:
    ACE_UINT16 m_wOxmClass;
    ACE_CDR::Octet   m_bOxmFieldAndHasMaskFlag;    // oxm_field:7 oxm_hasmask:1
    ACE_CDR::Octet   m_bOxmLen;                    // size of the used memory for payload, while m_wPayloadMemSize is the total 
                                         // size of memory.
    static std::allocator<ACE_CDR::Octet> m_alloc;
    ACE_CDR::Octet  *m_pPayload;
    ACE_UINT16 m_wPayloadMemSize;            // size of the memory for payload, while m_bOxmLen is the size of used memory.
};


inline ACE_UINT16 COFP13OxmTlv::GetOxmClass()                       const
{
    return m_wOxmClass;
}


inline ACE_CDR::Octet COFP13OxmTlv::GetOxmField()                         const
{
    return (m_bOxmFieldAndHasMaskFlag>>1);
}


inline ACE_CDR::Octet COFP13OxmTlv::GetOxmLen()                           const
{
    return m_bOxmLen;
}


inline const ACE_CDR::Octet *COFP13OxmTlv::GetPayload()                   const
{
    return m_pPayload;
}


inline void COFP13OxmTlv::SetOxmClass(ACE_UINT16 awClass)
{
    m_wOxmClass = awClass;
}


inline ACE_UINT16 COFP13OxmTlv::GetStreamLen()                      const
{
    return (m_bOxmLen + OFP13_MATCH_OXMTLV_HEADLEN);
}


inline ACE_UINT16 COFP13OxmTlv::GetEncodeSpace()                    const
{
    return GetStreamLen();
}


inline bool COFP13OxmTlv::isHardwareAddr(ACE_CDR::Octet abField)          const 
{
    return ((COFP13Match::OFPXMT_OFB_ETH_DST == abField)
            || (COFP13Match::OFPXMT_OFB_ETH_SRC == abField)
            || (COFP13Match::OFPXMT_OFB_ARP_SHA == abField)
            || (COFP13Match::OFPXMT_OFB_ARP_THA == abField));
}


inline bool COFP13OxmTlv::isIPv6Addr(ACE_CDR::Octet abField)              const
{
    return ((COFP13Match::OFPXMT_OFB_IPV6_SRC == abField)
            || (COFP13Match::OFPXMT_OFB_IPV6_DST == abField)
            || (COFP13Match::OFPXMT_OFB_IPV6_ND_TARGET == abField));
}
#endif  // COFP_MATCH_H

