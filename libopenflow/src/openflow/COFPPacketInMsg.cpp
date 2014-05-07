/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPacketInMsg.cpp
 * Description:      Source file for openflow packet-in message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-21
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPPacketInMsg.h"
#include "CmError.h"
#include "ace/Log_Msg.h"

COFP10PacketInMsg::~COFP10PacketInMsg()
{
    if (m_PacketData)
    {
        delete [] m_PacketData;
        m_PacketData = NULL;
    }
}

#if 0
const BYTE * COFP10PacketInMsg::GetEtherFrame() const
{
    const BYTE *pData = NULL;
    
    if (m_PacketData)
    {
        CCmString str = m_PacketData->FlattenChained();
        pData = (const BYTE *)str.c_str();
    }

    return pData;
}
#endif

ACE_CDR::Octet *COFP10PacketInMsg::GetPacketData() const
{
    return m_PacketData;
}

ACE_UINT16 COFP10PacketInMsg::GetPacketDataLength() const
{
    return m_tMember.total_len;
}


#if 0
CmResult COFP10PacketInMsg::AppendPacketData(CCmMessageBlock &aData)
{
    if (m_PacketData)
    {
        m_PacketData->Append(aData.DuplicateChained());
    }
    else
    {
        m_PacketData = aData.DuplicateChained();
    }

    m_wDataLen += aData.GetChainedLength();

    return CM_OK;
}

CmResult COFP10PacketInMsg::AppendPacketData(BYTE *apData, WORD16 awDataLen)
{
    //CM_ASSERTE_RETURN(apData, CM_ERROR_FAILURE);
    //CM_ASSERTE_RETURN(awDataLen, CM_ERROR_FAILURE);
    
    //CCmMessageBlock Data(awDataLen, (LPCSTR)apData, 0, awDataLen);

    //return AppendPacketData(Data);
}

#endif

CmResult COFP10PacketInMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PacketInMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    is>>m_tMember.buffer_id;
    is>>m_tMember.total_len;
    is>>m_tMember.in_port;
    is>>m_tMember.reason;
    is>>m_tMember.pad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    if (m_wLength <= COFPMessage::GetStreamLen() + sizeof(m_tMember))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PacketInMsg::StreamFrom, packet data len(%u) is invalid.\n"), m_wLength));
        ACE_ASSERT(0);
        return CM_ERROR_FAILURE;
    }

    m_wDataLen = m_wLength - COFPMessage::GetStreamLen() - sizeof(m_tMember);

    if (m_tMember.total_len < m_wDataLen)
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP10PacketInMsg::StreamFrom, m_wLength=%u, total_len=%u.\n"), 
                   m_wLength, 
                   m_tMember.total_len));
        return CM_ERROR_FAILURE;
    }

    m_PacketData = (ACE_CDR::Octet*)(new char[m_wDataLen]);
    if (m_PacketData == NULL)
    {
        return CM_ERROR_FAILURE;
    }

    bGood = is.read_octet_array(m_PacketData, m_tMember.total_len);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP10PacketInMsg::StreamTo(ACE_OutputCDR &os)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP10PacketInMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    os<<m_tMember.buffer_id;
    os<<m_tMember.total_len;
    os<<m_tMember.in_port;
    os<<m_tMember.reason;
    os<<m_tMember.pad;
    if ((m_PacketData != NULL) && (m_wDataLen > 0))
    {
        //CM_INFO_TRACE("m_PacketData->GetChainedLength() = " <<m_PacketData->GetChainedLength());

        //CCmString str = m_PacketData->FlattenChained();
        os.write_octet_array(m_PacketData, m_wDataLen);
    }
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

void COFP10PacketInMsg::Dump() const
{
    COFPMessage::Dump();
}





std::allocator<ACE_CDR::Octet> COFP13PacketInMsg::m_alloc;

COFP13PacketInMsg::COFP13PacketInMsg()
    : COFPMessage(OFP13::OFPT_PACKET_IN, OFP13_VERSION, 0, 32)
    , m_nBufferId(0) /* ID assigned by datapath. */
    , m_wTotalLen(0) /* Full length of frame. */
    , m_bReason(0)   /* Reason packet is being sent (one of OFPR_*) */
    , m_bTableId(0)  /* ID of the table that was looked up */
    , m_nCookie(0)   /* Cookie of the flow entry that was looked up. */
    , m_tMatch()     /* Packet metadata. Variable size. */
    , m_pEtherFrame(NULL)
    , m_nEtherFrameLen(0)
    , m_nEtherMemSize(0)
 {}

 
COFP13PacketInMsg::COFP13PacketInMsg(
    ACE_CDR::Octet   abReason,             /* Reason packet is being sent (one of OFPR_*) */
    ACE_UINT32 anBufferId,           /* ID assigned by datapath. */
    ACE_CDR::Octet   abTableId,            /* ID of the table that was looked up */
    ACE_UINT64 anCookie,             /* Cookie of the flow entry that was looked up. */
    ACE_CDR::Octet  *apEtherFrame,
    ACE_UINT32 anEtherFrameLen,
    ACE_UINT16 anTotalLen,           /* Full length of frame. */
    /* args for OFP Match */
    ACE_UINT16      awMatchType,
    /* args for OFP Header */
    ACE_UINT32  anOFPHeadXId,
    ACE_CDR::Octet    abOFPHeadVer)
    : COFPMessage(OFP13::OFPT_PACKET_IN, abOFPHeadVer, anOFPHeadXId, 16 + 2 + anEtherFrameLen)
    , m_nBufferId(anBufferId) /* ID assigned by datapath. */
    , m_wTotalLen(anTotalLen) /* Full length of frame. */
    , m_bReason(abReason)     /* Reason packet is being sent (one of OFPR_*) */
    , m_bTableId(abTableId)   /* ID of the table that was looked up */
    , m_nCookie(anCookie)     /* Cookie of the flow entry that was looked up. */
    , m_tMatch(awMatchType)
    , m_pEtherFrame(NULL)
    , m_nEtherFrameLen(0)
    , m_nEtherMemSize(0)
{
    if (NULL == apEtherFrame)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowPacketIn::COpenFlowPacketIn(): arg in, apEtherFrame, is NULL.\n")));
        return;
    }

    if (0 == anEtherFrameLen)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowPacketIn::COpenFlowPacketIn(): anEtherFrameLen = 0\n")));
        return;
    }
    
    m_pEtherFrame = m_alloc.allocate(anEtherFrameLen);
    if (NULL == m_pEtherFrame)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COpenFlowPacketIn::COpenFlowPacketIn(): m_alloc.allocate(%u) failed.\n"), 
                   anEtherFrameLen));
        return;
    }
    ::memcpy(m_pEtherFrame, apEtherFrame, anEtherFrameLen);
    m_nEtherFrameLen = anEtherFrameLen;
    m_nEtherMemSize  = anEtherFrameLen;
}


COFP13PacketInMsg::~COFP13PacketInMsg()
{
    if (m_pEtherFrame != NULL)
    {
        m_alloc.deallocate(m_pEtherFrame, m_nEtherMemSize);
        m_pEtherFrame = NULL;
    }
}


CmResult COFP13PacketInMsg::
SetEtherFrame(const ACE_CDR::Octet * apEtherFrame, ACE_UINT32 anEtherFrameLen)
{
    if (NULL == apEtherFrame)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COpenFlowPacketIn::SetEtherFrame(): arg in, apEtherFrame, is NULL.\n")));
        return CM_ERROR_NULL_POINTER;
    }

    if (0 == anEtherFrameLen)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COpenFlowPacketIn::SetEtherFrame(): anEtherFrameLen = 0\n")));
        return CM_ERROR_INVALID_ARG;
    }

    if (anEtherFrameLen <= m_nEtherMemSize)
    {
        m_nEtherFrameLen = anEtherFrameLen;
        ::memcpy(m_pEtherFrame, apEtherFrame, anEtherFrameLen);
    }
    else
    {
        m_alloc.deallocate(m_pEtherFrame, m_nEtherMemSize);
        m_nEtherMemSize  = 0;
        m_nEtherFrameLen = 0;
        m_pEtherFrame = m_alloc.allocate(anEtherFrameLen);
        if (NULL == m_pEtherFrame)
        {
            ACE_ASSERT(0);
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COpenFlowPacketIn::SetEtherFrame(): m_alloc.allocate(%u) failed.\n"),
                       anEtherFrameLen));
            return CM_ERROR_OUT_OF_MEMORY;
        }
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COpenFlowPacketIn::SetEtherFrame(): m_nEtherMemSize changed from %u to %u\n"),
                   m_nEtherMemSize,
                   anEtherFrameLen));
        m_nEtherMemSize  = anEtherFrameLen;
        m_nEtherFrameLen = anEtherFrameLen;
        ::memcpy(m_pEtherFrame, apEtherFrame, anEtherFrameLen);
    }
    
    return CM_OK;
}


CmResult COFP13PacketInMsg::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT16 wPad = 0;
    ACE_UINT32 nEtherFrameNewLen = 0;
    CmResult result = CM_ERROR_FAILURE;
    
    result = COFPMessage::StreamFrom(is);
    if (CM_FAILED(result))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PacketInMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return result;
    }
    
    is>>m_nBufferId;
    is>>m_wTotalLen;
    is>>m_bReason;
    is>>m_bTableId;
    is>>m_nCookie;
    result = m_tMatch.StreamFrom(is);
    CM_ASSERT_RETURN((CM_OK == result), result);
    is>>wPad;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    nEtherFrameNewLen = GetMessageLength() - COFPMessage::GetStreamLen() - 16 - m_tMatch.GetEncodeSpace() - 2; // 2 BYTE paddings
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("COpenFlowPacketIn::StreamFrom(): nEtherFrameNewLen=%u, m_nEtherMemSize=%u\n"),
               nEtherFrameNewLen,
               m_nEtherMemSize));
    if (nEtherFrameNewLen <= m_nEtherMemSize)
    {
        m_nEtherFrameLen = nEtherFrameNewLen;
        bGood = is.read_octet_array(m_pEtherFrame, m_nEtherFrameLen);
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);       
    }
    else
    {
        if (m_pEtherFrame != NULL)
        {
            m_alloc.deallocate(m_pEtherFrame, m_nEtherMemSize);
            m_pEtherFrame    = NULL;
            m_nEtherMemSize  = 0;
            m_nEtherFrameLen = 0;
        }
        
        m_pEtherFrame = m_alloc.allocate(nEtherFrameNewLen);
        if (NULL == m_pEtherFrame)
        {
            ACE_ASSERT(0);
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COpenFlowPacketIn::StreamFrom(): m_alloc.allocate(%u) failed.\n"), 
                       nEtherFrameNewLen));
            return CM_ERROR_OUT_OF_MEMORY;
        }
        m_nEtherMemSize  = nEtherFrameNewLen;

        is.read_octet_array(m_pEtherFrame, nEtherFrameNewLen);
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

        m_nEtherFrameLen = nEtherFrameNewLen;
    }
    
    return CM_OK;
}


CmResult COFP13PacketInMsg::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT16 wPad = 0;
    CmResult result = CM_ERROR_FAILURE;
    
    result = COFPMessage::StreamTo(os);
    if (CM_FAILED(result))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13PacketInMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return result;
    }
    
    os<<m_nBufferId;
    os<<m_wTotalLen;
    os<<m_bReason;
    os<<m_bTableId;
    os<<m_nCookie;
    result = m_tMatch.StreamTo(os);
    CM_ASSERT_RETURN((CM_OK == result), result);
    os<<wPad;
    
    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    os.write_octet_array(m_pEtherFrame, m_nEtherFrameLen);
    bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    
    return CM_OK;
}


void COFP13PacketInMsg::Dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COpenFlowPacketIn::Dump():\n")));
    
    COFPMessage::Dump();
    
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("bufferId=%u, totalLen=%u, reason=%u, tableId=%u, cookie=%u\n"),
               m_nBufferId,
               m_wTotalLen,
               m_bReason,
               m_bTableId,
               m_nCookie));
    m_tMatch.Dump();
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("pEtherFrame=%p, EtherFrameLen=%u, EtherFrameMemSize=%u\n"),
               m_pEtherFrame,
               m_nEtherFrameLen,
               m_nEtherMemSize));
    if (m_pEtherFrame != NULL)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("head: *pEtherFrame=%u\n"), *m_pEtherFrame));
        if (m_nEtherFrameLen > 1)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("tail: *(pEtherFrame+len-1)=%u\n"), *(m_pEtherFrame + m_nEtherFrameLen - 1)));
        }
    }
}


