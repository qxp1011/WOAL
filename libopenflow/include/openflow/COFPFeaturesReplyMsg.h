/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPFeaturesReplyMsg.h
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
#ifndef COFPFEATURESREPLYMSG_H
#define COFPFEATURESREPLYMSG_H

#include "COFPMessage.h"
#include "COFPPort.h"

class COFP10FeaturesReplyMsg : public COFPMessage
{
public:
    COFP10FeaturesReplyMsg();
    virtual ~COFP10FeaturesReplyMsg();

    inline ACE_UINT64 GetDataPathID() const
    {
        return m_lDataPathID;
    }

    inline ACE_UINT32 GetBuffers() const
    {
        return m_dwBuffers;
    }

    inline ACE_CDR::Octet GetTables() const
    {
        return m_bTables;
    }

    inline ACE_UINT32 GetCapabilities() const
    {
        return m_dwCapabilities;
    }

    inline ACE_UINT32 GetActions() const
    {
        return m_dwActions;
    }

    inline void SetDataPathID(ACE_UINT64 anDataPathID)
    {
        m_lDataPathID = anDataPathID;
    }

    inline void SetBuffers(ACE_UINT32 anBuffers)
    {
        m_dwBuffers = anBuffers;
    }

    inline void SetTables(ACE_CDR::Octet abTables)
    {
        m_bTables = abTables;
    }

    inline void SetCapabilities(ACE_UINT32 anCapabilities)
    {
        m_dwCapabilities = anCapabilities;
    }

    inline void SetActions(ACE_UINT32 adwActions)
    {
        m_dwActions = adwActions;
    }
    
    CmResult AddPortList(std::list<COFP10PhyPort *> &list);
    CmResult AppendPort(COFP10PhyPort *aPort);
    CmResult GetPortList(std::list<COFP10PhyPort *> &list);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return (32 + m_wPortLen);
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
private:
    ACE_UINT64 m_lDataPathID;
    ACE_UINT32 m_dwBuffers;
    ACE_CDR::Octet   m_bTables;
    ACE_UINT32 m_dwCapabilities;
    ACE_UINT32 m_dwActions;
    ACE_UINT16 m_wPortLen;
    std::list<COFP10PhyPort *> m_port_list;
};




class COFP13FeaturesReplyMsg : public COFPMessage
{
public:
     /*
        * Common capabilities supported by the datapath (struct ofp_switch_features,
        * member capabilities). 
        */
    enum ofp_capabilities {
        OFPC_FLOW_STATS     = 1 << 0,  /* Flow statistics. */
        OFPC_TABLE_STATS    = 1 << 1,  /* Table statistics. */
        OFPC_PORT_STATS     = 1 << 2,  /* Port statistics. */
        OFPC_GROUP_STATS    = 1 << 3,  /* Group statistics */
        OFPC_IP_REASM       = 1 << 5,  /* Can reassemble IP fragments. */
        OFPC_QUEUE_STATS    = 1 << 6,  /* Queue statistics. */
        OFPC_ARP_MATCH_IP   = 1 << 7   /* Match IP addresses in ARP pkts. */
    };
    COFP13FeaturesReplyMsg()
        : COFPMessage(OFP13::OFPT_FEATURES_REPLY, OFP13_VERSION)
        , m_nDataPathID(0)
        ,m_nBuffers(0)
        ,m_bTables(0)
        ,m_bAuxiliaryID(0)
        ,m_nCapabilities(0)
        ,m_nReserved(0)
     {}
    COFP13FeaturesReplyMsg(
        ACE_UINT32 anCapabilities,
        ACE_UINT64 anDataPathID  = 0,
        ACE_UINT32 anBuffers     = 0,
        ACE_CDR::Octet   abTables      = 0,
        ACE_CDR::Octet   abAuxiliaryID = 0,
        /* args for opf_header */
        ACE_UINT32 anOPFHeadXID  = 0,
        ACE_CDR::Octet   abOPFHeadVer  = OFP13_VERSION
        )
        : COFPMessage(OFP13::OFPT_FEATURES_REPLY, abOPFHeadVer, anOPFHeadXID, 24)
        , m_nDataPathID(anDataPathID)
        , m_nBuffers(anBuffers)
        , m_bTables(abTables)
        , m_bAuxiliaryID(abAuxiliaryID)
        , m_nCapabilities(anCapabilities)
        , m_nReserved(0)
     {}     
    virtual ~COFP13FeaturesReplyMsg(){}

    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return 32;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
public:
    inline ACE_UINT64 GetDataPathID() const
    {
        return m_nDataPathID;
    }

    inline ACE_UINT32 GetBuffers() const
    {
        return m_nBuffers;
    }

    inline ACE_CDR::Octet GetTables() const
    {
        return m_bTables;
    }

    inline ACE_CDR::Octet GetAuxiliaryID() const
    {
        return m_bAuxiliaryID;
    }
    
    inline ACE_UINT32 GetCapabilities() const
    {
        return m_nCapabilities;
    }

    inline void SetDataPathID(ACE_UINT64 anDataPathID)
    {
        m_nDataPathID = anDataPathID;
    }

    inline void SetBuffers(ACE_UINT32 anBuffers)
    {
        m_nBuffers = anBuffers;
    }

    inline void SetTables(ACE_CDR::Octet abTables)
    {
        m_bTables = abTables;
    }

    inline void SetAuxiliaryID(ACE_CDR::Octet abAuxiliaryID)
    {
        m_bAuxiliaryID = abAuxiliaryID;
    }

    inline void SetCapabilities(ACE_UINT32 anCapabilities)
    {
        m_nCapabilities = anCapabilities;
    }

protected:
    ACE_UINT64 m_nDataPathID;
    ACE_UINT32 m_nBuffers;
    ACE_CDR::Octet   m_bTables;
    ACE_CDR::Octet   m_bAuxiliaryID;
    //Features
    ACE_UINT32 m_nCapabilities;
    ACE_UINT32 m_nReserved;
};

#endif

