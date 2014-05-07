/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPMeterStatisticsMsg.cpp
 * Description:      Source file for openflow multipart meter statistics msg.
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

#include "COFPMPMeterStatisticsMsg.h"
#include "CmError.h"

//COFP13MPMeterStaticRequestMsg
CmResult COFP13MPMeterStaticRequestMsg::StreamFrom(ACE_InputCDR &is)
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

CmResult COFP13MPMeterStaticRequestMsg::
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

ACE_UINT16 COFP13MPMeterStaticRequestMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_request);    
}

void COFP13MPMeterStaticRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPMeterStaticRequestMsg::ofp_meter_multipart_request &
COFP13MPMeterStaticRequestMsg::GetMeterStatsRequest()
{
    return m_request;
}

void COFP13MPMeterStaticRequestMsg::
SetMeterStatsRequest(ofp_meter_multipart_request &request)
{
    ::memcpy(&m_request, &request, sizeof(request));
}


//COFP13MPMeterStaticReplyMsg
CmResult COFP13MPMeterStaticReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_meterstats.meter_id;
    is>>m_meterstats.len;
    is.read_octet_array(m_meterstats.pad, sizeof(m_meterstats.pad));
    is>>m_meterstats.flow_count;
    is>>m_meterstats.packet_in_count;
    is>>m_meterstats.byte_in_count;
    is>>m_meterstats.duration_sec;
    is>>m_meterstats.duration_nsec;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    ACE_INT16 bucketsize = m_meterstats.len-sizeof(m_meterstats);
    ofp_meter_band_stats bandstats;
    while (bucketsize>0)
    {
        is>>bandstats.packet_band_count;
        is>>bandstats.byte_band_count;
        AddMeterBand(bandstats);
        bucketsize = bucketsize-sizeof(ofp_meter_band_stats);
        if (bucketsize < 0)
        {
            return CM_ERROR_FAILURE;
        }
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;

}

CmResult COFP13MPMeterStaticReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    ACE_UINT16 len = sizeof(m_meterstats)+m_meter_band_list.size()*16;

    os<<m_meterstats.meter_id;
    os<<len;
    os.write_octet_array(m_meterstats.pad, sizeof(m_meterstats.pad));
    os<<m_meterstats.flow_count;
    os<<m_meterstats.packet_in_count;
    os<<m_meterstats.byte_in_count;
    os<<m_meterstats.duration_sec;
    os<<m_meterstats.duration_nsec;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    for (auto& meterband : m_meter_band_list) 
    {
        os<<meterband.packet_band_count;
        os<<meterband.byte_band_count;
        bGood = os.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;
}

ACE_UINT16 COFP13MPMeterStaticReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_meterstats)+m_meter_band_list.size()*16;    
}

void COFP13MPMeterStaticReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPMeterStaticReplyMsg::ofp_meter_stats &COFP13MPMeterStaticReplyMsg::
GetMeterStats()
{
    return m_meterstats;
}

void COFP13MPMeterStaticReplyMsg::SetMeterStats(ofp_meter_stats &meterstats)
{
    ::memcpy(&m_meterstats, &meterstats, sizeof(meterstats));
}


void COFP13MPMeterStaticReplyMsg::AddMeterBand(ofp_meter_band_stats &band)
{
    m_meter_band_list.push_back(band);
}

void COFP13MPMeterStaticReplyMsg::
GetMeterBandList(std::list<ofp_meter_band_stats> &list)
{
    while (!m_meter_band_list.empty())
    {
        ofp_meter_band_stats &item = m_meter_band_list.front();
        m_meter_band_list.pop_front();
        list.push_back(item);
    }
}

void COFP13MPMeterStaticReplyMsg::
SetMeterBandList(std::list<ofp_meter_band_stats> &list)
{
    while (!list.empty())
    {
        ofp_meter_band_stats &item = list.front();
        list.pop_front();
        m_meter_band_list.push_back(item);
    }
}

