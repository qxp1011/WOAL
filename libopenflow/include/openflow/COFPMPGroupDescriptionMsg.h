/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPGroupDescriptionMsg.h
 * Description:      
 * Others:
 * Version:          V1.0
 * Author:           
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFPMPGROUPDESCRIPTIONMSG_H
#define COFPMPGROUPDESCRIPTIONMSG_H

#include "COFPMultipartMsg.h"
#include "COFPBucket.h"

class COFP13MPGroupDescRequestMsg
    : public COFP13MultipartRequestMsg
{
public:
    COFP13MPGroupDescRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_GROUP_DESC)
    {
    }
    virtual ~COFP13MPGroupDescRequestMsg(){}
};


class COFP13MPGroupDescReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Body of reply to OFPMP_GROUP_DESC request. */
    struct ofp_group_desc_stats {
        ACE_UINT16 length; /* Length of this entry. */
        ACE_CDR::Octet type;    /* One of OFPGT_*. */
        ACE_CDR::Octet pad;     /* Pad to 64 bits. */
        ACE_UINT32 group_id; /* Group identifier. */
        //struct ofp_bucket buckets[0];
    };

    static_assert((sizeof(struct ofp_group_desc_stats) == 8), "must be 8");

    COFP13MPGroupDescReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_GROUP_DESC)
        ,m_wBucketLen(0)
    {
        ::memset(&m_groupdesc, 0, sizeof(m_groupdesc));
    }
    virtual ~COFP13MPGroupDescReplyMsg();

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPGroupDescReplyMsg::ofp_group_desc_stats &GetGroupDesc();
    void SetGroupDesc(ofp_group_desc_stats &groupdesc);

    CmResult AddBucketList(std::list<COFP13Bucket *> &list);
    CmResult AppendBucket(COFP13Bucket *apBucket);
    CmResult GetBucketList(std::list<COFP13Bucket *> &list);

private:
    ofp_group_desc_stats m_groupdesc;
    ACE_UINT16           m_wBucketLen;
    std::list<COFP13Bucket *> m_bucketlist;
};

#endif

