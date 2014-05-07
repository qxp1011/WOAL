/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPAggregateFlowStatisticsMsg.h
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
#ifndef COFPMPAGGREGATEFLOWSTATISTICSMSG_H
#define COFPMPAGGREGATEFLOWSTATISTICSMSG_H

#include "COFPMultipartMsg.h"
#include "COFPMatch.h"

class COFP13MPAggregateFlowStatsRequestMsg 
    : public COFP13MultipartRequestMsg
{
public:
    struct ofp_aggregate_stats_request {
        ACE_CDR::Octet table_id; /* ID of table to read (from ofp_table_stats)OFPTT_ALL for all tables. */
        ACE_CDR::Octet pad[3]; /* Align to 32 bits. */
        ACE_UINT32 out_port; /* Require matching entries to include this*/
        ACE_UINT32 out_group; /* Require matching entries to include this*/
        ACE_CDR::Octet pad2[4]; /* Align to 64 bits. */
        uint64_t cookie; /* Require matching entries to contain this cookie value */
        uint64_t cookie_mask; /* Mask used to restrict the cookie bits that must match. A value of 0 
                                                            indicates  no restriction. */
        //struct ofp_match match; /* Fields to match. Variable size. */
    };
    
    static_assert((sizeof(ofp_aggregate_stats_request) == 32),"must be 32");

    COFP13MPAggregateFlowStatsRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_AGGREGATE)
    {
        ::memset(&m_stats_request, 0,sizeof(m_stats_request));
    }
    virtual ~COFP13MPAggregateFlowStatsRequestMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

    COFP13MPAggregateFlowStatsRequestMsg::ofp_aggregate_stats_request &
    GetStatsRequestInfo();
    void SetStatsRequestInfo(
        COFP13MPAggregateFlowStatsRequestMsg::ofp_aggregate_stats_request &requestInfo);

    COFP13Match &GetProtocolMatch();
    void SetProtocolMatch(COFP13Match &match);
    
private:
    ofp_aggregate_stats_request m_stats_request;
    COFP13Match m_match;
};


class COFP13MPAggregateFlowStatsReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Body of reply to OFPMP_AGGREGATE request. */
    struct ofp_aggregate_stats_reply {
        uint64_t packet_count; /* Number of packets in flows. */
        uint64_t byte_count; /* Number of bytes in flows. */
        ACE_UINT32 flow_count; /* Number of flows. */
        ACE_CDR::Octet pad[4]; /* Align to 64 bits. */
    };
    static_assert((sizeof(struct ofp_aggregate_stats_reply) == 24), "must be 32");

    COFP13MPAggregateFlowStatsReplyMsg():COFP13MultipartReplyMsg(OFPMP_AGGREGATE)
    {
        ::memset(&m_flowstats, 0, sizeof(m_flowstats));
    }
    virtual ~COFP13MPAggregateFlowStatsReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

    COFP13MPAggregateFlowStatsReplyMsg::ofp_aggregate_stats_reply &
    GetFlowStatsInfo();
    void SetFlowStatsInfo(
        COFP13MPAggregateFlowStatsReplyMsg::ofp_aggregate_stats_reply &flowstats);

private:
    ofp_aggregate_stats_reply m_flowstats;
};

#endif

