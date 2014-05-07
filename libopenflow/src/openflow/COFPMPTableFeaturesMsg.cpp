/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPTableFeaturesMsg.cpp
 * Description:      Source file for openflow multipart table features msg.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-16
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/
#include "COFPMPTableFeaturesMsg.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP13MPTableFeaturesRequestMsg::~COFP13MPTableFeaturesRequestMsg()
{
    for (auto &item : m_table_feature_list)
    {
        delete item;
    }
}

CmResult COFP13MPTableFeaturesRequestMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13MPTableFeaturesRequestMsg::StreamFrom, failure\n")));
        return CM_ERROR_FAILURE;
    }

    is>>m_table_features.length;
    is>>m_table_features.table_id;
    is.read_octet_array(m_table_features.pad, sizeof(m_table_features.pad));
    is.read_char_array(m_table_features.name, sizeof(m_table_features.name));

    is>>m_table_features.metadata_match;
    is>>m_table_features.metadata_write;
    is>>m_table_features.config;
    is>>m_table_features.max_entries;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_INT16 propsize = m_table_features.length-sizeof(m_table_features);
    while (propsize>0)
    {
        COFP13TableFeatureProp *prop = nullptr;
        if (COFP13TableFeatureProp::DecodeMPTFP(is, prop) != CM_OK)
        {
            ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13TableFeatureProp::DecodeMPTFP, failure\n")));
            return CM_ERROR_FAILURE;
        }
        m_table_feature_list.push_back(prop);
        propsize = propsize-prop->GetEncodeSpace();
        if (propsize < 0)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13MPTableFeaturesRequestMsg::StreamFrom, failure propsize=%d\n"), 
                       propsize));
            return CM_ERROR_FAILURE;
        }
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;
}

CmResult COFP13MPTableFeaturesRequestMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_table_features.length;
    os<<m_table_features.table_id;
    os.write_octet_array(m_table_features.pad, sizeof(m_table_features.pad));
    os.write_char_array(m_table_features.name, sizeof(m_table_features.name));

    os<<m_table_features.metadata_match;
    os<<m_table_features.metadata_write;
    os<<m_table_features.config;
    os<<m_table_features.max_entries;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    for (auto &prop : m_table_feature_list) 
    {
        CmResult ret = prop->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(ret), CM_ERROR_FAILURE);
    }

    return CM_OK;
}

ACE_UINT16 COFP13MPTableFeaturesRequestMsg::GetStreamLen() const
{
    ACE_UINT16 size = COFP13MultipartMsg::GetStreamLen();
    ACE_UINT16 insize = sizeof(m_table_features);
    for (auto& prophead : m_table_feature_list) 
    {
        insize +=prophead->GetStreamLen();
    }

    return size+insize;
}

void COFP13MPTableFeaturesRequestMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPTableFeaturesRequestMsg::ofp_table_features &
COFP13MPTableFeaturesRequestMsg::GetTableFeatures()
{
    return m_table_features;
}

void COFP13MPTableFeaturesRequestMsg::
SetTableFeatures(ofp_table_features &meterfeatures)
{
    ::memcpy(&m_table_features, &meterfeatures, sizeof(meterfeatures));
    m_table_features.length = sizeof(meterfeatures);
}


void COFP13MPTableFeaturesRequestMsg::
SetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list)
{
    while (!list.empty())
    {
        COFP13TableFeatureProp *item = list.front();
        CM_ASSERT_RETURN_VOID(item != nullptr);
        list.pop_front();
        m_table_feature_list.push_back(item);
        m_table_features.length += item->GetStreamLen();
    } 
}

void COFP13MPTableFeaturesRequestMsg::
GetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list)
{
    while (!m_table_feature_list.empty())
    {
        COFP13TableFeatureProp *item = m_table_feature_list.front();
        CM_ASSERT_RETURN_VOID(item != nullptr);
        m_table_feature_list.pop_front();
        m_table_features.length -= item->GetStreamLen();
        list.push_back(item);
    }
}


void COFP13MPTableFeaturesRequestMsg::
AddTableFeaturesProt(COFP13TableFeatureProp *prop)
{
    CM_ASSERT_RETURN_VOID(prop != nullptr);

    m_table_feature_list.push_back(prop);
    m_table_features.length += prop->GetStreamLen();
}


//COFP13MPTableFeaturesReplyMsg
COFP13MPTableFeaturesReplyMsg::~COFP13MPTableFeaturesReplyMsg()
{
    for (auto &item : m_table_feature_list)
    {
        delete item;
    }
}

CmResult COFP13MPTableFeaturesReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_table_features.length;
    is>>m_table_features.table_id;
    is.read_octet_array(m_table_features.pad, sizeof(m_table_features.pad));
    is.read_char_array(m_table_features.name, sizeof(m_table_features.name));

    is>>m_table_features.metadata_match;
    is>>m_table_features.metadata_write;
    is>>m_table_features.config;
    is>>m_table_features.max_entries;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    ACE_INT16 propsize = m_table_features.length-sizeof(m_table_features);
    while (propsize>0)
    {
        COFP13TableFeatureProp *prop = nullptr;
        if (COFP13TableFeatureProp::DecodeMPTFP(is, prop) != CM_OK)
        {
            return CM_ERROR_FAILURE;
        }
        m_table_feature_list.push_back(prop);
        propsize = propsize - prop->GetEncodeSpace();
        if (propsize < 0)
        {
            return CM_ERROR_FAILURE;
        }
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    }

    return CM_OK;
}

CmResult COFP13MPTableFeaturesReplyMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_table_features.length;
    os<<m_table_features.table_id;
    os.write_octet_array(m_table_features.pad, sizeof(m_table_features.pad));
    os.write_char_array(m_table_features.name, sizeof(m_table_features.name));

    os<<m_table_features.metadata_match;
    os<<m_table_features.metadata_write;
    os<<m_table_features.config;
    os<<m_table_features.max_entries;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    for (auto &prop : m_table_feature_list) 
    {
        CmResult ret = prop->StreamTo(os);
        CM_ASSERT_RETURN(CM_SUCCEEDED(ret), CM_ERROR_FAILURE);
    }

    return CM_OK;

}

ACE_UINT16 COFP13MPTableFeaturesReplyMsg::GetStreamLen() const
{
    ACE_UINT16 size = COFP13MultipartMsg::GetStreamLen();
    ACE_UINT16 insize = sizeof(m_table_features);
    for (auto& prophead : m_table_feature_list) 
    {
        insize +=prophead->GetStreamLen();
    }

    return size+insize;
}

void COFP13MPTableFeaturesReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

COFP13MPTableFeaturesReplyMsg::ofp_table_features &
COFP13MPTableFeaturesReplyMsg::GetTableFeatures()
{
    return m_table_features;
}

void COFP13MPTableFeaturesReplyMsg::
SetTableFeatures(ofp_table_features &meterfeatures)
{
    ::memcpy(&m_table_features, &meterfeatures, sizeof(meterfeatures));
    m_table_features.length = sizeof(meterfeatures);
}


void COFP13MPTableFeaturesReplyMsg::
SetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list)
{
    while (!list.empty()) 
    {
        COFP13TableFeatureProp *item = list.front();
        CM_ASSERT_RETURN_VOID(item != nullptr);
        list.pop_front();
        m_table_feature_list.push_back(item);
        m_table_features.length += item->GetStreamLen();
    } 
}

void COFP13MPTableFeaturesReplyMsg::
GetTableFeaturesProtList(std::list<COFP13TableFeatureProp *> &list)
{
    while (!m_table_feature_list.empty()) 
    {
        COFP13TableFeatureProp *item = m_table_feature_list.front();
        CM_ASSERT_RETURN_VOID(item != nullptr);
        m_table_feature_list.pop_front();
        m_table_features.length -= item->GetStreamLen();
        list.push_back(item);
    }
}


void COFP13MPTableFeaturesReplyMsg::
AddTableFeaturesProt(COFP13TableFeatureProp *prop)
{
    CM_ASSERT_RETURN_VOID(prop != nullptr);
    m_table_feature_list.push_back(prop);
    m_table_features.length += prop->GetStreamLen();
}



