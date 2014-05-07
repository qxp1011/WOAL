#include "COFPStatsMsgHeader.h"

TEST(COFP10StatsMsgHeaderTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_CDR::Octet bMsgType = 241;
    WORD16 wStatsType = 0x12ca;
    WORD16 wFlag = 0xfd2d;
    COFP10StatsMsgHeader Encode(bMsgType, wStatsType, wFlag);
    Encode.SetMessageXID(dwXid);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFP10StatsMsgHeader Decode(0);
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), bMsgType);

    
    EXPECT_EQ(Encode.GetStatsType(), Decode.GetStatsType());
    EXPECT_EQ(Encode.GetFlag(), Decode.GetFlag());
    
    EXPECT_EQ(wStatsType, Decode.GetStatsType());
    EXPECT_EQ(wFlag, Decode.GetFlag());
}

TEST(COFP10FlowStatsReplyEleTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_CDR::Octet bId = 248;
    ACE_UINT32 dwSec = 0x2dcdacf8;
    ACE_UINT32 dwNsec = 0x9ad9cd23;
    WORD16 wPri = 0xd8cf;
    WORD16 wTimeout = 0x2dac;
    ACE_UINT64 lCookie = 0x2e03cdacf80390f0;
    ACE_UINT64 lCnt = 0x829dcf9afbbbac09;
    
    COFP10FlowStatsReplyEle Encode;    
    Encode.SetTableId(bId);
    Encode.SetDurationSec(dwSec);
    Encode.SetDurationNsec(dwNsec);
    Encode.SetPriority(wPri);
    Encode.SetIdleTimeout(wTimeout);
    Encode.SetHardTimeout(wTimeout);
    Encode.SetCookie(lCookie);
    Encode.SetPacketCount(lCnt);
    Encode.SetByteCount(lCnt);

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

    WORD16 wActOutPort = 8237;
    WORD16 wMaxLen = 24341;
    COFP10Action *Act = new COFP10ActOutput(wActOutPort, wMaxLen);
    EXPECT_NE(nullptr, Act);
    lRet = Encode.AppendAction(Act);
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFP10FlowStatsReplyEle Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    EXPECT_EQ(Encode.GetBodyLen(), Decode.GetBodyLen());
    EXPECT_EQ(Encode.GetTableId(), Decode.GetTableId());
    EXPECT_EQ(Encode.GetDurationSec(), Decode.GetDurationSec());
    EXPECT_EQ(Encode.GetDurationNsec(), Decode.GetDurationNsec());
    EXPECT_EQ(Encode.GetPriority(), Decode.GetPriority());
    EXPECT_EQ(Encode.GetIdleTimeout(), Decode.GetIdleTimeout());
    EXPECT_EQ(Encode.GetHardTimeout(), Decode.GetHardTimeout());
    EXPECT_EQ(Encode.GetCookie(), Decode.GetCookie());
    EXPECT_EQ(Encode.GetPacketCount(), Decode.GetPacketCount());
    EXPECT_EQ(Encode.GetByteCount(), Decode.GetByteCount());
    
    EXPECT_EQ(bId, Decode.GetTableId());
    EXPECT_EQ(dwSec, Decode.GetDurationSec());
    EXPECT_EQ(dwNsec, Decode.GetDurationNsec());
    EXPECT_EQ(wPri, Decode.GetPriority());
    EXPECT_EQ(wTimeout, Decode.GetIdleTimeout());
    EXPECT_EQ(wTimeout, Decode.GetHardTimeout());
    EXPECT_EQ(lCookie, Decode.GetCookie());
    EXPECT_EQ(lCnt, Decode.GetPacketCount());
    EXPECT_EQ(lCnt, Decode.GetByteCount());

    COFP10Match *pDecodeMatch = Decode.GetMatch();
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

    std::list<COFP10Action *> DecodeActList;
    lRet = Decode.GetActionList(DecodeActList);
    EXPECT_EQ(CM_OK, lRet);
    std::list<COFP10Action *>::iterator DecodeActIt = DecodeActList.begin();
    if (DecodeActIt == DecodeActList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10ActOutput *pDecodeAct = dynamic_cast<COFP10ActOutput *>((*DecodeActIt));
    EXPECT_NE(nullptr, pDecodeAct);

    
    std::list<COFP10Action *> EncodeActList;
    lRet = Encode.GetActionList(EncodeActList);
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

