/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPErrorMsg.cpp
 * Description:      Source file for openflow error message.
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

#include "COFPErrorMsg.h"
#include <string>
#include "CmError.h"

std::allocator<ACE_CDR::Octet> COFP10ErrorMsg::m_alloc;

COFP10ErrorMsg::
COFP10ErrorMsg(ACE_CDR::Octet abVersion, ACE_UINT16 awSrcLen, const ACE_CDR::Octet *apSrc, ACE_UINT16 awErrType, ACE_UINT16 awErrCode)
    :  COFPMessage(OFP10::OFPT_ERROR, abVersion, 0, 4 + awSrcLen + COFPMessage::GetStreamLen())
    , m_wErrType(0)
    , m_wErrCode(0)
    , m_wDataLen(0)
    , m_wDataMemSize(0)
    , m_pData(NULL)
{
    if (0 == awSrcLen)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowErrMsg::COpenFlowErrMsg(): awSrcLen=0\n")));
        return;
    }

    m_pData = m_alloc.allocate(awSrcLen);
    CM_ASSERT_RETURN_VOID(m_pData != NULL);
    m_wDataMemSize = awSrcLen;
    ::memset(m_pData, 0, awSrcLen);

    if (NULL == apSrc)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowErrMsg::COpenFlowErrMsg(): apSrc=NULL\n")));
        return;
    }

    ::memcpy(m_pData, apSrc, awSrcLen);
    m_wDataLen = awSrcLen;

    return;
}

COFP10ErrorMsg::~COFP10ErrorMsg()
{
    if (NULL != m_pData)
    {
        m_alloc.deallocate(m_pData, m_wDataMemSize);
        m_pData = NULL;
    }
}

CmResult COFP10ErrorMsg::SetErrData(const ACE_CDR::Octet *apData, ACE_UINT16 awDataLen)
{
    CmResult result = CM_ERROR_FAILURE;

    CM_ASSERT_RETURN(apData != NULL, CM_ERROR_FAILURE);
    CM_ASSERT_RETURN(awDataLen != 0, CM_ERROR_FAILURE);

    result = COFPMsgMemoryMgr::Copy(m_alloc, m_pData, m_wDataMemSize, m_wDataLen, apData, awDataLen);

    return result;
}

CmResult COFP10ErrorMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16 wDataLen = 0;
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10ErrorMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    ACE_UINT16 wStreamLen = (m_wLength - COFPMessage::GetStreamLen());
    CM_ASSERT_RETURN(wStreamLen >= 4, CM_ERROR_INVALID_ARG);
    
    is>>m_wErrType;
    is>>m_wErrCode;

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ErrorMsg::StreamFrom():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type=%u, code=%u\n"), m_wErrType, m_wErrCode));

    if (4 == wStreamLen)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowErrMsg::StreamFrom(): wStreamLen=4\n")));
        return CM_OK;
    }
        
    wDataLen = wStreamLen - 4;
    if (m_wDataMemSize < wDataLen)
    {
        if (m_pData != NULL)
        {
            m_alloc.deallocate(m_pData, m_wDataMemSize);
            m_pData       = NULL;
            m_wDataLen     = 0;
            m_wDataMemSize = 0;
        }
        
        m_pData = m_alloc.allocate(wDataLen);
        CM_ASSERT_RETURN(m_pData != NULL, CM_ERROR_OUT_OF_MEMORY);
        m_wDataMemSize = wDataLen;
        ::memset(m_pData, 0, wDataLen);
    }

    bool bGood = is.read_octet_array(m_pData, wDataLen);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    m_wDataLen = wDataLen;

    ACE_ERROR((LM_ERROR, ACE_TEXT("error data: ")));
    if (OFPET_HELLO_FAILED == m_wErrType)
    {
        m_pData[m_wDataLen - 1] = 0;
        ACE_ERROR((LM_ERROR, ACE_TEXT("%s\n"), m_pData));
    }
    else
    {
        std::string str((char *)m_pData,m_wDataLen);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("%s\n"), str.c_str()));
    }
    
    return CM_OK;
}

CmResult COFP10ErrorMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10ErrorMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_wErrType;
    os<<m_wErrCode;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    if (m_wDataLen > 0)
    {
        bGood = os.write_octet_array(m_pData, m_wDataLen);
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;
}

void COFP10ErrorMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowErrMsg::Dump():\n")));
    
    COFPMessage::Dump();
    
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type=%u, code=%u\n"), m_wErrType, m_wErrCode));
    if (m_wDataLen > 0)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("data[0]=%u\n"), m_pData[0]));
        if (m_wDataLen > 1)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("data[%u]=%u\n"), m_wDataLen-1, m_pData[m_wDataLen-1]));
        }
    }
}



COFP13ErrorMsg::
COFP13ErrorMsg(ACE_CDR::Octet abVersion, ACE_UINT16 awSrcLen, const ACE_CDR::Octet *apSrc, ACE_UINT16 awErrType, ACE_UINT16 awErrCode)
    :  COFP10ErrorMsg(abVersion, awSrcLen, apSrc, awErrType, awErrCode)
{
}

