/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        OFPMessageUtils.cpp
 * Description:      OpenFlow message utilities source file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-12
 *
 * History 1:
 *     Date:         2013-12-16
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "OFPMessageUtils.h"
#include "ace/Assert.h"
#include "CmError.h"

/* Copy memory from src to des. If the memory of des is insufficient, a larger memory is allocated. */
CmResult COFPMsgMemoryMgr::Copy(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_CDR::Octet &abDesUsedMemSize,
                                const ACE_CDR::Octet *apSrc, ACE_CDR::Octet abSrcSize)
{
    //CM_ASSERTE_RETURN((apDes != NULL), CM_ERROR_NULL_POINTER);
    //CM_ASSERTE_RETURN((apSrc != NULL), CM_ERROR_NULL_POINTER);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPMsgMemoryMgr::Copy, push\n")));
    
    if (awDesMemSize < abSrcSize)
    {
        if (apDes != NULL)
        {
            aAlloc.deallocate(apDes, awDesMemSize);
            apDes            = NULL;
            awDesMemSize     = 0;
            abDesUsedMemSize = 0;
        }

        apDes = aAlloc.allocate(abSrcSize);
        CM_ASSERT_RETURN((apDes != NULL), CM_ERROR_OUT_OF_MEMORY);
        awDesMemSize = abSrcSize;
    }

    if ((apSrc != NULL) && (abSrcSize != 0))
    {
        ::memcpy(apDes, apSrc, abSrcSize);
        abDesUsedMemSize = abSrcSize;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPMsgMemoryMgr::Copy, pop\n")));

    return CM_OK;
}


/* Copy memory from src to des. If the memory of des is insufficient, a larger memory is allocated. */
CmResult COFPMsgMemoryMgr::Copy(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize,
                                const ACE_CDR::Octet *apSrc, ACE_UINT16 awSrcSize)
{
    //CM_ASSERTE_RETURN((apDes != NULL), CM_ERROR_NULL_POINTER);
    //CM_ASSERTE_RETURN((apSrc != NULL), CM_ERROR_NULL_POINTER);

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPMsgMemoryMgr::Copy, push\n")));
    
    if (awDesMemSize < awSrcSize)
    {
        if (apDes != NULL)
        {
            aAlloc.deallocate(apDes, awDesMemSize);
            apDes            = NULL;
            awDesMemSize     = 0;
            awDesUsedMemSize = 0;
        }

        apDes = aAlloc.allocate(awSrcSize);
        CM_ASSERT_RETURN((apDes != NULL), CM_ERROR_OUT_OF_MEMORY);
        awDesMemSize = awSrcSize;
    }

    if ((apSrc != NULL) && (awSrcSize != 0))
    {
        ::memcpy(apDes, apSrc, awSrcSize);
        awDesUsedMemSize = awSrcSize;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPMsgMemoryMgr::Copy, pop\n")));

    return CM_OK;
}


/* Decode paddings with the specified length. */
CmResult COFPMsgMemoryMgr::PadStreamFrom(ACE_InputCDR &aIs, ACE_UINT16 aPadLen)
{
    if (0 == aPadLen)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPMsgMemoryMgr::PadStreamFrom, aPadLen = 0\n")));
        return CM_OK;
    }

    std::allocator<ACE_CDR::Octet> alloc;
    ACE_CDR::Octet *pad = alloc.allocate(aPadLen);
    CM_ASSERT_RETURN(pad != NULL, CM_ERROR_OUT_OF_MEMORY);
    
    bool bGood = aIs.read_octet_array(pad, aPadLen);
    if (!bGood)
    {
        ACE_ASSERT(0);
        alloc.deallocate(pad, aPadLen);
        return CM_ERROR_FAILURE;
    }

    alloc.deallocate(pad, aPadLen);

    return CM_OK;
}


/* Encode paddings with the specified length. */
CmResult COFPMsgMemoryMgr::PadStreamTo(ACE_OutputCDR &aOs, ACE_UINT16 aPadLen)
{
    if (0 == aPadLen)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFPMsgMemoryMgr::PadStreamTo, aPadLen = 0\n")));
        return CM_OK;
    }
    
    std::allocator<ACE_CDR::Octet> alloc;
    ACE_CDR::Octet *pad = alloc.allocate(aPadLen);
    CM_ASSERT_RETURN(pad != NULL, CM_ERROR_OUT_OF_MEMORY);
    ::memset(pad, 0, aPadLen);

    aOs.write_octet_array(pad, aPadLen);
    bool bGood = aOs.good_bit();
    if (!bGood)
    {
        ACE_ASSERT(0);
        alloc.deallocate(pad, aPadLen);
        return CM_ERROR_FAILURE;
    }

    alloc.deallocate(pad, aPadLen);

    return CM_OK;
}


/* Decode data from the specified stream. If the memory is insufficient, reallocate it. */
CmResult COFPMsgMemoryMgr::DataStreamFrom(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_InputCDR& aIs, ACE_UINT16 awDataSize,
                                          ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize)
{
    if (awDesMemSize < awDataSize)
    {
        if (apDes != NULL)
        {
            aAlloc.deallocate(apDes, awDesMemSize);
            apDes            = NULL;
            awDesMemSize     = 0;
            awDesUsedMemSize = 0;
        }

        apDes = aAlloc.allocate(awDataSize);
        CM_ASSERT_RETURN(apDes != NULL, CM_ERROR_OUT_OF_MEMORY);
        ::memset(apDes, 0, awDataSize);
        awDesMemSize = awDataSize;
    }

    bool bGood = aIs.read_octet_array(apDes, awDataSize);
    CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
    awDesUsedMemSize = awDataSize;

    return CM_OK;
}


/* Returns true if the 'n' bytes starting at 'p' are zeros. */
bool COFPMsgCommonUtils::is_all_zeros(const ACE_CDR::Octet *p, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++) {
        if (p[i] != 0x00) {
            return false;
        }
    }
    return true;
}


/* Returns true if the 'n' bytes starting at 'p' are 0xff. */
bool COFPMsgCommonUtils::is_all_ones(const ACE_CDR::Octet *p, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++) {
        if (p[i] != 0xff) {
            return false;
        }
    }
    return true;
}


ACE_CDR::Octet COFPMsgCommonUtils::CalcIpOneBitNum(ACE_UINT32 dwIpAddr)
{
    ACE_CDR::Octet   bBitCnt = 0;
    ACE_CDR::Octet   bShift  = 0;
    ACE_UINT32 dwCmp   = 0x80000000;

    if (0 == dwIpAddr)
    {
        return bBitCnt;
    }

    if (dwIpAddr >= dwCmp)
    {
        bBitCnt++;
    }

    while ((dwIpAddr <<= 1) >= dwCmp)
    {
        bBitCnt++;
        bShift++;
        if (bShift >= 31)
        {
            break;
        }
    }

    return bBitCnt;
}


bool COFPMsgCommonUtils::ipv6_addr_is_zero(const ACE_CDR::Octet addr[16])
{
    for (ACE_CDR::Octet i = 0; i < 16; ++i)
    {
        if (addr[i] != 0)
        {
            return false;
        }
    }

    return true;
}


int COFPMsgCommonUtils::ipv6_mask_is_exact(const ACE_CDR::Octet addr[16])
{
    for (ACE_CDR::Octet i = 0; i < 16; ++i)
    {
        if (addr[i] != OFP_MSG_UTILS_BYTE_MAX_VALUE)
        {
            return false;
        }
    }

    return true;
}


bool COFPMsgCommonUtils::array_is_zero(const ACE_CDR::Octet *apMask, ACE_UINT16 awArraySize)
{
    CM_ASSERT_RETURN(apMask != nullptr, false);
    
    for (ACE_UINT16 i = 0; i < awArraySize; ++i)
    {
        if (apMask[i] != 0)
        {
            return false;
        }
    }

    return true;
}



