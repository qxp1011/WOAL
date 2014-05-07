/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMatch.cpp
 * Description:      OpenFlow match source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-21
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPMatch.h"
#include "openflow-1.0.h"
#include "ace/Log_Msg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

std::allocator<ACE_CDR::Octet> COFP13OxmTlv::m_alloc;


void COFP10Match::SetDlSrc(const ACE_CDR::Octet aDlSrc[OFP_ETH_ALEN], const ACE_CDR::Octet aDlSrcMask[OFP_ETH_ALEN])
{
    for (ACE_CDR::Octet i = 0; i < OFP_ETH_ALEN; ++i)
    {
        m_tMember.dl_src[i] = (aDlSrc[i] & aDlSrcMask[i]);
    }
}


void COFP10Match::SetDLSrc(string &mac)
{
    const char *aacDlSrc = mac.c_str();
    int size = strlen(mac.c_str());
    if (size >= 6)
    {
        ::memcpy(m_tMember.dl_src, aacDlSrc, 6);
    }
    else
    {
        ::memcpy(m_tMember.dl_src, aacDlSrc, size);
    }
}


void COFP10Match::SetDlDst(const ACE_CDR::Octet aDlDst[OFP_ETH_ALEN], const ACE_CDR::Octet aDlDstMask[OFP_ETH_ALEN])
{
    for (ACE_CDR::Octet i = 0; i < OFP_ETH_ALEN; ++i)
    {
        m_tMember.dl_dst[i] = (aDlDst[i] & aDlDstMask[i]);
    }
}


void COFP10Match::SetDlDst(string &mac)
{
    const char *aacDlSrc = mac.c_str();
    int size = strlen(mac.c_str());
    if (size >= 6)
    {
        ::memcpy(m_tMember.dl_dst, aacDlSrc, 6);
    }
    else
    {
        ::memcpy(m_tMember.dl_dst, aacDlSrc, size);
    }    
}


CmResult COFP10Match::StreamFrom(ACE_InputCDR &is)
{
    is>>m_tMember.wildcards;
    is>>m_tMember.in_port;
    is.read_octet_array(m_tMember.dl_src, sizeof(m_tMember.dl_src));
    is.read_octet_array(m_tMember.dl_dst, sizeof(m_tMember.dl_dst));
    is>>m_tMember.dl_vlan;
    is>>m_tMember.dl_vlan_pcp;
    is>>m_tMember.pad1[0];
    is>>m_tMember.dl_type;
    is>>m_tMember.nw_tos;
    is>>m_tMember.nw_proto;
    is.read_octet_array(m_tMember.pad2, sizeof(m_tMember.pad2));
    is>>m_tMember.nw_src;
    is>>m_tMember.nw_dst;
    is>>m_tMember.tp_src;
    is>>m_tMember.tp_dst;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10Match::StreamTo(ACE_OutputCDR &os)
{
    os<<m_tMember.wildcards;
    os<<m_tMember.in_port;
    os.write_octet_array(m_tMember.dl_src, sizeof(m_tMember.dl_src));
    os.write_octet_array(m_tMember.dl_dst, sizeof(m_tMember.dl_dst));
    os<<m_tMember.dl_vlan;
    os<<m_tMember.dl_vlan_pcp;
    os<<m_tMember.pad1[0];
    os<<m_tMember.dl_type;
    os<<m_tMember.nw_tos;
    os<<m_tMember.nw_proto;
    os.write_octet_array(m_tMember.pad2, sizeof(m_tMember.pad2));
    os<<m_tMember.nw_src;
    os<<m_tMember.nw_dst;
    os<<m_tMember.tp_src;
    os<<m_tMember.tp_dst;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP10Match::Dump() const
{
}

#if 0
CmResult COFP10Match::ToOpenFlowMatch(COpenFlowMatch &aMatch)
{
    // 0905 implement 1.3.1 first
    #if 0
    using namespace wavplatform::utils;
    
    uint32_t ofpfw = (m_tMember.wildcards & OFPFW10_ALL);

    /* Initialize match->wc. */
    ::memset(&(aMatch.m_key), 0, sizeof(aMatch.m_key));
    ofputil_wildcard_from_ofpfw10(ofpfw, aMatch.m_mask);

    /* Initialize most of match->flow. */
    aMatch.m_key.m_match_key.nw_src = m_tMember.nw_src;
    aMatch.m_key.m_match_key.nw_dst = m_tMember.nw_dst;
    aMatch.m_key.m_match_key.in_port.ofp_port = m_tMember.in_port;
    aMatch.m_key.m_match_key.dl_type = m_tMember.dl_type;
    aMatch.m_key.m_match_key.tp_src = m_tMember.tp_src;
    aMatch.m_key.m_match_key.tp_dst = m_tMember.tp_dst;
    memcpy(aMatch.m_key.m_match_key.dl_src, m_tMember.dl_src, ETH_ADDR_LEN);
    memcpy(aMatch.m_key.m_match_key.dl_dst, m_tMember.dl_dst, ETH_ADDR_LEN);
    aMatch.m_key.m_match_key.nw_tos = m_tMember.nw_tos & IP_DSCP_MASK;
    aMatch.m_key.m_match_key.nw_proto = m_tMember.nw_proto;

    /* Translate VLANs. */
    if (!(ofpfw & OFPFW10_DL_VLAN) 
        && (OFP10_VLAN_NONE == m_tMember.dl_vlan)) {
        /* Match only packets without 802.1Q header.
         *
         * When OFPFW10_DL_VLAN_PCP is wildcarded, this is obviously correct.
         *
         * If OFPFW10_DL_VLAN_PCP is matched, the flow match is contradictory,
         * because we can't have a specific PCP without an 802.1Q header.
         * However, older versions of OVS treated this as matching packets
         * withut an 802.1Q header, so we do here too. */
        aMatch.m_key.m_match_key.vlan_tci = 0;
        ::memset(&(aMatch.m_mask.m_match_mask.vlan_tci), OFP_BYTE_MAX_VALUE, sizeof(uint16_t));
    } else {
        uint16_t vid, pcp, tci;

        vid = (m_tMember.dl_vlan & VLAN_VID_MASK);
        pcp = ((m_tMember.dl_vlan_pcp << VLAN_PCP_SHIFT) & VLAN_PCP_MASK);
        tci = (vid | pcp | VLAN_CFI);
        aMatch.m_key.m_match_key.vlan_tci = (tci & aMatch.m_mask.m_match_mask.vlan_tci);
    }

    /* Clean up. */
    aMatch.match_zero_wildcarded_fields();

    aMatch.ofputil_normalize_match();
    #endif

    return CM_OK;
}


CmResult COFP10Match::FromOpenFlowMatch(const COpenFlowMatch &aMatch)
{
    // 0905 implement 1.3.1 first
    #if 0
    using namespace wavplatform::utils;
    
    uint32_t ofpfw;

    /* Figure out most OpenFlow wildcards. */
    ofpfw = 0;
    if (!aMatch.m_mask.m_match_mask.in_port.ofp_port) {
        ofpfw |= OFPFW10_IN_PORT;
    }
    if (!aMatch.m_mask.m_match_mask.dl_type) {
        ofpfw |= OFPFW10_DL_TYPE;
    }
    if (!aMatch.m_mask.m_match_mask.nw_proto) {
        ofpfw |= OFPFW10_NW_PROTO;
    }
    ofpfw |= ((32 - COFPMsgCommonUtils::CalcIpOneBitNum(aMatch.m_mask.m_match_mask.nw_src))
              << OFPFW10_NW_SRC_SHIFT);
    ofpfw |= ((32 - COFPMsgCommonUtils::CalcIpOneBitNum(aMatch.m_mask.m_match_mask.nw_dst))
              << OFPFW10_NW_DST_SHIFT);
    if (!(aMatch.m_mask.m_match_mask.nw_tos & IP_DSCP_MASK)) {
        ofpfw |= OFPFW10_NW_TOS;
    }
    if (!aMatch.m_mask.m_match_mask.tp_src) {
        ofpfw |= OFPFW10_TP_SRC;
    }
    if (!aMatch.m_mask.m_match_mask.tp_dst) {
        ofpfw |= OFPFW10_TP_DST;
    }
    if (COFPMsgCommonUtils::eth_addr_is_zero(aMatch.m_mask.m_match_mask.dl_src)) {
        ofpfw |= OFPFW10_DL_SRC;
    }
    if (COFPMsgCommonUtils::eth_addr_is_zero(aMatch.m_mask.m_match_mask.dl_dst)) {
        ofpfw |= OFPFW10_DL_DST;
    }

    /* Translate VLANs. */
    m_tMember.dl_vlan = 0;
    m_tMember.dl_vlan_pcp = 0;
    if (0 == aMatch.m_mask.m_match_mask.vlan_tci) {
        ofpfw |= OFPFW10_DL_VLAN | OFPFW10_DL_VLAN_PCP;
    } else if ((aMatch.m_mask.m_match_mask.vlan_tci & VLAN_CFI != 0)
               && !(aMatch.m_key.m_match_key.vlan_tci & VLAN_CFI)) {
        m_tMember.dl_vlan = OFP10_VLAN_NONE;
        ofpfw |= OFPFW10_DL_VLAN_PCP;
    } else {
        if (!(aMatch.m_mask.m_match_mask.vlan_tci & VLAN_VID_MASK)) {
            ofpfw |= OFPFW10_DL_VLAN;
        } else {
            m_tMember.dl_vlan = vlan_tci_to_vid(aMatch.m_key.m_match_key.vlan_tci);
        }

        if (!(aMatch.m_mask.m_match_mask.vlan_tci & VLAN_PCP_MASK)) {
            ofpfw |= OFPFW10_DL_VLAN_PCP;
        } else {
            m_tMember.dl_vlan_pcp = vlan_tci_to_pcp(aMatch.m_key.m_match_key.vlan_tci);
        }
    }

    /* Compose most of the match structure. */
    m_tMember.wildcards = ofpfw;
    m_tMember.in_port = aMatch.m_key.m_match_key.in_port.ofp_port;
    memcpy(m_tMember.dl_src, aMatch.m_key.m_match_key.dl_src, ETH_ADDR_LEN);
    memcpy(m_tMember.dl_dst, aMatch.m_key.m_match_key.dl_dst, ETH_ADDR_LEN);
    m_tMember.dl_type = aMatch.m_key.m_match_key.dl_type;
    m_tMember.nw_src = aMatch.m_key.m_match_key.nw_src;
    m_tMember.nw_dst = aMatch.m_key.m_match_key.nw_dst;
    m_tMember.nw_tos = (aMatch.m_key.m_match_key.nw_tos & IP_DSCP_MASK);
    m_tMember.nw_proto = aMatch.m_key.m_match_key.nw_proto;
    m_tMember.tp_src = aMatch.m_key.m_match_key.tp_src;
    m_tMember.tp_dst = aMatch.m_key.m_match_key.tp_dst;
    memset(m_tMember.pad1, '\0', sizeof(m_tMember.pad1));
    memset(m_tMember.pad2, '\0', sizeof(m_tMember.pad2));
    #endif

    return CM_OK;
}
#endif

bool COFP10Match::Compare(COFP10Match &aMatch)
{
    ACE_INT16 wResult = ::memcmp(&m_tMember, &aMatch.m_tMember, sizeof(m_tMember));

    return (wResult == 0 ? true : false);
}

#if 0
/* Converts the OpenFlow 1.0 wildcards in 'ofpfw' (OFPFW10_*) into a
 * flow_wildcards in 'wc' for use in struct match.  It is the caller's
 * responsibility to handle the special case where the flow match's dl_vlan is
 * set to OFP_VLAN_NONE. */
void
COFP10Match::ofputil_wildcard_from_ofpfw10(uint32_t ofpfw, COpenFlowMatchMask &wc)
{
    using namespace wavplatform::utils;
    
    BUILD_ASSERT_DECL(FLOW_WC_SEQ == 20);

    /* Initialize most of wc. */
    ::memset(&wc, 0, sizeof(wc));

    if (!(ofpfw & OFPFW10_IN_PORT)) {
        wc.m_match_mask.in_port.ofp_port = UINT16_MAX;
    }

    if (!(ofpfw & OFPFW10_NW_TOS)) {
        wc.m_match_mask.nw_tos |= IP_DSCP_MASK;
    }

    if (!(ofpfw & OFPFW10_NW_PROTO)) {
        wc.m_match_mask.nw_proto = UINT8_MAX;
    }
    wc.m_match_mask.nw_src = COFPMsgCommonUtils::ofputil_wcbits_to_netmask(ofpfw >> OFPFW10_NW_SRC_SHIFT);
    wc.m_match_mask.nw_dst = COFPMsgCommonUtils::ofputil_wcbits_to_netmask(ofpfw >> OFPFW10_NW_DST_SHIFT);

    if (!(ofpfw & OFPFW10_TP_SRC)) {
        wc.m_match_mask.tp_src = UINT16_MAX;
    }
    if (!(ofpfw & OFPFW10_TP_DST)) {
        wc.m_match_mask.tp_dst = UINT16_MAX;
    }

    if (!(ofpfw & OFPFW10_DL_SRC)) {
        ::memset(wc.m_match_mask.dl_src, 0xff, ETH_ADDR_LEN);
    }
    if (!(ofpfw & OFPFW10_DL_DST)) {
        ::memset(wc.m_match_mask.dl_dst, 0xff, ETH_ADDR_LEN);
    }
    if (!(ofpfw & OFPFW10_DL_TYPE)) {
        wc.m_match_mask.dl_type = UINT16_MAX;
    }

    /* VLAN TCI mask. */
    if (!(ofpfw & OFPFW10_DL_VLAN_PCP)) {
        wc.m_match_mask.vlan_tci |= (VLAN_PCP_MASK | VLAN_CFI);
    }
    if (!(ofpfw & OFPFW10_DL_VLAN)) {
        wc.m_match_mask.vlan_tci |= (VLAN_VID_MASK | VLAN_CFI);
    }
}
#endif

COFP13Match::COFP13Match(ACE_UINT16 awType)
    : m_wType(awType)
    , m_wLength(OFP13_MATCH_HEADLEN)
{
}

COFP13Match::~COFP13Match()
{
    for (auto &item : m_matchFieldList)
    {
        delete item;
    }
}

COFP13Match &COFP13Match::operator=(COFP13Match &aMatch)
{
    m_wType   = aMatch.m_wType;
    m_wLength = aMatch.m_wLength;
    
    m_matchFieldList.clear();
    std::list<COFP13OxmTlv *> list;
    CmResult result = aMatch.GetOxmTlvList(list);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), *this);

    result = AddOxmTlvList(list);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), *this);

    return *this;
}


// Check whether pre-requisite are satisfied in Table~11: Match fields details.
// Notice: 
// 1. currently, we check the prerequisites by iterating the previous fields. This is of high time cost if there are many
//    matches with many fields. If it's a hot-spot, we will reconstruct the code as nx_pull_raw() does in OVS V1.10.0.
// 2. 'Mask' field in Table~11 only indicates the capabilities of the OXM. However, the match OXM whose 'Mask' is 'Yes' may
//    be with 0-value oxm_hasmask.
bool COFP13Match::
IsMeetPrerequisite(
    const std::list<COFP13OxmTlv *> &aMatchFieldList, 
    const COFP13OxmTlv *apOxmTlv)
{
    bool bResult = false;
   
    CM_ASSERT_RETURN(apOxmTlv != NULL, bResult);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Match::IsMeetPrerequisite, push\n")));

    ACE_UINT16 wOXMClass   = apOxmTlv->GetOxmClass();
    ACE_CDR::Octet   bOXMField   = apOxmTlv->GetOxmField();
    bool   bOXMHasMask = apOxmTlv->GetHasMask();
    ACE_CDR::Octet   bOXMLength  = apOxmTlv->GetOxmLen();
    
    CM_ASSERT_RETURN((OFPXMC_OPENFLOW_BASIC == wOXMClass), true);

    switch (bOXMField)
    {
        case OFPXMT_OFB_IN_PORT:
        {
            if ((0 == bOXMHasMask) && (OXM_OF_IN_PORT_BITS / OFP_ONEBYTE_SIZE == bOXMLength))
            {
                bResult = true;
            }
            break;
        }
        case OFPXMT_OFB_IN_PHY_PORT:
        {
            if ((bOXMHasMask != 0) || (bOXMLength != (OXM_OF_IN_PHY_PORT_BITS / OFP_ONEBYTE_SIZE)))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("(bOXMHasMask != 0) || (bOXMLength != (OXM_OF_IN_PHY_PORT_BITS / OFP_ONEBYTE_SIZE))."),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask, 
                           bOXMLength));
                break;
            }

            for (auto &item : aMatchFieldList)
            {
                ACE_CDR::Octet bTmpFieldVal = item->GetOxmField();
                if (OFPXMT_OFB_IN_PORT == bTmpFieldVal)
                {
                    bResult = true;
                    break;
                }
            }

            break;
        }
        case OFPXMT_OFB_METADATA:
        {
            if (((false == bOXMHasMask) && (OXM_OF_METADATA_BITS / OFP_ONEBYTE_SIZE == bOXMLength))
                || ((true == bOXMHasMask) && (OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_METADATA_BITS / OFP_ONEBYTE_SIZE == bOXMLength)))
            {
                bResult = true;
            }
            break;
        }
        case OFPXMT_OFB_ETH_DST:
        {
            if (((1 == bOXMHasMask) && (OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_ETH_ADDR_BITS / OFP_ONEBYTE_SIZE == bOXMLength))
                || ((0 == bOXMHasMask) && (OXM_OF_ETH_ADDR_BITS / OFP_ONEBYTE_SIZE == bOXMLength)))
            {
                bResult = true;
            }
            break;
        }
        case OFPXMT_OFB_ETH_SRC:
        {
            if (((1 == bOXMHasMask) && (OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_ETH_ADDR_BITS / OFP_ONEBYTE_SIZE  == bOXMLength))
                || ((0 == bOXMHasMask) && (OXM_OF_ETH_ADDR_BITS / OFP_ONEBYTE_SIZE  == bOXMLength)))
            {
                bResult = true;
            }
            break;
        }
        case OFPXMT_OFB_ETH_TYPE:
        {
            if ((0 == bOXMHasMask) && (OXM_OF_ETH_TYPE_BITS / OFP_ONEBYTE_SIZE == bOXMLength))
            {
                bResult = true;
            }
            break;
        }
        case OFPXMT_OFB_VLAN_VID:
        {
            if (((1 == bOXMHasMask) && (OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_VLAN_VID_ROUND_BYTE == bOXMLength))
                || ((0 == bOXMHasMask) && (OXM_OF_VLAN_VID_ROUND_BYTE == bOXMLength)))
            {
                bResult = true;
            }
            break;
        }
        case OFPXMT_OFB_VLAN_PCP:
        {
            if ((bOXMHasMask != 0) || (bOXMLength != OXM_OF_VLAN_PCP_ROUND_BYTE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("(bOXMHasMask != 0) || (bOXMLength != OXM_OF_VLAN_PCP_ROUND_BYTE). "),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }

            for (auto &item : aMatchFieldList)
            {
                ACE_CDR::Octet bTmpFieldVal      = item->GetOxmField();
                const ACE_CDR::Octet *apTmpField = item->GetPayload();
                ACE_UINT16 bTmpFieldPayload = *((ACE_UINT16 *)apTmpField);
                if ((OFPXMT_OFB_VLAN_VID == bTmpFieldVal) && (bTmpFieldPayload != OFPVID_NONE))
                {
                    bResult = true;
                    break;
                }
            }

            break;
        }
        case OFPXMT_OFB_IP_DSCP:
        case OFPXMT_OFB_IP_ECN:
        case OFPXMT_OFB_IP_PROTO:
        {
            if ((bOXMHasMask != false) || (bOXMLength != OFP_SIZEOF_BYTE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("(bOXMHasMask != 0) || (bOXMLength != 1). "),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, 
                                           OFPXMT_OFB_ETH_TYPE, 
                                           (ACE_UINT16)htons(OFP_ETHTYPE_IPV4_PROT), 
                                           (ACE_UINT16)htons(OFP_ETHTYPE_IPV6_PROT));
            
            break;
        }
        #if 0
        case OFPXMT_OFB_IP_PROTO:
        {
            if ((bOXMHasMask != 0) || (bOXMLength != OXM_OF_IP_PROTO_BITS / OFP_ONEBYTE_SIZE))
            {
                CM_ERROR_TRACE("(bOXMHasMask != 0) || (bOXMLength != 8). bOXMHasMask=" << bOXMHasMask 
                               << ", bOXMLength=" << bOXMLength);
                break;
            }

            for (std::list<CCmComAutoPtr<COFP13OxmTlv>>::const_iterator it = aMatchFieldList.begin(); 
                 it != aMatchFieldList.end(); 
                 it++)
            {
                BYTE bTmpFieldVal      = (*it)->GetOxmField();
                const BYTE *apTmpField = (*it)->GetPayload();
                WORD16 bTmpFieldPayload = *((WORD16 *)apTmpField);
                if ((OFPXMT_OFB_ETH_TYPE == bTmpFieldVal) 
                    && ((htons(OFP_ETHTYPE_IPV4_PROT) == bTmpFieldPayload) || (htons(OFP_ETHTYPE_IPV6_PROT) == bTmpFieldPayload)))
                {
                    bResult = TRUE;
                    break;
                }
                //it++;
            }
            
            break;
        }
        #endif
        case OFPXMT_OFB_IPV4_SRC:
        case OFPXMT_OFB_IPV4_DST:
        {
            if (1 == bOXMHasMask)
            {
                if (bOXMLength != OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_IPV4_ADDR_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 4 * 2).  bOXMLength=%u\n"), 
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength != OXM_OF_IPV4_ADDR_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 4)). bOXMLength=%u\n"), 
                               bOXMLength));
                    break;
                }
            }
            #if 0
            for (std::list<CCmComAutoPtr<COFP13OxmTlv>>::const_iterator it = aMatchFieldList.begin();
                 it != aMatchFieldList.end();
                 it++)
            {
                BYTE bTmpFieldVal      = (*it)->GetOxmField();
                const BYTE *apTmpField = (*it)->GetPayload();
                WORD16 bTmpFieldPayload = *((WORD16 *)(apTmpField + OFP13_MATCH_OXMTLV_HEADLEN));
                if ((OFPXMT_OFB_ETH_TYPE == bTmpFieldVal) && (OFP_ETHTYPE_IPV4_PROT == bTmpFieldPayload))
                {
                    bResult = TRUE;
                    break;
                }
            }
            #endif

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(OFP_ETHTYPE_IPV4_PROT));

            break;
        }
        case OFPXMT_OFB_TCP_SRC:
        case OFPXMT_OFB_TCP_DST:
        {
            if ((bOXMHasMask != 0) || (bOXMLength != OXM_OF_TCP_PORT_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) || (bOXMLength != 2)). "),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }

            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_IP_PROTO, OFP_TCP_TYPE);

            break;
        }
        case OFPXMT_OFB_UDP_SRC:
        case OFPXMT_OFB_UDP_DST:
        {
            if ((bOXMHasMask != 0) && (bOXMLength != OXM_OF_UDP_PORT_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 2)). "),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }

            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_IP_PROTO, OFP_UDP_TYPE);

            break;
        }
        case OFPXMT_OFB_SCTP_SRC:
        case OFPXMT_OFB_SCTP_DST:
        {
            if ((bOXMHasMask != 0) && (bOXMLength != OXM_OF_SCTP_PORT_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 2)). "),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }

            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_IP_PROTO, OFP_SCTP_TYPE);

            break;
        }
        case OFPXMT_OFB_ICMPV4_TYPE:
        case OFPXMT_OFB_ICMPV4_CODE:
        {
            if ((bOXMHasMask != 0) && (bOXMLength != OXM_OF_ICMPV4_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 1)). "),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_IP_PROTO, OFP_ICMPV4_TYPE);
        
            break;
        }
        case OFPXMT_OFB_ARP_OP:
        {
            if ((bOXMHasMask != 0) && (bOXMLength != OXM_OF_ARP_OP_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 2)). "),
                           ACE_TEXT("bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, ntohs(OFP_ETHTYPE_ARP_PROT));
        
            break;
        }
        case OFPXMT_OFB_ARP_SPA:         /* ARP source IPv4 address. */
        case OFPXMT_OFB_ARP_TPA:         /* ARP target IPv4 address. */
        {
            if (true == bOXMHasMask)
            {
                if (bOXMLength !=  OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_ARP_PA_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 2 * 32 / 8)). bOXMLength=%u\n"), 
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength !=  OXM_OF_ARP_PA_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 32 / 8)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(OFP_ETHTYPE_ARP_PROT));

            break;
        }
        case OFPXMT_OFB_ARP_SHA:         /* ARP source hardware address. */
        case OFPXMT_OFB_ARP_THA:         /* ARP target hardware address. */
        {
            if (true == bOXMHasMask)
            {
                if (bOXMLength !=  OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_ARP_HA_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 2 * 48 / 8)). bOXMLength=%u\n"), 
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength !=  OXM_OF_ARP_HA_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 48 / 8)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(OFP_ETHTYPE_ARP_PROT));

            break;
        }
        case OFPXMT_OFB_IPV6_SRC:
        case OFPXMT_OFB_IPV6_DST:
        {
            if (1 == bOXMHasMask)
            {
                if (bOXMLength !=  OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_IPV6_ADDR_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 2 * 128 / 8)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength !=  OXM_OF_IPV6_ADDR_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 128 / 8)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(OFP_ETHTYPE_IPV6_PROT));

            break;
        }
        case OFPXMT_OFB_IPV6_FLABEL:
        {
            if (1 == bOXMHasMask)
            {
                if (bOXMLength !=  OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_IPV6_FLABEL_ROUND_BYTE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 2 * 4)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength !=  OXM_OF_IPV6_FLABEL_ROUND_BYTE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 4)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(OFP_ETHTYPE_IPV6_PROT));

            break;
        }
        case OFPXMT_OFB_ICMPV6_TYPE:
        case OFPXMT_OFB_ICMPV6_CODE:
        {
            if ((bOXMHasMask != 0) && (bOXMLength != OXM_OF_ICMPV6_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 1)). bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_IP_PROTO, OFP_ICMPV4_TYPE);
        
            break;
        }
        case OFPXMT_OFB_IPV6_ND_TARGET:
        {
            if ((bOXMHasMask != 0) && (bOXMLength != OXM_OF_IPV6_ADDR_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 16)). bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_ICMPV6_TYPE, ND_NEIGHBOR_SOLICIT, ND_NEIGHBOR_ADVERT);
        
            break;
        }
        case OFPXMT_OFB_IPV6_ND_SLL:
        {
            if ((bOXMHasMask != false) && (bOXMLength != OXM_OF_IPV6_ND_LL_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != false) && (bOXMLength != 6)). bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_ICMPV6_TYPE, ND_NEIGHBOR_SOLICIT);
        
            break;
        }
        case OFPXMT_OFB_IPV6_ND_TLL:
        {
            if ((bOXMHasMask != false) && (bOXMLength != OXM_OF_IPV6_ND_LL_BITS / OFP_ONEBYTE_SIZE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != false) && (bOXMLength != 6)). bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_CDR::Octet>(aMatchFieldList, OFPXMT_OFB_ICMPV6_TYPE, ND_NEIGHBOR_ADVERT);
        
            break;
        }
        case OFPXMT_OFB_MPLS_LABEL:
        {
            if ((bOXMHasMask != false) && (bOXMLength != OXM_OF_MPLS_LABEL_ROUND_BYTES))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 4)). bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(ETH_TYPE_MPLS), htons(ETH_TYPE_MPLS_MCAST));
        
            break;
        }
        case OFPXMT_OFB_MPLS_TC:
        case OFPXMT_OFB_MPLS_BOS:
        {
            if ((bOXMHasMask != false) && (bOXMLength != OFP_SIZEOF_BYTE))
            {
                ACE_ERROR((LM_ERROR, 
                           ACE_TEXT("((bOXMHasMask != 0) && (bOXMLength != 1)). bOXMHasMask=%u, bOXMLength=%u\n"),
                           bOXMHasMask,
                           bOXMLength));
                break;
            }
        
            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(ETH_TYPE_MPLS), htons(ETH_TYPE_MPLS_MCAST));
        
            break;
        }
        case OFPXMT_OFB_PBB_ISID:
        {
            if (true == bOXMHasMask)
            {
                if (bOXMLength !=  OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_PBB_ISID_ROUND_BYTES)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 2 * 4)). bOXMLength=%u\n"), 
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength !=  OXM_OF_PBB_ISID_ROUND_BYTES)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 4)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(OFP_ETHTYPE_NPBTP_PROT));

            break;
        }
        case OFPXMT_OFB_TUNNEL_ID:
        {
            if (true == bOXMHasMask)
            {
                if (bOXMLength !=  OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_TUNNEL_ID_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 2 * 8)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength !=  OXM_OF_TUNNEL_ID_BITS / OFP_ONEBYTE_SIZE)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 8)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }

            bResult = true;

            break;
        }
        case OFPXMT_OFB_IPV6_EXTHDR:
        {
            if (true == bOXMHasMask)
            {
                if (bOXMLength !=  OFP_NUM_OF_COPIES_WITH_MASK * OXM_OF_IPV6_EXTHDR_ROUND_BYTES)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 1), (bOXMLength != 2 * 2)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }
            else
            {
                if (bOXMLength !=  OXM_OF_IPV6_EXTHDR_ROUND_BYTES)
                {
                    ACE_ERROR((LM_ERROR, 
                               ACE_TEXT("(bOXMHasMask == 0), (bOXMLength != 2)). bOXMLength=%u\n"),
                               bOXMLength));
                    break;
                }
            }

            bResult = CheckPrevOxm<ACE_UINT16>(aMatchFieldList, OFPXMT_OFB_ETH_TYPE, htons(OFP_ETHTYPE_IPV6_PROT));

            break;
        }
        default:
        {
            break;
        }
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Match::IsMeetPrerequisite, pop\n")));

    return bResult;
}


// aList should meet the pre-requisite in Table~11: Match fields details.
CmResult COFP13Match::AddOxmTlvList(std::list<COFP13OxmTlv *> &aList)
{
    ACE_UINT32 nOxmTlvLen = 0;
    while (!aList.empty())
    {
        COFP13OxmTlv *oxmTlv = aList.front();
        bool isMeetPrereq = IsMeetPrerequisite(m_matchFieldList, oxmTlv);
        if (!isMeetPrereq)
        {
            ACE_DEBUG((LM_DEBUG, 
                       ACE_TEXT("COFP13Match::AddOxmTlvList(): CheckMeetPrerequisite failed. "),
                       ACE_TEXT("oxmTlv: class=%u, field=%u, hasMask=%d, length=%u\n"),
                       oxmTlv->GetOxmClass(), oxmTlv->GetOxmField(), oxmTlv->GetHasMask(), oxmTlv->GetOxmLen()));
            return CM_ERROR_FAILURE;
        }
        aList.pop_front();
        nOxmTlvLen += oxmTlv->GetStreamLen();
        m_matchFieldList.push_back(oxmTlv);
    }

    m_wLength += nOxmTlvLen;

    return CM_OK;
}


CmResult COFP13Match::AppendOxmTlv(COFP13OxmTlv *apOxmTlv)
{
    bool isMeetPrereq = IsMeetPrerequisite(m_matchFieldList, apOxmTlv);
    if (!isMeetPrereq)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP13Match::AddOxmTlvList(): CheckMeetPrerequisite failed. "),
                   ACE_TEXT("apOxmTlv: class=%u, field=%u, hasMask=%u, length=%u\n"), 
                   apOxmTlv->GetOxmClass(), apOxmTlv->GetOxmField(), apOxmTlv->GetHasMask(), apOxmTlv->GetOxmLen()));
        return CM_ERROR_FAILURE;
    }

    m_matchFieldList.push_back(apOxmTlv);
    m_wLength += apOxmTlv->GetStreamLen();
    return CM_OK;
}


CmResult COFP13Match::GetOxmTlvList(std::list<COFP13OxmTlv *> &aList)
{
    while (!m_matchFieldList.empty())
    {
        COFP13OxmTlv *oxmTlv = m_matchFieldList.front();
        CM_ASSERT_RETURN(oxmTlv != nullptr, CM_ERROR_NULL_POINTER);
        m_matchFieldList.pop_front();
        m_wLength -= oxmTlv->GetOxmLen();
        aList.push_back(oxmTlv);
    }

    return CM_OK;
}


// for interface IOFPMsgParser
CmResult COFP13Match::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet   *pbPad   = NULL;
    ACE_UINT16  wPadLen = 0;

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Match::StreamFrom(): push\n")));
    
    is>>m_wType;
    is>>m_wLength;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\tType=%u, Length=%u\n"), m_wType, m_wLength));
    
    switch (m_wType)
    {
        case OFPMT_OXM:
        {
            break;
        }
        case OFPMT_STANDARD:
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("The OpenFlow 1.1 match type OFPMT_STANDARD is deprecated.\n")));
            return CM_ERROR_NOT_IMPLEMENTED;
        }
        default:
        {
            ACE_ERROR((LM_ERROR, ACE_TEXT("Unkown type, type=%u\n"), m_wType));
            return CM_ERROR_FAILURE;
        }
    }

    while (!m_matchFieldList.empty())
    {
        m_matchFieldList.pop_front();
    }

    ACE_UINT16 wNewFieldListLen = GetMatchOxmFieldListLen();

    if (wNewFieldListLen > 0)
    {
        ACE_INT16 remainFieldListLen = wNewFieldListLen;
        ACE_UINT16 i = 0;
        while (remainFieldListLen > 0)
        {
            COFP13OxmTlv *oxmTlv = nullptr;
            CmResult result = COFP13OxmTlv::DecodeOxmTlv(is, oxmTlv);
            if (CM_SUCCEEDED(result))
            {
                m_matchFieldList.push_back(oxmTlv);
                ACE_DEBUG((LM_DEBUG, ACE_TEXT("oxmTlv[%u], len=%u\n"), i++, (oxmTlv->GetOxmLen())));
                remainFieldListLen = remainFieldListLen - (oxmTlv->GetStreamLen());
                continue;
            }
            else
            {
                return CM_ERROR_FAILURE;
            }
        }
    }

    // encode paddings
    wPadLen = ((m_wLength + 7)/8*8 - m_wLength);
    std::allocator<ACE_CDR::Octet> alloc;
    pbPad = alloc.allocate(wPadLen);
    CM_ASSERT_RETURN((pbPad != NULL), CM_ERROR_OUT_OF_MEMORY);
    
    ::memset(pbPad, 0, wPadLen);
    bGood = is.read_octet_array(pbPad, wPadLen);
    if (!bGood)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("is.read_octet_array failed. pbPad=%p, wPadLen=%u\n"), pbPad, wPadLen));
        alloc.deallocate(pbPad, wPadLen);
        return CM_ERROR_FAILURE;
    }

    alloc.deallocate(pbPad, wPadLen);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Match::StreamFrom(): pop\n")));

    return CM_OK;
}


CmResult COFP13Match::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet   *pbPad   = NULL;
    ACE_UINT16        wPadLen = 0;

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Match::StreamTo(): push\n")));

    m_wLength = GetStreamLen();

    os<<m_wType;
    os<<m_wLength;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    CM_ASSERT_RETURN((OFPMT_OXM == m_wType), CM_ERROR_FAILURE);

    // encode OXM field list
    CM_ASSERT_RETURN((m_wLength >= OFP13_MATCH_HEADLEN), CM_ERROR_FAILURE);

    for (auto &oxmTlv : m_matchFieldList)
    {
        oxmTlv->StreamTo(os);
    }

    // encode paddings
    wPadLen = ((m_wLength + 7)/8*8 - m_wLength);
    std::allocator<ACE_CDR::Octet> alloc;
    pbPad = alloc.allocate(wPadLen);
    CM_ASSERT_RETURN((pbPad != NULL), CM_ERROR_OUT_OF_MEMORY);
    
    ::memset(pbPad, 0, wPadLen);
    bGood = os.write_octet_array(pbPad, wPadLen);
    if (!bGood)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("os.write_octet_array failed. pbPad=%p, wPadLen=%u\n"), pbPad, wPadLen));
        alloc.deallocate(pbPad, wPadLen);
        return CM_ERROR_FAILURE;
    }

    alloc.deallocate(pbPad, wPadLen);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Match::StreamTo(): pop\n")));
    
    return CM_OK;
}


// Length of ofp_match (excluding padding)
ACE_UINT16 COFP13Match::GetStreamLen() const
{
    ACE_UINT16 streamLen = OFP13_MATCH_HEADLEN;

    for (auto &item : m_matchFieldList)
    {
        streamLen += item->GetStreamLen();
    }
    
    return streamLen;
}


// Length of ofp_match (including padding)
ACE_UINT16 COFP13Match::GetEncodeSpace() const
{
    ACE_UINT16 streamLen = GetStreamLen();
    return ((streamLen + 7)/8*8);
}


void COFP13Match::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Match::Dump()\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type=%u, length=%u\n"), m_wType, m_wLength));

    // dump OXM field list
    CM_ASSERT_RETURN_VOID((m_wLength >= OFP13_MATCH_HEADLEN));

    ACE_UINT16 i = 0;
    for (auto &oxmTlv : m_matchFieldList)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("OXM TLV[%u]:\n"), i++));
        oxmTlv->Dump();
    }

    return;
}

#if 0
/* The following restriction, of OpenFlow Spec 1.3.0, is implemented in the function:
   Any given oxm_type may appear in an OpenFlow match at most once, otherwise the switch must generate an error (see 6.4). 
   pre-condition:
   the COpenFlowProtMatch arg passed SHOULD meet the pre-requisite in Table~11: Match fields details. And at present, only 
   the match fields, listed in Table~10: Required match fields, openflow-spec-v1.3.1, are supported. */
CmResult COFP13Match::ToOpenFlowMatch(COpenFlowMatch &aMatch)
{
    using namespace wavplatform::utils;

    CmResult  result    = CM_ERROR_FAILURE;
    FLOWKEY  &matchKey  = aMatch.m_match.key;
    FLOWKEY  &matchMask = aMatch.m_match.mask;
    
    for (auto &oxmTlv : m_matchFieldList)
    {
        CM_ASSERTE_RETURN(OFPXMC_OPENFLOW_BASIC == oxmTlv->GetOxmClass(), CM_ERROR_INVALID_ARG);
        const BYTE *payload = oxmTlv->GetPayload();
        
        switch (oxmTlv->GetOxmField())
        {
            case OFPXMT_OFB_IN_PORT:   /* Switch input port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.inport,   CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.inport,  CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.inport),  payload, sizeof(WORD32));
                ::memset(&(matchMask.inport), OFP_BYTE_MAX_VALUE, sizeof(WORD32));
                break;
            }
            case OFPXMT_OFB_IN_PHY_PORT: /* Switch physical input port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.in_phy_port,   CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.in_phy_port,  CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.in_phy_port),  payload, sizeof(WORD32));
                ::memset(&(matchMask.in_phy_port), OFP_BYTE_MAX_VALUE, sizeof(WORD32));
                break;
            }
            case OFPXMT_OFB_METADATA:  /* Metadata passed between tables. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.metadata,   CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.metadata, CM_ERROR_INVALID_ARG);
                result = FillMetaData(aMatch, oxmTlv);
                CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
                break;
            }
            case OFPXMT_OFB_ETH_DST:   /* Ethernet destination address. */
            {
                for (BYTE i = 0; i < OFP_ETH_ALEN; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.dl_dst[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.dl_dst[i], CM_ERROR_INVALID_ARG);
                }
                FillEthDst(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_ETH_SRC:   /* Ethernet source address. */
            {
                for (BYTE i = 0; i < OFP_ETH_ALEN; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.dl_src[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.dl_src[i], CM_ERROR_INVALID_ARG);
                }
                FillEthSrc(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_ETH_TYPE:  /* Ethernet frame type. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.eth_type,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.eth_type, CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.eth_type),  payload, OXM_OF_ETH_TYPE_BYTES);
                ::memset(&(matchMask.eth_type), OFP_BYTE_MAX_VALUE, OXM_OF_ETH_TYPE_BYTES);
                break;
            }
            case OFPXMT_OFB_VLAN_VID:  /* VLAN id. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.vlan_vid,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.vlan_vid, CM_ERROR_INVALID_ARG);                
                result = FillVlanVid(aMatch, oxmTlv);
                CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
                break;
            }
            case OFPXMT_OFB_VLAN_PCP:  /* VLAN priority. */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.vlan_pcp  & OFP_MATCH_VLAN_PCP_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.vlan_pcp & OFP_MATCH_VLAN_PCP_MASK), CM_ERROR_INVALID_ARG);
                match_set_dl_vlan_pcp(aMatch, *payload);
                break;
            }
            case OFPXMT_OFB_IP_DSCP:   /* IP DSCP (6 bits in ToS field). */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.ip_dscp  & OFP_MATCH_IP_DSCP_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.ip_dscp & OFP_MATCH_IP_DSCP_MASK), CM_ERROR_INVALID_ARG);
                match_set_nw_dscp(aMatch, *payload);
                break;
            }
            case OFPXMT_OFB_IP_ECN:    /* IP ECN (2 bits in ToS field). */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.ip_ecn  & IP_ECN_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.ip_ecn & IP_ECN_MASK), CM_ERROR_INVALID_ARG);
                match_set_nw_ecn(aMatch, *payload);
                break;
            }
            case OFPXMT_OFB_IP_PROTO:  /* IP protocol. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.ip_proto,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.ip_proto, CM_ERROR_INVALID_ARG);
                matchKey.ip_proto   = *payload;
                matchMask.ip_proto  = OFP_BYTE_MAX_VALUE;
                break;
            }
            case OFPXMT_OFB_IPV4_SRC:  /* IPv4 source address. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.ipv4_src,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.ipv4_src, CM_ERROR_INVALID_ARG);
                FillIPV4Src(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_IPV4_DST:  /* IPv4 destination address. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.ipv4_dst,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.ipv4_dst, CM_ERROR_INVALID_ARG);
                FillIPV4Dst(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_TCP_SRC:   /* TCP source port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.tcp_src,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.tcp_src, CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.tcp_src),  payload, sizeof(uint16_t));
                ::memset(&(matchMask.tcp_src), OFP_BYTE_MAX_VALUE, sizeof(uint16_t));
                break;
            }
            case OFPXMT_OFB_TCP_DST:   /* TCP destination port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.tcp_dst,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.tcp_dst, CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.tcp_dst),  payload, sizeof(uint16_t));
                ::memset(&(matchMask.tcp_dst), OFP_BYTE_MAX_VALUE, sizeof(uint16_t));
                break;
            }
            case OFPXMT_OFB_UDP_SRC:   /* UDP source port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.udp_src,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.udp_src, CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.udp_src),  payload, sizeof(uint16_t));
                ::memset(&(matchMask.udp_src), OFP_BYTE_MAX_VALUE, sizeof(uint16_t));
                break;
            }
            case OFPXMT_OFB_UDP_DST:   /* UDP destination port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.udp_dst,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.udp_dst, CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.udp_dst),  payload, sizeof(uint16_t));
                ::memset(&(matchMask.udp_dst), OFP_BYTE_MAX_VALUE, sizeof(uint16_t));
                break;
            }
            case OFPXMT_OFB_SCTP_SRC:  /* SCTP source port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.sctp_src,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.sctp_src, CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.sctp_src),  payload, sizeof(uint16_t));
                ::memset(&(matchMask.sctp_src), OFP_BYTE_MAX_VALUE, sizeof(uint16_t));
                break;
            }
            case OFPXMT_OFB_SCTP_DST:  /* SCTP destination port. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.sctp_dst,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.sctp_dst, CM_ERROR_INVALID_ARG);
                ::memcpy(&(matchKey.sctp_dst),  payload, sizeof(uint16_t));
                ::memset(&(matchMask.sctp_dst), OFP_BYTE_MAX_VALUE, sizeof(uint16_t));
                break;
            }
            // reuse tp_src field(8 Least Significant Bits)
            case OFPXMT_OFB_ICMPV4_TYPE: /* ICMP type. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.icmpv4_type,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.icmpv4_type, CM_ERROR_INVALID_ARG);
                matchKey.icmpv4_type  = *payload;
                matchMask.icmpv4_type = OFP_BYTE_MAX_VALUE;
                break;
            }
            // reuse tp_dst field(8 Least Significant Bits)
            case OFPXMT_OFB_ICMPV4_CODE: /* ICMP code. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.icmpv4_code,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.icmpv4_code, CM_ERROR_INVALID_ARG);
                matchKey.icmpv4_code  = *payload;
                matchMask.icmpv4_code = OFP_BYTE_MAX_VALUE;
                break;
            }
            case OFPXMT_OFB_ARP_OP:      /* ARP opcode. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.arp_op,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.arp_op, CM_ERROR_INVALID_ARG);
                matchKey.arp_op  = *payload;
                matchMask.arp_op = OFP_BYTE_MAX_VALUE;
                break;
            }
            case OFPXMT_OFB_ARP_SPA:   /* ARP source IPv4 address. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.arp_spa,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.arp_spa, CM_ERROR_INVALID_ARG);
                FillArpSpa(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_ARP_TPA:   /* ARP target IPv4 address. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.arp_tpa,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.arp_tpa, CM_ERROR_INVALID_ARG);
                FillArpTpa(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_ARP_SHA:     /* ARP source hardware address. */
            {
                for (BYTE i = 0; i < OFP_ETH_ALEN; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.arp_sha[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.arp_sha[i], CM_ERROR_INVALID_ARG);
                }
                FillArpSha(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_ARP_THA:      /* ARP target hardware address. */
            {
                for (BYTE i = 0; i < OFP_ETH_ALEN; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.arp_tha[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.arp_tha[i], CM_ERROR_INVALID_ARG);
                }
                FillArpTha(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_IPV6_SRC:     /* IPv6 source address. */
            {
                // DO NOT delete, as reserved for linux ipv6 struct
                /*
                #ifdef s6_addr32
                for (BYTE i = 0; i < 4; ++i)
                {
                    CM_ASSERTE_RETURN(0 == aMatch.m_key.m_match_key.ipv6_src.s6_addr32[i],   CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == aMatch.m_mask.m_match_mask.ipv6_src.s6_addr32[i], CM_ERROR_INVALID_ARG);
                }
                #else
                for (BYTE i = 0; i < 16; ++i)
                {
                    CM_ASSERTE_RETURN(0 == aMatch.m_key.m_match_key.ipv6_src.s6_addr[i],   CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == aMatch.m_mask.m_match_mask.ipv6_src.s6_addr[i], CM_ERROR_INVALID_ARG);
                }
                #endif */
                for (BYTE i = 0; i < 16; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.ipv6_src[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.ipv6_src[i], CM_ERROR_INVALID_ARG);
                }
                FillIPV6Addr(matchKey.ipv6_src, matchMask.ipv6_src, oxmTlv);
                break;
            }
            case OFPXMT_OFB_IPV6_DST:      /* IPv6 destination address. */
            {
                // DO NOT delete, as reserved for linux ipv6 struct
                /*#ifdef s6_addr32
                for (BYTE i = 0; i < 4; ++i)
                {
                    CM_ASSERTE_RETURN(0 == aMatch.m_key.m_match_key.ipv6_dst.s6_addr32[i],   CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == aMatch.m_mask.m_match_mask.ipv6_dst.s6_addr32[i], CM_ERROR_INVALID_ARG);
                }
                #else
                for (BYTE i = 0; i < 16; ++i)
                {
                    CM_ASSERTE_RETURN(0 == aMatch.m_key.m_match_key.ipv6_dst.s6_addr[i],   CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == aMatch.m_mask.m_match_mask.ipv6_dst.s6_addr[i], CM_ERROR_INVALID_ARG);
                }
                #endif*/
                for (BYTE i = 0; i < 16; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.ipv6_dst[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.ipv6_dst[i], CM_ERROR_INVALID_ARG);
                }
                FillIPV6Addr(matchKey.ipv6_dst, matchMask.ipv6_dst, oxmTlv);
                break;
            }
            case OFPXMT_OFB_IPV6_FLABEL:    /* IPv6 Flow Label */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.ipv6_label  & IPV6_LABEL_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.ipv6_label & IPV6_LABEL_MASK), CM_ERROR_INVALID_ARG);
                FillIPV6FLabel(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_ICMPV6_TYPE:    /* ICMPv6 type. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.icmpv6_type,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.icmpv6_type, CM_ERROR_INVALID_ARG);
                matchKey.icmpv6_type  = *payload;
                matchMask.icmpv6_type = OFP_BYTE_MAX_VALUE;
                break;
            }
            case OFPXMT_OFB_ICMPV6_CODE: /* ICMPv6 code. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.icmpv6_code,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.icmpv6_code, CM_ERROR_INVALID_ARG);
                matchKey.icmpv6_code  = *payload;
                matchMask.icmpv6_code = OFP_BYTE_MAX_VALUE;
                break;
            }
            case OFPXMT_OFB_IPV6_ND_TARGET: /* Target address for ND. */
            {
                // DO NOT delete, as reserved for linux ipv6 struct
                /*#ifdef s6_addr32
                for (BYTE i = 0; i < 4; ++i)
                {
                    CM_ASSERTE_RETURN(0 == aMatch.m_key.m_match_key.nd_target.s6_addr32[i],   CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == aMatch.m_mask.m_match_mask.nd_target.s6_addr32[i], CM_ERROR_INVALID_ARG);
                }
                #else
                for (BYTE i = 0; i < 16; ++i)
                {
                    CM_ASSERTE_RETURN(0 == aMatch.m_key.m_match_key.nd_target.s6_addr[i],   CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == aMatch.m_mask.m_match_mask.nd_target.s6_addr[i], CM_ERROR_INVALID_ARG);
                }
                #endif*/
                for (BYTE i = 0; i < 16; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.ipv6_nd_target[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.ipv6_nd_target[i], CM_ERROR_INVALID_ARG);
                }
                ::memcpy(matchKey.ipv6_nd_target,  payload, 16);
                ::memset(matchMask.ipv6_nd_target, OFP_BYTE_MAX_VALUE, 16);
                break;
            }
            case OFPXMT_OFB_IPV6_ND_SLL: /* Source link-layer for ND. */
            {
                for (BYTE i = 0; i < OFP_ETH_ALEN; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.ipv6_nd_sll[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.ipv6_nd_sll[i], CM_ERROR_INVALID_ARG);
                }
                FillIpv6NdSll(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_IPV6_ND_TLL:  /* Target link-layer for ND. */
            {
                for (BYTE i = 0; i < OFP_ETH_ALEN; ++i)
                {
                    CM_ASSERTE_RETURN(0 == matchKey.ipv6_nd_tll[i],  CM_ERROR_INVALID_ARG);
                    CM_ASSERTE_RETURN(0 == matchMask.ipv6_nd_tll[i], CM_ERROR_INVALID_ARG);
                }
                FillIpv6NdTll(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_MPLS_LABEL:     /* MPLS label. */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.mpls_label  & OFP_MATCH_MPLS_LABEL_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.mpls_label & OFP_MATCH_MPLS_LABEL_MASK), CM_ERROR_INVALID_ARG);
                matchMask.mpls_label |= OFP_MATCH_MPLS_LABEL_MASK;
                WORD32 mplsLabel = 0;
                ::memcpy(&mplsLabel, payload, sizeof(WORD32));
                matchKey.mpls_label |= (mplsLabel & OFP_MATCH_MPLS_LABEL_MASK);
                break;
            }
            case OFPXMT_OFB_MPLS_TC:        /* MPLS TC. */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.mpls_tc  & OFP_MATCH_MPLS_TC_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.mpls_tc & OFP_MATCH_MPLS_TC_MASK), CM_ERROR_INVALID_ARG);
                matchMask.mpls_tc |= OFP_MATCH_MPLS_TC_MASK;
                BYTE mplsTc = 0;
                ::memcpy(&mplsTc, payload, sizeof(BYTE));
                matchKey.mpls_tc |= (mplsTc & OFP_MATCH_MPLS_TC_MASK);
                break;
            }
            case OFPXMT_OFB_MPLS_BOS:       /* MPLS BoS bit. */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.mpls_bos  & OFP_MATCH_MPLS_BOS_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.mpls_bos & OFP_MATCH_MPLS_BOS_MASK), CM_ERROR_INVALID_ARG);
                matchMask.mpls_bos |= OFP_MATCH_MPLS_BOS_MASK;
                BYTE mplsBos = 0;
                ::memcpy(&mplsBos, payload, sizeof(BYTE));
                matchKey.mpls_bos |= (mplsBos & OFP_MATCH_MPLS_BOS_MASK);
                break;
            }
            case OFPXMT_OFB_PBB_ISID:       /* PBB I-SID. */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.pbb_isid  & OFP_MATCH_PBB_ISID_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.pbb_isid & OFP_MATCH_PBB_ISID_MASK), CM_ERROR_INVALID_ARG);
                FillPbbIsid(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_TUNNEL_ID:      /* Logical Port Metadata. */
            {
                CM_ASSERTE_RETURN(0 == matchKey.tunnel_id,  CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == matchMask.tunnel_id, CM_ERROR_INVALID_ARG);
                FillTunnelId(aMatch, oxmTlv);
                break;
            }
            case OFPXMT_OFB_IPV6_EXTHDR:    /* IPv6 Extension Header pseudo-field */
            {
                CM_ASSERTE_RETURN(0 == (matchKey.ipv6_exthdr  & OFP_MATCH_IPV6_EXTHDR_MASK), CM_ERROR_INVALID_ARG);
                CM_ASSERTE_RETURN(0 == (matchMask.ipv6_exthdr & OFP_MATCH_IPV6_EXTHDR_MASK), CM_ERROR_INVALID_ARG);
                FillIpv6Exthdr(aMatch, oxmTlv);
                break;
            }
            default:
            {
                CM_ERROR_TRACE("COFP13Match::ToOpenFlowMatch(): oxmTlv->GetOxmField()=" << oxmTlv->GetOxmField());
                CM_ASSERTE_RETURN(0, CM_ERROR_INVALID_ARG);
            }
        }
    }

    return CM_OK;
}


/* The logic of our function refered to nx_put_raw(), from ovs, 2013-08-13.
static int
nx_put_raw(struct ofpbuf *b, bool oxm, const struct match *match,
           ovs_be64 cookie, ovs_be64 cookie_mask)
*/
CmResult COFP13Match::FromOpenFlowMatch(const COpenFlowMatch &aMatch)
{
    using namespace wavplatform::utils;
    
    CmResult result = CM_ERROR_FAILURE;
    
    /* In port */
    WORD32 inport     = aMatch.m_match.key.inport;
    WORD32 inportMask = aMatch.m_match.mask.inport;
    result = AppendOxmWithoutMask<WORD32>(OFPXMT_OFB_IN_PORT, inport, inportMask, OFP_WORD32_MAX_VALUE);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

    /* In phy port */
    if ((inportMask != 0) && (inport != 0))
    {
        result = AppendOxmWithoutMask<WORD32>(OFPXMT_OFB_IN_PHY_PORT, 
                                              aMatch.m_match.key.in_phy_port, 
                                              aMatch.m_match.mask.in_phy_port,
                                              OFP_WORD32_MAX_VALUE);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }

    /* Metadata */
    result = AppendOxmWithMask<WORD64>(OFPXMT_OFB_METADATA, 
                                       aMatch.m_match.key.metadata, 
                                       aMatch.m_match.mask.metadata,
                                       OFP_WORD64_MAX_VALUE);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);        
    
    /* Ethernet. */
    result = AppendEthAddrOxm(OFPXMT_OFB_ETH_SRC, aMatch.m_match.key.dl_src, aMatch.m_match.mask.dl_src);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    
    result = AppendEthAddrOxm(OFPXMT_OFB_ETH_DST, aMatch.m_match.key.dl_dst, aMatch.m_match.mask.dl_dst);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

    WORD16 ethType     = aMatch.m_match.key.eth_type;
    WORD16 ethTypeMask = aMatch.m_match.mask.eth_type;
    result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_ETH_TYPE, ethType, ethTypeMask, OFP_WORD16_MAX_VALUE);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

    ethType &= ethTypeMask;

    /* 802.1Q. */
    WORD16 vid_cfi_mask = (VLAN_VID_MASK | VLAN_CFI);
    WORD16 vid = (aMatch.m_match.key.vlan_vid & vid_cfi_mask);
    WORD16 vidMask = (aMatch.m_match.mask.vlan_vid & vid_cfi_mask);
    result = AppendOxmWithMask<WORD16>(OFPXMT_OFB_VLAN_VID, vid, vidMask, vid_cfi_mask);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

    vid &= vidMask;

    if (vid != OFPVID_NONE) 
    {
        result = AppendOxmWithMask<BYTE>(OFPXMT_OFB_VLAN_PCP, aMatch.m_match.key.vlan_pcp, aMatch.m_match.mask.vlan_pcp, OFP_MATCH_VLAN_PCP_MASK);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }

    /* MPLS. */
    if (eth_type_mpls(ethType))
    {
        result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_MPLS_TC, 
                                            aMatch.m_match.key.mpls_tc, 
                                            aMatch.m_match.mask.mpls_tc, 
                                            OFP_MATCH_MPLS_TC_MASK);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_MPLS_BOS, 
                                            aMatch.m_match.key.mpls_bos, 
                                            aMatch.m_match.mask.mpls_bos, 
                                            OFP_MATCH_MPLS_BOS_MASK);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendOxmWithoutMask<WORD32>(OFPXMT_OFB_MPLS_LABEL, 
                                              aMatch.m_match.key.mpls_label,
                                              aMatch.m_match.mask.mpls_label,
                                              OFP_MATCH_MPLS_LABEL_MASK);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }

    /* L3. */
    if (ETH_TYPE_IP == ethType) {
        /* IP. */
        result = AppendOxmWithMask<WORD32>(OFPXMT_OFB_IPV4_SRC, 
                                           aMatch.m_match.key.ipv4_src, 
                                           aMatch.m_match.mask.ipv4_src,
                                           OFP_WORD32_MAX_VALUE);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendOxmWithMask<WORD32>(OFPXMT_OFB_IPV4_DST, 
                                           aMatch.m_match.key.ipv4_dst, 
                                           aMatch.m_match.mask.ipv4_dst,
                                           OFP_WORD32_MAX_VALUE);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendIpOxm(OFP_MATCH_IPV4, aMatch);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    } else if (ETH_TYPE_IPV6 == ethType) {
        /* IPv6. */
        result = AppendIpv6AddrOxm(OFPXMT_OFB_IPV6_SRC, aMatch.m_match.key.ipv6_src, aMatch.m_match.mask.ipv6_src);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendIpv6AddrOxm(OFPXMT_OFB_IPV6_DST, aMatch.m_match.key.ipv6_dst, aMatch.m_match.mask.ipv6_dst);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendOxmWithMask<WORD32>(OFPXMT_OFB_IPV6_FLABEL, 
                                   aMatch.m_match.key.ipv6_label, 
                                   aMatch.m_match.mask.ipv6_label,
                                   OFP_MATCH_IPV6_FLABEL_MASK);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendIpOxm(OFP_MATCH_IPV6, aMatch);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendOxmWithMask<WORD16>(OFPXMT_OFB_IPV6_EXTHDR, 
                                   aMatch.m_match.key.ipv6_exthdr, 
                                   aMatch.m_match.mask.ipv6_exthdr,
                                   OFP_MATCH_IPV6_EXTHDR_MASK);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }
    else if ((ETH_TYPE_ARP == ethType) || (ETH_TYPE_RARP == ethType))
    {
        /* ARP. */
        result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_ARP_OP, 
                                              aMatch.m_match.key.arp_op,
                                              aMatch.m_match.mask.arp_op,
                                              OFP_WORD16_MAX_VALUE);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendOxmWithMask<WORD32>(OFPXMT_OFB_ARP_SPA, 
                                           aMatch.m_match.key.arp_spa, 
                                           aMatch.m_match.mask.arp_spa,
                                           OFP_WORD32_MAX_VALUE);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendOxmWithMask<WORD32>(OFPXMT_OFB_ARP_TPA, 
                                           aMatch.m_match.key.arp_tpa, 
                                           aMatch.m_match.mask.arp_tpa,
                                           OFP_WORD32_MAX_VALUE);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendEthAddrOxm(OFPXMT_OFB_ARP_SHA, aMatch.m_match.key.arp_sha, aMatch.m_match.mask.arp_sha);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

        result = AppendEthAddrOxm(OFPXMT_OFB_ARP_THA, aMatch.m_match.key.arp_tha, aMatch.m_match.mask.arp_tha);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }
    else if (ETH_TYPE_PBB_ISID == ethType)
    {
        result = AppendOxmWithMask<WORD32>(OFPXMT_OFB_PBB_ISID, 
                                           aMatch.m_match.key.pbb_isid, 
                                           aMatch.m_match.mask.pbb_isid,
                                           OFP_MATCH_PBB_ISID_MASK);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);        
    }

    result = AppendOxmWithMask<WORD64>(OFPXMT_OFB_TUNNEL_ID, 
                                       aMatch.m_match.key.tunnel_id, 
                                       aMatch.m_match.mask.tunnel_id,
                                       OFP_WORD64_MAX_VALUE);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);        

    return CM_OK;
}
#endif

#if 0
CmResult COFP13Match::FillMetaData(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv)
{
    bool                hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE         *payload     = apOxmTlv->GetPayload();
    FLOWKEY            &matchKey    = aMatch.m_match.key;
    FLOWKEY            &matchMask   = aMatch.m_match.mask;

    if (FALSE == hasMaskFlag)
    {
        ::memcpy(&(matchKey.metadata),  payload, sizeof(WORD64));
        ::memset(&(matchMask.metadata), OFP_BYTE_MAX_VALUE, sizeof(WORD64));
    }
    else
    {
        WORD64 metaData = 0;
        WORD64 metaDataMask = 0;
        ::memcpy(&metaData, payload, sizeof(WORD64));
        ::memcpy(&metaDataMask, payload + sizeof(WORD64), sizeof(WORD64));

        matchKey.metadata  = (metaData & metaDataMask);
        matchMask.metadata = metaDataMask;
    }

    return CM_OK;
}


CmResult COFP13Match::FillVlanVid(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    const WORD16 vid_mask = (VLAN_VID_MASK | VLAN_CFI);
    
    bool   hasMaskFlag = apOxmTlv->GetHasMask();
    WORD16 vlanVid     = 0;
    ::memcpy(&vlanVid, apOxmTlv->GetPayload(), sizeof(vlanVid));

    if (FALSE == hasMaskFlag)
    {
        match_set_vlan_vid(aMatch, vlanVid);
    }
    else
    {
        WORD16 vlanVidMask = 0;
        ::memcpy(&vlanVidMask, apOxmTlv->GetPayload() + OXM_OF_VLAN_VID_ROUND_BYTE, sizeof(vlanVidMask));
        if (vid_mask == (vlanVidMask & vid_mask))
        {
            match_set_vlan_vid(aMatch, vlanVid);
            return CM_OK;
        }
        else if (COFPMsgCommonUtils::is_all_zeros(reinterpret_cast<const uint8_t *>(&vlanVidMask), OXM_OF_VLAN_VID_ROUND_BYTE))
        {
            match_set_any_vid(aMatch);
            return CM_OK;
        }

        match_set_vlan_vid_masked(aMatch, vlanVid, vlanVidMask);
    }
    
    return CM_OK;
}


/* Sets the VLAN VID that 'flow' matches to 'vid', which is interpreted as an
 * OpenFlow 1.2 "vlan_vid" value, that is, the low 13 bits of 'vlan_tci' (VID
 * plus CFI), with the corresponding 'mask'. */
void COFP13Match::match_set_vlan_vid_masked(COpenFlowMatch &aMatch, WORD16 awVid, WORD16 awMask)
{
    WORD16 vid_mask = (VLAN_VID_MASK | VLAN_CFI);

    awMask &= vid_mask;
    match_key_set_vlan_vid(aMatch, awVid & awMask);
    aMatch.m_match.mask.vlan_vid = awMask;
}


/* Sets the VLAN VID that 'flow' matches to 'vid', which is interpreted as an
 * OpenFlow 1.2 "vlan_vid" value, that is, the low 13 bits of 'vlan_tci' (VID
 * plus CFI). */
void COFP13Match::match_key_set_vlan_vid(COpenFlowMatch &aMatch, WORD16 awVid)
{
    WORD16 mask = (VLAN_VID_MASK | VLAN_CFI);
    aMatch.m_match.key.vlan_vid &= ~mask;
    aMatch.m_match.key.vlan_vid |= (awVid & mask);
}


/* Modifies 'match' so that the VLAN VID is wildcarded.  If the PCP is already
 * wildcarded, then 'match' will match a packet regardless of whether it has an
 * 802.1Q header or not. */
void COFP13Match::match_set_any_vid(COpenFlowMatch &aMatch)
{
    match_set_dl_tci_masked(aMatch, 0, 0);
}


void COFP13Match::match_set_dl_tci_masked(COpenFlowMatch &aMatch, WORD16 awTci, WORD16 awMask)
{
    aMatch.m_match.key.vlan_vid  = (awTci & awMask);
    aMatch.m_match.mask.vlan_vid = awMask;
}


/* Modifies 'match' so that it matches only packets with an 802.1Q header whose
 * PCP equals the low 3 bits of 'dl_vlan_pcp'. */
void COFP13Match::match_set_dl_vlan_pcp(COpenFlowMatch &aMatch, uint8_t abDlVlanPcp)
{
    flow_set_vlan_pcp(aMatch, abDlVlanPcp);
    aMatch.m_match.mask.vlan_pcp |= OFP_MATCH_VLAN_PCP_MASK;
}


/* Sets the VLAN PCP that 'flow' matches to 'pcp', which should be in the
 * range 0...7.
 *
 * This function has no effect on the VLAN ID that 'flow' matches.
 *
 * After calling this function, 'flow' will not match packets without a VLAN
 * header. */
void COFP13Match::flow_set_vlan_pcp(COpenFlowMatch &aMatch, uint8_t pcp)
{
    pcp &= OFP_MATCH_VLAN_PCP_MASK;
    aMatch.m_match.key.vlan_pcp &= (~OFP_MATCH_VLAN_PCP_MASK);
    aMatch.m_match.key.vlan_pcp |= pcp;
}


void COFP13Match::match_set_nw_dscp(COpenFlowMatch &aMatch, uint8_t abNwDscp)
{
    aMatch.m_match.mask.ip_dscp |= OFP_MATCH_IP_DSCP_MASK;
    aMatch.m_match.key.ip_dscp &= (~OFP_MATCH_IP_DSCP_MASK);
    aMatch.m_match.key.ip_dscp |= (abNwDscp & OFP_MATCH_IP_DSCP_MASK);
}


void COFP13Match::match_set_nw_ecn(COpenFlowMatch &aMatch, uint8_t abNwEcn)
{
    aMatch.m_match.mask.ip_ecn |= IP_ECN_MASK;
    aMatch.m_match.key.ip_ecn  &= (~IP_ECN_MASK);
    aMatch.m_match.key.ip_ecn  |= (abNwEcn & IP_ECN_MASK);
}


void COFP13Match::FillEthAddr(uint8_t *apAddr, uint8_t *apAddrMask, const CCmComAutoPtr<COFP13OxmTlv> &apOxmTlv)
{
    CM_ASSERTE_RETURN_VOID(apAddr != NULL);
    CM_ASSERTE_RETURN_VOID(apAddrMask != NULL);

    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();
    if (FALSE == hasMaskFlag)
    {
        ::memcpy(apAddr, payload, OFP_ETH_ALEN);
        ::memset(apAddrMask, OFP_BYTE_MAX_VALUE, OFP_ETH_ALEN);
    }
    else
    {
        const BYTE *mask = payload + sizeof(BYTE) * OFP_ETH_ALEN;
        for (BYTE i = 0; i < OFP_ETH_ALEN; ++i)
        {
            apAddr[i]     = (payload[i] & mask[i]);
            apAddrMask[i] = mask[i];
        }
    }
}


void COFP13Match::FillIPV4Addr(uint32_t &anAddr, uint32_t &anAddrMask, CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();

    ::memcpy(&anAddr, payload, sizeof(uint32_t));

    if (FALSE == hasMaskFlag)
    {
        ::memset(&anAddrMask, OFP_BYTE_MAX_VALUE, sizeof(uint32_t));
    }
    else
    {
        const BYTE *mask = payload + sizeof(uint32_t);
        ::memcpy(&anAddrMask, mask, sizeof(uint32_t));        

        anAddr &= anAddrMask;
    }
}


void COFP13Match::FillIPV6Addr(ofp_in6_addr &aIpv6AddrDst, ofp_in6_addr &aIpv6AddrMaskDst,
                               const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    const BYTE  ipv6AddrLen = sizeof(ofp_in6_addr);
    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();

    ofp_in6_addr ipv6AddrSrc;
    ::memset(&ipv6AddrSrc, 0,       ipv6AddrLen);
    ::memcpy(&ipv6AddrSrc, payload, ipv6AddrLen);

    if (FALSE == hasMaskFlag)
    {
        using namespace wavplatform::utils;
        aIpv6AddrDst     = ipv6AddrSrc;
        aIpv6AddrMaskDst = IN6ADDR_EXACT_INIT;
    }
    else
    {
        ofp_in6_addr ipv6AddrMaskSrc;        
        ::memset(&ipv6AddrMaskSrc, 0,                     ipv6AddrLen);
        ::memcpy(&ipv6AddrMaskSrc, payload + ipv6AddrLen, ipv6AddrLen);
        match_set_ipv6_addr_masked(aIpv6AddrDst, aIpv6AddrMaskDst, ipv6AddrSrc, ipv6AddrMaskSrc);
    }
}


void COFP13Match::FillIPV6Addr(uint8_t aIpv6AddrDst[16], uint8_t aIpv6AddrMaskDst[16],
                  const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    const BYTE  ipv6AddrLen = 16;
    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();

    uint8_t ipv6AddrSrc[16];
    ::memset(&ipv6AddrSrc, 0,       ipv6AddrLen);
    ::memcpy(&ipv6AddrSrc, payload, ipv6AddrLen);

    if (FALSE == hasMaskFlag)
    {
        using namespace wavplatform::utils;
        ::memcpy(aIpv6AddrDst, ipv6AddrSrc, 16);
        ::memset(aIpv6AddrMaskDst, OFP_BYTE_MAX_VALUE, 16);
    }
    else
    {
        uint8_t ipv6AddrMaskSrc[16];
        ::memset(ipv6AddrMaskSrc, 0,                     ipv6AddrLen);
        ::memcpy(ipv6AddrMaskSrc, payload + ipv6AddrLen, ipv6AddrLen);
        match_set_ipv6_addr_masked(aIpv6AddrDst, aIpv6AddrMaskDst, ipv6AddrSrc, ipv6AddrMaskSrc);
    }
}


void COFP13Match::match_set_ipv6_addr_masked(ofp_in6_addr &aIpv6AddrDst, ofp_in6_addr &aIpv6AddrMaskDst, 
                                            const ofp_in6_addr &aIpv6AddrSrc, const ofp_in6_addr &aIpv6AddrMaskSrc)
{
    ipv6_addr_bitand(aIpv6AddrSrc, aIpv6AddrMaskSrc, aIpv6AddrDst);
    aIpv6AddrMaskDst = aIpv6AddrMaskSrc;
}


void COFP13Match::match_set_ipv6_addr_masked(uint8_t aIpv6AddrDst[16], uint8_t aIpv6AddrMaskDst[16], 
                                const uint8_t aIpv6AddrSrc[16], const uint8_t aIpv6AddrMaskSrc[16])
{
    ipv6_addr_bitand(aIpv6AddrSrc, aIpv6AddrMaskSrc, aIpv6AddrDst);
    ::memcpy(aIpv6AddrMaskDst, aIpv6AddrMaskSrc, 16);
}


void COFP13Match::ipv6_addr_bitand(const ofp_in6_addr &aIpv6AddrSrc, const ofp_in6_addr &aIpv6AddrMaskSrc, 
                                   ofp_in6_addr &aIpv6AddrDst)
{
    int i;

#ifdef s6_addr32
    for (i=0; i<4; i++) {
        aIpv6AddrDst.s6_addr32[i] = (aIpv6AddrSrc.s6_addr32[i] & aIpv6AddrMaskSrc.s6_addr32[i]);
    }
#else
    for (i=0; i<16; i++) {
        aIpv6AddrDst.s6_addr[i]   = (aIpv6AddrSrc.s6_addr[i] & aIpv6AddrMaskSrc.s6_addr[i]);
    }
#endif
}


void COFP13Match::ipv6_addr_bitand(const uint8_t aIpv6AddrSrc[16], const uint8_t aIpv6AddrMaskSrc[16], 
                      uint8_t aIpv6AddrDst[16])
{
    int i;

    for (i=0; i<16; i++) {
        aIpv6AddrDst[i] = (aIpv6AddrSrc[i] & aIpv6AddrMaskSrc[i]);
    }
}


void COFP13Match::FillIPV6FLabel(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    using namespace wavplatform::utils;

    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();

    WORD32 ipv6FLabel = 0;
    ::memcpy(&ipv6FLabel, payload, sizeof(WORD32));
    ipv6FLabel &= IPV6_LABEL_MASK;

    if (FALSE == hasMaskFlag)
    {
        aMatch.m_match.key.ipv6_label  = ipv6FLabel;
        aMatch.m_match.mask.ipv6_label = IPV6_LABEL_MASK;
    }
    else
    {
        WORD32 ipv6FLabelMask = 0;
        ::memcpy(&ipv6FLabelMask, payload + sizeof(WORD32), sizeof(WORD32));
        ipv6FLabelMask &= IPV6_LABEL_MASK;
        aMatch.m_match.key.ipv6_label  = (ipv6FLabel & ipv6FLabelMask);
        aMatch.m_match.mask.ipv6_label = ipv6FLabelMask;
    }
}


void COFP13Match::FillPbbIsid(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();

    WORD32 pbbIsid = 0;
    ::memcpy(&pbbIsid, payload, sizeof(WORD32));
    pbbIsid &= OFP_MATCH_PBB_ISID_MASK;

    if (FALSE == hasMaskFlag)
    {
        aMatch.m_match.key.pbb_isid  = pbbIsid;
        aMatch.m_match.mask.pbb_isid = OFP_MATCH_PBB_ISID_MASK;
    }
    else
    {
        WORD32 pbbIsidMask = 0;
        ::memcpy(&pbbIsidMask, payload + sizeof(WORD32), sizeof(WORD32));
        pbbIsidMask &= OFP_MATCH_PBB_ISID_MASK;
        aMatch.m_match.key.pbb_isid  = (pbbIsid & pbbIsidMask);
        aMatch.m_match.mask.pbb_isid = pbbIsidMask;
    }
}


void COFP13Match::FillIpv6Exthdr(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();

    WORD16 ipv6Exthdr = 0;
    ::memcpy(&ipv6Exthdr, payload, sizeof(WORD16));
    ipv6Exthdr &= OFP_MATCH_IPV6_EXTHDR_MASK;

    if (FALSE == hasMaskFlag)
    {
        aMatch.m_match.key.ipv6_exthdr  = ipv6Exthdr;
        aMatch.m_match.mask.ipv6_exthdr = OFP_MATCH_IPV6_EXTHDR_MASK;
    }
    else
    {
        WORD32 ipv6ExthdrMask = 0;
        ::memcpy(&ipv6ExthdrMask, payload + sizeof(WORD16), sizeof(WORD16));
        ipv6ExthdrMask &= OFP_MATCH_IPV6_EXTHDR_MASK;
        aMatch.m_match.key.ipv6_exthdr  = (ipv6Exthdr & ipv6ExthdrMask);
        aMatch.m_match.mask.ipv6_exthdr = ipv6ExthdrMask;
    }
}


void COFP13Match::FillTunnelId(COpenFlowMatch &aMatch, const CCmComAutoPtr<COFP13OxmTlv> apOxmTlv)
{
    bool        hasMaskFlag = apOxmTlv->GetHasMask();
    const BYTE *payload     = apOxmTlv->GetPayload();

    ::memcpy(&(aMatch.m_match.key.tunnel_id), payload, sizeof(WORD64));

    if (FALSE == hasMaskFlag)
    {
        ::memset(&(aMatch.m_match.mask.tunnel_id), OFP_BYTE_MAX_VALUE, sizeof(WORD64));
    }
    else
    {
        WORD64 tunnelIdMask = 0;
        ::memcpy(&tunnelIdMask, payload + sizeof(WORD64), sizeof(WORD64));
        aMatch.m_match.key.tunnel_id   &= tunnelIdMask;
        aMatch.m_match.mask.tunnel_id   = tunnelIdMask;
    }
}


CmResult COFP13Match::AppendEthAddrOxm(BYTE abOxmField, const uint8_t aAddrVal[ETH_ADDR_LENTH], 
                                       const uint8_t aAddrMask[ETH_ADDR_LENTH])
{
    CM_ASSERTE_RETURN(((OFPXMT_OFB_ETH_SRC == abOxmField) 
                      || (OFPXMT_OFB_ETH_DST == abOxmField)
                      || (OFPXMT_OFB_ARP_SHA == abOxmField)
                      || (OFPXMT_OFB_ARP_THA == abOxmField)), 
                      CM_ERROR_INVALID_ARG);
    
    if (!COFPMsgCommonUtils::eth_addr_is_zero(aAddrMask)) {
        CmResult result = CM_ERROR_FAILURE;
        
        CCmComAutoPtr<COFP13OxmTlv> oxm = new COFP13OxmTlv();
        CM_ASSERTE_RETURN(oxm, CM_ERROR_OUT_OF_MEMORY);
        
        oxm->SetOxmField(abOxmField);

        if (COFPMsgCommonUtils::eth_mask_is_exact(aAddrMask)) {
            oxm->SetHasMask();
            result = oxm->SetPayload(aAddrVal, ETH_ADDR_LENTH);
        } else {
            oxm->SetHasMask(TRUE);
            result = oxm->SetPayload(aAddrVal, ETH_ADDR_LENTH, aAddrMask);
        }
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
        
        result = AppendOxmTlv(oxm);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }

    return CM_OK;
}


CmResult COFP13Match::AppendIpv6AddrOxm(BYTE abOxmField, const uint8_t aAddrVal[16], 
                                       const uint8_t aAddrMask[16])
{
    CM_ASSERTE_RETURN(((OFPXMT_OFB_IPV6_SRC == abOxmField) || (OFPXMT_OFB_IPV6_DST == abOxmField)), 
                      CM_ERROR_INVALID_ARG);
    
    if (!COFPMsgCommonUtils::ipv6_addr_is_zero(aAddrMask)) {
        CmResult result = CM_ERROR_FAILURE;
        
        CCmComAutoPtr<COFP13OxmTlv> oxm = new COFP13OxmTlv();
        CM_ASSERTE_RETURN(oxm, CM_ERROR_OUT_OF_MEMORY);
        
        oxm->SetOxmField(abOxmField);

        if (COFPMsgCommonUtils::ipv6_mask_is_exact(aAddrMask)) {
            oxm->SetHasMask();
            result = oxm->SetPayload(aAddrVal, 16);
        } else {
            oxm->SetHasMask(TRUE);
            result = oxm->SetPayload(aAddrVal, 16, aAddrMask);
        }
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
        
        result = AppendOxmTlv(oxm);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }

    return CM_OK;
}


template<typename T> CmResult COFP13Match::AppendOxmWithMask(BYTE abFieldType, T abPayload, T abMask, T abFullMask)
{
    if ((abMask & abFullMask) != 0)
    {
        CmResult result = CM_ERROR_FAILURE;
        
        CCmComAutoPtr<COFP13OxmTlv> oxm = new COFP13OxmTlv();
        CM_ASSERTE_RETURN(oxm.ParaIn() != nullptr, CM_ERROR_OUT_OF_MEMORY);
        
        oxm->SetOxmField(abFieldType);
    
        if (abFullMask == (abMask & abFullMask)) {
            oxm->SetHasMask();
            result = oxm->SetPayload(reinterpret_cast<const BYTE *>(&abPayload), sizeof(T));
        } else {
            oxm->SetHasMask(TRUE);
            result = oxm->SetPayload(reinterpret_cast<const BYTE *>(&abPayload), 
                                     sizeof(T), 
                                     reinterpret_cast<const BYTE *>(&abMask));
        }
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
        
        result = AppendOxmTlv(oxm);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }


    return CM_OK;
}


CmResult COFP13Match::AppendArrayOxmWithoutMask(BYTE abFieldType, const BYTE *apPayload, 
                                               const BYTE *apMask, WORD16 awArraySize)
{
    if (!COFPMsgCommonUtils::array_is_zero(apMask, awArraySize))
    {
        CCmComAutoPtr<COFP13OxmTlv> oxm = new COFP13OxmTlv();
        CM_ASSERTE_RETURN(oxm.ParaIn() != nullptr, CM_ERROR_OUT_OF_MEMORY);

        oxm->SetOxmField(abFieldType);
        oxm->SetHasMask();
        oxm->SetPayload(apPayload, awArraySize);

        CmResult result = AppendOxmTlv(oxm);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }

    return CM_OK;
}


template<typename T> CmResult COFP13Match::AppendOxmWithoutMask(BYTE abFieldType, T abPayload, T abMask, T abFullMask)
{
    if ((abMask & abFullMask) != 0)
    {
        CCmComAutoPtr<COFP13OxmTlv> oxm = new COFP13OxmTlv();
        CM_ASSERTE_RETURN(oxm.ParaIn() != nullptr, CM_ERROR_OUT_OF_MEMORY);

        oxm->SetOxmField(abFieldType);
        oxm->SetHasMask();
        oxm->SetPayload(reinterpret_cast<const BYTE *>(&abPayload), sizeof(T));

        CmResult result = AppendOxmTlv(oxm);
        CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
    }

    return CM_OK;
}


CmResult COFP13Match::AppendIpOxm(ofpIpType aIpType, const COpenFlowMatch &aMatch)
{
    CmResult result = CM_ERROR_FAILURE;
    
    result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_IP_DSCP, 
                                        aMatch.m_match.key.ip_dscp, 
                                        aMatch.m_match.mask.ip_dscp, 
                                        OFP_MATCH_IP_DSCP_MASK);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

    result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_IP_ECN, 
                                        aMatch.m_match.key.ip_ecn, 
                                        aMatch.m_match.mask.ip_ecn, 
                                        IP_ECN_MASK);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

    uint8_t ip_proto = aMatch.m_match.key.ip_proto;
    uint8_t ip_proto_mask = aMatch.m_match.mask.ip_proto;
    result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_IP_PROTO, 
                                        ip_proto,
                                        ip_proto_mask,
                                        OFP_BYTE_MAX_VALUE);
    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

    ip_proto &= ip_proto_mask;

    if (ip_proto != 0)
    {
        if (IPPROTO_TCP == ip_proto)
        {
            result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_TCP_SRC, 
                                                  aMatch.m_match.key.tcp_src,
                                                  aMatch.m_match.mask.tcp_src,
                                                  OFP_WORD16_MAX_VALUE);
            CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

            result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_TCP_DST, 
                                                  aMatch.m_match.key.tcp_dst,
                                                  aMatch.m_match.mask.tcp_dst,
                                                  OFP_WORD16_MAX_VALUE);
            CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
        }
        else if (IPPROTO_UDP == ip_proto)
        {
            result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_UDP_SRC, 
                                                  aMatch.m_match.key.udp_src,
                                                  aMatch.m_match.mask.udp_src,
                                                  OFP_WORD16_MAX_VALUE);
            CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

            result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_UDP_DST, 
                                                  aMatch.m_match.key.udp_dst,
                                                  aMatch.m_match.mask.udp_dst,
                                                  OFP_WORD16_MAX_VALUE);
            CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
        }
        else if (IPPROTO_SCTP == ip_proto)
        {
            result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_SCTP_SRC, 
                                                  aMatch.m_match.key.sctp_src,
                                                  aMatch.m_match.mask.sctp_src,
                                                  OFP_WORD16_MAX_VALUE);
            CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

            result = AppendOxmWithoutMask<WORD16>(OFPXMT_OFB_SCTP_DST, 
                                                  aMatch.m_match.key.sctp_dst,
                                                  aMatch.m_match.mask.sctp_dst,
                                                  OFP_WORD16_MAX_VALUE);
            CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
        }

        if (OFP_MATCH_IPV4 == aIpType)
        {
            if (IPPROTO_ICMP == ip_proto)
            {
                result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_ICMPV4_TYPE, 
                                                    aMatch.m_match.key.icmpv4_type,
                                                    aMatch.m_match.mask.icmpv4_type,
                                                    OFP_BYTE_MAX_VALUE);
                CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
            
                result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_ICMPV4_CODE, 
                                                    aMatch.m_match.key.icmpv4_code,
                                                    aMatch.m_match.mask.icmpv4_code,
                                                    OFP_BYTE_MAX_VALUE);
                CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
            }
        }
        else
        {
            if (IPPROTO_ICMPV6 == ip_proto)
            {
                uint8_t icmpv6_type      = aMatch.m_match.key.icmpv6_type;
                uint8_t icmpv6_type_mask = aMatch.m_match.mask.icmpv6_type;
                result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_ICMPV6_TYPE, 
                                                    icmpv6_type,
                                                    icmpv6_type_mask,
                                                    OFP_BYTE_MAX_VALUE);
                CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
            
                result = AppendOxmWithoutMask<BYTE>(OFPXMT_OFB_ICMPV6_CODE, 
                                                    aMatch.m_match.key.icmpv6_code,
                                                    aMatch.m_match.mask.icmpv6_code,
                                                    OFP_BYTE_MAX_VALUE);
                CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);

                icmpv6_type &= icmpv6_type_mask;
                
                if ((ND_NEIGHBOR_SOLICIT == icmpv6_type)
                    || (ND_NEIGHBOR_ADVERT == icmpv6_type))
                {
                    result = AppendArrayOxmWithoutMask(OFPXMT_OFB_IPV6_ND_TARGET,
                                                  aMatch.m_match.key.ipv6_nd_target,
                                                  aMatch.m_match.mask.ipv6_nd_target,
                                                  16);
                    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
                }
                
                if (ND_NEIGHBOR_SOLICIT == icmpv6_type)
                {
                    result = AppendArrayOxmWithoutMask(OFPXMT_OFB_IPV6_ND_SLL,
                                                  aMatch.m_match.key.ipv6_nd_sll,
                                                  aMatch.m_match.mask.ipv6_nd_sll,
                                                  ETH_ADDR_LENTH);
                    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
                }
                
                if (ND_NEIGHBOR_ADVERT  == icmpv6_type)
                {
                    result = AppendArrayOxmWithoutMask(OFPXMT_OFB_IPV6_ND_TLL,
                                                  aMatch.m_match.key.ipv6_nd_tll,
                                                  aMatch.m_match.mask.ipv6_nd_tll,
                                                  ETH_ADDR_LENTH);
                    CM_ASSERTE_RETURN(CM_SUCCEEDED(result), result);
                }                
            }
        }
    }

    return CM_OK;
}
#endif

template<typename T> 
bool COFP13Match::CheckPrevOxm(const std::list<COFP13OxmTlv *> aMatchFieldList, 
                 ACE_CDR::Octet abExpectType, 
                 T aExpectVal1, 
                 T aExpectVal2)
{
    for (auto &item : aMatchFieldList)
    {
        ACE_CDR::Octet bTmpFieldVal      = item->GetOxmField();
        const ACE_CDR::Octet *apTmpField = item->GetPayload();
        T bTmpFieldPayload = *((T *)apTmpField);
        if (abExpectType == bTmpFieldVal)
        {
            if((aExpectVal1 == bTmpFieldPayload)
                || ((aExpectVal2 != 0) && (aExpectVal2 == bTmpFieldPayload)))
            {
                return true;
            }
        }
    }

    return false;
}


COFP13OxmTlv::COFP13OxmTlv(
    ACE_CDR::Octet        abPayloadLen,
    const ACE_CDR::Octet *apPayload,
    ACE_CDR::Octet        abOxmFiled,
    ACE_CDR::Octet        abOxmHasMask,
    ACE_UINT16      awOxmClass)
    : m_wOxmClass(awOxmClass)
    , m_bOxmFieldAndHasMaskFlag((abOxmFiled << 1) | (abOxmHasMask & 0X01))    // oxm_field:7 oxm_hasmask:1
    , m_bOxmLen(0)          // size of the used memory for payload, while m_wPayloadMemSize is the total size of memory.
    , m_pPayload(NULL)
    , m_wPayloadMemSize(0)  // size of the memory for payload, while m_bOxmLen is the size of used memory.
{
    if (0 == abPayloadLen)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13OxmTlv::COFP13OxmTlv(): abPayloadLen=0\n")));
        return;
    }
    
    m_pPayload = m_alloc.allocate(abPayloadLen);
    CM_ASSERT_RETURN_VOID((m_pPayload != NULL));
    m_wPayloadMemSize = abPayloadLen;
    ::memset(m_pPayload, 0, abPayloadLen);

    if (NULL == apPayload)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13OxmTlv::COFP13OxmTlv(): apPayload=NULL\n")));
        return;
    }

    ::memcpy(m_pPayload, apPayload, abPayloadLen);
    m_bOxmLen = abPayloadLen;

    return;
}


COFP13OxmTlv::~COFP13OxmTlv()
{
    if (NULL != m_pPayload)
    {
        m_alloc.deallocate(m_pPayload, m_wPayloadMemSize);
    }
}


bool COFP13OxmTlv::GetHasMask() const
{
    if (1 == (m_bOxmFieldAndHasMaskFlag&0X01))
    {
        return true;
    }

    return false;
}


void COFP13OxmTlv::SetOxmField(ACE_CDR::Octet abField)
{
    m_bOxmFieldAndHasMaskFlag &= 0X01;
    m_bOxmFieldAndHasMaskFlag |= (abField << 1);
}


void COFP13OxmTlv::SetHasMask(bool abHasMask)
{
    m_bOxmFieldAndHasMaskFlag &= 0XFE;
    if (abHasMask)
    {
        m_bOxmFieldAndHasMaskFlag |= 0X01;
    }
}


CmResult COFP13OxmTlv::SetPayload(const ACE_CDR::Octet *apPayload, ACE_CDR::Octet abPayloadLen, const ACE_CDR::Octet *apPayloadMask)
{
    CM_ASSERT_RETURN(apPayload,         CM_ERROR_FAILURE);
    CM_ASSERT_RETURN(abPayloadLen != 0, CM_ERROR_FAILURE);

    if (NULL == apPayloadMask)
    {
        return COFPMsgMemoryMgr::Copy(m_alloc, m_pPayload, m_wPayloadMemSize, m_bOxmLen,
                                      apPayload, abPayloadLen);
    }
    else
    {
        ACE_UINT16 dataSize = 2 * abPayloadLen;
        ACE_CDR::Octet  *data     = m_alloc.allocate(dataSize);
        CM_ASSERT_RETURN(data, CM_ERROR_OUT_OF_MEMORY);
        
        ::memset(data,                0,             dataSize);
        ::memcpy(data,                apPayload,     abPayloadLen);
        ::memcpy(data + abPayloadLen, apPayloadMask, abPayloadLen);
        CmResult result = COFPMsgMemoryMgr::Copy(m_alloc, m_pPayload, m_wPayloadMemSize, m_bOxmLen,
                                                 data, dataSize);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
        
        m_alloc.deallocate(data, dataSize);
        return CM_OK;
    }
}


CmResult COFP13OxmTlv::
DecodeOxmTlv(ACE_InputCDR &is, COFP13OxmTlv * &apOxmTlv)
{
    CmResult result = CM_ERROR_FAILURE;
    COFP13OxmTlv *pOxmTlv = new COFP13OxmTlv();
    CM_ASSERT_RETURN(pOxmTlv != nullptr, CM_ERROR_OUT_OF_MEMORY);

    result = pOxmTlv->StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    apOxmTlv = pOxmTlv;
    
    return CM_OK;
}  

// for inteface IOFPMsgParser
CmResult COFP13OxmTlv::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet bOxmLen = 0;
    
    is>>m_wOxmClass;
    is>>m_bOxmFieldAndHasMaskFlag;
    is>>bOxmLen;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    if (m_wPayloadMemSize < bOxmLen)
    {
        if (m_pPayload != NULL)
        {
            m_alloc.deallocate(m_pPayload, m_wPayloadMemSize);
            m_pPayload = NULL;
            m_wPayloadMemSize = 0;
            m_bOxmLen = 0;
        }
        
        m_pPayload = m_alloc.allocate(bOxmLen);
        CM_ASSERT_RETURN((m_pPayload != NULL), CM_ERROR_OUT_OF_MEMORY);
        m_wPayloadMemSize = bOxmLen;
    }

    //CmResult result = ConvertPayloadByteOrder();
    //CM_ASSERTE_RETURN(CM_SUCCEEDED(result), CM_ERROR_FAILURE);
    
    bGood = is.read_octet_array(m_pPayload, bOxmLen);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    m_bOxmLen = bOxmLen;
    
    return CM_OK;
}


CmResult COFP13OxmTlv::StreamTo(ACE_OutputCDR &os)
{
    os<<m_wOxmClass;
    os<<m_bOxmFieldAndHasMaskFlag;
    os<<m_bOxmLen;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    //CmResult result = ConvertPayloadByteOrder();
    //CM_ASSERTE_RETURN(CM_SUCCEEDED(result), CM_ERROR_FAILURE);
    
    bGood = os.write_octet_array(m_pPayload, m_bOxmLen);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13OxmTlv::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13OxmTlv::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("class=%u, field=%u, hasMask=%u, OxmLen=%u\n"), 
               m_wOxmClass, (m_bOxmFieldAndHasMaskFlag>>1), (m_bOxmFieldAndHasMaskFlag&0X01), m_bOxmLen));
    if (m_bOxmLen > 0)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("payload[0]=%u\n"), *(m_pPayload)));
        if (m_bOxmLen > 1)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("last data, payload[%u]=%u\n"), m_bOxmLen-1, m_pPayload[m_bOxmLen-1]));
        }
    }
    
    return;
}


CmResult COFP13OxmTlv::ConvertPayloadByteOrder()
{
    CM_ASSERT_RETURN(COFP13Match::OFPXMC_OPENFLOW_BASIC == m_wOxmClass, CM_ERROR_INVALID_ARG);

    const ACE_CDR::Octet field = GetOxmField();
    const bool hasMaskFlag = GetHasMask();

    // It's not necessary to change the byte order of MAC.
    if (isHardwareAddr(field)
        || isIPv6Addr(field)
        || ((false == hasMaskFlag) && (OFP_SIZEOF_BYTE == m_wPayloadMemSize))
        || ((true == hasMaskFlag) && (OFP_SIZEOF_WORD16 == m_wPayloadMemSize)))
    {
        goto END;
    }

    if (false == hasMaskFlag)
    {
        CM_ASSERT_RETURN((OFP_SIZEOF_WORD16 == m_wPayloadMemSize)
                          || (OFP_SIZEOF_WORD32 == m_wPayloadMemSize)
                          || (OFP_SIZEOF_WORD64 == m_wPayloadMemSize), CM_ERROR_INVALID_ARG);
        if (OFP_SIZEOF_WORD64 == m_wPayloadMemSize)
        {
            ACE_CDR::swap_8(reinterpret_cast<const char *>(m_pPayload), 
                            reinterpret_cast<char *>(m_pPayload));
        }
        else if (OFP_SIZEOF_WORD32 == m_wPayloadMemSize)
        {
            ACE_CDR::swap_4(reinterpret_cast<const char *>(m_pPayload), 
                            reinterpret_cast<char *>(m_pPayload));
        }
        else if (OFP_SIZEOF_WORD16 == m_wPayloadMemSize)
        {
            ACE_CDR::swap_2(reinterpret_cast<const char *>(m_pPayload), 
                            reinterpret_cast<char *>(m_pPayload));
        }
        else
        {
            CM_ASSERT_RETURN(0, CM_ERROR_INVALID_ARG);            
        }
    }
    else
    {
        // Convert byte order of field value and its mask seperately.
        CM_ASSERT_RETURN((2 * OFP_SIZEOF_WORD64 == m_wPayloadMemSize)
                          || (OFP_SIZEOF_WORD64 == m_wPayloadMemSize)
                          || (OFP_SIZEOF_WORD32 == m_wPayloadMemSize), CM_ERROR_INVALID_ARG);
        if (OFP_SIZEOF_WORD32 == m_wPayloadMemSize)
        {
            ACE_CDR::swap_2(reinterpret_cast<const char *>(m_pPayload), 
                            reinterpret_cast<char *>(m_pPayload));
            ACE_CDR::swap_2(reinterpret_cast<const char *>(m_pPayload + OFP_SIZEOF_WORD16), 
                            reinterpret_cast<char *>(m_pPayload + OFP_SIZEOF_WORD16));
        }
        else if (OFP_SIZEOF_WORD64 == m_wPayloadMemSize)
        {
            ACE_CDR::swap_4(reinterpret_cast<const char *>(m_pPayload), 
                            reinterpret_cast<char *>(m_pPayload));
            ACE_CDR::swap_4(reinterpret_cast<const char *>(m_pPayload + OFP_SIZEOF_WORD32), 
                            reinterpret_cast<char *>(m_pPayload + OFP_SIZEOF_WORD32));
        }
        else if (OFP_SIZEOF_WORD64 * 2 == m_wPayloadMemSize)
        {
            ACE_CDR::swap_8(reinterpret_cast<const char *>(m_pPayload), 
                            reinterpret_cast<char *>(m_pPayload));
            ACE_CDR::swap_8(reinterpret_cast<const char *>(m_pPayload + OFP_SIZEOF_WORD64), 
                            reinterpret_cast<char *>(m_pPayload + OFP_SIZEOF_WORD64));
        }
        else
        {
            CM_ASSERT_RETURN(0, CM_ERROR_INVALID_ARG);            
        }
    }

END:    
    return CM_OK;
}



