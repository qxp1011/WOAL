/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPQueueStatisticsMsg.h
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
#ifndef COFPMPQUEUESTATISTICSMSG_H
#define COFPMPQUEUESTATISTICSMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPQueueStatsRequestMsg 
    : public COFP13MultipartRequestMsg
{
public:
    struct ofp_queue_stats_request {
        ACE_UINT32 port_no; /* All ports if OFPP_ANY. */
        ACE_UINT32 queue_id; /* All queues if OFPQ_ALL. */
    };

    static_assert((sizeof(ofp_queue_stats_request) == 8),"must be 8");

    COFP13MPQueueStatsRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_QUEUE)
    {
        ::memset(&m_stats_request, 0,sizeof(m_stats_request));
    }
    virtual ~COFP13MPQueueStatsRequestMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPQueueStatsRequestMsg::ofp_queue_stats_request &
    GetQueueStatsRequestInfo();
    void SetQueueStatsRequestInfo(ofp_queue_stats_request &requestInfo);
    
private:
    ofp_queue_stats_request m_stats_request;
};


class COFP13MPQueueStatsReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    struct ofp_queue_stats {
        ACE_UINT32 port_no;
        ACE_UINT32 queue_id; /* Queue i.d */
        uint64_t tx_bytes; /* Number of transmitted bytes. */
        uint64_t tx_packets; /* Number of transmitted packets. */
        uint64_t tx_errors; /* Number of packets dropped due to overrun. */
        ACE_UINT32 duration_sec; /* Time queue has been alive in seconds. */
        ACE_UINT32 duration_nsec; /* Time queue has been alive in nanoseconds beyond duration_sec. */
    };

    static_assert((sizeof(struct ofp_queue_stats) == 40), "must be 40");

    COFP13MPQueueStatsReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_QUEUE)
    {
        ::memset(&m_queuestats, 0, sizeof(m_queuestats));
    }
    virtual ~COFP13MPQueueStatsReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPQueueStatsReplyMsg::ofp_queue_stats &GetQueueStatsReplyInfo();
    void SetQueueStatsReplyInfo(ofp_queue_stats &queuestats);

private:
    ofp_queue_stats m_queuestats;
};
#endif

