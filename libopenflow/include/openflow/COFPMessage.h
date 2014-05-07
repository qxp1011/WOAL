/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMessage.h
 * Description:      OpenFlow Message header file
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
#ifndef COFP_MESSAGE_H
#define COFP_MESSAGE_H

#include "openflow.h"
#include "IOFPMsgParser.h"
#include "ace/Message_Block.h"

namespace OFP10
{
enum ofp_type {
    /* Immutable messages. */
    OFPT_HELLO                      = 0, /* Symmetric message */
    OFPT_ERROR                      = 1, /* Symmetric message */
    OFPT_ECHO_REQUEST               = 2, /* Symmetric message */
    OFPT_ECHO_REPLY                 = 3, /* Symmetric message */
    OFPT_VENDOR                     = 4, /* Symmetric message */
    /* Switch configuration messages. */
    OFPT_FEATURES_REQUEST           = 5, /* Controller/switch message */
    OFPT_FEATURES_REPLY             = 6, /* Controller/switch message */
    OFPT_GET_CONFIG_REQUEST         = 7, /* Controller/switch message */
    OFPT_GET_CONFIG_REPLY           = 8, /* Controller/switch message */
    OFPT_SET_CONFIG                 = 9, /* Controller/switch message */
    /* Asynchronous messages. */
    OFPT_PACKET_IN                  = 10, /* Async message */
    OFPT_FLOW_REMOVED               = 11, /* Async message */
    OFPT_PORT_STATUS                = 12, /* Async message */
    /* Controller command messages. */
    OFPT_PACKET_OUT                 = 13, /* Controller/switch message */
    OFPT_FLOW_MOD                   = 14, /* Controller/switch message */
    OFPT_PORT_MOD                   = 15, /* Controller/switch message */
    /* Statistics messages. */
    OFPT_STATS_REQUEST              = 16, /* Controller/switch message */
    OFPT_STATS_REPLY                = 17, /* Controller/switch message */
    /* Barrier messages. */
    OFPT_BARRIER_REQUEST            = 18, /* Controller/switch message */
    OFPT_BARRIER_REPLY              = 19, /* Controller/switch message */
    /* Queue Configuration messages. */
    OFPT_QUEUE_GET_CONFIG_REQUEST   = 20, /* Controller/switch message */
    OFPT_QUEUE_GET_CONFIG_REPLY     = 21, /* Controller/switch message */
    OFPT_INVALID                    = 0xff
};    
}

namespace OFP13
{
enum ofp_type {
    /* Immutable messages. */
    OFPT_HELLO                    = 0, /* Symmetric message */
    OFPT_ERROR                    = 1, /* Symmetric message */
    OFPT_ECHO_REQUEST             = 2, /* Symmetric message */
    OFPT_ECHO_REPLY               = 3, /* Symmetric message */
    OFPT_EXPERIMENTER             = 4, /* Symmetric message */
    /* Switch configuration messages. */
    OFPT_FEATURES_REQUEST         = 5, /* Controller/switch message */
    OFPT_FEATURES_REPLY           = 6, /* Controller/switch message */
    OFPT_GET_CONFIG_REQUEST       = 7, /* Controller/switch message */
    OFPT_GET_CONFIG_REPLY         = 8, /* Controller/switch message */
    OFPT_SET_CONFIG               = 9, /* Controller/switch message */
    /* Asynchronous messages. */
    OFPT_PACKET_IN                = 10, /* Async message */
    OFPT_FLOW_REMOVED             = 11, /* Async message */
    OFPT_PORT_STATUS              = 12, /* Async message */
    /* Controller command messages. */
    OFPT_PACKET_OUT               = 13, /* Controller/switch message */
    OFPT_FLOW_MOD                 = 14, /* Controller/switch message */
    OFPT_GROUP_MOD                = 15, /* Controller/switch message */
    OFPT_PORT_MOD                 = 16, /* Controller/switch message */
    OFPT_TABLE_MOD                = 17, /* Controller/switch message */
    /* Multipart messages. */
    OFPT_MULTIPART_REQUEST        = 18, /* Controller/switch message */
    OFPT_MULTIPART_REPLY          = 19, /* Controller/switch message */
    /* Barrier messages. */
    OFPT_BARRIER_REQUEST          = 20, /* Controller/switch message */
    OFPT_BARRIER_REPLY            = 21, /* Controller/switch message */
    /* Queue Configuration messages. */
    OFPT_QUEUE_GET_CONFIG_REQUEST = 22, /* Controller/switch message */
    OFPT_QUEUE_GET_CONFIG_REPLY   = 23, /* Controller/switch message */
    /* Controller role change request messages. */
    OFPT_ROLE_REQUEST             = 24, /* Controller/switch message */
    OFPT_ROLE_REPLY               = 25, /* Controller/switch message */
    /* Asynchronous message configuration. */
    OFPT_GET_ASYNC_REQUEST        = 26, /* Controller/switch message */
    OFPT_GET_ASYNC_REPLY          = 27, /* Controller/switch message */
    OFPT_SET_ASYNC                = 28, /* Controller/switch message */
    /* Meters and rate limiters configuration messages. */
    OFPT_METER_MOD                = 29, /* Controller/switch message */
    OFPT_INVALID                  = 0xff
};
}

#if 0
typedef enum {    
    OFP10_VERSION = 0x01,    
    OFP11_VERSION = 0x02,    
    OFP12_VERSION = 0x03,    
    OFP13_VERSION = 0x04      // Version 1.3.1
} EOpenFlowVersion;
#endif

#define OFP_MESSAGE_HDR_SIZE (sizeof(struct ofp_header))

class COFPMessage : public IOFPMsgParser
{
public:
    COFPMessage(
        ACE_CDR::Octet    abType    = 0,
        ACE_CDR::Octet    abVersion = OFP13_VERSION,
        ACE_UINT32  adwxid    = 0,
        ACE_UINT16  awLength  = 0);    
    virtual ~COFPMessage() {}
    
    ACE_CDR::Octet   GetMessageVersion()                          const;
    ACE_CDR::Octet   GetMessageType()                             const;
    ACE_UINT16 GetMessageLength()                           const;
    ACE_UINT32 GetMessageXID()                              const;

    void SetMessageVersion(ACE_CDR::Octet abVer);
    void SetMessageType(ACE_CDR::Octet abType);
    void SetMessageLength(ACE_UINT16 awLength);
    void SetMessageXID(ACE_UINT32 anXid);

public:    
    static CmResult DecodeMessage(ACE_Message_Block &aMB, COFPMessage * &apOFPMsg);
    CmResult StreamFrom(ACE_Message_Block &aMB);
    // In the implementation, we specify ACE_CDR::BYTE_ORDER_BIG_ENDIAN when construct the ACE_OutputCDR object.
    CmResult StreamTo(ACE_Message_Block &aMB);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

protected:
    ACE_CDR::Octet    m_bVersion;
    ACE_CDR::Octet    m_bType;
    ACE_UINT16        m_wLength;
    ACE_UINT32        m_dwxid;

private:
    static CmResult OFP10DecodeMessage(ACE_CDR::Octet abType, ACE_InputCDR &aIs, COFPMessage * &apOFPMsg);
    static CmResult OFP13DecodeMessage(ACE_CDR::Octet abType, ACE_InputCDR &aIs, COFPMessage * &apOFPMsg);
};

inline ACE_CDR::Octet COFPMessage::GetMessageVersion()            const
{
    return m_bVersion;
}

inline ACE_CDR::Octet COFPMessage::GetMessageType()               const
{
    return m_bType;
}

inline ACE_UINT16 COFPMessage::GetMessageLength()           const
{
    return m_wLength;
}

inline ACE_UINT32 COFPMessage::GetMessageXID()              const
{
    return m_dwxid;
}

inline void COFPMessage::SetMessageVersion(ACE_CDR::Octet abVer)
{
    m_bVersion = abVer;
}

inline void COFPMessage::SetMessageType(ACE_CDR::Octet abType)
{
    m_bType = abType;
}

inline void COFPMessage::SetMessageLength(ACE_UINT16 awLength)
{
    m_wLength = awLength;
}

inline void COFPMessage::SetMessageXID(ACE_UINT32 anXid)
{
    m_dwxid = anXid;
}

#endif  //COFP_MSG_H


