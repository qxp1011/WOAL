
#include "COFPGetConfigReplyMsg.h"

TEST(COFP13GetConfigReplyMsgTEST, simple)
{
    COFP13GetConfigReplyMsg getCfgReplyMsg;
    getCfgReplyMsg.SetFlags(COFPSwitchConfig::OFPC_FRAG_NORMAL);
    getCfgReplyMsg.SetMissSendLen(0xbbd0);

    CCmMessageBlock block(getCfgReplyMsg.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = getCfgReplyMsg.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFPMessage *pMsg = nullptr;
    rv = COFPMessage::DecodeMessage(block, pMsg);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(OFP13::OFPT_GET_CONFIG_REPLY, pMsg->GetMessageType());

    EXPECT_EQ(getCfgReplyMsg.GetMessageVersion(), pMsg->GetMessageVersion());
    EXPECT_EQ(getCfgReplyMsg.GetMessageType(),    pMsg->GetMessageType());
    EXPECT_EQ(getCfgReplyMsg.GetMessageLength(),  pMsg->GetMessageLength());
    EXPECT_EQ(getCfgReplyMsg.GetMessageXID(),     pMsg->GetMessageXID());

    COFP13GetConfigReplyMsg *pGetCfgReply = dynamic_cast<COFP13GetConfigReplyMsg *>(pMsg);
    if (pGetCfgReply != NULL)
    {
        EXPECT_EQ(getCfgReplyMsg.GetFlags(), pGetCfgReply->GetFlags());
        EXPECT_EQ(getCfgReplyMsg.GetMissSendLen(), pGetCfgReply->GetMissSendLen());
    }
}


