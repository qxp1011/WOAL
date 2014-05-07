/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPPortDescriptionMsg.cpp
 * Description:      
 * Others:
 * Version:          V1.0
 * Author:           
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#include "COFPMPPortDescriptionMsg.h"
#include "COFPPort.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP13MPPortDescReplyMsg::~COFP13MPPortDescReplyMsg()
{
    for (auto &item : m_port_list)
    {
        delete item;
    }
}

// aList should meet the pre-requisite in Table~11: Match fields details.
CmResult COFP13MPPortDescReplyMsg::AddPortList(std::list<COFP13Port *> &aList)
{
    ACE_UINT32 nPortListLen = 0;
    while (!aList.empty())
    {
        COFP13Port *port = aList.front();
        CM_ASSERT_RETURN(port != nullptr, CM_ERROR_NULL_POINTER);
        aList.pop_front();
        m_port_list.push_back(port);
        nPortListLen += port->GetStreamLen();
    }

    m_wLength += nPortListLen;

    return CM_OK;
}


CmResult COFP13MPPortDescReplyMsg::AppendPort(COFP13Port *apPort)
{
    CM_ASSERT_RETURN(apPort != nullptr, CM_ERROR_NULL_POINTER);
    m_port_list.push_back(apPort);
    m_wLength += apPort->GetStreamLen();
    return CM_OK;
}


CmResult COFP13MPPortDescReplyMsg::GetPortList(std::list<COFP13Port *> &aList)
{
    while (!m_port_list.empty())
    {
        COFP13Port *port = m_port_list.front();
        CM_ASSERT_RETURN(port != nullptr, CM_ERROR_NULL_POINTER);
        m_port_list.pop_front();
        m_wLength -= port->GetStreamLen();
        aList.push_back(port);
    }

    return CM_OK;
}

//COFP13MPPortDescReplyMsg
CmResult COFP13MPPortDescReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult result = CM_ERROR_FAILURE;

    result = COFP13MultipartMsg::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    ACE_INT16 size = m_wLength-8-COFPMessage::GetStreamLen();
    while (size > 0)
    {
        COFP13Port *port = nullptr;
        result = COFP13Port::DecodePort(is, port);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

        m_port_list.push_back(port);

        size -= port->GetStreamLen();
        if (size<0)
        {
            return CM_ERROR_FAILURE;
        }
    }

    return CM_OK;
}

CmResult COFP13MPPortDescReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult result = CM_ERROR_FAILURE;

    result = COFP13MultipartMsg::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    for (auto &port : m_port_list) 
    {
        result = port->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    }

    return CM_OK;
}

ACE_UINT16 COFP13MPPortDescReplyMsg::GetStreamLen() const
{
    ACE_UINT16 size = 0;
    for (auto &port : m_port_list)
    {
        size += port->GetStreamLen();
    }

    return size + COFP13MultipartMsg::GetStreamLen();
}

void COFP13MPPortDescReplyMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MPPortDescReplyMsg::Dump():\n")));
    
    COFP13MultipartMsg::Dump();
    ACE_UINT32 i = 0;
    for (auto &port : m_port_list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("port[%u]:\n"), i++));
        port->Dump();
    }
}


