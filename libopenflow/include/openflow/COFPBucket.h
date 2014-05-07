/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPBucket.h
 * Description:      OpenFlow bucket header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-13
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFP_BUCKET_H
#define COFP_BUCKET_H

#include "IOFPMsgParser.h"
#include "COFPAction.h"

class COFP13Bucket : public IOFPMsgParser
{
public:
    COFP13Bucket(
        ACE_UINT16 awWeight     = 0,
        ACE_UINT32 anWatchPort  = 0,
        ACE_UINT32 anWatchGroup = 0,
        ACE_UINT16 awLen        = 16
        );
    virtual ~COFP13Bucket();

    inline ACE_UINT16 GetLen()                                      const;
    inline ACE_UINT16 GetWeight()                                   const;
    inline ACE_UINT32 GetWatchPort()                                const;
    inline ACE_UINT32 GetWatchGroup()                               const;
    inline void   SetWeight(ACE_UINT16 awWeight);
    inline void   SetWatchPort(ACE_UINT32 adwWatchPort);
    inline void   SetWatchGroup(ACE_UINT32 adwWatchGroup);

    CmResult AddActionList(std::list<COFP13Action *> &list);
    CmResult AppendAction(COFP13Action *action);
    CmResult GetActionList(std::list<COFP13Action *> &list);

    static CmResult DecodeBucket(ACE_InputCDR &is, COFP13Bucket * &aBucket);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    ACE_UINT16 m_wLen;                       /* Length the bucket in bytes, including this header and any 
                                                padding to make it 64-bit aligned. */
    ACE_UINT16 m_wWeight;                    /* Relative weight of bucket. Only defined for select groups. */
    ACE_UINT32 m_dwWatchPort;                /* Port whose state affects whether this bucket is live. Only 
                                                required for fast failover groups. */
    ACE_UINT32 m_dwWatchGroup;               /* Group whose state affects whether this bucket is live. Only 
                                                required for fast failover groups. */
    std::list<COFP13Action *> m_action_list; /* The action length is inferred from the length field in the
                                                header. */
};


ACE_UINT16 COFP13Bucket::GetLen() const
{
    return m_wLen;
}


ACE_UINT16 COFP13Bucket::GetWeight() const
{
    return m_wWeight;
}


ACE_UINT32 COFP13Bucket::GetWatchPort() const
{
    return m_dwWatchPort;
}


ACE_UINT32 COFP13Bucket::GetWatchGroup() const
{
    return m_dwWatchGroup;
}


void COFP13Bucket::SetWeight(ACE_UINT16 awWeight)
{
    m_wWeight = awWeight;
}


void COFP13Bucket::SetWatchPort(ACE_UINT32 adwWatchPort)
{
    m_dwWatchPort = adwWatchPort;
}


void COFP13Bucket::SetWatchGroup(ACE_UINT32 adwWatchGroup)
{
    m_dwWatchGroup = adwWatchGroup;
}

#endif  // COFP_BUCKET_H
