/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPFeaturesReplyMsg.cpp
 * Description:      Source file for openflow features reply message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-17
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/
#include "COFPFeaturesReplyMsg.h"
#include "ace/CDR_Base.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP10FeaturesReplyMsg::COFP10FeaturesReplyMsg()
    : COFPMessage(OFP10::OFPT_FEATURES_REPLY, OFP10_VERSION)
    , m_lDataPathID(0)
    , m_dwBuffers(0)
    , m_bTables(0)
    , m_dwCapabilities(0)
    , m_dwActions(0)
    , m_wPortLen(0)
{
}

COFP10FeaturesReplyMsg::~COFP10FeaturesReplyMsg()
{
    for (auto &item : m_port_list)
    {
        delete item;
    }
}

CmResult COFP10FeaturesReplyMsg::
AddPortList(std::list<COFP10PhyPort *> &list)
{
    while (!list.empty())
    {
        COFP10PhyPort *Port = list.front();
        CM_ASSERT_RETURN(Port != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_wPortLen += Port->GetStreamLen();
        m_port_list.push_back(Port);
    }

    return CM_OK;
}

CmResult COFP10FeaturesReplyMsg::
AppendPort(COFP10PhyPort *aPort)
{
    CM_ASSERT_RETURN(aPort != nullptr, CM_ERROR_NULL_POINTER);
    m_port_list.push_back(aPort);
    m_wPortLen += aPort->GetStreamLen();
    return CM_OK;
}

CmResult COFP10FeaturesReplyMsg::
GetPortList(std::list<COFP10PhyPort *> &list)
{    
    while (!m_port_list.empty())
    {
        COFP10PhyPort *Port = m_port_list.front();
        CM_ASSERT_RETURN(Port != nullptr, CM_ERROR_NULL_POINTER);
        m_port_list.pop_front();
        m_wPortLen -= Port->GetStreamLen();
        list.push_back(Port);
    }

    return CM_OK;
}

CmResult COFP10FeaturesReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16     pad = 0;
    ACE_CDR::Octet pad2 = 0;
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FeaturesReplyMsg::StreamFrom(), COFPMessage::StreamFrom fail.\n")));
        return lRet;
    }

    is>>m_lDataPathID;
    is>>m_dwBuffers;
    is>>m_bTables;
    is>>pad;
    is>>pad2;
    is>>m_dwCapabilities;
    is>>m_dwActions;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_UINT16 wPortLen = 0;
    ACE_INT16 swListLen = m_wLength - COFPMessage::GetStreamLen() - 24;
    while (swListLen > 0)
    {
        COFP10PhyPort *Port = nullptr;
        lRet = COFP10PhyPort::DecodePort(is, Port);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Port != nullptr, CM_ERROR_FAILURE);
        wPortLen = Port->GetStreamLen();
        if (swListLen < wPortLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_port_list.push_back(Port);
        m_wPortLen += wPortLen;
        swListLen -= wPortLen;
    }

    return CM_OK;
}

CmResult COFP10FeaturesReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT16     pad  = 0;
    ACE_CDR::Octet pad2 = 0;
    CmResult       lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FeaturesReplyMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_lDataPathID;
    os<<m_dwBuffers;
    os<<m_bTables;
    os<<pad;
    os<<pad2;
    os<<m_dwCapabilities;
    os<<m_dwActions;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    std::list<COFP10PhyPort *>::const_iterator it = m_port_list.begin();
    while (it != m_port_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

void COFP10FeaturesReplyMsg::Dump() const
{
    COFPMessage::Dump();
}



CmResult COFP13FeaturesReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16 pad = 0;
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13FeaturesReplyMsg::StreamFrom(), COFPMessage::StreamFrom fail.\n")));
        return lRet;
    }

    is>>m_nDataPathID;
    is>>m_nBuffers;
    is>>m_bTables;
    is>>m_bAuxiliaryID;
    is>>pad;
    is>>m_nCapabilities;
    is>>m_nReserved;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


CmResult COFP13FeaturesReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT16 pad  = 0;
    CmResult   lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13FeaturesReplyMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_nDataPathID;
    os<<m_nBuffers;
    os<<m_bTables;
    os<<m_bAuxiliaryID;
    os<<pad;
    os<<m_nCapabilities;
    os<<m_nReserved;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


void COFP13FeaturesReplyMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13FeaturesReplyMsg::Dump():\n")));
    
    COFPMessage::Dump();

    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("dataPathId=%u, buffers=%u, tables=%u, auxiliaryId=%u, capabilities=%u\n"), 
               m_nDataPathID, m_nBuffers, m_bTables, m_bAuxiliaryID, m_nCapabilities));
}


