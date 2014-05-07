/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPSetConfigMsg.h
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
#ifndef COFP_SET_CONFIG_MSG_H
#define COFP_SET_CONFIG_MSG_H

#include "COFPSwitchConfig.h"

class COFP10SetConfigMsg : public COFPSwitchConfig
{
public:
    COFP10SetConfigMsg(ACE_UINT16 awFlags = 0, ACE_UINT16 awMissSendLen = 0xffff)
        : COFPSwitchConfig(OFP10::OFPT_SET_CONFIG, OFP10_VERSION, awFlags, awMissSendLen)
    {}
    virtual ~COFP10SetConfigMsg()   {}
};

class COFP13SetConfigMsg : public COFPSwitchConfig
{
public:
    COFP13SetConfigMsg(ACE_UINT16 awFlags = 0, ACE_UINT16 awMissSendLen = 0xffff)
        : COFPSwitchConfig(OFP13::OFPT_SET_CONFIG, OFP13_VERSION, awFlags, awMissSendLen)
    {}
    virtual ~COFP13SetConfigMsg()   {}
};

#endif

