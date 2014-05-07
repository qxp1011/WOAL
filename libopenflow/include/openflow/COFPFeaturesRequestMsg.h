/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPFeaturesRequestMsg.h
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
#ifndef COFPFEATURESREQUESTMSG_H
#define COFPFEATURESREQUESTMSG_H

#include "COFPMessage.h"

class COFP10FeaturesRequestMsg : public COFPMessage
{
public:
    COFP10FeaturesRequestMsg(
        ACE_UINT32 adwXid = 0, ACE_CDR::Octet abType = OFP10::OFPT_FEATURES_REQUEST, ACE_CDR::Octet abVersion = OFP10_VERSION)
        : COFPMessage(abType, abVersion, adwXid)
    {}
    virtual ~COFP10FeaturesRequestMsg(){}
};


class COFP13FeaturesRequestMsg : public COFP10FeaturesRequestMsg
{
public:
    COFP13FeaturesRequestMsg(
        ACE_UINT32 adwXid = 0, ACE_CDR::Octet abType = OFP13::OFPT_FEATURES_REQUEST, ACE_CDR::Octet abVersion = OFP13_VERSION)
        : COFP10FeaturesRequestMsg(adwXid, abType, abVersion)
    {
    }
    virtual ~COFP13FeaturesRequestMsg(){}
};

#endif

