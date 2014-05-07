/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPGetConfigRequestMsg.h
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
#ifndef COFPGETCONFIGREQUESTMSG_H
#define COFPGETCONFIGREQUESTMSG_H

#include "COFPMessage.h"

class COFP10GetConfigRequestMsg : public COFPMessage
{
public:
    COFP10GetConfigRequestMsg()
        : COFPMessage(OFP10::OFPT_GET_CONFIG_REQUEST, OFP10_VERSION)
    {}
    virtual ~COFP10GetConfigRequestMsg()    {}
};

class COFP13GetConfigRequestMsg : public COFPMessage
{
public:
    COFP13GetConfigRequestMsg()
        : COFPMessage(OFP13::OFPT_GET_CONFIG_REQUEST)
    {}
    virtual ~COFP13GetConfigRequestMsg()    {}
};

#endif

