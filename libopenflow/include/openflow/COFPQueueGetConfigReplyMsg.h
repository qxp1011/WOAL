/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPQueueGetConfigReplyMsg.h
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
#ifndef COFPQUEUEGETCONFIGREPLYMSG_H
#define COFPQUEUEGETCONFIGREPLYMSG_H

#include <list>
#include "COFPMessage.h"
#include "COFPPacketQueue.h"

class COFP10QueueGetConfigReplyMsg : public COFPMessage
{
public:
    COFP10QueueGetConfigReplyMsg(ACE_UINT16 awPortNo = 0)
        : COFPMessage(OFP10::OFPT_QUEUE_GET_CONFIG_REPLY, OFP10_VERSION)
        , m_wQueueLen(0)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
        m_tMember.port = awPortNo;
    }
    virtual ~COFP10QueueGetConfigReplyMsg();

    ACE_UINT16 GetPortNo() const
    {
        return m_tMember.port;
    }
    void SetPortNo(ACE_UINT16 awPortNo)
    {
        m_tMember.port = awPortNo;
    }

    CmResult AddQueueList(std::list<COFP10PacketQueue *> &list);
    CmResult AppendQueue(COFP10PacketQueue *pQueue);
    CmResult GetQueueList(std::list<COFP10PacketQueue *> &list);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + sizeof m_tMember + m_wQueueLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    struct member
    {
        ACE_UINT16 port;
        ACE_CDR::Octet pad[6];
    };
    
private:
    struct member m_tMember;
    ACE_UINT16 m_wQueueLen;
    std::list<COFP10PacketQueue *> m_queue_list;
};



class COFP13QueueGetConfigReplyMsg : public COFPMessage
{
public:
    COFP13QueueGetConfigReplyMsg(ACE_UINT32 adwPortNo = 0);
    virtual ~COFP13QueueGetConfigReplyMsg();

    inline void SetPort(ACE_UINT32 adwPortNo);
    inline ACE_UINT32 GetPort();

    CmResult AddQueueList(std::list<COFP13PacketQueue *> &list);
    CmResult AppendQueue(COFP13PacketQueue *pQueue);
    CmResult GetQueueList(std::list<COFP13PacketQueue *> &list);

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
    ACE_UINT16 m_wStreamLen;
    std::list<COFP13PacketQueue *> m_queue_list;
};

void COFP13QueueGetConfigReplyMsg::SetPort(ACE_UINT32 adwPortNo)
{
    m_dwPort = adwPortNo;
}

ACE_UINT32 COFP13QueueGetConfigReplyMsg::GetPort()
{
    return m_dwPort;
}

inline ACE_UINT16 COFP13QueueGetConfigReplyMsg::GetStreamLen() const
{
    return m_wStreamLen;
}

#endif

