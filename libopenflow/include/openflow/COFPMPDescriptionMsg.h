/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPDescriptionMsg.h
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
#ifndef COFPMPDESCRIPTIONMSG_H
#define COFPMPDESCRIPTIONMSG_H

#include "COFPMultipartMsg.h"

class COFP13MPDescriptionRequestMsg : public COFP13MultipartRequestMsg
{
public:
    COFP13MPDescriptionRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_DESC)
    {
    }
    virtual ~COFP13MPDescriptionRequestMsg(){}
};

static const ACE_UINT32 descStrLength = 256;
static const ACE_UINT32 serialNumLength = 32;

class COFP13MPDescriptionReplyMsg : public COFP13MultipartReplyMsg
{
public:
    struct ofp_desc {
        char mfr_desc[descStrLength]; /* Manufacturer description. */
        char hw_desc[descStrLength]; /* Hardware description. */
        char sw_desc[descStrLength]; /* Software description. */
        char serial_num[serialNumLength]; /* Serial number. */
        char dp_desc[descStrLength]; /* Human readable description of datapath. */
    };

    COFP13MPDescriptionReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_DESC)
    {
        ::memset(&m_desc, 0, sizeof(m_desc));
    }
    virtual ~COFP13MPDescriptionReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR& os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

private:
    ofp_desc m_desc;
};

#endif

