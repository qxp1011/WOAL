/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMeterBand.h
 * Description:      Openflow meter band header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-15
 *
 * History 1:
 *     Date:         2013-12-18
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/
#ifndef COFP_METER_BAND_H
#define COFP_METER_BAND_H

#include "IOFPMsgParser.h"

class COFP13MeterBand : public IOFPMsgParser
{
public:
    /* Meter band types */
    enum ofp_meter_band_type
    {
        OFPMBT_DROP         = 1,     /* Drop packet. */
        OFPMBT_DSCP_REMARK  = 2,     /* Remark DSCP in the IP header. */
        OFPMBT_EXPERIMENTER = 0xFFFF /* Experimenter meter band. */
    };
    
public:
    COFP13MeterBand(ACE_UINT16 awType, ACE_UINT16 awLen);
    virtual ~COFP13MeterBand()  {};

    inline ACE_UINT16 GetBandType()                                 const;
    inline ACE_UINT16 GetBandLen()                                  const;
    inline ACE_UINT32 GetRate()                                     const;
    inline ACE_UINT32 GetBurstSize()                                const;
    inline void   SetBandType(ACE_UINT16 awType);
    inline void   SetRate(ACE_UINT32 adwRate);
    inline void   SetBurstSize(ACE_UINT32 adwBurstSize);

    static CmResult DecodeMeterBand(ACE_InputCDR &is, COFP13MeterBand * &aBand);
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    
protected:
    ACE_UINT16 m_wBandType;/* One of OFPMBT_*. */
    ACE_UINT16 m_wBandLen; /* Length in bytes of this band. */
    ACE_UINT32 m_dwRate;   /* Rate for this band. */
    ACE_UINT32 m_dwBurstSize;/* Size of bursts. */
};

ACE_UINT16 COFP13MeterBand::GetBandType() const
{
    return m_wBandType;
}

ACE_UINT16 COFP13MeterBand::GetBandLen() const
{
    return m_wBandLen;
}

ACE_UINT32 COFP13MeterBand::GetRate() const
{
    return m_dwRate;
}

ACE_UINT32 COFP13MeterBand::GetBurstSize() const
{
    return m_dwBurstSize;
}

void COFP13MeterBand::SetBandType(ACE_UINT16 awType)
{
    m_wBandType = awType;
}

void COFP13MeterBand::SetRate(ACE_UINT32 adwRate)
{
    m_dwRate = adwRate;
}

void COFP13MeterBand::SetBurstSize(ACE_UINT32 adwBurstSize)
{
    m_dwBurstSize = adwBurstSize;
}

inline ACE_UINT16 COFP13MeterBand::GetStreamLen()                   const
{
    return (sizeof(m_wBandType) + sizeof(m_wBandLen) + sizeof(m_dwRate) + sizeof(m_dwBurstSize));
}

inline ACE_UINT16 COFP13MeterBand::GetEncodeSpace()                 const
{
    return GetStreamLen();
}


class COFP13MeterBandDrop : public COFP13MeterBand
{
public:
    COFP13MeterBandDrop();
    virtual ~COFP13MeterBandDrop()  {};

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
};

inline ACE_UINT16 COFP13MeterBandDrop::GetStreamLen()               const
{
    return 16;
}

inline ACE_UINT16 COFP13MeterBandDrop::GetEncodeSpace()             const
{
    return GetStreamLen();
}


class COFP13MeterBandDscpRemark : public COFP13MeterBand
{
public:
    COFP13MeterBandDscpRemark(ACE_CDR::Octet abLevel = 0);
    virtual ~COFP13MeterBandDscpRemark()    {};

    inline ACE_CDR::Octet GetPrecLevel()                                  const;
    inline void SetPrecLevel(ACE_CDR::Octet abLevel);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    ACE_CDR::Octet m_bPrecLevel;
};

ACE_CDR::Octet COFP13MeterBandDscpRemark::GetPrecLevel() const
{
    return m_bPrecLevel;
}

void COFP13MeterBandDscpRemark::SetPrecLevel(ACE_CDR::Octet abLevel)
{
    m_bPrecLevel = abLevel;
}

inline ACE_UINT16 COFP13MeterBandDscpRemark::GetStreamLen() const
{
    return 16;
}

inline ACE_UINT16 COFP13MeterBandDscpRemark::GetEncodeSpace() const
{
    return GetStreamLen();
}


#endif

