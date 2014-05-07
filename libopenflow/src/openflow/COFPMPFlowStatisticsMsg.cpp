/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPFlowStatisticsMsg.cpp
 * Description:      Source file for openflow multipart flow statistics msg.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-18
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPMPFlowStatisticsMsg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

CmResult COFP13MPFlowStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_stats_request.table_id;
    is.read_octet_array(m_stats_request.pad, sizeof(m_stats_request.pad));

    is>>m_stats_request.out_port;
    is>>m_stats_request.out_group;
    is.read_octet_array(m_stats_request.pad2, sizeof(m_stats_request.pad2));

    is>>m_stats_request.cookie;
    is>>m_stats_request.cookie_mask;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return m_match.StreamFrom(is);
}

CmResult COFP13MPFlowStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }
    
    os<<m_stats_request.table_id;
    os.write_octet_array(m_stats_request.pad, sizeof(m_stats_request.pad));

    os<<m_stats_request.out_port;
    os<<m_stats_request.out_group;
    os.write_octet_array(m_stats_request.pad2, sizeof(m_stats_request.pad2));

    os<<m_stats_request.cookie;
    os<<m_stats_request.cookie_mask;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return m_match.StreamTo(os);
}

ACE_UINT16 COFP13MPFlowStatsRequestMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_stats_request)+m_match.GetEncodeSpace();
}

void COFP13MPFlowStatsRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPFlowStatsRequestMsg::ofp_flow_stats_request &
COFP13MPFlowStatsRequestMsg::GetStatsRequestInfo()
{
    return m_stats_request;
}

void COFP13MPFlowStatsRequestMsg::
SetStatsRequestInfo(ofp_flow_stats_request &requestInfo)
{
    ::memcpy(&m_stats_request, &requestInfo, sizeof(requestInfo));
}


COFP13Match &COFP13MPFlowStatsRequestMsg::GetProtocolMatch()
{
    return m_match;
}

void COFP13MPFlowStatsRequestMsg::
SetProtocolMatch(COFP13Match &match)
{
    m_match = match;
}


COFP13MPFlowStatsReplyMsg::
~COFP13MPFlowStatsReplyMsg()
{
    for (auto &item : m_instruction_list)
    {
        delete item;
    }
}

CmResult COFP13MPFlowStatsReplyMsg::
AddInstructionList(std::list<COFP13Instruction *> &list)
{
    while (!list.empty())
    {
        COFP13Instruction *Instr = list.front();
        CM_ASSERT_RETURN(Instr != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_wInstructionLen += Instr->GetStreamLen();
        m_instruction_list.push_back(Instr);
    }

    return CM_OK;
}

CmResult COFP13MPFlowStatsReplyMsg::
AppendInstruction(COFP13Instruction *apInstruction)
{
    CM_ASSERT_RETURN(apInstruction != nullptr, CM_ERROR_NULL_POINTER);

    m_instruction_list.push_back(apInstruction);
    m_wInstructionLen += apInstruction->GetStreamLen();
    
    return CM_OK;
}

CmResult COFP13MPFlowStatsReplyMsg::
GetInstructionList(std::list<COFP13Instruction *> &list)
{
    while (!m_instruction_list.empty())
    {
        COFP13Instruction *Instr = m_instruction_list.front();
        CM_ASSERT_RETURN(Instr != nullptr, CM_ERROR_NULL_POINTER);
        m_instruction_list.pop_front();
        m_wInstructionLen -= Instr->GetInstructLen();
        list.push_back(Instr);
    }

    return CM_OK;
}

CmResult COFP13MPFlowStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }
    //is.Read(&m_flowstats, sizeof(m_flowstats));
    is>>m_flowstats.length;
    is>>m_flowstats.table_id;
    is>>m_flowstats.pad;
    is>>m_flowstats.duration_sec;
    is>>m_flowstats.duration_nsec;
    is>>m_flowstats.priority;
    is>>m_flowstats.idle_timeout;
    is>>m_flowstats.hard_timeout;
    is>>m_flowstats.flags;
    is.read_octet_array(m_flowstats.pad2, sizeof(m_flowstats.pad2));
    is>>m_flowstats.cookie;
    is>>m_flowstats.packet_count;
    is>>m_flowstats.byte_count;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    lRet = m_match.StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
    
    ACE_UINT16 wInstruLen = 0;
    ACE_INT16  swListLen = m_wLength - COFP13MultipartMsg::GetStreamLen() - m_match.GetEncodeSpace() - sizeof(m_flowstats);
    while (swListLen > 0)
    {
        COFP13Instruction *Instru = nullptr;
        lRet = COFP13Instruction::DecodeInstruction(is, Instru);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Instru, CM_ERROR_FAILURE);
        wInstruLen = Instru->GetInstructLen();
        if (swListLen < wInstruLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_instruction_list.push_back(Instru);
        m_wInstructionLen += wInstruLen;
        swListLen -= wInstruLen;
    }

    return CM_OK;
}

CmResult COFP13MPFlowStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }
    
    //os.Write(&m_flowstats, sizeof(m_flowstats));
    os<<m_flowstats.length;
    os<<m_flowstats.table_id;
    os<<m_flowstats.pad;
    os<<m_flowstats.duration_sec;
    os<<m_flowstats.duration_nsec;
    os<<m_flowstats.priority;
    os<<m_flowstats.idle_timeout;
    os<<m_flowstats.hard_timeout;
    os<<m_flowstats.flags;
    os.write_octet_array(m_flowstats.pad2, sizeof(m_flowstats.pad2));
    os<<m_flowstats.cookie;
    os<<m_flowstats.packet_count;
    os<<m_flowstats.byte_count;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    if (m_match.StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }
    
    std::list<COFP13Instruction *>::const_iterator it = m_instruction_list.begin();
    while(it != m_instruction_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

ACE_UINT16 COFP13MPFlowStatsReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_flowstats)+m_match.GetEncodeSpace()+m_wInstructionLen;
}

void COFP13MPFlowStatsReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPFlowStatsReplyMsg::ofp_flow_stats &COFP13MPFlowStatsReplyMsg::
GetFlowStatsInfo()
{
    return m_flowstats;
}

void COFP13MPFlowStatsReplyMsg::SetFlowStatsInfo(ofp_flow_stats &flowstats)
{
    ::memcpy(&m_flowstats, &flowstats, sizeof(flowstats));
}


COFP13Match &COFP13MPFlowStatsReplyMsg::GetProtocolMatch()
{
    return m_match;
}

void COFP13MPFlowStatsReplyMsg::SetProtocolMatch(COFP13Match &match)
{
    m_match = match;
}

