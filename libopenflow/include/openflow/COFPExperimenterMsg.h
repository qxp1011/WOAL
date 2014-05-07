/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPExperimenterMsg.h
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
#ifndef COFPEXPERIMENTERMSG_H
#define COFPEXPERIMENTERMSG_H

#include "COFPMessage.h"
#include "OFPMessageUtils.h"

class COFP13ExperimenterMsg : public COFPMessage
{
public:
    COFP13ExperimenterMsg(
        ACE_UINT16      awAdditionalDataSize = 0,
        const ACE_CDR::Octet *apAdditionalData     = NULL,
        ACE_UINT32      anExperimenter       = 0, 
        ACE_UINT32      anExpType            = 0,
        // args for OFP header
        ACE_UINT32      anXid                = 0,
        ACE_CDR::Octet        abVersion            = OFP13_VERSION);
    virtual ~COFP13ExperimenterMsg();
    
    inline ACE_UINT32 GetExperimenter() const 
    { 
        return m_Experimenter;
    }
    inline ACE_UINT32 GetExpType() const 
    { 
        return m_ExpType;
    }
    inline void GetAdditionalData(ACE_CDR::Octet *&data, ACE_UINT16 &size)
    {
        size = m_wUsedDataSize;
        data = m_pAdditionalData;
    }

    inline void SetExperimenter(ACE_UINT32 aExperimenter)
    {
        m_Experimenter = aExperimenter;
    }
    inline void SetExpType(ACE_UINT32 aExpType)
    {
        m_ExpType = aExpType;
    }
    void SetAdditionalData(const ACE_CDR::Octet *data, ACE_UINT16 size);

    friend CmResult COFPMsgMemoryMgr::Copy(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, 
                                           ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize,
                                           const ACE_CDR::Octet *apSrc, ACE_UINT16 awSrcSize);

protected:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const 
    {
        return sizeof(m_Experimenter)+sizeof(m_ExpType)+m_wUsedDataSize+COFPMessage::GetStreamLen();
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
private:
    ACE_UINT32 m_Experimenter;
    ACE_UINT32 m_ExpType;
    static std::allocator<ACE_CDR::Octet> m_alloc;
    ACE_CDR::Octet  *m_pAdditionalData;
    ACE_UINT16 m_wUsedDataSize;
    ACE_UINT16 m_wTotalDataSize;
};

#endif

