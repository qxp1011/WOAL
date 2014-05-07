/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPTableFeatureProp.cpp
 * Description:      OpenFlow table feature properties source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-16
 *
 * History 1:
 *     Date:         2013-12-16
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPTableFeatureProp.h"
#include "CmError.h"

CmResult COFP13TableFeatureProp::DecodeMPTFP(ACE_InputCDR &is, COFP13TableFeatureProp * &prop)
{
    ACE_UINT16 type;
    bool bGood = is.pre_read(&type, sizeof(type));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    ACE_CDR::swap_2(reinterpret_cast<const char *>(&type), reinterpret_cast<char *>(&type));

    COFP13TableFeatureProp *propData = nullptr;
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableFeatureProp::DecodeMPTFP, type=%u\n"), type));

    switch (type)
    {
        case OFPTFPT_INSTRUCTIONS:
        case OFPTFPT_INSTRUCTIONS_MISS:
        {
            propData = new COFP13TableFeaturePropInstruct(type);
            break;
        }
        case OFPTFPT_EXPERIMENTER:
        case OFPTFPT_EXPERIMENTER_MISS:
        {
            propData = new COFP13TableFeaturePropExperimenter(type);
            break;
        }
        case OFPTFPT_NEXT_TABLES:
        case OFPTFPT_NEXT_TABLES_MISS:
        {
            propData = new COFP13TableFeaturePropNextTables(type);
        }
        case OFPTFPT_WRITE_ACTIONS:
        case OFPTFPT_WRITE_ACTIONS_MISS:
        case OFPTFPT_APPLY_ACTIONS:
        case OFPTFPT_APPLY_ACTIONS_MISS:
        {
            propData = new COFP13TableFeaturePropActions(type);
        }
        case OFPTFPT_MATCH:
        case OFPTFPT_WILDCARDS:
        case OFPTFPT_WRITE_SETFIELD:
        case OFPTFPT_WRITE_SETFIELD_MISS:
        case OFPTFPT_APPLY_SETFIELD:
        case OFPTFPT_APPLY_SETFIELD_MISS:
        {
            propData = new COFP13TableFeaturePropOXM(type);
        }
        default:
        {
            break;
        }
    }
    CM_ASSERT_RETURN(propData != nullptr, CM_ERROR_OUT_OF_MEMORY);

    CmResult rv = propData->StreamFrom(is);
    if (CM_FAILED(rv))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13TableFeatureProp::DecodeMPTFP error\n")));
        return CM_ERROR_FAILURE;
    }

    prop = propData;

    return CM_OK;
}

CmResult COFP13TableFeatureProp::StreamFrom(ACE_InputCDR &is)
{
    is>>m_header.type;
    is>>m_header.length;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);    
    return CM_OK;
}

CmResult COFP13TableFeatureProp::StreamTo(ACE_OutputCDR &os)
{
    m_header.length = GetStreamLen();
    
    os<<m_header.type;
    os<<m_header.length;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

ACE_UINT16 COFP13TableFeatureProp::GetStreamLen() const
{
    //WORD32 pad = (m_header.length + 7)/8*8-(m_header.length);

    return sizeof(m_header);
}

ACE_UINT16 COFP13TableFeatureProp::GetEncodeSpace() const
{
    //WORD32 pad = (m_header.length + 7)/8*8-(m_header.length);

    //return sizeof(m_header)+pad; // missing m_header.length-sizeof(m_header)
    return (m_header.length + 7)/8*8;
}

void COFP13TableFeatureProp::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableFeatureProp::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type=%u, len=%u\n"), m_header.type, m_header.length));
}


COFP13TableFeaturePropInstruct::~COFP13TableFeaturePropInstruct()
{
    for (auto &item : m_instruction_list)
    {
        delete item;
    }
}

void COFP13TableFeaturePropInstruct::AddInstruction(COFP13Instruction *inst)
{
    CM_ASSERT_RETURN_VOID(inst != nullptr);
    m_instruction_list.push_back(inst);
    m_header.length += inst->GetStreamLen();
}

void COFP13TableFeaturePropInstruct::GetInstructionList(std::list<COFP13Instruction *> &list)
{
    while (!m_instruction_list.empty())
    {
        COFP13Instruction *item = m_instruction_list.front();
        CM_ASSERT_RETURN_VOID(item != nullptr);
        m_instruction_list.pop_front();
        m_header.length -= item->GetStreamLen();
        list.push_back(item);
    }
}

void COFP13TableFeaturePropInstruct::SetInstructionList(std::list<COFP13Instruction *> &list)
{
    while (!list.empty())
    {
        COFP13Instruction *item = list.front();
        CM_ASSERT_RETURN_VOID(item != nullptr);
        list.pop_front();
        m_header.length += item->GetStreamLen();
        m_instruction_list.push_back(item);
    }
}

CmResult COFP13TableFeaturePropInstruct::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13TableFeatureProp::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    ACE_INT16   size    = m_header.length - sizeof(m_header);
    ACE_UINT16  padLen  = (m_header.length + 7)/8*8 - (m_header.length);
    size = size - padLen;
    
    CmResult result = CM_ERROR_FAILURE;
    while (size > 0)
    {
        COFP13Instruction *instruction = nullptr;
        result = COFP13Instruction::DecodeInstruction(is, instruction);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
        
        m_instruction_list.push_back(instruction);
        size = size - instruction->GetStreamLen();
    }

    CM_ASSERT_RETURN(0 == size, CM_ERROR_INVALID_ARG);

    // deal with padLen
    result = COFPMsgMemoryMgr::PadStreamFrom(is, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    return CM_OK;
}

CmResult COFP13TableFeaturePropInstruct::StreamTo(ACE_OutputCDR &os)
{
    //m_header.length = GetStreamLen();
    
    if (COFP13TableFeatureProp::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    CmResult result = CM_ERROR_FAILURE;
    for (auto &inst : m_instruction_list) 
    {
        result = inst->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    }

    ACE_UINT32  padLen  = (m_header.length + 7)/8*8 - (m_header.length);
    result = COFPMsgMemoryMgr::PadStreamTo(os, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    return CM_OK;
}

ACE_UINT16 COFP13TableFeaturePropInstruct::GetStreamLen() const
{
    ACE_UINT16 size = COFP13TableFeatureProp::GetStreamLen();
    for (auto& inst : m_instruction_list) 
    {
        size += inst->GetStreamLen();
    }

    return size;
}

ACE_UINT16 COFP13TableFeaturePropInstruct::GetEncodeSpace() const
{
    ACE_UINT16 size = GetStreamLen();
    
    ACE_UINT32 pad = (size + 7)/8*8-(size);

    return size+pad;
}

void COFP13TableFeaturePropInstruct::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableFeaturePropInstruct::Dump():\n")));
    COFP13TableFeatureProp::Dump();

    if (!m_instruction_list.empty())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("instruction list:\n")));
        ACE_UINT16 i = 0;
        for (auto &item : m_instruction_list)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("instruction[%u]:\n"), i++));
            item->Dump();
        }
    }
}

//COFP13TableFeaturePropNextTables
void COFP13TableFeaturePropNextTables::AddNextTableID(ACE_CDR::Octet id)
{
    m_next_table_id_list.push_back(id);
    m_header.length += sizeof(id);
}

void COFP13TableFeaturePropNextTables::GetNextTableIDList(std::list<ACE_CDR::Octet> &list)
{
    while (!m_next_table_id_list.empty())
    {
        ACE_CDR::Octet item = m_next_table_id_list.front();
        m_next_table_id_list.pop_front();
        m_header.length -= sizeof(item);
        list.push_back(item);
    }
}

CmResult COFP13TableFeaturePropNextTables::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13TableFeatureProp::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    ACE_INT16 size    = m_header.length - sizeof(m_header);
    ACE_UINT32  padLen  = (m_header.length + 7)/8*8 - (m_header.length);
    size = size - padLen;

    bool bGood = false;
    
    while (size > 0)
    {
        ACE_CDR::Octet id = 0;
        is>>id;
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        
        m_next_table_id_list.push_back(id);
        size -= 1;
    }

    CM_ASSERT_RETURN(0 == size, CM_ERROR_INVALID_ARG);

    CmResult result = COFPMsgMemoryMgr::PadStreamFrom(is, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    return CM_OK;
}

CmResult COFP13TableFeaturePropNextTables::StreamTo(ACE_OutputCDR &os)
{
    //m_header.length = GetStreamLen();
    
    if (COFP13TableFeatureProp::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    bool       bGood  = false;
    ACE_UINT16 padLen = (m_header.length + 7) / 8 * 8 - m_header.length;
    
    for (auto &id : m_next_table_id_list) 
    {
        os<<id;
        bGood = os.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    CmResult result = COFPMsgMemoryMgr::PadStreamTo(os, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    return CM_OK;
}

ACE_UINT16 COFP13TableFeaturePropNextTables::GetStreamLen() const
{
    ACE_UINT16 size = COFP13TableFeatureProp::GetStreamLen();
    for (auto& id : m_next_table_id_list) 
    {
        size += 1;
    }
    return size;
}

ACE_UINT16 COFP13TableFeaturePropNextTables::GetEncodeSpace() const
{
    ACE_UINT16 size = GetStreamLen();
    
    ACE_UINT32 pad = (size + 7)/8*8-(size);

    return size+pad;
}

void COFP13TableFeaturePropNextTables::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableFeaturePropNextTables::Dump():\n")));
    COFP13TableFeatureProp::Dump();
    if (!m_next_table_id_list.empty())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("next table id list:\n")));
        ACE_UINT16 i = 0;
        for (auto &item : m_next_table_id_list)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("next_table_id[%u]=%u\n"), i++, item));
        }
    }
}

//COFP13TableFeaturePropActions
COFP13TableFeaturePropActions::~COFP13TableFeaturePropActions()
{
    for (auto &item : m_action_list)
    {
        delete item;
    }
}

void COFP13TableFeaturePropActions::AddAction(COFP13Action *action)
{
    CM_ASSERT_RETURN_VOID(action != nullptr);
    m_action_list.push_back(action);
    m_header.length += action->GetStreamLen();
}

void COFP13TableFeaturePropActions::SetActionList(std::list<COFP13Action *> &list)
{
    while (!list.empty())
    {
        COFP13Action *act = list.front();
        CM_ASSERT_RETURN_VOID(act != nullptr);
        list.pop_front();
        m_action_list.push_back(act);
        m_header.length += act->GetStreamLen();
    }
}

void COFP13TableFeaturePropActions::GetActionList(std::list<COFP13Action *> &list)
{
    while (!m_action_list.empty())
    {
        COFP13Action *act = m_action_list.front();
        CM_ASSERT_RETURN_VOID(act != nullptr);
        m_action_list.pop_front();
        m_header.length -= act->GetStreamLen();
        list.push_back(act);
    }
}

CmResult COFP13TableFeaturePropActions::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13TableFeatureProp::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }
    
    ACE_INT16 size   = m_header.length - sizeof(m_header);
    ACE_UINT32  padLen = (m_header.length + 7)/8*8 - (m_header.length);
    size = size - padLen;

    CmResult result = CM_ERROR_FAILURE;
    while (size > 0)
    {
        COFP13Action *action = nullptr;
        result = COFP13Action::DecodeAction(is, action);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
        
        AddAction(action);
        size = size - action->GetStreamLen();
    }

    CM_ASSERT_RETURN(0 == size, CM_ERROR_INVALID_ARG);

    result = COFPMsgMemoryMgr::PadStreamFrom(is, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    return CM_OK;
}

CmResult COFP13TableFeaturePropActions::StreamTo(ACE_OutputCDR &os)
{
    //m_header.length = GetStreamLen();
    
    if (COFP13TableFeatureProp::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    CmResult result = CM_ERROR_FAILURE;
    for (auto &item : m_action_list) 
    {
        result = item->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    }

    ACE_UINT16 padLen = (m_header.length + 7) / 8 * 8 - m_header.length;
    result = COFPMsgMemoryMgr::PadStreamTo(os, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    return CM_OK;
}

ACE_UINT16 COFP13TableFeaturePropActions::GetStreamLen() const
{
    ACE_UINT16 size = COFP13TableFeatureProp::GetStreamLen();
    for (auto &action : m_action_list) 
    {
        size += action->GetStreamLen();
    }
    return size;
}

ACE_UINT16 COFP13TableFeaturePropActions::GetEncodeSpace() const
{
    ACE_UINT16 size = GetStreamLen();
    
    ACE_UINT32 pad = (size + 7)/8*8-(size);

    return size+pad;
}

void COFP13TableFeaturePropActions::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableFeaturePropActions::Dump()\n")));
    COFP13TableFeatureProp::Dump();
    if (!m_action_list.empty())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("action list:\n")));
        ACE_UINT16 i = 0;
        for (auto &action : m_action_list)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("action[%u]:\n"), i++));
            action->Dump();
        }
    }
}

//COFP13TableFeaturePropOXM
COFP13TableFeaturePropOXM::~COFP13TableFeaturePropOXM()
{
    for (auto &item : m_oxm_list)
    {
        delete item;
    }
}

void COFP13TableFeaturePropOXM::AddOXM(COFP13OxmTlv *oxm)
{
    CM_ASSERT_RETURN_VOID(oxm != nullptr);
    m_oxm_list.push_back(oxm);
    m_header.length += oxm->GetStreamLen();
}

void COFP13TableFeaturePropOXM::SetOXMList(std::list<COFP13OxmTlv *> &list)
{
    while (!list.empty())
    {
        COFP13OxmTlv *tlv = list.front();
        CM_ASSERT_RETURN_VOID(tlv != nullptr);
        list.pop_front();
        m_oxm_list.push_back(tlv);
        m_header.length += tlv->GetStreamLen();
    }
}

void COFP13TableFeaturePropOXM::GetOXMList(std::list<COFP13OxmTlv *> &list)
{
    while (!m_oxm_list.empty())
    {
        COFP13OxmTlv *tlv = m_oxm_list.front();
        CM_ASSERT_RETURN_VOID(tlv != nullptr);
        m_oxm_list.pop_front();
        m_header.length -= tlv->GetStreamLen();
        list.push_back(tlv);
    }
}

CmResult COFP13TableFeaturePropOXM::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13TableFeatureProp::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }
    
    ACE_INT16 size   = m_header.length - sizeof(m_header);
    ACE_UINT16  padLen = (m_header.length + 7)/8*8 - (m_header.length);
    size = size - padLen;

    CmResult result = CM_ERROR_FAILURE;
    while (size > 0)
    {
        COFP13OxmTlv *oxm = nullptr;
        result = COFP13OxmTlv::DecodeOxmTlv(is, oxm);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

        m_oxm_list.push_back(oxm);
        //AddOXM(oxm); // will increase m_header.length
        size = size - oxm->GetStreamLen();
    }

    CM_ASSERT_RETURN(0 == size, CM_ERROR_INVALID_ARG);

    result = COFPMsgMemoryMgr::PadStreamFrom(is, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    return CM_OK;
}

CmResult COFP13TableFeaturePropOXM::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13TableFeatureProp::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    CmResult result = CM_ERROR_FAILURE;
    for (auto &oxm : m_oxm_list) 
    {
        result = oxm->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    }

    ACE_UINT16 padLen = (m_header.length+7)/8*8 - m_header.length;
    result = COFPMsgMemoryMgr::PadStreamTo(os, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    return CM_OK;
}

ACE_UINT16 COFP13TableFeaturePropOXM::GetStreamLen() const
{
    ACE_UINT16 size = COFP13TableFeatureProp::GetStreamLen();
    for (auto& oxm : m_oxm_list) 
    {
        size += oxm->GetStreamLen();
    }

    return size;
}

ACE_UINT16 COFP13TableFeaturePropOXM::GetEncodeSpace() const
{
    ACE_UINT16 size = GetStreamLen();
    
    ACE_UINT32 pad = (size + 7)/8*8-(size);

    return size+pad;
}

void COFP13TableFeaturePropOXM::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableFeaturePropOXM::Dump():\n")));
    COFP13TableFeatureProp::Dump();
    if (!m_oxm_list.empty())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("oxm list:\n")));
        ACE_UINT16 i = 0;
        for (auto &item : m_oxm_list)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("oxm[%u]:\n"), i++));
            item->Dump();
        }
    }
}

// COFP13TableFeaturePropExperimenter
void COFP13TableFeaturePropExperimenter::AddExpData(ACE_UINT32 data)
{
    m_experimenter_data_list.push_back(data);
    m_header.length +=sizeof(data);
}

void COFP13TableFeaturePropExperimenter::GetExpDataList(std::list<ACE_UINT32> &list)
{
    for (auto& item : m_experimenter_data_list) 
    {
        list.push_back(item);
    }
}

CmResult COFP13TableFeaturePropExperimenter::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13TableFeatureProp::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_experimenter;
    is>>m_exp_type;

    ACE_INT16 size   = m_header.length - sizeof(m_header) - sizeof(m_experimenter) - sizeof(m_exp_type);
    ACE_UINT16  padLen = (m_header.length + 7)/8*8 - (m_header.length);
    size = size - padLen;

    while (size > 0)
    {
        ACE_UINT32 item;
        is>>item;
        m_experimenter_data_list.push_back(item);   // can't call AddExpData() to avoid increasing m_header.length
        size -= sizeof(item);
    }

    CM_ASSERT_RETURN(0 == size, CM_ERROR_INVALID_ARG);

    CmResult result = COFPMsgMemoryMgr::PadStreamFrom(is, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    return CM_OK;
}

CmResult COFP13TableFeaturePropExperimenter::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13TableFeatureProp::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_experimenter;
    os<<m_exp_type;
    
    for (auto &item : m_experimenter_data_list) 
    {
        os<<item;
    }

    ACE_UINT16 padLen = (m_header.length+7)/8*8 - m_header.length;
    CmResult   result = COFPMsgMemoryMgr::PadStreamTo(os, padLen);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    return CM_OK;
}

ACE_UINT16 COFP13TableFeaturePropExperimenter::GetStreamLen() const
{
    ACE_UINT16 size = COFP13TableFeatureProp::GetStreamLen();
    size += sizeof(m_experimenter)+sizeof(m_exp_type);
    size += m_experimenter_data_list.size() * sizeof(ACE_UINT32);
    return size;
}

ACE_UINT16 COFP13TableFeaturePropExperimenter::GetEncodeSpace() const
{
    ACE_UINT16 size = GetStreamLen();
    ACE_UINT32 pad = (size + 7)/8*8-(size);

    return size+pad;
}

void COFP13TableFeaturePropExperimenter::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableFeaturePropExperimenter::Dump()\n")));
    COFP13TableFeatureProp::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("experimenter=%u, exp_type=%u\n"), m_experimenter, m_exp_type));
    if (!m_experimenter_data_list.empty())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("data list:\n")));
        ACE_UINT16 i = 0;
        for (auto &item : m_experimenter_data_list)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("data[%u]=%u\n"), i++, item));
        }
    }
}


