#include "COFPFlowRemovedMsg.h"

TEST(COFP10FlowRemovedMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_UINT64 lCookie = 0x1234567890abcdef;
    WORD16 awPriority = 60020;
    ACE_CDR::Octet bReason = 230;
    ACE_UINT32 dwSec = 0xabcdef12;
    ACE_UINT32 dwNsec = 0x12345678;
    WORD16 wTimeout = 0xabcd;
    ACE_UINT64 lPktCnt = 0x98076374acdfee43;
    ACE_UINT64 lByteCnt = 0x98076373da98ee43;
	COFP10FlowRemovedMsg Encode;
    Encode.SetMessageXID(dwXid);   
    Encode.SetCookie(lCookie);
    Encode.SetPriority(awPriority);
    Encode.SetReason(bReason);
    Encode.SetDurationSec(dwSec);
    Encode.SetDurationNsec(dwNsec);
    Encode.SetIdleTimeout(wTimeout);
    Encode.SetPacketCount(lPktCnt);
    Encode.SetByteCount(lByteCnt);

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
    

    COFP10FlowRemovedMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);
    

    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_FLOW_REMOVED);


    EXPECT_EQ(Encode.GetCookie(), Decode.GetCookie());
    EXPECT_EQ(Encode.GetPriority(), Decode.GetPriority());
    EXPECT_EQ(Encode.GetReason(), Decode.GetReason());
    EXPECT_EQ(Encode.GetDurationSec(), Decode.GetDurationSec());
    EXPECT_EQ(Encode.GetDurationNsec(), Decode.GetDurationNsec());
    EXPECT_EQ(Encode.GetIdleTimeout(), Decode.GetIdleTimeout());
    EXPECT_EQ(Encode.GetPacketCount(), Decode.GetPacketCount());
    EXPECT_EQ(Encode.GetByteCount(), Decode.GetByteCount());

    
    EXPECT_EQ(lCookie, Decode.GetCookie());
    EXPECT_EQ(awPriority, Decode.GetPriority());
    EXPECT_EQ(bReason, Decode.GetReason());
    EXPECT_EQ(dwSec, Decode.GetDurationSec());
    EXPECT_EQ(dwNsec, Decode.GetDurationNsec());
    EXPECT_EQ(wTimeout, Decode.GetIdleTimeout());
    EXPECT_EQ(lPktCnt, Decode.GetPacketCount());
    EXPECT_EQ(lByteCnt, Decode.GetByteCount());


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
}

