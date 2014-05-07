#include "COFPStatsReplyMsg.h"

TEST(COFP10StatsReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wStatsType = 0x12ca;
    WORD16 wFlag = 0xfd2d;
    COFP10StatsReplyMsg Encode;
    Encode.SetMessageXID(dwXid);
    Encode.SetStatsType(wStatsType);
    Encode.SetFlag(wFlag);
    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFP10StatsReplyMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REPLY);

    
    EXPECT_EQ(Encode.GetStatsType(), Decode.GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), Decode.GetFlag());
    
    EXPECT_EQ(wStatsType, Decode.GetStatsType());
    EXPECT_EQ(wFlag, Decode.GetFlag());
}

TEST(COFP10DescStatsReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10DescStatsReplyMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);

    string MfrDesc = "addfjadiofjad;fj2938-a;";
    string HwDesc = "djajp2i09cvq;eiru-";
    string SfDesc = "289jc;q9imc; p9a ";
    string SerNum = "8cna  a;d 9 ";
    string PathDesc = "1zci -923  a9d 923 ";
    lRet = Encode.SetManufacturerDesc((char *)MfrDesc.data());
    EXPECT_EQ(CM_OK, lRet);
    lRet = Encode.SetHardwareDesc((char *)HwDesc.data());
    EXPECT_EQ(CM_OK, lRet);
    lRet = Encode.SetSoftwareDesc((char *)SfDesc.data());
    EXPECT_EQ(CM_OK, lRet);
    lRet = Encode.SetSerialNum((char *)SerNum.data());
    EXPECT_EQ(CM_OK, lRet);
    lRet = Encode.SetDatapathDesc((char *)PathDesc.data());
    EXPECT_EQ(CM_OK, lRet);

    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10DescStatsReplyMsg *pDecode = 
        dynamic_cast<COFP10DescStatsReplyMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REPLY);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());
    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_DESC, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());

    const char *pDecodeStr = pDecode->GetManufacturerDesc();
    EXPECT_NE(nullptr, pDecodeStr);
    const char *pEncodeStr = Encode.GetManufacturerDesc();
    EXPECT_NE(nullptr, pEncodeStr);
    WORD16 wCmp = ::memcmp(pDecodeStr, pEncodeStr, DESC_STR_LEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeStr, MfrDesc.data(), ::strlen(MfrDesc.data()));
    EXPECT_EQ(0, wCmp);

    pDecodeStr = pDecode->GetHardwareDesc();
    EXPECT_NE(nullptr, pDecodeStr);
    pEncodeStr = Encode.GetHardwareDesc();
    EXPECT_NE(nullptr, pEncodeStr);
    wCmp = ::memcmp(pDecodeStr, pEncodeStr, DESC_STR_LEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeStr, HwDesc.data(), ::strlen(HwDesc.data()));
    EXPECT_EQ(0, wCmp);
    
    pDecodeStr = pDecode->GetSoftwareDesc();
    EXPECT_NE(nullptr, pDecodeStr);
    pEncodeStr = Encode.GetSoftwareDesc();
    EXPECT_NE(nullptr, pEncodeStr);
    wCmp = ::memcmp(pDecodeStr, pEncodeStr, DESC_STR_LEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeStr, SfDesc.data(), ::strlen(SfDesc.data()));
    EXPECT_EQ(0, wCmp);
    
    pDecodeStr = pDecode->GetSerialNum();
    EXPECT_NE(nullptr, pDecodeStr);
    pEncodeStr = Encode.GetSerialNum();
    EXPECT_NE(nullptr, pEncodeStr);
    wCmp = ::memcmp(pDecodeStr, pEncodeStr, SERIAL_NUM_LEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeStr, SerNum.data(), ::strlen(SerNum.data()));
    EXPECT_EQ(0, wCmp);

    pDecodeStr = pDecode->GetDatapathDesc();
    EXPECT_NE(nullptr, pDecodeStr);
    pEncodeStr = Encode.GetDatapathDesc();
    EXPECT_NE(nullptr, pEncodeStr);
    wCmp = ::memcmp(pDecodeStr, pEncodeStr, DESC_STR_LEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeStr, PathDesc.data(), ::strlen(PathDesc.data()));
    EXPECT_EQ(0, wCmp);
}

TEST(COFP10FlowStatsReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10FlowStatsReplyMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);


    COFP10FlowStatsReplyEle *Ele = new COFP10FlowStatsReplyEle;
    EXPECT_NE(nullptr, Ele);
    ACE_CDR::Octet bId = 248;
    ACE_UINT32 dwSec = 0x2dcdacf8;
    ACE_UINT32 dwNsec = 0x9ad9cd23;
    WORD16 wPri = 0xd8cf;
    WORD16 wTimeout = 0x2dac;
    ACE_UINT64 lCookie = 0x2e03cdacf80390f0;
    ACE_UINT64 lCnt = 0x829dcf9afbbbac09;
    Ele->SetTableId(bId);
    Ele->SetDurationSec(dwSec);
    Ele->SetDurationNsec(dwNsec);
    Ele->SetPriority(wPri);
    Ele->SetIdleTimeout(wTimeout);
    Ele->SetHardTimeout(wTimeout);
    Ele->SetCookie(lCookie);
    Ele->SetPacketCount(lCnt);
    Ele->SetByteCount(lCnt);

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

    COFP10Match *pMatch = Ele->GetMatch();
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
    
    WORD16 wActOutPort = 8237;
    WORD16 wMaxLen = 24341;
    COFP10Action *Act = new COFP10ActOutput(wActOutPort, wMaxLen);
    EXPECT_NE(nullptr, Act);
    lRet = Ele->AppendAction(Act);
    EXPECT_EQ(CM_OK, lRet);

    lRet = Encode.AppendEle(Ele);
    EXPECT_EQ(CM_OK, lRet);
    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10FlowStatsReplyMsg *pDecode = 
        dynamic_cast<COFP10FlowStatsReplyMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REPLY);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_FLOW, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());

    
    std::list<COFP10FlowStatsReplyEle *> DecodeList;
    lRet = pDecode->GetEleList(DecodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10FlowStatsReplyEle *>::iterator DecodeIt = DecodeList.begin();
    if (DecodeIt == DecodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10FlowStatsReplyEle *pDecodeEle = dynamic_cast<COFP10FlowStatsReplyEle *>((*DecodeIt));
    EXPECT_NE(nullptr, pDecodeEle);

    
    std::list<COFP10FlowStatsReplyEle *> EncodeList;
    lRet = Encode.GetEleList(EncodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10FlowStatsReplyEle *>::iterator EncodeIt = EncodeList.begin();
    if (EncodeIt == EncodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10FlowStatsReplyEle *pEncodeEle = dynamic_cast<COFP10FlowStatsReplyEle *>((*EncodeIt));
    EXPECT_NE(nullptr, pEncodeEle);
    

    EXPECT_EQ(pEncodeEle->GetBodyLen(), pDecodeEle->GetBodyLen());
    EXPECT_EQ(pEncodeEle->GetTableId(), pDecodeEle->GetTableId());
    EXPECT_EQ(pEncodeEle->GetDurationSec(), pDecodeEle->GetDurationSec());
    EXPECT_EQ(pEncodeEle->GetDurationNsec(), pDecodeEle->GetDurationNsec());
    EXPECT_EQ(pEncodeEle->GetPriority(), pDecodeEle->GetPriority());
    EXPECT_EQ(pEncodeEle->GetIdleTimeout(), pDecodeEle->GetIdleTimeout());
    EXPECT_EQ(pEncodeEle->GetHardTimeout(), pDecodeEle->GetHardTimeout());
    EXPECT_EQ(pEncodeEle->GetCookie(), pDecodeEle->GetCookie());
    EXPECT_EQ(pEncodeEle->GetPacketCount(), pDecodeEle->GetPacketCount());
    EXPECT_EQ(pEncodeEle->GetByteCount(), pDecodeEle->GetByteCount());
    
    EXPECT_EQ(bId, pDecodeEle->GetTableId());
    EXPECT_EQ(dwSec, pDecodeEle->GetDurationSec());
    EXPECT_EQ(dwNsec, pDecodeEle->GetDurationNsec());
    EXPECT_EQ(wPri, pDecodeEle->GetPriority());
    EXPECT_EQ(wTimeout, pDecodeEle->GetIdleTimeout());
    EXPECT_EQ(wTimeout, pDecodeEle->GetHardTimeout());
    EXPECT_EQ(lCookie, pDecodeEle->GetCookie());
    EXPECT_EQ(lCnt, pDecodeEle->GetPacketCount());
    EXPECT_EQ(lCnt, pDecodeEle->GetByteCount());

    COFP10Match *pDecodeMatch = pDecodeEle->GetMatch();
    EXPECT_NE(nullptr, pDecodeMatch);
    COFP10Match *pEncodeMatch = pEncodeEle->GetMatch();
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

    std::list<COFP10Action *> DecodeActList;
    lRet = pDecodeEle->GetActionList(DecodeActList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10Action *>::iterator DecodeActIt = DecodeActList.begin();
    if (DecodeActIt == DecodeActList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10ActOutput *pDecodeAct = dynamic_cast<COFP10ActOutput *>((*DecodeActIt));
    EXPECT_NE(nullptr, pDecodeAct);

    
    std::list<COFP10Action *> EncodeActList;
    lRet = pEncodeEle->GetActionList(EncodeActList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10Action *>::iterator EncodeActIt = EncodeActList.begin();
    if (EncodeActIt == EncodeActList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10ActOutput *pEncodeAct = dynamic_cast<COFP10ActOutput *>((*EncodeActIt));
    EXPECT_NE(nullptr, pEncodeAct);
    
    EXPECT_EQ(pEncodeAct->GetActionType(), pDecodeAct->GetActionType());
    EXPECT_EQ(pEncodeAct->GetActionLen(), pDecodeAct->GetActionLen());
    EXPECT_EQ(pEncodeAct->GetOutPort(), pDecodeAct->GetOutPort());
    EXPECT_EQ(pEncodeAct->GetMaxLen(), pDecodeAct->GetMaxLen());
    
    EXPECT_EQ(COFP10Action::OFPAT_OUTPUT, pDecodeAct->GetActionType());
    EXPECT_EQ(8, pDecodeAct->GetActionLen());
    EXPECT_EQ(wActOutPort, pDecodeAct->GetOutPort());
    EXPECT_EQ(wMaxLen, pDecodeAct->GetMaxLen());
}

TEST(COFP10AggregateStatsReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10AggregateStatsReplyMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);

    ACE_UINT64 lPktCnt = 0x92cadfbce9082d11;
    ACE_UINT64 lByteCnt = 0xfccadf27e9082d1d;
    ACE_UINT32 dwFlowCnt = 0x972c78cf;
    Encode.SetPacketCount(lPktCnt);
    Encode.SetByteCount(lByteCnt);
    Encode.SetFlowCount(dwFlowCnt);

    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10AggregateStatsReplyMsg *pDecode = 
        dynamic_cast<COFP10AggregateStatsReplyMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REPLY);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_AGGREGATE, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());

    
    EXPECT_EQ(Encode.GetPacketCount(), pDecode->GetPacketCount());
    EXPECT_EQ(Encode.GetByteCount(), pDecode->GetByteCount());
    EXPECT_EQ(Encode.GetFlowCount(), pDecode->GetFlowCount());
    
    EXPECT_EQ(lPktCnt, pDecode->GetPacketCount());
    EXPECT_EQ(lByteCnt, pDecode->GetByteCount());
    EXPECT_EQ(dwFlowCnt, pDecode->GetFlowCount());
}

TEST(COFP10TableStatsReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10TableStatsReplyMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);


    ACE_CDR::Octet bTableId = 255;
    string Name = "daipc9if9 ds9a";
    ACE_UINT32 dwWildcards = 0x8c02acb0;
    ACE_UINT32 dwMaxEntries = 0x9cac8204;
    ACE_UINT32 dwActiveCount = 0x92cfe20c;
    ACE_UINT64 lLookupCount = 0x892103cf92be9200;
    ACE_UINT64 lMatchedCount = 0x8923cfac0938cfaa;
    
    COFP10TableStatsReplyMsg::TableStatsReply tTable;
    tTable.table_id = bTableId;
    ::memcpy(tTable.name, Name.data(), ::strlen(Name.data()));
    tTable.wildcards = dwWildcards;
    tTable.max_entries = dwMaxEntries;
    tTable.active_count = dwActiveCount;
    tTable.lookup_count = lLookupCount;
    tTable.matched_count = lMatchedCount;

    lRet = Encode.AppendTable(tTable);
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10TableStatsReplyMsg *pDecode = 
        dynamic_cast<COFP10TableStatsReplyMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REPLY);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_TABLE, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());


    std::list<COFP10TableStatsReplyMsg::TableStatsReply> DecodeList;
    lRet = pDecode->GetTableList(DecodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10TableStatsReplyMsg::TableStatsReply>::iterator DecodeIt = DecodeList.begin();
    if (DecodeIt == DecodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10TableStatsReplyMsg::TableStatsReply *pDecodeTable = &(*DecodeIt);
    EXPECT_NE(nullptr, pDecodeTable);

    
    std::list<COFP10TableStatsReplyMsg::TableStatsReply> EncodeList;
    lRet = Encode.GetTableList(EncodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10TableStatsReplyMsg::TableStatsReply>::iterator EncodeIt = EncodeList.begin();
    if (EncodeIt == EncodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10TableStatsReplyMsg::TableStatsReply *pEncodeTable = &(*EncodeIt);
    EXPECT_NE(nullptr, pEncodeTable);


    WORD16 wCmp = ::memcmp(pDecodeTable, pEncodeTable, sizeof(*pEncodeTable));
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeTable->name, Name.data(), sizeof(Name.data()));
    EXPECT_EQ(0, wCmp);
    EXPECT_EQ(bTableId, pDecodeTable->table_id);
    EXPECT_EQ(dwWildcards, pDecodeTable->wildcards);
    EXPECT_EQ(dwMaxEntries, pDecodeTable->max_entries);
    EXPECT_EQ(dwActiveCount, pDecodeTable->active_count);
    EXPECT_EQ(lLookupCount, pDecodeTable->lookup_count);
    EXPECT_EQ(lMatchedCount, pDecodeTable->matched_count);    
}

TEST(COFP10PortStatsReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10PortStatsReplyMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);


    WORD16 port_no = 0x829c;
    ACE_UINT64 rx_packets = 0x9d2daabb90c3be83;
    ACE_UINT64 tx_packets = 0x9d2da0c3beabb021;
    ACE_UINT64 rx_bytes = 0xac2da0c3bfe1b021;
    ACE_UINT64 tx_bytes = 0xfc1da2c3bfc3b021;
    ACE_UINT64 rx_dropped = 0x9d2df3c3beaba0c1;
    ACE_UINT64 tx_dropped = 0xffaf2ca0c3cab021;
    ACE_UINT64 rx_errors = 0x9d2da0c322445566;
    ACE_UINT64 tx_errors = 0xfaccfda3beabb021;
    ACE_UINT64 rx_frame_err = 0x9fdfac0c3aecb0f1;
    ACE_UINT64 rx_over_err = 0x8cac2da0f3beab21;
    ACE_UINT64 rx_crc_err = 0x028da0c0c90df056;
    ACE_UINT64 collisions = 0x00c2da0c3babb021;

    COFP10PortStatsReplyMsg::PortStatsReply tPort;    
    tPort.port_no = port_no;
    tPort.rx_packets = rx_packets;
    tPort.tx_packets = tx_packets;
    tPort.rx_bytes = rx_bytes;
    tPort.tx_bytes = tx_bytes;
    tPort.rx_dropped = rx_dropped;
    tPort.tx_dropped = tx_dropped;
    tPort.rx_errors = rx_errors;
    tPort.tx_errors = tx_errors;
    tPort.rx_frame_err = rx_frame_err;
    tPort.rx_over_err = rx_over_err;
    tPort.rx_crc_err = rx_crc_err;
    tPort.collisions = collisions;

    lRet = Encode.AppendPort(tPort);
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10PortStatsReplyMsg *pDecode = 
        dynamic_cast<COFP10PortStatsReplyMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REPLY);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_PORT, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());


    std::list<COFP10PortStatsReplyMsg::PortStatsReply> DecodeList;
    lRet = pDecode->GetPortList(DecodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10PortStatsReplyMsg::PortStatsReply>::iterator DecodeIt = DecodeList.begin();
    if (DecodeIt == DecodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10PortStatsReplyMsg::PortStatsReply *pDecodePort = &(*DecodeIt);
    EXPECT_NE(nullptr, pDecodePort);

    
    std::list<COFP10PortStatsReplyMsg::PortStatsReply> EncodeList;
    lRet = Encode.GetPortList(EncodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10PortStatsReplyMsg::PortStatsReply>::iterator EncodeIt = EncodeList.begin();
    if (EncodeIt == EncodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10PortStatsReplyMsg::PortStatsReply *pEncodePort = &(*EncodeIt);
    EXPECT_NE(nullptr, pEncodePort);


    WORD16 wCmp = ::memcmp(pDecodePort, pEncodePort, sizeof(*pDecodePort));
    EXPECT_EQ(0, wCmp);
    EXPECT_EQ(port_no, pDecodePort->port_no);
    EXPECT_EQ(rx_packets, pDecodePort->rx_packets);
    EXPECT_EQ(tx_packets, pDecodePort->tx_packets);
    EXPECT_EQ(rx_bytes, pDecodePort->rx_bytes);
    EXPECT_EQ(tx_bytes, pDecodePort->tx_bytes);
    EXPECT_EQ(rx_dropped, pDecodePort->rx_dropped); 
    EXPECT_EQ(tx_dropped, pDecodePort->tx_dropped);
    EXPECT_EQ(rx_errors, pDecodePort->rx_errors);
    EXPECT_EQ(tx_errors, pDecodePort->tx_errors);
    EXPECT_EQ(rx_frame_err, pDecodePort->rx_frame_err);
    EXPECT_EQ(rx_over_err, pDecodePort->rx_over_err);
    EXPECT_EQ(rx_crc_err, pDecodePort->rx_crc_err); 
    EXPECT_EQ(collisions, pDecodePort->collisions); 
}

TEST(COFP10QueueStatsReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wFlag = 0xfd2d;
    COFP10QueueStatsReplyMsg Encode(wFlag);
    Encode.SetMessageXID(dwXid);

    
    WORD16 port_no = 0x829c;
    ACE_UINT32 queue_id = 0x82afecbf;
    ACE_UINT64 tx_bytes = 0xac2da0c3bfe1b021;
    ACE_UINT64 tx_packets = 0xfaccfda3beabb021;
    ACE_UINT64 tx_errors = 0x028da0c0c90df056;

    COFP10QueueStatsReplyMsg::QueueStatsReply tQueue;    
    tQueue.port_no = port_no;
    tQueue.queue_id = queue_id;
    tQueue.tx_bytes = tx_bytes;
    tQueue.tx_packets = tx_packets;
    tQueue.tx_errors = tx_errors;

    lRet = Encode.AppendQueue(tQueue);
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10QueueStatsReplyMsg *pDecode = 
        dynamic_cast<COFP10QueueStatsReplyMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_STATS_REPLY);

    
    EXPECT_EQ(Encode.GetStatsType(), pDecode->GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), pDecode->GetFlag());    
    EXPECT_EQ(COFP10StatsMsgHeader::OFPST_QUEUE, pDecode->GetStatsType());
    EXPECT_EQ(wFlag, pDecode->GetFlag());


    std::list<COFP10QueueStatsReplyMsg::QueueStatsReply> DecodeList;
    lRet = pDecode->GetQueueList(DecodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10QueueStatsReplyMsg::QueueStatsReply>::iterator DecodeIt = DecodeList.begin();
    if (DecodeIt == DecodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10QueueStatsReplyMsg::QueueStatsReply *pDecodeQueue = &(*DecodeIt);
    EXPECT_NE(nullptr, pDecodeQueue);

    
    std::list<COFP10QueueStatsReplyMsg::QueueStatsReply> EncodeList;
    lRet = Encode.GetQueueList(EncodeList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10QueueStatsReplyMsg::QueueStatsReply>::iterator EncodeIt = EncodeList.begin();
    if (EncodeIt == EncodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10QueueStatsReplyMsg::QueueStatsReply *pEncodeQueue = &(*EncodeIt);
    EXPECT_NE(nullptr, pEncodeQueue);


    WORD16 wCmp = ::memcmp(pDecodeQueue, pEncodeQueue, sizeof(*pEncodeQueue));
    EXPECT_EQ(0, wCmp);
    EXPECT_EQ(port_no, pDecodeQueue->port_no);
    EXPECT_EQ(queue_id, pDecodeQueue->queue_id);
    EXPECT_EQ(tx_bytes, pDecodeQueue->tx_bytes);
    EXPECT_EQ(tx_packets, pDecodeQueue->tx_packets); 
    EXPECT_EQ(tx_errors, pDecodeQueue->tx_errors);
}

