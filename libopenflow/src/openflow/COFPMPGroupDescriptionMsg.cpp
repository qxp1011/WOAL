/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPGroupDescriptionMsg.cpp
 * Description:      
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

#include "COFPMPGroupDescriptionMsg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

//COFP13MPGroupDescReplyMsg
COFP13MPGroupDescReplyMsg::~COFP13MPGroupDescReplyMsg()
{
    for (auto &item : m_bucketlist)
    {
        delete item;
    }
}

CmResult COFP13MPGroupDescReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;    
    
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_groupdesc.length;
    is>>m_groupdesc.type;
    is>>m_groupdesc.pad;
    is>>m_groupdesc.group_id;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_UINT16 wBucketLen = 0;
    ACE_INT16  ListLen = 
    GetMessageLength()-COFP13MultipartMsg::GetStreamLen()-sizeof(ofp_group_desc_stats);
    while (ListLen > 0)
    {
        COFP13Bucket *Bucket = nullptr;
        lRet = COFP13Bucket::DecodeBucket(is, Bucket);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Bucket, CM_ERROR_FAILURE);
        wBucketLen = Bucket->GetStreamLen();
        if (ListLen < wBucketLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_bucketlist.push_back(Bucket);
        m_wBucketLen += wBucketLen;
        ListLen -= wBucketLen;
    }
    
    return CM_OK;
}

CmResult COFP13MPGroupDescReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_groupdesc.length;
    os<<m_groupdesc.type;
    os<<m_groupdesc.pad;
    os<<m_groupdesc.group_id;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    std::list<COFP13Bucket *>::const_iterator it = m_bucketlist.begin();
    while (it != m_bucketlist.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }
    
    return CM_OK; 
}

ACE_UINT16 COFP13MPGroupDescReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(ofp_group_desc_stats)+m_wBucketLen;
}

void COFP13MPGroupDescReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPGroupDescReplyMsg::ofp_group_desc_stats &COFP13MPGroupDescReplyMsg::
GetGroupDesc()
{
    return m_groupdesc;
}

void COFP13MPGroupDescReplyMsg::SetGroupDesc(ofp_group_desc_stats &groupdesc)
{
    ::memcpy(&m_groupdesc, &groupdesc, sizeof(groupdesc));
}

CmResult COFP13MPGroupDescReplyMsg::
AddBucketList(std::list<COFP13Bucket *> &list)
{
    ACE_UINT16 wLen = 0;
    
    while (!list.empty())
    {
        COFP13Bucket *Bucket = list.front();
        CM_ASSERT_RETURN(Bucket != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_bucketlist.push_back(Bucket);
        wLen += Bucket->GetStreamLen();
    }

    m_wBucketLen += wLen;

    return CM_OK;
}

CmResult COFP13MPGroupDescReplyMsg::
AppendBucket(COFP13Bucket *apBucket)
{
    CM_ASSERT_RETURN(apBucket != nullptr, CM_ERROR_NULL_POINTER);

    m_wBucketLen += apBucket->GetStreamLen();
    m_bucketlist.push_back(apBucket);
    
    return CM_OK;
}

CmResult COFP13MPGroupDescReplyMsg::
GetBucketList(std::list<COFP13Bucket *> &list)
{
    while (!m_bucketlist.empty())
    {
        COFP13Bucket *Bucket = m_bucketlist.front();
        CM_ASSERT_RETURN(Bucket != nullptr, CM_ERROR_NULL_POINTER);
        m_bucketlist.pop_front();
        m_wBucketLen -= Bucket->GetStreamLen();
        list.push_back(Bucket);
    }

    return CM_OK;
}


