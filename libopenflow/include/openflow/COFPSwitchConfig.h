/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPSwitchConfig.h
 * Description:      OpenFlow switch config header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-15
 *
 * History 1:
 *     Date:         
 *     Version:      
 *     Author:       
 *     Modification:
**********************************************************************/
#ifndef COFP_SWITCH_CONFIG_H
#define COFP_SWITCH_CONFIG_H

#include "COFPMessage.h"

/* The OFPT_SET_CONFIG and OFPT_GET_CONFIG_REPLY use the following: */
class COFPSwitchConfig : public COFPMessage
{
public:
    enum ofp_config_flags {
        /* Handling of IP fragments. */
        OFPC_FRAG_NORMAL = 0,      /* No special handling for fragments. */
        OFPC_FRAG_DROP   = 1 << 0, /* Drop fragments. */
        OFPC_FRAG_REASM  = 1 << 1, /* Reassemble (only if OFPC_IP_REASM set). */
        OFPC_FRAG_MASK   = 3,
    };

    COFPSwitchConfig(ACE_CDR::Octet abType, 
                                        ACE_CDR::Octet abVersion = OFP10_VERSION, 
                                        ACE_UINT16 awFlags = 0, 
                                        ACE_UINT16 awMissSendLen = 0xffff);
    
    virtual ~COFPSwitchConfig()   {}

    inline ACE_UINT16 GetFlags()                                    const;
    inline ACE_UINT16 GetMissSendLen()                              const;
    inline void   SetFlags(ACE_UINT16 awFlags);
    inline void   SetMissSendLen(ACE_UINT16 awMissSendLen);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

protected:
    ACE_UINT16 m_wFlags;       // OFPC_* flags.
    ACE_UINT16 m_wMissSendLen; /* Max bytes of packet that datapath should send to the controller. 
                              See ofp_controller_max_len for valid values. */
};


inline ACE_UINT16 COFPSwitchConfig::GetFlags()                    const
{
    return m_wFlags;
}


inline ACE_UINT16 COFPSwitchConfig::GetMissSendLen()              const
{
    return m_wMissSendLen;
}


inline void COFPSwitchConfig::SetFlags(ACE_UINT16 awFlags) 
{
    m_wFlags = awFlags;
}


inline void COFPSwitchConfig::SetMissSendLen(ACE_UINT16 awMissSendLen) 
{
    m_wMissSendLen = awMissSendLen;
}


inline ACE_UINT16 COFPSwitchConfig::GetStreamLen()                const
{
    return 12;
}


inline ACE_UINT16 COFPSwitchConfig::GetEncodeSpace()              const
{
    return GetStreamLen();
}

#endif

