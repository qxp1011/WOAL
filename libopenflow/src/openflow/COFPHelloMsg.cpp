/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPHelloMsg.cpp
 * Description:      Source file for openflow hello message.
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:         2013-12-15
 *     Version:      V1.1
 *     Author:       mazhh@wfnex.com
 *     Modification: Adapt to ACE.
**********************************************************************/

#include "COFPHelloMsg.h"
#include "ace/Log_Msg.h"
#include "CmError.h"

std::allocator<ACE_CDR::Octet> COFP13HelloMsg::m_alloc;        // Handler for allocating/deallocating memory

COFP13HelloMsg::COFP13HelloMsg(
    ACE_UINT16  awElemListLen,
    ACE_CDR::Octet   *apElemList,        // Hello elements with the format: {(struct ofp_hello_elem_header) + data}
    // args for OFP header
    ACE_UINT32  adwxid,
    ACE_CDR::Octet    abVersion)
    : COFPMessage(OFP13::OFPT_HELLO, abVersion, adwxid, awElemListLen + COFPMessage::GetStreamLen())
    , m_pElemList(NULL)
    , m_wElemListLen(0)
    , m_wElemListMemSize(0)
{
    if (awElemListLen < 4)
    {
        //CM_ASSERTE(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::COFP13HelloMsg(): awElemListLen(%u) < 4.\n"),
                   awElemListLen));
        return;
    }

    m_pElemList = m_alloc.allocate(awElemListLen);
    if (NULL == m_pElemList)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::COFP13HelloMsg(): m_alloc.allocate(%u) failed.\n"),
                   awElemListLen));
        return;        
    }

    m_wElemListMemSize = awElemListLen;
    ::memset(m_pElemList, 0, awElemListLen);

    if (NULL == apElemList)
    {
        //CM_ASSERTE(0);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13HelloMsg::COFP13HelloMsg(): arg in, apElemList, is NULL.\n")));
        return;
    }

    ::memcpy(m_pElemList, apElemList, awElemListLen);
    m_wElemListLen = awElemListLen;
    
    return;
}


COFP13HelloMsg::~COFP13HelloMsg()
{
    if (m_pElemList != NULL)
    {
        m_alloc.deallocate(m_pElemList, m_wElemListMemSize);
        m_pElemList = NULL;
    }
}


/* Common header for all Hello Elements */
//struct ofp_hello_elem_header {
//uint16_t type; /* One of OFPHET_*. */
//uint16_t length; /* Length in bytes of this element. */
//};
// apHelloElem in the following funcs is the addr of the hello elem header.
ACE_UINT16 COFP13HelloMsg::GetHelloElemType(const ACE_CDR::Octet *apHelloElem) const
{
    if (NULL == apHelloElem)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::GetHelloElemType(): arg in, apHelloElem, is NULL.\n")));
        return 0;
    }
    
    return *((ACE_UINT16 *)apHelloElem);
}


// Get the length of the hello elem without paddings.
ACE_UINT16 COFP13HelloMsg::GetHelloElemLen(const ACE_CDR::Octet *apHelloElem) const 
{
    if (NULL == apHelloElem)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::GetHelloElemLen(): arg in(apHelloElem) is NULL.\n")));
        return 0;
    }

    return *((ACE_UINT16 *)(apHelloElem + 2));
}


// Get the specified bitmap in the Version Bitmap Hello Elem.
// Notice: you must ensure the length is right.
// ofp versions 0 to 31 are encoded in the first bitmap(awIndex = 0), ofp versions 32 to 63 are encoded in the second 
// bitmap(awIndex = 1) and so on.
// eg: you can use GetVerBMHelloElemBitmap(apElem, 0) to get the first bitmap, which encodes ofp versions 0 to 31.
// args in:
// @apVerBMHelloElem:  the addr of the Version Bitmap Hello Elem
// @awIndex:                index of the bitmap, starting from 0
ACE_UINT32 COFP13HelloMsg::
GetVerBMHelloElemBitmap(const ACE_CDR::Octet *apVerBMHelloElem, ACE_UINT16 awIndex) const
{
    ACE_UINT32 nBitmap = 0;
    
    if (NULL == apVerBMHelloElem)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::GetVerBMHelloElemBitmap(): arg in, apVerBMHelloElem, is NULL.\n")));
        return nBitmap;
    }

    const ACE_UINT16 wElemLen = GetHelloElemLen(apVerBMHelloElem);
    if (wElemLen < 4)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::GetVerBMHelloElemBitmap(): wElemLen(%u) is less than 4.\n"),
                   wElemLen));
        return nBitmap;
    }

    const ACE_UINT16 wElemMinLen = (4 + (awIndex + 1) * 4);
    if (wElemLen < wElemMinLen)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::GetVerBMHelloElemBitmap(): wElemLen(%u) is less than wElemMinLen(%u)\n"),
                   wElemLen,
                   wElemMinLen));
        return nBitmap;
    }

    nBitmap = *((ACE_UINT32 *)(apVerBMHelloElem + 4) + awIndex);
    return nBitmap;
}    


CmResult COFP13HelloMsg::
SetHelloElemList(const ACE_CDR::Octet *apSrcElem, ACE_UINT16 awElemListLen)
{
    CmResult result = CM_ERROR_FAILURE;
    
    if (NULL == apSrcElem)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::SetHelloElemList(): arg in, apSrcElem, is NULL.\n")));
        return CM_ERROR_NULL_POINTER;
    }

    if (awElemListLen < 4)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::SetHelloElemList(): arg in, awElemListLen(%u) < 4.\n"), 
                   awElemListLen));
        return CM_ERROR_INVALID_ARG;
    }

    result = COFPMsgMemoryMgr::Copy(m_alloc, m_pElemList, m_wElemListMemSize, 
                                         m_wElemListLen, apSrcElem, awElemListLen);

    return result;
}


CmResult COFP13HelloMsg::SetHelloElemType(ACE_CDR::Octet *apHelloElem, ACE_UINT16 awType)
{
    if (NULL == apHelloElem)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::SetHelloElemType(): arg in, apHelloElem, is NULL.\n")));
        return CM_ERROR_NULL_POINTER;
    }

    *((ACE_UINT16 *)apHelloElem) = awType;
    return CM_OK;
}


// Set the length of the hello elem without paddings.
CmResult COFP13HelloMsg::SetHelloElemLen(ACE_CDR::Octet *apHelloElem, ACE_UINT16 awLen)
{
    if (NULL == apHelloElem)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::SetHelloElemLen(): arg in, apHelloElem, is NULL.\n")));
        return CM_ERROR_NULL_POINTER;
    }

    *((ACE_UINT16 *)(apHelloElem + 2)) = awLen;
    return CM_OK;
}


// Set the specified bitmap in the Version Bitmap Hello Elem.
// Notice: you must ensure the length is right.
// ofp versions 0 to 31 are encoded in the first bitmap(awIndex = 0), ofp versions 32 to 63 are encoded in the second 
// bitmap(awIndex = 1) and so on.
CmResult COFP13HelloMsg::
SetVerBMHelloElemBitmap(ACE_CDR::Octet *apVerBMHelloElem, ACE_UINT16 awIndex, ACE_UINT32 anBitmap)
{
    if (NULL == apVerBMHelloElem)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::SetVerBMHelloElemBitmap(): arg in, apVerBMHelloElem, is NULL.\n")));
        return CM_ERROR_NULL_POINTER;
    }

    const ACE_UINT16 wElemLen = GetHelloElemLen(apVerBMHelloElem);
    if (wElemLen < 4)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::SetVerBMHelloElemBitmap(): wElemLen(%u) is less than 4.\n"),
                   wElemLen));
        return CM_ERROR_INVALID_ARG;
    }

    const ACE_UINT16 wElemMinLen = (4 + (awIndex + 1) * 4);
    if (wElemLen < wElemMinLen)
    {
        ACE_ASSERT(0);
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::SetVerBMHelloElemBitmap(): wElemLen(%u) is less than wElemMinLen(%u)\n"),
                   wElemLen,
                   wElemMinLen));
        return CM_ERROR_INVALID_ARG;
    }

    *((ACE_UINT32 *)(apVerBMHelloElem + 4) + awIndex) = anBitmap;

    return CM_OK;
}


/* Set simple version bitmap hello elem, which contains only one bitmap. */
CmResult COFP13HelloMsg::SetSimpleVerBMHelloElem(ACE_UINT32 anBitmap)
{
    CmResult result   = CM_OK;
    ACE_UINT16   wElemLen = 2*sizeof(ACE_UINT16)+sizeof(ACE_UINT32);
    
    result = SetHelloElemType(m_pElemList, OFPHET_VERSIONBITMAP);
    if (result != CM_OK)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP13HelloMsg::SetSimpleVerBMHelloElem(): "),
                   ACE_TEXT("SetHelloElemType(m_pElemList, OFPHET_VERSIONBITMAP) returns %l. m_pElemList = %p\n"),
                   result,
                   m_pElemList));
        return result;
    }
    
    result = SetHelloElemLen(m_pElemList, wElemLen);
    if (result != CM_OK)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP13HelloMsg::SetSimpleVerBMHelloElem(): "),
                   ACE_TEXT("SetHelloElemLen(m_pElemList, wElemLen) returns %l. m_pElemList=%p, wElemLen=%u\n"),
                   result,
                   m_pElemList,
                   wElemLen));
        return result;
    }
    
    result = SetVerBMHelloElemBitmap(m_pElemList, 0, anBitmap);
    if (result != CM_OK)
    {
        ACE_DEBUG((LM_DEBUG, 
                   ACE_TEXT("COFP13HelloMsg::SetSimpleVerBMHelloElem(): "),
                   ACE_TEXT("SetVerBMHelloElemBitmap(m_pElemList, 0, anBitmap) returns %l. m_pElemList=%p, anBitmap=%u\n"),
                   result,
                   m_pElemList,
                   anBitmap));
        return result;
    }
    
    m_wElemListLen = wElemLen;

    return result;
}


CmResult COFP13HelloMsg::StreamFrom(ACE_InputCDR &is)
{
    CmResult lRet = CM_ERROR_FAILURE;
    ACE_UINT16 i = 0;
    bool bGood;

    lRet = COFPMessage::StreamFrom(is);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::StreamFrom(), COFPMessage::StreamFrom fail\n")));
        return lRet;
    }
    
    if (m_wLength < COFPMessage::GetStreamLen())
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::StreamFrom(): "),
                   ACE_TEXT("m_wLength(%u) is less than COFPMessage::GetStreamLen()(%u).\n"), 
                   m_wLength,
                   COFPMessage::GetStreamLen()));
        return CM_ERROR_FAILURE;
    }

    ACE_UINT16 wElemListLen = m_wLength - COFPMessage::GetStreamLen();
    ACE_UINT16 data_size    = wElemListLen;
    ACE_UINT16 iter_size    = 0;
    ACE_CDR::Octet  *pElem        = m_pElemList;

    while (iter_size < wElemListLen)
    {
        ACE_UINT16 elem_type = 0;
        ACE_UINT16 elem_size = 0;  // length of the element, excluding paddings
        
        is>>elem_type;
        is>>elem_size;
        
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13HelloMsg::StreamFrom(): parse hello elem %u.\n"), i++));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("elem_type=%u, elem_size=%u\n"), elem_type, elem_size));

        SetHelloElemType(pElem, elem_type);
        SetHelloElemLen(pElem, elem_size);
        
        if (elem_size < 4)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::StreamFrom(): elem_size(%u) error, less than 4.\n"),
                       elem_size));
            return CM_ERROR_FAILURE;
        }

        if (((elem_size + 7)/8*8) > data_size)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::StreamFrom(): "),
                       ACE_TEXT("((elem_size + 7)/8*8)=%u, greater than data_size(%u)\n"),
                       ((elem_size + 7)/8*8),
                       data_size));
            return CM_ERROR_FAILURE;
        }
        
        if (elem_type != OFPHET_VERSIONBITMAP)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::StreamFrom(): elem_type(%u) isnot OFPHET_VERSIONBITMAP(%u)\n"),
                       elem_type,
                       OFPHET_VERSIONBITMAP));
            return CM_ERROR_FAILURE;
        }

        ACE_UINT16 bitmap_size      = elem_size-4;
        CM_ASSERT_RETURN((bitmap_size/4*4 == bitmap_size), CM_ERROR_INVALID_ARG);
        
        ACE_UINT16 iter_bitmap_size = 0;
        ACE_UINT16 bitmap_index     = 0;

        while (iter_bitmap_size < bitmap_size)//(bitmap_size > 0)
        {
            ACE_UINT32 bitmap = 0;
            is>>bitmap;
            bGood = is.good_bit();
            CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("bitmap[%u]=%u\n"), bitmap_index, bitmap));

            SetVerBMHelloElemBitmap(pElem, bitmap_index++, bitmap);
            //bitmap_size -= sizeof(bitmap);
            iter_bitmap_size += sizeof(bitmap);
        }

        // deal with paddings
        ACE_UINT16 pad_size = (elem_size + 7)/8*8 - elem_size;
        is.read_octet_array(pElem + 4 + bitmap_index * sizeof(ACE_UINT32), pad_size);
        bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        
        data_size      -= (elem_size + pad_size);
        iter_size      += (elem_size + pad_size);
        pElem          += (elem_size + pad_size);
        m_wElemListLen += (elem_size + pad_size);
    }

    return CM_OK;
}


// Logic of StreamTo is similar to StreamFrom.
CmResult COFP13HelloMsg::StreamTo(ACE_OutputCDR &os)
{
    bool       bGood = false;
    ACE_UINT16 i     = 0;
    CmResult   lRet  = CM_ERROR_FAILURE;
    
    lRet = COFPMessage::StreamTo(os);
    if (CM_FAILED(lRet))
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::StreamTo, COFPMessage::StreamTo fail\n")));
        return lRet;
    }

    if (m_wLength < COFPMessage::GetStreamLen())
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::StreamTo(): m_wLength(%u) is less than OFP_MESSAGE_HDR_SIZE(%u).\n"),
                   m_wLength,
                   COFPMessage::GetStreamLen()));
        return CM_ERROR_FAILURE;
    }

    ACE_UINT16             data_size = m_wElemListLen; //m_wLength-OPENFLOW_MESSAGE_HDR_SIZE;
    ACE_UINT16             iter_size = 0;
    const ACE_CDR::Octet  *pElem     = m_pElemList;

    while (iter_size < m_wElemListLen)
    {
        ACE_UINT16 elem_type = GetHelloElemType(pElem);
        ACE_UINT16 elem_size = GetHelloElemLen(pElem);  // elem size, excluding paddings
        
        os<<elem_type;
        os<<elem_size;
        bGood = os.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13HelloMsg::StreamTo(): encode hello elem %u\n"), i++));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("elem_type=%u, elem_size=%u\n"), elem_type, elem_size));

        if (elem_size < 4)
        {
            ACE_ERROR((LM_ERROR, ACE_TEXT("COFP13HelloMsg::StreamTo(): elem_size(%u) error, less than 4."), elem_size));
            return CM_ERROR_FAILURE;
        }

        if ((elem_size + 7)/8*8 > data_size)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::StreamTo(): (elem_size + 7)/8*8=%u, greater than data_size(%u)\n"), 
                       (elem_size + 7)/8*8, 
                       data_size));
            return CM_ERROR_FAILURE;
        }

        if (elem_type != OFPHET_VERSIONBITMAP)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::StreamTo(): elem_type(%u) isnot OFPHET_VERSIONBITMAP(%u).\n"),
                       elem_type,
                       OFPHET_VERSIONBITMAP));
            return CM_ERROR_FAILURE;
        }

        ACE_UINT16 bitmap_size      = elem_size-4;
        CM_ASSERT_RETURN((bitmap_size/4*4 == bitmap_size), CM_ERROR_INVALID_ARG);
        
        ACE_UINT16 iter_bitmap_size = 0;
        ACE_UINT16 bitmap_index     = 0;

        while (iter_bitmap_size < bitmap_size)//(bitmap_size > 0)
        {
            ACE_UINT32 bitmap = GetVerBMHelloElemBitmap(pElem, bitmap_index);
            os<<bitmap;
            bGood = os.good_bit();
            CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13HelloMsg::StreamTo, bitmap[%u]=%u\n"), bitmap_index++, bitmap));

            //bitmap_size -= sizeof(bitmap);
            iter_bitmap_size += sizeof(bitmap);
        }

        // deal with paddings
        ACE_UINT16 pad_length = (elem_size + 7)/8*8 - elem_size;
        bGood = os.write_octet_array(pElem + 4 + sizeof(ACE_UINT32) * bitmap_index, pad_length);
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);

        iter_size += (elem_size + pad_length);
        data_size -= (elem_size + pad_length);
        pElem     += (elem_size + pad_length);
    }

    return CM_OK;
}


void COFP13HelloMsg::Dump() const
{
    ACE_UINT16 i = 0;

    COFPMessage::Dump();

    if (m_wLength < COFPMessage::GetStreamLen())
    {
        ACE_ERROR((LM_ERROR, 
                   ACE_TEXT("COFP13HelloMsg::Dump(): m_wLength(%u) is less than OFP_MESSAGE_HDR_SIZE(%u).\n"),
                   m_wLength,
                   COFPMessage::GetStreamLen()));
        return;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("COFP13HelloMsg::Dump(): \n")));

    ACE_UINT16       data_size = m_wElemListLen; //m_wLength-OPENFLOW_MESSAGE_HDR_SIZE;
    ACE_UINT16       iter_size = 0;
    const ACE_CDR::Octet  *pElem     = m_pElemList;

    while (iter_size < m_wElemListLen)
    {
        ACE_UINT16 elem_type             = GetHelloElemType(pElem);
        ACE_UINT16 elem_size_withoutPads = GetHelloElemLen(pElem);
        
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("dump hello elem %u\n"), i++));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("elem_type=%u, elem_size_withoutPads=%u\n"), elem_type, elem_size_withoutPads));

        if (elem_size_withoutPads < 4)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::Dump(): elem_size_withoutPads(%u) error, less than 4.\n"),
                       elem_size_withoutPads));
            return;
        }

        ACE_UINT16 elem_size_withPads = (elem_size_withoutPads+7)/8*8;

        if (elem_size_withPads > data_size)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::Dump(): (elem_size_withoutPads+7)/8*8=%u, greater than data_size(%u)\n"),
                       (elem_size_withoutPads+7)/8*8,
                       data_size));
            return;
        }

        if (elem_type != OFPHET_VERSIONBITMAP)
        {
            ACE_ERROR((LM_ERROR, 
                       ACE_TEXT("COFP13HelloMsg::Dump(): elem_type(%u) isnot OFPHET_VERSIONBITMAP(%u).\n"),
                       elem_type,
                       OFPHET_VERSIONBITMAP));
            return;
        }

        ACE_UINT16 bitmap_size  = elem_size_withoutPads-4;
        CM_ASSERT_RETURN_VOID((bitmap_size/4*4 == bitmap_size));
        
        ACE_UINT16 iter_bitmap_size = 0;
        ACE_UINT16 bitmap_index     = 0;

        while (iter_bitmap_size < bitmap_size)
        {
            ACE_UINT32 bitmap = GetVerBMHelloElemBitmap(pElem, bitmap_index);
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("bitmap[%u]=%u\n"), bitmap_index++, bitmap));

            //bitmap_size -= sizeof(bitmap);
            iter_bitmap_size += sizeof(bitmap);
        }

        data_size -= elem_size_withPads;
        iter_size += elem_size_withPads;
        pElem     += elem_size_withPads;
    }

    return;    
}

