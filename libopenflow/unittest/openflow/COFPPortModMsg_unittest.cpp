#include "COFPPortModMsg.h"

TEST(COFP10PortModMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wPortNo = 0x2839;
    ACE_CDR::Octet acAddr[OFP_ETH_ALEN] = {0x00,0x12,0x00,0xcd,0x00,0xfe};
    ACE_UINT32 dwCfg = 0xadcdfe12;
    ACE_UINT32 dwMask = 0xac239f0d;
    ACE_UINT32 dwAdvertise = 0x982934da;
	COFP10PortModMsg Encode;
    Encode.SetMessageXID(dwXid);    
    Encode.SetPortNo(wPortNo);
    Encode.SetHwAddr(acAddr);
    Encode.SetConfig(dwCfg);
    Encode.SetMask(dwMask);
    Encode.SetAdvertise(dwAdvertise);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);
    

    COFP10PortModMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);
    

    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_PORT_MOD);

    
    EXPECT_EQ(Encode.GetPortNo(), Decode.GetPortNo());
    EXPECT_EQ(Encode.GetConfig(), Decode.GetConfig());
    EXPECT_EQ(Encode.GetMask(), Decode.GetMask());
    EXPECT_EQ(Encode.GetAdvertise(), Decode.GetAdvertise());

    
    EXPECT_EQ(wPortNo, Decode.GetPortNo());
    EXPECT_EQ(dwCfg, Decode.GetConfig());
    EXPECT_EQ(dwMask, Decode.GetMask());
    EXPECT_EQ(dwAdvertise, Decode.GetAdvertise());


    const ACE_CDR::Octet *pDecodeHwAddr = Decode.GetHwAddr();
    EXPECT_NE(nullptr, pDecodeHwAddr);
    const ACE_CDR::Octet *pEncodeHwAddr = Encode.GetHwAddr();
    EXPECT_NE(nullptr, pEncodeHwAddr);
    
    ACE_INT16 wCmp = ::memcmp(pDecodeHwAddr, pEncodeHwAddr, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeHwAddr, acAddr, OFP_ETH_ALEN);
    EXPECT_EQ(0, wCmp);
}

