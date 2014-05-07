/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPExperimenterMsg.cpp
 * Description:      Source file for openflow experimenter message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-20
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPExperimenterMsg.h"
#include "ace/Log_Msg.h"
#include "CmError.h"

std::allocator<ACE_CDR::Octet> COFP13ExperimenterMsg::m_alloc;

COFP13ExperimenterMsg::COFP13ExperimenterMsg(
    ACE_UINT16      awAdditionalDataSize,
    const ACE_CDR::Octet *apAdditionalData,
    ACE_UINT32      anExperimenter, 
    ACE_UINT32      anExpType,
    // args for OFP header
    ACE_UINT32      anXid,
    ACE_CDR::Octet        abVersion)
    : COFPMessage(OFP13::OFPT_EXPERIMENTER, abVersion, anXid, 2*sizeof(ACE_UINT32) + awAdditionalDataSize + COFPMessage::GetStreamLen())
    , m_Experimenter(anExperimenter)
    , m_ExpType(anExpType)
    , m_pAdditionalData(NULL)
    , m_wUsedDataSize(0)
    , m_wTotalDataSize(0)
{
    if (0 == awAdditionalDataSize)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COpenFlowMessageExperimenter::COpenFlowMessageExperimenter(): awAdditionalDataSize=0\n")));
        return;
    }

    m_pAdditionalData = m_alloc.allocate(awAdditionalDataSize);
    if (NULL == m_pAdditionalData)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COpenFlowMessageExperimenter::COpenFlowMessageExperimenter(): m_alloc.allocate(%u) failed.\n"),
                   awAdditionalDataSize));
        return;
    }

    m_wTotalDataSize = awAdditionalDataSize;
    ::memset(m_pAdditionalData, 0, awAdditionalDataSize);
    
    if (NULL == apAdditionalData)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COpenFlowMessageExperimenter::COpenFlowMessageExperimenter(): apAdditionalData is NULL.\n")));
        return;
    }
    
    ::memcpy(m_pAdditionalData, apAdditionalData, awAdditionalDataSize);
    m_wUsedDataSize = awAdditionalDataSize;

    return;
}


COFP13ExperimenterMsg::~COFP13ExperimenterMsg()
{
    if (m_pAdditionalData)
    {
        m_alloc.deallocate(m_pAdditionalData, m_wTotalDataSize);
        m_pAdditionalData = NULL;
    }
}


void COFP13ExperimenterMsg::SetAdditionalData(const ACE_CDR::Octet *data, ACE_UINT16 size)
{
    CmResult result = CM_ERROR_FAILURE;

    result = COFPMsgMemoryMgr::Copy(m_alloc, m_pAdditionalData, m_wTotalDataSize, m_wUsedDataSize, data, size);

    CM_ASSERT_RETURN_VOID((CM_OK == result));
}


CmResult COFP13ExperimenterMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13ExperimenterMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_Experimenter;
    is>>m_ExpType;

    const ACE_UINT16 wPreFieldsLen = (OFP_MESSAGE_HDR_SIZE + 2 * sizeof(ACE_UINT32)); // 16
    CM_ASSERT_RETURN((m_wLength >= wPreFieldsLen), CM_ERROR_INVALID_ARG);
    
    if (m_wLength == wPreFieldsLen)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COpenFlowMessageExperimenter::StreamFrom(): m_wLength == wPreFieldsLen == %u\n"), 
                   m_wLength));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("No additional data.\n")));
        return CM_OK;
    }

    ACE_UINT16 size = m_wLength-wPreFieldsLen;
    if (m_wTotalDataSize < size)
    {
        if (m_pAdditionalData != NULL)
        {
            m_alloc.deallocate(m_pAdditionalData, m_wTotalDataSize);
            m_pAdditionalData = NULL;
            m_wTotalDataSize  = 0;
            m_wUsedDataSize   = 0;
        }

        m_pAdditionalData = m_alloc.allocate(size);
        CM_ASSERT_RETURN((m_pAdditionalData != NULL), CM_ERROR_OUT_OF_MEMORY);
        m_wTotalDataSize = size;
    }
    
    is.read_octet_array(m_pAdditionalData,size);
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    m_wUsedDataSize = size;

    return CM_OK;  
}


CmResult COFP13ExperimenterMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13ExperimenterMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_Experimenter;
    os<<m_ExpType;

    if (m_wUsedDataSize > 0)
    {
        os.write_octet_array(m_pAdditionalData, m_wUsedDataSize);
    }

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK; 
}


void COFP13ExperimenterMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowMessageExperimenter::Dump():\n")));
    
    COFPMessage::Dump();

    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("experimenter=%u, expType=%u, additionalData=%u, addionalDataLen=%u\n"), 
               m_Experimenter, m_ExpType, m_pAdditionalData, m_wUsedDataSize));

    if ((m_pAdditionalData != NULL) && (m_wUsedDataSize > 0))
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("Data[0]=%u, "), m_pAdditionalData[0]));
        if (m_wUsedDataSize > 1)
        {
            ACE_DEBUG((LM_DEBUG, 
                       ACE_TEXT("Data[%u]=%u\n"), 
                       (m_wUsedDataSize - 1), 
                       m_pAdditionalData[m_wUsedDataSize - 1]));
        }
    }

    return;    
}

