/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPStatsMsgHeader.cpp
 * Description:      Source file for openflow stats message header.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-15
 *
 * History 1:
 *     Date:         2013-12-18
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/
#include "COFPStatsMsgHeader.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

CmResult COFP10StatsMsgHeader::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10StatsMsgHeader::StreamFrom, COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_wStatsType;
    is>>m_wFlag;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10StatsMsgHeader::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10StatsMsgHeader::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_wStatsType;
    os<<m_wFlag;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

void COFP10StatsMsgHeader::Dump() const
{
    COFPMessage::Dump();
}


COFP10FlowStatsReplyEle::
~COFP10FlowStatsReplyEle()
{
    for (auto &item : m_act_list)
    {
        delete item;
    }
}

CmResult COFP10FlowStatsReplyEle::
AddActionList(std::list<COFP10Action *> &list)
{
    while (!list.empty())
    {
        COFP10Action *action = list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_act_list.push_back(action);
        m_tMember.length += action->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP10FlowStatsReplyEle::
AppendAction(COFP10Action *pAct)
{
    CM_ASSERT_RETURN(pAct != nullptr, CM_ERROR_NULL_POINTER);
    m_act_list.push_back(pAct);
    m_tMember.length += pAct->GetStreamLen();
    return CM_OK;
}

CmResult COFP10FlowStatsReplyEle::
GetActionList(std::list<COFP10Action *> &list)
{
    while (!m_act_list.empty())
    {
        COFP10Action *action = m_act_list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        m_act_list.pop_front();
        m_tMember.length -= action->GetStreamLen();
        list.push_back(action);
    }

    return CM_OK;
}

CmResult COFP10FlowStatsReplyEle::StreamFrom(ACE_InputCDR &is)
{
    is>>m_tMember.length;
    is>>m_tMember.table_id;
    is>>m_tMember.pad;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    CmResult lRet = m_Match.StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowStatsReplyEle::StreamFrom, COFP10Match::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.duration_sec;
    is>>m_tMember.duration_nsec;
    is>>m_tMember.priority;
    is>>m_tMember.idle_timeout;
    is>>m_tMember.hard_timeout;
    is.read_octet_array(m_tMember.pad2, sizeof m_tMember.pad2);
    is>>m_tMember.cookie;
    is>>m_tMember.packet_count;
    is>>m_tMember.byte_count;
    
    bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_INT16 swListLen = m_tMember.length - sizeof m_tMember - m_Match.GetStreamLen();
    if (swListLen > 0)
    {
        ACE_UINT16 i = 0;
        while (swListLen > 0)
        {
            COFP10Action *action = nullptr;
            lRet = COFP10Action::DecodeAction(is, action);
            if (CM_SUCCEEDED(lRet))
            {
                m_act_list.push_back(action);
                ACE_DEBUG((LM_DEBUG, 
                           ACE_TEXT("COFP10FlowStatsReplyEle::StreamFrom, action[%u], len=%u\n"), 
                           i++, 
                           action->GetActionLen()));
                swListLen -= action->GetActionLen();
            }
            else
            {
                CM_ASSERT_RETURN(lRet, CM_ERROR_FAILURE);
            }
        }
    }

    return CM_OK;
}

CmResult COFP10FlowStatsReplyEle::StreamTo(ACE_OutputCDR &os)
{
    os<<m_tMember.length;
    os<<m_tMember.table_id;
    os<<m_tMember.pad;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    CmResult lRet = m_Match.StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowStatsReplyEle::StreamTo, COFP10Match::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.duration_sec;
    os<<m_tMember.duration_nsec;
    os<<m_tMember.priority;
    os<<m_tMember.idle_timeout;
    os<<m_tMember.hard_timeout;
    os.write_octet_array(m_tMember.pad2, sizeof m_tMember.pad2);
    os<<m_tMember.cookie;
    os<<m_tMember.packet_count;
    os<<m_tMember.byte_count;
    
    bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFP10Action *>::const_iterator it = m_act_list.begin();
    while (it != m_act_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

void COFP10FlowStatsReplyEle::Dump() const
{
}

