#include "COFPFeaturesRequestMsg.h"

TEST(COFP10FeaturesRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;
    
    COFP10FeaturesRequestMsg Encode;
    Encode.SetMessageXID(dwXid);
    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFP10FeaturesRequestMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_FEATURES_REQUEST);
}

TEST(COFP13FeaturesRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;
    
    COFP13FeaturesRequestMsg Encode;
    Encode.SetMessageXID(dwXid);
    
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
    EXPECT_EQ(Encode.GetMessageType(), OFP13::OFPT_FEATURES_REQUEST);
}

