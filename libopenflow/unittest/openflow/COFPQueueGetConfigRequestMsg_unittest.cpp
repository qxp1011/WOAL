#include "COFPQueueGetConfigRequestMsg.h"

TEST(COFP10QueueGetConfigRequestMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;
    WORD16 wPortNo = 0x1dfe;
    
    COFP10QueueGetConfigRequestMsg Encode(wPortNo);
    Encode.SetMessageXID(dwXid);

    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10QueueGetConfigRequestMsg *pDecode = 
        dynamic_cast<COFP10QueueGetConfigRequestMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_QUEUE_GET_CONFIG_REQUEST);

    
    EXPECT_EQ(Encode.GetPortNo(), pDecode->GetPortNo());
    EXPECT_EQ(wPortNo, pDecode->GetPortNo());
}

