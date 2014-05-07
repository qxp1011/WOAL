/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMultipartMsg.h
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
#ifndef COFPMULTIPARTMSG_H
#define COFPMULTIPARTMSG_H

#include "COFPMessage.h"

enum ofp_multipart_types {
    /* Description of this OpenFlow switch.
             * The request body is empty.
            * The reply body is struct ofp_desc. */
    OFPMP_DESC = 0,
    /* Individual flow statistics.
            * The request body is struct ofp_flow_stats_request.
            * The reply body is an array of struct ofp_flow_stats. */
    OFPMP_FLOW = 1,
    /* Aggregate flow statistics.
            * The request body is struct ofp_aggregate_stats_request.
            * The reply body is struct ofp_aggregate_stats_reply. */
    OFPMP_AGGREGATE = 2,
    /* Flow table statistics.
            * The request body is empty.
            * The reply body is an array of struct ofp_table_stats. */
    OFPMP_TABLE = 3,
    /* Port statistics.
            * The request body is struct ofp_port_stats_request.
            * The reply body is an array of struct ofp_port_stats. */
    OFPMP_PORT_STATS = 4,
    /* Queue statistics for a port
            * The request body is struct ofp_queue_stats_request.
            * The reply body is an array of struct ofp_queue_stats */
    OFPMP_QUEUE = 5,
    /* Group counter statistics.
            * The request body is struct ofp_group_stats_request.
            * The reply is an array of struct ofp_group_stats. */
    OFPMP_GROUP = 6,
    /* Group description.
            * The request body is empty.
            * The reply body is an array of struct ofp_group_desc_stats. */
    OFPMP_GROUP_DESC = 7,
    /* Group features.
            * The request body is empty.
            * The reply body is struct ofp_group_features. */
    OFPMP_GROUP_FEATURES = 8,
    /* Meter statistics.
            * The request body is struct ofp_meter_multipart_requests.
            * The reply body is an array of struct ofp_meter_stats. */
    OFPMP_METER = 9,
    /* Meter configuration.
            * The request body is struct ofp_meter_multipart_requests.
            * The reply body is an array of struct ofp_meter_config. */
    OFPMP_METER_CONFIG = 10,
    /* Meter features. 73 . 2012; The Open Networking FoundationOpenFlow Switch Specication Version 1.3.1
            * The request body is empty.
            * The reply body is struct ofp_meter_features. */
    OFPMP_METER_FEATURES = 11,
    /* Table features.
                * The request body is either empty or contains an array of
                * struct ofp_table_features containing the controller's
                * desired view of the switch. If the switch is unable to
                * set the specified view an error is returned.
                * The reply body is an array of struct ofp_table_features. */
    OFPMP_TABLE_FEATURES = 12,
    /* Port description.
            * The request body is empty.
            * The reply body is an array of struct ofp_port. */
    OFPMP_PORT_DESC = 13,
    /* Experimenter extension.
            * The request and reply bodies begin with
            * struct ofp_experimenter_multipart_header.
            * The request and reply bodies are otherwise experimenter-defined. */
    OFPMP_EXPERIMENTER = 0xffff
};

class COFP13MultipartMsg : public COFPMessage
{
public:
    COFP13MultipartMsg(ACE_UINT32 msg_type, ACE_UINT16 atype, ACE_UINT16 aflags = 0)
        :COFPMessage(msg_type)
        ,m_type(atype)
        ,m_flags(aflags)
    {
    }
    virtual ~COFP13MultipartMsg(){}  
    //static CmResult DecodeRequestMessage(CCmMessageBlock& aMB, COpenFlowMultipartMessagesRequest*&pOpenFlowMsg);
    //static CmResult DecodeReplyMessage(CCmMessageBlock& aMB, COpenFlowMultipartMessagesReply*&pOpenFlowMsg);

    ACE_UINT16 GetType()  { return m_type; }
    ACE_UINT16 GetFlags() { return m_flags; }
    
    static CmResult DecodeMPMessage(ACE_InputCDR &is, COFPMessage * &pOpenFlowMsg, bool isRequest);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const
    {
        // TBD
    }
    
protected:
    ACE_UINT16 m_type; /* One of the OFPMP_* constants. */
    ACE_UINT16 m_flags; /* OFPMPF_REQ_* flags. */
    ACE_CDR::Octet  m_pad[4];
};

class COFP13MultipartRequestMsg : public COFP13MultipartMsg
{
public:
    enum ofp_multipart_request_flags {
        OFPMPF_REQ_MORE = 1 << 0 /* More requests to follow. */
    };
    
    COFP13MultipartRequestMsg(ACE_UINT16 type)
        :   COFP13MultipartMsg(OFP13::OFPT_MULTIPART_REQUEST, type, OFPMPF_REQ_MORE)
    {
 
    }
    virtual ~COFP13MultipartRequestMsg()    {}
};

class COFP13MultipartReplyMsg : public COFP13MultipartMsg
{
public:
    enum ofp_multipart_reply_flags {
        OFPMPF_REPLY_MORE = 1 << 0 /* More replies to follow. */
    };
    
    COFP13MultipartReplyMsg(ACE_UINT16 type)
        :   COFP13MultipartMsg(OFP13::OFPT_MULTIPART_REPLY, type, OFPMPF_REPLY_MORE)
    {
    }
    virtual ~COFP13MultipartReplyMsg()  {}
};


#endif

