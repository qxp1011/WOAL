/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPRoleMsg.cpp
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

#include "COFPRoleMsg.h"
#include "CmError.h"

COFP13RoleMsg::
COFP13RoleMsg(ACE_CDR::Octet abType, ACE_UINT32 adwRole, ACE_UINT64 alId)
    : COFPMessage(abType, OFP13_VERSION, 0, GetStreamLen())
    , m_dwRole(adwRole)
    , m_lGenerationId(alId)
{
    if ((abType != OFP13::OFPT_ROLE_REQUEST) && (abType != OFP13::OFPT_ROLE_REPLY))
    {
        ACE_ASSERT(0);
    }
}

CmResult COFP13RoleMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 pad = 0;
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13RoleMsg::StreamFrom, COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_dwRole;
    is>>pad;
    is>>m_lGenerationId;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13RoleMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 pad  = 0;
    CmResult   lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13RoleMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_dwRole;
    os<<pad;
    os<<m_lGenerationId;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13RoleMsg::Dump()const
{
    COFPMessage::Dump();
}

