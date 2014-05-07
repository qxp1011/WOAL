/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPSwitchConfig.cpp
 * Description:      OpenFlow switch config source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-15
 *
 * History 1:
 *     Date:         2013-12-21
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPSwitchConfig.h"
#include "CmError.h"

COFPSwitchConfig::COFPSwitchConfig(ACE_CDR::Octet abType, ACE_CDR::Octet abVersion, ACE_UINT16 awFlags, ACE_UINT16 awMissSendLen)
    : COFPMessage(abType, abVersion, 0, GetStreamLen())
    , m_wFlags(awFlags)
    , m_wMissSendLen(awMissSendLen)
{
    // omitting OFP10 type checking, as their values are the same as OFP13
    CM_ASSERT_RETURN_VOID((OFP13::OFPT_SET_CONFIG == abType) || (OFP13::OFPT_GET_CONFIG_REPLY == abType));
}


CmResult COFPSwitchConfig::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFPSwitchConfig::StreamFrom, COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_wFlags;
    is>>m_wMissSendLen;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


CmResult COFPSwitchConfig::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFPSwitchConfig::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_wFlags;
    os<<m_wMissSendLen;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


void COFPSwitchConfig::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPSwitchConfig::dump():\n")));
    COFPMessage::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("flags=%u, missSendLen=%u\n"), m_wFlags, m_wMissSendLen));
}



