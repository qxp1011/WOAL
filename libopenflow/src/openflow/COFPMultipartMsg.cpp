/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMultipartMsg.cpp
 * Description:      Source file for openflow multipart message.
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

#include "COFPMultipartMsg.h"
#include "COFPMPDescriptionMsg.h"
#include "COFPMPFlowStatisticsMsg.h"
#include "COFPMPAggregateFlowStatisticsMsg.h"
#include "COFPMPTableStatisticsMsg.h"
#include "COFPMPPortStatisticsMsg.h"
#include "COFPMPQueueStatisticsMsg.h"
#include "COFPMPGroupStatisticsMsg.h"
#include "COFPMPGroupDescriptionMsg.h"
#include "COFPMPGroupFeaturesMsg.h"
#include "COFPMPMeterStatisticsMsg.h"
#include "COFPMPMeterConfigStatisticsMsg.h"
#include "COFPMPMeterFeaturesStatisticsMsg.h"
#include "COFPMPTableFeaturesMsg.h"
#include "COFPMPPortDescriptionMsg.h"
#include "ByteOrder.h"
#include "ace/Log_Msg.h"
#include "CmError.h"

using namespace openflow;

//static CmResult DecodeRequestMessage(CCmMessageBlock& aMB, COFP13MultipartMsg*&pOpenFlowMsg);
CmResult COFP13MultipartMsg::
DecodeMPMessage(ACE_InputCDR &is, COFPMessage *&pOpenFlowMsg, bool isRequest)
{
    ACE_CDR::Octet  pad[10];
    ACE_CDR::Octet *ptype = NULL;
    ACE_UINT16      atype;
    COFP13MultipartMsg *pMessage = NULL;
    
    bool bGood = is.pre_read(pad, sizeof(pad));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    ptype = pad+8;
    atype = *(ACE_UINT16*)ptype;
    atype = NetworkToHost16(atype);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MultipartMsg::DecodeMPMessage, m_type=%u\n"), atype));

    switch (atype)
    {
        case OFPMP_DESC:
        {
            if (isRequest)
                pMessage = new COFP13MPDescriptionRequestMsg();
            else
                pMessage = new COFP13MPDescriptionReplyMsg();
            break;
        }
        case OFPMP_FLOW:
        {
            if (isRequest)
                pMessage = new COFP13MPFlowStatsRequestMsg();
            else
                pMessage = new COFP13MPFlowStatsReplyMsg();
            break;
        }
        case OFPMP_AGGREGATE:
        {
            if (isRequest)
                pMessage = new COFP13MPAggregateFlowStatsRequestMsg();
            else
                pMessage = new COFP13MPAggregateFlowStatsReplyMsg();
            break;
        }
        case OFPMP_TABLE:
        {
            if (isRequest)
                pMessage = new COFP13MPTableStatsRequestMsg();
            else
                pMessage = new COFP13MPTableStatsReplyMsg();
            break;
        }
        case OFPMP_PORT_STATS:
        {
            if (isRequest)
                pMessage = new COFP13MPPortStatsRequestMsg();
            else
                pMessage = new COFP13MPPortStatsReplyMsg();
            break;
        }
        case OFPMP_QUEUE:
        {
            if (isRequest)
                pMessage = new COFP13MPQueueStatsRequestMsg();
            else
                pMessage = new COFP13MPQueueStatsReplyMsg();
            break;
        }
        case OFPMP_GROUP:
        {
            if (isRequest)
                pMessage = new COFP13MPGroupStatsRequestMsg();
            else
                pMessage = new COFP13MPGroupStatsReplyMsg();
            break;
        }
        case OFPMP_GROUP_DESC:
        {
            if (isRequest)
                pMessage = new COFP13MPGroupDescRequestMsg();
            else
                pMessage = new COFP13MPGroupDescReplyMsg();
            break;
        }
        case OFPMP_GROUP_FEATURES:
        {
            if (isRequest)
                pMessage = new COFP13MPGroupFeaturesRequestMsg();
            else
                pMessage = new COFP13MPGroupFeaturesReplyMsg();
            break;
        }
        case OFPMP_METER:
        {
            if (isRequest)
                pMessage = new COFP13MPMeterStaticRequestMsg();
            else
                pMessage = new COFP13MPMeterStaticReplyMsg();
            break;
        }
        case OFPMP_METER_CONFIG:
        {
            if (isRequest)
                pMessage = new COFP13MPMeterConfigStaticRequestMsg();
            else
                pMessage = new COFP13MPMeterConfigStaticReplyMsg();
            break;
        }
        case OFPMP_METER_FEATURES:
        {
            if (isRequest)
                pMessage = new COFP13MPMeterFeaturesStaticRequestMsg();
            else
                pMessage = new COFP13MPMeterFeaturesStaticReplyMsg();
            break;
        }
        case OFPMP_TABLE_FEATURES:
        {
            if (isRequest)
                pMessage = new COFP13MPTableFeaturesRequestMsg();
            else
                pMessage = new COFP13MPTableFeaturesReplyMsg();
            break;
        }
        case OFPMP_PORT_DESC:
        {
            if (isRequest)
                pMessage = new COFP13MPPortDescRequestMsg();
            else
                pMessage = new COFP13MPPortDescReplyMsg();
            break;
        }
        default:
            return CM_ERROR_NOT_FOUND;
    }

    CM_ASSERT_RETURN(pMessage, CM_ERROR_OUT_OF_MEMORY);

    pOpenFlowMsg = reinterpret_cast<COFPMessage *>(pMessage);

    return CM_OK;

}

CmResult COFP13MultipartMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_CDR::Octet pad[4]={0};
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13MultipartMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_type;
    is>>m_flags;
    is.read_octet_array(pad, sizeof(pad));
    
    bool bGood = is.good_bit();
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MultipartMsg::StreamFrom, m_type=%u\n"), m_type));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP13MultipartMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_CDR::Octet pad[4]={0};
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13MultipartMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }
    
    os<<m_type;
    os<<m_flags;
    os.write_octet_array(pad, sizeof(pad));
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


ACE_UINT16 COFP13MultipartMsg::GetStreamLen() const
{
    return sizeof(m_type)+sizeof(m_flags)+sizeof(m_pad)+COFPMessage::GetStreamLen();
}

