/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPEchoRequestMsg.cpp
 * Description:      Source file for openflow echo request message.
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

#include "COFPEchoRequestMsg.h"
#include "CmError.h"

std::allocator<ACE_CDR::Octet> COFP10EchoRequestMsg::m_alloc;

COFP10EchoRequestMsg::COFP10EchoRequestMsg(
        ACE_UINT16  awPayloadSize,
        ACE_CDR::Octet   *apPayload,
        // args for OFP header
        ACE_UINT32  adwXid,
        ACE_CDR::Octet    abVersion)
        : COFPMessage(OFP10::OFPT_ECHO_REQUEST, abVersion, adwXid, awPayloadSize + COFPMessage::GetStreamLen())
        , m_payload(NULL)     
        , m_used_payload_size(0)  // Initialized to 0. After the memory is allocated, it will be assigned the proper value.
        , m_payload_total_size(0)   // Initialized to 0. After the memory is allocated, it will be assigned the proper value. 
{
    if (0 == awPayloadSize)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10EchoRequestMsg::COFP10EchoRequestMsg(): awPayloadSize = 0\n")));
        return;
    }
    
    m_payload = m_alloc.allocate(awPayloadSize);
    if (NULL == m_payload)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP10EchoRequestMsg::COFP10EchoRequestMsg(): m_alloc.allocate(%u) failed\n"),
                   awPayloadSize));
        return;
    }
    m_payload_total_size = awPayloadSize;
    ::memset(m_payload, 0, awPayloadSize);

    if (NULL == apPayload)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP10EchoRequestMsg::COFP10EchoRequestMsg(): ptr apPayload passed in is NULL\n")));
        return;
    }

    ::memcpy(m_payload, apPayload, awPayloadSize);
    m_used_payload_size = awPayloadSize;
    
    return;
}

COFP10EchoRequestMsg::~COFP10EchoRequestMsg()
{
    if (m_payload)
    {
        m_alloc.deallocate(m_payload, m_payload_total_size);
        m_payload = NULL;
    }
}

ACE_UINT32 COFP10EchoRequestMsg::
GetPayload(const ACE_CDR::Octet *&payload, ACE_UINT16 &payload_size) const
{
    payload      = m_payload;
    payload_size = m_used_payload_size;

    return CM_OK;
}


CmResult COFP10EchoRequestMsg::
SetPayload(const ACE_CDR::Octet *payload, ACE_UINT16 payload_size)
{
    CmResult result = CM_ERROR_FAILURE;

    result = COFPMsgMemoryMgr::Copy(m_alloc, m_payload, m_payload_total_size, 
                                         m_used_payload_size, payload, payload_size);

    return result;
}

CmResult COFP10EchoRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10EchoRequestMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    if (m_wLength < OFP_MESSAGE_HDR_SIZE)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                  ACE_TEXT("COFPEchoRequest::StreamFrom(): m_wLength(%u) < %u\n"),
                  m_wLength,
                  OFP_MESSAGE_HDR_SIZE));
        return CM_ERROR_FAILURE;
    }

    ACE_UINT16 wEchoReqPayloadLen = m_wLength - OFP_MESSAGE_HDR_SIZE;
    if (m_payload_total_size < wEchoReqPayloadLen)
    {
        if (m_payload != NULL)
        {
            m_alloc.deallocate(m_payload, m_payload_total_size);
            m_payload            = NULL;
            m_payload_total_size = 0;
            m_used_payload_size  = 0;
        }

        m_payload = m_alloc.allocate(wEchoReqPayloadLen);
        if (NULL == m_payload)
        {
            ACE_ASSERT(0);
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFPEchoRequest::StreamFrom(): m_alloc.allocate(%u) failed.\n"),
                       wEchoReqPayloadLen));
            return CM_ERROR_FAILURE;
        }
        m_payload_total_size = wEchoReqPayloadLen;
    }
    
    if (wEchoReqPayloadLen != 0)
    {
        bool bGood = is.read_octet_array(m_payload, wEchoReqPayloadLen);
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        m_used_payload_size = wEchoReqPayloadLen;
    }

    return CM_OK;  
}


CmResult COFP10EchoRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10EchoRequestMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    if ((m_payload != NULL) && (m_used_payload_size != 0))
    {
        bool bGood = os.write_octet_array(m_payload, m_used_payload_size);
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;  
}


void COFP10EchoRequestMsg::Dump() const
{
    COFPMessage::Dump();
    
    if (NULL == m_payload)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFPEchoRequest::Dump(): m_payload is NULL.\n")));
        return;
    }
    
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPEchoRequest::Dump()\n")));
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

