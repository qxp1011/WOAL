/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPAction.cpp
 * Description:      OpenFlow action source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-12
 *
 * History 1:
 *     Date:         2013-12-15 
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: adapt to ACE
**********************************************************************/
#include "ByteOrder.h"
#include "COFPAction.h"
#include "OFPMessageUtils.h"
#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"
#include "CmError.h"

using namespace openflow;

COFP13Action::COFP13Action(ACE_UINT16 awType, ACE_UINT16 awLen)
    : m_wType(awType)
    , m_wLen(0)
{
    CM_ASSERT_RETURN_VOID((OFP13_ACTION_HEADER_LEN == awLen) || (0 == (awLen % 8)));
    m_wLen = awLen;
}

CmResult COFP13Action::
DecodeAction(ACE_InputCDR &is, COFP13Action * &apAction)
{
    CmResult      lRet  = CM_ERROR_FAILURE;
    ACE_UINT16    wType = 0;
    COFP13Action *pAct  = nullptr;
    
    bool bGood = is.pre_read(&wType, sizeof(wType));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    ACE_CDR::swap_2(reinterpret_cast<const char *>(&wType), reinterpret_cast<char *>(&wType));

    switch (wType)
    {
        case OFPAT_OUTPUT:
        {
            pAct = new COFP13ActOutput();
            break;
        }
        case OFPAT_GROUP:
        {
            pAct = new COFP13ActGroup();
            break;
        }
        case OFPAT_SET_QUEUE:
        {
            pAct = new COFP13ActSetQueue();
            break;
        }
        case OFPAT_SET_MPLS_TTL:
        {
            pAct = new COFP13ActSetMplsTtl();
            break;
        }
        case OFPAT_DEC_MPLS_TTL:
        {
            pAct = new COFP13ActDecreMplsTtl();
            break;
        }
        case OFPAT_SET_NW_TTL:
        {
            pAct = new COFP13ActSetNwTtl();
            break;
        }
        case OFPAT_DEC_NW_TTL:
        {
            pAct = new COFP13ActDecreNwTtl();
            break;
        }
        case OFPAT_COPY_TTL_OUT:
        {
            pAct = new COFP13ActCopyTtlOut();
            break;
        }
        case OFPAT_COPY_TTL_IN:
        {
            pAct = new COFP13ActCopyTtlIn();
            break;
        }
        case OFPAT_PUSH_VLAN:
        case OFPAT_PUSH_MPLS:
        case OFPAT_PUSH_PBB:
        {
            pAct = new COFP13ActPush(wType);
            break;
        }
        case OFPAT_POP_VLAN:
        {
            pAct = new COFP13ActPopVlan();
            break;
        }
        case OFPAT_POP_PBB:
        {
            pAct = new COFP13ActPopPbb();
            break;
        }
        case OFPAT_POP_MPLS:
        {
            pAct = new COFP13ActPopMpls();
            break;
        }
        case OFPAT_SET_FIELD:
        {
            pAct = new COFP13ActSetField();
            break;
        }
        default:
        {
            break;
        }
    }
    
    CM_ASSERT_RETURN(pAct != nullptr, CM_ERROR_OUT_OF_MEMORY);

    lRet = pAct->StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13Action::DecodeAction(): fail in StreamFrom(), lRet=%l\n"), lRet));
        return lRet;
    }

    apAction = pAct;
    
    return CM_OK;
}

CmResult COFP13Action::StreamFrom(ACE_InputCDR &is)
{
    is>>m_wType;
    is>>m_wLen;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13Action::StreamTo(ACE_OutputCDR &os)
{
    m_wLen = GetStreamLen();    // + OFP13_ACTION_HEADER_LEN;
    
    os<<m_wType;
    os<<m_wLen;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13Action::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Action::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type=%u, len=%u\n"), m_wType, m_wLen));
}


COFP13ActOnlyHeader::COFP13ActOnlyHeader(ACE_UINT16 awActType)
    : COFP13Action(awActType, GetStreamLen())
{};


CmResult COFP13ActOnlyHeader::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[4] = {0};

    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    bool bGood = is.read_octet_array(pad, 4);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActOnlyHeader::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[4] = {0};

    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    bool bGood = os.write_octet_array(pad, 4);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActOnlyHeader::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActOnlyHeader::Dump():\n")));
    return;
}


COFP13ActOutput::COFP13ActOutput(ACE_UINT32 anOutport, ACE_UINT16 awMaxLen)
    : COFP13Action(OFPAT_OUTPUT, GetStreamLen())
    , m_dwOutPort(anOutport)
    , m_wMaxLen(0)
{
    CM_ASSERT_RETURN_VOID((OFPCML_NO_BUFFER == awMaxLen) || (awMaxLen <= OFPCML_MAX));
    m_wMaxLen = awMaxLen;
}


CmResult COFP13ActOutput::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[6] = {0};

    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_dwOutPort;
    is>>m_wMaxLen;
	
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    bGood = is.read_octet_array(pad, 6);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActOutput::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[6] = {0};
    
    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_dwOutPort;
    os<<m_wMaxLen;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    bGood = os.write_octet_array(pad, 6);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActOutput::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActOutput::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("outPort=%u, maxLen=%u"), m_dwOutPort, m_wMaxLen));
}


COFP13ActGroup::COFP13ActGroup(ACE_UINT32 anGroupId)
    : COFP13Action(OFPAT_GROUP, GetStreamLen())
    , m_nGroupId(anGroupId)
{
}


CmResult COFP13ActGroup::StreamFrom(ACE_InputCDR &is)
{
    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_nGroupId;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActGroup::StreamTo(ACE_OutputCDR &os)
{
    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    os<<m_nGroupId;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActGroup::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActGroup::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("groupId=%u\n"), m_nGroupId));
}


COFP13ActSetQueue::COFP13ActSetQueue(ACE_UINT32 anQueueId)
    : COFP13Action(OFPAT_SET_QUEUE, GetStreamLen())
    , m_dwQueueId(anQueueId)
{
}


CmResult COFP13ActSetQueue::StreamFrom(ACE_InputCDR &is)
{
    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_dwQueueId;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActSetQueue::StreamTo(ACE_OutputCDR &os)
{
    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    os<<m_dwQueueId;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActSetQueue::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActSetQueue::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("queueId=%u\n"), m_dwQueueId));
}


COFP13ActSetMplsTtl::COFP13ActSetMplsTtl(ACE_CDR::Octet abMplsTtl)
    : COFP13Action(OFPAT_SET_MPLS_TTL, GetStreamLen())
    , m_bMplsTtl(abMplsTtl)
{
}


CmResult COFP13ActSetMplsTtl::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[3] = {0};

    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_bMplsTtl;
    is.read_octet_array(pad, sizeof(pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActSetMplsTtl::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[3] = {0};

    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_bMplsTtl;
    os.write_octet_array(pad, sizeof pad);

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActSetMplsTtl::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActSetMplsTtl::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("mplsTtl=%u\n"), m_bMplsTtl));
}


COFP13ActDecreMplsTtl::COFP13ActDecreMplsTtl()
    : COFP13ActOnlyHeader(OFPAT_DEC_MPLS_TTL)
{
}


COFP13ActSetNwTtl::COFP13ActSetNwTtl(ACE_CDR::Octet abNwTtl)
    : COFP13Action(OFPAT_SET_NW_TTL, GetStreamLen())
    , m_bNwTtl(abNwTtl)
{
}


CmResult COFP13ActSetNwTtl::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[3] = {0};

    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_bNwTtl;
    is.read_octet_array(pad, 3);

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActSetNwTtl::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[3] = {0};

    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_bNwTtl;
    os.write_octet_array(pad, 3);

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActSetNwTtl::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActSetNwTtl::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("nwTtl=%u\n"), m_bNwTtl));
}


COFP13ActDecreNwTtl::COFP13ActDecreNwTtl()
    : COFP13ActOnlyHeader(OFPAT_DEC_NW_TTL)
{
}


COFP13ActCopyTtlOut::COFP13ActCopyTtlOut()
    : COFP13ActOnlyHeader(OFPAT_COPY_TTL_OUT)
{
}


COFP13ActCopyTtlIn::COFP13ActCopyTtlIn()
    : COFP13ActOnlyHeader(OFPAT_COPY_TTL_IN)
{
}


COFP13ActPush::COFP13ActPush(ACE_UINT16 awType, ACE_UINT16 awEtherType)
    : COFP13Action(awType, GetStreamLen())
    , m_wEtherType(awEtherType)
{
    if ((awType != OFPAT_PUSH_VLAN)
        && (awType != OFPAT_PUSH_MPLS)
        && (awType != OFPAT_PUSH_PBB))
    {
        ACE_ASSERT(0);
    }
}


CmResult COFP13ActPush::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16 pad = 0;

    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_wEtherType;    
    is>>pad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActPush::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT16 pad = 0;

    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_wEtherType;
    os<<pad;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActPush::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActPush::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("EtherType=%u\n"), m_wEtherType));
}


COFP13ActPopVlan::COFP13ActPopVlan()
    : COFP13ActOnlyHeader(OFPAT_POP_VLAN)
{
}


COFP13ActPopPbb::COFP13ActPopPbb()
    : COFP13ActOnlyHeader(OFPAT_POP_PBB)
{
}


COFP13ActPopMpls::COFP13ActPopMpls(ACE_UINT16 awEtherType)
    : COFP13Action(OFPAT_POP_MPLS, GetStreamLen())
    , m_wEtherType(awEtherType)
{
}


CmResult COFP13ActPopMpls::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16 pad = 0;

    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_wEtherType;    
    is>>pad;
	
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP13ActPopMpls::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT16 pad = 0;

    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_wEtherType;
    os<<pad;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


void COFP13ActPopMpls::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActPopMpls::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("etherType=%u\n"), m_wEtherType));
}


COFP13ActSetField::COFP13ActSetField(
        ACE_CDR::Octet         abOxmPayloadLen,
        const ACE_CDR::Octet  *apOxmPayload,
        ACE_CDR::Octet         abOxmFiled,
        ACE_CDR::Octet         abOxmHasMask,
        ACE_UINT16       awOxmClass)
    : COFP13Action(OFPAT_SET_FIELD)
    , m_oxmTlv(abOxmPayloadLen, apOxmPayload, abOxmFiled, 0, awOxmClass)
{
}


ACE_UINT16 COFP13ActSetField::CalcTotalPadLen() const
{
#if 0
    WORD16 wOxmTlvLen = (abOxmPayloadLen + OFP13_MATCH_OXMTLV_HEADLEN);
    
    return ((wOxmTlvLen+7)/8*8 - (wOxmTlvLen + 4));
#endif
    ACE_UINT16 length = m_oxmTlv.GetStreamLen();

    return ((length+4+7)/8*8 - (length + 4));

}
    
    
bool COFP13ActSetField::IsOxmTlvValid(const COFP13OxmTlv *apOxmTlv)
{
    /* The value of oxm_hasmask must be zero and no oxm_mask is included. */
    CM_ASSERT_RETURN(!(apOxmTlv->GetHasMask()), false);

    /* Set-Field actions for OXM types OFPXMT_OFB_IN_PORT, OXM_OF_IN_PHY_PORT and OFPXMT_OFB_METADATA 
       are not supported, because those are not header fields. */
    ACE_CDR::Octet bOxmField = apOxmTlv->GetOxmField();
    CM_ASSERT_RETURN(((bOxmField != COFP13Match::OFPXMT_OFB_IN_PORT) 
                      && (bOxmField != COFP13Match::OFPXMT_OFB_IN_PHY_PORT)
                      && (bOxmField != COFP13Match::OFPXMT_OFB_METADATA)), false);

    return true;
}


CmResult COFP13ActSetField::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet    *pad    = NULL;
    std::allocator<ACE_CDR::Octet> alloc;

    CmResult result = COFP13Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    CmResult lRet = m_oxmTlv.StreamFrom(is);
    CM_ASSERT_RETURN((CM_OK == lRet), CM_ERROR_FAILURE);
    
    CM_ASSERT_RETURN((IsOxmTlvValid(&m_oxmTlv)), CM_ERROR_INVALID_ARG);
    
    ACE_UINT16 padLen = CalcTotalPadLen();
#if 0
    CM_ASSERTE_RETURN(padLen == (m_wLen - OFP13_ACTION_HEADER_LEN - OFP13_MATCH_OXMTLV_HEADLEN - m_oxmTlv.GetOxmLen()), 
                      CM_ERROR_INVALID_ARG);
 #endif   
    pad = alloc.allocate(padLen);
    CM_ASSERT_RETURN((pad != NULL), CM_ERROR_OUT_OF_MEMORY);

    bool bGood = is.read_octet_array(pad, padLen);
    if (!bGood)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13ActSetField::StreamFrom(): is.read_octet_array(pad, padLen) failed, "),
                   ACE_TEXT("pad=%p, padLen=%u\n"),
                   pad,
                   padLen));
        alloc.deallocate(pad, padLen);
        return CM_ERROR_FAILURE;
    }

    alloc.deallocate(pad, padLen);

    return CM_OK;
}


CmResult COFP13ActSetField::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet     *pad  = NULL;
    std::allocator<ACE_CDR::Octet> alloc;

    //m_wLen = GetStreamLen();
    
    CmResult result = COFP13Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    CM_ASSERT_RETURN((IsOxmTlvValid(&m_oxmTlv)), CM_ERROR_FAILURE);
    
    CmResult  lRet = m_oxmTlv.StreamTo(os);    
    CM_ASSERT_RETURN((CM_OK == lRet), CM_ERROR_FAILURE);
    
    ACE_UINT16    padLen = CalcTotalPadLen();
    pad = alloc.allocate(padLen);
    CM_ASSERT_RETURN((pad != NULL), CM_ERROR_OUT_OF_MEMORY);
    ::memset(pad, 0, padLen);
    bool bGood = os.write_octet_array(pad, padLen);
    if (!bGood)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13ActSetField::StreamTo(): os.write_octet_array(pad, padLen) failed, pad=%p, padLen=%u\n"),
                   pad,
                   padLen));
        alloc.deallocate(pad, padLen);
        return CM_ERROR_FAILURE;
    }

    alloc.deallocate(pad, padLen);

    return CM_OK;
}


void COFP13ActSetField::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13ActSetField::Dump():\n")));
    COFP13Action::Dump();
    CM_ASSERT_RETURN_VOID(IsOxmTlvValid(&m_oxmTlv));
    m_oxmTlv.Dump();
}



