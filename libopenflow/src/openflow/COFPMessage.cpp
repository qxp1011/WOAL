/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMessage.cpp
 * Description:      OpenFlow Message source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/

#include "COFPMessage.h"
#include "COFPHelloMsg.h"
#include "COFPErrorMsg.h"
#include "COFPEchoRequestMsg.h"
#include "COFPEchoReplyMsg.h"
#include "COFPExperimenterMsg.h"
#include "COFPFeaturesRequestMsg.h"
#include "COFPFeaturesReplyMsg.h"
#include "COFPGetConfigRequestMsg.h"
#include "COFPGetConfigReplyMsg.h"
#include "COFPPacketInMsg.h"
#include "COFPPacketOutMsg.h"
#include "COFPFlowModMsg.h"
#include "COFPPortStatusMsg.h"
#include "COFPTableModMsg.h"
#include "COFPMultipartMsg.h"
#include "COFPSetConfigMsg.h"
#include "COFPExperimenterMsg.h"
#include "COFPFlowRemovedMsg.h"
#include "COFPGroupModMsg.h"
#include "COFPMeterModMsg.h"
#include "COFPGetAsyncReplyMsg.h"
#include "COFPGetAsyncRequestMsg.h"
#include "COFPSetAsyncMsg.h"
#include "COFPRoleMsg.h"
#include "COFPQueueGetConfigRequestMsg.h"
#include "COFPQueueGetConfigReplyMsg.h"
#include "COFPBarrierMsg.h"
#include "COFPPortModMsg.h"
#include "COFPStatsRequestMsg.h"
#include "COFPStatsReplyMsg.h"
#include "CmError.h"
#include "ace/CDR_Base.h"

COFPMessage::COFPMessage(
    ACE_CDR::Octet    abType,
    ACE_CDR::Octet    abVersion,
    ACE_UINT32  adwxid,
    ACE_UINT16  awLength)
    : m_bVersion(abVersion) 
    , m_bType(abType)
    , m_wLength(awLength)
    , m_dwxid(adwxid)
{}


CmResult COFPMessage::
DecodeMessage(ACE_Message_Block &aMB, COFPMessage * &apOFPMsg)
{
    ACE_CDR::Octet acData[2] = {0};
    ACE_CDR::Octet bVer = 0;
    ACE_CDR::Octet bType = 0;
    ACE_InputCDR is(&aMB, ACE_CDR::BYTE_ORDER_BIG_ENDIAN);

    bool bGood = is.pre_read(acData, sizeof(acData));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    bVer  = acData[0];
    bType = acData[1];

    CmResult result = CM_ERROR_FAILURE;
    
    switch (bVer)
    {
        case OFP10_VERSION:
        {
            result = OFP10DecodeMessage(bType, is, apOFPMsg);
            break;
        }
        case OFP13_VERSION:
        {
            result = OFP13DecodeMessage(bType, is, apOFPMsg);
            break;
        }
        default:
        {
            CM_ASSERT_RETURN(0, CM_ERROR_INVALID_ARG);
        }
    }

    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    return CM_OK;
}

CmResult COFPMessage::OFP10DecodeMessage(ACE_CDR::Octet abType, ACE_InputCDR &aIs, COFPMessage * &apOFPMsg)
{   
    using namespace OFP10;
    COFPMessage *pMessage = nullptr;

    switch (abType)
    {
        case OFPT_HELLO:
        {
            pMessage = new COFP10HelloMsg();    //(wLength - OPENFLOW_MESSAGE_HDR_SIZE);
            break;
        }
        case OFPT_ERROR:
        {
            pMessage = new COFP10ErrorMsg();
            break;
        }
        case OFPT_ECHO_REQUEST:
        {
            pMessage = new COFP10EchoRequestMsg();
            break;
        }
        case OFPT_ECHO_REPLY:
        {
            pMessage = new COFP10EchoReplyMsg();
            break;
        }
        case OFPT_VENDOR:
        {
            ACE_ERROR((LM_ERROR, ACE_TEXT("unsupported type: OFPT_VENDOR\n")));
            ACE_ASSERT(0);
            break;
        }
        /* Switch configuration messages. */
        case OFPT_FEATURES_REQUEST :
        {
            pMessage = new COFP10FeaturesRequestMsg();
            break;
        }
        case OFPT_FEATURES_REPLY:
        {
            pMessage = new COFP10FeaturesReplyMsg();
            break;
        }
        case OFPT_GET_CONFIG_REQUEST:
        {
            pMessage = new COFP10GetConfigRequestMsg();
            break;
        }
        case OFPT_GET_CONFIG_REPLY:
        {
            pMessage = new COFP10GetConfigReplyMsg();
            break;
        }
        case OFPT_SET_CONFIG:
        {
            pMessage = new COFP10SetConfigMsg();
            break;
        }
        /* Asynchronous messages. */
        case OFPT_PACKET_IN:
        {
            pMessage = new COFP10PacketInMsg();
            break;
        }
        case OFPT_FLOW_REMOVED:
        {
            pMessage = new COFP10FlowRemovedMsg();
            break;
        }
        case OFPT_PORT_STATUS:
        {
            pMessage = new COFP10PortStatusMsg();
            break;
        }
        /* Controller command messages. */
        case OFPT_PACKET_OUT:
        {
            pMessage = new COFP10PacketOutMsg();
            break;
        }
        case OFPT_FLOW_MOD:
        {
            pMessage = new COFP10FlowModMsg();
            break;
        }
        case OFPT_PORT_MOD:
        {
            pMessage = new COFP10PortModMsg();
            break;
        }
        /* Statistics messages. */
        case OFPT_STATS_REQUEST:
        {
            CmResult lRet = COFP10StatsRequestMsg::COFP10DecodeStatsRequestMsg(aIs, pMessage);
            CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
            break;
        }
        case OFPT_STATS_REPLY:
        {
            CmResult lRet = COFP10StatsReplyMsg::COFP10DecodeStatsReplyMsg(aIs, pMessage);
            CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), CM_ERROR_FAILURE);
            break;
        }
        /* Barrier messages. */
        case OFPT_BARRIER_REQUEST:
        {
            pMessage = new COFP10BarrierMsg(OFPT_BARRIER_REQUEST);
            break;
        }
        case OFPT_BARRIER_REPLY:
        {
            pMessage = new COFP10BarrierMsg(OFPT_BARRIER_REPLY);
            break;
        }
        /* Queue Configuration messages. */
        case OFPT_QUEUE_GET_CONFIG_REQUEST:
        {
            pMessage = new COFP10QueueGetConfigRequestMsg;
            break;
        }
        case OFPT_QUEUE_GET_CONFIG_REPLY:
        {
            pMessage = new COFP10QueueGetConfigReplyMsg;
            break;
        }
        default:
        {
            ACE_ERROR((LM_ERROR, ACE_TEXT("COFPMessage::CreatePdu, type=%u\n"), abType));
            return CM_ERROR_NOT_FOUND;
        }
    }

    CM_ASSERT_RETURN(pMessage != nullptr, CM_ERROR_OUT_OF_MEMORY);

    CmResult nResult = pMessage->StreamFrom(aIs);
    if (CM_FAILED(nResult)) {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFPMessage::CreatePdu, fail in pdu->StreamFrom(), nResult=%l\n"), nResult));
        return nResult;
    }

    apOFPMsg = pMessage;

    return CM_OK;
}

CmResult COFPMessage::OFP13DecodeMessage(ACE_CDR::Octet abType, ACE_InputCDR &aIs, COFPMessage * &apOFPMsg)
{   
    using namespace OFP13;
    COFPMessage *pMessage = nullptr;

    switch (abType)
    {
        case OFPT_HELLO:
        {
            pMessage = new COFP13HelloMsg();    //(wLength - OPENFLOW_MESSAGE_HDR_SIZE);
            break;
        }
        case OFPT_ERROR:
        {
            pMessage = new COFP13ErrorMsg();
            break;
        }
        case OFPT_ECHO_REQUEST:
        {
            pMessage = new COFP13EchoRequestMsg();
            break;
        }
        case OFPT_ECHO_REPLY:
        {
            pMessage = new COFP13EchoReplyMsg();
            break;
        }
        case OFPT_EXPERIMENTER:
        {
            pMessage = new COFP13ExperimenterMsg();
            break;
        }
        /* Switch configuration messages. */
        case OFPT_FEATURES_REQUEST :
        {
            pMessage = new COFP13FeaturesRequestMsg();
            break;
        }
        case OFPT_FEATURES_REPLY:
        {
            pMessage = new COFP13FeaturesReplyMsg();
            break;
        }
        case OFPT_GET_CONFIG_REQUEST:
        {
            pMessage = new COFP13GetConfigRequestMsg();
            break;
        }
        case OFPT_GET_CONFIG_REPLY:
        {
            pMessage = new COFP13GetConfigReplyMsg();
            break;
        }
        case OFPT_SET_CONFIG:
        {
            pMessage = new COFP13SetConfigMsg();
            break;
        }
        /* Asynchronous messages. */
        case OFPT_PACKET_IN:
        {
            pMessage = new COFP13PacketInMsg();
            break;
        }
        case OFPT_FLOW_REMOVED:
            pMessage = new COFP13FlowRemovedMsg;
            break;
        case OFPT_PORT_STATUS:
        {
            pMessage = new COFP13PortStatusMsg();
            break;
        }
        /* Controller command messages. */
        case OFPT_PACKET_OUT:
        {
            pMessage = new COFP13PacketOutMsg();
            break;
        }
        case OFPT_FLOW_MOD:
        {
            pMessage = new COFP13FlowModMsg();
            break;
        }
        case OFPT_GROUP_MOD:
            pMessage = new COFP13GroupModMsg;
            break;
        case OFPT_PORT_MOD:
            pMessage = new COFP13PortModMsg;
            break;
        case OFPT_TABLE_MOD:
        {
            pMessage = new COFP13TableModMsg();
            break;
        }
        /* Multipart messages. */
        case OFPT_MULTIPART_REQUEST:
        {
            if (COFP13MultipartMsg::DecodeMPMessage(aIs,pMessage,true) != CM_OK)
            {
                ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13MultipartMsg::DecodeMPMessage error\n")));
                return CM_ERROR_FAILURE;
            }
            break;
        }
        case OFPT_MULTIPART_REPLY:
        {
            if (COFP13MultipartMsg::DecodeMPMessage(aIs,pMessage,false) != CM_OK)
            {
                return CM_ERROR_FAILURE;
            }

            break;
        }
        /* Barrier messages. */
        case OFPT_BARRIER_REQUEST:
            pMessage = new COFP13BarrierMsg(abType);
            break;
        case OFPT_BARRIER_REPLY:
            pMessage = new COFP13BarrierMsg(abType);
            break;
        /* Queue Configuration messages. */
        case OFPT_QUEUE_GET_CONFIG_REQUEST:
            pMessage = new COFP13QueueGetConfigRequestMsg;
            break;
        case OFPT_QUEUE_GET_CONFIG_REPLY:
            pMessage = new COFP13QueueGetConfigReplyMsg;
            break;
        /* Controller role change request messages. */
        case OFPT_ROLE_REQUEST:
            pMessage = new COFP13RoleMsg(abType);
            break;
        case OFPT_ROLE_REPLY:
            pMessage = new COFP13RoleMsg(abType);
            break;
        /* Asynchronous message configuration. */
        case OFPT_GET_ASYNC_REQUEST:
            pMessage = new COFP13GetAsyncRequestMsg;
            break;
        case OFPT_GET_ASYNC_REPLY:
            pMessage = new COFP13GetAsyncReplyMsg;
            break;
        case OFPT_SET_ASYNC:
            pMessage = new COFP13SetAsyncMsg;
            break;
        /* Meters and rate limiters configuration messages. */
        case OFPT_METER_MOD:
            pMessage = new COFP13MeterModMsg;
            break;
        default:
            ACE_ERROR((LM_ERROR, ACE_TEXT("COFPMessage::CreatePdu, type=%u\n"), abType));
            return CM_ERROR_NOT_FOUND;
    }

    CM_ASSERT_RETURN(pMessage != nullptr, CM_ERROR_OUT_OF_MEMORY);

    CmResult nResult = pMessage->StreamFrom(aIs);
    if (CM_FAILED(nResult)) {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFPMessage::CreatePdu,"" fail in pdu->StreamFrom(), nResult=%l\n"), nResult));
        return nResult;
    }

    apOFPMsg = pMessage;

    return CM_OK;
}

CmResult COFPMessage::StreamFrom(ACE_Message_Block &aMB)
{
    ACE_InputCDR is(&aMB);
    return StreamFrom(is);
}

CmResult COFPMessage::StreamFrom(ACE_InputCDR &is)
{
    is>>m_bVersion;
    is>>m_bType;
    is>>m_wLength;
    is>>m_dwxid;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("version=%u, type=%u, len=%u, xid=%u\n"), 
               m_bVersion, m_bType, m_wLength, m_dwxid));
    
    return CM_OK;
}

CmResult COFPMessage::StreamTo(ACE_Message_Block &aMB)
{
    ACE_OutputCDR os(&aMB, ACE_CDR::BYTE_ORDER_BIG_ENDIAN);
    return StreamTo(os);
}

CmResult COFPMessage::StreamTo(ACE_OutputCDR &os)
{
    m_wLength = GetStreamLen();

    os<<((ACE_OutputCDR::from_octet)m_bVersion);
    os<<((ACE_OutputCDR::from_octet)m_bType);
    os<<m_wLength;
    os<<m_dwxid;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

ACE_UINT16 COFPMessage::GetStreamLen()                          const
{
    return (sizeof(m_bVersion) + sizeof(m_bType) + sizeof(m_wLength) + sizeof(m_dwxid));
}

ACE_UINT16 COFPMessage::GetEncodeSpace()                        const
{
    ACE_UINT16 length = GetStreamLen();
    return ((length + 7)/8*8);
}

void COFPMessage::Dump()                                    const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPMessage::Dump()\n")));
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("version=%u, type=%u, length=%u, xid=%u\n"),
               m_bVersion, m_bType, m_wLength, m_dwxid));
}


