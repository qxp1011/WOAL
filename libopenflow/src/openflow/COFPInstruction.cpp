/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPInstruction.cpp
 * Description:      OpenFlow instruction source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-12
 *
 * History 1:
 *     Date:         2013-12-17
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "ByteOrder.h"
#include "COFPInstruction.h"
#include "ace/CDR_Base.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

using namespace openflow;

COFP13Instruction::COFP13Instruction(ACE_UINT16 awType, ACE_UINT16 awLen)
    : m_wType(awType)
    , m_wLen(awLen)
{
}

CmResult COFP13Instruction::
DecodeInstruction(ACE_InputCDR &is, COFP13Instruction * &aInstru)
{
    CmResult           lRet        = CM_ERROR_FAILURE;
    ACE_UINT16         wType       = 0;
    COFP13Instruction *instruction = nullptr;
    
    bool bGood = is.pre_read(&wType, sizeof(wType));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    ACE_CDR::swap_2(reinterpret_cast<const char *>(&wType), reinterpret_cast<char *>(&wType));

    switch (wType)
    {
        case OFPIT_GOTO_TABLE:
        {
            instruction = new COFP13InstructGotoTable();
            break;
        }
        case OFPIT_WRITE_METADATA:
        {
            instruction = new COFP13InstructWriteMetadata();
            break;
        }
        case OFPIT_WRITE_ACTIONS:
        case OFPIT_APPLY_ACTIONS:
        case OFPIT_CLEAR_ACTIONS:
        {
            instruction = new COFP13InstructAction(wType);
            break;
        }
        case OFPIT_METER:
        {
            instruction = new COFP13InstructionMeter();
            break;
        }
        default:
        {
            break;
        }
    }
    
    CM_ASSERT_RETURN(instruction != nullptr, CM_ERROR_OUT_OF_MEMORY);

    lRet = instruction->StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13Action::DecodeAction(): fail in StreamFrom(), lRet=%l\n"), 
                   lRet));
        return lRet;
    }

    aInstru = instruction;
    
    return CM_OK;
}

CmResult COFP13Instruction::StreamFrom(ACE_InputCDR &is)
{
    is>>m_wType;
    is>>m_wLen;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13Instruction::StreamTo(ACE_OutputCDR &os)
{
    m_wLen = GetStreamLen();

    CM_ASSERT_RETURN((m_wLen % 8) == 0, CM_ERROR_FAILURE);
    
    os<<m_wType;
    os<<m_wLen;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13Instruction::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Instruction::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type=%u, len=%u\n"), m_wType, m_wLen));
}

COFP13InstructGotoTable::COFP13InstructGotoTable(ACE_CDR::Octet abTableId)
    : COFP13Instruction(OFPIT_GOTO_TABLE, 8)
    , m_bTableId(abTableId)
{
}

CmResult COFP13InstructGotoTable::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad = 0;

    CmResult result = COFP13Instruction::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_bTableId;
    
    for (ACE_UINT32 i = 0; i < 3; i++)
    {
        is>>pad;
    }

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13InstructGotoTable::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad = 0;

    CmResult result = COFP13Instruction::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_bTableId;

    for (ACE_UINT32 i = 0; i < 3; i++)
    {
        os<<pad;
    }

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13InstructGotoTable::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13InstructGotoTable::Dump():\n")));
    COFP13Instruction::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("table_id=%u\n"), m_bTableId));
}

COFP13InstructWriteMetadata::COFP13InstructWriteMetadata(ACE_UINT64 alMetadata, ACE_UINT64 alMetaDataMask)
    : COFP13Instruction(OFPIT_WRITE_METADATA, 24)
    , m_lMetadata(alMetadata)
    , m_lMetaDataMask(alMetaDataMask)
{
}

CmResult COFP13InstructWriteMetadata::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 pad = 0;

    CmResult result = COFP13Instruction::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    is>>pad;
    is>>m_lMetadata;
    is>>m_lMetaDataMask;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13InstructWriteMetadata::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 pad = 0;

    CmResult result = COFP13Instruction::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<pad;
    os<<m_lMetadata;
    os<<m_lMetaDataMask;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13InstructWriteMetadata::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13InstructWriteMetadata::Dump():\n")));
    COFP13Instruction::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("metadata=%ull, metadata_mask=%ull\n"), m_lMetadata, m_lMetaDataMask));
}

COFP13InstructAction::COFP13InstructAction(ACE_UINT16 awType)
    : COFP13Instruction(0, 8)
{
    CM_ASSERT_RETURN_VOID((OFPIT_WRITE_ACTIONS == awType)
                           || (OFPIT_APPLY_ACTIONS == awType)
                           || (OFPIT_CLEAR_ACTIONS == awType));
    m_wType = awType;
}

COFP13InstructAction::~COFP13InstructAction()
{
    for (auto &item : m_action_list)
    {
        delete item;
    }
}

CmResult COFP13InstructAction::
AddActionList(std::list<COFP13Action *> &list)
{
    /* For the Clear-Actions instruction, the structure does not contain any actions. */
    CM_ASSERT_RETURN((m_wType != OFPIT_CLEAR_ACTIONS), CM_ERROR_INVALID_ARG);
    
    while (!list.empty())
    {
        COFP13Action *action = list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        if ((action->GetStreamLen()% 8) !=0)
        {
            ACE_ASSERT(0);
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13InstructWriteMetadata::AddActionList(), action must 64bit aligen\n")));

            return CM_ERROR_FAILURE;
        }
        m_action_list.push_back(action);
        m_wLen += action->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP13InstructAction::
AppendAction(COFP13Action *action)
{
    CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);

    /* For the Clear-Actions instruction, the structure does not contain any actions. */
    CM_ASSERT_RETURN((m_wType != OFPIT_CLEAR_ACTIONS), CM_ERROR_INVALID_ARG);

    if ((action->GetStreamLen()% 8) !=0)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13InstructWriteMetadata::AddActionList(), action must 64bit aligned.\n")));
    
        return CM_ERROR_FAILURE;
    }

    m_action_list.push_back(action);
    m_wLen += action->GetStreamLen();
    
    return CM_OK;
}

CmResult COFP13InstructAction::
GetActionList(std::list<COFP13Action *> &list)
{
    while (!m_action_list.empty())
    {
        COFP13Action *act = m_action_list.front();
        CM_ASSERT_RETURN(act != nullptr, CM_ERROR_NULL_POINTER);
        m_action_list.pop_front();
        m_wLen -= act->GetStreamLen();
        list.push_back(act);
    }

    return CM_OK;
}

CmResult COFP13InstructAction::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet    = CM_ERROR_FAILURE;
    ACE_UINT32   pad     = 0;
    ACE_UINT16   wLen    = 0;
    ACE_UINT16   wActLen = 0;

    lRet = COFP13Instruction::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);
    CM_ASSERT_RETURN(GetInstructLen() >= (OFP13_INSTRUCTION_HEADLEN + 4 * sizeof(ACE_CDR::Octet)), CM_ERROR_INVALID_ARG);
    
    is>>pad;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    wLen = GetInstructLen() - OFP13_INSTRUCTION_HEADLEN - 4 * sizeof(ACE_CDR::Octet);
    if (wLen == 0)
    {
        return CM_OK;
    }

    /* For the Clear-Actions instruction, the structure does not contain any actions. */
    CM_ASSERT_RETURN(m_wType != OFPIT_CLEAR_ACTIONS, CM_ERROR_INVALID_ARG);
    
    while (wLen > 0)
    {
        COFP13Action *Act = nullptr;
        lRet = COFP13Action::DecodeAction(is, Act);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Act != nullptr, CM_ERROR_FAILURE);
        wActLen = Act->GetActionLen();
        if (wLen < wActLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_action_list.push_back(Act);
        wLen -= wActLen;
    }

    return CM_OK;
}

CmResult COFP13InstructAction::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32   pad = 0;

    //m_wLen = GetStreamLen();
    
    lRet = COFP13Instruction::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<pad;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFP13Action *>::const_iterator it = m_action_list.begin();
    while (it != m_action_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

ACE_UINT16 COFP13InstructAction::GetStreamLen() const
{
    ACE_UINT16 wLen = COFP13Instruction::GetStreamLen() + 4;
    
    std::list<COFP13Action *>::const_iterator it = m_action_list.begin();
    while(it != m_action_list.end())
    {
        wLen += (*it)->GetStreamLen();
        it++;
    }

    return wLen;
}

ACE_UINT16 COFP13InstructAction::GetEncodeSpace() const
{
    // The body of the instruction are all 8-bytes aligned.
    return GetStreamLen();
}

void COFP13InstructAction::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13InstructAction::Dump():\n")));
    COFP13Instruction::Dump();

    if (!m_action_list.empty())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("action list:\n")));
    }
    ACE_UINT16 i = 0;
    for (auto &item : m_action_list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("action[%u]:\n"), i++));
        item->Dump();
    }
}

COFP13InstructionMeter::COFP13InstructionMeter(ACE_UINT32 anMeterId)
    : COFP13Instruction(OFPIT_METER, OFP13_INSTRUCTION_HEADLEN + sizeof(ACE_UINT32))
    , m_dwMeterId(anMeterId)
{
}

CmResult COFP13InstructionMeter::StreamFrom(ACE_InputCDR &is)
{
    CmResult result = COFP13Instruction::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_dwMeterId;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;    
}

CmResult COFP13InstructionMeter::StreamTo(ACE_OutputCDR &os)
{
    CmResult result = COFP13Instruction::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_dwMeterId;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13InstructionMeter::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13InstructionMeter::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("meterId=%u\n"), m_dwMeterId));
}


