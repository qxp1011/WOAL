
#include "COFPPort.h"

TEST(COFP10PhyPortTEST, Simple)
{
    ACE_CDR::Octet       hwAddr[OFP_ETH_ALEN] = {0XEE, 0XBB, 0XA5, 0X0, 0X33, 0XFE};
    char       portName[OFP_MAX_PORT_NAME_LEN] = "ThisIsWp.";

    COFP10PhyPort port;

    port.SetPortNum(0XAB3D);
    port.SetHWAddr(hwAddr, OFP_ETH_ALEN);
    port.SetName(portName, OFP_MAX_PORT_NAME_LEN);
    port.SetConfig(COFP10PhyPort::OFPPC_NO_FWD);
    port.SetState(COFP10PhyPort::OFPPS_STP_BLOCK);
    port.SetCurr(COFP10PhyPort::OFPPF_AUTONEG);
    port.SetAdvertised(COFP10PhyPort::OFPPF_COPPER);
    port.SetSupported(COFP10PhyPort::OFPPF_100MB_HD);
    port.SetPeer(COFP10PhyPort::OFPPF_100MB_FD);

    CCmMessageBlock block(port.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = port.StreamTo(stream);

    EXPECT_EQ(CM_OK, rv);

    COFP10PhyPort port2;
    rv = port2.StreamFrom(stream);

    EXPECT_EQ(CM_OK, rv);

    EXPECT_EQ(port.GetPortNum(), port2.GetPortNum());
    EXPECT_EQ(0, ::memcmp(port.GetPortHWAddr(), port2.GetPortHWAddr(), OFP_ETH_ALEN));
    EXPECT_EQ(port.GetPortNameLen(), port2.GetPortNameLen());
    EXPECT_STREQ(port.GetPortName(), port2.GetPortName());
    EXPECT_EQ(port.GetPortConfig(), port2.GetPortConfig());
    EXPECT_EQ(port.GetPortState(), port2.GetPortState());
    EXPECT_EQ(port.GetPortCurr(), port2.GetPortCurr());
    EXPECT_EQ(port.GetPortAdvertised(), port2.GetPortAdvertised());
    EXPECT_EQ(port.GetPortSupported(), port2.GetPortSupported());
    EXPECT_EQ(port.GetPortPeer(), port2.GetPortPeer());
}

TEST(COFP13PortTEST, Simple) 
{
    ACE_CDR::Octet       hwAddr[OFP_ETH_ALEN] = {0XEE, 0XBB, 0XA5, 0X0, 0X33, 0XFE};
    char       portName[OFP_MAX_PORT_NAME_LEN] = "ThisIsWp.";
    COFP13Port port;

    port.SetPortNum(0XABCDEF03);
    port.SetHWAddr(hwAddr, OFP_ETH_ALEN);
    port.SetName(portName, OFP_MAX_PORT_NAME_LEN);
    port.SetConfig(COFP13Port::OFPPC_NO_RECV);
    port.SetState(COFP13Port::OFPPS_BLOCKED);
    port.SetCurr(COFP13Port::OFPPF_100GB_FD);
    port.SetAdvertised(COFP13Port::OFPPF_10GB_FD);
    port.SetSupported(COFP13Port::OFPPF_FIBER);
    port.SetPeer(COFP13Port::OFPPF_PAUSE_ASYM);
    port.SetCurrSpeed(0XABDDEE03);
    port.SetMaxSpeed(0XEEFF0103);
    
    CCmMessageBlock block(port.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = port.StreamTo(stream);

    EXPECT_EQ(CM_OK, rv);

    COFP13Port port2;
    rv = port2.StreamFrom(stream);

    EXPECT_EQ(CM_OK, rv);

    EXPECT_EQ(port.GetPortNum(), port2.GetPortNum());
    EXPECT_EQ(0, ::memcmp(port.GetPortHWAddr(), port2.GetPortHWAddr(), OFP_ETH_ALEN));
    EXPECT_EQ(port.GetPortNameLen(), port2.GetPortNameLen());
    EXPECT_STREQ(port.GetPortName(), port2.GetPortName());
    EXPECT_EQ(port.GetPortConfig(), port2.GetPortConfig());
    EXPECT_EQ(port.GetPortState(), port2.GetPortState());
    EXPECT_EQ(port.GetPortCurr(), port2.GetPortCurr());
    EXPECT_EQ(port.GetPortAdvertised(), port2.GetPortAdvertised());
    EXPECT_EQ(port.GetPortSupported(), port2.GetPortSupported());
    EXPECT_EQ(port.GetPortPeer(), port2.GetPortPeer());
    EXPECT_EQ(port.GetPortCurrSpeed(), port2.GetPortCurrSpeed());
    EXPECT_EQ(port.GetPortMaxSpeed(), port2.GetPortMaxSpeed());
}


