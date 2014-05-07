/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPBarrierMsg.h
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
#ifndef COFPBARRIERMSG_H
#define COFPBARRIERMSG_H

#include "COFPMessage.h"

class COFP10BarrierMsg : public COFPMessage
{
public:
    COFP10BarrierMsg(ACE_CDR::Octet abType, ACE_CDR::Octet abVersion = OFP10_VERSION)        
        : COFPMessage(abType, abVersion)
    {
        if ((abType != OFP10::OFPT_BARRIER_REQUEST) && (abType != OFP10::OFPT_BARRIER_REPLY))
        {
            ACE_ASSERT(0);
        }
    }
    virtual ~COFP10BarrierMsg(){}
};


class COFP13BarrierMsg : public COFPMessage
{
public:
    COFP13BarrierMsg(ACE_CDR::Octet abType, ACE_CDR::Octet abVersion = OFP13_VERSION)        
        : COFPMessage(abType, abVersion)
    {
        CM_ASSERT_RETURN_VOID((OFP13::OFPT_BARRIER_REQUEST == abType) 
                               || (OFP13::OFPT_BARRIER_REPLY == abType));
    }
    virtual ~COFP13BarrierMsg(){}
};

#endif

