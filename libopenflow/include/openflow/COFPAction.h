/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPAction.h
 * Description:      OpenFlow action header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-12
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFP_ACTION_H
#define COFP_ACTION_H

#include "IOFPMsgParser.h"
#include "COFPMatch.h"


#define OFP13_ACTION_HEADER_LEN  (4)

class COFP13Action : public IOFPMsgParser
{
public:
    enum ofp_action_type
    {
        OFPAT_OUTPUT       = 0,  /* Output to switch port. */
        OFPAT_COPY_TTL_OUT = 11, /* Copy TTL "outwards" -from next-to-outermost  to outermost */
        OFPAT_COPY_TTL_IN  = 12, /* Copy TTL "inwards" -from outermost to next-to-outermost */
        OFPAT_SET_MPLS_TTL = 15, /* MPLS TTL */
        OFPAT_DEC_MPLS_TTL = 16, /* Decrement MPLS TTL */
        OFPAT_PUSH_VLAN    = 17, /* Push a new VLAN tag */
        OFPAT_POP_VLAN     = 18, /* Pop the outer VLAN tag */
        OFPAT_PUSH_MPLS    = 19, /* Push a new MPLS tag */
        OFPAT_POP_MPLS     = 20, /* Pop the outer MPLS tag */
        OFPAT_SET_QUEUE    = 21, /* Set queue id when outputting to a port */
        OFPAT_GROUP        = 22, /* Apply group. */
        OFPAT_SET_NW_TTL   = 23, /* IP TTL. */
        OFPAT_DEC_NW_TTL   = 24, /* Decrement IP TTL. */
        OFPAT_SET_FIELD    = 25, /* Set a header field using OXM TLV format. */
        OFPAT_PUSH_PBB     = 26, /* Push a new PBB service tag (I-TAG) */
        OFPAT_POP_PBB      = 27, /* Pop the outer PBB service tag (I-TAG) */
        OFPAT_EXPERIMENTER = 0xffff
    };
    
public:
    COFP13Action(ACE_UINT16 awType, ACE_UINT16 awLen = OFP13_ACTION_HEADER_LEN); 
    virtual ~COFP13Action() {};

    inline ACE_UINT16 GetActionType()                               const;
    inline ACE_UINT16 GetActionLen()                                const;
    inline void   SetActionType(ACE_UINT16 awType);
    inline void   SetActionLen(ACE_UINT16 awLen);
    
    static CmResult DecodeAction(ACE_InputCDR &is, COFP13Action * &apAction);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
        
protected:
    ACE_UINT16 m_wType; /* One of OFPAT_*. */
    ACE_UINT16 m_wLen;  /* Length of action, including this header. This is the length of action,
                       including any padding to make it 64-bit aligned. */
};


inline ACE_UINT16 COFP13Action::GetActionType()                     const
{
    return m_wType;
}


inline ACE_UINT16 COFP13Action::GetActionLen()                      const
{
    return m_wLen;
}


inline void COFP13Action::SetActionType(ACE_UINT16 awType)
{
    m_wType = awType;
}


inline void COFP13Action::SetActionLen(ACE_UINT16 awLen)
{
    m_wLen = awLen;
}


inline ACE_UINT16 COFP13Action::GetStreamLen()                      const
{
    return (sizeof(m_wType) + sizeof(m_wLen));
}


inline ACE_UINT16 COFP13Action::GetEncodeSpace()                    const
{
    return GetStreamLen() + 4 * sizeof(ACE_CDR::Octet);  // or only GetStreamLen()?
}


#define OFP13_ACT_ONLYHEAD_LEN (8)

/* Action header that is common to all actions. The length includes the
* header and any padding used to make the action 64-bit aligned.
* NB: The length of an action *must* always be a multiple of eight. */
//struct ofp_action_header {
//  uint16_t type; /* One of OFPAT_*. */
//  uint16_t len;  /* Length of action, including this header. This is the length of action,
//                    including any padding to make it 64-bit aligned. */
//  uint8_t pad[4];
//};

// Class COFP13ActOnlyHeader deal with 4 BYTE paddings after class COFP13Action.
class COFP13ActOnlyHeader : public COFP13Action
{
public:
    COFP13ActOnlyHeader(ACE_UINT16 awActType);
    virtual ~COFP13ActOnlyHeader() {};

    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void     Dump()                                     const;
};


inline ACE_UINT16 COFP13ActOnlyHeader::GetStreamLen() const
{
    return OFP13_ACT_ONLYHEAD_LEN;
}


inline ACE_UINT16 COFP13ActOnlyHeader::GetEncodeSpace() const
{
    return GetStreamLen();
}


#define OFP13_ACT_OUTPUT_LEN  (16)

class COFP13ActOutput : public COFP13Action
{
public:
    enum ofp_controller_max_len {
        OFPCML_MAX       = 0xffe5, /* maximum max_len value which can be used to request a specific byte length. */
        OFPCML_NO_BUFFER = 0xffff  /* indicates that no buffering should be applied and the whole packet is to be
                                      sent to the controller. */
    };
    
    COFP13ActOutput(ACE_UINT32 anOutport = 0, ACE_UINT16 awMaxLen = 0);
    virtual ~COFP13ActOutput() {};

    inline ACE_UINT32 GetOutPort()                                  const;
    inline ACE_UINT16 GetMaxLen()                                   const;
    inline void   SetOutPort(ACE_UINT32 adwPortNo);
    inline void   SetMaxLen(ACE_UINT16 awLen);

protected:
    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void     Dump()                                     const;
    
private:
    ACE_UINT32 m_dwOutPort;    /* Output port. */
    ACE_UINT16 m_wMaxLen;      /* Max length to send to controller. */
};


inline ACE_UINT32 COFP13ActOutput::GetOutPort()                     const
{
    return m_dwOutPort;
}


inline ACE_UINT16 COFP13ActOutput::GetMaxLen()                      const
{
    return m_wMaxLen;
}


inline void COFP13ActOutput::SetOutPort(ACE_UINT32 adwPortNo)
{
    m_dwOutPort = adwPortNo;
}


inline void COFP13ActOutput::SetMaxLen(ACE_UINT16 awLen)
{
    m_wMaxLen = awLen;
}


inline ACE_UINT16 COFP13ActOutput::GetStreamLen() const
{
    return OFP13_ACT_OUTPUT_LEN;
}


inline ACE_UINT16 COFP13ActOutput::GetEncodeSpace() const
{
    return GetStreamLen();
}


#define OFP13_ACT_GROUP_LEN  (8)

class COFP13ActGroup : public COFP13Action
{
public:
    COFP13ActGroup(ACE_UINT32 anGroupId = 0);
    virtual ~COFP13ActGroup() {};

    inline ACE_UINT32 GetGroupId()                                  const;
    inline void   SetGroupId(ACE_UINT32 adwGroupId);

protected:    
    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void     Dump()                                     const;
    
private:
    ACE_UINT32 m_nGroupId;    /* Group identifier. */
};


inline ACE_UINT32 COFP13ActGroup::GetGroupId()                      const
{
    return m_nGroupId;
}


inline void COFP13ActGroup::SetGroupId(ACE_UINT32 adwGroupId)
{
    m_nGroupId = adwGroupId;
}


inline ACE_UINT16 COFP13ActGroup::GetStreamLen() const
{
    return OFP13_ACT_GROUP_LEN;
}


inline ACE_UINT16 COFP13ActGroup::GetEncodeSpace() const
{
    return GetStreamLen();
}


#define OFP13_ACT_SET_QUEUE_LEN  (8)

class COFP13ActSetQueue : public COFP13Action
{
public:
    COFP13ActSetQueue(ACE_UINT32 anQueueId = 0);
    virtual ~COFP13ActSetQueue() {};

    inline ACE_UINT32 GetQueueId()                                  const;
    inline void   SetQueueId(ACE_UINT32 adwQueueId);

protected:    
    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void     Dump()                                     const;
    
private:
    ACE_UINT32 m_dwQueueId;    /* Queue id for the packets. */
};


inline ACE_UINT32 COFP13ActSetQueue::GetQueueId()                   const
{
    return m_dwQueueId;
}


inline void COFP13ActSetQueue::SetQueueId(ACE_UINT32 adwQueueId)
{
    m_dwQueueId = adwQueueId;
}


inline ACE_UINT16 COFP13ActSetQueue::GetStreamLen()                 const
{
    return OFP13_ACT_SET_QUEUE_LEN;
}


inline ACE_UINT16 COFP13ActSetQueue::GetEncodeSpace()               const
{
    return GetStreamLen();
}


#define OFP13_ACT_SET_MPLS_TTL_LEN  (8)

class COFP13ActSetMplsTtl : public COFP13Action
{
public:

    COFP13ActSetMplsTtl(ACE_CDR::Octet abMplsTtl = 0);
    virtual ~COFP13ActSetMplsTtl() {};

    inline ACE_CDR::Octet GetMplsTtl()                                    const;
    inline void SetMplsTtl(ACE_CDR::Octet abTtl);

protected:    
    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void     Dump()                                     const;

private:
    ACE_CDR::Octet   m_bMplsTtl;     /* MPLS TTL */
};


inline ACE_CDR::Octet COFP13ActSetMplsTtl::GetMplsTtl() const
{
    return m_bMplsTtl;
}


inline void COFP13ActSetMplsTtl::SetMplsTtl(ACE_CDR::Octet abTtl)
{
    m_bMplsTtl = abTtl;
}


inline ACE_UINT16 COFP13ActSetMplsTtl::GetStreamLen()               const
{
    return OFP13_ACT_SET_MPLS_TTL_LEN;
}


inline ACE_UINT16 COFP13ActSetMplsTtl::GetEncodeSpace()             const
{
    return GetStreamLen();
}


/* A Decrement MPLS TTL action takes no arguments and consists only of a generic ofp_action_header.
   The action decrements the MPLS TTL. */
class COFP13ActDecreMplsTtl : public COFP13ActOnlyHeader
{
public:
    COFP13ActDecreMplsTtl();
    virtual ~COFP13ActDecreMplsTtl() {};
};


#define OFP13_ACT_SET_NW_TTL_LEN  (8)

class COFP13ActSetNwTtl : public COFP13Action
{
public:
    COFP13ActSetNwTtl(ACE_CDR::Octet abNwTtl = 0);
    virtual ~COFP13ActSetNwTtl()    {};

    ACE_CDR::Octet GetNwTtl()                                             const;
    void SetNwTtl(ACE_CDR::Octet abTtl);

    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is);
    virtual CmResult StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void     Dump()                                     const;

private:
    ACE_CDR::Octet   m_bNwTtl;       /* IP TTL */
};


inline ACE_CDR::Octet COFP13ActSetNwTtl::GetNwTtl() const
{
    return m_bNwTtl;
}


inline void COFP13ActSetNwTtl::SetNwTtl(ACE_CDR::Octet abTtl)
{
    m_bNwTtl = abTtl;
}


inline ACE_UINT16 COFP13ActSetNwTtl::GetStreamLen()                 const
{
    return OFP13_ACT_SET_NW_TTL_LEN;
}


inline ACE_UINT16 COFP13ActSetNwTtl::GetEncodeSpace()               const
{
    return GetStreamLen();
}


/* An Decrement IPv4 TTL action takes no arguments and consists only of a generic ofp_action_header.
   This action decrement the TTL in the IP header if one is present.*/
class COFP13ActDecreNwTtl : public COFP13ActOnlyHeader
{
public:
    COFP13ActDecreNwTtl();
    virtual ~COFP13ActDecreNwTtl() {};
};


/* A Copy TTL outwards action takes no arguments and consists only of a generic ofp_action_header. */
class COFP13ActCopyTtlOut : public COFP13ActOnlyHeader
{
public:
    COFP13ActCopyTtlOut();
    virtual ~COFP13ActCopyTtlOut() {};
};


/* A Copy TTL inwards action takes no arguments and consists only of a generic ofp_action_header. */
class COFP13ActCopyTtlIn : public COFP13ActOnlyHeader
{
public:
    COFP13ActCopyTtlIn();
    virtual ~COFP13ActCopyTtlIn() {};
};


#define OFP13_ACT_PUSH_LEN  (8)

/* A Push VLAN header action, Push MPLS header and Push PBB header action use the following
   structure and fields: */
class COFP13ActPush : public COFP13Action
{
public:
    COFP13ActPush(ACE_UINT16 awType, ACE_UINT16 awEtherType = 0);
    virtual ~COFP13ActPush(){};

    ACE_UINT16 GetEtherType()                                       const;
    void   SetEtherType(ACE_UINT16 awEtherType);

    // for interface IOFPMsgParser
    virtual CmResult    StreamFrom(ACE_InputCDR &is);
    virtual CmResult    StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16  GetStreamLen()                              const;
    virtual ACE_UINT16  GetEncodeSpace()                            const;
    virtual void        Dump()                                      const;

private:
    ACE_UINT16 m_wEtherType;   /* Ethertype */
};


inline ACE_UINT16 COFP13ActPush::GetEtherType()                     const
{
    return m_wEtherType;
}


inline void COFP13ActPush::SetEtherType(ACE_UINT16 awEtherType)
{
    m_wEtherType = awEtherType;
}


inline ACE_UINT16 COFP13ActPush::GetStreamLen()                     const
{
    return OFP13_ACT_PUSH_LEN;
}


inline ACE_UINT16 COFP13ActPush::GetEncodeSpace()                   const
{
    return GetStreamLen();
}


/* A Pop VLAN header action takes no arguments and consists only of a generic ofp_action_header. */
class COFP13ActPopVlan : public COFP13ActOnlyHeader
{
public:
    COFP13ActPopVlan();
    virtual ~COFP13ActPopVlan() {};
};


/* A Pop PBB header action takes no arguments and consists only of a generic ofp_action_header. */
class COFP13ActPopPbb : public COFP13ActOnlyHeader
{
public:
    COFP13ActPopPbb();
    virtual ~COFP13ActPopPbb() {};
};


#define OFP13_ACT_POP_MPLS_LEN  (8)

class COFP13ActPopMpls : public COFP13Action
{
public:
    COFP13ActPopMpls(ACE_UINT16 awEtherType = 0);
    virtual ~COFP13ActPopMpls() {};

    ACE_UINT16 GetEtherType()                                       const;
    void   SetEtherType(ACE_UINT16 awEtherType);

protected:    
    // for interface IOFPMsgParser
    virtual CmResult    StreamFrom(ACE_InputCDR &is);
    virtual CmResult    StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16  GetStreamLen()                              const;
    virtual ACE_UINT16  GetEncodeSpace()                            const;
    virtual void        Dump()                                      const;

private:
    ACE_UINT16 m_wEtherType;   /* Ethertype */
};


inline ACE_UINT16 COFP13ActPopMpls::GetEtherType()                  const
{
    return m_wEtherType;
}


inline void COFP13ActPopMpls::SetEtherType(ACE_UINT16 awEtherType)
{
    m_wEtherType = awEtherType;
}


inline ACE_UINT16 COFP13ActPopMpls::GetStreamLen()                  const
{
    return OFP13_ACT_POP_MPLS_LEN;
}


inline ACE_UINT16 COFP13ActPopMpls::GetEncodeSpace()                const
{
    return GetStreamLen();
}


// NB: the following is NOT implemented.
/* Set Field actions, A.2.5 Action Structures
   The match of the ow entry must contain the OXM prerequisite
   curresponding to the field to be set (see A.2.3.6), otherwise an error must be generated (see 6.4).*/

class COFP13ActSetField : public COFP13Action
{
public:
    // If you know the length of payload, you SHOULD pass it as an arguement of COFP13ActSetField().
    // Otherwise, if you call COFP13ActSetField() and then SetOxmTlv(m_oxmTlv), you SHOULD call SetActionLen()
    // to adjust the total length of action.
    COFP13ActSetField(
        ACE_CDR::Octet         abOxmPayloadLen = 0,
        const ACE_CDR::Octet  *apOxmPayload    = NULL,
        ACE_CDR::Octet         abOxmFiled   = 0,
        ACE_CDR::Octet         abOxmHasMask = 0,
        ACE_UINT16       awOxmClass   = COFP13Match::OFPXMC_OPENFLOW_BASIC);
    virtual ~COFP13ActSetField()    {};

    COFP13OxmTlv *GetOxmTlv();
    ACE_UINT16    CalcTotalPadLen() const;
    static bool   IsOxmTlvValid(const COFP13OxmTlv *apOxmTlv);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    
private:
    /* Action structure for OFPAT_SET_FIELD. */
    //struct ofp_action_set_field {
    //  uint16_t type; /* OFPAT_SET_FIELD. */
    //  uint16_t len; /* Length is padded to 64 bits. */
    /* Followed by:
    * - Exactly oxm_len bytes containing a single OXM TLV, then
    * - Exactly ((oxm_len + 4) + 7)/8*8 - (oxm_len + 4) (between 0 and 7)
    * bytes of all-zero bytes
    */
    //  uint8_t field[4]; /* OXM TLV - Make compiler happy */
    //};
    COFP13OxmTlv m_oxmTlv; /* OXM TLV */
};


inline COFP13OxmTlv * COFP13ActSetField::GetOxmTlv()
{
    return &m_oxmTlv;
}


inline ACE_UINT16 COFP13ActSetField::GetStreamLen() const
{
    //return OFP13_ACTION_HEADER_LEN + ((m_oxmTlv.GetOxmLen()+OFP13_MATCH_OXMTLV_HEADLEN)+7)/8*8 + 4;
    return OFP13_ACTION_HEADER_LEN + m_oxmTlv.GetStreamLen()+CalcTotalPadLen();
}


inline ACE_UINT16 COFP13ActSetField::GetEncodeSpace() const
{
    return GetStreamLen();
}


// TBD
// An Experimenter action uses the following structure and fields: 
/* Action header for OFPAT_EXPERIMENTER.
* The rest of the body is experimenter-defined. */
//struct ofp_action_experimenter_header {
//uint16_t type; /* OFPAT_EXPERIMENTER. */
//uint16_t len; /* Length is a multiple of 8. */
//uint32_t experimenter; /* Experimenter ID which takes the same form as in struct ofp_experimenter_header. */
//};
//OFP_ASSERT(sizeof(struct ofp_action_experimenter_header) == 8);

#endif  // COFP_ACTION_H

