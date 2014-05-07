/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPFlowStatisticsMsg.h
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
#ifndef COFPMPFLOWSTATISTICSMSG_H
#define COFPMPFLOWSTATISTICSMSG_H

#include "COFPMultipartMsg.h"
#include "COFPMatch.h"
#include "COFPInstruction.h"

class COFP13MPFlowStatsRequestMsg : public COFP13MultipartRequestMsg
{
public:
    /* Body for ofp_multipart_request of type OFPMP_FLOW. */
    struct ofp_flow_stats_request {
        ACE_CDR::Octet table_id; /* ID of table to read (from ofp_table_stats),OFPTT_ALL for all tables. */
        ACE_CDR::Octet pad[3]; /* Align to 32 bits. */
        ACE_UINT32 out_port; 
        ACE_UINT32 out_group; 
        ACE_CDR::Octet pad2[4]; /* Align to 64 bits. */
        uint64_t cookie; /* Require matching entries to contain this cookie value */
        uint64_t cookie_mask; /* Mask used to restrict the cookie bits that*/
        //struct ofp_match match; /* Fields to match. Variable size. */
    };
    COFP13MPFlowStatsRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_FLOW)
    {
        ::memset(&m_stats_request, 0,sizeof(m_stats_request));
    }
    virtual ~COFP13MPFlowStatsRequestMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

    COFP13MPFlowStatsRequestMsg::ofp_flow_stats_request &GetStatsRequestInfo();
    void SetStatsRequestInfo(
        COFP13MPFlowStatsRequestMsg::ofp_flow_stats_request &requestInfo);

    COFP13Match &GetProtocolMatch();
    void SetProtocolMatch(COFP13Match &match);
private:
    ofp_flow_stats_request m_stats_request;
    COFP13Match m_match;
};


class COFP13MPFlowStatsReplyMsg : public COFP13MultipartReplyMsg
{
public:
    /* Body of reply to OFPMP_FLOW request. */
    struct ofp_flow_stats {
        ACE_UINT16 length; /* Length of this entry. */
        ACE_CDR::Octet table_id; /* ID of table flow came from. */
        ACE_CDR::Octet pad;
        ACE_UINT32 duration_sec; /* Time flow has been alive in seconds. */
        ACE_UINT32 duration_nsec; /* Time flow has been alive in nanoseconds beyond duration_sec. */
        ACE_UINT16 priority; /* Priority of the entry. */
        ACE_UINT16 idle_timeout; /* Number of seconds idle before expiration. */
        ACE_UINT16 hard_timeout; /* Number of seconds before expiration. */
        ACE_UINT16 flags; /* One of OFPFF_*. */
        ACE_CDR::Octet pad2[4]; /* Align to 64-bits. */
        uint64_t cookie; /* Opaque controller-issued identifier. */
        uint64_t packet_count; /* Number of packets in flow. */
        uint64_t byte_count; /* Number of bytes in flow. */
        //struct ofp_match match; /* Description of fields. Variable size. */
        //struct ofp_instruction instructions[0]; /* Instruction set. */
    };

    COFP13MPFlowStatsReplyMsg():COFP13MultipartReplyMsg(OFPMP_FLOW)
    {
        ::memset(&m_flowstats, 0, sizeof(m_flowstats));
        m_wInstructionLen = 0;
    }
    virtual ~COFP13MPFlowStatsReplyMsg();

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPFlowStatsReplyMsg::ofp_flow_stats &GetFlowStatsInfo();
    void SetFlowStatsInfo(COFP13MPFlowStatsReplyMsg::ofp_flow_stats &flowstats);

    COFP13Match &GetProtocolMatch();
    void SetProtocolMatch(COFP13Match &match);
    
    CmResult AddInstructionList(std::list<COFP13Instruction *> &list);
    CmResult AppendInstruction(COFP13Instruction *apInstruction);
    CmResult GetInstructionList(std::list<COFP13Instruction *> &list);

private:
    ofp_flow_stats m_flowstats;
    COFP13Match    m_match;
    ACE_UINT16     m_wInstructionLen;
    std::list<COFP13Instruction *> m_instruction_list;
};

#endif

