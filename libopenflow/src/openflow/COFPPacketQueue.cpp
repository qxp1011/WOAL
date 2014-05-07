/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPacketQueue.cpp
 * Description:      OpenFlow packet queue source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-20
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/
#include "ByteOrder.h"
#include "openflow-common.h"
#include "COFPPacketQueue.h"
#include "OFPMessageUtils.h"
#include "CmError.h"
#include "ace/Log_Msg.h"

using namespace openflow;

std::allocator<ACE_CDR::Octet> COFP13QueuePropExperimenter::m_alloc;

COFPQueueProp::
COFPQueueProp(ACE_UINT16 awProp, ACE_UINT16 awPropLen)
    : m_wProperty(awProp)
    , m_wPropLen(awPropLen)
{
    CM_ASSERT_RETURN_VOID(m_wPropLen > 8);
}

CmResult COFPQueueProp::
DecodeQueueProp(ACE_InputCDR &is, COFPQueueProp * &aProp)
{
    CmResult   lRet      = CM_ERROR_FAILURE;
    ACE_UINT16 wProperty = 0;

    bool bGood = is.pre_read(&wProperty, sizeof(wProperty));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    COFPQueueProp *Prop = nullptr;
    ACE_CDR::swap_2(reinterpret_cast<const char *>(&wProperty), reinterpret_cast<char *>(&wProperty));

    switch(wProperty)
    {
        case OFPQT_MIN_RATE:
        case OFPQT_MAX_RATE:
        {
            Prop = new COFPQueuePropRate(wProperty);
            break;
        }
        case OFPQT_EXPERIMENTER:
        {
            Prop = new COFP13QueuePropExperimenter();
            break;
        }
        default:
        {
            break;
        }
    }

    CM_ASSERT_RETURN(Prop, CM_ERROR_OUT_OF_MEMORY);
    
    lRet = Prop->StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("DecodeQueueProp::StreamFrom(): fail in StreamFrom(), lRet=%l\n"), lRet));
        return lRet;
    }

    aProp = Prop;
    
    return CM_OK;
}

// for interface IOFPMsgParser
CmResult COFPQueueProp::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 pad = 0;
    
    is>>m_wProperty;
    is>>m_wPropLen;
    is>>pad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFPQueueProp::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 pad = 0;

    m_wPropLen = GetStreamLen();
    
    os<<m_wProperty;
    os<<m_wPropLen;
    os<<pad;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFPQueueProp::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPQueueProp::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("property=%u, propertyLen=%u\n"), m_wProperty, m_wPropLen));
}

COFPQueuePropRate::
COFPQueuePropRate(ACE_UINT16 awProp, ACE_UINT16 awRate)
    : COFPQueueProp(0, 16)
    , m_wRate(awRate)
{
    CM_ASSERT_RETURN_VOID((OFPQT_MIN_RATE == awProp) || (OFPQT_MAX_RATE == awProp));
    m_wProperty = awProp;
}

CmResult COFPQueuePropRate::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;

    CmResult result = COFPQueueProp::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_wRate;
    is>>pad;
    is>>pad2;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFPQueuePropRate::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;

    CmResult result = COFPQueueProp::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    os<<m_wRate;
    os<<pad;
    os<<pad2;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFPQueuePropRate::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPQueuePropRate::Dump():\n")));
    COFPQueueProp::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("rate=%u\n"), m_wRate));
}

COFP13QueuePropExperimenter::
COFP13QueuePropExperimenter(ACE_UINT32 anExperimenterId)
    : COFPQueueProp(OFPQT_EXPERIMENTER, 16)
    , m_experimenter(anExperimenterId)
    , m_dataTotalSize(0) // total size of allocated memory
    , m_data(NULL)
{
}

COFP13QueuePropExperimenter::
~COFP13QueuePropExperimenter()
{
    if (m_data != NULL)
    {
        m_alloc.deallocate(m_data, m_dataTotalSize);
    }
}

CmResult COFP13QueuePropExperimenter::
SetData(ACE_CDR::Octet *abData, ACE_UINT16 awDataSize)
{
    ACE_UINT16 wUsedMemSize = GetUsedDataSize();
    CmResult result = COFPMsgMemoryMgr::Copy(m_alloc, m_data, m_dataTotalSize, wUsedMemSize, abData, awDataSize);
    if (CM_SUCCEEDED(result))
    {
        m_wPropLen += awDataSize;
    }
    
    return result;
}

CmResult COFP13QueuePropExperimenter::StreamFrom(ACE_InputCDR &is)
{
    CmResult result = COFPQueueProp::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    ACE_UINT32 pad = 0;

    is>>m_experimenter;
    is>>pad;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_UINT16 dataSize     = GetUsedDataSize();
    ACE_UINT16 usedDataSize = 0;    // no use, just for calling func
    result = COFPMsgMemoryMgr::DataStreamFrom(m_alloc, is, dataSize, m_data, m_dataTotalSize, usedDataSize);
    
    return result;
}

CmResult COFP13QueuePropExperimenter::StreamTo(ACE_OutputCDR &os)
{
    CmResult result = COFPQueueProp::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    ACE_UINT32 pad = 0;

    os<<m_experimenter;
    os<<pad;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_UINT16 dataSize = GetUsedDataSize();
    bGood = os.write_octet_array(m_data, dataSize);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13QueuePropExperimenter::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13QueuePropExperimenter::Dump():\n")));
    COFPQueueProp::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("experimenter=%u\n"), m_experimenter));

    ACE_UINT16 dataSize = GetUsedDataSize();
    if (dataSize > 0)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("data:\n")));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("data[0]=%u\n"), m_data[0]));
        if (dataSize > 1)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("data[%u]=%u\n"), dataSize - 1, m_data[dataSize - 1]));
        }
    }
}

COFP10PacketQueue::
~COFP10PacketQueue()
{
    for (auto &item : m_prop_list)
    {
        delete item;
    }
}

CmResult COFP10PacketQueue::
AddPropList(std::list<COFPQueueProp *> &list)
{
    while (!list.empty())
    {
        COFPQueueProp *Prop = list.front();
        CM_ASSERT_RETURN(Prop != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_prop_list.push_back(Prop);
        m_tMember.len += Prop->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP10PacketQueue::AppendProp(COFPQueueProp * Prop)
{
    CM_ASSERT_RETURN(Prop != nullptr, CM_ERROR_NULL_POINTER);
    m_tMember.len += Prop->GetStreamLen();
    m_prop_list.push_back(Prop);
    
    return CM_OK;
}

CmResult COFP10PacketQueue::
GetPropList(std::list<COFPQueueProp *> &list)
{
    while (!m_prop_list.empty())
    {
        COFPQueueProp *Prop = m_prop_list.front();
        CM_ASSERT_RETURN(Prop != nullptr, CM_ERROR_NULL_POINTER);
        m_prop_list.pop_front();
        m_tMember.len -= Prop->GetStreamLen();
        list.push_back(Prop);
    }

    return CM_OK;
}

CmResult COFP10PacketQueue::StreamFrom(ACE_InputCDR &is)
{
    is>>m_tMember.queue_id;
    is>>m_tMember.len;
    is.read_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_INT16 swListLen = m_tMember.len - sizeof m_tMember;
    if (swListLen == 0)
    {
        return CM_OK;
    }
    else if (swListLen < 0)
    {
        CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
    }
    
    ACE_UINT16 wPropLen = 0;
    while (swListLen > 0)
    {
        COFPQueueProp *Prop = nullptr;
        CmResult lRet = COFPQueueProp::DecodeQueueProp(is, Prop);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Prop, CM_ERROR_FAILURE);
        wPropLen = Prop->GetPropertyLen();
        if (swListLen < wPropLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_prop_list.push_back(Prop);
        swListLen -= wPropLen;
    }

    return CM_OK;
}

CmResult COFP10PacketQueue::StreamTo(ACE_OutputCDR &os)
{
    os<<m_tMember.queue_id;
    os<<m_tMember.len;
    os.write_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFPQueueProp *>::const_iterator it = m_prop_list.begin();
    while (it != m_prop_list.end())
    {
        CmResult lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

void COFP10PacketQueue::Dump() const
{
}



COFP13PacketQueue::
COFP13PacketQueue(ACE_UINT32 adwQueueId, ACE_UINT32 adwPort)
    : m_dwQueueId(adwQueueId)
    , m_dwPort(adwPort)
    , m_wQueueLen(16)
{
}

COFP13PacketQueue::
~COFP13PacketQueue()
{
    for (auto &item : m_prop_list)
    {
        delete item;
    }
}

CmResult COFP13PacketQueue::
AddPropList(std::list<COFPQueueProp *> &list)
{
    while (!list.empty())
    {
        COFPQueueProp *Prop = list.front();
        CM_ASSERT_RETURN(Prop != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_prop_list.push_back(Prop);
        m_wQueueLen += Prop->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP13PacketQueue::
AppendProp(COFPQueueProp *Prop)
{
    CM_ASSERT_RETURN(Prop != nullptr, CM_ERROR_NULL_POINTER);
    m_prop_list.push_back(Prop);
    m_wQueueLen += Prop->GetStreamLen();
    
    return CM_OK;
}

CmResult COFP13PacketQueue::
GetPropList(std::list<COFPQueueProp *> &list)
{
    while (!m_prop_list.empty())
    {
        COFPQueueProp *Prop = m_prop_list.front();
        CM_ASSERT_RETURN(Prop != nullptr, CM_ERROR_NULL_POINTER);
        m_prop_list.pop_front();
        m_wQueueLen -= Prop->GetStreamLen();
        list.push_back(Prop);
    }

    return CM_OK;
}

CmResult COFP13PacketQueue::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_INT16 swListLen = 0;
    ACE_UINT16 wPropLen = 0;
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;

    is>>m_dwQueueId;
    is>>m_dwPort;
    is>>m_wQueueLen;
    is>>pad;
    is>>pad2;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    CM_ASSERT_RETURN(m_wQueueLen >= 16, CM_ERROR_FAILURE);
    
    if (16 == m_wQueueLen)
    {
        return CM_OK;;
    }
    
    swListLen = m_wQueueLen - 16;
    while (swListLen > 0)
    {
        COFPQueueProp *Prop = nullptr;
        lRet = COFPQueueProp::DecodeQueueProp(is, Prop);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Prop, CM_ERROR_FAILURE);
        wPropLen = Prop->GetPropertyLen();
        if (swListLen < wPropLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_prop_list.push_back(Prop);
        swListLen -= wPropLen;
    }
    
    return CM_OK;
}

CmResult COFP13PacketQueue::StreamTo(ACE_OutputCDR &os)
{
    CmResult   lRet = CM_ERROR_FAILURE;
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;

    m_wQueueLen = GetStreamLen();

    os<<m_dwQueueId;
    os<<m_dwPort;
    os<<m_wQueueLen;
    os<<pad;
    os<<pad2;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFPQueueProp *>::const_iterator it = m_prop_list.begin();
    while (it != m_prop_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

ACE_UINT16 COFP13PacketQueue::GetStreamLen()                             const
{
    ACE_UINT16 streamLen = 0;
    streamLen += sizeof(m_dwQueueId) + sizeof(m_dwPort) + sizeof(m_wQueueLen) + 6 * sizeof(ACE_CDR::Octet);
    for (auto &item : m_prop_list)
    {
        streamLen += item->GetStreamLen();
    }
    
    return streamLen;
}

ACE_UINT16 COFP13PacketQueue::GetEncodeSpace()                           const
{
    return GetStreamLen();
}

void COFP13PacketQueue::Dump()                                       const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13PacketQueue::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("queueId=%u, port=%u, len=%u\n"), 
               m_dwQueueId,
               m_dwPort,
               m_wQueueLen));
    ACE_UINT32 i = 0;
    if (!m_prop_list.empty())
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("prop list:\n")));
    }
    for (auto &item : m_prop_list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("prop[%u]:\n"), i++));
        item->Dump();
    }
}


