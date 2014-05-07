
#include "COFPMatch.h"

TEST(COFP10MatchTEST, simple)
{
    COFP10Match match;
    ACE_CDR::Octet dlSrc[OFP_ETH_ALEN] = {0XEE, 0XFD, 0X01, 0XAB, 0X0,  0XAA};
    ACE_CDR::Octet dlDst[OFP_ETH_ALEN] = {0XAB, 0X0,  0X13, 0XEE, 0XAB, 0XEE};

    match.SetWildcards(COFP10Match::OFPFW_NW_DST_SHIFT);
    match.SetInPort(0XABCD);
    match.SetDlSrc(dlSrc);
    match.SetDlDst(dlDst);
    match.SetDlVlan(0X1378);
    match.SetDlVlanPcp(0XE8);
    match.SetDlType(0XEE87);
    match.SetNwTos(0X78);
    match.SetNwProto(0XE8);
    match.SetNwSrc(0X77889900);
    match.SetNwDst(0X5889EEAA);
    match.SetTpSrc(0X1378);
    match.SetTpDst(0XAB78);

    match.Dump();
    
    CCmMessageBlock block(match.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = match.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFP10Match match2;
    rv = match2.StreamFrom(stream);
    EXPECT_EQ(CM_OK, rv);

    EXPECT_TRUE(match == match2);

    EXPECT_EQ(match.GetWildcards(), match2.GetWildcards());
    EXPECT_EQ(match.GetInPort(), match2.GetInPort());
    EXPECT_EQ(0, ::memcmp(match.GetDlSrc(), match2.GetDlSrc(), OFP_ETH_ALEN));
    EXPECT_EQ(0, ::memcmp(match.GetDlDst(), match2.GetDlDst(), OFP_ETH_ALEN));
    EXPECT_EQ(match.GetDlVlan(), match2.GetDlVlan());
    EXPECT_EQ(match.GetDlVlanPcp(), match2.GetDlVlanPcp());
    EXPECT_EQ(match.GetDlType(), match2.GetDlType());
    EXPECT_EQ(match.GetNwTos(), match2.GetNwTos());
    EXPECT_EQ(match.GetNwProto(), match2.GetNwProto());
    EXPECT_EQ(match.GetNwSrc(), match2.GetNwSrc());
    EXPECT_EQ(match.GetNwDst(), match2.GetNwDst());
    EXPECT_EQ(match.GetTpSrc(), match2.GetTpSrc());
    EXPECT_EQ(match.GetTpDst(), match2.GetTpDst());
}

TEST(COFP13OxmTlvTEST, simple)
{
    COFP13OxmTlv oxmTlv;
    WORD16 wUdpSrc = 0XEEBC;

    oxmTlv.SetOxmClass(COFP13Match::OFPXMC_OPENFLOW_BASIC);
    oxmTlv.SetOxmField(COFP13Match::OFPXMT_OFB_UDP_SRC);
    oxmTlv.SetHasMask(false);
    CmResult rv = oxmTlv.SetPayload(reinterpret_cast<const ACE_CDR::Octet *>(&wUdpSrc), sizeof(wUdpSrc));
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    CCmMessageBlock block(oxmTlv.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    rv = oxmTlv.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFP13OxmTlv *pOxmTlv = nullptr;
    rv = COFP13OxmTlv::DecodeOxmTlv(stream, pOxmTlv);
    EXPECT_TRUE(CM_SUCCEEDED(rv));

    EXPECT_EQ(oxmTlv.GetOxmClass(), pOxmTlv->GetOxmClass());
    EXPECT_EQ(oxmTlv.GetOxmField(), pOxmTlv->GetOxmField());
    EXPECT_FALSE(pOxmTlv->GetHasMask());
    EXPECT_EQ(oxmTlv.GetOxmLen(), pOxmTlv->GetOxmLen());
    EXPECT_EQ(0, ::memcmp(oxmTlv.GetPayload(), pOxmTlv->GetPayload(), 2));
}


TEST(COFP13MatchTEST, simple)
{
    CmResult result = CM_ERROR_FAILURE;

    COFP13Match match;
    /* fill in match with the following fields in sequence,
    
       field    hasMask   value                 mask
       inPort   no       0xabab0013             N/A
       ethDst   yes      0xeeff0011aacc         0xffff00ee1122
       ethSrc   no       0xfeff0112bacc         N/A
       ethType  no       ETH_TYPE_IP
       vlanVid  yes      0x1eab                 0x10ef
       vlanPcp  no       0xaa(more than 3 bits) N/A
       ipProto  no       IPPROTO_ICMP           N/A
//     icmpv4Type no     2                      N/A             IsMeetPrerequisite don't support this field.
     */
    COFP13OxmTlv *oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_IN_PORT);
    ACE_UINT32 inPort = 0xabab0013;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&inPort), sizeof(inPort));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    std::list<COFP13OxmTlv *> list;
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MatchTEST, append OFPXMT_OFB_IN_PORT\n")));
        item->Dump();
    }

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_DST);
    oxm->SetHasMask(true);
    ACE_CDR::Octet ethDst[ETH_ADDR_LENTH]     = {0xee, 0xff, 0, 0x11, 0xaa, 0xcc};
    ACE_CDR::Octet ethDstMask[ETH_ADDR_LENTH] = {0xff, 0xff, 0, 0xee, 0x11, 0x22};
    result = oxm->SetPayload(ethDst, ETH_ADDR_LENTH, ethDstMask);
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    list.clear();
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MatchTEST, append OFPXMT_OFB_ETH_DST\n")));
        item->Dump();
    }

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_SRC);
    ACE_CDR::Octet ethSrc[ETH_ADDR_LENTH]     = {0xfe, 0xff, 0x01, 0x12, 0xba, 0xcc};
    result = oxm->SetPayload(ethSrc, ETH_ADDR_LENTH);
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    list.clear();
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MatchTEST, append OFPXMT_OFB_ETH_SRC\n")));
        item->Dump();
    }

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_TYPE);
    WORD16 ethType = ETH_TYPE_IP;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&ethType), sizeof(ethType));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    list.clear();
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MatchTEST, append OFPXMT_OFB_ETH_TYPE\n")));
        item->Dump();
    }

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_VLAN_VID);
    oxm->SetHasMask(true);
    WORD16 vlanVid     = 0x1eab;
    WORD16 vlanVidMask = 0x10ef;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&vlanVid), sizeof(vlanVid), 
                             reinterpret_cast<const ACE_CDR::Octet*>(&vlanVidMask));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    list.clear();
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MatchTEST, append OFPXMT_OFB_VLAN_VID\n")));
        item->Dump();
    }

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_VLAN_PCP);
    ACE_CDR::Octet vlanPcp = 0xaa;  // (more than 3 bits)
    result = oxm->SetPayload(&vlanPcp, sizeof(vlanPcp));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    list.clear();
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MatchTEST, append OFPXMT_OFB_VLAN_PCP\n")));
        item->Dump();
    }

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_IP_PROTO);
    ACE_CDR::Octet ipProto = IPPROTO_ICMP;
    result = oxm->SetPayload(&ipProto, sizeof(ipProto));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    list.clear();
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13MatchTEST, append OFPXMT_OFB_IP_PROTO\n")));
        item->Dump();
    }
    #if 0
    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm.ParaIn() != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ICMPV4_TYPE);
    BYTE icmpv4Type = 2;
    result = oxm->SetPayload(&icmpv4Type, sizeof(icmpv4Type));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    list.clear();
    result = match.GetOxmTlvList(list);
    EXPECT_EQ(result, CM_OK);
    for (auto &item : list)
    {
        CM_INFO_TRACE("append OFPXMT_OFB_ICMPV4_TYPE");
        item->Dump();
    }
    #endif
    
    match.Dump();
    
    CCmMessageBlock block(match.GetEncodeSpace());
    CCmByteStreamNetwork stream(block);
    CmResult rv = match.StreamTo(stream);
    EXPECT_EQ(CM_OK, rv);

    COFP13Match match2;
    rv = match2.StreamFrom(stream);
    EXPECT_EQ(CM_OK, rv);

    /* Check the following fields in openflowMatch,
    
       field    hasMask   value                 mask
       inPort   no       0xabab0013             N/A
       ethDst   yes      0xeeff0011aacc         0xffff00ee1122
       ethSrc   no       0xfeff0112bacc         N/A
       ethType  no       ETH_TYPE_IP
       vlanVid  yes      0x1eab                 0x10ef
       vlanPcp  no       0xaa(more than 3 bits) N/A
       ipProto  no       IPPROTO_ICMP           N/A
//     icmpv4Type no     2                      N/A             IsMeetPrerequisite don't support this field.
     */

    typedef std::list<COFP13OxmTlv *>::const_iterator LISTIT;
    std::list<COFP13OxmTlv *> list1;
    std::list<COFP13OxmTlv *> list2;
    rv = match.GetOxmTlvList(list1);
    EXPECT_EQ(CM_OK, rv);
    rv = match.GetOxmTlvList(list2);
    EXPECT_EQ(CM_OK, rv);
    LISTIT it1 = list1.begin();
    LISTIT it2 = list2.begin();
    while ((it1 != list1.end()) && (it2 != list2.end()))
    {
        EXPECT_EQ((*it1)->GetOxmClass(), (*it2)->GetOxmClass());
        EXPECT_EQ((*it1)->GetOxmField(), (*it2)->GetOxmField());
        EXPECT_EQ((*it1)->GetHasMask(),  (*it2)->GetHasMask());
        EXPECT_EQ((*it1)->GetOxmLen(),   (*it2)->GetOxmLen());
        EXPECT_TRUE(0 == ::memcmp((*it1)->GetPayload(), (*it2)->GetPayload(), (*it1)->GetOxmLen()));

        ++it1;
        ++it2;
    }
}

TEST(IsMeetPrerequisiteTest, simple)
{
    COFP13OxmTlv *oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_TCP_SRC);
    WORD16 tcpSrc = 0xabab;
    CmResult result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&tcpSrc), sizeof(tcpSrc));
    EXPECT_EQ(result, CM_OK);

    COFP13Match match;
    std::list<COFP13OxmTlv *> matchFieldList;
    result = match.GetOxmTlvList(matchFieldList);
    EXPECT_EQ(result, CM_OK);

    bool isMeet = match.IsMeetPrerequisite(matchFieldList, oxm);
    EXPECT_EQ(false, isMeet);
}

TEST(IsMeetPrerequisiteTest, complex1)
{
    CmResult result = CM_ERROR_FAILURE;

    COFP13Match match;
    /* fill in match with the following fields in sequence,
    
       field    hasMask   value                 mask
       inPort   no       0xabab0013             N/A
       ethDst   yes      0xeeff0011aacc         0xffff00ee1122
       ethSrc   no       0xfeff0112bacc         N/A
       ethType  no       ETH_TYPE_IP
       vlanVid  yes      0x1eab                 0x10ef
       vlanPcp  no       0xaa(more than 3 bits) N/A
       ipProto  no       IPPROTO_ICMP           N/A
//     icmpv4Type no     2                      N/A             IsMeetPrerequisite don't support this field.
     */
    COFP13OxmTlv *oxm = nullptr;
    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_IN_PORT);
    ACE_UINT32 inPort = 0xabab0013;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&inPort), sizeof(inPort));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_DST);
    oxm->SetHasMask(true);
    ACE_CDR::Octet ethDst[ETH_ADDR_LENTH]     = {0xee, 0xff, 0, 0x11, 0xaa, 0xcc};
    ACE_CDR::Octet ethDstMask[ETH_ADDR_LENTH] = {0xff, 0xff, 0, 0xee, 0x11, 0x22};
    result = oxm->SetPayload(ethDst, ETH_ADDR_LENTH, ethDstMask);
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_SRC);
    ACE_CDR::Octet ethSrc[ETH_ADDR_LENTH]     = {0xfe, 0xff, 0x01, 0x12, 0xba, 0xcc};
    result = oxm->SetPayload(ethSrc, ETH_ADDR_LENTH);
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_TYPE);
    WORD16 ethType = ETH_TYPE_IP;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&ethType), sizeof(ethType));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_VLAN_VID);
    oxm->SetHasMask(true);
    WORD16 vlanVid     = 0x1eab;
    WORD16 vlanVidMask = 0x10ef;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&vlanVid), sizeof(vlanVid), 
                             reinterpret_cast<const ACE_CDR::Octet*>(&vlanVidMask));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_VLAN_PCP);
    ACE_CDR::Octet vlanPcp = 0xaa;  // (more than 3 bits)
    result = oxm->SetPayload(&vlanPcp, sizeof(vlanPcp));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_IP_PROTO);
    ACE_CDR::Octet ipProto = IPPROTO_ICMP;
    result = oxm->SetPayload(&ipProto, sizeof(ipProto));
    EXPECT_EQ(result, CM_OK);

    std::list<COFP13OxmTlv *> matchFieldList;
    result = match.GetOxmTlvList(matchFieldList);
    EXPECT_EQ(result, CM_OK);
    
    bool isMeet = match.IsMeetPrerequisite(matchFieldList, oxm);
    EXPECT_EQ(true, isMeet);
}

TEST(IsMeetPrerequisiteTest, complex2)
{
    CmResult result = CM_ERROR_FAILURE;

    COFP13Match match;
    /* fill in match with the following fields in sequence,
    
       field    hasMask   value                 mask
       inPort   no       0xabab0013             N/A
       ethDst   yes      0xeeff0011aacc         0xffff00ee1122
       ethSrc   no       0xfeff0112bacc         N/A
//     ethType  no       ETH_TYPE_IP                            delete this for test
       vlanVid  yes      0x1eab                 0x10ef
       vlanPcp  no       0xaa(more than 3 bits) N/A
       ipProto  no       IPPROTO_ICMP           N/A
//     icmpv4Type no     2                      N/A             IsMeetPrerequisite don't support this field.
     */
    COFP13OxmTlv *oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_IN_PORT);
    ACE_UINT32 inPort = 0xabab0013;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&inPort), sizeof(inPort));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_DST);
    oxm->SetHasMask(true);
    ACE_CDR::Octet ethDst[ETH_ADDR_LENTH]     = {0xee, 0xff, 0, 0x11, 0xaa, 0xcc};
    ACE_CDR::Octet ethDstMask[ETH_ADDR_LENTH] = {0xff, 0xff, 0, 0xee, 0x11, 0x22};
    result = oxm->SetPayload(ethDst, ETH_ADDR_LENTH, ethDstMask);
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_SRC);
    ACE_CDR::Octet ethSrc[ETH_ADDR_LENTH]     = {0xfe, 0xff, 0x01, 0x12, 0xba, 0xcc};
    result = oxm->SetPayload(ethSrc, ETH_ADDR_LENTH);
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    #if 0
    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm.ParaIn() != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_ETH_TYPE);
    WORD16 ethType = ETH_TYPE_IP;
    result = oxm->SetPayload(reinterpret_cast<const BYTE*>(&ethType), sizeof(ethType));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);
    #endif

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_VLAN_VID);
    oxm->SetHasMask(true);
    WORD16 vlanVid     = 0x1eab;
    WORD16 vlanVidMask = 0x10ef;
    result = oxm->SetPayload(reinterpret_cast<const ACE_CDR::Octet*>(&vlanVid), sizeof(vlanVid), 
                             reinterpret_cast<const ACE_CDR::Octet*>(&vlanVidMask));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);

    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_VLAN_PCP);
    ACE_CDR::Octet vlanPcp = 0xaa;  // (more than 3 bits)
    result = oxm->SetPayload(&vlanPcp, sizeof(vlanPcp));
    EXPECT_EQ(result, CM_OK);
    result = match.AppendOxmTlv(oxm);
    EXPECT_EQ(result, CM_OK);

    oxm = new COFP13OxmTlv();
    EXPECT_TRUE(oxm != nullptr);
    
    oxm->SetOxmField(COFP13Match::OFPXMT_OFB_IP_PROTO);
    ACE_CDR::Octet ipProto = IPPROTO_ICMP;
    result = oxm->SetPayload(&ipProto, sizeof(ipProto));
    EXPECT_EQ(result, CM_OK);

    std::list<COFP13OxmTlv *> matchFieldList;
    result = match.GetOxmTlvList(matchFieldList);
    EXPECT_EQ(result, CM_OK);
    
    bool isMeet = match.IsMeetPrerequisite(matchFieldList, oxm);
    EXPECT_EQ(false, isMeet);
}

