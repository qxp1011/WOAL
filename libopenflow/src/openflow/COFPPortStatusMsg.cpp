/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPortStatusMsg.cpp
 * Description:      Source file for openflow port status message.
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

#include "OFPMessageUtils.h"
#include "COFPPortStatusMsg.h"
#include "CmError.h"

COFP10PortStatusMsg::COFP10PortStatusMsg(ACE_CDR::Octet abVersion, ACE_CDR::Octet abReason)
    : COFPMessage(OFP10::OFPT_PORT_STATUS, abVersion)
    , m_reason(abReason)
{
}

CmResult COFP10PortStatusMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortStatusMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_reason;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    lRet = COFPMsgMemoryMgr::PadStreamTo(os, 7);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortStatusMsg::StreamTo, COFPMsgMemoryMgr::PadStreamTo fail\n")));
        return lRet;
    }

    return m_tDesc.StreamTo(os);
}

CmResult COFP10PortStatusMsg::StreamFrom(ACE_InputCDR &is)
{    
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortStatusMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_reason;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    lRet = COFPMsgMemoryMgr::PadStreamFrom(is, 7);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFPMsgMemoryMgr::PadStreamFrom fail\n")));
        return lRet;
    }

    return m_tDesc.StreamFrom(is);
}


void COFP10PortStatusMsg::Dump() const
{    
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10PortStatusMsg::Dump()\n")));
    
    COFPMessage::Dump();

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("reason = %u\n"), m_reason));
    m_tDesc.Dump();
}


COFP13PortStatusMsg::COFP13PortStatusMsg()
    : COFPMessage(OFP13::OFPT_PORT_STATUS)
    , m_reason(0)
{
}

CmResult COFP13PortStatusMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[7] = {0};
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PortStatusMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_reason;
    os.write_octet_array(pad, sizeof pad);

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return m_tDesc.StreamTo(os);
}


CmResult COFP13PortStatusMsg::StreamFrom(ACE_InputCDR &is)
{    
    ACE_CDR::Octet pad[7] = {0};
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PortStatusMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_reason;
    is.read_octet_array(pad, sizeof(pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return m_tDesc.StreamFrom(is);
}


void COFP13PortStatusMsg::Dump() const
{    
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13PortStatusMsg::Dump()\n")));
    
    COFPMessage::Dump();

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("reason = %u\n"), m_reason));
    m_tDesc.Dump();
}


