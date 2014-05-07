/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMeterModMsg.h
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
#ifndef COFPMETERMODMSG_H
#define COFPMETERMODMSG_H

#include "COFPMessage.h"
#include "COFPMeterBand.h"

class COFP13MeterModMsg : public COFPMessage
{
public:

    enum ofp_meter_mod_commd
    {
        OFPMC_ADD,
        OFPMC_MODIFY,
        OFPMC_DELETE,
    };

    enum ofp_meter_flags
    {
        OFPMF_KBPS = 1<<0,
        OFPMF_PKTPS = 1<<1,
        OFPMF_BURST = 1 <<2,
        OFPMF_STATS = 1<<3,
    };
    
    COFP13MeterModMsg();
    virtual ~COFP13MeterModMsg();

    inline void SetCommand(ACE_UINT16 awCmd);
    inline void SetFlag(ACE_UINT16 awFlag);
    inline void SetMeterId(ACE_UINT32 adwId);
    inline ACE_UINT16 GetCommand();
    inline ACE_UINT16 GetFlag();
    inline ACE_UINT32 GetMeterId();
    
    CmResult AddBandList(std::list<COFP13MeterBand *> &list);
    CmResult AppendBand(COFP13MeterBand *Band);
    CmResult GetBandList(std::list<COFP13MeterBand *> &list);
    
protected:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const;
    virtual void         Dump()              const;

private:
    ACE_UINT16 m_wStreamLen;
    ACE_UINT16 m_wCommand;
    ACE_UINT16 m_wFlag;
    ACE_UINT32 m_dwMeterId;
    std::list<COFP13MeterBand *> m_meterband_list;
};

void COFP13MeterModMsg::SetCommand(ACE_UINT16 awCmd)
{
    m_wCommand = awCmd;
}

void COFP13MeterModMsg::SetFlag(ACE_UINT16 awFlag)
{
    m_wFlag = awFlag;
}

void COFP13MeterModMsg::SetMeterId(ACE_UINT32 adwId)
{
    m_dwMeterId = adwId;
}

ACE_UINT16 COFP13MeterModMsg::GetCommand()
{
    return m_wCommand;
}

ACE_UINT16 COFP13MeterModMsg::GetFlag()
{
    return m_wFlag;
}

ACE_UINT32 COFP13MeterModMsg::GetMeterId()
{
    return m_dwMeterId;
}

inline ACE_UINT16 COFP13MeterModMsg::GetStreamLen() const
{
    return m_wStreamLen + COFPMessage::GetStreamLen();
}

inline ACE_UINT16 COFP13MeterModMsg::GetEncodeSpace() const
{
    return GetStreamLen();
}

#endif

