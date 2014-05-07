/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMeterModMsg.cpp
 * Description:      Soure file for openflow meter modification msg.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-18
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com 
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPMeterModMsg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP13MeterModMsg::COFP13MeterModMsg()
    : COFPMessage(OFP13::OFPT_METER_MOD)
    , m_wStreamLen(8)
    , m_wCommand(0)
    , m_wFlag(0)
    , m_dwMeterId(0)    
{
}

COFP13MeterModMsg::~COFP13MeterModMsg()
{
    for (auto &item : m_meterband_list)
    {
        delete item;
    }
}

CmResult COFP13MeterModMsg::
AddBandList(std::list<COFP13MeterBand *> &list)
{
    while (!list.empty())
    {
        COFP13MeterBand *Band = list.front();
        CM_ASSERT_RETURN(Band != nullptr, CM_ERROR_NULL_POINTER);
        m_wStreamLen += Band->GetStreamLen();
        list.pop_front();
        m_meterband_list.push_back(Band);
    }

    return CM_OK;
}

CmResult COFP13MeterModMsg::
AppendBand(COFP13MeterBand *Band)
{
    CM_ASSERT_RETURN(Band != nullptr, CM_ERROR_NULL_POINTER);
    m_wStreamLen += Band->GetStreamLen();
    m_meterband_list.push_back(Band);
    
    return CM_OK;
}

CmResult COFP13MeterModMsg::
GetBandList(std::list<COFP13MeterBand *> &list)
{
    while (!m_meterband_list.empty())
    {
        COFP13MeterBand *Band = m_meterband_list.front();
        CM_ASSERT_RETURN(Band != nullptr, CM_ERROR_NULL_POINTER);
        m_meterband_list.pop_front();
        m_wStreamLen -= Band->GetStreamLen();        
        list.push_back(Band);
    }

    return CM_OK;
}

CmResult COFP13MeterModMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT16 wListLen = 0;
    ACE_UINT16 wBandLen = 0;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13MeterModMsg::StreamFrom, COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_wCommand;
    is>>m_wFlag;
    is>>m_dwMeterId;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    m_wStreamLen = m_wLength - COFPMessage::GetStreamLen();
    wListLen = m_wStreamLen - 8;    
    if (wListLen > 0)
    {
        COFP13MeterBand *Band = nullptr;
        lRet = COFP13MeterBand::DecodeMeterBand(is, Band);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        CM_ASSERT_RETURN(Band, CM_ERROR_FAILURE);
        wBandLen = Band->GetBandLen();
        if (wListLen < wBandLen)
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
        }
        m_meterband_list.push_back(Band);
        wListLen -= wBandLen;
    }

    return CM_OK;
}

CmResult COFP13MeterModMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13MeterModMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_wCommand;
    os<<m_wFlag;
    os<<m_dwMeterId;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    std::list<COFP13MeterBand *>::const_iterator it = m_meterband_list.begin();
    while (it != m_meterband_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}


void COFP13MeterModMsg::Dump()const
{
    COFPMessage::Dump();
}

