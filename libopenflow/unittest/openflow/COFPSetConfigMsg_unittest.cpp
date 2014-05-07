
#include "COFPSetConfigMsg.h"

TEST(COFP10SetConfigMsgTEST, simple)
{
    COFP10SetConfigMsg setCfgMsg;
    setCfgMsg.SetFlags(COFPSwitchConfig::OFPC_FRAG_DROP);
    setCfgMsg.SetMissSendLen(0xbbdc);

    CCmMessageBlock block(setCfgMsg.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = setCfgMsg.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFPMessage *pMsg = nullptr;
    rv = COFPMessage::DecodeMessage(block, pMsg);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(OFP10::OFPT_SET_CONFIG, pMsg->GetMessageType());

    EXPECT_EQ(setCfgMsg.GetMessageVersion(), pMsg->GetMessageVersion());
    EXPECT_EQ(setCfgMsg.GetMessageType(),    pMsg->GetMessageType());
    EXPECT_EQ(setCfgMsg.GetMessageLength(),  pMsg->GetMessageLength());
    EXPECT_EQ(setCfgMsg.GetMessageXID(),     pMsg->GetMessageXID());

    COFP10SetConfigMsg *pSetSwCfg = dynamic_cast<COFP10SetConfigMsg *>(pMsg);
    if (pSetSwCfg != NULL)
    {
        EXPECT_EQ(setCfgMsg.GetFlags(), pSetSwCfg->GetFlags());
        EXPECT_EQ(setCfgMsg.GetMissSendLen(), pSetSwCfg->GetMissSendLen());
    }
}

