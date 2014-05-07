/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPMeterStatisticsMsg.h
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
#ifndef COFPMPMETERSTATISTICSMSG_H
#define COFPMPMETERSTATISTICSMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPMeterStaticRequestMsg
    : public COFP13MultipartRequestMsg
{
public:
    /* Body of OFPMP_METER and OFPMP_METER_CONFIG requests. */
    struct ofp_meter_multipart_request {
        ACE_UINT32 meter_id; /* Meter instance, or OFPM_ALL. */
        ACE_CDR::Octet pad[4]; /* Align to 64 bits. */
    };
    
    static_assert((sizeof(struct ofp_meter_multipart_request) == 8), "must be 8");

    COFP13MPMeterStaticRequestMsg()
        :   COFP13MultipartRequestMsg(OFPMP_METER)
    {
        ::memset(&m_request, 0, sizeof(m_request));
    }
    virtual ~COFP13MPMeterStaticRequestMsg()    {}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPMeterStaticRequestMsg::ofp_meter_multipart_request &
    GetMeterStatsRequest();
    void SetMeterStatsRequest(ofp_meter_multipart_request &request);
    
private:
    ofp_meter_multipart_request m_request;
};


class COFP13MPMeterStaticReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Group configuration flags */
    enum ofp_group_capabilities {
        OFPGFC_SELECT_WEIGHT = 1 << 0, /* Support weight for select groups */
        OFPGFC_SELECT_LIVENESS = 1 << 1, /* Support liveness for select groups */
        OFPGFC_CHAINING = 1 << 2, /* Support chaining groups */
        OFPGFC_CHAINING_CHECKS = 1 << 3, /* Check chaining for loops and delete */
    };

    /* Statistics for each meter band */
    struct ofp_meter_band_stats {
        uint64_t packet_band_count; /* Number of packets in band. */
        uint64_t byte_band_count; /* Number of bytes in band. */
    };

    
    /* Body of reply to OFPMP_METER request. Meter statistics. */
    struct ofp_meter_stats {
        ACE_UINT32 meter_id; /* Meter instance. */
        ACE_UINT16 len; /* Length in bytes of this stats. */
        ACE_CDR::Octet pad[6];
        ACE_UINT32 flow_count; /* Number of flows bound to meter. */
        uint64_t packet_in_count; /* Number of packets in input. */
        uint64_t byte_in_count; /* Number of bytes in input. */
        ACE_UINT32 duration_sec; /* Time meter has been alive in seconds. */
        ACE_UINT32 duration_nsec; /* Time meter has been alive in nanoseconds beyond*/
        //struct ofp_meter_band_stats band_stats[0]; /* The band_stats length */
    };

    static_assert((sizeof(struct ofp_meter_band_stats) == 16), "must be 16");

    static_assert((sizeof(struct ofp_meter_stats) == 40), "must be 40");

    COFP13MPMeterStaticReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_METER)
    {
        ::memset(&m_meterstats, 0, sizeof(m_meterstats));
    }
    virtual ~COFP13MPMeterStaticReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPMeterStaticReplyMsg::ofp_meter_stats &GetMeterStats();
    void SetMeterStats(ofp_meter_stats &meterstats);


    void AddMeterBand(ofp_meter_band_stats &band);
    void GetMeterBandList(std::list<ofp_meter_band_stats> &list);
    void SetMeterBandList(std::list<ofp_meter_band_stats> &list);

private:
    ofp_meter_stats m_meterstats;
    std::list<ofp_meter_band_stats> m_meter_band_list;
};
#endif

