/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPMeterConfigStatisticsMsg.cpp
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

#include "COFPMPMeterConfigStatisticsMsg.h"
#include "CmError.h"

//COFP13MPMeterConfigStaticRequestMsg

CmResult COFP13MPMeterConfigStaticRequestMsg::
StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_request.meter_id;
    is.read_octet_array(m_request.pad, sizeof(m_request.pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13MPMeterConfigStaticRequestMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_request.meter_id;
    os.write_octet_array(m_request.pad, sizeof(m_request.pad));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

ACE_UINT16 COFP13MPMeterConfigStaticRequestMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_request);
}

void COFP13MPMeterConfigStaticRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPMeterConfigStaticRequestMsg::ofp_meter_multipart_request &
COFP13MPMeterConfigStaticRequestMsg::GetMeterStatsRequest()
{
    return m_request;
}

void COFP13MPMeterConfigStaticRequestMsg::
SetMeterStatsRequest(ofp_meter_multipart_request &request)
{
    ::memcpy(&m_request, &request, sizeof(request));
}

//COFP13MPMeterConfigStaticReplyMsg
CmResult COFP13MPMeterConfigStaticReplyMsg::
StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_meter_config_stats.length;
    is>>m_meter_config_stats.flags;
    is>>m_meter_config_stats.meter_id;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);


    ACE_INT16 bandsize = m_meter_config_stats.length-sizeof(m_meter_config_stats);
    ofp_meter_band_header bandheader;
    while (bandsize>0)
    {
        is>>bandheader.type;
        is>>bandheader.len;
        is>>bandheader.rate;
        is>>bandheader.burst_size;
        AddMeterBandHeader(bandheader);
        bandsize = bandsize-sizeof(ofp_meter_band_header);
        if (bandsize < 0)
        {
            return CM_ERROR_FAILURE;
        }
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;

}

CmResult COFP13MPMeterConfigStaticReplyMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    ACE_UINT16 len = sizeof(m_meter_config_stats)+m_meter_band_list.size()*12;

    os<<len;
    os<<m_meter_config_stats.flags;
    os<<m_meter_config_stats.meter_id;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    for (auto& meterbandheader : m_meter_band_list) 
    {
        os<<meterbandheader.type;
        os<<meterbandheader.len;
        os<<meterbandheader.rate;
        os<<meterbandheader.burst_size;
        bGood = os.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;
}

ACE_UINT16 COFP13MPMeterConfigStaticReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_meter_config_stats)+m_meter_band_list.size()*12;
}

void COFP13MPMeterConfigStaticReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPMeterConfigStaticReplyMsg::ofp_meter_config &
COFP13MPMeterConfigStaticReplyMsg::GetMeterStats()
{
    return m_meter_config_stats;
}

void COFP13MPMeterConfigStaticReplyMsg::
SetMeterConfigStats(ofp_meter_config &meterstats)
{
    ::memcpy(&m_meter_config_stats, &meterstats, sizeof(meterstats));
}


void COFP13MPMeterConfigStaticReplyMsg::
AddMeterBandHeader(ofp_meter_band_header &band)
{
    m_meter_band_list.push_back(band);
}

void COFP13MPMeterConfigStaticReplyMsg::
GetMeterBandHeaderList(std::list<ofp_meter_band_header> &list)
{
    while (!m_meter_band_list.empty())
    {
        ofp_meter_band_header &item = m_meter_band_list.front();
        m_meter_band_list.pop_front();
        list.push_back(item);
    }
}

void COFP13MPMeterConfigStaticReplyMsg::
SetMeterBandHeaderList(std::list<ofp_meter_band_header> &list)
{
    while (!list.empty())
    {
        ofp_meter_band_header &item = list.front();
        list.pop_front();
        m_meter_band_list.push_back(item);
    }
}

