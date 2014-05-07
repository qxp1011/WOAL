/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPTableFeaturesMsg.h
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
#ifndef COFPMPTABLEFEATURESMSG_H
#define COFPMPTABLEFEATURESMSG_H

#include "COFPMultipartMsg.h"
#include "COFPTableFeatureProp.h"

static const ACE_UINT32 sgOFP_MAX_TABLE_NAME_LEN = 32;

class COFP13MPTableFeaturesRequestMsg
    : public COFP13MultipartRequestMsg
{
public:
    /* Body for ofp_multipart_request of type OFPMP_TABLE_FEATURES.
        * Body of reply to OFPMP_TABLE_FEATURES request. */
    struct ofp_table_features {
        ACE_UINT16 length; /* Length is padded to 64 bits. */
        ACE_CDR::Octet table_id; /* Identifier of table. Lower numbered tables
                                                 are consulted first. */
        ACE_CDR::Octet pad[5]; /* Align to 64-bits. */
        char name[sgOFP_MAX_TABLE_NAME_LEN];
        uint64_t metadata_match; /* Bits of metadata table can match. */
        uint64_t metadata_write; /* Bits of metadata table can write. */
        ACE_UINT32 config; /* Bitmap of OFPTC_* values */
        ACE_UINT32 max_entries; /* Max number of entries supported. */
        /* Table Feature Property list */
        //struct ofp_table_feature_prop_header properties[0];
    };

    static_assert((sizeof(struct ofp_table_features) == 64), "must be 64");

    COFP13MPTableFeaturesRequestMsg()
        :   COFP13MultipartRequestMsg(OFPMP_TABLE_FEATURES)
    {
        ::memset(&m_table_features, 0, sizeof(m_table_features));
    }
    virtual ~COFP13MPTableFeaturesRequestMsg();

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPTableFeaturesRequestMsg::ofp_table_features &GetTableFeatures();
    void SetTableFeatures(ofp_table_features &meterfeatures);

    void SetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list);
    void GetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list);
    void AddTableFeaturesProt(COFP13TableFeatureProp *prop);
    
private:
    ofp_table_features m_table_features;
    std::list<COFP13TableFeatureProp *> m_table_feature_list;
};


class COFP13MPTableFeaturesReplyMsg
    : public COFP13MultipartReplyMsg
{
public:
    /* Body for ofp_multipart_request of type OFPMP_TABLE_FEATURES.
     * Body of reply to OFPMP_TABLE_FEATURES request. */
    struct ofp_table_features {
        ACE_UINT16 length;  /* Length is padded to 64 bits. */
        ACE_CDR::Octet table_id; /* Identifier of table. Lower numbered tables are consulted first. */
        ACE_CDR::Octet pad[5];   /* Align to 64-bits. */
        char name[sgOFP_MAX_TABLE_NAME_LEN];
        uint64_t metadata_match; /* Bits of metadata table can match. */
        uint64_t metadata_write; /* Bits of metadata table can write. */
        ACE_UINT32 config;         /* Bitmap of OFPTC_* values */
        ACE_UINT32 max_entries;    /* Max number of entries supported. */
        /* Table Feature Property list */
        //struct ofp_table_feature_prop_header properties[0];
    };

    static_assert((sizeof(struct ofp_table_features) == 64), "must be 64");

    COFP13MPTableFeaturesReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_TABLE_FEATURES)
    {
        ::memset(&m_table_features, 0, sizeof(m_table_features));
    }
    virtual ~COFP13MPTableFeaturesReplyMsg();

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;

    COFP13MPTableFeaturesReplyMsg::ofp_table_features &GetTableFeatures();
    void SetTableFeatures(ofp_table_features &meterfeatures);

    void SetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list);
    void GetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list);
    void AddTableFeaturesProt(COFP13TableFeatureProp *prop);
    
private:
    ofp_table_features m_table_features;
    std::list<COFP13TableFeatureProp *> m_table_feature_list;
};

#endif

