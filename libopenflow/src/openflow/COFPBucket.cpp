/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPBucket.cpp
 * Description:      OpenFlow bucket source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-13
 *
 * History 1:
 *     Date:         2013-12-21
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPBucket.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP13Bucket::COFP13Bucket(
    ACE_UINT16 awWeight,
    ACE_UINT32 anWatchPort,
    ACE_UINT32 anWatchGroup,
    ACE_UINT16 awLen)
    : m_wLen(16)
    , m_wWeight(awWeight)
    , m_dwWatchPort(anWatchPort)
    , m_dwWatchGroup(anWatchGroup)
{
    CM_ASSERT_RETURN_VOID(0 == (awLen % 8));
    m_wLen = awLen;
}


COFP13Bucket::~COFP13Bucket()
{
    for (auto &item : m_action_list)
    {
        delete item;
    }
}


CmResult COFP13Bucket::
AddActionList(std::list<COFP13Action *> &list)
{
    while (!list.empty())
    {
        COFP13Action *action = list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_action_list.push_back(action);
        m_wLen += action->GetStreamLen();
    }

    return CM_OK;
}


CmResult COFP13Bucket::
AppendAction(COFP13Action *action)
{
    CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
    m_action_list.push_back(action);
    m_wLen += action->GetStreamLen();
    
    return CM_OK;
}


CmResult COFP13Bucket::
GetActionList(std::list<COFP13Action *> &list)
{
    while (!m_action_list.empty())
    {
        COFP13Action *action = m_action_list.front();
        CM_ASSERT_RETURN(action != nullptr, CM_ERROR_NULL_POINTER);
        m_action_list.pop_front();
        m_wLen -= action->GetStreamLen();
        list.push_back(action);
    }

    CM_ASSERT_RETURN(16 == m_wLen, CM_ERROR_FAILURE);
    
    return CM_OK;
}


CmResult COFP13Bucket::
DecodeBucket(ACE_InputCDR &is, COFP13Bucket * &aBucket)
{
    CmResult lRet = CM_ERROR_FAILURE;

    COFP13Bucket *pBucket = new COFP13Bucket();
    CM_ASSERT_RETURN(pBucket != nullptr, CM_ERROR_OUT_OF_MEMORY);

    lRet = pBucket->StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    aBucket = pBucket;

    return CM_OK;
}

CmResult COFP13Bucket::StreamFrom(ACE_InputCDR &is)
{
    CmResult   lRet     = CM_ERROR_FAILURE;
    ACE_UINT16 wListLen = 0;
    ACE_UINT16 wActLen  = 0;
    ACE_UINT32 pad      = 0;

    is>>m_wLen;
    is>>m_wWeight;
    is>>m_dwWatchPort;
    is>>m_dwWatchGroup;
    is>>pad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    CM_ASSERT_RETURN(0 == (m_wLen % 8), CM_ERROR_INVALID_ARG);
    CM_ASSERT_RETURN(m_wLen >= 16,      CM_ERROR_INVALID_ARG);
    
    if (16 == m_wLen)
    {
        return CM_OK;
    }

    wListLen = m_wLen - 16;
    while (wListLen > 0)
    {
        COFP13Action *Act = nullptr;
        lRet = COFP13Action::DecodeAction(is, Act);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Act != nullptr, CM_ERROR_FAILURE);
        wActLen = Act->GetActionLen();
        if (wListLen < wActLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_action_list.push_back(Act);
        wListLen -= wActLen;
    }

    return CM_OK;
}


CmResult COFP13Bucket::StreamTo(ACE_OutputCDR &os)
{
    CmResult     lRet = CM_ERROR_FAILURE;
    ACE_UINT32   pad  = 0;

    m_wLen = GetStreamLen();
    
    CM_ASSERT_RETURN((m_wLen % 8) == 0, CM_ERROR_FAILURE);
    CM_ASSERT_RETURN(m_wLen >= 16,      CM_ERROR_FAILURE);
        
    os<<m_wLen;
    os<<m_wWeight;
    os<<m_dwWatchPort;
    os<<m_dwWatchGroup;
    os<<pad;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFP13Action *>::const_iterator it = m_action_list.begin();
    while (it != m_action_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}


ACE_UINT16 COFP13Bucket::GetStreamLen() const
{
    ACE_UINT16 streamLen = 16;
    for (auto &item : m_action_list)
    {
        streamLen += item->GetStreamLen();
    }

    return streamLen;
}


ACE_UINT16 COFP13Bucket::GetEncodeSpace() const
{
    return GetStreamLen();
}


void COFP13Bucket::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Bucket::Dump()\n")));
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("len=%u, weight=%u, watch_port=%u, watch_group=%u\n"), 
               m_wLen,
               m_wWeight,
               m_dwWatchPort,
               m_dwWatchGroup));
    if (GetStreamLen() > 16)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("action list:\n")));
        ACE_UINT16 i = 0;
        for (auto &item : m_action_list)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("action[%u]:\n"), i++));
            item->Dump();
        }
    }
}



