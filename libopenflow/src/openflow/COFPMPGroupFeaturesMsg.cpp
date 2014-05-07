/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPGroupFeaturesMsg.cpp
 * Description:      Source file for openflow multipart group features msg.
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

#include "COFPMPGroupFeaturesMsg.h"
#include "CmError.h"

//COFP13MPGroupFeaturesReplyMsg
CmResult COFP13MPGroupFeaturesReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    is>>m_groupfeatures.types;
    is>>m_groupfeatures.capabilities;
    is>>m_groupfeatures.max_groups[0];
    is>>m_groupfeatures.max_groups[1];
    is>>m_groupfeatures.max_groups[2];
    is>>m_groupfeatures.max_groups[3];

    is>>m_groupfeatures.actions[0];
    is>>m_groupfeatures.actions[1];
    is>>m_groupfeatures.actions[2];
    is>>m_groupfeatures.actions[3];

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;
}

CmResult COFP13MPGroupFeaturesReplyMsg::
StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    os<<m_groupfeatures.types;
    os<<m_groupfeatures.capabilities;
    os<<m_groupfeatures.max_groups[0];
    os<<m_groupfeatures.max_groups[1];
    os<<m_groupfeatures.max_groups[2];
    os<<m_groupfeatures.max_groups[3];

    os<<m_groupfeatures.actions[0];
    os<<m_groupfeatures.actions[1];
    os<<m_groupfeatures.actions[2];
    os<<m_groupfeatures.actions[3];

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

    return CM_OK;

}

ACE_UINT16 COFP13MPGroupFeaturesReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_groupfeatures);
}

void COFP13MPGroupFeaturesReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}


COFP13MPGroupFeaturesReplyMsg::ofp_group_features &
COFP13MPGroupFeaturesReplyMsg::GetGroupFeatures()
{
    return m_groupfeatures;
}

void COFP13MPGroupFeaturesReplyMsg::
SetGroupFeatures(ofp_group_features &groupfeatures)
{
    ::memcpy(&m_groupfeatures, &groupfeatures, sizeof(groupfeatures));
}

