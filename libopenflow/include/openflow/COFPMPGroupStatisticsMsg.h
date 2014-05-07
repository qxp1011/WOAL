/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPGroupStatisticsMsg.h
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
#ifndef COFPMPGROUPSTATISTICSMSG_H
#define COFPMPGROUPSTATISTICSMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPGroupStatsRequestMsg 
    : public COFP13MultipartRequestMsg
{
public:
    /* Body of OFPMP_GROUP request. */
    struct ofp_group_stats_request {
        ACE_UINT32 group_id; /* All groups if OFPG_ALL. */
        ACE_CDR::Octet pad[4]; /* Align to 64 bits. */
    };

    static_assert((sizeof(ofp_group_stats_request) == 8),"must be 8");

    COFP13MPGroupStatsRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_GROUP)
    {
        ::memset(&m_stats_request, 0,sizeof(m_stats_request));
    }
    virtual ~COFP13MPGroupStatsRequestMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void     Dump() const;

    COFP13MPGroupStatsRequestMsg::ofp_group_stats_request &
    GetGroupStatsRequestInfo();
    void SetGroupStatsRequestInfo(ofp_group_stats_request &requestInfo);
    
private:
    ofp_group_stats_request m_stats_request;
};


class COFP13MPGroupStatsReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Used in group stats replies. */
    struct ofp_bucket_counter {
        uint64_t packet_count; /* Number of packets processed by bucket. */
        uint64_t byte_count; /* Number of bytes processed by bucket. */
    };

    /* Body of reply to OFPMP_GROUP request. */
    struct ofp_group_stats {
        ACE_UINT16 length; /* Length of this entry. */
        ACE_CDR::Octet pad[2]; /* Align to 64 bits. */
        ACE_UINT32 group_id; /* Group identifier. */
        ACE_UINT32 ref_count; /* Number of flows or groups that directly forward to this group. */
        ACE_CDR::Octet pad2[4]; /* Align to 64 bits. */
        uint64_t packet_count; /* Number of packets processed by group. */
        uint64_t byte_count; /* Number of bytes processed by group. */
        ACE_UINT32 duration_sec; /* Time group has been alive in seconds. */
        ACE_UINT32 duration_nsec; /* Time group has been alive in nanoseconds beyond duration_sec. */
        //struct ofp_bucket_counter bucket_stats[0];
    };
    
    static_assert((sizeof(struct ofp_bucket_counter) == 16), "must be 16");

    static_assert((sizeof(struct ofp_group_stats) == 40), "must be 40");

    COFP13MPGroupStatsReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_GROUP)
    {
        ::memset(&m_groupstats, 0, sizeof(m_groupstats));
    }
    virtual ~COFP13MPGroupStatsReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void     Dump() const;

    COFP13MPGroupStatsReplyMsg::ofp_group_stats &GetGroupStatsReplyInfo();
    void SetGroupStatsReplyInfo(ofp_group_stats &groupstats);

    void AddBucketCounter(ofp_bucket_counter &counter);
    void GetBucketCounterList(std::list<ofp_bucket_counter> &list);
    void SetBucketCounterList(std::list<ofp_bucket_counter> &list);

    ACE_UINT32 GetBucketListSize() const;
    
private:
    ofp_group_stats m_groupstats;
    std::list<ofp_bucket_counter> m_bucket_list;
};

#endif

