/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPStatsRequestMsg.h
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
#ifndef COFPSTATSREQUESTMSG_H
#define COFPSTATSREQUESTMSG_H

#include "COFPStatsMsgHeader.h"
#include "COFPMatch.h"


class COFP10StatsRequestMsg : public COFP10StatsMsgHeader
{
public:
    COFP10StatsRequestMsg(ACE_UINT16 awStatsType = 0, ACE_UINT16 awFlag = 0)
        : COFP10StatsMsgHeader(OFP10::OFPT_STATS_REQUEST, awStatsType, awFlag)
    {
    }
    ~COFP10StatsRequestMsg(){}
    
    static CmResult COFP10DecodeStatsRequestMsg(ACE_InputCDR &aIs, COFPMessage * &pMsg);
};


class COFP10DescStatsRequestMsg : public COFP10StatsRequestMsg
{
public:
    COFP10DescStatsRequestMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsRequestMsg(OFPST_DESC, awFlag)
    {
    }
    ~COFP10DescStatsRequestMsg(){}
};


class COFP10FlowStatsRequestMsg : public COFP10StatsRequestMsg
{
public:
    COFP10FlowStatsRequestMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsRequestMsg(OFPST_FLOW, awFlag)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
    }
    ~COFP10FlowStatsRequestMsg(){}

    COFP10Match *GetMatch()
    {
        return &m_Match;
    }
    ACE_CDR::Octet GetTableId() const
    {
        return m_tMember.table_id;
    }
    ACE_UINT16 GetOutPort() const
    {
        return m_tMember.out_port;
    }

    void SetTableId(ACE_CDR::Octet abTableId)
    {
        m_tMember.table_id = abTableId;
    }
    void SetOutPort(ACE_UINT16 awPortNo)
    {
        m_tMember.out_port = awPortNo;
    }

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsRequestMsg::GetStreamLen() + m_Match.GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
protected:
    struct member
    {
        ACE_CDR::Octet table_id; /* ID of table to read (from ofp_table_stats), 0xff for all tables or 0xfe for emergency. */
        ACE_CDR::Octet pad;      /* Align to 32 bits. */
        ACE_UINT16 out_port; /* Require matching entries to include this as an output port. 
                                               A value of OFPP_NONE indicates no restriction. */
    };
    
private:
    COFP10Match m_Match;
    struct member m_tMember;
};


class COFP10AggregateStatsRequestMsg : public COFP10StatsRequestMsg
{
public:
    COFP10AggregateStatsRequestMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsRequestMsg(OFPST_AGGREGATE, awFlag)
    {
    }
    ~COFP10AggregateStatsRequestMsg(){}

    COFP10Match *GetMatch()
    {
        return &m_Match;
    }
    ACE_CDR::Octet GetTableId() const
    {
        return m_tMember.table_id;
    }
    ACE_UINT16 GetOutPort() const
    {
        return m_tMember.out_port;
    }

    void SetTableId(ACE_CDR::Octet abTableId)
    {
        m_tMember.table_id = abTableId;
    }
    void SetOutPort(ACE_UINT16 awPortNo)
    {
        m_tMember.out_port = awPortNo;
    }

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsRequestMsg::GetStreamLen() + m_Match.GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
protected:
    struct member
    {
        ACE_CDR::Octet table_id; /* ID of table to read (from ofp_table_stats), 0xff for all tables or 0xfe for emergency. */
        ACE_CDR::Octet pad;      /* Align to 32 bits. */
        ACE_UINT16 out_port; /* Require matching entries to include this as an output port. 
                                               A value of OFPP_NONE indicates no restriction. */
    };
    
private:
    COFP10Match m_Match;
    struct member m_tMember;
};


class COFP10TableStatsRequestMsg : public COFP10StatsRequestMsg
{
public:
    COFP10TableStatsRequestMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsRequestMsg(OFPST_TABLE, awFlag)
    {
    }
    ~COFP10TableStatsRequestMsg(){}
};


class COFP10PortStatsRequestMsg : public COFP10StatsRequestMsg
{
public:
    COFP10PortStatsRequestMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsRequestMsg(OFPST_PORT, awFlag)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
    }
    ~COFP10PortStatsRequestMsg(){}

    ACE_UINT16 GetPortNo() const
    {
        return m_tMember.port_no;
    }

    void SetPortNo(ACE_UINT16 awPortNo)
    {
        m_tMember.port_no = awPortNo;
    }
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsRequestMsg::GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    struct member
    {
        ACE_UINT16 port_no; /* OFPST_PORT message must request statistics
                                           * either for a single port (specified in port_no) or for all ports 
                                           * (if port_no == OFPP_NONE). */
        ACE_CDR::Octet pad[6];
    };

private:
    struct member m_tMember;
};


class COFP10QueueStatsRequestMsg : public COFP10StatsRequestMsg
{
public:
    COFP10QueueStatsRequestMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsRequestMsg(OFPST_QUEUE, awFlag)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
    }
    ~COFP10QueueStatsRequestMsg(){}

    ACE_UINT16 GetPortNo() const
    {
        return m_tMember.port_no;
    }
    ACE_UINT32 GetQueueId() const
    {
        return m_tMember.queue_id;
    }

    void SetPortNo(ACE_UINT16 awPortNo)
    {
        m_tMember.port_no = awPortNo;
    }
    void SetQueueId(ACE_UINT32 adwQueueId)
    {
        m_tMember.queue_id = adwQueueId;
    }
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsRequestMsg::GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    struct member
    {
        ACE_UINT16 port_no; /* All ports if OFPT_ALL. */
        ACE_CDR::Octet pad[2];   /* Align to 32-bits. */
        ACE_UINT32 queue_id; /* All queues if OFPQ_ALL. */
    };

private:
    struct member m_tMember;
};


#endif

