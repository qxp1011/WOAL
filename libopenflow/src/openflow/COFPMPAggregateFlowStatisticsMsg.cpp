/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPAggregateFlowStatisticsMsg.cpp
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

#include "COFPMPAggregateFlowStatisticsMsg.h"
#include "CmError.h"

//COFP13MPAggregateFlowStatsRequestMsg
CmResult COFP13MPAggregateFlowStatsRequestMsg::
StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_stats_request.table_id;
    is.read_octet_array(m_stats_request.pad,sizeof(m_stats_request.pad));

    is>>m_stats_request.out_port;
    is>>m_stats_request.out_group;

    is.read_octet_array(m_stats_request.pad2,sizeof(m_stats_request.pad2));

    
    is>>m_stats_request.cookie;
    is>>m_stats_request.cookie_mask;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return m_match.StreamFrom(is);

}

CmResult COFP13MPAggregateFlowStatsRequestMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_stats_request.table_id;
    os.write_octet_array(m_stats_request.pad, sizeof(m_stats_request.pad));

    os<<m_stats_request.out_port;
    os<<m_stats_request.out_group;

    os.write_octet_array(m_stats_request.pad2, sizeof(m_stats_request.pad2));

    
    os<<m_stats_request.cookie;
    os<<m_stats_request.cookie_mask;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return m_match.StreamTo(os);

}

ACE_UINT16 COFP13MPAggregateFlowStatsRequestMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_stats_request)+m_match.GetEncodeSpace();
}

void COFP13MPAggregateFlowStatsRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPAggregateFlowStatsRequestMsg::ofp_aggregate_stats_request &
COFP13MPAggregateFlowStatsRequestMsg::GetStatsRequestInfo()
{
    return m_stats_request;
}

void COFP13MPAggregateFlowStatsRequestMsg::SetStatsRequestInfo(
ofp_aggregate_stats_request &requestInfo)
{
    ::memcpy(&m_stats_request, &requestInfo, sizeof(requestInfo));
}


COFP13Match &COFP13MPAggregateFlowStatsRequestMsg::GetProtocolMatch()
{
    return m_match;
}

void COFP13MPAggregateFlowStatsRequestMsg::
SetProtocolMatch(COFP13Match &match)
{
    m_match = match;
}


//COFP13MPAggregateFlowStatsReplyMsg

CmResult COFP13MPAggregateFlowStatsReplyMsg::
StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_flowstats.packet_count;
    is>>m_flowstats.byte_count;
    is>>m_flowstats.flow_count;

    is.read_octet_array(m_flowstats.pad,sizeof(m_flowstats.pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;

}

CmResult COFP13MPAggregateFlowStatsReplyMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_flowstats.packet_count;
    os<<m_flowstats.byte_count;
    os<<m_flowstats.flow_count;
    os.write_octet_array(m_flowstats.pad, sizeof(m_flowstats.pad));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;

}

ACE_UINT16 COFP13MPAggregateFlowStatsReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_flowstats);
}

void COFP13MPAggregateFlowStatsReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPAggregateFlowStatsReplyMsg::ofp_aggregate_stats_reply &
COFP13MPAggregateFlowStatsReplyMsg::GetFlowStatsInfo()
{
    return m_flowstats;
}

void COFP13MPAggregateFlowStatsReplyMsg::SetFlowStatsInfo(
ofp_aggregate_stats_reply &flowstats)
{
    ::memcpy(&m_flowstats, &flowstats, sizeof(flowstats));
}

