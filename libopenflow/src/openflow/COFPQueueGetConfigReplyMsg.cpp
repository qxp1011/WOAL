/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPQueueGetConfigReplyMsg.cpp
 * Description:      Source file for openflow queue get config reply msg.
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
#include "COFPQueueGetConfigReplyMsg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP10QueueGetConfigReplyMsg::
~COFP10QueueGetConfigReplyMsg()
{
    for (auto &item : m_queue_list)
    {
        delete item;
    }
}

CmResult COFP10QueueGetConfigReplyMsg::
AddQueueList(std::list<COFP10PacketQueue *> &list)
{
    while (!list.empty())
    {
        COFP10PacketQueue *Queue = list.front();
        CM_ASSERT_RETURN(Queue != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_queue_list.push_back(Queue);
        m_wQueueLen += Queue->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP10QueueGetConfigReplyMsg::
AppendQueue(COFP10PacketQueue *pQueue)
{
    CM_ASSERT_RETURN(pQueue != nullptr, CM_ERROR_NULL_POINTER);
    m_queue_list.push_back(pQueue);
    m_wQueueLen += pQueue->GetStreamLen();
    
    return CM_OK;
}

CmResult COFP10QueueGetConfigReplyMsg::
GetQueueList(std::list<COFP10PacketQueue *> &list)
{
    while (!m_queue_list.empty())
    {
        COFP10PacketQueue *Queue = m_queue_list.front();
        CM_ASSERT_RETURN(Queue != nullptr, CM_ERROR_NULL_POINTER);
        m_queue_list.pop_front();
        m_wQueueLen -= Queue->GetStreamLen();
        list.push_back(Queue);
    }

    return CM_OK;
}

CmResult COFP10QueueGetConfigReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_INT16 swListLen = 0;
    ACE_UINT16 wQueueLen = 0;
    ACE_UINT32 pad = 0;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueGetConfigReplyMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.port;
    is.read_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    swListLen = m_wLength - COFPMessage::GetStreamLen() - sizeof m_tMember;
    if (0 == swListLen)
    {
        return CM_OK;
    }
    else if (swListLen < 0)
    {
        CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
    }

    while (swListLen > 0)
    {
        COFP10PacketQueue *Queue = new COFP10PacketQueue;
        CM_ASSERT_RETURN(Queue != nullptr, CM_ERROR_OUT_OF_MEMORY);
        lRet = Queue->StreamFrom(is);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        wQueueLen = Queue->GetQueueLen();
        if (swListLen < wQueueLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_queue_list.push_back(Queue);
        swListLen -= wQueueLen;
    }

    return CM_OK;
}

CmResult COFP10QueueGetConfigReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueGetConfigReplyMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_tMember.port;
    os.write_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    std::list<COFP10PacketQueue *>::const_iterator it = m_queue_list.begin();
    while (it != m_queue_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

void COFP10QueueGetConfigReplyMsg::Dump() const
{
    COFPMessage::Dump();
}



COFP13QueueGetConfigReplyMsg::COFP13QueueGetConfigReplyMsg(ACE_UINT32 adwPortNo)
    : COFPMessage(OFP13::OFPT_QUEUE_GET_CONFIG_REPLY, OFP13_VERSION, 0, GetStreamLen())
    , m_dwPort(adwPortNo)
    , m_wStreamLen(8)
{
}

COFP13QueueGetConfigReplyMsg::~COFP13QueueGetConfigReplyMsg()
{
    for (auto &item : m_queue_list)
    {
        delete item;
    }
}

CmResult COFP13QueueGetConfigReplyMsg::
AddQueueList(std::list<COFP13PacketQueue *> &list)
{
    while (!list.empty())
    {
        COFP13PacketQueue *Queue = list.front();
        CM_ASSERT_RETURN(Queue != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_queue_list.push_back(Queue);
        m_wStreamLen += Queue->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP13QueueGetConfigReplyMsg::
AppendQueue(COFP13PacketQueue *pQueue)
{
    CM_ASSERT_RETURN(pQueue != nullptr, CM_ERROR_NULL_POINTER);
    m_queue_list.push_back(pQueue);
    m_wStreamLen += pQueue->GetStreamLen();
    
    return CM_OK;
}

CmResult COFP13QueueGetConfigReplyMsg::
GetQueueList(std::list<COFP13PacketQueue *> &list)
{
    while (!m_queue_list.empty())
    {
        COFP13PacketQueue *Queue = m_queue_list.front();
        CM_ASSERT_RETURN(Queue != nullptr, CM_ERROR_NULL_POINTER);
        m_queue_list.pop_front();
        m_wStreamLen -= Queue->GetStreamLen();
        list.push_back(Queue);
    }

    return CM_OK;
}

CmResult COFP13QueueGetConfigReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_INT16 swListLen = 0;
    ACE_UINT16 wQueueLen = 0;
    ACE_UINT32 pad = 0;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13QueueGetConfigReplyMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_dwPort;
    is>>pad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    m_wStreamLen = m_wLength - COFPMessage::GetStreamLen();
    swListLen = m_wStreamLen - 8;    
    while (swListLen > 0)
    {
        COFP13PacketQueue *Queue = new COFP13PacketQueue;
        CM_ASSERT_RETURN(Queue != nullptr, CM_ERROR_OUT_OF_MEMORY);
        lRet = Queue->StreamFrom(is);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        wQueueLen = Queue->GetQueueLen();
        if (swListLen < wQueueLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_queue_list.push_back(Queue);
        swListLen -= wQueueLen;
    }
    
    return CM_OK;
}

CmResult COFP13QueueGetConfigReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult   lRet = CM_ERROR_FAILURE;
    ACE_UINT32 pad  = 0;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13QueueGetConfigReplyMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_dwPort;
    os<<pad;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFP13PacketQueue *>::const_iterator it = m_queue_list.begin();
    while (it != m_queue_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

void COFP13QueueGetConfigReplyMsg::Dump() const
{
    COFPMessage::Dump();
}

