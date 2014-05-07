
#include "COFPSwitchConfig.h"

TEST(COFPSwitchConfigTEST, simple)
{
    COFPSwitchConfig swCfg(OFP10::OFPT_GET_CONFIG_REPLY);
    swCfg.SetFlags(COFPSwitchConfig::OFPC_FRAG_REASM);
    swCfg.SetMissSendLen(0xbbdd);

    CCmMessageBlock block(swCfg.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = swCfg.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFPMessage *pMsg = nullptr;
    rv = COFPMessage::DecodeMessage(block, pMsg);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(OFP10::OFPT_GET_CONFIG_REPLY, pMsg->GetMessageType());

    EXPECT_EQ(swCfg.GetMessageVersion(), pMsg->GetMessageVersion());
    EXPECT_EQ(swCfg.GetMessageType(),    pMsg->GetMessageType());
    EXPECT_EQ(swCfg.GetMessageLength(),  pMsg->GetMessageLength());
    EXPECT_EQ(swCfg.GetMessageXID(),     pMsg->GetMessageXID());

    COFPSwitchConfig *pSwCfg = dynamic_cast<COFPSwitchConfig *>(pMsg);
    if (pSwCfg != NULL)
    {
        EXPECT_EQ(swCfg.GetFlags(), pSwCfg->GetFlags());
        EXPECT_EQ(swCfg.GetMissSendLen(), pSwCfg->GetMissSendLen());
    }
}

