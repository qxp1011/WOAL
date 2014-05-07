#include "COFPPacketInMsg.h"

TEST(COFP10PacketInMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_UINT32 dwBufferId = 0xdac3293d;
    WORD16 wTotalLen = 2837;
    WORD16 wInPortNo = 999;
    ACE_CDR::Octet bReason = 23;
    COFP10PacketInMsg Encode;
    Encode.SetMessageXID(dwXid);   
    Encode.SetBufferId(dwBufferId);
    Encode.SetTotalLen(wTotalLen);
    Encode.SetInPort(wInPortNo);
    Encode.SetReason(bReason);
    

    ACE_CDR::Octet acData[] = {0x00,0x23,0x22,0xad,0x83,0xf8,0x93,0x1a,0x98,0x73,0x23,0x9c,0x00,0x00};
    CCmMessageBlock Data(sizeof(acData), (LPCSTR)acData, 0, sizeof acData);
    lRet = Encode.AppendPacketData(Data);
    EXPECT_EQ(CM_OK, lRet);

    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFP10PacketInMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_PACKET_IN);

    
    EXPECT_EQ(Encode.GetBufferId(), Decode.GetBufferId());
    EXPECT_EQ(Encode.GetTotalLen(), Decode.GetTotalLen());
    EXPECT_EQ(Encode.GetInPort(), Decode.GetInPort());
    EXPECT_EQ(Encode.GetReason(), Decode.GetReason());
    
    EXPECT_EQ(dwBufferId, Decode.GetBufferId());
    EXPECT_EQ(wTotalLen, Decode.GetTotalLen());
    EXPECT_EQ(wInPortNo, Decode.GetInPort());
    EXPECT_EQ(bReason, Decode.GetReason());


    const ACE_CDR::Octet *pDecodeData = Decode.GetEtherFrame();   
    EXPECT_NE(nullptr, pDecodeData);
    const ACE_CDR::Octet *pEncodeData = Encode.GetEtherFrame();
    EXPECT_NE(nullptr, pEncodeData);    
    ACE_INT16 wResult = ::memcmp(pDecodeData, pEncodeData, sizeof(acData));
    EXPECT_EQ(0, wResult);

    wResult = ::memcmp(pDecodeData, acData, sizeof(acData));
    EXPECT_EQ(0, wResult);
}

