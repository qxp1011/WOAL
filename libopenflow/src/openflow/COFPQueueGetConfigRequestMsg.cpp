/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPQueueGetConfigRequestMsg.cpp
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

#include "OFPMessageUtils.h"
#include "COFPQueueGetConfigRequestMsg.h"
#include "CmError.h"

CmResult COFP10QueueGetConfigRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueGetConfigRequestMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_wPortNo;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamFrom(is, 2);    
}

CmResult COFP10QueueGetConfigRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueGetConfigRequestMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_wPortNo;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamTo(os, 2);    
}

void COFP10QueueGetConfigRequestMsg::Dump() const
{
    COFPMessage::Dump();
}




COFP13QueueGetConfigRequestMsg::
COFP13QueueGetConfigRequestMsg(ACE_UINT32 adwPortNo)
    : COFPMessage(OFP13::OFPT_QUEUE_GET_CONFIG_REQUEST, OFP13_VERSION, 0, GetStreamLen())
    , m_dwPort(adwPortNo)
{
}

CmResult COFP13QueueGetConfigRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 pad = 0;
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13QueueGetConfigRequestMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_dwPort;
    is>>pad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13QueueGetConfigRequestMsg::
StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 pad  = 0;
    CmResult   lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13QueueGetConfigRequestMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_dwPort;
    os<<pad;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13QueueGetConfigRequestMsg::Dump() const
{
    COFPMessage::Dump();
}

