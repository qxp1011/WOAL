/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPGroupModMsg.cpp
 * Description:      source file for openflow group modification message
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-15
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPGroupModMsg.h"
#include "ace/Log_Msg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP13GroupModMsg::COFP13GroupModMsg(
        ACE_UINT16 awCommand,     /* One of OFPGC_*. */
        ACE_CDR::Octet   abGroupType,   /* One of OFPGT_*. */
        ACE_UINT32 adwGroupId)
    : COFPMessage(OFP13::OFPT_GROUP_MOD)
    , m_wStreamLen(8)
    , m_wCommand(awCommand)
    , m_bGroupType(abGroupType)
    , m_dwGroupId(adwGroupId)
{
}


COFP13GroupModMsg::~COFP13GroupModMsg()
{
    for (auto &item : m_bucket_list)
    {
        delete item;
    }
}


CmResult COFP13GroupModMsg::
AddBucketList(std::list<COFP13Bucket *> &list)
{
    while (!list.empty())
    {
        COFP13Bucket *Bucket = list.front();
        CM_ASSERT_RETURN(Bucket != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_bucket_list.push_back(Bucket);
        m_wStreamLen += Bucket->GetStreamLen();
    }

    return CM_OK;
}


CmResult COFP13GroupModMsg::
AppendBucket(COFP13Bucket *pBucket)
{
    CM_ASSERT_RETURN(pBucket != nullptr, CM_ERROR_NULL_POINTER);
    m_wStreamLen += pBucket->GetStreamLen();
    m_bucket_list.push_back(pBucket);
    
    return CM_OK;
}


CmResult COFP13GroupModMsg::
GetBucketList(std::list<COFP13Bucket *> &list)
{
    while (!m_bucket_list.empty())
    {
        COFP13Bucket *pBucket = m_bucket_list.front();
        CM_ASSERT_RETURN(pBucket != nullptr, CM_ERROR_NULL_POINTER);
        m_bucket_list.pop_front();
        m_wStreamLen -= pBucket->GetStreamLen();
        list.push_back(pBucket);
    }

    return CM_OK;
}

CmResult COFP13GroupModMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_INT16 swListLen = 0;
    ACE_UINT16  wBucketLen = 0;
    ACE_CDR::Octet    pad = 0;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13GroupModMsg::StreamFrom, COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_wCommand;
    is>>m_bGroupType;
    is>>pad;
    is>>m_dwGroupId;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    m_wStreamLen = m_wLength - COFPMessage::GetStreamLen();
    swListLen   = m_wStreamLen - 8;    
    while (swListLen > 0)
    {
        COFP13Bucket *Bucket = nullptr;
        lRet = COFP13Bucket::DecodeBucket(is, Bucket);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Bucket != nullptr, CM_ERROR_FAILURE);
        wBucketLen = Bucket->GetLen();
        if (swListLen < wBucketLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_bucket_list.push_back(Bucket);
        swListLen -= wBucketLen;
    }

    return CM_OK;
}


CmResult COFP13GroupModMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult        lRet = CM_ERROR_FAILURE;
    ACE_CDR::Octet  pad  = 0;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13GroupModMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_wCommand;
    os<<m_bGroupType;
    os<<pad;
    os<<m_dwGroupId;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFP13Bucket *>::const_iterator it = m_bucket_list.begin();
    while (it != m_bucket_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}


void COFP13GroupModMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13GroupModMsg::Dump()\n")));
    COFPMessage::Dump();
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("command=%u, type=%u, group_id=%u\n"),
               m_wCommand,
               m_bType,
               m_dwGroupId));
    std::list<COFP13Bucket *>::const_iterator it = m_bucket_list.begin();

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("Bucket List:\n")));
    while(it != m_bucket_list.end())
    {
        //(*it)->Dump();
        it++;
    }
}



