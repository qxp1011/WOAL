#include "COFPPacketOutMsg.h"

TEST(COFP10PacketOutMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_UINT32 dwBufferId = 0x129837aa;
    WORD16 awPortNo = 3139;
    COFP10PacketOutMsg Encode;
    Encode.SetMessageXID(dwXid);   
    Encode.SetBufferId(dwBufferId);
    Encode.SetInPort(awPortNo);

    
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


    ACE_CDR::Octet acData[] = {0x00,0x23,0x22,0xad,0x83,0xf8,0x93,0x1a,0x98,0x73,0x23,0x9c,0x00,0x00};
    lRet = Encode.AppendPacketData(acData, sizeof acData);
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFP10PacketOutMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_PACKET_OUT);

    
    EXPECT_EQ(Encode.GetBufferId(), Decode.GetBufferId());
    EXPECT_EQ(Encode.GetInPort(), Decode.GetInPort());
    EXPECT_EQ(Encode.GetActionsLen(), Decode.GetActionsLen());
    
    EXPECT_EQ(dwBufferId, Decode.GetBufferId());
    EXPECT_EQ(awPortNo, Decode.GetInPort());
    EXPECT_NE(0, Decode.GetActionsLen());
    

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


    const ACE_CDR::Octet *pDecodeData = Decode.GetEtherFrame();   
    EXPECT_NE(nullptr, pDecodeData);
    const ACE_CDR::Octet *pEncodeData = Encode.GetEtherFrame();
    EXPECT_NE(nullptr, pEncodeData);    
    ACE_INT16 wResult = ::memcmp(pDecodeData, pEncodeData, sizeof(acData));
    EXPECT_EQ(0, wResult);

    wResult = ::memcmp(pDecodeData, acData, sizeof(acData));
    EXPECT_EQ(0, wResult);
}

