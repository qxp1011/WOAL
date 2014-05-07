/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPGetConfigReplyMsg.h
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
#ifndef COFP_GET_CONFIG_REPLY_MSG_H
#define COFP_GET_CONFIG_REPLY_MSG_H

#include "COFPSwitchConfig.h"


class COFP10GetConfigReplyMsg : public COFPSwitchConfig
{
public:
    COFP10GetConfigReplyMsg(ACE_UINT16 awFlags = 0, ACE_UINT16 awMissSendLen = 0)
        : COFPSwitchConfig(OFP10::OFPT_GET_CONFIG_REPLY, OFP10_VERSION, awFlags, awMissSendLen)
    {}
    virtual ~COFP10GetConfigReplyMsg()  {}
};

class COFP13GetConfigReplyMsg : public COFPSwitchConfig
{
public:
    COFP13GetConfigReplyMsg(ACE_UINT16 awFlags = 0, ACE_UINT16 awMissSendLen = 0)
        : COFPSwitchConfig(OFP13::OFPT_GET_CONFIG_REPLY, OFP13_VERSION, awFlags, awMissSendLen)
    {}
    virtual ~COFP13GetConfigReplyMsg()  {}
};

#endif

