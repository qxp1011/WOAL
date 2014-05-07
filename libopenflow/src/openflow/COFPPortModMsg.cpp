/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPortModMsg.cpp
 * Description:      Source file for openflow port modification msg.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-16
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPPortModMsg.h"
#include "CmError.h"

CmResult COFP10PortModMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortModMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.port_no;
    is.read_octet_array(m_tMember.hw_addr, sizeof m_tMember.hw_addr);
    is>>m_tMember.config;
    is>>m_tMember.mask;    
    is>>m_tMember.advertise;
    is.read_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10PortModMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortModMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.port_no;
    os.write_octet_array(m_tMember.hw_addr, sizeof m_tMember.hw_addr);
    os<<m_tMember.config;
    os<<m_tMember.mask;    
    os<<m_tMember.advertise;
    os.write_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10PortModMsg::Dump() const
{
    COFPMessage::Dump();
}



COFP13PortModMsg::COFP13PortModMsg(ACE_UINT32 adwPortNo)
    : COFPMessage(OFP13::OFPT_PORT_MOD)
    , m_dwPortNo(adwPortNo)
    , m_dwConfig(0)
    , m_dwMask(0)
    , m_dwAdvertise(0)
{
    ::memset(m_acHwAddr, 0, OFP_ETH_ALEN);
}

CmResult COFP13PortModMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PortModMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_dwPortNo;
    is>>pad;
    is.read_octet_array(m_acHwAddr, OFP_ETH_ALEN);
    is>>pad2;
    is>>m_dwConfig;
    is>>m_dwMask;
    is>>m_dwAdvertise;
    is>>pad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13PortModMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;
    CmResult   lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PortModMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_dwPortNo;
    os<<pad;
    os.write_octet_array(m_acHwAddr, OFP_ETH_ALEN);
    os<<pad2;
    os<<m_dwConfig;
    os<<m_dwMask;
    os<<m_dwAdvertise;
    os<<pad;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP13PortModMsg::Dump()const
{
    COFPMessage::Dump();
}

