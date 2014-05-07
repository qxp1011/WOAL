/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPFlowModMsg.cpp
 * Description:      Source file of openflow flow modification message
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

#include "COFPFlowModMsg.h"
#include "ace/Log_Msg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP10FlowModMsg::~COFP10FlowModMsg()
{
    for (auto &item : m_action_list)
    {
        delete item;
    }
}

CmResult COFP10FlowModMsg::
AddActionList(std::list<COFP10Action *> &list)
{
    while (!list.empty())
    {
        COFP10Action *action = list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_wActLen += action->GetStreamLen();
        m_action_list.push_back(action);
    }

    return CM_OK;
}


CmResult COFP10FlowModMsg::
AppendAction(COFP10Action *apAct)
{
    CM_ASSERT_RETURN(apAct != nullptr, CM_ERROR_NULL_POINTER);
    m_action_list.push_back(apAct);
    m_wActLen += apAct->GetStreamLen();
    return CM_OK;
}


CmResult COFP10FlowModMsg::
GetActionList(std::list<COFP10Action *> &list)
{
    while (!m_action_list.empty())
    {
        COFP10Action *action = m_action_list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        m_action_list.pop_front();  // Let the caller to manage the action memory.
        m_wActLen -= action->GetStreamLen();
        list.push_back(action);
    }

    return CM_OK;
}

CmResult COFP10FlowModMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowModMsg::StreamFrom(), COFPMessage::StreamFrom fail.\n")));
        return lRet;
    }

    lRet = m_Match.StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10Match::StreamFrom fail.\n")));
        return lRet;
    }

    is>>m_tMember.lCookie;
    is>>m_tMember.wCommand;
    is>>m_tMember.wIdleTimeout;
    is>>m_tMember.wHardTimeout;
    is>>m_tMember.wPriority;
    is>>m_tMember.dwBufferId;
    is>>m_tMember.wOutPort;
    is>>m_tMember.wFlags;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_INT16 swListLen = 0; 
    ACE_UINT16 wActLen = 0;
    swListLen = m_wLength - COFPMessage::GetStreamLen() - m_Match.GetStreamLen() - sizeof(m_tMember);
    while (swListLen > 0)
    {
        COFP10Action *action = nullptr;
        lRet = COFP10Action::DecodeAction(is, action);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(action, CM_ERROR_FAILURE);
        wActLen = action->GetActionLen();
        if (swListLen < wActLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_action_list.push_back(action);
        m_wActLen += wActLen;
        swListLen -= wActLen;
    }

    return CM_OK;
}

CmResult COFP10FlowModMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowModMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    lRet = m_Match.StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowModMsg::StreamTo, COFP10Match::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_tMember.lCookie;
    os<<m_tMember.wCommand;
    os<<m_tMember.wIdleTimeout;
    os<<m_tMember.wHardTimeout;
    os<<m_tMember.wPriority;
    os<<m_tMember.dwBufferId;
    os<<m_tMember.wOutPort;
    os<<m_tMember.wFlags;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    std::list<COFP10Action *>::const_iterator it = m_action_list.begin();
    while (it != m_action_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

void COFP10FlowModMsg::Dump() const
{
    COFPMessage::Dump();
    m_Match.Dump();
}



COFP13FlowModMsg::~COFP13FlowModMsg()
{
    for (auto &item : m_instruction_list)
    {
        delete item;
    }
}

CmResult COFP13FlowModMsg::
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

CmResult COFP13FlowModMsg::
AppendInstruction(COFP13Instruction *aInstruction)
{
    CM_ASSERT_RETURN(aInstruction != nullptr, CM_ERROR_NULL_POINTER);
    m_instruction_list.push_back(aInstruction);
    m_wInstructionLen += aInstruction->GetStreamLen();
    
    return CM_OK;
}

CmResult COFP13FlowModMsg::
GetInstructionList(std::list<COFP13Instruction *> &list)
{
    while (!m_instruction_list.empty())
    {
        COFP13Instruction *Instr = m_instruction_list.front();
        CM_ASSERT_RETURN(Instr != nullptr, CM_ERROR_NULL_POINTER);
        m_wInstructionLen -= Instr->GetInstructLen();
        m_instruction_list.pop_front();
        list.push_back(Instr);
    }

    return CM_OK;
}

CmResult COFP13FlowModMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16 pad = 0;
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13FlowModMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_nCookie;
    is>>m_nCookieMask;
    is>>m_bTableId;
    is>>m_bCommand;
    is>>m_wIdleTimeout;
    is>>m_wHardTimeout;
    is>>m_wPriority;
    is>>m_nBufferId;
    is>>m_nOutPort;
    is>>m_nOutGroup;
    is>>m_wFlags;
    is>>pad;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    lRet = m_tMatch.StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("match StreamFrom fail\n")));
        return lRet;
    }

    ACE_UINT16 wInstruLen = 0;
    ACE_INT16 swListLen = m_wLength - COFPMessage::GetStreamLen() - m_tMatch.GetEncodeSpace() - 40;
    if (swListLen > 0)
    {
        COFP13Instruction *Instru = nullptr;
        lRet = COFP13Instruction::DecodeInstruction(is, Instru);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Instru != nullptr, CM_ERROR_FAILURE);
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


CmResult COFP13FlowModMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT16 pad  = 0;
    CmResult   lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13FlowModMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_nCookie;
    os<<m_nCookieMask;
    os<<m_bTableId;
    os<<m_bCommand;
    os<<m_wIdleTimeout;
    os<<m_wHardTimeout;
    os<<m_wPriority;
    os<<m_nBufferId;
    os<<m_nOutPort;
    os<<m_nOutGroup;
    os<<m_wFlags;
    os<<pad;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    lRet = m_tMatch.StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13FlowModMsg::StreamTo, match StreamTo fail\n")));
        return lRet;
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


ACE_UINT16 COFP13FlowModMsg::GetStreamLen() const
{
    return (40 + COFPMessage::GetStreamLen() + m_tMatch.GetEncodeSpace() + m_wInstructionLen);
}


void COFP13FlowModMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowFlowModMsg::Dump():\n")));
    COFPMessage::Dump();
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("cookie=%u, cookieMask=%u, tableId=%u, command=%u, idleTimeout=%u, hardTimeout=%u, \n"),
               ACE_TEXT("priority=%u, bufferId=%u, outport=%u, outGroup=%u, flags=%u\n"),
               m_nCookie,
               m_nCookieMask,
               m_bTableId,
               m_bCommand,
               m_wIdleTimeout,
               m_wHardTimeout,
               m_wPriority,
               m_nBufferId,
               m_nOutPort,
               m_nOutGroup,
               m_wFlags));
    m_tMatch.Dump();
    //instruction
    for (auto &a : m_instruction_list)
    {
        a->Dump();
    }
}



