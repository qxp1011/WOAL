/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFP10Action.cpp
 * Description:      
 * Others:
 * Version:          V1.0
 * Author:           
 * Date:             2013-07-12
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#include "ByteOrder.h"
#include "COFP10Action.h"
#include "OFPMessageUtils.h"
#include "CmError.h"
#include "ace/Log_Msg.h"
#include "ace/CDR_Base.h"

using namespace openflow;

CmResult COFP10Action::
DecodeAction(ACE_InputCDR& is, COFP10Action * &apAction)
{
    CmResult      lRet  = CM_ERROR_FAILURE;
    ACE_UINT16    wType = 0;
    COFP10Action *pAct  = nullptr;
    
    bool bGood = is.pre_read(&wType, sizeof(wType));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
	ACE_CDR::swap_2(reinterpret_cast<const char *>(&wType), reinterpret_cast<char *>(&wType));

    switch (wType)
    {
        case OFPAT_OUTPUT:
        {
            pAct = new COFP10ActOutput();
            break;
        }
        case OFPAT_SET_VLAN_VID:
        {
            pAct = new COFP10ActSetVlanVid();
            break;
        }
        case OFPAT_SET_VLAN_PCP:
        {
            pAct = new COFP10ActSetVlanPcp();
            break;
        }
        case OFPAT_STRIP_VLAN:
        {
            pAct = new COFP10ActStripVlan();
            break;
        }
        case OFPAT_SET_DL_SRC:
        case OFPAT_SET_DL_DST:
        {
            pAct = new COFP10ActSetDlAddr(wType);
            break;
        }
        case OFPAT_SET_NW_SRC:
        case OFPAT_SET_NW_DST:
        {
            pAct = new COFP10ActSetNwAddr(wType);
            break;
        }
        case OFPAT_SET_NW_TOS:
        {
            pAct = new COFP10ActSetNwTos();
            break;
        }
        case OFPAT_SET_TP_SRC:
        case OFPAT_SET_TP_DST:
        {
            pAct = new COFP10ActSetTpPort(wType);
            break;
        }
        case OFPAT_ENQUEUE:
        {
            pAct = new COFP10ActEnqueue();
            break;
        }
        default:
        {
            break;
        }
    }
    
    CM_ASSERT_RETURN(pAct != nullptr, CM_ERROR_OUT_OF_MEMORY);

    lRet = pAct->StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10Action::DecodeAction(): fail in StreamFrom(), lRet=%d\n"), lRet));
        return lRet;
    }

    apAction = pAct;
    
    return CM_OK;
}

CmResult COFP10Action::StreamFrom(ACE_InputCDR &is)
{
    is>>m_wType;
    is>>m_wLen;
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10Action::StreamTo(ACE_OutputCDR &os)
{
    m_wLen = GetStreamLen();
    
    os<<m_wType;
    os<<m_wLen;
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10Action::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10Action::Dump():\n")));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type=%u, len=%u\n"), m_wType, m_wLen));
}


CmResult COFP10ActOnlyHeader::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[4] = {0};

    CmResult result = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

	bool bGood = is.read_octet_array(pad, 4);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10ActOnlyHeader::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[4] = {0};

    CmResult result = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);

    os.write_octet_array(pad, 4);

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}



CmResult COFP10ActOutput::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    is>>m_wOutPort;
    is>>m_wMaxLen;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10ActOutput::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<m_wOutPort;
    os<<m_wMaxLen;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10ActOutput::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActOutput::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("port=%u, max_len=%u\n"), m_wOutPort, m_wMaxLen));
}


CmResult COFP10ActEnqueue::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[6] = {0};
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    is>>m_wPort;
	is.read_octet_array(pad, sizeof(pad));
    is>>m_dwQueueId;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10ActEnqueue::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[6] = {0};
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<m_wPort;
    os.write_octet_array(pad, sizeof(pad));
    os<<m_dwQueueId;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10ActEnqueue::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActEnqueue::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("port=%u, queueId=%u\n"), m_wPort, m_dwQueueId));
}


CmResult COFP10ActSetVlanVid::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[2] = {0};
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    is>>m_wVlanVid;
	is.read_octet_array(pad, sizeof(pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10ActSetVlanVid::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[2] = {0};
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<m_wVlanVid;
    os.write_octet_array(pad, sizeof(pad));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10ActSetVlanVid::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActSetVlanVid::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("vlan_vid=%u\n"), m_wVlanVid));
}


CmResult COFP10ActSetVlanPcp::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[3] = {0};
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    is>>m_bVlanPcp;
    is.read_octet_array(pad, sizeof(pad));

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10ActSetVlanPcp::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[3] = {0};
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<m_bVlanPcp;
    os.write_octet_array(pad, sizeof(pad));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10ActSetVlanPcp::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActSetVlanPcp::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("vlan_pcp=%u\n"), m_bVlanPcp));
}


CmResult COFP10ActSetDlAddr::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    bool bGood = is.read_octet_array(m_acDlAddr, sizeof(m_acDlAddr));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamFrom(is, 6);
}

CmResult COFP10ActSetDlAddr::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os.write_octet_array(m_acDlAddr, sizeof(m_acDlAddr));

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamTo(os, 6);
}

void COFP10ActSetDlAddr::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActSetDlAddr::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("dl_addr: %u-%u-%u-%u-%u-%u\n"), 
               m_acDlAddr[0], m_acDlAddr[1], m_acDlAddr[2], m_acDlAddr[3], m_acDlAddr[4], m_acDlAddr[5]));
}


CmResult COFP10ActSetNwAddr::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    is>>m_dwNwAddr;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10ActSetNwAddr::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<m_dwNwAddr;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10ActSetNwAddr::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActSetNwAddr::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("nw_addr=%u\n"), m_dwNwAddr));
}


CmResult COFP10ActSetNwTos::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    is>>m_bNwTos;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamFrom(is, 3);
}

CmResult COFP10ActSetNwTos::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<m_bNwTos;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamTo(os, 3);
}

void COFP10ActSetNwTos::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActSetNwTos::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("nw_tos=%u\n"), m_bNwTos));
}


CmResult COFP10ActSetTpPort::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    is>>m_wTpPort;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamFrom(is, 2);
}

CmResult COFP10ActSetTpPort::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10Action::StreamTo(os);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);

    os<<m_wTpPort;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return COFPMsgMemoryMgr::PadStreamTo(os, 2);
}

void COFP10ActSetTpPort::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10ActSetTpPort::Dump():\n")));
    COFP10Action::Dump();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("tp_port=%u\n"), m_wTpPort));
}

