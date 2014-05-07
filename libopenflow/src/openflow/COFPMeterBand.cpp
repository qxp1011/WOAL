/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMeterBand.cpp
 * Description:      OpenFlow meter band source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-15
 *
 * History 1:
 *     Date:         2013-12-21
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "ByteOrder.h"
#include "COFPMeterBand.h"
#include "CmError.h"

using namespace openflow;

COFP13MeterBand::
COFP13MeterBand(ACE_UINT16 awType, ACE_UINT16 awLen)
    : m_wBandType(awType)
    , m_wBandLen(awLen)
    , m_dwRate(0)
    , m_dwBurstSize(0)
{
}

CmResult COFP13MeterBand::
DecodeMeterBand(ACE_InputCDR &is, COFP13MeterBand * &aBand)
{
    CmResult   lRet      = CM_ERROR_FAILURE;
    ACE_UINT16 wBandType = 0;
    
    bool bGood = is.pre_read(&wBandType, sizeof(wBandType));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    wBandType = NetworkToHost16(wBandType);
    COFP13MeterBand *Band = nullptr;

    switch(wBandType)
    {
        case OFPMBT_DROP:
        {
            Band = new COFP13MeterBandDrop();
            break;
        }
        case OFPMBT_DSCP_REMARK:
        {
            Band = new COFP13MeterBandDscpRemark();
            break;
        }
        default:
        {
            break;
        }
    }

    CM_ASSERT_RETURN(Band != nullptr, CM_ERROR_OUT_OF_MEMORY);
    
    lRet = Band->StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("DecodeMeterBand::StreamFrom(): fail in StreamFrom(), lRet=%l\n"), lRet));
        return lRet;
    }

    aBand = Band;
    
    return CM_OK;
}

CmResult COFP13MeterBand::StreamFrom(ACE_InputCDR &is)
{
    is>>m_wBandType;
    is>>m_wBandLen;
    is>>m_dwRate;
    is>>m_dwBurstSize;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13MeterBand::StreamTo(ACE_OutputCDR &os)
{
    m_wBandLen = GetStreamLen();
    
    os<<m_wBandType;
    os<<m_wBandLen;
    os<<m_dwRate;
    os<<m_dwBurstSize;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13MeterBand::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MeterBand::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("type=%u, len=%u, rate=%u, burstSize=%u\n"),
               m_wBandType,
               m_wBandLen,
               m_dwRate,
               m_dwBurstSize));
}


COFP13MeterBandDrop::COFP13MeterBandDrop()
    : COFP13MeterBand(OFPMBT_DROP, 16)
{
}

CmResult COFP13MeterBandDrop::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 pad = 0;

    CmResult result = COFP13MeterBand::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    is>>pad;
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13MeterBandDrop::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 pad = 0;

    CmResult result = COFP13MeterBand::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    os<<pad;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13MeterBandDrop::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MeterBandDrop::Dump():\n")));
    COFP13MeterBand::Dump();
}

COFP13MeterBandDscpRemark::COFP13MeterBandDscpRemark(ACE_CDR::Octet abLevel)
    : COFP13MeterBand(OFPMBT_DSCP_REMARK, GetStreamLen())
    , m_bPrecLevel(abLevel)
{
}

CmResult COFP13MeterBandDscpRemark::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad = 0;
    ACE_UINT16 pad2 = 0;

    CmResult result = COFP13MeterBand::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    is>>m_bPrecLevel;
    is>>pad;
    is>>pad2;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13MeterBandDscpRemark::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad = 0;
    ACE_UINT16     pad2 = 0;
    
    CmResult result = COFP13MeterBand::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    os<<m_bPrecLevel;
    os<<pad;
    os<<pad2;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13MeterBandDscpRemark::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MeterBandDscpRemark::Dump()\n")));
    COFP13MeterBand::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("prec_level=%u\n"), m_bPrecLevel));
}


