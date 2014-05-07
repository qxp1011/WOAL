/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPTableModMsg.cpp
 * Description:      Source file for openflow table modification msg.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-16
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPTableModMsg.h"
#include "CmError.h"

CmResult COFP13TableModMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16 pad1 = 0;
    ACE_CDR::Octet   pad2 = 0;
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13TableModMsg::StreamFrom, COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_bTableId;
    is>>pad1;
    is>>pad2;
    is>>m_nConfig;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


CmResult COFP13TableModMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT16       pad1 = 0;
    ACE_CDR::Octet   pad2 = 0;
    CmResult         lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13TableModMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_bTableId;
    os<<pad1;
    os<<pad2;
    os<<m_nConfig;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


void COFP13TableModMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13TableModMsg::Dump()\n")));
    COFPMessage::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("tableId=%u, config=%u\n"), m_bTableId, m_nConfig));
}



