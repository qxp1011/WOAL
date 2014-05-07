/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPDescriptionMsg.cpp
 * Description:      Source file for openflow multipart description message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-16 
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPMPDescriptionMsg.h"
#include "CmError.h"

CmResult COFP13MPDescriptionReplyMsg::StreamFrom(ACE_InputCDR &is)
{
    if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }
    
    bool bGood = is.read_char_array(reinterpret_cast<char *>(&m_desc), sizeof(m_desc));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP13MPDescriptionReplyMsg::StreamTo(ACE_OutputCDR &os)
{
    if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
    {
        return CM_ERROR_FAILURE;
    }

    bool bGood = os.write_char_array(reinterpret_cast<const char *>(&m_desc), sizeof(m_desc));
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

ACE_UINT16 COFP13MPDescriptionReplyMsg::GetStreamLen() const
{
    return COFP13MultipartMsg::GetStreamLen()+sizeof(m_desc);
}

void COFP13MPDescriptionReplyMsg::Dump() const
{
    COFP13MultipartMsg::Dump();
}

