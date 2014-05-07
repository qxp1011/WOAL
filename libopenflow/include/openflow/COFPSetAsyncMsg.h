/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPSetAsyncMsg.h
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
#ifndef COFPSETASYNCMSG_H
#define COFPSETASYNCMSG_H

#include "COFPAsyncConfig.h"

class COFP13SetAsyncMsg : public COFP13AsyncConfig
{
public:
    COFP13SetAsyncMsg()
        : COFP13AsyncConfig(OFP13::OFPT_SET_ASYNC)
    {
    }
    virtual ~COFP13SetAsyncMsg(){}
};

#endif
