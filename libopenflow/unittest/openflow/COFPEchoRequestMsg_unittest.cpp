
#include "COFPEchoRequestMsg.h"

TEST(COFP10EchoRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwStamp = 0x12345678;
    ACE_UINT32 dwXid = 0x90abcdef;
    
    COFP10EchoRequestMsg Encode;
    Encode.SetMessageXID(dwXid);
    lRet = Encode.SetPayload((const ACE_CDR::Octet *)&dwStamp, sizeof dwStamp);
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFP10EchoRequestMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_ECHO_REQUEST);

    WORD16 wEncodeLen = 0;
    WORD16 wDecodeLen = 0;
    const ACE_CDR::Octet *pEncode = NULL;
    lRet = Encode.GetPayload(pEncode, wEncodeLen);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_EQ(sizeof(dwStamp), wEncodeLen);
    EXPECT_EQ(dwStamp, *(ACE_UINT32 *)pEncode);

    const ACE_CDR::Octet *pDecode = NULL;
    lRet = Decode.GetPayload(pDecode, wDecodeLen);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_EQ(sizeof(dwStamp), wDecodeLen);
    EXPECT_EQ(dwStamp, *(ACE_UINT32 *)pDecode);
}

TEST(COFP13EchoRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwStamp = 0x12345678;
    ACE_UINT32 dwXid = 0x90abcdef;
    
    COFP13EchoRequestMsg Encode;
    Encode.SetMessageXID(dwXid);
    lRet = Encode.SetPayload((const ACE_CDR::Octet *)&dwStamp, sizeof dwStamp);
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    COFPMessage *Decode = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, Decode);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, Decode);

    EXPECT_EQ(Encode.GetMessageVersion(), Decode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP13_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP13::OFPT_ECHO_REQUEST);

    WORD16 wEncodeLen = 0;
    WORD16 wDecodeLen = 0;
    const ACE_CDR::Octet *pEncode = NULL;
    lRet = Encode.GetPayload(pEncode, wEncodeLen);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_EQ(sizeof(dwStamp), wEncodeLen);
    EXPECT_EQ(dwStamp, *(ACE_UINT32 *)pEncode);

    const ACE_CDR::Octet *pDecode = NULL;
    COFP13EchoRequestMsg *pEcho = dynamic_cast<COFP13EchoRequestMsg *>(Decode.Get());
    EXPECT_NE(nullptr, pEcho);
    lRet = pEcho->GetPayload(pDecode, wDecodeLen);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_EQ(sizeof(dwStamp), wDecodeLen);
    EXPECT_EQ(dwStamp, *(ACE_UINT32 *)pDecode);
}

