/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPStatsRequestMsg.cpp
 * Description:      Source file for openflow stats request message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-15
 *
 * History 1:
 *     Date:         2013-12-18
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/
#include "ByteOrder.h"
#include "COFPStatsRequestMsg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

using namespace openflow;

CmResult COFP10StatsRequestMsg::
COFP10DecodeStatsRequestMsg(ACE_InputCDR &aIs, COFPMessage * &aMsg)
{
    ACE_CDR::Octet acTemp[10] = {0};
    ACE_UINT16 wStatsType = 0;
    
    bool bGood = aIs.pre_read(acTemp, sizeof acTemp);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    wStatsType = *(ACE_UINT16 *)(acTemp + 8);
    ACE_CDR::swap_2(reinterpret_cast<const char *>(&wStatsType), reinterpret_cast<char *>(&wStatsType));

    switch (wStatsType)
    {
        case OFPST_DESC:
        {
            aMsg = new COFP10DescStatsRequestMsg;
            break;
        }
        case OFPST_FLOW:
        {
            aMsg = new COFP10FlowStatsRequestMsg;
            break;
        }
        case OFPST_AGGREGATE:
        {
            aMsg = new COFP10AggregateStatsRequestMsg;
            break;
        }
        case OFPST_TABLE:
        {
            aMsg = new COFP10TableStatsRequestMsg;
            break;
        }
        case OFPST_PORT:
        {
            aMsg = new COFP10PortStatsRequestMsg;
            break;
        }
        case OFPST_QUEUE:
        {
            aMsg = new COFP10QueueStatsRequestMsg;
            break;
        }
        case OFPST_VENDOR:
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
            break;
        }
        default:
        {
            CM_ASSERT_RETURN(0, CM_ERROR_FAILURE);
            break;
        }
    }

    CM_ASSERT_RETURN(aMsg != nullptr, CM_ERROR_OUT_OF_MEMORY);
    return CM_OK;
}

CmResult COFP10FlowStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowStatsRequestMsg::StreamFrom, COFP10StatsRequestMsg::StreamFrom fail\n")));
        return lRet;
    }

    lRet = m_Match.StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowStatsRequestMsg::StreamFrom, COFP10Match::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.table_id;
    is>>m_tMember.pad;
    is>>m_tMember.out_port;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10FlowStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowStatsRequestMsg::StreamTo, COFP10StatsRequestMsg::StreamTo fail\n")));
        return lRet;
    }

    lRet = m_Match.StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowStatsRequestMsg::StreamTo, COFP10Match::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.table_id;
    os<<m_tMember.pad;
    os<<m_tMember.out_port;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

void COFP10FlowStatsRequestMsg::Dump() const
{
    COFP10StatsRequestMsg::Dump();
}


CmResult COFP10AggregateStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10AggregateStatsRequestMsg::StreamFrom, COFP10StatsRequestMsg::StreamFrom fail\n")));
        return lRet;
    }

    lRet = m_Match.StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10AggregateStatsRequestMsg::StreamFrom, COFP10Match::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.table_id;
    is>>m_tMember.pad;
    is>>m_tMember.out_port;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10AggregateStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP10AggregateStatsRequestMsg::StreamTo, COFP10StatsRequestMsg::StreamTo fail\n")));
        return lRet;
    }

    lRet = m_Match.StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP10AggregateStatsRequestMsg::StreamTo, COFP10Match::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.table_id;
    os<<m_tMember.pad;
    os<<m_tMember.out_port;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

void COFP10AggregateStatsRequestMsg::Dump() const
{
    COFP10StatsRequestMsg::Dump();
}


CmResult COFP10PortStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortStatsRequestMsg::StreamFrom, COFP10StatsRequestMsg::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.port_no;
    is.read_octet_array(m_tMember.pad, sizeof m_tMember.pad);

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10PortStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortStatsRequestMsg::StreamTo, COFP10StatsRequestMsg::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.port_no;
    os.write_octet_array(m_tMember.pad, sizeof m_tMember.pad);

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

void COFP10PortStatsRequestMsg::Dump() const
{
    COFP10StatsRequestMsg::Dump();
}


CmResult COFP10QueueStatsRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueStatsRequestMsg::StreamFrom, COFP10StatsRequestMsg::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.port_no;
    is.read_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    is>>m_tMember.queue_id;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10QueueStatsRequestMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsRequestMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueStatsRequestMsg::StreamTo, COFP10StatsRequestMsg::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.port_no;
    os.write_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    os<<m_tMember.queue_id;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

void COFP10QueueStatsRequestMsg::Dump() const
{
    COFP10StatsRequestMsg::Dump();
}

