/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPStatsReplyMsg.h
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
#ifndef COFPSTATSREPLYMSG_H
#define COFPSTATSREPLYMSG_H

#include "COFPStatsMsgHeader.h"

class COFP10StatsReplyMsg : public COFP10StatsMsgHeader
{
public:
    COFP10StatsReplyMsg(ACE_UINT16 awStatsType = 0, ACE_UINT16 awFlag = 0)
        : COFP10StatsMsgHeader(OFP10::OFPT_STATS_REPLY, awStatsType, awFlag)
    {
    }
    ~COFP10StatsReplyMsg(){}

    static CmResult COFP10DecodeStatsReplyMsg(ACE_InputCDR &aIs, COFPMessage * &pMsg);
};


class COFP10DescStatsReplyMsg : public COFP10StatsReplyMsg
{
public:
    COFP10DescStatsReplyMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsReplyMsg(OFPST_DESC, awFlag)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
    }
    ~COFP10DescStatsReplyMsg(){}

    const char *GetManufacturerDesc() const
    {
        return m_tMember.mfr_desc;
    }
    const char *GetHardwareDesc() const
    {
        return m_tMember.hw_desc;
    }
    const char *GetSoftwareDesc() const
    {
        return m_tMember.sw_desc;
    }
    const char *GetSerialNum() const
    {
        return m_tMember.serial_num;
    }
    const char *GetDatapathDesc() const
    {
        return m_tMember.dp_desc;
    }

    CmResult SetManufacturerDesc(char *apDesc);
    CmResult SetHardwareDesc(char *apDesc);
    CmResult SetSoftwareDesc(char *apDesc);
    CmResult SetSerialNum(char *apNum);
    CmResult SetDatapathDesc(char *apDesc);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsReplyMsg::GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
protected:
    /*Each entry is a NULL-terminated ASCII string. */
    struct member
    {
        char mfr_desc[DESC_STR_LEN]; /* Manufacturer description. */
        char hw_desc[DESC_STR_LEN]; /* Hardware description. */
        char sw_desc[DESC_STR_LEN]; /* Software description. */
        char serial_num[SERIAL_NUM_LEN]; /* Serial number. */
        char dp_desc[DESC_STR_LEN]; /* Human readable description of datapath. */
    };
    
private:
    struct member m_tMember;
};


class COFP10FlowStatsReplyMsg : public COFP10StatsReplyMsg
{
public:
    COFP10FlowStatsReplyMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsReplyMsg(OFPST_FLOW, awFlag)
        , m_wEleLen(0)
    {
    }
    virtual ~COFP10FlowStatsReplyMsg();

    CmResult AddEleList(std::list<COFP10FlowStatsReplyEle *> &list);
    CmResult AppendEle(COFP10FlowStatsReplyEle *aEle);
    CmResult GetEleList(std::list<COFP10FlowStatsReplyEle *> &list);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsReplyMsg::GetStreamLen() + m_wEleLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

private:
    ACE_UINT16 m_wEleLen;
    std::list<COFP10FlowStatsReplyEle *> m_ele_list;
};


class COFP10AggregateStatsReplyMsg : public COFP10StatsReplyMsg
{
public:
    COFP10AggregateStatsReplyMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsReplyMsg(OFPST_AGGREGATE, awFlag)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
    }
    ~COFP10AggregateStatsReplyMsg(){}

    ACE_UINT64 GetPacketCount() const
    {
        return m_tMember.packet_count;
    }
    ACE_UINT64 GetByteCount() const
    {
        return m_tMember.byte_count;
    }
    ACE_UINT32 GetFlowCount() const
    {
        return m_tMember.flow_count;
    }

    void SetPacketCount(ACE_UINT64 alCnt)
    {
        m_tMember.packet_count = alCnt;
    }
    void SetByteCount(ACE_UINT64 alCnt)
    {
        m_tMember.byte_count = alCnt;
    }
    void SetFlowCount(ACE_UINT32 adwCnt)
    {
        m_tMember.flow_count = adwCnt;
    }
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsReplyMsg::GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    struct member
    {
        ACE_UINT64 packet_count; /* Number of packets in flows. */
        ACE_UINT64 byte_count; /* Number of bytes in flows. */
        ACE_UINT32 flow_count; /* Number of flows. */
        ACE_CDR::Octet pad[4]; /* Align to 64 bits. */
    };

private:
    struct member m_tMember;
};


class COFP10TableStatsReplyMsg : public COFP10StatsReplyMsg
{
public:
    struct TableStatsReply
    {
        TableStatsReply()
        {
            ::memset(this, 0, sizeof(TableStatsReply));
        }
        
        ACE_CDR::Octet table_id; /* Identifier of table. Lower numbered tables are consulted first. */
        ACE_CDR::Octet pad[3]; /* Align to 32-bits. */
        char name[OFP_MAX_TABLE_NAME_LEN];
        ACE_UINT32 wildcards; /* Bitmap of OFPFW_* wildcards that are supported by the table. */
        ACE_UINT32 max_entries; /* Max number of entries supported. */
        ACE_UINT32 active_count; /* Number of active entries. */
        ACE_UINT64 lookup_count; /* Number of packets looked up in table. */
        ACE_UINT64 matched_count; /* Number of packets that hit table. */
    };

    static_assert(64 == sizeof(TableStatsReply), "TableStatsReply error len value");

public:
    COFP10TableStatsReplyMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsReplyMsg(OFPST_TABLE, awFlag)
        , m_wTableLen(0)
    {
    }
    ~COFP10TableStatsReplyMsg();

    CmResult AddTableList(std::list<TableStatsReply *> &list);
    CmResult AppendTable(TableStatsReply *aTable);
    CmResult GetTableList(std::list<TableStatsReply *> &list);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsReplyMsg::GetStreamLen() + m_wTableLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

private:
    ACE_UINT16 m_wTableLen;
    std::list<TableStatsReply *> m_table_list;
};



class COFP10PortStatsReplyMsg : public COFP10StatsReplyMsg
{
public:
    /*If a counter is unsupported, set the field to all ones. */
    struct PortStatsReply
    {
        PortStatsReply()
        {
            ::memset(this, 0, sizeof(PortStatsReply));
        }
        
        ACE_UINT16 port_no;
        ACE_CDR::Octet pad[6]; /* Align to 64-bits. */
        ACE_UINT64 rx_packets; /* Number of received packets. */
        ACE_UINT64 tx_packets; /* Number of transmitted packets. */
        ACE_UINT64 rx_bytes; /* Number of received bytes. */
        ACE_UINT64 tx_bytes; /* Number of transmitted bytes. */
        ACE_UINT64 rx_dropped; /* Number of packets dropped by RX. */
        ACE_UINT64 tx_dropped; /* Number of packets dropped by TX. */
        ACE_UINT64 rx_errors; /* Number of receive errors. This is a super-set of more specific receive 
                                             errors and should be greater than or equal to the sum of all rx_*_err values. */
        ACE_UINT64 tx_errors; /* Number of transmit errors. This is a super-set of more 
                                             specific transmit errors and should be greater than or equal 
                                             to the sum of all tx_*_err values (none currently defined.) */
        ACE_UINT64 rx_frame_err; /* Number of frame alignment errors. */
        ACE_UINT64 rx_over_err; /* Number of packets with RX overrun. */
        ACE_UINT64 rx_crc_err; /* Number of CRC errors. */
        ACE_UINT64 collisions; /* Number of collisions. */
    };

    static_assert(104 == sizeof(PortStatsReply), "PortStatsReply error len value");
    
public:
    COFP10PortStatsReplyMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsReplyMsg(OFPST_PORT, awFlag)
        , m_wPortLen(0)
    {
    }
    virtual ~COFP10PortStatsReplyMsg();
    
    CmResult AddPortList(std::list<PortStatsReply *> &list);
    CmResult AppendPort(PortStatsReply *aPort);
    CmResult GetPortList(std::list<PortStatsReply *> &list);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsReplyMsg::GetStreamLen() + m_wPortLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

private:
    ACE_UINT16 m_wPortLen;
    std::list<PortStatsReply *> m_port_list;
};


class COFP10QueueStatsReplyMsg : public COFP10StatsReplyMsg
{
public:
    struct QueueStatsReply
    {
        QueueStatsReply()
        {
            ::memset(this, 0, sizeof(QueueStatsReply));
        }
        
        ACE_UINT16 port_no;
        ACE_CDR::Octet pad[2]; /* Align to 32-bits. */
        ACE_UINT32 queue_id; /* Queue i.d */
        ACE_UINT64 tx_bytes; /* Number of transmitted bytes. */
        ACE_UINT64 tx_packets; /* Number of transmitted packets. */
        ACE_UINT64 tx_errors; /* Number of packets dropped due to overrun. */
    };

    static_assert(32 == sizeof(QueueStatsReply), "QueueStatsReply error len value");
    
public:
    COFP10QueueStatsReplyMsg(ACE_UINT16 awFlag = 0)
        : COFP10StatsReplyMsg(OFPST_QUEUE, awFlag)
        , m_wQueueLen(0)
    {
    }
    ~COFP10QueueStatsReplyMsg();
    
    CmResult AddQueueList(std::list<QueueStatsReply *> &list);
    CmResult AppendQueue(QueueStatsReply *aQueue);
    CmResult GetQueueList(std::list<QueueStatsReply *> &list);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFP10StatsReplyMsg::GetStreamLen() + m_wQueueLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;


private:
    ACE_UINT16 m_wQueueLen;
    std::list<QueueStatsReply *> m_queue_list;
};

#endif

