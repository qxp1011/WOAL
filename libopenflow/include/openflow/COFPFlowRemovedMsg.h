/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPFlowRemovedMsg.h
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
#ifndef COFPFLOWREMOVEDMSG_H
#define COFPFLOWREMOVEDMSG_H

#include "COFPMessage.h"
#include "COFPMatch.h"

class COFP10FlowRemovedMsg : public COFPMessage
{
public:
    COFP10FlowRemovedMsg(ACE_CDR::Octet abVersion = OFP10_VERSION)
        : COFPMessage(OFP10::OFPT_FLOW_REMOVED, abVersion)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
    }
    virtual ~COFP10FlowRemovedMsg(){}

    COFP10Match *GetMatch()
    {
        return &m_Match;
    }
    ACE_UINT64 GetCookie() const
    {
        return m_tMember.cookie;
    }
    ACE_UINT16 GetPriority() const
    {
        return m_tMember.priority;
    }
    ACE_CDR::Octet GetReason() const
    {
        return m_tMember.reason;
    }
    ACE_UINT32 GetDurationSec() const
    {
        return m_tMember.duration_sec;
    }
    ACE_UINT32 GetDurationNsec() const
    {
        return m_tMember.duration_nsec;
    }
    ACE_UINT16 GetIdleTimeout() const
    {
        return m_tMember.idle_timeout;
    }
    ACE_UINT64 GetPacketCount() const
    {
        return m_tMember.packet_count;
    }
    ACE_UINT64 GetByteCount() const
    {
        return m_tMember.byte_count;
    }


    void SetCookie(ACE_UINT64 alCookie)
    {
        m_tMember.cookie = alCookie;
    }
    void SetPriority(ACE_UINT16 awPri)
    {
        m_tMember.priority = awPri;
    }
    void SetReason(ACE_CDR::Octet abReason)
    {
        m_tMember.reason = abReason;
    }
    void SetDurationSec(ACE_UINT32 adwSec)
    {
        m_tMember.duration_sec = adwSec;
    }
    void SetDurationNsec(ACE_UINT32 adwNsec)
    {
        m_tMember.duration_nsec = adwNsec;
    }
    void SetIdleTimeout(ACE_UINT16 awTimeout)
    {
        m_tMember.idle_timeout = awTimeout;
    }
    void SetPacketCount(ACE_UINT64 alCnt)
    {
        m_tMember.packet_count = alCnt;
    }
    void SetByteCount(ACE_UINT64 alCnt)
    {
        m_tMember.byte_count = alCnt;
    }

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + m_Match.GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    struct member
    {
        uint64_t cookie; /* Opaque controller-issued identifier. */
        ACE_UINT16 priority; /* Priority level of flow entry. */
        ACE_CDR::Octet reason; /* One of OFPRR_*. */
        ACE_CDR::Octet pad[1]; /* Align to 32-bits. */
        ACE_UINT32 duration_sec; /* Time flow was alive in seconds. */
        ACE_UINT32 duration_nsec; /* Time flow was alive in nanoseconds beyond duration_sec. */
        ACE_UINT16 idle_timeout; /* Idle timeout from original flow mod. */
        ACE_CDR::Octet pad2[2]; /* Align to 64-bits. */
        uint64_t packet_count;
        uint64_t byte_count;
    };

private:
    struct member m_tMember;
    COFP10Match   m_Match;
};



class COFP13FlowRemovedMsg : public COFPMessage
{
public:
    COFP13FlowRemovedMsg();
    virtual ~COFP13FlowRemovedMsg(){};

    inline void SetCookie(ACE_UINT64 alCookie);
    inline void SetPriority(ACE_UINT16 awPri);
    inline void SetReason(ACE_CDR::Octet abReason);
    inline void SetTableId(ACE_CDR::Octet abTableId);
    inline void SetDurationSec(ACE_UINT32 adwSec);
    inline void SetDurationNsec(ACE_UINT32 adwSec);
    inline void SetIdleTimeout(ACE_UINT16 awTime);
    inline void SetHardTimeout(ACE_UINT16 awTime);
    inline void SetPacketCount(ACE_UINT64 alCount);
    inline void SetByteCount(ACE_UINT64 alCount);
    inline COFP13Match *GetMatch();
    inline ACE_UINT64 GetCookie();
    inline ACE_UINT16 GetPriority();
    inline ACE_CDR::Octet GetReason();
    inline ACE_CDR::Octet GetTableId();
    inline ACE_UINT32 GetDurationSec();
    inline ACE_UINT32 GetDurationNsec();
    inline ACE_UINT16 GetIdleTimeout();
    inline ACE_UINT16 GetHardTimeout();
    inline ACE_UINT64 GetPacketCount();
    inline ACE_UINT64 GetByteCount();

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return 40 + m_Match.GetEncodeSpace() + COFPMessage::GetStreamLen();
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

private:
    ACE_UINT64 m_lCookie;
    ACE_UINT16 m_wPriority;
    ACE_CDR::Octet   m_bReason;
    ACE_CDR::Octet   m_bTableId;
    ACE_UINT32 m_dwDurationSec;
    ACE_UINT32 m_dwDurationNsec;
    ACE_UINT16 m_wIdleTimeout;
    ACE_UINT16 m_wHardTimeout;
    ACE_UINT64 m_lPacketCount;
    ACE_UINT64 m_lByteCount;
    COFP13Match m_Match;
};

void COFP13FlowRemovedMsg::SetCookie(ACE_UINT64 alCookie)
{
    m_lCookie = alCookie;
}

void COFP13FlowRemovedMsg::SetPriority(ACE_UINT16 awPri)
{
    m_wPriority = awPri;
}

void COFP13FlowRemovedMsg::SetReason(ACE_CDR::Octet abReason)
{
    m_bReason = abReason;
}

void COFP13FlowRemovedMsg::SetTableId(ACE_CDR::Octet abTableId)
{
    m_bTableId = abTableId;
}

void COFP13FlowRemovedMsg::SetDurationSec(ACE_UINT32 adwSec)
{
    m_dwDurationSec = adwSec;
}

void COFP13FlowRemovedMsg::SetDurationNsec(ACE_UINT32 adwSec)
{
    m_dwDurationNsec = adwSec;
}

void COFP13FlowRemovedMsg::SetIdleTimeout(ACE_UINT16 awTime)
{
    m_wIdleTimeout = awTime;
}

void COFP13FlowRemovedMsg::SetHardTimeout(ACE_UINT16 awTime)
{
    m_wHardTimeout = awTime;
}

void COFP13FlowRemovedMsg::SetPacketCount(ACE_UINT64 alCount)
{
    m_lPacketCount = alCount;
}

void COFP13FlowRemovedMsg::SetByteCount(ACE_UINT64 alCount)
{
    m_lByteCount = alCount;
}

COFP13Match *COFP13FlowRemovedMsg::GetMatch()
{
    return &m_Match;
}
ACE_UINT64 COFP13FlowRemovedMsg::GetCookie()
{
    return m_lCookie;
}

ACE_UINT16 COFP13FlowRemovedMsg::GetPriority()
{
    return m_wPriority;
}

ACE_CDR::Octet COFP13FlowRemovedMsg::GetReason()
{
    return m_bReason;
}

ACE_CDR::Octet COFP13FlowRemovedMsg::GetTableId()
{
    return m_bTableId;
}

ACE_UINT32 COFP13FlowRemovedMsg::GetDurationSec()
{
    return m_dwDurationSec;
}

ACE_UINT32 COFP13FlowRemovedMsg::GetDurationNsec()
{
    return m_dwDurationNsec;
}

ACE_UINT16 COFP13FlowRemovedMsg::GetIdleTimeout()
{
    return m_wIdleTimeout;
}

ACE_UINT16 COFP13FlowRemovedMsg::GetHardTimeout()
{
    return m_wHardTimeout;
}

ACE_UINT64 COFP13FlowRemovedMsg::GetPacketCount()
{
    return m_lPacketCount;
}

ACE_UINT64 COFP13FlowRemovedMsg::GetByteCount()
{
    return m_lByteCount;
}


#endif

