/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPMeterFeaturesStatisticsMsg.cpp
 * Description:      
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

#include "COFPMPMeterFeaturesStatisticsMsg.h"
#include "CmError.h"

//COFP13MPMeterFeaturesStaticReplyMsg
CmResult COFP13MPMeterFeaturesStaticReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_meterfeatures.max_meter;
    is>>m_meterfeatures.band_types;
    is>>m_meterfeatures.capabilities;
    is>>m_meterfeatures.max_bands;
    is>>m_meterfeatures.max_color;
    is.read_octet_array(m_meterfeatures.pad, sizeof(m_meterfeatures.pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP13MPMeterFeaturesStaticReplyMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_meterfeatures.max_meter;
    os<<m_meterfeatures.band_types;
    os<<m_meterfeatures.capabilities;
    os<<m_meterfeatures.max_bands;
    os<<m_meterfeatures.max_color;
    os.write_octet_array(m_meterfeatures.pad, sizeof(m_meterfeatures.pad));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

ACE_UINT16 COFP13MPMeterFeaturesStaticReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_meterfeatures);
}

void COFP13MPMeterFeaturesStaticReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPMeterFeaturesStaticReplyMsg::ofp_meter_features &
COFP13MPMeterFeaturesStaticReplyMsg::GetMeterFeatures()
{
    return m_meterfeatures;
}

void COFP13MPMeterFeaturesStaticReplyMsg::
SetMeterFeatures(ofp_meter_features &meterfeatures)
{
    ::memcpy(&m_meterfeatures, &meterfeatures, sizeof(meterfeatures));
}

