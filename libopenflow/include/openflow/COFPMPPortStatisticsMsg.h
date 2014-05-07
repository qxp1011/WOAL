/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPPortStatisticsMsg.h
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
#ifndef COFPMPPORTSTATISTICSMSG_H
#define COFPMPPORTSTATISTICSMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPPortStatsRequestMsg 
    : public COFP13MultipartRequestMsg
{
public:
    /* Body for ofp_multipart_request of type OFPMP_PORT. */
    struct ofp_port_stats_request {
        ACE_UINT32 port_no; /* OFPMP_PORT message must request statistics*/
        ACE_CDR::Octet pad[4];
    };

    static_assert((sizeof(ofp_port_stats_request) == 8),"must be 8");

    COFP13MPPortStatsRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_PORT_STATS)
    {
        ::memset(&m_stats_request, 0,sizeof(m_stats_request));
    }
    virtual ~COFP13MPPortStatsRequestMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPPortStatsRequestMsg::ofp_port_stats_request &
    GetPortStatsRequestInfo();
    void SetPortStatsRequestInfo(ofp_port_stats_request &requestInfo);
    
private:
    ofp_port_stats_request m_stats_request;
};


class COFP13MPPortStatsReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    struct ofp_port_stats {
        ACE_UINT32 port_no;
        ACE_CDR::Octet pad[4]; /* Align to 64-bits. */
        uint64_t rx_packets; /* Number of received packets. */
        uint64_t tx_packets; /* Number of transmitted packets. */
        uint64_t rx_bytes; /* Number of received bytes. */
        uint64_t tx_bytes; /* Number of transmitted bytes. */
        uint64_t rx_dropped; /* Number of packets dropped by RX. */
        uint64_t tx_dropped; /* Number of packets dropped by TX. */
        uint64_t rx_errors; /* Number of receive errors. This is a super-set
                                                        of more specific receive errors and should be
                                                        greater than or equal to the sum of all
                                                        rx_*_err values. */
        uint64_t tx_errors; /* Number of transmit errors. This is a super-set
                                                        of more specific transmit errors and should be
                                                        greater than or equal to the sum of all
                                                        tx_*_err values (none currently defined.) */
        uint64_t rx_frame_err; /* Number of frame alignment errors. */
        uint64_t rx_over_err; /* Number of packets with RX overrun. */
        uint64_t rx_crc_err; /* Number of CRC errors. */
        uint64_t collisions; /* Number of collisions. */
        ACE_UINT32 duration_sec; /* Time port has been alive in seconds. */
        ACE_UINT32 duration_nsec; /* Time port has been alive in nanoseconds beyond
                                                            82 . 2012; The Open Networking Foundation
                                                            OpenFlow Switch Specification Version 1.3.1
                                                            duration_sec. */
    };

    static_assert((sizeof(struct ofp_port_stats) == 112), "must be 112");

    COFP13MPPortStatsReplyMsg():COFP13MultipartReplyMsg(OFPMP_PORT_STATS)
    {
        ::memset(&m_portstats, 0, sizeof(m_portstats));
    }
    virtual ~COFP13MPPortStatsReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;
    
    COFP13MPPortStatsReplyMsg::ofp_port_stats &GetPortStatsReplyInfo();
    void SetPortStatsReplyInfo(ofp_port_stats &portstats);

private:
    ofp_port_stats m_portstats;
};

#endif
