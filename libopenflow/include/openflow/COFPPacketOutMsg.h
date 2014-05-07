/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPacketOutMsg.h
 * Description:      Header file for openflow packet-out message.
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
#ifndef COFPPACKETOUTMSG_H
#define COFPPACKETOUTMSG_H

#include <list>
#include "COFPMessage.h"
#include "COFPAction.h"
#include "COFP10Action.h"
#include "ace/Message_Block.h"

class COFP10PacketOutMsg : public COFPMessage
{
public:
    COFP10PacketOutMsg(ACE_CDR::Octet abVersion = OFP10_VERSION)
        : COFPMessage(OFP10::OFPT_PACKET_OUT, abVersion)
    {
        ::memset(&m_tMember, 0, sizeof(m_tMember));
        m_wDataLen = 0;
        m_PacketData = NULL;
    }
    virtual ~COFP10PacketOutMsg();

    ACE_UINT32 GetBufferId() const
    {
        return m_tMember.buffer_id;
    }
    ACE_UINT16 GetInPort() const
    {
        return m_tMember.in_port;
    }
    ACE_UINT16 GetActionsLen() const
    {
        return m_tMember.actions_len;
    }

    void SetBufferId(ACE_UINT32 adwId)
    {
        m_tMember.buffer_id = adwId;
    }
    void SetInPort(ACE_UINT16 awPortNo)
    {
        m_tMember.in_port = awPortNo;
    }

    CmResult AddActionList(std::list<COFP10Action *> &list);
    CmResult AppendAction(COFP10Action *apAct);
    CmResult GetActionList(std::list<COFP10Action *> &list);
    
    const ACE_CDR::Octet * GetEtherFrame() const;
    ACE_UINT16 GetFrameLen(){return m_wDataLen;}
    ACE_Message_Block *GetPacketData() const;
    CmResult AppendPacketData(ACE_Message_Block &aData);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + sizeof(m_tMember) + m_tMember.actions_len + m_wDataLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
protected:
    struct member
    {
        ACE_UINT32 buffer_id;   /* ID assigned by datapath (-1 if none). */
        ACE_UINT16 in_port;     /* Packet's input port (OFPP_NONE if none). */
        ACE_UINT16 actions_len; /* Size of action array in bytes. */
    };

private:
    struct member m_tMember;
    ACE_UINT16 m_wDataLen;
    std::list<COFP10Action *> m_action_list;
    ACE_Message_Block *m_PacketData;    
};



class COFP13PacketOutMsg : public COFPMessage
{
public:
    COFP13PacketOutMsg(ACE_UINT32 abuffer_id = 0, ACE_UINT32 aInPort = 0, ACE_UINT32 aXid = 0);
    virtual ~COFP13PacketOutMsg();

    // for m_action_list
    CmResult AddActionList(std::list<COFP13Action *> &list);
    CmResult AppendAction(COFP13Action *action);
    CmResult GetActionList(std::list<COFP13Action *> &list);
    // for m_PacketData
    ACE_Message_Block *GetPacketData() const;
    CmResult AppendPacketData(ACE_Message_Block &data);
    
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
    ACE_UINT32 m_buffer_id; /* ID assigned by datapath (OFP_NO_BUFFER if none). */
    ACE_UINT32 m_in_port; /* Packet's input port or OFPP_CONTROLLER. */
    ACE_UINT16 m_actions_len; /* Size of action array in bytes. */
    ACE_CDR::Octet   m_pad[6];
    std::list<COFP13Action *> m_action_list;
    ACE_Message_Block *m_PacketData;
};

#endif

