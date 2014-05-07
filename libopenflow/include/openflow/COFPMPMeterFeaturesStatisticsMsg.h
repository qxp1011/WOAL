/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPMeterFeaturesStatisticsMsg.h
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
#ifndef COFPMPMETERFEATURESSTATISTICSMSG_H
#define COFPMPMETERFEATURESSTATISTICSMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPMeterFeaturesStaticRequestMsg 
    : public COFP13MultipartRequestMsg
{
public:
    COFP13MPMeterFeaturesStaticRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_METER_FEATURES)
    {
    }
    virtual ~COFP13MPMeterFeaturesStaticRequestMsg(){}
};


class COFP13MPMeterFeaturesStaticReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Body of reply to OFPMP_METER_FEATURES request. Meter features. */
    struct ofp_meter_features {
        ACE_UINT32 max_meter; /* Maximum number of meters. */
        ACE_UINT32 band_types; /* Bitmaps of OFPMBT_* values supported. */
        ACE_UINT32 capabilities; /* Bitmaps of "ofp_meter_flags". */
        ACE_CDR::Octet max_bands; /* Maximum bands per meters */
        ACE_CDR::Octet max_color; /* Maximum color value */
        ACE_CDR::Octet pad[2];
    };

    static_assert((sizeof(struct ofp_meter_features) == 16), "must be 40");

    COFP13MPMeterFeaturesStaticReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_METER_FEATURES)
    {
    }
    virtual ~COFP13MPMeterFeaturesStaticReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPMeterFeaturesStaticReplyMsg::ofp_meter_features &GetMeterFeatures();
    void SetMeterFeatures(ofp_meter_features &meterfeatures);

private:
    ofp_meter_features m_meterfeatures;
};

#endif

