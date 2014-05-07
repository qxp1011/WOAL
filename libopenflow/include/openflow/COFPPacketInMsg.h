/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPacketInMsg.h
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
#ifndef COFPPACKETINMSG_H
#define COFPPACKETINMSG_H

#include "COFPMessage.h"
#include "COFPMatch.h"

class COFP10PacketInMsg : public COFPMessage
{
public:
    enum ofp_packet_in_reason {
        OFPR_NO_MATCH, /* No matching flow. */
        OFPR_ACTION    /* Action explicitly output to controller. */
    };
    
public:
    COFP10PacketInMsg(ACE_CDR::Octet abVersion = OFP10_VERSION)
        : COFPMessage(OFP10::OFPT_PACKET_IN, abVersion)
        , m_wDataLen(0)
        , m_PacketData(NULL)
    {
        ::memset(&m_tMember, 0, sizeof(m_tMember));
    }
    virtual ~COFP10PacketInMsg();

    ACE_UINT32 GetBufferId() const
    {
        return m_tMember.buffer_id;
    }
    ACE_UINT16 GetTotalLen() const
    {
        return m_tMember.total_len;
    }
    ACE_UINT16 GetInPort() const
    {
        return m_tMember.in_port;
    }
    ACE_CDR::Octet GetReason() const
    {
        return m_tMember.reason;
    }

    void SetBufferId(ACE_UINT32 adwId)
    {
        m_tMember.buffer_id = adwId;
    }
    void SetTotalLen(ACE_UINT16 awLen)
    {
        m_tMember.total_len = awLen;
    }
    void SetInPort(ACE_UINT16 awPortNo)
    {
        m_tMember.in_port = awPortNo;
    }
    void SetReason(ACE_CDR::Octet abReason)
    {
        m_tMember.reason = abReason;
    }
    
    //const BYTE * GetEtherFrame() const;
    ACE_CDR::Octet *GetPacketData() const;
    //CmResult AppendPacketData(BYTE *apData, WORD16 awDataLen);
    //CmResult AppendPacketData(CCmMessageBlock &aData);
    ACE_UINT16 GetPacketDataLength() const;

public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return COFPMessage::GetStreamLen() + sizeof(m_tMember) + m_wDataLen;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:
    struct member
    {
        ACE_UINT32 buffer_id; /* ID assigned by datapath. */
        ACE_UINT16 total_len; /* Full length of frame. */
        ACE_UINT16 in_port; /* Port on which frame was received. */
        ACE_CDR::Octet reason; /* Reason packet is being sent (one of OFPR_*) */
        ACE_CDR::Octet pad;
    };

private:
    struct member m_tMember;
    ACE_UINT16 m_wDataLen;
    //CCmMessageBlock *m_PacketData;
    ACE_CDR::Octet *m_PacketData;
};



/* Packet received on port (datapath -> controller). */
class  COFP13PacketInMsg : public COFPMessage
{
public:
    /* Why is this packet being sent to the controller? */
    enum ofp_packet_in_reason {
        OFPR_NO_MATCH    = 0, /* No matching flow (table-miss flow entry). */
        OFPR_ACTION      = 1, /* Action explicitly output to controller. */
        OFPR_INVALID_TTL = 2, /* Packet has invalid TTL */
    };

    COFP13PacketInMsg();
    COFP13PacketInMsg(
        ACE_CDR::Octet   abReason,                            /* Reason packet is being sent (one of OFPR_*) */
        ACE_UINT32 anBufferId            = 0,           /* ID assigned by datapath. */
        ACE_CDR::Octet   abTableId             = 0,           /* ID of the table that was looked up */
        ACE_UINT64 anCookie              = 0,           /* Cookie of the flow entry that was looked up. */
        ACE_CDR::Octet  *apEtherFrame          = NULL,
        ACE_UINT32 anEtherFrameLen       = 0,
        ACE_UINT16 anTotalLen            = 0,           /* Full length of frame. */
        /* args for OFP Match */
        ACE_UINT16      awMatchType      = OFPMT_OXM,
        /* args for OFP Header */
        ACE_UINT32  anOFPHeadXId         = 0,
        ACE_CDR::Octet    abOFPHeadVer         = OFP13_VERSION);
    virtual ~COFP13PacketInMsg();
    
    inline ACE_UINT32 GetBufferId() const
    {
        return m_nBufferId;
    }

    inline ACE_UINT16 GetTotalLen() const
    {
        return m_wTotalLen;
    }

    inline ACE_CDR::Octet GetReason() const
    {
        return m_bReason;
    }

    inline ACE_CDR::Octet GetTableId() const
    {
        return m_bTableId;
    }

    inline ACE_UINT64 GetCookie() const
    {
        return m_nCookie;
    }

    inline const COFP13Match *GetMatch() const
    {
        return &m_tMatch;
    }

    inline const ACE_CDR::Octet * GetEtherFrame() const
    {
        return m_pEtherFrame;
    }

    inline const ACE_UINT32 GetEtherFrameLen() const
    {
        return m_nEtherFrameLen;
    }

    inline const ACE_UINT32 GetEtherMemSize() const
    {
        return m_nEtherMemSize;
    }
    
    inline void SetBufferId(ACE_UINT32 anBufferId)
    {
        m_nBufferId = anBufferId;
    }

    inline void SetTotalLen(ACE_UINT16 awTotalLen)
    {
        m_wTotalLen = awTotalLen;
    }

    inline void SetReason(ACE_CDR::Octet abReason)
    {
        m_bReason = abReason;
    }

    inline void SetTableId(ACE_CDR::Octet abTableId)
    {
        m_bTableId = abTableId;
    }

    inline void SetCookie(ACE_UINT64 anCookie)
    {
        m_nCookie = anCookie;
    }

    /* overload assignment operator
    inline void SetMatch(const COFP13Match &  anMatch)  // no inline for performance?
    {
        m_tMatch = anMatch;
    }
    */

    CmResult SetEtherFrame(const ACE_CDR::Octet * apEtherFrame, ACE_UINT32 anEtherFrameLen);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return (16 + m_tMatch.GetEncodeSpace() + 2 + m_nEtherFrameLen + COFPMessage::GetStreamLen());
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

protected:    
    ACE_UINT32 m_nBufferId; /* ID assigned by datapath. */
    ACE_UINT16 m_wTotalLen; /* Full length of frame. NB: This field is included in the packet-in struct. */
    ACE_CDR::Octet   m_bReason;   /* Reason packet is being sent (one of OFPR_*) */
    ACE_CDR::Octet   m_bTableId;  /* ID of the table that was looked up */
    ACE_UINT64 m_nCookie;   /* Cookie of the flow entry that was looked up. */
    COFP13Match m_tMatch; /* Packet metadata. Variable size. */
    /* Followed by:
        * - Exactly 2 all-zero padding bytes, then
        * - An Ethernet frame whose length is inferred from header.length.
        * The padding bytes preceding the Ethernet frame ensure that the IP
        * header (if any) following the Ethernet header is 32-bit aligned.
        */
    //uint8_t pad[2]; /* Align to 64 bit + 16 bit */
    //uint8_t data[0]; /* Ethernet frame */
    static std::allocator<ACE_CDR::Octet> m_alloc;  /* Handler for allocate/deallocate memory. */
    ACE_CDR::Octet  *m_pEtherFrame;     /* Ptr to ether frame */
    ACE_UINT32 m_nEtherFrameLen;  /* The length parsed from the ethernet frame, and it is the actual length of the frame. */
    ACE_UINT32 m_nEtherMemSize;   /* size of memory allocated for the Ether frame */
};

#endif

