/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        IOFPMsgParser.h
 * Description:      Interface for openflow message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/

#ifndef IOFP_MSG_PARSER_H
#define IOFP_MSG_PARSER_H

#include "ace/Basic_Types.h"
#include "ace/CDR_Stream.h"
#include "OFPMessageUtils.h"

class IOFPMsgParser 
{
public:
    virtual ~IOFPMsgParser(){};

    virtual CmResult     StreamFrom(ACE_InputCDR &is)                 = 0;
    // Important:
    // Currently, our code runs on x86(litter-endian machine), so @a os should be constructed with 
    // ACE_CDR::BYTE_ORDER_BIG_ENDIAN, to let ACE transform the byte order of the stream.
    virtual CmResult     StreamTo(ACE_OutputCDR &os)                  = 0;
    virtual ACE_UINT16   GetStreamLen()                         const = 0;
    virtual ACE_UINT16   GetEncodeSpace()                       const = 0;
    virtual void         Dump()                                 const = 0;
};


#endif  //IOFP_MSG_PARSER_H

