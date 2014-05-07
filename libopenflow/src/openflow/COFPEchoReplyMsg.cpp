/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPEchoReplyMsg.cpp
 * Description:      Source file for openflow echo reply message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-15
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: adapt to ACE
**********************************************************************/

#include "COFPEchoReplyMsg.h"
#include "ace/Log_Msg.h"
#include "CmError.h"

std::allocator<ACE_CDR::Octet> COFP10EchoReplyMsg::m_alloc;  // Handler for allocating/deallocating memory

COFP10EchoReplyMsg::COFP10EchoReplyMsg(ACE_CDR::Octet abVersion)
    : COFPMessage(OFP10::OFPT_ECHO_REPLY, abVersion)
    , m_payload(NULL)
    , m_used_payload_size(0)
    , m_total_payload_size(0)
{
}

COFP10EchoReplyMsg::COFP10EchoReplyMsg(COFP10EchoRequestMsg &request)
    : COFPMessage(OFP10::OFPT_ECHO_REPLY,
                  request.GetMessageVersion(),
                  request.GetMessageXID(),
                  request.GetMessageLength())
    , m_payload(NULL)
    , m_used_payload_size(0)
    , m_total_payload_size(0)
{
    ACE_UINT16 size = 0;
    const ACE_CDR::Octet  *payload = NULL;
    
    // unnecessary to check the return value
    request.GetPayload(payload,size);

    if ((payload)&&(size!=0))
    {
        CmResult result = COFPMsgMemoryMgr::Copy(m_alloc, m_payload, m_total_payload_size, 
                                                      m_used_payload_size, payload, size);
        CM_ASSERT_RETURN_VOID((CM_OK == result));
    }
    
    return;
}

COFP10EchoReplyMsg::~COFP10EchoReplyMsg()
{
    if (m_payload)
    {
        m_alloc.deallocate(m_payload, m_total_payload_size);
        m_payload = NULL;
    }
}

CmResult COFP10EchoReplyMsg::
GetPayload(const ACE_CDR::Octet *&payload, ACE_UINT16 &payload_size) const
{
    payload      = m_payload;
    payload_size = m_used_payload_size;

    return CM_OK;
}


CmResult COFP10EchoReplyMsg::
SetPayload(const ACE_CDR::Octet *payload, ACE_UINT16 payload_size)
{
    CmResult result = CM_ERROR_FAILURE;

    result = COFPMsgMemoryMgr::Copy(m_alloc, m_payload, m_total_payload_size, 
                                         m_used_payload_size, payload, payload_size);

    return result;
}

CmResult COFP10EchoReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10EchoReplyMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    if (m_wLength < OFP_MESSAGE_HDR_SIZE)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COpenFlowMessageEchoReply::StreamFrom(): m_wLength(%u) < %u\n"),
                   m_wLength,
                   OFP_MESSAGE_HDR_SIZE));
        return CM_ERROR_FAILURE;
    }

    ACE_UINT16 payload_size = m_wLength-OFP_MESSAGE_HDR_SIZE;

    if (m_total_payload_size < payload_size)
    {
        if (m_payload != NULL)
        {
            m_alloc.deallocate(m_payload, m_total_payload_size);
            m_payload            = NULL;
            m_total_payload_size = 0;
            m_used_payload_size  = 0;
        }
        
        m_payload = m_alloc.allocate(payload_size);
        if (NULL == m_payload)
        {
            ACE_ASSERT(0);
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COpenFlowMessageEchoReply::StreamFrom(): m_alloc.allocate(%u) failed.\n"),
                       payload_size));
            return CM_ERROR_OUT_OF_MEMORY;
        }
        m_total_payload_size = payload_size;
    }
    
    if (payload_size != 0)
    {
        bool bGood = is.read_octet_array(m_payload, payload_size);
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        m_used_payload_size = payload_size;
    }

    return CM_OK;  
}

CmResult COFP10EchoReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10EchoReplyMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    if ((m_payload != NULL) && (m_used_payload_size != 0))
    {
        bool bGood = os.write_octet_array(m_payload, m_used_payload_size);
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }
   
    return CM_OK;  
}

void COFP10EchoReplyMsg::Dump() const
{
    COFPMessage::Dump();

    if (NULL == m_payload)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COpenFlowMessageEchoReply::Dump(): m_payload is NULL.\n")));
        return;
    }
    
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowMessageEchoReply::Dump()\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("payload=%p, payload_size=%u\n"), m_payload, m_used_payload_size));

    if (m_used_payload_size > 0)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("data[0]=%u\n"), m_payload[0]));
        if (m_used_payload_size > 1)
        {
            ACE_DEBUG((LM_DEBUG, 
                       ACE_TEXT("last data, data[%u]=%u\n"),
                       (m_used_payload_size - 1),
                       m_payload[m_used_payload_size - 1]));
        }
    }
    
    return;
}


