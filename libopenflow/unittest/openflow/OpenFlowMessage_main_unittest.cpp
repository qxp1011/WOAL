#include "gtest/gtest.h"
//#include "CmThreadManager.h"

// common structures
#include "COFPMessage_unittest.cpp"
//#include "COFPPort_unittest.cpp"
//#include "COFPMatch_unittest.cpp"
//#include "COFPInstruction_unittest.cpp"
//#include "COFP10Action_unittest.cpp"
//#include "COFPPacketQueue_unittest.cpp"
//#include "COFPStatsMsgHeader_unittest.cpp"

// controller to switch
//#include "COFPFeaturesRequestMsg_unittest.cpp"
//#include "COFPFeaturesReplyMsg_unittest.cpp"
//#include "COFPSwitchConfig_unittest.cpp"
//#include "COFPSetConfigMsg_unittest.cpp"
//#include "COFPGetConfigReplyMsg_unittest.cpp"
//#include "COFPFlowModMsg_unittest.cpp"
//#include "COFPPortModMsg_unittest.cpp"
//#include "COFPPacketOutMsg_unittest.cpp"
//#include "COFPBarrierMsg_unittest.cpp"
//#include "COFPQueueGetConfigRequestMsg_unittest.cpp"
//#include "COFPQueueGetConfigReplyMsg_unittest.cpp"
//#include "COFPStatsReplyMsg_unittest.cpp"
//#include "COFPStatsRequestMsg_unittest.cpp"


// asynchronous msg
//#include "COFPPacketInMsg_unittest.cpp"
//#include "COFPFlowRemovedMsg_unittest.cpp"
//#include "COFPPortStatusMsg_unittest.cpp"
//#include "COFPErrorMsg_unittest.cpp"

// symmetric msg
//#include "COFPHelloMsg_unittest.cpp"
//#include "COFPEchoRequestMsg_unittest.cpp"
//#include "COFPEchoReplyMsg_unittest.cpp"


//CCmThreadManager theThreadManager;


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  //theThreadManager.InitMainThread(argc, argv);
  return RUN_ALL_TESTS();
}



