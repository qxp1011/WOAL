/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPTableStatisticsMsg.h
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
#ifndef COFPMPTABLESTATISTICSMSG_H
#define COFPMPTABLESTATISTICSMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPTableStatsRequestMsg 
    : public COFP13MultipartRequestMsg
{
public:
    COFP13MPTableStatsRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_TABLE)
    {
    }
    virtual ~COFP13MPTableStatsRequestMsg(){}
};


class COFP13MPTableStatsReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Body of reply to OFPMP_TABLE request. */
    struct ofp_table_stats {
        ACE_CDR::Octet table_id; /* Identifier of table. Lower numbered tables are consulted first. */
        ACE_CDR::Octet pad[3]; /* Align to 32-bits. */
        ACE_UINT32 active_count; /* Number of active entries. */
        uint64_t lookup_count; /* Number of packets looked up in table. */
        uint64_t matched_count; /* Number of packets that hit table. */
    };

    static_assert((sizeof(struct ofp_table_stats) == 24), "must be 24");

    COFP13MPTableStatsReplyMsg():COFP13MultipartReplyMsg(OFPMP_TABLE)
    {
    }
    virtual ~COFP13MPTableStatsReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPTableStatsReplyMsg::ofp_table_stats &GetTableStatsInfo();
    void SetTableStatsInfo(
        COFP13MPTableStatsReplyMsg::ofp_table_stats &tabletats);

private:
    ofp_table_stats m_tablestats;
};

#endif

