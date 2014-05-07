/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFP10Action.h
 * Description:      Header file for openflow 1.0 action.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-12
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFP10ACTION_H
#define COFP10ACTION_H

#include "IOFPMsgParser.h"
#include "openflow-common.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

class COFP10Action : public IOFPMsgParser
{
public:
    enum ofp_action_type {
        OFPAT_OUTPUT, /* Output to switch port. */
        OFPAT_SET_VLAN_VID, /* Set the 802.1q VLAN id. */
        OFPAT_SET_VLAN_PCP, /* Set the 802.1q priority. */
        OFPAT_STRIP_VLAN, /* Strip the 802.1q header. */
        OFPAT_SET_DL_SRC, /* Ethernet source address. */
        OFPAT_SET_DL_DST, /* Ethernet destination address. */
        OFPAT_SET_NW_SRC, /* IP source address. */
        OFPAT_SET_NW_DST, /* IP destination address. */
        OFPAT_SET_NW_TOS, /* IP ToS (DSCP field, 6 bits). */
        OFPAT_SET_TP_SRC, /* TCP/UDP source port. */
        OFPAT_SET_TP_DST, /* TCP/UDP destination port. */
        OFPAT_ENQUEUE, /* Output to queue. */
        OFPAT_VENDOR = 0xffff
    };

public:
    COFP10Action(ACE_UINT16 awType, ACE_UINT16 awLen = 0)
        : m_wType(awType)
        , m_wLen(0)
    {
        CM_ASSERT_RETURN_VOID((0 == awLen) || (awLen >= 8));
        if (awLen != 0)
        {
            CM_ASSERT_RETURN_VOID(0 == (awLen % 8));
            m_wLen = awLen;
        }
    }
    virtual ~COFP10Action() {}
    
    ACE_UINT16 GetActionType() const
    {
        return m_wType;
    }
    ACE_UINT16 GetActionLen() const
    {
        return m_wLen;
    }
    
    void SetActionType(ACE_UINT16 awType)
    {
        m_wType = awType;
    }
    
    static CmResult DecodeAction(ACE_InputCDR &is, COFP10Action * &apAction);

public:
    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16 GetStreamLen()                             const
    {
        return (sizeof(m_wType) + sizeof(m_wLen));
    }
    virtual ACE_UINT16 GetEncodeSpace()                           const
    {
        return GetStreamLen() + 4 * sizeof(ACE_CDR::Octet);  // or only GetStreamLen()?
    }
    virtual void     Dump()                                     const;
    
protected:
    ACE_UINT16 m_wType; /* One of OFPAT_*. */
    ACE_UINT16 m_wLen;  /* Length of action, including this header. This is the length of action,
                           including any padding to make it 64-bit aligned. */
};


class COFP10ActOnlyHeader : public COFP10Action
{
public:
    COFP10ActOnlyHeader(ACE_UINT16 awActType)
        : COFP10Action(awActType, GetStreamLen())
    {
    }
    virtual ~COFP10ActOnlyHeader()  {}

    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);    
    virtual ACE_UINT16 GetStreamLen() const
    {
        return COFP10Action::GetStreamLen() + 4 * sizeof(ACE_CDR::Octet);
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
};


class COFP10ActOutput : public COFP10Action
{
public:
    enum ofp_controller_max_len {
        OFPCML_MAX       = 0xffe5, /* maximum max_len value which can be used to request a specific byte length. */
        OFPCML_NO_BUFFER = 0xffff  /* indicates that no buffering should be applied and the whole packet is to be
                                                           sent to the controller. */
    };
    
    COFP10ActOutput(ACE_UINT16 awOutport = 0, ACE_UINT16 awMaxLen = OFPCML_NO_BUFFER)
        : COFP10Action(OFPAT_OUTPUT, GetStreamLen())
        , m_wOutPort(awOutport)
        , m_wMaxLen(awMaxLen)
    {
    }
    virtual ~COFP10ActOutput()  {}

    ACE_UINT16 GetOutPort() const
    {
        return m_wOutPort;
    }
    ACE_UINT16 GetMaxLen() const
    {
        return m_wMaxLen;
    }
    void SetOutPort(ACE_UINT16 awPortNo)
    {
        m_wOutPort = awPortNo;
    }
    void SetMaxLen(ACE_UINT16 awLen)
    {
        m_wMaxLen = awLen;
    }

public:
    //for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);    
    virtual ACE_UINT16 GetStreamLen() const
    {
        return COFP10Action::GetStreamLen() + sizeof(m_wOutPort) + sizeof(m_wMaxLen);
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;
    
private:
    ACE_UINT16 m_wOutPort;     /* Output port. */
    ACE_UINT16 m_wMaxLen;      /* Max length to send to controller. */
};


class COFP10ActEnqueue : public COFP10Action
{
public:
    COFP10ActEnqueue(ACE_UINT16 awPortNo = 0, ACE_UINT32 adwQueueId = 0)
        : COFP10Action(OFPAT_ENQUEUE, GetStreamLen())
        , m_wPort(awPortNo)
        , m_dwQueueId(adwQueueId)
    {
    }
    virtual ~COFP10ActEnqueue(){}

    ACE_UINT16 GetPortNum() const
    {
        return m_wPort;
    }
    ACE_UINT32 GetQueueId() const
    {
        return m_dwQueueId;
    }
    void SetPortNum(ACE_UINT16 awPortNo)
    {
        m_wPort = awPortNo;
    }
    void SetQueueId(ACE_UINT32 adwId)
    {
        m_dwQueueId = adwId;
    }

public:
    //for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);    
    virtual ACE_UINT16 GetStreamLen() const
    {
        return 16;
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;

private:
    ACE_UINT16 m_wPort;     /* Port that queue belongs. Should refer to a valid physical 
                               port (i.e. < OFPP_MAX) or OFPP_IN_PORT. */
    ACE_UINT32 m_dwQueueId; /* Where to enqueue the packets. */
};

#define COFP10_NO_VLAN_ID (0xffff)

class COFP10ActSetVlanVid : public COFP10Action
{
public:
    COFP10ActSetVlanVid(ACE_UINT16 awVlan = COFP10_NO_VLAN_ID)
        : COFP10Action(OFPAT_SET_VLAN_VID, GetStreamLen())
        , m_wVlanVid(awVlan)
    {
    }
    virtual ~COFP10ActSetVlanVid()  {}
    
    ACE_UINT16 GetVlanVid() const
    {
        return m_wVlanVid;
    }
    void SetVlanVid(ACE_UINT16 awVlan)
    {
        m_wVlanVid = awVlan;
    }
    
public:
    //for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16 GetStreamLen() const
    {
        return 8;
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;
    
private:
    ACE_UINT16 m_wVlanVid;
};


class COFP10ActSetVlanPcp : public COFP10Action
{
public:
    COFP10ActSetVlanPcp(ACE_CDR::Octet abVlanPcp = 0)
        : COFP10Action(OFPAT_SET_VLAN_PCP, GetStreamLen())
        , m_bVlanPcp(abVlanPcp)
    {
    }
    virtual ~COFP10ActSetVlanPcp()  {}
    
    ACE_CDR::Octet GetVlanPcp() const
    {
        return m_bVlanPcp;
    }
    void SetVlanPcp(ACE_CDR::Octet abVlanPcp)
    {
        m_bVlanPcp = abVlanPcp;
    }
    
public:
    //for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16 GetStreamLen() const
    {
        return 8;
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;
    
private:
    ACE_CDR::Octet m_bVlanPcp;
};


class COFP10ActStripVlan : public COFP10ActOnlyHeader
{
public:
    COFP10ActStripVlan()
        : COFP10ActOnlyHeader(COFP10Action::OFPAT_STRIP_VLAN)
    {
    }
    virtual ~COFP10ActStripVlan()   {};
};


class COFP10ActSetDlAddr : public COFP10Action
{
public:
    COFP10ActSetDlAddr(ACE_UINT16 awType, ACE_CDR::Octet *aacAddr = NULL)
        : COFP10Action(awType, GetStreamLen())
    {
        CM_ASSERT_RETURN_VOID((OFPAT_SET_DL_SRC == awType) 
                              || (OFPAT_SET_DL_DST == awType));
        if (NULL == aacAddr)
        {
            ::memset(m_acDlAddr, 0, sizeof(m_acDlAddr));
        }
        else
        {
            ::memcpy(m_acDlAddr, aacAddr, sizeof(m_acDlAddr));
        }
    }
    virtual ~COFP10ActSetDlAddr()   {}
    
    const ACE_CDR::Octet *GetDlAddr() const
    {
        return m_acDlAddr;
    }
    void SetDlAddr(ACE_CDR::Octet aacAddr[OFP_ETH_ALEN])
    {
        ::memcpy(m_acDlAddr, aacAddr, sizeof(m_acDlAddr));
    }
    
public:
    //for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16 GetStreamLen() const
    {
        return 16;
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;
    
private:
    ACE_CDR::Octet m_acDlAddr[OFP_ETH_ALEN];
};


class COFP10ActSetNwAddr : public COFP10Action
{
public:
    COFP10ActSetNwAddr(ACE_UINT16 awType, ACE_UINT32 adwAddr = 0)
        : COFP10Action(awType, GetStreamLen())
        , m_dwNwAddr(adwAddr)
    {
        CM_ASSERT_RETURN_VOID((OFPAT_SET_NW_SRC == awType) 
                               || (OFPAT_SET_NW_DST == awType));
    }
    virtual ~COFP10ActSetNwAddr()   {}

    ACE_UINT32 GetNwAddr() const
    {
        return m_dwNwAddr;
    }
    void SetNwAddr(ACE_UINT32 adwAddr)
    {
        m_dwNwAddr = adwAddr;
    }

public:
    //for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16 GetStreamLen() const
    {
        return 8;
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;
    
private:
    ACE_UINT32 m_dwNwAddr;
};


class COFP10ActSetNwTos : public COFP10Action
{
public:
    COFP10ActSetNwTos(ACE_CDR::Octet abTos = 0)
        : COFP10Action(OFPAT_SET_NW_TOS, GetStreamLen())
        , m_bNwTos(abTos)
    {
    }
    virtual ~COFP10ActSetNwTos()    {}

    ACE_CDR::Octet GetNwTos() const
    {
        return m_bNwTos;
    }
    void SetNwTos(ACE_CDR::Octet abTos)
    {
        m_bNwTos = abTos;
    }
    
public:
    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16 GetStreamLen() const
    {
        return 8;
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;
    
private:
    ACE_CDR::Octet m_bNwTos;
};


class COFP10ActSetTpPort : public COFP10Action
{
public:
    COFP10ActSetTpPort(ACE_CDR::Octet abType, ACE_UINT16 awPort = 0)
        : COFP10Action(abType, GetStreamLen())
        , m_wTpPort(awPort)
    {
        CM_ASSERT_RETURN_VOID((OFPAT_SET_TP_SRC == abType) 
                               || (OFPAT_SET_TP_DST == abType));
    }
    virtual ~COFP10ActSetTpPort()   {}

    ACE_UINT16 GetTpPort() const
    {
        return m_wTpPort;
    }
    void SetTpPort(ACE_UINT16 awPort)
    {
        m_wTpPort = awPort;
    }
    
public:
    //for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16 GetStreamLen() const
    {
        return 8;
    }
    virtual ACE_UINT16 GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void Dump() const;
    
private:
    ACE_UINT16 m_wTpPort;
};

#endif

