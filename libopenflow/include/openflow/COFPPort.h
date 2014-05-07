/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPort.h
 * Description:      OpenFlow port header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFP_PORT_H
#define COFP_PORT_H

#include "IOFPMsgParser.h"
#include "openflow-common.h"


class COFP10PhyPort : public IOFPMsgParser
{
public:
    /* Port numbering. Physical ports are numbered starting from 1. */
    enum ofp_port {
        /* Maximum number of physical switch ports. */
        OFPP_MAX        = 0xff00,
        /* Fake output "ports". */
        OFPP_IN_PORT    = 0xfff8, /* Send the packet out the input port. This virtual port
                                                     must be explicitly used in order to send back out of the input port*/
        OFPP_TABLE      = 0xfff9, /* Perform actions in flow table.
                                                  NB: This can only be the destination port for packet-out messages. */
        OFPP_NORMAL     = 0xfffa, /* Process with normal L2/L3 switching. */
        OFPP_FLOOD      = 0xfffb, /* All physical ports except input port and those disabled by STP. */
        OFPP_ALL        = 0xfffc, /* All physical ports except input port. */
        OFPP_CONTROLLER = 0xfffd, /* Send to controller. */
        OFPP_LOCAL      = 0xfffe, /* Local openflow "port". */
        OFPP_NONE       = 0xffff  /* Not associated with a physical port. */
    };
    
    /* Flags to indicate behavior of the physical port. These flags are
        * used in ofp_phy_port to describe the current configuration. They are
        * used in the ofp_port_mod message to configure the port's behavior.
        */
    enum ofp_port_config {
        OFPPC_PORT_DOWN    = 1 << 0, /* Port is administratively down. */
        OFPPC_NO_STP       = 1 << 1, /* Disable 802.1D spanning tree on port. */
        OFPPC_NO_RECV      = 1 << 2, /* Drop all packets except 802.1D spanning tree packets. */
        OFPPC_NO_RECV_STP  = 1 << 3, /* Drop received 802.1D STP packets. */
        OFPPC_NO_FLOOD     = 1 << 4, /* Do not include this port when flooding. */
        OFPPC_NO_FWD       = 1 << 5, /* Drop packets forwarded to port. */
        OFPPC_NO_PACKET_IN = 1 << 6  /* Do not send packet-in msgs for port. */
    };

    /* Current state of the physical port. These are not configurable from
        * the controller.
        */
    enum ofp_port_state {
        OFPPS_LINK_DOWN = 1 << 0, /* No physical link present. */
        /* The OFPPS_STP_* bits have no effect on switch operation. The
            * controller must adjust OFPPC_NO_RECV, OFPPC_NO_FWD, and
            * OFPPC_NO_PACKET_IN appropriately to fully implement an 802.1D spanning
            * tree. */
        OFPPS_STP_LISTEN = 0 << 8, /* Not learning or relaying frames. */
        OFPPS_STP_LEARN = 1 << 8, /* Learning but not relaying frames. */
        OFPPS_STP_FORWARD = 2 << 8, /* Learning and relaying frames. */
        OFPPS_STP_BLOCK = 3 << 8, /* Not part of spanning tree. */
        OFPPS_STP_MASK = 3 << 8 /* Bit mask for OFPPS_STP_* values. */
    };
    
    /* Features of physical ports available in a datapath. */
    enum ofp_port_features {
        OFPPF_10MB_HD = 1 << 0, /* 10 Mb half-duplex rate support. */
        OFPPF_10MB_FD = 1 << 1, /* 10 Mb full-duplex rate support. */
        OFPPF_100MB_HD = 1 << 2, /* 100 Mb half-duplex rate support. */
        OFPPF_100MB_FD = 1 << 3, /* 100 Mb full-duplex rate support. */
        OFPPF_1GB_HD = 1 << 4, /* 1 Gb half-duplex rate support. */
        OFPPF_1GB_FD = 1 << 5, /* 1 Gb full-duplex rate support. */
        OFPPF_10GB_FD = 1 << 6, /* 10 Gb full-duplex rate support. */
        OFPPF_COPPER = 1 << 7, /* Copper medium. */
        OFPPF_FIBER = 1 << 8, /* Fiber medium. */
        OFPPF_AUTONEG = 1 << 9, /* Auto-negotiation. */
        OFPPF_PAUSE = 1 << 10, /* Pause. */
        OFPPF_PAUSE_ASYM = 1 << 11 /* Asymmetric pause. */
    };

public:
    COFP10PhyPort();
    virtual ~COFP10PhyPort()    {}
    
    ACE_UINT16      GetPortNum() const
    {
        return m_wPortNum;
    }
    const ACE_CDR::Octet *GetPortHWAddr() const
    {
        return m_bHWAddr;
    }
    const char *GetPortName() const
    {
        return m_acName;
    }
    ACE_UINT32      GetPortNameLen() const
    {
        return ::strlen(m_acName);
    }
    ACE_UINT32      GetPortConfig() const
    {
        return m_nConfig;
    }
    ACE_UINT32      GetPortState() const
    {
        return m_nState;
    }
    ACE_UINT32      GetPortCurr() const
    {
        return m_nCurr;
    }
    ACE_UINT32      GetPortAdvertised() const
    {
        return m_nAdvertised;
    }
    ACE_UINT32      GetPortSupported() const
    {
        return m_nSupported;
    }
    ACE_UINT32      GetPortPeer() const
    {
        return m_nPeer;
    }
    
    void SetPortNum(ACE_UINT16 anPortNum)
    {
        m_wPortNum = anPortNum;
    }
    void SetHWAddr(const ACE_CDR::Octet *apHWAddr, ACE_UINT16 awHWAddrLen);
    void SetName(const char *apName, ACE_UINT16 awLen);
    void SetConfig(ACE_UINT32 anConfig)
    {
        m_nConfig = anConfig;
    }
    void SetState(ACE_UINT32 anState)
    {
        m_nState = anState;
    }
    void SetCurr(ACE_UINT32 anCurr)
    {
        m_nCurr = anCurr;
    }
    void SetAdvertised(ACE_UINT32 anAdvertised)
    {
        m_nAdvertised = anAdvertised;
    }
    void SetSupported(ACE_UINT32 anSupported)
    {
        m_nSupported = anSupported;
    }
    void SetPeer(ACE_UINT32 anPeer)
    {
        m_nPeer = anPeer;
    }
    
public:
    static CmResult DecodePort(ACE_InputCDR &is, COFP10PhyPort * &aPort);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const
    {
        return 48;
    }
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const
    {
    }

private:
    ACE_UINT16 m_wPortNum;
    ACE_CDR::Octet   m_bHWAddr[OFP_ETH_ALEN];
    char   m_acName[OFP_MAX_PORT_NAME_LEN]; /* Null-terminated */
    ACE_UINT32 m_nConfig;                       /* Bitmap of OFPPC_* flags. */
    ACE_UINT32 m_nState;                        /* Bitmap of OFPPS_* flags. */
    /* Bitmaps of OFPPF_* that describe features. All bits zeroed if unsupported or unavailable. */
    ACE_UINT32 m_nCurr;                         /* Current features. */
    ACE_UINT32 m_nAdvertised;                   /* Features being advertised by the port. */
    ACE_UINT32 m_nSupported;                    /* Features supported by the port. */
    ACE_UINT32 m_nPeer;                         /* Features advertised by peer. */
};



/* Description of a port */
class COFP13Port : public IOFPMsgParser
{
public:
    /* Port numbering. Ports are numbered starting from 1. */
    enum ofp_port_no {
        /* Maximum number of physical and logical switch ports. */
        OFPP_MAX        = 0xffffff00,
        /* Reserved OpenFlow Port (fake output "ports"). */
        OFPP_IN_PORT    = 0xfffffff8, /* Send the packet out the input port. This reserved port must be explicitly used
                                         in order to send back out of the input port. */
        OFPP_TABLE      = 0xfffffff9, /* Submit the packet to the first flow table
                                         NB: This destination port can only be used in packet-out messages. */
        OFPP_NORMAL     = 0xfffffffa, /* Process with normal L2/L3 switching. */
        OFPP_FLOOD      = 0xfffffffb, /* All physical ports in VLAN, except input port and those blocked or link down. */
        OFPP_ALL        = 0xfffffffc, /* All physical ports except input port. */
        OFPP_CONTROLLER = 0xfffffffd, /* Send to controller. */
        OFPP_LOCAL      = 0xfffffffe, /* Local openflow "port". */
        OFPP_ANY        = 0xffffffff  /* Wildcard port used only for flow mod (delete) and flow stats requests. Selects
                                         all flows regardless of output port (including flows with no output port). */
    };
    
   /* Flags to indicate behavior of the physical port. These flags are
      * used in ofp_port to describe the current configuration. They are
      * used in the ofp_port_mod message to configure the port's behavior.
      */
    enum ofp_port_config {
        OFPPC_PORT_DOWN    = 1 << 0, /* Port is administratively down. */
        OFPPC_NO_RECV      = 1 << 2, /* Drop all packets received by port. */
        OFPPC_NO_FWD       = 1 << 5, /* Drop packets forwarded to port. */
        OFPPC_NO_PACKET_IN = 1 << 6  /* Do not send packet-in msgs for port. */
    };

   /* Current state of the physical port. These are not configurable from
      * the controller.
      */
    enum ofp_port_state {
        OFPPS_LINK_DOWN = 1 << 0, /* No physical link present. */
        OFPPS_BLOCKED   = 1 << 1, /* Port is blocked */
        OFPPS_LIVE      = 1 << 2, /* Live for Fast Failover Group. */
    };

    /* Features of ports available in a datapath. */
    enum ofp_port_features {
        OFPPF_10MB_HD    = 1 << 0, /* 10 Mb half-duplex rate support. */
        OFPPF_10MB_FD    = 1 << 1, /* 10 Mb full-duplex rate support. */
        OFPPF_100MB_HD   = 1 << 2, /* 100 Mb half-duplex rate support. */
        OFPPF_100MB_FD   = 1 << 3, /* 100 Mb full-duplex rate support. */
        OFPPF_1GB_HD     = 1 << 4, /* 1 Gb half-duplex rate support. */
        OFPPF_1GB_FD     = 1 << 5, /* 1 Gb full-duplex rate support. */
        OFPPF_10GB_FD    = 1 << 6, /* 10 Gb full-duplex rate support. */
        OFPPF_40GB_FD    = 1 << 7, /* 40 Gb full-duplex rate support. */
        OFPPF_100GB_FD   = 1 << 8, /* 100 Gb full-duplex rate support. */
        OFPPF_1TB_FD     = 1 << 9, /* 1 Tb full-duplex rate support. */
        OFPPF_OTHER      = 1 << 10, /* Other rate, not in the list. */
        OFPPF_COPPER     = 1 << 11, /* Copper medium. */
        OFPPF_FIBER      = 1 << 12, /* Fiber medium. */
        OFPPF_AUTONEG    = 1 << 13, /* Auto-negotiation. */
        OFPPF_PAUSE      = 1 << 14, /* Pause. */
        OFPPF_PAUSE_ASYM = 1 << 15  /* Asymmetric pause. */
    };

    COFP13Port();
    COFP13Port(
          ACE_UINT32 anPortNum
        , ACE_CDR::Octet  *apHWAddrVal
        , ACE_UINT32 anHWAddrLen
        , char  *apNameVal
        , ACE_UINT32 anNameLen
        , ACE_UINT32 anConfig                       /* Bitmap of OFPPC_* flags. */
        , ACE_UINT32 anState                        /* Bitmap of OFPPS_* flags. */
        , ACE_UINT32 anCurr                         /* Current features. */
        , ACE_UINT32 anAdvertised                   /* Features being advertised by the port. */
        , ACE_UINT32 anSupported                    /* Features supported by the port. */
        , ACE_UINT32 anPeer                         /* Features advertised by peer. */
        , ACE_UINT32 anCurrSpeed                    /* Current port bitrate in kbps. */
        , ACE_UINT32 anMaxSpeed);                   /* Max port bitrate in kbps */
    COFP13Port(const ACE_CDR::Octet *apSrc, ACE_UINT32 anLen);  // use the specified memory to initialize COFP13Port
    virtual ~COFP13Port() {}

    ACE_UINT32      GetPortNum()                                    const;
    const ACE_CDR::Octet *GetPortHWAddr()                                 const;
    const char *GetPortName()                                   const;
    ACE_UINT32      GetPortNameLen()                                const;
    ACE_UINT32      GetPortConfig()                                 const;
    ACE_UINT32      GetPortState()                                  const;
    ACE_UINT32      GetPortCurr()                                   const;
    ACE_UINT32      GetPortAdvertised()                             const;
    ACE_UINT32      GetPortSupported()                              const;
    ACE_UINT32      GetPortPeer()                                   const;
    ACE_UINT32      GetPortCurrSpeed()                              const;
    ACE_UINT32      GetPortMaxSpeed()                               const;
    
    void SetPortNum(ACE_UINT32 anPortNum);
    void SetHWAddr(const ACE_CDR::Octet *apHWAddr, ACE_UINT16 awHWAddrLen);
    void SetName(const char *apName, ACE_UINT16 awLen);
    void SetConfig(ACE_UINT32 anConfig);
    void SetState(ACE_UINT32 anState);
    void SetCurr(ACE_UINT32 anCurr);
    void SetAdvertised(ACE_UINT32 anAdvertised);
    void SetSupported(ACE_UINT32 anSupported);
    void SetPeer(ACE_UINT32 anPeer);
    void SetCurrSpeed(ACE_UINT32 anCurrSpeed);
    void SetMaxSpeed(ACE_UINT32 anMaxSpeed);

    static CmResult DecodePort(ACE_InputCDR &is, COFP13Port * &apPort);

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

protected:
    ACE_UINT32 m_nPortNum;
    ACE_CDR::Octet   m_bHWAddr[OFP_ETH_ALEN];
    char   m_acName[OFP_MAX_PORT_NAME_LEN]; /* Null-terminated */
    ACE_UINT32 m_nConfig;                       /* Bitmap of OFPPC_* flags. */
    ACE_UINT32 m_nState;                        /* Bitmap of OFPPS_* flags. */
    /* Bitmaps of OFPPF_* that describe features. All bits zeroed if unsupported or unavailable. */
    ACE_UINT32 m_nCurr;                         /* Current features. */
    ACE_UINT32 m_nAdvertised;                   /* Features being advertised by the port. */
    ACE_UINT32 m_nSupported;                    /* Features supported by the port. */
    ACE_UINT32 m_nPeer;                         /* Features advertised by peer. */
    ACE_UINT32 m_nCurrSpeed;                    /* Current port bitrate in kbps. */
    ACE_UINT32 m_nMaxSpeed;                     /* Max port bitrate in kbps */
};

inline ACE_UINT32 COFP13Port::GetPortNum()                          const
{
    return m_nPortNum;
}

inline const ACE_CDR::Octet *COFP13Port::GetPortHWAddr()                  const
{
    return m_bHWAddr;
}

inline const char *COFP13Port::GetPortName()                    const
{
    return m_acName;
}

inline ACE_UINT32 COFP13Port::GetPortNameLen()                      const
{
    return ::strlen(m_acName);
}

inline ACE_UINT32 COFP13Port::GetPortConfig()                       const
{
    return m_nConfig;
}

inline ACE_UINT32 COFP13Port::GetPortState()                        const
{
    return m_nState;
}

inline ACE_UINT32 COFP13Port::GetPortCurr()                         const
{
    return m_nCurr;
}

inline ACE_UINT32 COFP13Port::GetPortAdvertised()                   const
{
    return m_nAdvertised;
}

inline ACE_UINT32 COFP13Port::GetPortSupported()                    const
{
    return m_nSupported;
}

inline ACE_UINT32 COFP13Port::GetPortPeer()                         const
{
    return m_nPeer;
}

inline ACE_UINT32 COFP13Port::GetPortCurrSpeed()                    const
{
    return m_nCurrSpeed;
}

inline ACE_UINT32 COFP13Port::GetPortMaxSpeed()                     const
{
    return m_nMaxSpeed;
}

inline void COFP13Port::SetPortNum(ACE_UINT32 anPortNum)
{
    m_nPortNum = anPortNum;
}

inline void COFP13Port::SetConfig(ACE_UINT32 anConfig)
{
    m_nConfig = anConfig;
}

inline void COFP13Port::SetState(ACE_UINT32 anState)
{
    m_nState = anState;
}

inline void COFP13Port::SetCurr(ACE_UINT32 anCurr)
{
    m_nCurr = anCurr;
}

inline void COFP13Port::SetAdvertised(ACE_UINT32 anAdvertised)
{
    m_nAdvertised = anAdvertised;
}

inline void COFP13Port::SetSupported(ACE_UINT32 anSupported)
{
    m_nSupported = anSupported;
}

inline void COFP13Port::SetPeer(ACE_UINT32 anPeer)
{
    m_nPeer = anPeer;
}

inline void COFP13Port::SetCurrSpeed(ACE_UINT32 anCurrSpeed)
{
    m_nCurrSpeed = anCurrSpeed;
}

inline void COFP13Port::SetMaxSpeed(ACE_UINT32 anMaxSpeed)
{
    m_nMaxSpeed = anMaxSpeed;
}

// for IOFPMsgParser
inline ACE_UINT16 COFP13Port::GetStreamLen()    const
{
    return 64;
}

inline ACE_UINT16 COFP13Port::GetEncodeSpace()  const
{
    return GetStreamLen();
}

#endif


