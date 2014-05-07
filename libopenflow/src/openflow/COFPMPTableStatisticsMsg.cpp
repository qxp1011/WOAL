/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPTableStatisticsMsg.cpp
 * Description:      Source file for openflow multipart table statistics msg.
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

#include "COFPMPTableStatisticsMsg.h"
#include "CmError.h"

CmResult COFP13MPTableStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_tablestats.table_id;
    is.read_octet_array(m_tablestats.pad, sizeof(m_tablestats.pad));
    is>>m_tablestats.active_count;
    is>>m_tablestats.lookup_count;
    is>>m_tablestats.matched_count;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP13MPTableStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_tablestats.table_id;
    os.write_octet_array(m_tablestats.pad, sizeof(m_tablestats.pad));
    os<<m_tablestats.active_count;
    os<<m_tablestats.lookup_count;
    os<<m_tablestats.matched_count;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

ACE_UINT16 COFP13MPTableStatsReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_tablestats);
}

void COFP13MPTableStatsReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPTableStatsReplyMsg::ofp_table_stats &COFP13MPTableStatsReplyMsg::
GetTableStatsInfo()
{
    return m_tablestats;
}

void COFP13MPTableStatsReplyMsg::
SetTableStatsInfo(COFP13MPTableStatsReplyMsg::ofp_table_stats &tabletats)
{
    ::memcpy(&m_tablestats, &tabletats, sizeof(tabletats));
}

