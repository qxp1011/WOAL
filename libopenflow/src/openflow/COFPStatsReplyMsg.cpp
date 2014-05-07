/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPStatsReplyMsg.cpp
 * Description:      Source file for openflow stats reply message.
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
#include "COFPStatsReplyMsg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

using namespace openflow;

CmResult COFP10StatsReplyMsg::
COFP10DecodeStatsReplyMsg(ACE_InputCDR &aIs, COFPMessage * &aMsg)
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
            aMsg = new COFP10DescStatsReplyMsg;
            break;
        }
        case OFPST_FLOW:
        {
            aMsg = new COFP10FlowStatsReplyMsg;
            break;
        }
        case OFPST_AGGREGATE:
        {
            aMsg = new COFP10AggregateStatsReplyMsg;
            break;
        }
        case OFPST_TABLE:
        {
            aMsg = new COFP10TableStatsReplyMsg;
            break;
        }
        case OFPST_PORT:
        {
            aMsg = new COFP10PortStatsReplyMsg;
            break;
        }
        case OFPST_QUEUE:
        {
            aMsg = new COFP10QueueStatsReplyMsg;
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

    return CM_OK;
}

CmResult COFP10DescStatsReplyMsg::SetManufacturerDesc(char *apDesc)
{
    CM_ASSERT_RETURN(apDesc, CM_ERROR_FAILURE);
    
    if (::strlen(apDesc) < sizeof(m_tMember.mfr_desc))
    {
        ::memcpy(m_tMember.mfr_desc, apDesc, ::strlen(apDesc));
        return CM_OK;
    }

    return CM_ERROR_FAILURE;
}

CmResult COFP10DescStatsReplyMsg::SetHardwareDesc(char *apDesc)
{
    CM_ASSERT_RETURN(apDesc, CM_ERROR_FAILURE);
    
    if (::strlen(apDesc) < sizeof(m_tMember.hw_desc))
    {
        ::memcpy(m_tMember.hw_desc, apDesc, ::strlen(apDesc));
        return CM_OK;
    }

    return CM_ERROR_FAILURE;
}

CmResult COFP10DescStatsReplyMsg::SetSoftwareDesc(char *apDesc)
{
    CM_ASSERT_RETURN(apDesc, CM_ERROR_FAILURE);
    
    if (::strlen(apDesc) < sizeof(m_tMember.sw_desc))
    {
        ::memcpy(m_tMember.sw_desc, apDesc, ::strlen(apDesc));
        return CM_OK;
    }

    return CM_ERROR_FAILURE;
}

CmResult COFP10DescStatsReplyMsg::SetSerialNum(char *apNum)
{
    CM_ASSERT_RETURN(apNum, CM_ERROR_FAILURE);
    
    if (::strlen(apNum) < sizeof(m_tMember.serial_num))
    {
        ::memcpy(m_tMember.serial_num, apNum, ::strlen(apNum));
        return CM_OK;
    }

    return CM_ERROR_FAILURE;
}

CmResult COFP10DescStatsReplyMsg::SetDatapathDesc(char *apDesc)
{
    CM_ASSERT_RETURN(apDesc, CM_ERROR_FAILURE);
    
    if (::strlen(apDesc) < sizeof(m_tMember.dp_desc))
    {
        ::memcpy(m_tMember.dp_desc, apDesc, ::strlen(apDesc));
        return CM_OK;
    }

    return CM_ERROR_FAILURE;
}

CmResult COFP10DescStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_DEBUG, ACE_TEXT("COFP10DescStatsReplyMsg::StreamFrom, COFP10StatsReplyMsg::StreamFrom fail\n")));
        return lRet;
    }

    is.read_char_array(m_tMember.mfr_desc, sizeof m_tMember.mfr_desc);
    is.read_char_array(m_tMember.hw_desc, sizeof m_tMember.hw_desc);
    is.read_char_array(m_tMember.sw_desc, sizeof m_tMember.sw_desc);
    is.read_char_array(m_tMember.serial_num, sizeof m_tMember.serial_num);
    is.read_char_array(m_tMember.dp_desc, sizeof m_tMember.dp_desc);

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10DescStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10DescStatsReplyMsg::StreamTo, COFP10StatsReplyMsg::StreamTo fail\n")));
        return lRet;
    }

    os.write_char_array(m_tMember.mfr_desc, sizeof m_tMember.mfr_desc);
    os.write_char_array(m_tMember.hw_desc, sizeof m_tMember.hw_desc);
    os.write_char_array(m_tMember.sw_desc, sizeof m_tMember.sw_desc);
    os.write_char_array(m_tMember.serial_num, sizeof m_tMember.serial_num);
    os.write_char_array(m_tMember.dp_desc, sizeof m_tMember.dp_desc);

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

void COFP10DescStatsReplyMsg::Dump() const
{
    COFP10StatsReplyMsg::Dump();
}



COFP10FlowStatsReplyMsg::
~COFP10FlowStatsReplyMsg()
{
    for (auto &item : m_ele_list)
    {
        delete item;
    }
}

CmResult COFP10FlowStatsReplyMsg::
AddEleList(std::list<COFP10FlowStatsReplyEle *> &list)
{
    while (!list.empty())
    {
        COFP10FlowStatsReplyEle *Ele = list.front();
        CM_ASSERT_RETURN(Ele != nullptr, CM_ERROR_NULL_POINTER);
        list.pop_front();
        m_ele_list.push_back(Ele);
        m_wEleLen += Ele->GetStreamLen();
    }

    return CM_OK;
}

CmResult COFP10FlowStatsReplyMsg::
AppendEle(COFP10FlowStatsReplyEle *aEle)
{
    CM_ASSERT_RETURN(aEle != nullptr, CM_ERROR_NULL_POINTER);
    m_ele_list.push_back(aEle);
    m_wEleLen += aEle->GetStreamLen();
    return CM_OK;
}

CmResult COFP10FlowStatsReplyMsg::
GetEleList(std::list<COFP10FlowStatsReplyEle *> &list)
{
    while (!m_ele_list.empty())
    {
        COFP10FlowStatsReplyEle *Ele = m_ele_list.front();
        CM_ASSERT_RETURN(Ele != nullptr, CM_ERROR_NULL_POINTER);
        m_ele_list.pop_front();
        m_wEleLen -= Ele->GetStreamLen();
        list.push_back(Ele);
    }

    return CM_OK;
}

CmResult COFP10FlowStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10FlowStatsReplyMsg::StreamFrom, COFP10StatsReplyMsg::StreamFrom fail\n")));
        return lRet;
    }

    ACE_INT16 swListLen = m_wLength - COFP10StatsReplyMsg::GetStreamLen();
    if (swListLen > 0)
    {
        while (swListLen > 0)
        {
            COFP10FlowStatsReplyEle *Ele = new COFP10FlowStatsReplyEle;
            CM_ASSERT_RETURN(Ele != nullptr, CM_ERROR_OUT_OF_MEMORY);
            lRet = Ele->StreamFrom(is);
            if (CM_SUCCEEDED(lRet))
            {
                m_ele_list.push_back(Ele);
                m_wEleLen += Ele->GetStreamLen();
                swListLen -= Ele->GetStreamLen();
            }
            else
            {
                CM_ASSERT_RETURN(lRet, CM_ERROR_FAILURE);
            }
        }
    }

    return CM_OK;
}

CmResult COFP10FlowStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP10FlowStatsReplyMsg::StreamTo, COFP10StatsReplyMsg::StreamTo fail\n")));
        return lRet;
    }
    
    std::list<COFP10FlowStatsReplyEle *>::const_iterator it = m_ele_list.begin();
    while (it != m_ele_list.end())
    {
        lRet = (*it)->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
        it++;
    }

    return CM_OK;
}

void COFP10FlowStatsReplyMsg::Dump() const
{
    COFP10StatsReplyMsg::Dump();
}


CmResult COFP10AggregateStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP10AggregateStatsReplyMsg::StreamFrom, COFP10StatsReplyMsg::StreamFrom fail\n")));
        return lRet;
    }

    is>>m_tMember.packet_count;
    is>>m_tMember.byte_count;
    is>>m_tMember.flow_count;
    is.read_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    
    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10AggregateStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10AggregateStatsReplyMsg::StreamTo, COFP10StatsReplyMsg::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_tMember.packet_count;
    os<<m_tMember.byte_count;
    os<<m_tMember.flow_count;
    os.write_octet_array(m_tMember.pad, sizeof m_tMember.pad);
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

void COFP10AggregateStatsReplyMsg::Dump() const
{
    COFP10StatsReplyMsg::Dump();
}



COFP10TableStatsReplyMsg::~COFP10TableStatsReplyMsg()
{
    for (auto &item : m_table_list)
    {
        delete item;
    }
}

CmResult COFP10TableStatsReplyMsg::AddTableList(std::list<TableStatsReply *> &list)
{
    if (!list.empty())
    {
        std::list<TableStatsReply *>::iterator It = list.begin();
        while (It != list.end())
        {
            TableStatsReply *reply = *It;
            It = list.erase(It);
            m_table_list.push_back(reply);
            m_wTableLen += sizeof(TableStatsReply);
        }
    }

    return CM_OK;
}

CmResult COFP10TableStatsReplyMsg::AppendTable(TableStatsReply *aTable)
{
    m_table_list.push_back(aTable);
    m_wTableLen += sizeof(TableStatsReply);
    
    return CM_OK;
}

CmResult COFP10TableStatsReplyMsg::GetTableList(std::list<TableStatsReply *> &list)
{
    if (!m_table_list.empty())
    {
        std::list<TableStatsReply *>::iterator It = m_table_list.begin();
        while (It != m_table_list.end())
        {
            TableStatsReply *reply = *It;
            It = m_table_list.erase(It);
            m_wTableLen -= sizeof(TableStatsReply);
            list.push_back(reply);
        }
    }
    
    return CM_OK;
}

CmResult COFP10TableStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10TableStatsReplyMsg::StreamFrom, COFP10StatsReplyMsg::StreamFrom fail\n")));
        return lRet;
    }

    bool bGood = false;
    ACE_INT16 swTableList = m_wLength - COFP10StatsReplyMsg::GetStreamLen();
    while (swTableList > 0)
    {
        CM_ASSERT_RETURN((swTableList % sizeof(TableStatsReply) == 0), CM_ERROR_FAILURE);
        TableStatsReply *pTable = new TableStatsReply;
        CM_ASSERT_RETURN(pTable != nullptr, CM_ERROR_OUT_OF_MEMORY);
        
        is>>pTable->table_id;
        is.read_octet_array(pTable->pad, sizeof pTable->pad);
        is.read_char_array(pTable->name, sizeof pTable->name);
        is>>pTable->wildcards;
        is>>pTable->max_entries;
        is>>pTable->active_count;
        is>>pTable->lookup_count;
        is>>pTable->matched_count;
        
        bGood = is.good_bit();            
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

        m_table_list.push_back(pTable);
        m_wTableLen += sizeof(TableStatsReply);
        swTableList -= sizeof(TableStatsReply);
    }

    return CM_OK;
}

CmResult COFP10TableStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10TableStatsReplyMsg::StreamTo, COFP10StatsReplyMsg::StreamTo fail\n")));
        return lRet;
    }
    
    if (!m_table_list.empty())
    {
        bool bGood = false;
        std::list<TableStatsReply *>::iterator It = m_table_list.begin();
        while (It != m_table_list.end())
        {
            os<<(*It)->table_id;
            os.write_octet_array((*It)->pad, sizeof (*It)->pad);
            os.write_char_array((*It)->name, sizeof (*It)->name);
            os<<(*It)->wildcards;
            os<<(*It)->max_entries;
            os<<(*It)->active_count;
            os<<(*It)->lookup_count;
            os<<(*It)->matched_count;
            
            bGood = os.good_bit();            
            CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
            
            It++;
        }
    }

    return CM_OK;
}

void COFP10TableStatsReplyMsg::Dump() const
{
    COFP10StatsReplyMsg::Dump();
}


COFP10PortStatsReplyMsg::~COFP10PortStatsReplyMsg()
{
    for (auto &item : m_port_list)
    {
        delete item;
    }
}

CmResult COFP10PortStatsReplyMsg::AddPortList(std::list<PortStatsReply *> &list)
{
    if (!list.empty())
    {
        std::list<PortStatsReply *>::iterator It = list.begin();
        while (It != list.end())
        {
            PortStatsReply *reply = *It;
            It = list.erase(It);
            m_port_list.push_back(reply);
            m_wPortLen += sizeof(PortStatsReply);
        }
    }

    return CM_OK;
}

CmResult COFP10PortStatsReplyMsg::AppendPort(PortStatsReply *aPort)
{
    m_port_list.push_back(aPort);
    m_wPortLen += sizeof(PortStatsReply);
    
    return CM_OK;
}

CmResult COFP10PortStatsReplyMsg::GetPortList(std::list<PortStatsReply *> &list)
{
    if (!m_port_list.empty())
    {
        std::list<PortStatsReply *>::iterator It = m_port_list.begin();
        while (It != m_port_list.end())
        {
            PortStatsReply *reply = *It;
            It = m_port_list.erase(It);
            m_wPortLen -= sizeof(PortStatsReply);
            list.push_back(reply);
        }
    }
    
    return CM_OK;
}

CmResult COFP10PortStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortStatsReplyMsg::StreamFrom, COFP10StatsReplyMsg::StreamFrom fail\n")));
        return lRet;
    }

    bool bGood = false;
    ACE_INT16 swPortList = m_wLength - COFP10StatsReplyMsg::GetStreamLen();
    while (swPortList > 0)
    {
        CM_ASSERT_RETURN((swPortList % sizeof(PortStatsReply) == 0), CM_ERROR_FAILURE);
        PortStatsReply *port = new PortStatsReply;
        CM_ASSERT_RETURN(port != nullptr, CM_ERROR_OUT_OF_MEMORY);
        is>>port->port_no;
        is.read_octet_array(port->pad, sizeof port->pad);
        is>>port->rx_packets;
        is>>port->tx_packets;
        is>>port->rx_bytes;
        is>>port->tx_bytes;
        is>>port->rx_dropped;
        is>>port->tx_dropped;
        is>>port->rx_errors;
        is>>port->tx_errors;
        is>>port->rx_frame_err;
        is>>port->rx_over_err;
        is>>port->rx_crc_err;
        is>>port->collisions;
        
        bGood = is.good_bit();            
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

        m_port_list.push_back(port);
        m_wPortLen += sizeof(PortStatsReply);
        swPortList -= sizeof(PortStatsReply);
    }

    return CM_OK;
}

CmResult COFP10PortStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PortStatsReplyMsg::StreamTo, COFP10StatsReplyMsg::StreamTo fail\n")));
        return lRet;
    }

    if (!m_port_list.empty())
    {
        bool bGood = false;
        std::list<PortStatsReply *>::iterator It = m_port_list.begin();
        while (It != m_port_list.end())
        {
            os<<(*It)->port_no;
            os.write_octet_array((*It)->pad, sizeof (*It)->pad);
            os<<(*It)->rx_packets;
            os<<(*It)->tx_packets;
            os<<(*It)->rx_bytes;
            os<<(*It)->tx_bytes;
            os<<(*It)->rx_dropped;
            os<<(*It)->tx_dropped;
            os<<(*It)->rx_errors;
            os<<(*It)->tx_errors;
            os<<(*It)->rx_frame_err;
            os<<(*It)->rx_over_err;
            os<<(*It)->rx_crc_err;
            os<<(*It)->collisions;
            
            bGood = os.good_bit();            
            CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
            
            It++;
        }
    }

    return CM_OK;
}

void COFP10PortStatsReplyMsg::Dump() const
{
    COFP10StatsReplyMsg::Dump();
}


COFP10QueueStatsReplyMsg::~COFP10QueueStatsReplyMsg()
{
    for (auto &item : m_queue_list)
    {
        delete item;
    }
}

CmResult COFP10QueueStatsReplyMsg::AddQueueList(std::list<QueueStatsReply *> &list)
{
    if (!list.empty())
    {
        std::list<QueueStatsReply *>::iterator It = list.begin();
        while (It != list.end())
        {
            QueueStatsReply *reply = *It;
            It = list.erase(It);
            m_queue_list.push_back(reply);
            m_wQueueLen += sizeof(QueueStatsReply);
        }
    }

    return CM_OK;
}

CmResult COFP10QueueStatsReplyMsg::AppendQueue(QueueStatsReply *aQueue)
{
    m_queue_list.push_back(aQueue);
    m_wQueueLen += sizeof(QueueStatsReply);
    
    return CM_OK;
}

CmResult COFP10QueueStatsReplyMsg::GetQueueList(std::list<QueueStatsReply *> &list)
{
    if (!m_queue_list.empty())
    {
        std::list<QueueStatsReply *>::iterator It = m_queue_list.begin();
        while (It != m_queue_list.end())
        {
            QueueStatsReply *reply = *It;
            It = m_queue_list.erase(It);
            m_wQueueLen -= sizeof(QueueStatsReply);
            list.push_back(reply);
        }
    }
    
    return CM_OK;
}

CmResult COFP10QueueStatsReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    bool bGood = false;

    lRet = COFP10StatsReplyMsg::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueStatsReplyMsg::StreamFrom, COFP10StatsReplyMsg::StreamFrom fail\n")));
        return lRet;
    }

    ACE_INT16 swQueueList = m_wLength - COFP10StatsReplyMsg::GetStreamLen();
    while (swQueueList > 0)
    {
        CM_ASSERT_RETURN((swQueueList % sizeof(QueueStatsReply) == 0), CM_ERROR_FAILURE);
        QueueStatsReply *pQueue = new QueueStatsReply;
        CM_ASSERT_RETURN(pQueue != nullptr, CM_ERROR_OUT_OF_MEMORY);
        is>>pQueue->port_no;
        is.read_octet_array(pQueue->pad, sizeof pQueue->pad);
        is>>pQueue->queue_id;
        is>>pQueue->tx_bytes;
        is>>pQueue->tx_packets;
        is>>pQueue->tx_errors;
        
        bGood = is.good_bit();            
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

        m_queue_list.push_back(pQueue);
        m_wQueueLen += sizeof(QueueStatsReply);
        swQueueList -= sizeof(QueueStatsReply);
    }

    return CM_OK;
}

CmResult COFP10QueueStatsReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFP10StatsReplyMsg::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10QueueStatsReplyMsg::StreamTo, COFP10StatsReplyMsg::StreamTo fail\n")));
        return lRet;
    }    

    if (!m_queue_list.empty())
    {
        bool bGood = false;
        std::list<QueueStatsReply *>::iterator It = m_queue_list.begin();
        while (It != m_queue_list.end())
        {
            os<<(*It)->port_no;
            os.write_octet_array((*It)->pad, sizeof (*It)->pad);
            os<<(*It)->queue_id;
            os<<(*It)->tx_bytes;
            os<<(*It)->tx_packets;
            os<<(*It)->tx_errors;
            
            bGood = os.good_bit();            
            CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
            
            It++;
        }
    }

    return CM_OK;
}

void COFP10QueueStatsReplyMsg::Dump() const
{
    COFP10StatsReplyMsg::Dump();
}

