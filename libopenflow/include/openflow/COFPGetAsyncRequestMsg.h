/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPGetAsyncRequestMsg.h
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
#ifndef COFPGETASYNCREQUESTMSG_H
#define COFPGETASYNCREQUESTMSG_H

#include "COFPMessage.h"

class COFP13GetAsyncRequestMsg : public COFPMessage
{
public:
    COFP13GetAsyncRequestMsg()
        : COFPMessage(OFP13::OFPT_GET_ASYNC_REQUEST)
    {
    }
    virtual ~COFP13GetAsyncRequestMsg(){}
};

#endif

