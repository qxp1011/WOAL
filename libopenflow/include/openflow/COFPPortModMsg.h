/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPortModMsg.h
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
#ifndef COFPPORTMODMSG_H
#define COFPPORTMODMSG_H

#include "COFPMessage.h"

class COFP10PortModMsg : public COFPMessage
{
public:
    COFP10PortModMsg(ACE_CDR::Octet abVersion = OFP10_VERSION)
        : COFPMessage(OFP10::OFPT_PORT_MOD, abVersion)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
    }
    virtual ~COFP10PortModMsg() {}

    ACE_UINT16 GetPortNo() const
    {
        return m_tMember.port_no;
    }
    const ACE_CDR::Octet *GetHwAddr() const
    {
        return m_tMember.hw_addr;
    }
    ACE_UINT32 GetConfig() const
    {
        return m_tMember.config;
    }
    ACE_UINT32 GetMask() const
    {
        return m_tMember.mask;
    }
    ACE_UINT32 GetAdvertise() const
    {
        return m_tMember.advertise;
    }

    void SetPortNo(ACE_UINT16 awPortNo)
    {
        m_tMember.port_no = awPortNo;
    }
    void SetHwAddr(ACE_CDR::Octet aacAddr[OFP_ETH_ALEN])
    {
        ::memcpy(m_tMember.hw_addr, aacAddr, OFP_ETH_ALEN);
    }
    void SetConfig(ACE_UINT32 adwCfg)
    {
        m_tMember.config = adwCfg;
    }
    void SetMask(ACE_UINT32 adwMask)
    {
        m_tMember.mask = adwMask;
    }
    void SetAdvertise(ACE_UINT32 adwAdvertise)
    {
        m_tMember.advertise = adwAdvertise;
    }
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + sizeof m_tMember;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
protected:
    struct member
    {
        ACE_UINT16 port_no;
        ACE_CDR::Octet hw_addr[OFP_ETH_ALEN]; /* The hardware address is not configurable. This is used to
                                                                  sanity-check the request, so it must be the same as 
                                                                  returned in an ofp_phy_port struct. */
        ACE_UINT32 config; /* Bitmap of OFPPC_* flags. */
        ACE_UINT32 mask; /* Bitmap of OFPPC_* flags to be changed. */
        ACE_UINT32 advertise; /* Bitmap of "ofp_port_features"s. Zero all bits to prevent any action taking place. */
        ACE_CDR::Octet pad[4]; /* Pad to 64-bits. */
    };

private:
    struct member m_tMember;
};




class COFP13PortModMsg : public COFPMessage
{
public:
    COFP13PortModMsg(ACE_UINT32 adwPortNo = 0);
    virtual ~COFP13PortModMsg(){};

    inline void SetPortNo(ACE_UINT32 adwPortNo);
    inline void SetHwAddr(ACE_CDR::Octet aacHwAddr[OFP_ETH_ALEN]);
    inline void SetConfig(ACE_UINT32 adwCfg);
    inline void SetMask(ACE_UINT32 adwMask);
    inline void SetAdvertise(ACE_UINT32 adwAdver);
    inline ACE_UINT32 GetPortNo();
    inline const ACE_CDR::Octet *GetHwAddr();
    inline ACE_UINT32 GetConfig();
    inline ACE_UINT32 GetMask();
    inline ACE_UINT32 GetAdvertise();
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const;
    virtual void         Dump()              const;

private:
    ACE_UINT32 m_dwPortNo;
    ACE_CDR::Octet   m_acHwAddr[OFP_ETH_ALEN];
    ACE_UINT32 m_dwConfig;
    ACE_UINT32 m_dwMask;
    ACE_UINT32 m_dwAdvertise;
};

void COFP13PortModMsg::SetPortNo(ACE_UINT32 adwPortNo)
{
    m_dwPortNo = adwPortNo;
}

void COFP13PortModMsg::SetHwAddr(ACE_CDR::Octet aacHwAddr[OFP_ETH_ALEN])
{
    ::memcpy(m_acHwAddr, aacHwAddr, OFP_ETH_ALEN);
}

void COFP13PortModMsg::SetConfig(ACE_UINT32 adwCfg)
{
    m_dwConfig = adwCfg;
}

void COFP13PortModMsg::SetMask(ACE_UINT32 adwMask)
{
    m_dwMask = adwMask;
}

void COFP13PortModMsg::SetAdvertise(ACE_UINT32 adwAdver)
{
    m_dwAdvertise = adwAdver;
}

ACE_UINT32 COFP13PortModMsg::GetPortNo()
{
    return m_dwPortNo;
}

const ACE_CDR::Octet *COFP13PortModMsg::GetHwAddr()
{
    return m_acHwAddr;
}

ACE_UINT32 COFP13PortModMsg::GetConfig()
{
    return m_dwConfig;
}

ACE_UINT32 COFP13PortModMsg::GetMask()
{
    return m_dwMask;
}

ACE_UINT32 COFP13PortModMsg::GetAdvertise()
{
    return m_dwAdvertise;
}

inline ACE_UINT16 COFP13PortModMsg::GetStreamLen() const
{
    return (ACE_UINT16)40; 
}

inline ACE_UINT16 COFP13PortModMsg::GetEncodeSpace() const
{
    return GetStreamLen();
}

#endif

