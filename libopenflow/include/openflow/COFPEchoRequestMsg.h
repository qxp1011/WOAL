/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPEchoRequestMsg.h
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
#ifndef COFPECHOREQUESTMSG_H
#define COFPECHOREQUESTMSG_H

#include "COFPMessage.h"
#include "OFPMessageUtils.h"

class COFP10EchoRequestMsg : public COFPMessage
{
public:
    COFP10EchoRequestMsg(
        ACE_UINT16  awPayloadSize   = 0,
        ACE_CDR::Octet   *apPayload       = NULL,
        // args for OFP header
        ACE_UINT32  adwxid          = 0,
        ACE_CDR::Octet    abVersion       = OFP10_VERSION);
    virtual ~COFP10EchoRequestMsg();
    
    ACE_UINT32   GetPayload(const ACE_CDR::Octet *&payload, ACE_UINT16 &payload_size) const; 
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
    static std::allocator<ACE_CDR::Octet> m_alloc;
    ACE_CDR::Octet                       *m_payload;
    ACE_UINT16                      m_used_payload_size;   // the actual size of used memory
    ACE_UINT16                      m_payload_total_size;  // the total size of the allocated memory
};



class COFP13EchoRequestMsg : public COFP10EchoRequestMsg
{
public:
    COFP13EchoRequestMsg(ACE_UINT16  awPayloadSize   = 0,
                                     ACE_CDR::Octet   *apPayload       = NULL,
                                     // args for OFP header
                                     ACE_UINT32  adwxid          = 0,
                                     ACE_CDR::Octet    abVersion       = OFP13_VERSION)
        : COFP10EchoRequestMsg(awPayloadSize, apPayload, adwxid, abVersion)
    {
    }
    virtual ~COFP13EchoRequestMsg(){}
};

#endif

