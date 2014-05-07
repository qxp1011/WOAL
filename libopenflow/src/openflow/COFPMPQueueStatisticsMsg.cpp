/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPQueueStatisticsMsg.cpp
 * Description:      Source file for openflow multipart queue statistics msg.
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

#include "COFPMPQueueStatisticsMsg.h"
#include "CmError.h"

//COFP13MPQueueStatsRequestMsg
CmResult COFP13MPQueueStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_stats_request.port_no;
    is>>m_stats_request.queue_id;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;  
}

CmResult COFP13MPQueueStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_stats_request.port_no;
    os<<m_stats_request.queue_id;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK; 
}

ACE_UINT16 COFP13MPQueueStatsRequestMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_stats_request);
}

void COFP13MPQueueStatsRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPQueueStatsRequestMsg::ofp_queue_stats_request &
COFP13MPQueueStatsRequestMsg::GetQueueStatsRequestInfo()
{
    return m_stats_request;
}

void COFP13MPQueueStatsRequestMsg::
SetQueueStatsRequestInfo(ofp_queue_stats_request &requestInfo)
{
    ::memcpy(&m_stats_request, &requestInfo, sizeof(requestInfo));
}


//COFP13MPQueueStatsReplyMsg
CmResult COFP13MPQueueStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_queuestats.port_no;
    is>>m_queuestats.queue_id;

    is>>m_queuestats.tx_bytes;
    is>>m_queuestats.tx_packets;
    is>>m_queuestats.tx_errors;
    is>>m_queuestats.duration_sec;
    is>>m_queuestats.duration_nsec;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP13MPQueueStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_queuestats.port_no;
    os<<m_queuestats.queue_id;

    os<<m_queuestats.tx_bytes;
    os<<m_queuestats.tx_packets;
    os<<m_queuestats.tx_errors;
    os<<m_queuestats.duration_sec;
    os<<m_queuestats.duration_nsec;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK; 
}

ACE_UINT16 COFP13MPQueueStatsReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_queuestats);
}

void COFP13MPQueueStatsReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPQueueStatsReplyMsg::ofp_queue_stats &COFP13MPQueueStatsReplyMsg::
GetQueueStatsReplyInfo()
{
    return m_queuestats;
}

void COFP13MPQueueStatsReplyMsg::
SetQueueStatsReplyInfo(ofp_queue_stats &queuestats)
{
    ::memcpy(&m_queuestats, &queuestats, sizeof(queuestats));
}

