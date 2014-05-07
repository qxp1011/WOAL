/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPTableModMsg.h
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
#ifndef COFPTABLEMODMSG_H
#define COFPTABLEMODMSG_H

#include "COFPMessage.h"
#include "COFPFlowModMsg.h"

/* Configure/Modify behavior of a flow table. OFP_TABLE_MOD use the following class. */
class COFP13TableModMsg : public COFPMessage
{
public:
    /* Flags to configure the table. Reserved for future use. */
    enum ofp_table_config {
        OFPTC_DEPRECATED_MASK = 3, /* Deprecated bits */
    };

    COFP13TableModMsg()
        : COFPMessage(OFP13::OFPT_TABLE_MOD, OFP13_VERSION, 0, GetStreamLen())
        , m_bTableId(0)
        , m_nConfig(0)
     {}
    COFP13TableModMsg(
        ACE_CDR::Octet   abTableId, 
        ACE_UINT32 anConfig, 
        // args for OFP header
        ACE_UINT32 anOFPHeadXId, 
        ACE_CDR::Octet   abVersion = OFP13_VERSION)
        : COFPMessage(OFP13::OFPT_TABLE_MOD, abVersion, anOFPHeadXId, GetStreamLen())
        , m_bTableId(abTableId)
        , m_nConfig(anConfig)
    {
        ACE_ASSERT(abTableId <= OFPTT_MAX);
    }
    virtual ~COFP13TableModMsg()    {}
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const
    {
        return 16;
    }
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;
    
    inline ACE_CDR::Octet GetTableId() const
    {
        return m_bTableId;
    }

    inline ACE_UINT32 GetConfig() const
    {
        return m_nConfig;
    }

    inline void SetTableId(ACE_CDR::Octet abTableId)
    {
        m_bTableId = abTableId;
    }

    inline void SetConfig(ACE_UINT32 anConfig)
    {
        m_nConfig = anConfig;
    }

protected:
    ACE_CDR::Octet   m_bTableId;  /* ID of the table, OFPTT_ALL indicates all tables */
    ACE_UINT32 m_nConfig;   /* Bitmap of OFPTC_* flags */
};
#endif

