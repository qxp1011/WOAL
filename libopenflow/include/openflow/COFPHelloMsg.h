/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPHelloMsg.h
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
#ifndef COFPHELLOMSG_H
#define COFPHELLOMSG_H

#include "COFPMessage.h"
#include "OFPMessageUtils.h"

class COFP10HelloMsg : public COFPMessage
{
public:
    COFP10HelloMsg(ACE_CDR::Octet abVersion = OFP10_VERSION)
        : COFPMessage(OFP10::OFPT_HELLO, abVersion)
    {
    }
    virtual ~COFP10HelloMsg(){}
};



// length of simple version bitmap hello elem, which contains only one bitmap
#define OFPMSG_SIMPLE_VERBITMAP_HELLOELEM_SIZE  (2*sizeof(ACE_UINT16)+sizeof(ACE_UINT32))

/* Before operating the COpenFlowMessageHello obj, you should specify its 
length, using COpenFlowMessageHello(elemLength), to allocate 
    memory. Otherwise, there would be errors. */
class COFP13HelloMsg : public COFPMessage
{
public:
    /* Hello elements types. */
    enum ofp_hello_elem_type {
        OFPHET_VERSIONBITMAP = 1, /* Bitmap of version supported. */
    };

    /* Version bitmap Hello Element */
    struct ofp_hello_elem_versionbitmap {
        ACE_UINT16 type;   /* OFPHET_VERSIONBITMAP. */
        ACE_UINT16 length; /* Length in bytes of this element. */
        /* Followed by:
               * - Exactly (length - 4) bytes containing the bitmaps, then
               * - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
               * bytes of all-zero bytes */
        ACE_UINT32 bitmaps[0]; /* List of bitmaps - supported versions */
    };

    // awElemListLen is used only to specify the size of memory allocated. 
    // If you use SetVerBMHelloElemBitmap() to set multiple elements, you SHOULD call SetHelloElemListLen()
    // to specify the size of used memory, which is showed in test_COpenFlowHelloWithMultiElems().
    // However, there is no need to call SetHelloElemListLen() after you call SetSimpleVerBMHelloElem(), which
    // is showed in test_COpenFlowSimpleVerBMHello().
    COFP13HelloMsg(
        ACE_UINT16  awElemListLen = 0,
        ACE_CDR::Octet   *apElemList    = NULL,        // Hello elements with the format: {(struct ofp_hello_elem_header) + data}
        // args for OFP header
        ACE_UINT32  adwxid        = 0,
        ACE_CDR::Octet    abVersion     = OFP13_VERSION);        
    virtual ~COFP13HelloMsg();

    // Get the the address of the first hello element in the list.
    inline const ACE_CDR::Octet * GetFirstHelloElem() const
    {
        return m_pElemList;
    }
    // Get the length in bytes of the hello element list.
    inline const ACE_UINT16 GetHelloElemListLength() const
    {
        return m_wElemListLen;
    }
    /* Common header for all Hello Elements */
    //struct ofp_hello_elem_header {
    //uint16_t type; /* One of OFPHET_*. */
    //uint16_t length; /* Length in bytes of this element. */
    //};
    // apHelloElem in the following funcs is the addr of the hello elem header.
    ACE_UINT16 GetHelloElemType(const ACE_CDR::Octet *apHelloElem) const;
    
    ACE_UINT16 GetHelloElemLen(const ACE_CDR::Octet *apHelloElem) const;
    // Get the specified bitmap in the Version Bitmap Hello Elem.
    // Notice: you must ensure the length is right.
    // ofp versions 0 to 31 are encoded in the first bitmap(awIndex = 0), ofp versions 32 to 63 are encoded in the second 
    // bitmap(awIndex = 1) and so on.
    // eg: you can use GetVerBMHelloElemBitmap(apElem, 0) to get the first bitmap, which encodes ofp versions 0 to 31.
    // args in:
    // @apVerBMHelloElem:  the addr of the Version Bitmap Hello Elem
    // @awIndex:                index of the bitmap, starting from 0
    ACE_UINT32 GetVerBMHelloElemBitmap(const ACE_CDR::Octet *apVerBMHelloElem, ACE_UINT16 awIndex) const;
    
    CmResult SetHelloElemList(const ACE_CDR::Octet *apSrcElem, ACE_UINT16 awElemListLen);
    // Set the length of actually used list.
    inline void SetHelloElemListLen(ACE_UINT16 awElemListLen);
    
    CmResult SetHelloElemType(ACE_CDR::Octet *apHelloElem, ACE_UINT16 awType);
    CmResult SetHelloElemLen(ACE_CDR::Octet *apHelloElem, ACE_UINT16 awLen);
    
    // Set the specified bitmap in the Version Bitmap Hello Elem.
    // Notice: you must ensure the length is right.
    // ofp versions 0 to 31 are encoded in the first bitmap(awIndex = 0), ofp versions 32 to 63 are encoded in the second 
    // bitmap(awIndex = 1) and so on.
    CmResult SetVerBMHelloElemBitmap(ACE_CDR::Octet *apVerBMHelloElem, ACE_UINT16 awIndex, ACE_UINT32 anBitmap);
    
    /* Set simple version bitmap hello elem, which contains only one bitmap. */
    CmResult SetSimpleVerBMHelloElem(ACE_UINT32 anBitmap);

    friend CmResult COFPMsgMemoryMgr::Copy(
        std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize,
                const ACE_CDR::Octet *apSrc, ACE_UINT16 awSrcSize);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return m_wElemListLen + COFPMessage::GetStreamLen();
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    /* Hello element list */
    //struct ofp_hello_elem_header elements[0];
    static std::allocator<ACE_CDR::Octet> m_alloc;        // Handler for allocating/deallocating memory
    ACE_CDR::Octet                       *m_pElemList;    // Hello elements with the format: {(struct ofp_hello_elem_header) + data}
    ACE_UINT16                      m_wElemListLen;      // length of used memory
    ACE_UINT16                      m_wElemListMemSize;  // size of allocated memory
};

// Set the length of actually used list.
inline void COFP13HelloMsg::SetHelloElemListLen(ACE_UINT16 awElemListLen)
{
    m_wElemListLen = awElemListLen;
}

#endif

