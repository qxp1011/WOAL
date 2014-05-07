/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPMeterConfigStatisticsMsg.h
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
#ifndef COFPMPMETERCONFIGSTATISTICSMSG_H
#define COFPMPMETERCONFIGSTATISTICSMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPMeterConfigStaticRequestMsg
    : public COFP13MultipartRequestMsg
{
public:
    /* Body of OFPMP_METER and OFPMP_METER_CONFIG requests. */
    struct ofp_meter_multipart_request {
        ACE_UINT32 meter_id; /* Meter instance, or OFPM_ALL. */
        ACE_CDR::Octet pad[4]; /* Align to 64 bits. */
    };

    static_assert((sizeof(struct ofp_meter_multipart_request) == 8), "must be 8");

    COFP13MPMeterConfigStaticRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_METER_CONFIG)
    {
        ::memset(&m_request, 0, sizeof(m_request));
    }
    virtual ~COFP13MPMeterConfigStaticRequestMsg()  {}
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()          const;
    virtual ACE_UINT16   GetEncodeSpace()        const
    {
        return GetStreamLen();
    }
    virtual void         Dump()                  const;

    COFP13MPMeterConfigStaticRequestMsg::ofp_meter_multipart_request &
    GetMeterStatsRequest();
    void SetMeterStatsRequest(ofp_meter_multipart_request &request);
    
private:
    ofp_meter_multipart_request m_request;
};


class COFP13MPMeterConfigStaticReplyMsg
    : public COFP13MultipartReplyMsg
{
public:

    /* Common header for all meter bands */
    struct ofp_meter_band_header {
        ACE_UINT16 type; /* One of OFPMBT_*. */
        ACE_UINT16 len; /* Length in bytes of this band. */
        ACE_UINT32 rate; /* Rate for this band. */
        ACE_UINT32 burst_size; /* Size of bursts. */
    };
    
    /* Body of reply to OFPMP_METER_CONFIG request. Meter configuration. */
    struct ofp_meter_config {
        ACE_UINT16 length; /* Length of this entry. */
        ACE_UINT16 flags; /* All OFPMC_* that apply. */
        ACE_UINT32 meter_id; /* Meter instance. */
        //struct ofp_meter_band_header bands[0]; /* The bands length is*/
    };

    static_assert((sizeof(struct ofp_meter_band_header) == 12), "must be 12");

    static_assert((sizeof(struct ofp_meter_config) == 8), "must be 8");

    COFP13MPMeterConfigStaticReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_METER_CONFIG)
    {
        ::memset(&m_meter_config_stats, 0, sizeof(m_meter_config_stats));
    }
    virtual ~COFP13MPMeterConfigStaticReplyMsg() {}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPMeterConfigStaticReplyMsg::ofp_meter_config &GetMeterStats();
    void SetMeterConfigStats(ofp_meter_config &meterstats);


    void AddMeterBandHeader(ofp_meter_band_header &band);
    void GetMeterBandHeaderList(std::list<ofp_meter_band_header> &list);
    void SetMeterBandHeaderList(std::list<ofp_meter_band_header> &list);

private:
    ofp_meter_config m_meter_config_stats;
    std::list<ofp_meter_band_header> m_meter_band_list;
};

#endif

