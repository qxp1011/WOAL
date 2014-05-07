/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPEchoReplyMsg.h
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
#ifndef COFPECHOREPLYMSG_H
#define COFPECHOREPLYMSG_H

#include "COFPMessage.h"
#include "OFPMessageUtils.h"
#include "COFPEchoRequestMsg.h"


class COFP10EchoReplyMsg : public COFPMessage
{
public:
    COFP10EchoReplyMsg(ACE_CDR::Octet abVersion = OFP10_VERSION);
    COFP10EchoReplyMsg(COFP10EchoRequestMsg &aRequest);
    virtual ~COFP10EchoReplyMsg();

    CmResult GetPayload(const ACE_CDR::Octet *&payload, ACE_UINT16 &payload_size) const; 
    CmResult SetPayload(const ACE_CDR::Octet *payload, ACE_UINT16 payload_size);
    friend CmResult COFPMsgMemoryMgr::Copy(
        std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize,
                const ACE_CDR::Octet *apSrc, ACE_UINT16 awSrcSize);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return m_used_payload_size + COFPMessage::GetStreamLen();
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
private:
    static std::allocator<ACE_CDR::Octet> m_alloc;  // Handler for allocating deallocating memory
    ACE_CDR::Octet  *m_payload;
    ACE_UINT16 m_used_payload_size;
    ACE_UINT16 m_total_payload_size;
};



class COFP13EchoReplyMsg : public COFP10EchoReplyMsg
{
public:
    COFP13EchoReplyMsg(ACE_CDR::Octet abVersion = OFP13_VERSION)
        : COFP10EchoReplyMsg(abVersion)
    {
    }    
    COFP13EchoReplyMsg(COFP13EchoRequestMsg &aRequest)
        : COFP10EchoReplyMsg(aRequest)
    {
    }
    virtual ~COFP13EchoReplyMsg(){}
};

#endif

