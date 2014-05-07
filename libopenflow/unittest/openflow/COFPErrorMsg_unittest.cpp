#include "COFPErrorMsg.h"

TEST(COFP10ErrorMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;

    WORD16 wType = 0xadcd;
    WORD16 wCode = 0x2940;
    const char *pData = "error";
	COFP10ErrorMsg Encode;
    Encode.SetMessageXID(dwXid);
    Encode.SetErrType(wType);
    Encode.SetErrCode(wCode);
    lRet = Encode.SetErrData((const ACE_CDR::Octet *)pData, strlen(pData));
    EXPECT_EQ(CM_OK, lRet);

    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    COFP10ErrorMsg Decode;
    CCmByteStreamNetwork is(Msg);
    lRet = Decode.StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetMessageVersion(), Decode.GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), Decode.GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), Decode.GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), Decode.GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_ERROR);

    
    EXPECT_EQ(Encode.GetErrType(), Decode.GetErrType());
    EXPECT_EQ(Encode.GetErrCode(), Decode.GetErrCode());
    EXPECT_EQ(Encode.GetErrDataLen(), Decode.GetErrDataLen());

    
    EXPECT_EQ(wType, Decode.GetErrType());
    EXPECT_EQ(wCode, Decode.GetErrCode());
    EXPECT_EQ(strlen(pData), Decode.GetErrDataLen());
    
    const ACE_CDR::Octet *pDecodeData = Decode.GetErrData();
    EXPECT_NE(nullptr, pDecodeData);
    const ACE_CDR::Octet *pEncodeData = Encode.GetErrData();
    EXPECT_NE(nullptr, pEncodeData);
    
    ACE_INT16 wCmp = ::memcmp(pDecodeData, pEncodeData, strlen(pData));
    EXPECT_EQ(0, wCmp);
    wCmp = ::memcmp(pDecodeData, pData, strlen(pData));
    EXPECT_EQ(0, wCmp);
}

