
#include "COFP10Action.h"


// No need to test COFP10ActOnlyHeader, as some class inherits it.
/*TEST(COFP10ActionTEST, simple)
{
    COFP10Action action(COFP10Action::OFPAT_SET_DL_DST);
    action.SetActionType(COFP10Action::OFPAT_SET_TP_DST);

    CCmMessageBlock block(action.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = action.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action action2(COFP10Action::OFPAT_SET_DL_DST);
    rv = action2.StreamFrom(stream);
    EXPECT_EQ(CM_OK, rv);

    EXPECT_EQ(action.GetActionType(), action2.GetActionType());
    EXPECT_EQ(action.GetActionLen(),  action2.GetActionLen());
}*/

// No need to test COFP10ActOnlyHeader, as some class inherits it.
/*
TEST(COFP10ActOnlyHeaderTEST, simple)
{
    COFP10ActOnlyHeader action();
}*/

TEST(COFP10ActOutputTEST, simple)
{
    COFP10ActOutput actOutput;

    actOutput.SetOutPort(0XABED);
    actOutput.SetMaxLen(COFP10ActOutput::OFPCML_NO_BUFFER);

    actOutput.Dump();
    
    ACE_Message_Block block(actOutput.GetEncodeSpace());
    ACE_OutputCDR outStream(&block);
    CmResult rv = actOutput.StreamTo(outStream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    ACE_InputCDR inStream(&block);
    rv = COFP10Action::DecodeAction(inStream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_OUTPUT, pAction->GetActionType());

    EXPECT_EQ(actOutput.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actOutput.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActOutput *pActOutput = dynamic_cast<COFP10ActOutput *>(pAction);

    if (pActOutput != NULL)
    {
        EXPECT_EQ(actOutput.GetOutPort(), pActOutput->GetOutPort());
        EXPECT_EQ(actOutput.GetMaxLen(),  pActOutput->GetMaxLen());
    }
}

TEST(COFP10ActEnqueueTEST, simple)
{
    COFP10ActEnqueue actEnqueue;
    actEnqueue.SetPortNum(0XABDD);
    actEnqueue.SetQueueId(0X1345ABDD);

    actEnqueue.Dump();
    
    CCmMessageBlock block(actEnqueue.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actEnqueue.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_ENQUEUE, pAction->GetActionType());

    EXPECT_EQ(actEnqueue.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actEnqueue.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActEnqueue *pActEnqueue = dynamic_cast<COFP10ActEnqueue *>(pAction);

    if (pActEnqueue != NULL)
    {
        EXPECT_EQ(actEnqueue.GetPortNum(), pActEnqueue->GetPortNum());
        EXPECT_EQ(actEnqueue.GetQueueId(), pActEnqueue->GetQueueId());
    }    
}

TEST(COFP10ActSetVlanVidTEST, simple)
{
    COFP10ActSetVlanVid actSetVlanVid;
    actSetVlanVid.SetVlanVid(0x13dd);

    actSetVlanVid.Dump();
    
    CCmMessageBlock block(actSetVlanVid.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actSetVlanVid.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_SET_VLAN_VID, pAction->GetActionType());

    EXPECT_EQ(actSetVlanVid.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actSetVlanVid.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActSetVlanVid *pActSetVlanVid = dynamic_cast<COFP10ActSetVlanVid *>(pAction);

    if (pActSetVlanVid != NULL)
    {
        EXPECT_EQ(actSetVlanVid.GetVlanVid(), pActSetVlanVid->GetVlanVid());
    }        
}

TEST(COFP10ActSetVlanPcpTEST, simple)
{
    COFP10ActSetVlanPcp actSetVlanPcp;
    actSetVlanPcp.SetVlanPcp(0x17);

    actSetVlanPcp.Dump();

    CCmMessageBlock block(actSetVlanPcp.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actSetVlanPcp.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_SET_VLAN_PCP, pAction->GetActionType());

    EXPECT_EQ(actSetVlanPcp.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actSetVlanPcp.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActSetVlanPcp *pActSetVlanPcp = dynamic_cast<COFP10ActSetVlanPcp *>(pAction);

    if (pActSetVlanPcp != NULL)
    {
        EXPECT_EQ(actSetVlanPcp.GetVlanPcp(), pActSetVlanPcp->GetVlanPcp());
    }
}

TEST(COFP10ActStripVlanTEST, simple)
{
    COFP10ActStripVlan actStripVlan;
    actStripVlan.Dump();

    CCmMessageBlock block(actStripVlan.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actStripVlan.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_STRIP_VLAN, pAction->GetActionType());

    EXPECT_EQ(actStripVlan.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actStripVlan.GetActionLen(),  pAction->GetActionLen());     
}

TEST(COFP10ActSetDlAddrTEST, simple)
{
    ACE_CDR::Octet addr[OFP_ETH_ALEN] = {0xad, 0x0, 0x13, 0x55, 0xed, 0xff};
    
    COFP10ActSetDlAddr actSetDlAddr(COFP10Action::OFPAT_SET_DL_DST);
    actSetDlAddr.SetDlAddr(addr);

    EXPECT_EQ(0, ::memcmp(addr, actSetDlAddr.GetDlAddr(), OFP_ETH_ALEN));
    
    actSetDlAddr.Dump();

    CCmMessageBlock block(actSetDlAddr.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actSetDlAddr.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_SET_DL_DST, pAction->GetActionType());

    EXPECT_EQ(actSetDlAddr.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actSetDlAddr.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActSetDlAddr *pActSetDlAddr = dynamic_cast<COFP10ActSetDlAddr *>(pAction);

    if (pActSetDlAddr != NULL)
    {
        EXPECT_EQ(0, ::memcmp(addr, pActSetDlAddr->GetDlAddr(), OFP_ETH_ALEN));
    }
}

TEST(COFP10ActSetNwAddrTEST, simple)
{
    COFP10ActSetNwAddr actSetNwAddr(COFP10Action::OFPAT_SET_NW_SRC);
    actSetNwAddr.SetNwAddr(0xab11ee77);

    actSetNwAddr.Dump();

    CCmMessageBlock block(actSetNwAddr.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actSetNwAddr.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_SET_NW_SRC, pAction->GetActionType());

    EXPECT_EQ(actSetNwAddr.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actSetNwAddr.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActSetNwAddr *pActSetNwAddr = dynamic_cast<COFP10ActSetNwAddr *>(pAction);

    if (pActSetNwAddr != NULL)
    {
        EXPECT_EQ(actSetNwAddr.GetNwAddr(), pActSetNwAddr->GetNwAddr());
    }
}

TEST(COFP10ActSetNwTosTEST, simple)
{
    COFP10ActSetNwTos actSetNwTos;
    actSetNwTos.SetNwTos(0xab);

    actSetNwTos.Dump();

    CCmMessageBlock block(actSetNwTos.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actSetNwTos.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_SET_NW_TOS, pAction->GetActionType());

    EXPECT_EQ(actSetNwTos.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actSetNwTos.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActSetNwTos *pActSetNwTos = dynamic_cast<COFP10ActSetNwTos *>(pAction);

    if (pActSetNwTos != NULL)
    {
        EXPECT_EQ(actSetNwTos.GetNwTos(), pActSetNwTos->GetNwTos());
    }
}

TEST(COFP10ActSetTpPortTEST, simple)
{
    COFP10ActSetTpPort actSetTpPort(COFP10Action::OFPAT_SET_TP_DST);
    actSetTpPort.SetTpPort(0xab01);

    actSetTpPort.Dump();

    CCmMessageBlock block(actSetTpPort.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = actSetTpPort.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP10Action *pAction = nullptr;
    rv = COFP10Action::DecodeAction(stream, pAction);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_TRUE(pAction != NULL);
    EXPECT_EQ(COFP10Action::OFPAT_SET_TP_DST, pAction->GetActionType());

    EXPECT_EQ(actSetTpPort.GetActionType(), pAction->GetActionType());
    EXPECT_EQ(actSetTpPort.GetActionLen(),  pAction->GetActionLen());    

    COFP10ActSetTpPort *pActSetTpPort = dynamic_cast<COFP10ActSetTpPort *>(pAction);

    if (pActSetTpPort != NULL)
    {
        EXPECT_EQ(actSetTpPort.GetTpPort(), pActSetTpPort->GetTpPort());
    }    
}