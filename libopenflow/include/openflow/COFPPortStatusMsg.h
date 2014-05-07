/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPortStatusMsg.h
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
#ifndef COFPPORTSTATUSMSG_H
#define COFPPORTSTATUSMSG_H

#include "COFPMessage.h"
#include "COFPPort.h"

/* What changed about the physical port */
enum ofp_port_reason {
    OFPPR_ADD    = 0, /* The port was added. */
    OFPPR_DELETE = 1, /* The port was removed. */
    OFPPR_MODIFY = 2, /* Some attribute of the port has changed. */
};

class COFP10PortStatusMsg : public COFPMessage
{
public:
    COFP10PortStatusMsg(ACE_CDR::Octet abVersion = OFP10_VERSION, ACE_CDR::Octet abReason = 0);
    virtual ~COFP10PortStatusMsg(){};

    ACE_CDR::Octet GetReason() const
    {
        return m_reason;
    }

    COFP10PhyPort *GetPort()
    {
        return &m_tDesc;
    }

    const COFP10PhyPort &GetPortRef() const
    {
        return m_tDesc;
    }

    void SetReason(ACE_CDR::Octet aReason)
    {
        m_reason = aReason;
    }

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return (m_tDesc.GetStreamLen() + 8 + COFPMessage::GetStreamLen());
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
private:    
    ACE_CDR::Octet          m_reason;   /* One of OFPPR_*. */
    COFP10PhyPort m_tDesc;
};


/* A physical port has changed in the datapath */
class COFP13PortStatusMsg : public COFPMessage
{
public:
    COFP13PortStatusMsg();
    virtual ~COFP13PortStatusMsg(){};

    inline ACE_CDR::Octet GetReason() const
    {
        return m_reason;
    }
    COFP13Port *GetPort()
    {
        return &m_tDesc;
    }

    const COFP13Port &GetPortRef() const
    {
        return m_tDesc;
    }

    inline void SetReason(ACE_CDR::Octet aReason)
    {
        m_reason = aReason;
    }

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return (m_tDesc.GetStreamLen() + 8 + COFPMessage::GetStreamLen());
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
private:
    ACE_CDR::Octet       m_reason;   /* One of OFPPR_*. */
    COFP13Port m_tDesc;
};
#endif

