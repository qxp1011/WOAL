#include "COFPMessage.h"
#include "ace/Log_Msg.h"
#include "gtest/gtest.h"
#include "CmError.h"

TEST(COFPMessageTEST, Simple) 
{
    COFPMessage message;

    message.SetMessageVersion(OFP13_VERSION);
    message.SetMessageType(OFP13::OFPT_SET_CONFIG);
    message.SetMessageLength(OFP_MESSAGE_HDR_SIZE);
    message.SetMessageXID(0XABCDEF03);

    ACE_OutputCDR outcdr(message.GetEncodeSpace(), ACE_CDR::BYTE_ORDER_BIG_ENDIAN);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("message encodeSpace=%u, length=%u\n"), message.GetEncodeSpace(), message.GetStreamLen()));
    CmResult rv = message.StreamTo(outcdr);
    EXPECT_EQ(CM_OK, rv);

    const ACE_Message_Block *pblock = outcdr.begin();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("pblocklength=%d\n"), pblock->length()));

    ACE_InputCDR incdr(pblock, ACE_CDR::BYTE_ORDER_BIG_ENDIAN);
    COFPMessage message2;
    rv = message2.StreamFrom(incdr);
    EXPECT_EQ(CM_OK, rv);

    EXPECT_EQ(message.GetMessageVersion(), message2.GetMessageVersion());
    EXPECT_EQ(message.GetMessageType(), message2.GetMessageType());
    EXPECT_EQ(message.GetMessageLength(), message2.GetMessageLength());
    EXPECT_EQ(message.GetMessageXID(), message2.GetMessageXID());
}


