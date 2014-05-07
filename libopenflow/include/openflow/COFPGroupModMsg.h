/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPGroupModMsg.h
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
#ifndef COFPGROUPMODMSG_H
#define COFPGROUPMODMSG_H

#include "COFPMessage.h"
#include "COFPBucket.h"

class COFP13GroupModMsg : public COFPMessage
{
public:
    /* Group commands */
    enum ofp_group_mod_command
    {
        OFPGC_ADD = 0,    /* New group. */
        OFPGC_MODIFY = 1, /* Modify all matching groups. */
        OFPGC_DELETE = 2, /* Delete all matching groups. */
    };

    /* Group types. Values in the range [128, 255] are reserved for experimental use. */
    enum ofp_group_type
    {
        OFPGT_ALL      = 0, /* All (multicast/broadcast) group. */
        OFPGT_SELECT   = 1, /* Select group. */
        OFPGT_INDIRECT = 2, /* Indirect group. */
        OFPGT_FF       = 3, /* Fast failover group. */
    };

    /* Group numbering. Groups can use any number up to OFPG_MAX. */
    enum ofp_group
    {
        /* Last usable group number. */
        OFPG_MAX = 0xffffff00,
        /* Fake groups. */
        OFPG_ALL = 0xfffffffc, /* Represents all groups for group delete commands. */
        OFPG_ANY = 0xffffffff  /* Wildcard group used only for flow stats requests. Selects
                                                     all flows regardless of group (including flows with no group). */
    };
    
public:
    COFP13GroupModMsg(
        ACE_UINT16 awCommand   = 0,   /* One of OFPGC_*. */
        ACE_CDR::Octet   abGroupType = 0,   /* One of OFPGT_*. */
        ACE_UINT32 adwGroupId  = 0);
    virtual ~COFP13GroupModMsg();

    inline ACE_UINT16 GetCommand()              const;
    inline ACE_CDR::Octet   GetGroupType()            const;
    inline ACE_UINT32 GetGroupId()              const;
    inline void SetCommand(ACE_UINT16 awCmd);
    inline void SetGroupType(ACE_CDR::Octet abType);
    inline void SetGroupId(ACE_UINT32 adwId);

    CmResult AddBucketList(std::list<COFP13Bucket *> &list);
    CmResult AppendBucket(COFP13Bucket *pBucket);
    CmResult GetBucketList(std::list<COFP13Bucket *> &list);

protected:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const;
    virtual ACE_UINT16   GetEncodeSpace() const;
    virtual void         Dump() const;

private:
    ACE_UINT16       m_wStreamLen;
    ACE_UINT16       m_wCommand;   /* One of OFPGC_*. */
    ACE_CDR::Octet   m_bGroupType; /* One of OFPGT_*. */
    ACE_UINT32       m_dwGroupId;
    std::list<COFP13Bucket *> m_bucket_list;
};


inline ACE_UINT16 COFP13GroupModMsg::GetCommand() const
{
    return m_wCommand;
}


inline ACE_CDR::Octet COFP13GroupModMsg::GetGroupType() const
{
    return m_bGroupType;
}


inline ACE_UINT32 COFP13GroupModMsg::GetGroupId() const
{
    return m_dwGroupId;
}


inline void COFP13GroupModMsg::SetCommand(ACE_UINT16 awCmd)
{
    m_wCommand = awCmd;
}


inline void COFP13GroupModMsg::SetGroupType(ACE_CDR::Octet abType)
{
    m_bGroupType = abType;
}


inline void COFP13GroupModMsg::SetGroupId(ACE_UINT32 adwId)
{
    m_dwGroupId = adwId;
}

inline ACE_UINT16 COFP13GroupModMsg::GetStreamLen() const
{
    return m_wStreamLen;
}

inline ACE_UINT16 COFP13GroupModMsg::GetEncodeSpace() const
{
    return GetStreamLen();
}

#endif

