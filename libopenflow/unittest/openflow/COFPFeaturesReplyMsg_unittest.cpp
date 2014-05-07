#include "COFPFeaturesReplyMsg.h"

TEST(COFP10FeaturesReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_UINT64 lDataPathID = 0x9038adffce1209a8;
    ACE_UINT32 dwBuffers = 0x2345afb4;
    ACE_CDR::Octet bTables = 83;
    ACE_UINT32 dwCapabilities = 0xad492fc0;
    ACE_UINT32 dwActions = 0x88daffce;
    COFP10FeaturesReplyMsg Encode;
    Encode.SetMessageXID(dwXid);
    Encode.SetDataPathID(lDataPathID);
    Encode.SetBuffers(dwBuffers);
    Encode.SetTables(bTables);
    Encode.SetCapabilities(dwCapabilities);
    Encode.SetActions(dwActions);

    WORD16 wPortNum = 8372;
    ACE_CDR::Octet acHwAddr[OFP_ETH_ALEN] = {0x22, 0x3a, 0xfa, 0x44, 0x24, 0xad};
    ACE_CDR::Octet acName[OFP_MAX_PORT_NAME_LEN] = {0xaa,0x23,0x89,0xfc,0xac,0x82,0x91,0x00};
    ACE_UINT32 dwConfig = 0x8882addc;
    ACE_UINT32 dwState = 0xddaddadc;
    ACE_UINT32 dwCurr = 0x11200993;
    ACE_UINT32 dwAdvertised = 0x1aadf0c9;
    ACE_UINT32 dwSupported = 0xadad2392;
    ACE_UINT32 dwPeer = 0x1addc000;
    COFP10PhyPort *Port = new COFP10PhyPort;
    EXPECT_TRUE(Port != nullptr);
    
    Port->SetPortNum(wPortNum);
    Port->SetHWAddr((const ACE_CDR::Octet *)acHwAddr, sizeof acHwAddr);
    Port->SetName((const char *)acName, sizeof acName);
    Port->SetConfig(dwConfig);
    Port->SetState(dwState);
    Port->SetCurr(dwCurr);
    Port->SetAdvertised(dwAdvertised);
    Port->SetSupported(dwSupported);
    Port->SetPeer(dwPeer);
    lRet = Encode.AppendPort(Port);
    EXPECT_EQ(CM_OK, lRet);


    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);
    

    COFP10FeaturesReplyMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);
    

    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_FEATURES_REPLY);


    EXPECT_EQ(Encode.GetDataPathID(), Decode.GetDataPathID());
    EXPECT_EQ(Encode.GetBuffers(), Decode.GetBuffers());
    EXPECT_EQ(Encode.GetTables(), Decode.GetTables());
    EXPECT_EQ(Encode.GetCapabilities(), Decode.GetCapabilities());
    EXPECT_EQ(Encode.GetActions(), Decode.GetActions());
    
    EXPECT_EQ(lDataPathID, Decode.GetDataPathID());
    EXPECT_EQ(dwBuffers, Decode.GetBuffers());
    EXPECT_EQ(bTables, Decode.GetTables());
    EXPECT_EQ(dwCapabilities, Decode.GetCapabilities());
    EXPECT_EQ(dwActions, Decode.GetActions());



    std::list<COFP10PhyPort *> DecodeList;
    lRet = Decode.GetPortList(DecodeList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFP10PhyPort *>::iterator DecodeIt = DecodeList.begin();
    if (DecodeIt == DecodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10PhyPort *pDecodePort = dynamic_cast<COFP10PhyPort *>(*DecodeIt);
    EXPECT_NE(nullptr, pDecodePort);

    
    std::list<COFP10PhyPort *> EncodeList;
    lRet = Encode.GetPortList(EncodeList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFP10PhyPort *>::iterator EncodeIt = EncodeList.begin();
    if (EncodeIt == EncodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10PhyPort *pEncodePort = dynamic_cast<COFP10PhyPort *>(*EncodeIt);
    EXPECT_NE(nullptr, pEncodePort);
    
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

TEST(COFP13FeaturesReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    ACE_UINT64 lDataPathID = 0x9038adffce1209a8;
    ACE_UINT32 dwBuffers = 0x2345afb4;
    ACE_CDR::Octet bTables = 83;
    ACE_CDR::Octet bAuxiliaryId = 129;
    ACE_UINT32 dwCapabilities = 0xad492fc0;
    
    COFP13FeaturesReplyMsg Encode;
    Encode.SetMessageXID(dwXid);
    Encode.SetDataPathID(lDataPathID);
    Encode.SetBuffers(dwBuffers);
    Encode.SetTables(bTables);
    Encode.SetAuxiliaryID(bAuxiliaryId);
    Encode.SetCapabilities(dwCapabilities);
    
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
    EXPECT_EQ(Encode.GetMessageType(), OFP13::OFPT_FEATURES_REPLY);

    COFP13FeaturesReplyMsg *pReply = dynamic_cast<COFP13FeaturesReplyMsg *>(Decode.Get());
    EXPECT_NE(nullptr, pReply);

    EXPECT_EQ(Encode.GetDataPathID(), pReply->GetDataPathID());
    EXPECT_EQ(Encode.GetBuffers(), pReply->GetBuffers());
    EXPECT_EQ(Encode.GetTables(), pReply->GetTables());
    EXPECT_EQ(Encode.GetAuxiliaryID(), pReply->GetAuxiliaryID());
    EXPECT_EQ(Encode.GetCapabilities(), pReply->GetCapabilities());
    
    EXPECT_EQ(lDataPathID, pReply->GetDataPathID());
    EXPECT_EQ(dwBuffers, pReply->GetBuffers());
    EXPECT_EQ(bTables, pReply->GetTables());
    EXPECT_EQ(bAuxiliaryId, pReply->GetAuxiliaryID());
    EXPECT_EQ(dwCapabilities, pReply->GetCapabilities());
}

