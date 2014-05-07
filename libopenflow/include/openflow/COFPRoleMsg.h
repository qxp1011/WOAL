/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPRoleMsg.h
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
#ifndef COFPROLEMSG_H
#define COFPROLEMSG_H

#include "COFPMessage.h"

class COFP13RoleMsg : public COFPMessage
{
public:
    /* Controller roles. */
    enum ofp_controller_role
    {
        OFPCR_ROLE_NOCHANGE = 0, /* Don't change current role. */
        OFPCR_ROLE_EQUAL = 1, /* Default role, full access. */
        OFPCR_ROLE_MASTER = 2, /* Full access, at most one master. */
        OFPCR_ROLE_SLAVE = 3, /* Read-only access. */
    };
    
public:
    COFP13RoleMsg(ACE_CDR::Octet abType, ACE_UINT32 adwRole = 0, ACE_UINT64 alId = 0);
    virtual ~COFP13RoleMsg(){}

    inline void SetRole(ACE_UINT32 adwRole);
    inline void SetGenerationId(ACE_UINT64 alId);
    inline ACE_UINT32 GetRole();
    inline ACE_UINT64 GetGenerationId();

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
    ACE_UINT32 m_dwRole;       /* One of OFPCR_ROLE_*. */
    ACE_UINT64 m_lGenerationId;/* Master Election Generation Id */
};

void COFP13RoleMsg::SetRole(ACE_UINT32 adwRole)
{
    m_dwRole = adwRole;
}

void COFP13RoleMsg::SetGenerationId(ACE_UINT64 alId)
{
    m_lGenerationId = alId;
}

ACE_UINT32 COFP13RoleMsg::GetRole()
{
    return m_dwRole;
}

ACE_UINT64 COFP13RoleMsg::GetGenerationId()
{
    return m_lGenerationId;
}

inline ACE_UINT16 COFP13RoleMsg::GetStreamLen() const
{
    return 16 + COFPMessage::GetStreamLen();
}


#endif

