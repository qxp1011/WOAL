/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPGetAsyncReplyMsg.h
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
#ifndef COFPGETASYNCREPLYMSG_H
#define COFPGETASYNCREPLYMSG_H

#include "COFPAsyncConfig.h"

class COFP13GetAsyncReplyMsg : public COFP13AsyncConfig
{
public:
    COFP13GetAsyncReplyMsg()
        : COFP13AsyncConfig(OFP13::OFPT_GET_ASYNC_REPLY)
    {
    }
    virtual ~COFP13GetAsyncReplyMsg(){}
};

#endif

