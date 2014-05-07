#include "COFPFlowModMsg.h"


TEST(COFP10FlowModMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;
    ACE_UINT64 lCookie = 0x1234567890abcdef;
    WORD16 wCmd = 9831;
    WORD16 wIdleTimeout = 42056;    
    WORD16 wHardTimeout = 7056;
    WORD16 wPri = 2839;
    ACE_UINT32 wBufferId = 0x34561206;
    WORD16 wOutPortNo = 39829;
    WORD16 wFlag = 12309;

    COFP10FlowModMsg Encode;    
    Encode.SetMessageXID(dwXid);   
    Encode.SetCookie(lCookie);
    Encode.SetCommand(wCmd);
    Encode.SetIdleTimeout(wIdleTimeout);
    Encode.SetHardTimeout(wHardTimeout);
    Encode.SetPriority(wPri);
    Encode.SetBufferId(wBufferId);
    Encode.SetOutPort(wOutPortNo);
    Encode.SetFlags(wFlag);

    
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
    COFP10Action *Act = new COFP10ActOutput;
    EXPECT_NE(nullptr, Act);
    COFP10ActOutput *pAct = dynamic_cast<COFP10ActOutput *>(Act);
    EXPECT_NE(nullptr, pAct);
    pAct->SetOutPort(wActOutPort);
    pAct->SetMaxLen(wMaxLen);
    lRet = Encode.AppendAction(Act);
    EXPECT_EQ(CM_OK, lRet);
    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);
    

    COFP10FlowModMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);
    

    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_FLOW_MOD);


    
    EXPECT_EQ(Encode.GetCookie(), Decode.GetCookie());
    EXPECT_EQ(Encode.GetCommand(), Decode.GetCommand());
    EXPECT_EQ(Encode.GetIdleTimeout(), Decode.GetIdleTimeout());
    EXPECT_EQ(Encode.GetHardTimeout(), Decode.GetHardTimeout());
    EXPECT_EQ(Encode.GetPriority(), Decode.GetPriority());
    EXPECT_EQ(Encode.GetBufferId(), Decode.GetBufferId());
    EXPECT_EQ(Encode.GetOutPort(), Decode.GetOutPort());
    EXPECT_EQ(Encode.GetFlags(), Decode.GetFlags());
    
    EXPECT_EQ(Encode.GetCookie(), lCookie);
    EXPECT_EQ(Encode.GetCommand(), wCmd);
    EXPECT_EQ(Encode.GetIdleTimeout(), wIdleTimeout);
    EXPECT_EQ(Encode.GetHardTimeout(), wHardTimeout);
    EXPECT_EQ(Encode.GetPriority(), wPri);
    EXPECT_EQ(Encode.GetBufferId(), wBufferId);
    EXPECT_EQ(Encode.GetOutPort(), wOutPortNo);
    EXPECT_EQ(Encode.GetFlags(), wFlag);


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


    std::list<COFP10Action *> DecodeList;
    lRet = Decode.GetActionList(DecodeList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFP10Action *>::iterator DecodeIt = DecodeList.begin();
    if (DecodeIt == DecodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10ActOutput *pDecodeAct = dynamic_cast<COFP10ActOutput *>((*DecodeIt));
    EXPECT_NE(nullptr, pDecodeAct);

    
    std::list<COFP10Action *> EncodeList;
    lRet = Encode.GetActionList(EncodeList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFP10Action *>::iterator EncodeIt = EncodeList.begin();
    if (EncodeIt == EncodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10ActOutput *pEncodeAct = dynamic_cast<COFP10ActOutput *>((*EncodeIt));
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


