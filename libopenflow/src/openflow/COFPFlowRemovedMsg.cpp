/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPFlowRemovedMsg.cpp
 * Description:      Source file for openflow flow removed message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-15
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPFlowRemovedMsg.h"
#include "ace/Log_Msg.h"
#include "CmError.h"

CmResult COFP10FlowRemovedMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowRemovedMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    lRet = m_Match.StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10Match::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.cookie;
    is>>m_tMember.priority;
    is>>m_tMember.reason;    
    is>>m_tMember.pad[0];
    is>>m_tMember.duration_sec;
    is>>m_tMember.duration_nsec;
    is>>m_tMember.idle_timeout;
    is>>m_tMember.pad2[0];
    is>>m_tMember.pad2[1];
    is>>m_tMember.packet_count;
    is>>m_tMember.byte_count;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}


CmResult COFP10FlowRemovedMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowRemovedMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    lRet = m_Match.StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowRemovedMsg::StreamTo, COFP10Match::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.cookie;
    os<<m_tMember.priority;
    os<<m_tMember.reason;    
    os<<m_tMember.pad[0];
    os<<m_tMember.duration_sec;
    os<<m_tMember.duration_nsec;
    os<<m_tMember.idle_timeout;
    os<<m_tMember.pad2[0];
    os<<m_tMember.pad2[1];
    os<<m_tMember.packet_count;
    os<<m_tMember.byte_count;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10FlowRemovedMsg::Dump() const
{
    COFPMessage::Dump();
    m_Match.Dump();
}



COFP13FlowRemovedMsg::COFP13FlowRemovedMsg()
    : COFPMessage(OFP13::OFPT_FLOW_REMOVED)
    , m_lCookie(0)
    , m_wPriority(0)
    , m_bReason(0)
    , m_bTableId(0)
    , m_dwDurationSec(0)
    , m_dwDurationNsec(0)
    , m_wIdleTimeout(0)
    , m_wHardTimeout(0)
    , m_lPacketCount(0)
    , m_lByteCount(0)
{
}

CmResult COFP13FlowRemovedMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13FlowRemovedMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_lCookie;
    is>>m_wPriority;
    is>>m_bReason;
    is>>m_bTableId;
    is>>m_dwDurationSec;
    is>>m_dwDurationNsec;
    is>>m_wIdleTimeout;
    is>>m_wHardTimeout;
    is>>m_lPacketCount;
    is>>m_lByteCount;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return m_Match.StreamFrom(is);
}

CmResult COFP13FlowRemovedMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13FlowRemovedMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_lCookie;
    os<<m_wPriority;
    os<<m_bReason;
    os<<m_bTableId;
    os<<m_dwDurationSec;
    os<<m_dwDurationNsec;
    os<<m_wIdleTimeout;
    os<<m_wHardTimeout;
    os<<m_lPacketCount;
    os<<m_lByteCount;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return m_Match.StreamTo(os);
}

void COFP13FlowRemovedMsg::Dump() const
{
    COFPMessage::Dump();
}

