/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPGroupFeaturesMsg.h
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
#ifndef COFPMPGROUPFEATURESMSG_H
#define COFPMPGROUPFEATURESMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPGroupFeaturesRequestMsg 
    : public COFP13MultipartRequestMsg
{
public:
    COFP13MPGroupFeaturesRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_GROUP_FEATURES)
    {
    }
    virtual ~COFP13MPGroupFeaturesRequestMsg(){}
};


class COFP13MPGroupFeaturesReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Group configuration flags */
    enum ofp_group_capabilities {
        OFPGFC_SELECT_WEIGHT = 1 << 0, /* Support weight for select groups */
        OFPGFC_SELECT_LIVENESS = 1 << 1, /* Support liveness for select groups */
        OFPGFC_CHAINING = 1 << 2, /* Support chaining groups */
        OFPGFC_CHAINING_CHECKS = 1 << 3, /* Check chaining for loops and delete */
    };
    /* Body of reply to OFPMP_GROUP_FEATURES request. Group features. */
    struct ofp_group_features {
        ACE_UINT32 types; /* Bitmap of OFPGT_* values supported. */
        ACE_UINT32 capabilities; /* Bitmap of OFPGFC_* capability supported. */
        ACE_UINT32 max_groups[4]; /* Maximum number of groups for each type. */
        ACE_UINT32 actions[4]; /* Bitmaps of OFPAT_* that are supported. */
    };

    static_assert((sizeof(struct ofp_group_features) == 40), "must be 40");

    COFP13MPGroupFeaturesReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_GROUP_FEATURES)
    {
    }
    virtual ~COFP13MPGroupFeaturesReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

    COFP13MPGroupFeaturesReplyMsg::ofp_group_features &GetGroupFeatures();
    void SetGroupFeatures(ofp_group_features &groupfeatures);

private:
    ofp_group_features m_groupfeatures;
};

#endif
