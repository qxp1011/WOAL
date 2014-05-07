#include "COFPStatsRequestMsg.h"

TEST(COFP10StatsRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wStatsType = 0x12ca;
    WORD16 wFlag = 0xfd2d;
    COFP10StatsRequestMsg Encode(wStatsType, wFlag);
    Encode.SetMessageXID(dwXid);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFP10StatsRequestMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REQUEST);

    
    EXPECT_EQ(Encode.GetStatsType(), Decode.GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), Decode.GetFlag());
    
    EXPECT_EQ(wStatsType, Decode.GetStatsType());
    EXPECT_EQ(wFlag, Decode.GetFlag());
}

TEST(COFP10DescStatsRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10DescStatsRequestMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10DescStatsRequestMsg *pDecode = 
        dynamic_cast<COFP10DescStatsRequestMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REQUEST);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());
    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_DESC, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());
}

TEST(COFP10FlowStatsRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_CDR::Octet bTableId = 255;
    WORD16 wPortNo = 0x238c;
    WORD16 wFlag = 0xfd2d;
    COFP10FlowStatsRequestMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);
    Encode.SetTableId(bTableId);
    Encode.SetOutPort(wPortNo);


    ACE_UINT32 dwWildcards = 123456;
    WORD16 wInPortNo = 7293;
    ACE_CDR::Octet acDlSrc[OFP_ETH_ALEN] = {0x22, 0x3a, 0xfa, 0x44, 0x24, 0xad};
    ACE_CDR::Octet acDlDst[OFP_ETH_ALEN] = {0xad, 0x7a, 0xaa, 0x24, 0x94, 0xaf};
    WORD16 wVlan = 237;
    ACE_CDR::Octet bVlanPcp = 30;
    WORD16 wDlType = 8388;
    ACE_CDR::Octet bNwTos = 200;
    ACE_CDR::Octet bNwProto = 129;
    ACE_UINT32 dwNwSrc = 0x64123987;
    ACE_UINT32 dwNwDst = 0x45902358;
    WORD16 wTpSrc = 12901;
    WORD16 wTpDst = 9834;
    
    COFP10Match *pMatch = Encode.GetMatch();
    EXPECT_NE(nullptr, pMatch);
    pMatch->SetWildcards(dwWildcards);
    pMatch->SetInPort(wInPortNo);
    pMatch->SetDlSrc(acDlSrc);
    pMatch->SetDlDst(acDlDst);
    pMatch->SetDlVlan(wVlan);
    pMatch->SetDlVlanPcp(bVlanPcp);
    pMatch->SetDlType(wDlType);
    pMatch->SetNwTos(bNwTos);
    pMatch->SetNwProto(bNwProto);
    pMatch->SetNwSrc(dwNwSrc);
    pMatch->SetNwDst(dwNwDst);
    pMatch->SetTpSrc(wTpSrc);
    pMatch->SetTpDst(wTpDst);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10FlowStatsRequestMsg *pDecode = 
        dynamic_cast<COFP10FlowStatsRequestMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REQUEST);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_FLOW, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());


    COFP10Match *pDecodeMatch = pDecode->GetMatch();
    EXPECT_NE(nullptr, pDecodeMatch);
    COFP10Match *pEncodeMatch = Encode.GetMatch();
    EXPECT_NE(nullptr, pEncodeMatch);
    EXPECT_EQ(true, *pDecodeMatch == *pEncodeMatch);

    
    EXPECT_EQ(pDecodeMatch->GetWildcards(), dwWildcards);
    EXPECT_EQ(pDecodeMatch->GetInPort(), wInPortNo);
    EXPECT_EQ(pDecodeMatch->GetDlVlan(), wVlan);
    EXPECT_EQ(pDecodeMatch->GetDlVlanPcp(), bVlanPcp);
    EXPECT_EQ(pDecodeMatch->GetDlType(), wDlType);
    EXPECT_EQ(pDecodeMatch->GetNwTos(), bNwTos);
    EXPECT_EQ(pDecodeMatch->GetNwProto(), bNwProto);
    EXPECT_EQ(pDecodeMatch->GetNwSrc(), dwNwSrc);
    EXPECT_EQ(pDecodeMatch->GetNwDst(), dwNwDst);
    EXPECT_EQ(pDecodeMatch->GetTpSrc(), wTpSrc);
    EXPECT_EQ(pDecodeMatch->GetTpDst(), wTpDst);

    const ACE_CDR::Octet *pDecodeDlSrc = pDecodeMatch->GetDlSrc();
    EXPECT_NE(nullptr, pDecodeDlSrc);
    const ACE_CDR::Octet *pDecodeDlDst = pDecodeMatch->GetDlDst();
    EXPECT_NE(nullptr, pDecodeDlDst);
    
    ACE_INT16 wCmp = ::memcmp(pDecodeDlSrc, acDlSrc, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeDlDst, acDlDst, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
}

TEST(COFP10AggregateStatsRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_CDR::Octet bTableId = 255;
    WORD16 wPortNo = 0x238c;
    WORD16 wFlag = 0xfd2d;
    COFP10AggregateStatsRequestMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);
    Encode.SetTableId(bTableId);
    Encode.SetOutPort(wPortNo);


    ACE_UINT32 dwWildcards = 123456;
    WORD16 wInPortNo = 7293;
    ACE_CDR::Octet acDlSrc[OFP_ETH_ALEN] = {0x22, 0x3a, 0xfa, 0x44, 0x24, 0xad};
    ACE_CDR::Octet acDlDst[OFP_ETH_ALEN] = {0xad, 0x7a, 0xaa, 0x24, 0x94, 0xaf};
    WORD16 wVlan = 237;
    ACE_CDR::Octet bVlanPcp = 30;
    WORD16 wDlType = 8388;
    ACE_CDR::Octet bNwTos = 200;
    ACE_CDR::Octet bNwProto = 129;
    ACE_UINT32 dwNwSrc = 0x64123987;
    ACE_UINT32 dwNwDst = 0x45902358;
    WORD16 wTpSrc = 12901;
    WORD16 wTpDst = 9834;
    
    COFP10Match *pMatch = Encode.GetMatch();
    EXPECT_NE(nullptr, pMatch);
    pMatch->SetWildcards(dwWildcards);
    pMatch->SetInPort(wInPortNo);
    pMatch->SetDlSrc(acDlSrc);
    pMatch->SetDlDst(acDlDst);
    pMatch->SetDlVlan(wVlan);
    pMatch->SetDlVlanPcp(bVlanPcp);
    pMatch->SetDlType(wDlType);
    pMatch->SetNwTos(bNwTos);
    pMatch->SetNwProto(bNwProto);
    pMatch->SetNwSrc(dwNwSrc);
    pMatch->SetNwDst(dwNwDst);
    pMatch->SetTpSrc(wTpSrc);
    pMatch->SetTpDst(wTpDst);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10AggregateStatsRequestMsg *pDecode = 
        dynamic_cast<COFP10AggregateStatsRequestMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REQUEST);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_AGGREGATE, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());


    COFP10Match *pDecodeMatch = pDecode->GetMatch();
    EXPECT_NE(nullptr, pDecodeMatch);
    COFP10Match *pEncodeMatch = Encode.GetMatch();
    EXPECT_NE(nullptr, pEncodeMatch);
    EXPECT_EQ(true, *pDecodeMatch == *pEncodeMatch);

    
    EXPECT_EQ(pDecodeMatch->GetWildcards(), dwWildcards);
    EXPECT_EQ(pDecodeMatch->GetInPort(), wInPortNo);
    EXPECT_EQ(pDecodeMatch->GetDlVlan(), wVlan);
    EXPECT_EQ(pDecodeMatch->GetDlVlanPcp(), bVlanPcp);
    EXPECT_EQ(pDecodeMatch->GetDlType(), wDlType);
    EXPECT_EQ(pDecodeMatch->GetNwTos(), bNwTos);
    EXPECT_EQ(pDecodeMatch->GetNwProto(), bNwProto);
    EXPECT_EQ(pDecodeMatch->GetNwSrc(), dwNwSrc);
    EXPECT_EQ(pDecodeMatch->GetNwDst(), dwNwDst);
    EXPECT_EQ(pDecodeMatch->GetTpSrc(), wTpSrc);
    EXPECT_EQ(pDecodeMatch->GetTpDst(), wTpDst);

    const ACE_CDR::Octet *pDecodeDlSrc = pDecodeMatch->GetDlSrc();
    EXPECT_NE(nullptr, pDecodeDlSrc);
    const ACE_CDR::Octet *pDecodeDlDst = pDecodeMatch->GetDlDst();
    EXPECT_NE(nullptr, pDecodeDlDst);
    
    ACE_INT16 wCmp = ::memcmp(pDecodeDlSrc, acDlSrc, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeDlDst, acDlDst, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
}

TEST(COFP10TableStatsRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10TableStatsRequestMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10TableStatsRequestMsg *pDecode = dynamic_cast<COFP10TableStatsRequestMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REQUEST);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());
    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_TABLE, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());
}

TEST(COFP10PortStatsRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wPortNo = 0x238c;
    WORD16 wFlag = 0xfd2d;
    COFP10PortStatsRequestMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);
    Encode.SetPortNo(wPortNo);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10PortStatsRequestMsg *pDecode = 
        dynamic_cast<COFP10PortStatsRequestMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REQUEST);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_PORT, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());

    EXPECT_EQ(Encode.GetPortNo(), pDecode->GetPortNo());
    EXPECT_EQ(wPortNo, pDecode->GetPortNo());
}


TEST(COFP10QueueStatsRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wPortNo = 0x238c;
    ACE_UINT32 dwQueueId = 0x83dcaef2;
    WORD16 wFlag = 0xfd2d;
    COFP10QueueStatsRequestMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);
    Encode.SetPortNo(wPortNo);
    Encode.SetQueueId(dwQueueId);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10QueueStatsRequestMsg *pDecode = 
        dynamic_cast<COFP10QueueStatsRequestMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REQUEST);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_QUEUE, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());

    EXPECT_EQ(Encode.GetPortNo(), pDecode->GetPortNo());
    EXPECT_EQ(Encode.GetQueueId(), pDecode->GetQueueId());
    EXPECT_EQ(wPortNo, pDecode->GetPortNo());
    EXPECT_EQ(dwQueueId, pDecode->GetQueueId());
}

