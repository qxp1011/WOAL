
#include "COFPPacketQueue.h"

TEST(COFPQueuePropRateTEST, Simple) 
{
    COFPQueuePropRate queuePropRate(COFPQueueProp::OFPQT_MAX_RATE);

    queuePropRate.SetRate(0XABDD);

    CCmMessageBlock      block(queuePropRate.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = queuePropRate.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFPQueueProp *pQueueProp = nullptr;
    rv = COFPQueueProp::DecodeQueueProp(stream, pQueueProp);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(COFPQueueProp::OFPQT_MAX_RATE, pQueueProp->GetQueueProperty());

    COFPQueuePropRate *pQueuePropRate
        = dynamic_cast<COFPQueuePropRate *>(pQueueProp);

    if (pQueuePropRate != NULL)
    {
        EXPECT_EQ(queuePropRate.GetQueueProperty(), pQueuePropRate->GetQueueProperty());
        EXPECT_EQ(queuePropRate.GetPropertyLen(),   pQueuePropRate->GetPropertyLen());
        EXPECT_EQ(queuePropRate.GetRate(),          pQueuePropRate->GetRate());
    }
}

TEST(COFP13QueuePropExperimenterTEST, Simple)
{
    COFP13QueuePropExperimenter queuePropExperimenter;
    std::allocator<ACE_CDR::Octet> alloc;
    WORD16 dataSize = 1200;
    ACE_CDR::Octet *data = alloc.allocate(dataSize);
    CM_ASSERT_RETURN_VOID(data != NULL);
    ::memset(data, 0XED, dataSize);

    queuePropExperimenter.SetExperimenter(0X13ED78AB);
    queuePropExperimenter.SetData(data, dataSize);

    ACE_CDR::Octet *ptr1 = NULL;
    WORD16 dataSize1 = 0;

    queuePropExperimenter.GetUsedData(ptr1, dataSize1);
    EXPECT_EQ(dataSize, dataSize1);
    WORD16 i = 0;
    for (; i < dataSize; ++i)
    {
        EXPECT_EQ(data[i], ptr1[i]);
    }

    CCmMessageBlock      block(queuePropExperimenter.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = queuePropExperimenter.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFPQueueProp *pQueueProp = nullptr;
    rv = COFPQueueProp::DecodeQueueProp(stream, pQueueProp);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(COFPQueueProp::OFPQT_EXPERIMENTER, pQueueProp->GetQueueProperty());

    COFP13QueuePropExperimenter *pQueuePropExperimenter
        = dynamic_cast<COFP13QueuePropExperimenter *>(pQueueProp);

    if (pQueuePropExperimenter != NULL)
    {
        EXPECT_EQ(queuePropExperimenter.GetQueueProperty(), pQueuePropExperimenter->GetQueueProperty());
        EXPECT_EQ(queuePropExperimenter.GetPropertyLen(),   pQueuePropExperimenter->GetPropertyLen());
        EXPECT_EQ(queuePropExperimenter.GetExperimenter(),  pQueuePropExperimenter->GetExperimenter());

        ACE_CDR::Octet *ptr2 = NULL;
        WORD16 dataSize2 = 0;

        pQueuePropExperimenter->GetUsedData(ptr2, dataSize2);
        EXPECT_EQ(dataSize, dataSize2);
        for (i = 0; i < dataSize; ++i)
        {
            EXPECT_EQ(data[i], ptr2[i]);
        }
    }

    alloc.deallocate(data, dataSize);
}

TEST(COFP10PacketQueueTEST, Simple)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT32 dwQueueId = 0x89cdfac3;
    COFP10PacketQueue Encode(dwQueueId);
    
    WORD16 wProperty = COFPQueueProp::OFPQT_MIN_RATE;
    WORD16 wRate = 0xac83;
    COFPQueueProp *Prop = new COFPQueuePropRate(wProperty, wRate);
    EXPECT_NE(nullptr, Prop);
    lRet = Encode.AppendProp(Prop);
    EXPECT_EQ(CM_OK, lRet);

    
    CCmMessageBlock Msg(Encode.GetEncodeSpace());
    CCmByteStreamNetwork os(Msg);
    lRet = Encode.StreamTo(os);
    EXPECT_EQ(CM_OK, lRet);


    CCmByteStreamNetwork is(Msg);
    COFP10PacketQueue *Decode = new COFP10PacketQueue;
    EXPECT_NE(nullptr, Decode);
    lRet = Decode->StreamFrom(is);
    EXPECT_EQ(CM_OK, lRet);

    
    EXPECT_EQ(Encode.GetQueueId(), Decode->GetQueueId());
    EXPECT_EQ(Encode.GetQueueLen(), Decode->GetQueueLen());    
    EXPECT_EQ(dwQueueId, Decode->GetQueueId());
    

    std::list<COFPQueueProp *> DecodePropList;
    lRet = Decode->GetPropList(DecodePropList);
    EXPECT_EQ(CM_OK, lRet);

    std::list<COFPQueueProp *>::iterator DecodePropIt = DecodePropList.begin();
    if (DecodePropIt == DecodePropList.end())
    {
        EXPECT_EQ(CM_OK, CM_ERROR_FAILURE);
    }
    COFPQueuePropRate *pDecodeProp = dynamic_cast<COFPQueuePropRate *>((*DecodePropIt));
    EXPECT_NE(nullptr, pDecodeProp);

    std::list<COFPQueueProp *> EncodePropList;
    lRet = Encode.GetPropList(EncodePropList);
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

TEST(COFP13PacketQueueTEST, Simple)
{
    COFP13PacketQueue pktQueue;
    pktQueue.SetQueueId(0X33FF44DD);
    pktQueue.SetQueuePort(0XFF03AB13);

    // construct prop list
    std::list<COFPQueueProp *> propList;

    // prop[0]
    COFPQueuePropRate *propMaxRate = new COFPQueuePropRate(COFPQueueProp::OFPQT_MAX_RATE);
    EXPECT_NE(nullptr, propMaxRate);
    propMaxRate->SetRate(0XABDD);
    propList.push_front(reinterpret_cast<COFPQueueProp *>(propMaxRate));
    // prop[1]
    COFPQueuePropRate *propMinRate = new COFPQueuePropRate(COFPQueueProp::OFPQT_MIN_RATE);
    EXPECT_NE(nullptr, propMinRate);
    propMinRate->SetRate(0XABD3);
    propList.push_front(reinterpret_cast<COFPQueueProp *>(propMinRate));

    CmResult result = pktQueue.AddPropList(propList);
    EXPECT_TRUE(CM_SUCCEEDED(result));

    // experimenter prop
    std::allocator<ACE_CDR::Octet> alloc;
    WORD16 dataSize = 1200;
    ACE_CDR::Octet *data = alloc.allocate(dataSize);
    EXPECT_TRUE(data != NULL);
    ::memset(data, 0XED, dataSize);

    COFP13QueuePropExperimenter *propExperimenter = new COFP13QueuePropExperimenter();
    EXPECT_NE(nullptr, propExperimenter);
    propExperimenter->SetExperimenter(0X13ED78AB);
    propExperimenter->SetData(data, dataSize);

    COFPQueueProp *pPropExperimenter = reinterpret_cast<COFPQueueProp *>(propExperimenter);
    result = pktQueue.AppendProp(pPropExperimenter);
    EXPECT_TRUE(CM_SUCCEEDED(result));

    std::list<COFPQueueProp *> list1;
    result = pktQueue.GetPropList(list1);
    EXPECT_TRUE(CM_SUCCEEDED(result));

    EXPECT_EQ(3, list1.size());

    pktQueue.Dump();
    
    // begin encoding and decoding
    CCmMessageBlock      block(pktQueue.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    result = pktQueue.StreamTo(stream);
    EXPECT_EQ(CM_OK, result);

    COFP13PacketQueue *queue = new COFP13PacketQueue;
    EXPECT_NE(nullptr, queue);
    result = queue->StreamFrom(stream);
    EXPECT_EQ(CM_OK, result);

    EXPECT_EQ(pktQueue.GetQueueId(),   queue->GetQueueId());
    EXPECT_EQ(pktQueue.GetQueuePort(), queue->GetQueuePort());
    EXPECT_EQ(pktQueue.GetQueueLen(),  queue->GetQueueLen());

    std::list<COFPQueueProp *> list2;
    result = queue->GetPropList(list2);
    EXPECT_EQ(CM_OK, result);

    EXPECT_EQ(3, list2.size());
    
    std::list<COFPQueueProp *>::const_iterator it1 = list1.begin();
    std::list<COFPQueueProp *>::const_iterator it2 = list2.begin();
    while (it1 != list1.end())
    {
        WORD16 property = (*it1)->GetQueueProperty();

        EXPECT_EQ(property, (*it2)->GetQueueProperty());
        EXPECT_EQ((*it1)->GetPropertyLen(), (*it2)->GetPropertyLen());

        switch (property)
        {
            case COFPQueueProp::OFPQT_MIN_RATE:
            case COFPQueueProp::OFPQT_MAX_RATE:
            {
                COFPQueueProp *ptr1 = (*it1);
                COFPQueuePropRate *rate1 = dynamic_cast<COFPQueuePropRate *>(ptr1);

                COFPQueueProp *ptr2 = (*it2);
                COFPQueuePropRate *rate2 = dynamic_cast<COFPQueuePropRate *>(ptr2);

                if ((rate1 != NULL) && (rate2 != NULL))
                {
                    EXPECT_EQ(rate1->GetRate(), rate2->GetRate());
                }
                else
                {
                    EXPECT_TRUE(0);
                }
                break;
            }
            case COFPQueueProp::OFPQT_EXPERIMENTER:
            {
                COFPQueueProp *ptr1 = (*it1);
                COFP13QueuePropExperimenter *experimenter1 = 
                dynamic_cast<COFP13QueuePropExperimenter *>(ptr1);

                COFPQueueProp *ptr2 = (*it2);
                COFP13QueuePropExperimenter  *experimenter2 = 
                dynamic_cast<COFP13QueuePropExperimenter *>(ptr2);

                if ((experimenter1 != NULL) && (experimenter2 != NULL))
                {
                    EXPECT_EQ(experimenter1->GetExperimenter(), experimenter2->GetExperimenter());

                
                    ACE_CDR::Octet *ptr2 = NULL;
                    WORD16 dataSize2 = 0;
                    
                    experimenter2->GetUsedData(ptr2, dataSize2);
                    EXPECT_EQ(dataSize, dataSize2);
                    for (WORD16 i = 0; i < dataSize; ++i)
                    {
                        EXPECT_EQ(data[i], ptr2[i]);
                    }
                }
                else
                {
                    EXPECT_TRUE(0);
                }

                break;
            }
            default:
            {
                EXPECT_FALSE(1);
                break;
            }
        }
        it1++;
        it2++;
    }    

    alloc.deallocate(data, dataSize);
}

