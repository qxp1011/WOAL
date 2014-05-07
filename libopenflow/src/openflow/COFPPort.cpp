/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPort.cpp
 * Description:      OpenFlow port source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-18
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPPort.h"
#include "OFPMessageUtils.h"
#include "CmError.h"

COFP10PhyPort::COFP10PhyPort()
    : m_wPortNum(0)
    , m_nConfig(0)                       /* Bitmap of OFPPC_* flags. */
    , m_nState(0)                        /* Bitmap of OFPPS_* flags. */
    /* Bitmaps of OFPPF_* that describe features. All bits zeroed if unsupported or unavailable. */
    , m_nCurr(0)                         /* Current features. */
    , m_nAdvertised(0)                   /* Features being advertised by the port. */
    , m_nSupported(0)                    /* Features supported by the port. */
    , m_nPeer(0)                         /* Features advertised by peer. */
{
    ::memset(m_bHWAddr, 0, OFP_ETH_ALEN);
    ::memset(m_acName, 0, OFP_MAX_PORT_NAME_LEN); /* Null-terminated */
}

void COFP10PhyPort::SetHWAddr(const ACE_CDR::Octet *apHWAddr, ACE_UINT16 awHWAddrLen)
{
    if (NULL == apHWAddr)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10PhyPort::SetHWAddr(): apHWAddr is NULL.\n")));
        return;
    }

    if (awHWAddrLen != OFP_ETH_ALEN)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP10PhyPort::SetHWAddr(): awHWAddrLen(%u) ISNOT %u\n"), 
                   awHWAddrLen,
                   OFP_ETH_ALEN));
        return;
    }

    ::memcpy(m_bHWAddr, apHWAddr, awHWAddrLen);
}

void COFP10PhyPort::SetName(const char *apName, ACE_UINT16 awLen)
{
    if (NULL == apName)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP10PhyPort::SetName(): apName is NULL.\n")));
        return;
    }

    if (awLen > OFP_MAX_PORT_NAME_LEN)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP10PhyPort::SetName(): awLen(%u) exceeds %u\n"), 
                   awLen,
                   OFP_MAX_PORT_NAME_LEN));
        return;
    }

    ::strncpy(m_acName, apName, awLen);
    m_acName[OFP_MAX_PORT_NAME_LEN-1] = 0;
}

CmResult COFP10PhyPort::
DecodePort(ACE_InputCDR &is, COFP10PhyPort * &aPort)
{
    CmResult lRet = CM_ERROR_FAILURE;
    
    COFP10PhyPort *Port = new COFP10PhyPort();
    CM_ASSERT_RETURN(Port != nullptr, CM_ERROR_OUT_OF_MEMORY);

    lRet = Port->StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(lRet), lRet);
    
    aPort = Port;

    return CM_OK;
}

CmResult COFP10PhyPort::StreamFrom(ACE_InputCDR &is)
{
    is>>m_wPortNum;
    is.read_octet_array(m_bHWAddr, OFP_ETH_ALEN);
    is.read_char_array(m_acName, OFP_MAX_PORT_NAME_LEN);
    is>>m_nConfig;
    is>>m_nState;
    is>>m_nCurr;
    is>>m_nAdvertised;
    is>>m_nSupported;
    is>>m_nPeer;

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}

CmResult COFP10PhyPort::StreamTo(ACE_OutputCDR &os)
{
    os<<m_wPortNum;
    os.write_octet_array(m_bHWAddr, OFP_ETH_ALEN);    
    os.write_char_array(m_acName, OFP_MAX_PORT_NAME_LEN);
    os<<m_nConfig;
    os<<m_nState;
    os<<m_nCurr;
    os<<m_nAdvertised;
    os<<m_nSupported;
    os<<m_nPeer;

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}



COFP13Port::COFP13Port()
    : m_nPortNum(0)
    , m_nConfig(0)
    , m_nState(0)
    , m_nCurr(0)
    , m_nAdvertised(0)
    , m_nSupported(0)
    , m_nPeer(0)
    , m_nCurrSpeed(0)
    , m_nMaxSpeed(0)
{
    ::memset(m_bHWAddr, 0, OFP_ETH_ALEN);
    ::memset(m_acName,  0, OFP_MAX_PORT_NAME_LEN);
}

COFP13Port::COFP13Port(ACE_UINT32 anPortNum
    , ACE_CDR::Octet  *apHWAddrVal
    , ACE_UINT32 anHWAddrLen
    , char  *apNameVal
    , ACE_UINT32 anNameLen
    , ACE_UINT32 anConfig                       /* Bitmap of OFPPC_* flags. */
    , ACE_UINT32 anState                        /* Bitmap of OFPPS_* flags. */
    , ACE_UINT32 anCurr                         /* Current features. */
    , ACE_UINT32 anAdvertised                   /* Features being advertised by the port. */
    , ACE_UINT32 anSupported                    /* Features supported by the port. */
    , ACE_UINT32 anPeer                         /* Features advertised by peer. */
    , ACE_UINT32 anCurrSpeed                    /* Current port bitrate in kbps. */
    , ACE_UINT32 anMaxSpeed)                    /* Max port bitrate in kbps */
    : m_nPortNum(anPortNum)
    , m_nConfig(anConfig)
    , m_nState(anState)
    , m_nCurr(anCurr)
    , m_nAdvertised(anAdvertised)
    , m_nSupported(anSupported)
    , m_nPeer(anPeer)
    , m_nCurrSpeed(anCurrSpeed)
    , m_nMaxSpeed(anMaxSpeed)
{
    if (NULL == apHWAddrVal)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Port::COFP13Port(): apHWAddrVal is NULL.\n")));
        return;
    }

    if (NULL == apNameVal)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Port::COFP13Port(): apNameVal is NULL.\n")));
        return;
    }
        
    if (anHWAddrLen != OFP_ETH_ALEN)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP13Port::COFP13Port(): anHWAddrLen(%u) ISNOT %u\n"), 
                   anHWAddrLen, 
                   OFP_ETH_ALEN));
        return;
    }

    if (anNameLen > OFP_MAX_PORT_NAME_LEN)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP13Port::COFP13Port(): anNameLen(%u) exceeds %u\n"),
                   anNameLen,
                   OFP_MAX_PORT_NAME_LEN));
        return;
    }

    ::memset(m_bHWAddr, 0, OFP_ETH_ALEN);
    ::memset(m_acName,  0, OFP_MAX_PORT_NAME_LEN);
    
    ::memcpy(m_bHWAddr, apHWAddrVal, anHWAddrLen);
    ::strncpy(m_acName, apNameVal, anNameLen);
    m_acName[OFP_MAX_PORT_NAME_LEN-1] = 0;
}

/* use the specified memory to initialize COFP13Port. The format of the memory is as follows,
struct ofp_port {
    uint32_t port_no;
    uint8_t pad[4];
    uint8_t hw_addr[OFP_ETH_ALEN];
    uint8_t pad2[2]; 
    char name[OFP_MAX_PORT_NAME_LEN]; 
    uint32_t config;
    uint32_t state; 
    uint32_t curr; 
    uint32_t advertised; 
    uint32_t supported; 
    uint32_t peer; 
    uint32_t curr_speed;
    uint32_t max_speed; 
};
OFP_ASSERT(sizeof(struct ofp_port) == 64);
*/
COFP13Port::COFP13Port(const ACE_CDR::Octet *apSrc, ACE_UINT32 anLen)
{
    const ACE_CDR::Octet *pPtr = NULL;
    ACE_ASSERT(anLen == COFP13Port::GetStreamLen());
    ACE_ASSERT(apSrc != NULL);

    if (NULL == apSrc)
    {
        return;
    }
    
    m_nPortNum = *((ACE_UINT32 *)apSrc);
    apSrc += 8;
    ::memcpy(m_bHWAddr, apSrc, OFP_ETH_ALEN);
    apSrc += (OFP_ETH_ALEN + 2);
    ::memcpy(m_acName, apSrc, OFP_MAX_PORT_NAME_LEN);
    apSrc += OFP_MAX_PORT_NAME_LEN;
    m_nConfig = *((ACE_UINT32 *)apSrc);
    apSrc += 4;
    m_nState = *((ACE_UINT32 *)apSrc);
    apSrc += 4;
    m_nCurr = *((ACE_UINT32 *)apSrc);
    apSrc += 4;
    m_nAdvertised = *((ACE_UINT32 *)apSrc);
    apSrc += 4;
    m_nSupported = *((ACE_UINT32 *)apSrc);
    apSrc += 4;
    m_nPeer = *((ACE_UINT32 *)apSrc);
    apSrc += 4;
    m_nCurrSpeed = *((ACE_UINT32 *)apSrc);
    apSrc += 4;
    m_nMaxSpeed = *((ACE_UINT32 *)apSrc);

    return;
}

void COFP13Port::SetHWAddr(const ACE_CDR::Octet *apHWAddr, ACE_UINT16 awHWAddrLen)
{
    if (NULL == apHWAddr)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Port::SetHWAddr(): apHWAddr is NULL.\n")));
        return;
    }

    if (awHWAddrLen != OFP_ETH_ALEN)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP13Port::SetHWAddr(): awHWAddrLen(%u) ISNOT %u\n"), 
                   awHWAddrLen, 
                   OFP_ETH_ALEN));
        return;
    }

    ::memcpy(m_bHWAddr, apHWAddr, awHWAddrLen);
}

void COFP13Port::SetName(const char *apName, ACE_UINT16 awLen)
{
    if (NULL == apName)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Port::SetName(): apName is NULL.\n")));
        return;
    }

    if (awLen > OFP_MAX_PORT_NAME_LEN)
    {
        ACE_ASSERT(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Port::SetName(): awLen(%u) exceeds %u\n"), awLen, OFP_MAX_PORT_NAME_LEN));
        return;
    }

    ::strncpy(m_acName, apName, awLen);
    m_acName[OFP_MAX_PORT_NAME_LEN-1] = 0;
}

CmResult COFP13Port::
DecodePort(ACE_InputCDR &is, COFP13Port * &apPort)
{
    CmResult result = CM_ERROR_FAILURE;
    COFP13Port *pPort = new COFP13Port();
    CM_ASSERT_RETURN(pPort != nullptr, CM_ERROR_OUT_OF_MEMORY);

    result = pPort->StreamFrom(is);
    CM_ASSERT_RETURN(CM_SUCCEEDED(result), result);
    
    apPort = pPort;
    
    return CM_OK;
}

CmResult COFP13Port::StreamFrom(ACE_InputCDR &is)
{
    ACE_UINT32 i = 0;
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;

    is>>m_nPortNum;
    is>>pad;
    for (i = 0; i < OFP_ETH_ALEN; ++i)
    {
        is>>m_bHWAddr[i];
    }
    is>>pad2;
    for (i = 0; i < OFP_MAX_PORT_NAME_LEN; ++i)
    {
        is>>m_acName[i];
    }
    is>>m_nConfig;                       /* Bitmap of OFPPC_* flags. */
    is>>m_nState;                        /* Bitmap of OFPPS_* flags. */
    /* Bitmaps of OFPPF_* that describe features. All bits zeroed if unsupported or unavailable. */
    is>>m_nCurr;                         /* Current features. */
    is>>m_nAdvertised;                   /* Features being advertised by the port. */
    is>>m_nSupported;                    /* Features supported by the port. */
    is>>m_nPeer;                         /* Features advertised by peer. */
    is>>m_nCurrSpeed;                    /* Current port bitrate in kbps. */
    is>>m_nMaxSpeed;                     /* Max port bitrate in kbps */

    bool bGood = is.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


CmResult COFP13Port::StreamTo(ACE_OutputCDR &os)
{
    ACE_UINT32 i = 0;
    ACE_UINT32 pad = 0;
    ACE_UINT16 pad2 = 0;

    os<<m_nPortNum;
    os<<pad;
    os.write_octet_array(m_bHWAddr, OFP_ETH_ALEN);
    os<<pad2;
    os.write_char_array(m_acName, OFP_MAX_PORT_NAME_LEN);
    os<<m_nConfig;                       /* Bitmap of OFPPC_* flags. */
    os<<m_nState;                        /* Bitmap of OFPPS_* flags. */
    /* Bitmaps of OFPPF_* that describe features. All bits zeroed if unsupported or unavailable. */
    os<<m_nCurr;                         /* Current features. */
    os<<m_nAdvertised;                   /* Features being advertised by the port. */
    os<<m_nSupported;                    /* Features supported by the port. */
    os<<m_nPeer;                         /* Features advertised by peer. */
    os<<m_nCurrSpeed;                    /* Current port bitrate in kbps. */
    os<<m_nMaxSpeed;                     /* Max port bitrate in kbps */

    bool bGood = os.good_bit();
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    return CM_OK;
}


void COFP13Port::Dump() const
{
    ACE_UINT32 i = 0;

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13Port::Dump()\n")));
    ACE_DEBUG((LM_DEBUG, 
               ACE_TEXT("PortNum=%u, config=%u, state=%u, curr=%u, advertised=%u, \n"),
               ACE_TEXT("supported=%u, peer=%u, currSpeed=%u, maxSpeed=%u\n"),
               ACE_TEXT("HWAddr: %x-%x-%x-%x-%x-%x\n"),
               m_nPortNum, m_nConfig, m_nState, m_nCurr, m_nAdvertised, 
               m_nSupported, m_nPeer, m_nCurrSpeed, m_nMaxSpeed, 
               m_bHWAddr[0], m_bHWAddr[1], m_bHWAddr[2], m_bHWAddr[3], m_bHWAddr[4], m_bHWAddr[5]));

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("Name=%s\n"), m_acName));
}


