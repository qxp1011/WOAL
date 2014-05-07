/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPAsyncConfig.h
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
#ifndef COFPASYNCCONFIG_H
#define COFPASYNCCONFIG_H

#include "COFPMessage.h"

#define ASYNC_CFG_MASK_ELE_NUM  (2)

class COFP13AsyncConfig : public COFPMessage
{
public:
    COFP13AsyncConfig(ACE_CDR::Octet abType);
    virtual ~COFP13AsyncConfig(){}

    inline void SetPacketInMask(ACE_UINT32 aacMask[ASYNC_CFG_MASK_ELE_NUM]);
    inline void SetPortStatusMask(ACE_UINT32 aacMask[ASYNC_CFG_MASK_ELE_NUM]);
    inline void SetFlowRemovedMask(ACE_UINT32 aacMask[ASYNC_CFG_MASK_ELE_NUM]);
    inline void GetPacketInMask(ACE_UINT32 (&aacMask)[ASYNC_CFG_MASK_ELE_NUM]);
    inline void GetPortStatusMask(ACE_UINT32 (&aacMask)[ASYNC_CFG_MASK_ELE_NUM]);
    inline void GetFlowRemovedMask(ACE_UINT32 (&aacMask)[ASYNC_CFG_MASK_ELE_NUM]);

public:
    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                     const;
    virtual ACE_UINT16   GetEncodeSpace()                   const
    {
        return GetStreamLen();
    }
    virtual void     Dump()                                 const;

protected:
    ACE_UINT32 m_dwPacketInMask[ASYNC_CFG_MASK_ELE_NUM];   /* Bitmasks of OFPR_* values. */
    ACE_UINT32 m_dwPortStatusMask[ASYNC_CFG_MASK_ELE_NUM]; /* Bitmasks of OFPPR_* values. */
    ACE_UINT32 m_dwFlowRemovedMask[ASYNC_CFG_MASK_ELE_NUM];/* Bitmasks of OFPRR_* values. */
};

#endif

