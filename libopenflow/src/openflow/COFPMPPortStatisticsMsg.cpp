/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPPortStatisticsMsg.cpp
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

#include "COFPMPPortStatisticsMsg.h"
#include "CmError.h"

//COFP13MPPortStatsRequestMsg

CmResult COFP13MPPortStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_stats_request.port_no;
    is.read_octet_array(m_stats_request.pad, sizeof(m_stats_request.pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP13MPPortStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_stats_request.port_no;
    os.write_octet_array(m_stats_request.pad, sizeof(m_stats_request.pad));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

ACE_UINT16 COFP13MPPortStatsRequestMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_stats_request);
}

void COFP13MPPortStatsRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPPortStatsRequestMsg::ofp_port_stats_request &
COFP13MPPortStatsRequestMsg::GetPortStatsRequestInfo()
{
    return m_stats_request;
}

void COFP13MPPortStatsRequestMsg::
SetPortStatsRequestInfo(ofp_port_stats_request &requestInfo)
{
    ::memcpy(&m_stats_request, &requestInfo, sizeof(requestInfo));
}

//COFP13MPPortStatsReplyMsg
CmResult COFP13MPPortStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_portstats.port_no;
    is.read_octet_array(m_portstats.pad, sizeof(m_portstats.pad));
    is>>m_portstats.rx_packets;
    is>>m_portstats.tx_packets;
    is>>m_portstats.rx_bytes;
    is>>m_portstats.tx_bytes;
    is>>m_portstats.rx_dropped;
    is>>m_portstats.tx_dropped;

    is>>m_portstats.rx_errors;
    is>>m_portstats.tx_errors;

    is>>m_portstats.rx_frame_err;
    is>>m_portstats.rx_over_err;

    is>>m_portstats.rx_crc_err;
    is>>m_portstats.collisions;
    is>>m_portstats.duration_nsec;


    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP13MPPortStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_portstats.port_no;
    os.write_octet_array(m_portstats.pad, sizeof(m_portstats.pad));
    os<<m_portstats.rx_packets;
    os<<m_portstats.tx_packets;
    os<<m_portstats.rx_bytes;
    os<<m_portstats.tx_bytes;
    os<<m_portstats.rx_dropped;
    os<<m_portstats.tx_dropped;

    os<<m_portstats.rx_errors;
    os<<m_portstats.tx_errors;

    os<<m_portstats.rx_frame_err;
    os<<m_portstats.rx_over_err;

    os<<m_portstats.rx_crc_err;
    os<<m_portstats.collisions;
    os<<m_portstats.duration_nsec;


    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

ACE_UINT16 COFP13MPPortStatsReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_portstats);
}

void COFP13MPPortStatsReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPPortStatsReplyMsg::ofp_port_stats &COFP13MPPortStatsReplyMsg::
GetPortStatsReplyInfo()
{
    return m_portstats;
}

void COFP13MPPortStatsReplyMsg::
SetPortStatsReplyInfo(ofp_port_stats &portstats)
{
    ::memcpy(&m_portstats, &portstats, sizeof(portstats));
}

