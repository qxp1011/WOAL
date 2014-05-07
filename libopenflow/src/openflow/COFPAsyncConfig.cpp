/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPAsyncConfig.cpp
 * Description:      
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-15
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: adapt to ACE
**********************************************************************/

#include "COFPAsyncConfig.h"
#include "ace/Log_Msg.h"
#include "CmError.h"

COFP13AsyncConfig::COFP13AsyncConfig(ACE_CDR::Octet abType)
    : COFPMessage(abType)
{
    ::memset(m_dwPacketInMask, 0, sizeof(m_dwPacketInMask));
    ::memset(m_dwPortStatusMask, 0, sizeof(m_dwPortStatusMask));
    ::memset(m_dwFlowRemovedMask, 0, sizeof(m_dwFlowRemovedMask));
}

void COFP13AsyncConfig::
SetPacketInMask(ACE_UINT32 aacMask[ASYNC_CFG_MASK_ELE_NUM])
{
    for (ACE_CDR::Octet i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        m_dwPacketInMask[i] = aacMask[i];
    }
}

void COFP13AsyncConfig::
SetPortStatusMask(ACE_UINT32 aacMask[ASYNC_CFG_MASK_ELE_NUM])
{
    for (ACE_CDR::Octet i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        m_dwPortStatusMask[i] = aacMask[i];
    }
}

void COFP13AsyncConfig::
SetFlowRemovedMask(ACE_UINT32 aacMask[ASYNC_CFG_MASK_ELE_NUM])
{
    for (ACE_CDR::Octet i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        m_dwFlowRemovedMask[i] = aacMask[i];
    }
}

void COFP13AsyncConfig::
GetPacketInMask(ACE_UINT32 (&aacMask)[ASYNC_CFG_MASK_ELE_NUM])
{
    for (ACE_CDR::Octet i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        aacMask[i] = m_dwPacketInMask[i];
    }
}

void COFP13AsyncConfig::
GetPortStatusMask(ACE_UINT32 (&aacMask)[ASYNC_CFG_MASK_ELE_NUM])
{
    for (ACE_CDR::Octet i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        aacMask[i] = m_dwPortStatusMask[i];
    }
}

void COFP13AsyncConfig::
GetFlowRemovedMask(ACE_UINT32 (&aacMask)[ASYNC_CFG_MASK_ELE_NUM])
{
    for (ACE_CDR::Octet i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        aacMask[i] = m_dwFlowRemovedMask[i];
    }
}

CmResult COFP13AsyncConfig::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_CDR::Octet i = 0;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13AsyncConfig::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    for (i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        is>>m_dwPacketInMask[i];
    }    
    for (i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        is>>m_dwPortStatusMask[i];
    }
    for (i = 0; i < ASYNC_CFG_MASK_ELE_NUM; i++)
    {
        is>>m_dwFlowRemovedMask[i];
    }
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13AsyncConfig::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;    
    ACE_CDR::Octet i = 0;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13AsyncConfig::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }    

    os.write_ulong_array(m_dwPacketInMask, ASYNC_CFG_MASK_ELE_NUM);
    os.write_ulong_array(m_dwPortStatusMask, ASYNC_CFG_MASK_ELE_NUM);
    os.write_ulong_array(m_dwFlowRemovedMask, ASYNC_CFG_MASK_ELE_NUM);
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

ACE_UINT16 COFP13AsyncConfig::GetStreamLen() const
{
    ACE_UINT16 wLen = sizeof(m_dwPacketInMask)+sizeof(m_dwPortStatusMask)+sizeof(m_dwFlowRemovedMask);
    return COFPMessage::GetStreamLen() + wLen;
}

void COFP13AsyncConfig::Dump() const
{
    COFPMessage::Dump();
}

