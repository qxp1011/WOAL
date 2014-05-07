/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPFlowModMsg.h
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
#ifndef COFPFLOWMODMSG_H
#define COFPFLOWMODMSG_H

#include <list>
#include "COFPMessage.h"
#include "COFPInstruction.h"
#include "COFPMatch.h"
#include "COFP10Action.h"

class COFP10FlowModMsg : public COFPMessage
{
public:
    enum ofp_flow_mod_command {
        OFPFC_ADD, /* New flow. */
        OFPFC_MODIFY, /* Modify all matching flows. */
        OFPFC_MODIFY_STRICT, /* Modify entry strictly matching wildcards */
        OFPFC_DELETE, /* Delete all matching flows. */
        OFPFC_DELETE_STRICT /* Strictly match wildcards and priority. */
    };

    enum ofp_flow_mod_flags {
        OFPFF_SEND_FLOW_REM = 1 << 0, /* Send flow removed message when flow expires or is deleted. */
        OFPFF_CHECK_OVERLAP = 1 << 1, /* Check for overlapping entries first. */
        OFPFF_EMERG = 1 << 2 /* Remark this is for emergency. */
    };

    
    COFP10FlowModMsg(ACE_CDR::Octet abVersion = OFP10_VERSION)
        : COFPMessage(OFP10::OFPT_FLOW_MOD, abVersion)
    {
        ::memset(&m_tMember, 0, sizeof(m_tMember));
        m_wActLen = 0;
    }
    virtual ~COFP10FlowModMsg();

    COFP10Match *GetMatch()
    {
        return &m_Match;
    }

    ACE_UINT64 GetCookie() const
    {
        return m_tMember.lCookie;
    }
    ACE_UINT16 GetCommand() const
    {
        return m_tMember.wCommand;
    }
    ACE_UINT16 GetIdleTimeout() const
    {
        return m_tMember.wIdleTimeout;
    }
    ACE_UINT16 GetHardTimeout() const
    {
        return m_tMember.wHardTimeout;
    }
    ACE_UINT16 GetPriority() const
    {
        return m_tMember.wPriority;
    }
    ACE_UINT32 GetBufferId() const
    {
        return m_tMember.dwBufferId;
    }
    ACE_UINT16 GetOutPort() const
    {
        return m_tMember.wOutPort;
    }
    ACE_UINT16 GetFlags() const
    {
        return m_tMember.wFlags;
    }

    void SetCookie(ACE_UINT64 alCookie)
    {
        m_tMember.lCookie = alCookie;
    }
    void SetCommand(ACE_UINT16 awCmd)
    {
        m_tMember.wCommand = awCmd;
    }
    void SetIdleTimeout(ACE_UINT16 awTimeout)
    {
        m_tMember.wIdleTimeout = awTimeout;
    }
    void SetHardTimeout(ACE_UINT16 awTimeout)
    {
        m_tMember.wHardTimeout = awTimeout;
    }
    void SetPriority(ACE_UINT16 awPri)
    {
        m_tMember.wPriority = awPri;
    }
    void SetBufferId(ACE_UINT32 awId)
    {
        m_tMember.dwBufferId = awId;
    }
    void SetOutPort(ACE_UINT16 awPortNo)
    {
        m_tMember.wOutPort = awPortNo;
    }
    void SetFlags(ACE_UINT16 awFlag)
    {
        m_tMember.wFlags = awFlag;
    }
    
    CmResult AddActionList(std::list<COFP10Action *> &list);
    CmResult AppendAction(COFP10Action *apAct);
    CmResult GetActionList(std::list<COFP10Action *> &list);
        
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + m_Match.GetStreamLen() + sizeof(m_tMember) + m_wActLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    struct member
    {
        ACE_UINT64 lCookie;
        ACE_UINT16 wCommand;
        ACE_UINT16 wIdleTimeout;
        ACE_UINT16 wHardTimeout;
        ACE_UINT16 wPriority;
        ACE_UINT32 dwBufferId;
        ACE_UINT16 wOutPort;
        ACE_UINT16 wFlags;
    };
    
private:
    COFP10Match m_Match;
    struct member m_tMember;
    ACE_UINT16 m_wActLen;
    std::list<COFP10Action *> m_action_list;
};


#define OFP_NO_BUFFER ((ACE_UINT32)0xffffffff)

enum ofp_table {
    /* Last usable table number. */
    OFPTT_MAX = 0xfe,
    /* Fake tables. */
    OFPTT_ALL = 0xff /* Wildcard table used for table config,
        flow stats and flow deletes. */
};

/* Flow setup and teardown (controller -> datapath). */
class COFP13FlowModMsg : public COFPMessage
{
public:
    enum ofp_flow_mod_command {
        OFPFC_ADD           = 0, /* New flow. */
        OFPFC_MODIFY        = 1, /* Modify all matching flows. */
        OFPFC_MODIFY_STRICT = 2, /* Modify entry strictly matching wildcards and priority. */
        OFPFC_DELETE        = 3, /* Delete all matching flows. */
        OFPFC_DELETE_STRICT = 4, /* Delete entry strictly matching wildcards and priority. */
    };

    enum ofp_flow_mod_flags {
        OFPFF_SEND_FLOW_REM = 1 << 0, /* Send flow removed message when flow expires or is deleted. */
        OFPFF_CHECK_OVERLAP = 1 << 1, /* Check for overlapping entries first. */
        OFPFF_RESET_COUNTS  = 1 << 2, /* Reset flow packet and byte counts. */
        OFPFF_NO_PKT_COUNTS = 1 << 3, /* Don't keep track of packet count. */
        OFPFF_NO_BYT_COUNTS = 1 << 4, /* Don't keep track of byte count. */
    };

    COFP13FlowModMsg()
        : COFPMessage(OFP13::OFPT_FLOW_MOD, OFP13_VERSION, 0, 48 + COFPMessage::GetStreamLen())
        , m_nCookie(0)
        , m_nCookieMask(0)
        , m_bTableId(0)
        , m_bCommand(0)
        , m_wIdleTimeout(0)
        , m_wHardTimeout(0)
        , m_wPriority(0)
        , m_nBufferId(0)
        , m_nOutPort(0)
        , m_nOutGroup(0)
        , m_wFlags(0)
        , m_tMatch()
        , m_wInstructionLen(0)
     {}
    COFP13FlowModMsg(
        // args for ofp_flow_mod
        ACE_CDR::Octet abCommand, 
        ACE_UINT16 awFlags,
        // args for ofp_match
        ACE_UINT16      awType           = OFPMT_OXM,
        // args for ofp_flow_mod
        ACE_UINT64 anCookie       = 0,
        ACE_UINT64 anCookieMask   = 0, 
        ACE_CDR::Octet abTableId      = 0, 
        ACE_UINT16 awIdleTimeout  = 0, 
        ACE_UINT16 awHardTimeout  = 0, 
        ACE_UINT16 awPriority     = 0,
        ACE_UINT32 anBufferId     = 0, 
        ACE_UINT32 anOutPort      = 0,
        ACE_UINT32 anOutGroup     = 0,
        // args for OFP head
        ACE_UINT32 anOFPHeadXId   = 0, 
        ACE_CDR::Octet abVersion      = OFP13_VERSION
        )
        : COFPMessage(OFP13::OFPT_FLOW_MOD, abVersion, anOFPHeadXId, 48)
        , m_nCookie(anCookie)
        , m_nCookieMask(anCookieMask)
        , m_bTableId(abTableId)
        , m_bCommand(abCommand)
        , m_wIdleTimeout(awIdleTimeout)
        , m_wHardTimeout(awHardTimeout)
        , m_wPriority(awPriority)
        , m_nBufferId(anBufferId)
        , m_nOutPort(anOutPort)
        , m_nOutGroup(anOutGroup)
        , m_wFlags(awFlags)
        , m_tMatch(awType)
        , m_wInstructionLen(0)
     {}
    virtual ~COFP13FlowModMsg();

public:
    inline ACE_UINT64 GetCookie() const
    {
        return m_nCookie;
    }

    inline ACE_UINT64 GetCookieMask() const
    {
        return m_nCookieMask;
    }

    inline ACE_CDR::Octet GetTableId() const
    {
        return m_bTableId;
    }

    inline ACE_CDR::Octet GetCommand() const
    {
        return m_bCommand;
    }

    inline ACE_UINT16 GetIdleTimeout() const
    {
        return m_wIdleTimeout;
    }

    inline ACE_UINT16 GetHardTimeout() const
    {
        return m_wHardTimeout;
    }

    inline ACE_UINT16 GetPriority() const
    {
        return m_wPriority;
    }

    inline ACE_UINT32 GetBufferId() const
    {
        return m_nBufferId;
    }

    inline ACE_UINT32 GetOutPort() const
    {
        return m_nOutPort;
    }

    inline ACE_UINT32 GetOutGroup() const
    {
        return m_nOutGroup;
    }

    inline ACE_UINT16 GetFlags() const
    {
        return m_wFlags;
    }

    inline const COFP13Match *GetMatch() const
    {
        return &m_tMatch;
    }

    inline void SetCookie(ACE_UINT64 anCookie)
    {
        m_nCookie = anCookie;
    }

    inline void SetCookieMask(ACE_UINT64 anCookieMask)
    {
        m_nCookieMask = anCookieMask;
    }

    inline void SetTableId(ACE_CDR::Octet abTableId)
    {
        m_bTableId = abTableId;
    }

    inline void SetCommand(ACE_CDR::Octet abCommand)
    {
        m_bCommand = abCommand;
    }

    inline void SetIdleTimeout(ACE_UINT16 awIdleTimeout)
    {
        m_wIdleTimeout = awIdleTimeout;
    }

    inline void SetHardTimeout(ACE_UINT16 awHardTimeout)
    {
        m_wHardTimeout = awHardTimeout;
    }

    inline void SetPriority(ACE_UINT16 awPriority)
    {
        m_wPriority = awPriority;
    }

    inline void SetBufferId(ACE_UINT32 anBufferId)
    {
        m_nBufferId = anBufferId;
    }

    inline void SetOutPort(ACE_UINT32 anOutPort)
    {
        m_nOutPort = anOutPort;
    }

    inline void SetOutGroup(ACE_UINT32 anOutGroup)
    {
        m_nOutGroup = anOutGroup;
    }

    inline void SetFlags(ACE_UINT16 awFlags)
    {
        m_wFlags = awFlags;
    }

    // reload COFP13Match's assignment operator
    inline void SetMatch(COFP13Match& atMatch)
    {
        m_tMatch = atMatch;
    }

    CmResult AddInstructionList(std::list<COFP13Instruction *> &list);
    CmResult AppendInstruction(COFP13Instruction *aInstruction);
    CmResult GetInstructionList(std::list<COFP13Instruction *> &list);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR& os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    ACE_UINT64 m_nCookie;     /* Opaque controller-issued identifier. */
    ACE_UINT64 m_nCookieMask; /* Mask used to restrict the cookie bits that must match when the command is
                                                OFPFC_MODIFY* or OFPFC_DELETE*. A value of 0 indicates no restriction. */
    /* Flow actions. */
    ACE_CDR::Octet m_bTableId; /* ID of the table to put the flow in. For OFPFC_DELETE_* commands, OFPTT_ALL
                                                can also be used to delete matching flows from all tables. */
    ACE_CDR::Octet m_bCommand; /* One of OFPFC_*. */
    ACE_UINT16 m_wIdleTimeout; /* Idle time before discarding (seconds). */
    ACE_UINT16 m_wHardTimeout; /* Max time before discarding (seconds). */
    ACE_UINT16 m_wPriority; /* Priority level of flow entry. */
    ACE_UINT32 m_nBufferId; /* Buffered packet to apply to, or OFP_NO_BUFFER. Not meaningful for OFPFC_DELETE*. */
    ACE_UINT32 m_nOutPort;  /* For OFPFC_DELETE* commands, require matching entries to include this as an 
                                            output port. A value of OFPP_ANY indicates no restriction. */
    ACE_UINT32 m_nOutGroup; /* For OFPFC_DELETE* commands, require matching entries to include this as an
                                            output group. A value of OFPG_ANY indicates no restriction. */
    ACE_UINT16 m_wFlags;    /* One of OFPFF_*. */
    COFP13Match m_tMatch; /* Fields to match. Variable size. */
    ACE_UINT16 m_wInstructionLen;
    std::list<COFP13Instruction *> m_instruction_list;
};

#endif

