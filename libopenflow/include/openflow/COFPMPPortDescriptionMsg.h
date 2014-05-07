/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPPortDescriptionMsg.h
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
#ifndef COFPMPPORTDESCRIPTIONMSG_H
#define COFPMPPORTDESCRIPTIONMSG_H

#include "COFPMultipartMsg.h"
#include "COFPPort.h"
class COFP13MPPortDescRequestMsg: public COFP13MultipartRequestMsg
{
public:
    
    COFP13MPPortDescRequestMsg()
        :COFP13MultipartRequestMsg(OFPMP_PORT_DESC)
    {
    }
    virtual ~COFP13MPPortDescRequestMsg(){}

};

class COFP13MPPortDescReplyMsg : public COFP13MultipartReplyMsg
{
public:
    COFP13MPPortDescReplyMsg()
        :COFP13MultipartReplyMsg(OFPMP_PORT_DESC)
    {
    
    }
    virtual ~COFP13MPPortDescReplyMsg();

    CmResult AddPortList(std::list<COFP13Port *> &aList);
    CmResult AppendPort(COFP13Port *aPort);
    CmResult GetPortList(std::list<COFP13Port *> &aList);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()      const;
    virtual ACE_UINT16   GetEncodeSpace()    const
    {
        return GetStreamLen();
    }
    virtual void         Dump()              const;
    
private:
    std::list<COFP13Port *> m_port_list;
};

#endif

