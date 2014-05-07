/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPacketOutMsg.cpp
 * Description:      Source file for openflow packet-out message.
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

#include "COFPPacketOutMsg.h"
#include "ace/Log_Msg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP10PacketOutMsg::~COFP10PacketOutMsg()
{
    for (auto &item : m_action_list)
    {
        delete item;
    }
    
    if (m_PacketData)
    {
        m_PacketData->release();
        m_PacketData = NULL;
    }
}

CmResult COFP10PacketOutMsg::
AddActionList(std::list<COFP10Action *> &list)
{
    while (!list.empty())
    {
        COFP10Action *action = list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_action_list.push_back(action);
        m_tMember.actions_len += action->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP10PacketOutMsg::
AppendAction(COFP10Action *apAct)
{
    CM_ASSERT_RETURN(apAct != nullptr, CM_ERROR_NULL_POINTER);
    m_action_list.push_back(apAct);
    m_tMember.actions_len += apAct->GetStreamLen();
    return CM_OK;
}

CmResult COFP10PacketOutMsg::
GetActionList(std::list<COFP10Action *> &list)
{
    while (!m_action_list.empty())
    {
        COFP10Action *action = m_action_list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        m_action_list.pop_front();
        m_tMember.actions_len -= action->GetStreamLen();
        list.push_back(action);
    }

    return CM_OK;
}

const ACE_CDR::Octet * COFP10PacketOutMsg::GetEtherFrame() const
{
    const ACE_CDR::Octet *pData = NULL;
    
    if (m_PacketData)
    {
        std::string str = m_PacketData->flatten_chained();
        pData = (const ACE_CDR::Octet *)str.c_str();
    }

    return pData;
}

ACE_Message_Block *COFP10PacketOutMsg::GetPacketData() const
{
    return m_PacketData;
}

CmResult COFP10PacketOutMsg::AppendPacketData(ACE_Message_Block &aData)
{    
    if (m_PacketData != nullptr)
    {
        m_PacketData->append(aData.clone());
    }
    else
    {
        m_PacketData = aData.clone();
    }

    m_wDataLen += aData.total_length();

    return CM_OK;
}


CmResult COFP10PacketOutMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PacketOutMsg::StreamFrom, COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.buffer_id;
    is>>m_tMember.in_port;
    is>>m_tMember.actions_len;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    if (m_tMember.actions_len > 0)
    {
        ACE_INT16 swListLen = m_tMember.actions_len;
        ACE_UINT16 i = 0;
        while (swListLen > 0)
        {
            COFP10Action *action = nullptr;
            lRet = COFP10Action::DecodeAction(is, action);
            if (CM_SUCCEEDED(lRet))
            {
                m_action_list.push_back(action);
                ACE_DEBUG((LM_DEBUG, ACE_TEXT("action[%u], len=%u\n"), i++, action->GetActionLen()));
                swListLen -= action->GetActionLen();
            }
            else
            {
                ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PacketOutMsg::StreamFrom, COFP10Action::DecodeAction failed\n")));
                return CM_ERROR_FAILURE;
            }
        }
    }

    ACE_INT16 swDataLen = m_wLength - COFPMessage::GetStreamLen() - sizeof(m_tMember) - m_tMember.actions_len;
    if (swDataLen < 0)
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP10PacketOutMsg::StreamFrom, packet data len(%d) is invalid\n"), 
                   swDataLen));
        ACE_ASSERT(0);
        return CM_ERROR_FAILURE;
    }

    if (swDataLen == 0)
    {
        m_wDataLen = 0;
        return CM_OK;
    }

    ACE_Message_Block Data(swDataLen);

    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("COFP10PacketOutMsg::StreamFrom, wr_ptr = %p, space = %u\n"), 
               Data.wr_ptr(),
               Data.space()));

    bGood = is.read_char_array(Data.wr_ptr(), swDataLen);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    Data.wr_ptr((size_t)swDataLen);
    
    if (m_PacketData)
    {
        m_PacketData->append(Data.duplicate());
    }
    else
    {
        m_PacketData = Data.duplicate();
    }
    m_wDataLen += swDataLen;

    return CM_OK;
}

CmResult COFP10PacketOutMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PacketOutMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.buffer_id;
    os<<m_tMember.in_port;
    os<<m_tMember.actions_len;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    std::list<COFP10Action *>::const_iterator it = m_action_list.begin();
    while (it != m_action_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }
    
    if (m_PacketData)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP10PacketOutMsg::StreamTo, m_PacketData->total_length() = %u\n"),
                   m_PacketData->total_length()));

        std::string str = m_PacketData->flatten_chained();
        os.write_char_array(str.c_str(), str.length());
    }
    
    bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10PacketOutMsg::Dump() const
{
    COFPMessage::Dump();
}



COFP13PacketOutMsg::COFP13PacketOutMsg(
                    ACE_UINT32 abuffer_id,
                    ACE_UINT32 aInPort,
                    ACE_UINT32 aXid)
                    : COFPMessage(OFP13::OFPT_PACKET_OUT, OFP13_VERSION, aXid, 24)
                    , m_buffer_id(abuffer_id)
                    , m_in_port(aInPort)
                    , m_actions_len(0)
                    , m_PacketData(0)
{
    ::memset(m_pad, 0, 6);
}


COFP13PacketOutMsg::~COFP13PacketOutMsg()
{
    for (auto &item : m_action_list)
    {
        delete item;
    }
    
    if (m_PacketData)
    {
        m_PacketData->release();
        m_PacketData = NULL;
    }
}
    

CmResult COFP13PacketOutMsg::
AddActionList(std::list<COFP13Action *> &list)
{
    ACE_UINT32 actionLen = 0;
    while (!list.empty())
    {
        COFP13Action *action = list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_action_list.push_back(action);
        actionLen += action->GetStreamLen();
    }

    m_actions_len += actionLen;

    return CM_OK;
}


CmResult COFP13PacketOutMsg::
AppendAction(COFP13Action *action)
{
    CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
    m_action_list.push_back(action);
    m_actions_len += action->GetStreamLen();
    return CM_OK;
}


CmResult COFP13PacketOutMsg::
GetActionList(std::list<COFP13Action *> &list)
{
    ACE_UINT32 actionLen = 0;
    
    while (!m_action_list.empty())
    {
        COFP13Action *action = m_action_list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        actionLen += action->GetActionLen();
        m_action_list.pop_front();
        list.push_back(action);
    }

    m_actions_len -= actionLen;

    return CM_OK;
}


ACE_Message_Block *COFP13PacketOutMsg::GetPacketData() const
{
    return m_PacketData;
}


CmResult COFP13PacketOutMsg::AppendPacketData(ACE_Message_Block &data)
{
    if (m_PacketData)
    {
        m_PacketData->append(data.clone());
    }
    else
    {
        m_PacketData = data.clone();
    }

    return CM_OK;
}

CmResult COFP13PacketOutMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_INT32 remainActLen = 0;
    ACE_INT32 packet_length = 0;
    ACE_CDR::Octet pad[6];
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PacketOutMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13PacketOutMsg::StreamFrom(): \n")));
    
    is>>m_buffer_id;
    is>>m_in_port;
    is>>m_actions_len;
    is.read_octet_array(pad, sizeof(pad));
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_actions_len=%u\n"), m_actions_len));
    
    if (m_actions_len>0)
    {
        remainActLen = m_actions_len;
        ACE_UINT16 i = 0;
        while (remainActLen > 0)
        {
            COFP13Action *action = nullptr;
            CmResult result = COFP13Action::DecodeAction(is, action);
            if (CM_SUCCEEDED(result))
            {
                m_action_list.push_back(action);
                ACE_DEBUG((LM_DEBUG, ACE_TEXT("action[%u], len=%u\n"), i++, action->GetStreamLen()));
                remainActLen = remainActLen - (action->GetStreamLen());
            }
            else
            {
                ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13Action::DecodeAction failed.\n")));
                return CM_ERROR_FAILURE;
            }
        }
    }

    ACE_UINT32 length = sizeof(m_buffer_id)+sizeof(m_in_port)+sizeof(m_actions_len)+sizeof(m_pad);
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("length=%u\n"), length));

    packet_length = m_wLength-COFPMessage::GetStreamLen()-length-m_actions_len;
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("packet_length=%d, GetStreamLen=%u\n"), packet_length, GetStreamLen()));
    if (packet_length < 0)
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13PacketOutMsg::StreamFrom, error, length=%u, actionlength=%u\n"), 
                   length, 
                   m_actions_len));
        return CM_ERROR_FAILURE;
    }

    ACE_Message_Block data(packet_length);

    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("COFP13PacketOutMsg::StreamFrom, wr_ptr=%p, space=%u\n"),
               data.wr_ptr(),
               data.space()));
    
    bGood = is.read_char_array(data.wr_ptr(), packet_length);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    data.wr_ptr((size_t)packet_length);
    
    if (m_PacketData)
    {
        m_PacketData->append(data.duplicate());
    }
    else
    {
        m_PacketData = data.duplicate();
    }

    return CM_OK;
}


CmResult COFP13PacketOutMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_CDR::Octet pad[6];
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PacketOutMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13PacketOutMsg::StreamTo():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("msgLen=%u\n"), GetMessageLength()));
    
    os<<m_buffer_id;
    os<<m_in_port;
    os<<m_actions_len;
    os.write_octet_array(pad, sizeof(pad));
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_actions_len=%u\n"), m_actions_len));
    
#if 0
    while(!m_action_list.empty())
    {
        CCmComAutoPtr<COpenFlowProtAction> action;
        action = m_action_list.front();
        m_action_list.pop_front();
        action->EncodeAction(os);
    }
#endif

    ACE_UINT16 i = 0;
    std::list<COFP13Action *>::const_iterator it = m_action_list.begin();
    while(it != m_action_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("action[%u]'s length=%u\n"), i++, (*it)->GetActionLen()));
        bGood = os.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        it++;
    }

    // just for debugging
    ACE_UINT32 length = sizeof(m_buffer_id)+sizeof(m_in_port)+sizeof(m_actions_len)+sizeof(m_pad);
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("length=%u\n"), length));
    ACE_INT32 dataLen = GetStreamLen()-COFPMessage::GetStreamLen()-length-m_actions_len;
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("dataLen=%d\n"), dataLen));

    if (m_PacketData)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_PacketData->total_length() = %u\n"), m_PacketData->total_length()));

        std::string str = m_PacketData->flatten_chained();
        bGood = os.write_char_array(str.c_str(), str.length());
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;
}


ACE_UINT16 COFP13PacketOutMsg::GetStreamLen() const
{
    ACE_UINT32 headlength = COFPMessage::GetStreamLen();
    //OFP_ASSERT(sizeof(struct ofp_packet_out) == 24);
    ACE_UINT32 length = headlength+sizeof(m_buffer_id)+sizeof(m_in_port)+sizeof(m_actions_len)+sizeof(m_pad);
    
    //length += m_PacketData->GetChainedLength();
    std::list<COFP13Action *>::const_iterator it = m_action_list.begin();
    while(it != m_action_list.end())
    {
        length += (*it)->GetStreamLen();
        it++;
    }
    //CM_ASSERTE_RETURN(actionLen == m_actions_len, 0);

    if (m_PacketData)
    {
        length += m_PacketData->total_length();
    }

    return length;
}


void COFP13PacketOutMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13PacketOutMsg::dump()\n")));
    
    COFPMessage::Dump();
    
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("m_buffer_id=%u, m_in_port=%u, m_actions_len=%u\n"),
               m_buffer_id,
               m_in_port,
               m_actions_len));
    std::list<COFP13Action *>::const_iterator it = m_action_list.begin();

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("Action List:\n")));
    ACE_UINT16 i = 0;
    while(it != m_action_list.end())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("action[%u]:\n"), i++));
        (*it)->Dump();
        it++;
    }
    if (m_PacketData)
    {
        ACE_UINT32 nPktLen = m_PacketData->length();
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n")));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("pkt: length=%u\n"), nPktLen));
        if (nPktLen > 0)
        {
            ACE_DEBUG((LM_DEBUG, 
                       ACE_TEXT("pkt[0]=%u\n"), 
                       *(reinterpret_cast<const ACE_CDR::Octet *>(m_PacketData->rd_ptr()))));
            if (nPktLen > 1)
            {
                ACE_DEBUG((LM_DEBUG, 
                           ACE_TEXT("pkt[%u]=%u\n"), 
                           nPktLen-1,
                           *(reinterpret_cast<ACE_CDR::Octet *>(m_PacketData->wr_ptr()-1))));
            }
        }
    }
}


