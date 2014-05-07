/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPGroupStatisticsMsg.cpp
 * Description:      Source file for openflow multipart group statistics msg.
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

#include "COFPMPGroupStatisticsMsg.h"
#include "CmError.h"

//COFP13MPGroupStatsRequestMsg
CmResult COFP13MPGroupStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_stats_request.group_id;
    is.read_octet_array(m_stats_request.pad, sizeof(m_stats_request.pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK; 

}

CmResult COFP13MPGroupStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_stats_request.group_id;
    os.write_octet_array(m_stats_request.pad, sizeof(m_stats_request.pad));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK; 

}

ACE_UINT16 COFP13MPGroupStatsRequestMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_stats_request);
}

void COFP13MPGroupStatsRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPGroupStatsRequestMsg::ofp_group_stats_request &
COFP13MPGroupStatsRequestMsg::GetGroupStatsRequestInfo()
{
    return m_stats_request;
}

void COFP13MPGroupStatsRequestMsg::
SetGroupStatsRequestInfo(ofp_group_stats_request &requestInfo)
{
    ::memcpy(&m_stats_request, &requestInfo, sizeof(requestInfo));
}


//COFP13MPGroupStatsReplyMsg
CmResult COFP13MPGroupStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    //m_groupstats.length = sizeof(m_groupstats)+m_bucket_list.size()*16;

    is>>m_groupstats.length;
    is.read_octet_array(m_groupstats.pad, sizeof(m_groupstats.pad));
    is>>m_groupstats.group_id;
    is>>m_groupstats.ref_count;
    is.read_octet_array(m_groupstats.pad2, sizeof(m_groupstats.pad2));
    is>>m_groupstats.packet_count;
    is>>m_groupstats.byte_count;
    is>>m_groupstats.duration_sec;
    is>>m_groupstats.duration_nsec;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_INT16 bucketsize = m_groupstats.length-sizeof(m_groupstats);
    ofp_bucket_counter counter;
    while (bucketsize>0)
    {
        is>>counter.packet_count;
        is>>counter.byte_count;
        AddBucketCounter(counter);
        bucketsize = bucketsize-sizeof(ofp_bucket_counter);
        if (bucketsize < 0)
        {
            return CM_ERROR_FAILURE;
        }
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK; 

}

CmResult COFP13MPGroupStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    ACE_UINT16 length = sizeof(m_groupstats)+m_bucket_list.size()*16;

    os<<length;
    os.write_octet_array(m_groupstats.pad,sizeof(m_groupstats.pad));
    os<<m_groupstats.group_id;
    os<<m_groupstats.ref_count;
    os.write_octet_array(m_groupstats.pad2,sizeof(m_groupstats.pad2));
    os<<m_groupstats.packet_count;
    os<<m_groupstats.byte_count;
    os<<m_groupstats.duration_sec;
    os<<m_groupstats.duration_nsec;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    for (auto &acounter : m_bucket_list) 
    {
        os<<acounter.packet_count;
        os<<acounter.byte_count;
        bGood = os.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK; 
}

ACE_UINT16 COFP13MPGroupStatsReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_groupstats)+m_bucket_list.size()*16;   
}

void COFP13MPGroupStatsReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPGroupStatsReplyMsg::ofp_group_stats &
COFP13MPGroupStatsReplyMsg::GetGroupStatsReplyInfo()
{
    return m_groupstats;
}

void COFP13MPGroupStatsReplyMsg::
SetGroupStatsReplyInfo(ofp_group_stats &groupstats)
{
    ::memcpy(&m_groupstats, &groupstats, sizeof(groupstats));
}

void COFP13MPGroupStatsReplyMsg::AddBucketCounter(ofp_bucket_counter &counter)
{
    m_bucket_list.push_back(counter);
}

void COFP13MPGroupStatsReplyMsg::
GetBucketCounterList(std::list<ofp_bucket_counter> &list)
{
    while (!m_bucket_list.empty())
    {
        ofp_bucket_counter &item = m_bucket_list.front();
        m_bucket_list.pop_front();
        list.push_back(item);
    }
}

void COFP13MPGroupStatsReplyMsg::
SetBucketCounterList(std::list<ofp_bucket_counter> &list)
{
    while (!list.empty())
    {
        ofp_bucket_counter &item = list.front();
        list.pop_front();
        m_bucket_list.push_back(item);
    }
}

ACE_UINT32 COFP13MPGroupStatsReplyMsg::GetBucketListSize() const
{
    return m_bucket_list.size();
}

