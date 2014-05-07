#include "COFPQueueGetConfigReplyMsg.h"

TEST(COFP10QueueGetConfigReplyMsgTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwXid = 0x90abcdef;
    WORD16 wPortNo = 0x1dfe;
    
    COFP10QueueGetConfigReplyMsg Encode(wPortNo);
    Encode.SetMessageXID(dwXid);


    WORD16 wProperty = COFPQueueProp::OFPQT_MIN_RATE;
    WORD16 wRate = 0xac83;
    COFPQueueProp *Prop = new COFPQueuePropRate(wProperty, wRate);
    EXPECT_NE(nullptr, Prop);
    
    ACE_UINT32 dwQueueId = 0x89cdfac3;
    COFP10PacketQueue *Queue = new COFP10PacketQueue(dwQueueId);
    EXPECT_NE(nullptr, Queue);    
    lRet = Queue->AppendProp(Prop);
    EXPECT_EQ(CM_OK, lRet);

    lRet = Encode.AppendQueue(Queue);
    EXPECT_EQ(CM_OK, lRet);

    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);

    
    COFPMessage *OFPMsg = nullptr;
    lRet = COFPMessage::DecodeMessage(Msg, OFPMsg);
    EXPECT_EQ(CM_OK, lRet);
    EXPECT_NE(nullptr, OFPMsg);

    COFP10QueueGetConfigReplyMsg *pDecode = 
        dynamic_cast<COFP10QueueGetConfigReplyMsg *>(OFPMsg);
    EXPECT_NE(nullptr, pDecode);

    
    EXPECT_EQ(Encode.GetMessageVersion(), pDecode->GetMessageVersion());
    EXPECT_EQ(Encode.GetMessageType(), pDecode->GetMessageType());
    EXPECT_EQ(Encode.GetMessageLength(), pDecode->GetMessageLength());
    EXPECT_EQ(Encode.GetMessageXID(), pDecode->GetMessageXID());
    
    EXPECT_EQ(Encode.GetMessageXID(), dwXid);
    EXPECT_EQ(Encode.GetMessageVersion(), OFP10_VERSION);
    EXPECT_EQ(Encode.GetMessageType(), OFP10::OFPT_QUEUE_GET_CONFIG_REPLY);
    
    EXPECT_EQ(Encode.GetPortNo(), pDecode->GetPortNo());
    EXPECT_EQ(wPortNo, pDecode->GetPortNo());


    std::list<COFP10PacketQueue *> DecodeList;
    lRet = pDecode->GetQueueList(DecodeList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFP10PacketQueue *>::iterator DecodeIt = DecodeList.begin();
    if (DecodeIt == DecodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10PacketQueue *pDecodeQueue = dynamic_cast<COFP10PacketQueue *>((*DecodeIt));
    EXPECT_NE(nullptr, pDecodeQueue);

    
    std::list<COFP10PacketQueue *> EncodeList;
    lRet = Encode.GetQueueList(EncodeList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFP10PacketQueue *>::iterator EncodeIt = EncodeList.begin();
    if (EncodeIt == EncodeList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFP10PacketQueue *pEncodeQueue = dynamic_cast<COFP10PacketQueue *>((*EncodeIt));
    EXPECT_NE(nullptr, pEncodeQueue);

    
    EXPECT_EQ(pEncodeQueue->GetQueueId(), pDecodeQueue->GetQueueId());
    EXPECT_EQ(pEncodeQueue->GetQueueLen(), pDecodeQueue->GetQueueLen());
    
    EXPECT_EQ(dwQueueId, pDecodeQueue->GetQueueId());


    std::list<COFPQueueProp *> DecodePropList;
    lRet = pDecodeQueue->GetPropList(DecodePropList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFPQueueProp *>::iterator DecodePropIt = DecodePropList.begin();
    if (DecodePropIt == DecodePropList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFPQueuePropRate *pDecodeProp = dynamic_cast<COFPQueuePropRate *>((*DecodePropIt));
    EXPECT_NE(nullptr, pDecodeProp);

    std::list<COFPQueueProp *> EncodePropList;
    lRet = pEncodeQueue->GetPropList(EncodePropList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFPQueueProp *>::iterator EncodePropIt = EncodePropList.begin();
    if (EncodePropIt == EncodePropList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFPQueuePropRate *pEncodeProp = dynamic_cast<COFPQueuePropRate *>((*EncodePropIt));
    EXPECT_NE(nullptr, pEncodeProp);

    
    EXPECT_EQ(pEncodeProp->GetQueueProperty(), pDecodeProp->GetQueueProperty());
    EXPECT_EQ(pEncodeProp->GetPropertyLen(), pDecodeProp->GetPropertyLen());    
    EXPECT_EQ(pEncodeProp->GetRate(), pDecodeProp->GetRate());
    
    EXPECT_EQ(wProperty, pDecodeProp->GetQueueProperty());
    EXPECT_EQ(wRate, pDecodeProp->GetRate());
}

