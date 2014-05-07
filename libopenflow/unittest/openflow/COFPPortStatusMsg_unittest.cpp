#include "COFPPortStatusMsg.h"

TEST(COFP10PortStatusMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_CDR::Octet bReason = 200;
    COFP10PortStatusMsg Encode;
    Encode.SetMessageXID(dwXid);
    Encode.SetReason(bReason);

    WORD16 wPortNum = 8372;
    ACE_CDR::Octet acHwAddr[OFP_ETH_ALEN] = {0x22, 0x3a, 0xfa, 0x44, 0x24, 0xad};
    ACE_CDR::Octet acName[OFP_MAX_PORT_NAME_LEN] = {0xaa,0x23,0x89,0xfc,0xac,0x82,0x91,0x00};
    ACE_UINT32 dwConfig = 0x8882addc;
    ACE_UINT32 dwState = 0xddaddadc;
    ACE_UINT32 dwCurr = 0x11200993;
    ACE_UINT32 dwAdvertised = 0x1aadf0c9;
    ACE_UINT32 dwSupported = 0xadad2392;
    ACE_UINT32 dwPeer = 0x1addc000;
    COFP10PhyPort *pPort = Encode.GetPort();
    EXPECT_NE(nullptr, pPort);
    pPort->SetPortNum(wPortNum);
    pPort->SetHWAddr((const ACE_CDR::Octet *)acHwAddr, sizeof acHwAddr);
    pPort->SetName((const char *)acName, sizeof acName);
    pPort->SetConfig(dwConfig);
    pPort->SetState(dwState);
    pPort->SetCurr(dwCurr);
    pPort->SetAdvertised(dwAdvertised);
    pPort->SetSupported(dwSupported);
    pPort->SetPeer(dwPeer);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);
    

    COFP10PortStatusMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);
    

    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_PORT_STATUS);

    
    EXPECT_EQ(Encode.GetReason(), Decode.GetReason());
    EXPECT_EQ(bReason, Decode.GetReason());


    COFP10PhyPort *pEncodePort = Encode.GetPort();
    EXPECT_NE(nullptr, pEncodePort);
    COFP10PhyPort *pDecodePort = Decode.GetPort();
    EXPECT_NE(nullptr, pDecodePort);


    EXPECT_EQ(pEncodePort->GetPortNum(), pDecodePort->GetPortNum());
    EXPECT_EQ(pEncodePort->GetPortConfig(), pDecodePort->GetPortConfig());
    EXPECT_EQ(pEncodePort->GetPortState(), pDecodePort->GetPortState());
    EXPECT_EQ(pEncodePort->GetPortCurr(), pDecodePort->GetPortCurr());
    EXPECT_EQ(pEncodePort->GetPortAdvertised(), pDecodePort->GetPortAdvertised());
    EXPECT_EQ(pEncodePort->GetPortSupported(), pDecodePort->GetPortSupported());
    EXPECT_EQ(pEncodePort->GetPortPeer(), pDecodePort->GetPortPeer());

    const ACE_CDR::Octet *pEncodeHwAddr = pEncodePort->GetPortHWAddr();
    EXPECT_NE(nullptr, pEncodeHwAddr);
    const ACE_CDR::Octet *pDecodeHwAddr = pDecodePort->GetPortHWAddr();
    EXPECT_NE(nullptr, pDecodeHwAddr);
    ACE_INT16 wCmp = ::memcmp(pEncodeHwAddr, pDecodeHwAddr, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);

    
    const char *pEncodeName = pEncodePort->GetPortName();
    EXPECT_NE(nullptr, pEncodeName);
    const char *pDecodeName = pDecodePort->GetPortName();
    EXPECT_NE(nullptr, pDecodeName);
    wCmp = ::memcmp(pEncodeName, pDecodeName, OFP_MAX_PORT_NAME_LEN);
    EXPECT_EQ(0, wCmp);

    
    wCmp = ::memcmp(acHwAddr, pDecodeHwAddr, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(acName, pDecodeName, OFP_MAX_PORT_NAME_LEN);
    EXPECT_EQ(0, wCmp);
    EXPECT_EQ(wPortNum, pDecodePort->GetPortNum());
    EXPECT_EQ(dwConfig, pDecodePort->GetPortConfig());
    EXPECT_EQ(dwState, pDecodePort->GetPortState());
    EXPECT_EQ(dwCurr, pDecodePort->GetPortCurr());
    EXPECT_EQ(dwAdvertised, pDecodePort->GetPortAdvertised());
    EXPECT_EQ(dwSupported, pDecodePort->GetPortSupported());
    EXPECT_EQ(dwPeer, pDecodePort->GetPortPeer());
}

TEST(COFP13PortStatusMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_CDR::Octet bReason = 200;
    COFP13PortStatusMsg Encode;
    Encode.SetMessageXID(dwXid);
    Encode.SetReason(bReason);

    ACE_UINT32 dwPortNum = 8372;
    ACE_CDR::Octet acHwAddr[OFP_ETH_ALEN] = {0x00, 0x3a, 0xfa, 0x44, 0x24, 0xad};
    ACE_CDR::Octet acName[OFP_MAX_PORT_NAME_LEN] = {0xaa,0x23,0x89,0xfc,0xac,0x82,0x91,0x00};
    ACE_UINT32 dwConfig = 0x8882addc;
    ACE_UINT32 dwState = 0xddaddadc;
    ACE_UINT32 dwCurr = 0x11200993;
    ACE_UINT32 dwAdvertised = 0x1aadf0c9;
    ACE_UINT32 dwSupported = 0xadad2392;
    ACE_UINT32 dwPeer = 0x1addc000;
    ACE_UINT32 dwCurrSpeed = 0x54fa9000;
    ACE_UINT32 dwMaxSpeed = 0xff0000ac;
    COFP13Port *pPort = Encode.GetPort();
    EXPECT_NE(nullptr, pPort);
    pPort->SetPortNum(dwPortNum);
    pPort->SetHWAddr((const ACE_CDR::Octet *)acHwAddr, sizeof acHwAddr);
    pPort->SetName((const char *)acName, sizeof acName);
    pPort->SetConfig(dwConfig);
    pPort->SetState(dwState);
    pPort->SetCurr(dwCurr);
    pPort->SetAdvertised(dwAdvertised);
    pPort->SetSupported(dwSupported);
    pPort->SetPeer(dwPeer);
    pPort->SetCurrSpeed(dwCurrSpeed);
    pPort->SetMaxSpeed(dwMaxSpeed);

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
    EXPECT_EQ(Encode.GetMessageType(), OFP13::OFPT_PORT_STATUS);

    
    COFP13PortStatusMsg *pStatus = dynamic_cast<COFP13PortStatusMsg *>(Decode.Get());
    EXPECT_NE(nullptr, pStatus);

    EXPECT_EQ(Encode.GetReason(), pStatus->GetReason());
    EXPECT_EQ(bReason, pStatus->GetReason());


    COFP13Port *pEncodePort = Encode.GetPort();
    EXPECT_NE(nullptr, pEncodePort);
    COFP13Port *pDecodePort = pStatus->GetPort();
    EXPECT_NE(nullptr, pDecodePort);


    EXPECT_EQ(pEncodePort->GetPortNum(), pDecodePort->GetPortNum());
    EXPECT_EQ(pEncodePort->GetPortConfig(), pDecodePort->GetPortConfig());
    EXPECT_EQ(pEncodePort->GetPortState(), pDecodePort->GetPortState());
    EXPECT_EQ(pEncodePort->GetPortCurr(), pDecodePort->GetPortCurr());
    EXPECT_EQ(pEncodePort->GetPortAdvertised(), pDecodePort->GetPortAdvertised());
    EXPECT_EQ(pEncodePort->GetPortSupported(), pDecodePort->GetPortSupported());
    EXPECT_EQ(pEncodePort->GetPortPeer(), pDecodePort->GetPortPeer());
    EXPECT_EQ(pEncodePort->GetPortCurrSpeed(), pDecodePort->GetPortCurrSpeed());
    EXPECT_EQ(pEncodePort->GetPortMaxSpeed(), pDecodePort->GetPortMaxSpeed());

    const ACE_CDR::Octet *pEncodeHwAddr = pEncodePort->GetPortHWAddr();
    EXPECT_NE(nullptr, pEncodeHwAddr);
    const ACE_CDR::Octet *pDecodeHwAddr = pDecodePort->GetPortHWAddr();
    EXPECT_NE(nullptr, pDecodeHwAddr);
    ACE_INT16 wCmp = ::memcmp(pEncodeHwAddr, pDecodeHwAddr, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);

    
    const char *pEncodeName = pEncodePort->GetPortName();
    EXPECT_NE(nullptr, pEncodeName);
    const char *pDecodeName = pDecodePort->GetPortName();
    EXPECT_NE(nullptr, pDecodeName);
    wCmp = ::memcmp(pEncodeName, pDecodeName, OFP_MAX_PORT_NAME_LEN);
    EXPECT_EQ(0, wCmp);

    
    wCmp = ::memcmp(acHwAddr, pDecodeHwAddr, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(acName, pDecodeName, OFP_MAX_PORT_NAME_LEN);
    EXPECT_EQ(0, wCmp);
    EXPECT_EQ(dwPortNum, pDecodePort->GetPortNum());
    EXPECT_EQ(dwConfig, pDecodePort->GetPortConfig());
    EXPECT_EQ(dwState, pDecodePort->GetPortState());
    EXPECT_EQ(dwCurr, pDecodePort->GetPortCurr());
    EXPECT_EQ(dwAdvertised, pDecodePort->GetPortAdvertised());
    EXPECT_EQ(dwSupported, pDecodePort->GetPortSupported());
    EXPECT_EQ(dwPeer, pDecodePort->GetPortPeer());
    EXPECT_EQ(dwCurrSpeed, pDecodePort->GetPortCurrSpeed());
    EXPECT_EQ(dwMaxSpeed, pDecodePort->GetPortMaxSpeed());
}

