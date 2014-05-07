
#include "COFPInstruction.h"

#if 0
TEST(COFP13InstructionTEST, simple)
{
    COFP13Instruction instruct;

    instruct.SetInstructType(OFPIT_APPLY_ACTIONS);
    instruct.SetInstructLen(OFP13_INSTRUCTION_HEADLEN);    

    CCmMessageBlock block(instruct.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = instruct.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));
    
    COFP13Instruction instruct2;
    rv = instruct2.StreamFrom(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    EXPECT_EQ(instruct.GetInstructType(), instruct2.GetInstructType());
    EXPECT_EQ(instruct.GetInstructLen(),  instruct2.GetInstructLen());
}
#endif

TEST(COFP13InstructGotoTableTEST, simple)
{
    COFP13InstructGotoTable instructGotoTable;
    instructGotoTable.SetTableId(0XAB);

    CCmMessageBlock block(instructGotoTable.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = instructGotoTable.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP13Instruction *pInstruct = nullptr;
    rv = COFP13Instruction::DecodeInstruction(stream, pInstruct);
    EXPECT_TRUE(CM_SUCCEEDED(rv));
    EXPECT_EQ(OFPIT_GOTO_TABLE, pInstruct->GetInstructType());

    EXPECT_EQ(instructGotoTable.GetInstructType(), pInstruct->GetInstructType());
    EXPECT_EQ(instructGotoTable.GetInstructLen(),  pInstruct->GetInstructLen());

    COFP13InstructGotoTable *pInstructGotoTable = dynamic_cast<COFP13InstructGotoTable *>(pInstruct);
    if (pInstructGotoTable != NULL)
    {
        EXPECT_EQ(instructGotoTable.GetTableId(), pInstructGotoTable->GetTableId());
    }
}

TEST(COFP13InstructWriteMetadataTEST, simple)
{
    COFP13InstructWriteMetadata instructWriteMetaData;

    instructWriteMetaData.SetMetadata(0X1122334455667788);
    instructWriteMetaData.SetMetadataMask(0X112FF34455667788);

    CCmMessageBlock block(instructWriteMetaData.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = instructWriteMetaData.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP13Instruction *pInstruct = nullptr;
    rv = COFP13Instruction::DecodeInstruction(stream, pInstruct);
    EXPECT_TRUE(CM_SUCCEEDED(rv));
    EXPECT_EQ(OFPIT_WRITE_METADATA, pInstruct->GetInstructType());

    EXPECT_EQ(instructWriteMetaData.GetInstructType(), pInstruct->GetInstructType());
    EXPECT_EQ(instructWriteMetaData.GetInstructLen(),  pInstruct->GetInstructLen());

    COFP13InstructWriteMetadata *pInstructWriteMetaData = dynamic_cast<COFP13InstructWriteMetadata *>(pInstruct);
    if (pInstructWriteMetaData != NULL)
    {
        EXPECT_EQ(instructWriteMetaData.GetMetadata(),     pInstructWriteMetaData->GetMetadata());
        EXPECT_EQ(instructWriteMetaData.GetMetadataMask(), pInstructWriteMetaData->GetMetadataMask());
    }
}

TEST(COFP13InstructActionTEST, complex)
{
    COFP13InstructAction instructAct(OFPIT_WRITE_ACTIONS);

    std::list<COFP13Action *> actList;
    // elem[0]
    COFP13Action *actOutput = new COFP13ActOutput();
    EXPECT_TRUE(actOutput != nullptr);
    COFP13ActOutput *pActOutput = static_cast<COFP13ActOutput *>(actOutput);
    pActOutput->SetOutPort(0XABCDEEFF);
    pActOutput->SetMaxLen(0XCDDD);
    actList.push_back(actOutput);
    
    // elem[1]
    COFP13Action *actGrp = new COFP13ActGroup(0XEEFF0132);
    EXPECT_TRUE(actGrp != nullptr);
    actList.push_back(actGrp);
    
    CmResult rv = instructAct.AddActionList(actList);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    std::list<COFP13Action *> list1;
    rv = instructAct.GetActionList(list1);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(2, list1.size());

    // elem[2]
    COFP13Action *actSetQueue = new COFP13ActSetQueue(0X3312EEF0);
    EXPECT_TRUE(actSetQueue != nullptr);
    rv = instructAct.AppendAction(actSetQueue);
    EXPECT_EQ(CM_OK, rv);

    list1.clear();
    rv = instructAct.GetActionList(list1);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(3, list1.size());

    instructAct.Dump();
    
    CCmMessageBlock block(instructAct.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    rv = instructAct.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP13Instruction *pInstruct = nullptr;
    rv = COFP13Instruction::DecodeInstruction(stream, pInstruct);
    EXPECT_TRUE(CM_SUCCEEDED(rv));
    EXPECT_EQ(OFPIT_WRITE_ACTIONS, pInstruct->GetInstructType());

    EXPECT_EQ(instructAct.GetInstructType(), pInstruct->GetInstructType());
    EXPECT_EQ(instructAct.GetInstructLen(),  pInstruct->GetInstructLen());

    COFP13InstructAction *pInstructAct = dynamic_cast<COFP13InstructAction *>(pInstruct);
    EXPECT_TRUE(pInstructAct != nullptr);
    
    std::list<COFP13Action *> list2;
    rv = pInstructAct->GetActionList(list2);
    EXPECT_EQ(CM_OK, rv);
    EXPECT_EQ(3, list2.size());

    std::list<COFP13Action *>::const_iterator it1 = list1.begin();
    std::list<COFP13Action *>::const_iterator it2 = list2.begin();
    
    while (it1 != list1.end())
    {
        WORD16 actType1 = (*it1)->GetActionType();
        EXPECT_EQ(actType1, (*it2)->GetActionType());
        switch (actType1)
        {
            case COFP13Action::OFPAT_OUTPUT:
            {
                COFP13ActOutput *pActOutput1 = dynamic_cast<COFP13ActOutput *>(*it1);
                COFP13ActOutput *pActOutput2 = dynamic_cast<COFP13ActOutput *>(*it2);
                if ((pActOutput1 != NULL) && (pActOutput2 != NULL))
                {
                    EXPECT_EQ(pActOutput1->GetOutPort(), pActOutput2->GetOutPort());
                    EXPECT_EQ(pActOutput1->GetMaxLen(),  pActOutput2->GetMaxLen());
                }
                else
                {
                    EXPECT_TRUE(0);
                }
                
                break;
            }
            case COFP13Action::OFPAT_GROUP:
            {
                COFP13ActGroup *pActGrp1 = dynamic_cast<COFP13ActGroup *>(*it1);
                COFP13ActGroup *pActGrp2 = dynamic_cast<COFP13ActGroup *>(*it2);
                if ((pActGrp1 != NULL) && (pActGrp2 != NULL))
                {
                    EXPECT_EQ(pActGrp1->GetGroupId(), pActGrp2->GetGroupId());
                }
                else
                {
                    EXPECT_TRUE(0);
                }
                
                break;
            }
            case COFP13Action::OFPAT_SET_QUEUE:
            {
                COFP13ActSetQueue *pActSetQueue1 = dynamic_cast<COFP13ActSetQueue *>(*it1);
                COFP13ActSetQueue *pActSetQueue2 = dynamic_cast<COFP13ActSetQueue *>(*it2);
                if ((pActSetQueue1 != NULL) && (pActSetQueue2 != NULL))
                {
                    EXPECT_EQ(pActSetQueue1->GetQueueId(), pActSetQueue2->GetQueueId());
                }
                else
                {
                    EXPECT_TRUE(0);
                }                
                
                break;
            }
            default:
            {
                EXPECT_TRUE(0);
                break;
            }                
        }
        ++it1;
        ++it2;
    }
}

TEST(COFP13InstructionMeterTEST, simple)
{
    COFP13InstructionMeter instructMeter;
    instructMeter.SetMeterId(0XBDEF0102);

    CCmMessageBlock block(instructMeter.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = instructMeter.StreamTo(stream);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    COFP13Instruction *pInstruct = nullptr;
    rv = COFP13Instruction::DecodeInstruction(stream, pInstruct);
    EXPECT_TRUE(CM_SUCCEEDED(rv));
    EXPECT_EQ(OFPIT_METER, pInstruct->GetInstructType());

    EXPECT_EQ(instructMeter.GetInstructType(), pInstruct->GetInstructType());
    EXPECT_EQ(instructMeter.GetInstructLen(),  pInstruct->GetInstructLen());

    COFP13InstructionMeter *pInstructMeter = dynamic_cast<COFP13InstructionMeter *>(pInstruct);
    if (pInstructMeter != NULL)
    {
        EXPECT_EQ(instructMeter.GetMeterId(), pInstructMeter->GetMeterId());
    }    
}

