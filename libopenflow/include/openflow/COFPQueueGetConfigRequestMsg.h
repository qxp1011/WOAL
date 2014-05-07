/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPQueueGetConfigRequestMsg.h
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
#ifndef COFPQUEUEGETCONFIGREQUESTMSG_H
#define COFPQUEUEGETCONFIGREQUESTMSG_H

#include "COFPMessage.h"

class COFP10QueueGetConfigRequestMsg : public COFPMessage
{
public:
    COFP10QueueGetConfigRequestMsg(ACE_UINT16 awPortNo = 0)
        : COFPMessage(OFP10::OFPT_QUEUE_GET_CONFIG_REQUEST, OFP10_VERSION)
        , m_wPortNo(awPortNo)
    {
    }
    virtual ~COFP10QueueGetConfigRequestMsg(){}

    ACE_UINT16 GetPortNo() const
    {
        return m_wPortNo;
    }
    void SetPortNo(ACE_UINT16 awPortNo)
    {
        m_wPortNo = awPortNo;
    }

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + 4;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

private:
    ACE_UINT16 m_wPortNo;
};



class COFP13QueueGetConfigRequestMsg : public COFPMessage
{
public:
    COFP13QueueGetConfigRequestMsg(ACE_UINT32 adwPortNo = 0);
    virtual ~COFP13QueueGetConfigRequestMsg(){}

    inline void SetPort(ACE_UINT32 adwPortNo);
    inline ACE_UINT32 GetPort();

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

private:
    ACE_UINT32 m_dwPort;
};

void COFP13QueueGetConfigRequestMsg::SetPort(ACE_UINT32 adwPortNo)
{
    m_dwPort = adwPortNo;
}

ACE_UINT32 COFP13QueueGetConfigRequestMsg::GetPort()
{
    return m_dwPort;
}

inline ACE_UINT16 COFP13QueueGetConfigRequestMsg::GetStreamLen() const
{
    return 8 + COFPMessage::GetStreamLen();
}

#endif

