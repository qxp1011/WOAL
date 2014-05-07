/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPStatsMsgHeader.h
 * Description:      
 * Others:
 * Version:          V1.0
 * Author:          
 * Date:             2013-07-15
 *
 * History 1:
 *     Date:         
 *     Version:      
 *     Author:       
 *     Modification:
**********************************************************************/
#ifndef COFPSTATSMSGHEADER_H
#define COFPSTATSMSGHEADER_H

#include "COFPMessage.h"
#include "COFPMatch.h"
#include "COFP10Action.h"

class COFP10StatsMsgHeader : public COFPMessage
{
public:
    enum ofp_stats_types 
    {
        /* Description of this OpenFlow switch.
              * The request body is empty.
              * The reply body is struct ofp_desc_stats. */
        OFPST_DESC,
        
        /* Individual flow statistics.
              * The request body is struct ofp_flow_stats_request.
              * The reply body is an array of struct ofp_flow_stats. */
        OFPST_FLOW,
        
        /* Aggregate flow statistics.
              * The request body is struct ofp_aggregate_stats_request.
              * The reply body is struct ofp_aggregate_stats_reply. */
        OFPST_AGGREGATE,
        
        /* Flow table statistics.
              * The request body is empty.
              * The reply body is an array of struct ofp_table_stats. */
        OFPST_TABLE,
        
        /* Physical port statistics.
              * The request body is struct ofp_port_stats_request.
              * The reply body is an array of struct ofp_port_stats. */
        OFPST_PORT,
        
        /* Queue statistics for a port
              * The request body defines the port
              * The reply body is an array of struct ofp_queue_stats */
        OFPST_QUEUE,
        
        /* Vendor extension.
              * The request and reply bodies begin with a 32-bit vendor ID, which takes
              * the same form as in "struct ofp_vendor_header". The request and reply
              * bodies are otherwise vendor-defined. */
        OFPST_VENDOR = 0xffff
    };

public:
    COFP10StatsMsgHeader(ACE_CDR::Octet abMsgType, ACE_UINT16 awStatsType = 0, ACE_UINT16 awFlag = 0)
        : COFPMessage(abMsgType, OFP10_VERSION)
        , m_wStatsType(awStatsType)
        , m_wFlag(awFlag)
    {
        if ((OFP10::OFPT_STATS_REQUEST != abMsgType) && (OFP10::OFPT_STATS_REPLY != abMsgType))
        {
            ACE_ASSERT(0);
        }
    }
    ~COFP10StatsMsgHeader(){}

    ACE_UINT16 GetStatsType() const
    {
        return m_wStatsType;
    }
    ACE_UINT16 GetFlag() const
    {
        return m_wFlag;
    }
    
    void SetStatsType(ACE_UINT16 awType)
    {
        m_wStatsType = awType;
    }
    void SetFlag(ACE_UINT16 awFlag)
    {
        m_wFlag = awFlag;
    }

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + sizeof m_wStatsType + sizeof m_wFlag;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
protected:
    ACE_UINT16 m_wStatsType;
    ACE_UINT16 m_wFlag;
};


class COFP10FlowStatsReplyEle : public IOFPMsgParser
{
public:
    COFP10FlowStatsReplyEle()
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
        m_tMember.length = m_Match.GetStreamLen() + sizeof m_tMember;
    }
    virtual ~COFP10FlowStatsReplyEle();

    COFP10Match *GetMatch()
    {
        return &m_Match;
    }
    ACE_UINT16 GetBodyLen() const
    {
        return m_tMember.length;
    }
    ACE_CDR::Octet GetTableId() const
    {
        return m_tMember.table_id;
    }
    ACE_UINT32 GetDurationSec() const
    {
        return m_tMember.duration_sec;
    }
    ACE_UINT32 GetDurationNsec() const
    {
        return m_tMember.duration_nsec;
    }
    ACE_UINT16 GetPriority() const
    {
        return m_tMember.priority;
    }
    ACE_UINT16 GetIdleTimeout() const
    {
        return m_tMember.idle_timeout;
    }
    ACE_UINT16 GetHardTimeout() const
    {
        return m_tMember.hard_timeout;
    }
    ACE_UINT64 GetCookie() const
    {
        return m_tMember.cookie;
    }
    ACE_UINT64 GetPacketCount() const
    {
        return m_tMember.packet_count;
    }
    ACE_UINT64 GetByteCount() const
    {
        return m_tMember.byte_count;
    }

    void SetTableId(ACE_CDR::Octet abId)
    {
        m_tMember.table_id = abId;
    }    
    void SetDurationSec(ACE_UINT32 adwSec)
    {
        m_tMember.duration_sec = adwSec;
    }
    void SetDurationNsec(ACE_UINT32 adwNsec)
    {
        m_tMember.duration_nsec = adwNsec;
    }
    void SetPriority(ACE_UINT16 awPri)
    {
        m_tMember.priority = awPri;
    }
    void SetIdleTimeout(ACE_UINT16 awTimeout)
    {
        m_tMember.idle_timeout = awTimeout;
    }
    void SetHardTimeout(ACE_UINT16 awTimeout)
    {
        m_tMember.hard_timeout = awTimeout;
    }
    void SetCookie(ACE_UINT64 alCookie)
    {
        m_tMember.cookie = alCookie;
    }
    void SetPacketCount(ACE_UINT64 alCnt)
    {
        m_tMember.packet_count = alCnt;
    }
    void SetByteCount(ACE_UINT64 alCnt)
    {
        m_tMember.byte_count = alCnt;
    }
    
    CmResult AddActionList(std::list<COFP10Action *> &list);
    CmResult AppendAction(COFP10Action *pAct);
    CmResult GetActionList(std::list<COFP10Action *> &list);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const
    {
        return m_tMember.length;
    }
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

protected:
    struct member
    {
        ACE_UINT16 length; /* Length of this entry. */
        ACE_CDR::Octet table_id; /* ID of table flow came from. */
        ACE_CDR::Octet pad;
        ACE_UINT32 duration_sec; /* Time flow has been alive in seconds. */
        ACE_UINT32 duration_nsec; /* Time flow has been alive in nanoseconds beyond duration_sec. */
        ACE_UINT16 priority; /* Priority of the entry. Only meaningful when this is not an exact-match entry. */
        ACE_UINT16 idle_timeout; /* Number of seconds idle before expiration. */
        ACE_UINT16 hard_timeout; /* Number of seconds before expiration. */
        ACE_CDR::Octet pad2[6]; /* Align to 64-bits. */
        ACE_UINT64 cookie; /* Opaque controller-issued identifier. */
        ACE_UINT64 packet_count; /* Number of packets in flow. */
        ACE_UINT64 byte_count; /* Number of bytes in flow. */
    };

private:
    COFP10Match m_Match;
    struct member m_tMember;
    std::list<COFP10Action *> m_act_list;
};

#endif

