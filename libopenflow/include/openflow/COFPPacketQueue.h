/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPPacketQueue.h
 * Description:      OpenFlow packet queue header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-11
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFP_PACKET_QUEUE_H
#define COFP_PACKET_QUEUE_H

#include "IOFPMsgParser.h"
#include "OFPMessageUtils.h"


class COFPQueueProp : public IOFPMsgParser
{
public:
    enum ofp_queue_properties
    {
        OFPQT_NONE = 0,     /* No property defined for queue (default). for version1.0*/
        OFPQT_MIN_RATE = 1, /* Minimum datarate guaranteed. */
        OFPQT_MAX_RATE = 2, /* Maximum datarate. */
        OFPQT_EXPERIMENTER = 0xffff /* Experimenter defined property. */
    };
    
    COFPQueueProp(ACE_UINT16 awProp, ACE_UINT16 awPropLen);
    virtual ~COFPQueueProp()  {}

    inline ACE_UINT16 GetQueueProperty()                            const;
    inline ACE_UINT16 GetPropertyLen()                              const;
    inline void   SetQueueProperty(ACE_UINT16 awProp);

    static CmResult DecodeQueueProp(ACE_InputCDR &is, COFPQueueProp * &aProp);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

protected:
    ACE_UINT16 m_wProperty;/* One of OFPQT_. */
    ACE_UINT16 m_wPropLen; /* Length of property, including this header. */
    //uint8_t pad[4]; /* 64-bit alignemnt. */
};

ACE_UINT16 COFPQueueProp::GetQueueProperty()                      const
{
    return m_wProperty;
}

ACE_UINT16 COFPQueueProp::GetPropertyLen()                        const
{
    return m_wPropLen;
}

void COFPQueueProp::SetQueueProperty(ACE_UINT16 awProp)
{
    m_wProperty = awProp;
}

// for IOFPMsgParser
inline ACE_UINT16 COFPQueueProp::GetStreamLen()                   const
{
    return (sizeof(m_wProperty) + sizeof(m_wPropLen) + 4 * sizeof(ACE_CDR::Octet));
}

inline ACE_UINT16 COFPQueueProp::GetEncodeSpace()                 const
{
    return GetStreamLen();
}

/* If rate is not configured, it is set to OFPQ_RATE_UNCFG, which is 0xffff. */
#define OFPQ_RATE_UNCFG     (0xffff)
#define OFPQ_MIN_RATE_UNCFG OFPQ_RATE_UNCFG
#define OFPQ_MAX_RATE_UNCFG OFPQ_RATE_UNCFG

/* The minimum-rate and maximum-rate queue share the following class. */
class COFPQueuePropRate : public COFPQueueProp
{
public:
    COFPQueuePropRate(ACE_UINT16 awProp, ACE_UINT16 awRate = OFPQ_RATE_UNCFG);
    virtual ~COFPQueuePropRate()  {}

    inline ACE_UINT16 GetRate()                                     const;
    inline void   SetRate(ACE_UINT16 awRate);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    ACE_UINT16 m_wRate;
};

ACE_UINT16 COFPQueuePropRate::GetRate()                           const
{
    return m_wRate;
}

void COFPQueuePropRate::SetRate(ACE_UINT16 awRate)
{
    m_wRate = awRate;
}

inline ACE_UINT16 COFPQueuePropRate::GetStreamLen()               const
{
    return (COFPQueueProp::GetStreamLen() + sizeof(m_wRate) + 6 * sizeof(ACE_CDR::Octet));
}

inline ACE_UINT16 COFPQueuePropRate::GetEncodeSpace()             const
{
    return GetStreamLen();
}

class COFP13QueuePropExperimenter : public COFPQueueProp
{
public:
    COFP13QueuePropExperimenter(ACE_UINT32 anExperimenterId = 0);
    ~COFP13QueuePropExperimenter();

    inline ACE_UINT32   GetExperimenter()                             const;
    inline ACE_UINT16   GetUsedDataSize()                             const;
    inline void     GetUsedData(ACE_CDR::Octet *&abData, ACE_UINT16 &awDataSize) const;
    inline void     SetExperimenter(ACE_UINT32 anExperimenter);
    CmResult SetData(ACE_CDR::Octet *abData, ACE_UINT16 awDataSize);

    friend CmResult COFPMsgMemoryMgr::Copy(std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, 
                                           ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize,
                                           const ACE_CDR::Octet *apSrc, ACE_UINT16 awSrcSize);
    friend CmResult COFPMsgMemoryMgr::DataStreamFrom(std::allocator<ACE_CDR::Octet> &aAlloc, 
                                                     ACE_InputCDR &aIs, 
                                                     ACE_UINT16 awDataSize,
                                                     ACE_CDR::Octet * &apDes, 
                                                     ACE_UINT16 &awDesMemSize, 
                                                     ACE_UINT16 &awDesUsedMemSize);
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    ACE_UINT32 m_experimenter;
    static std::allocator<ACE_CDR::Octet> m_alloc;
    ACE_UINT16 m_dataTotalSize; // total size of allocated memory
    ACE_CDR::Octet   *m_data;
};

ACE_UINT32 COFP13QueuePropExperimenter::GetExperimenter()           const
{
    return m_experimenter;
}

ACE_UINT16 COFP13QueuePropExperimenter::GetUsedDataSize()           const
{
    return (m_wPropLen - 16);
}

void COFP13QueuePropExperimenter::GetUsedData(ACE_CDR::Octet *&abData, ACE_UINT16 &awDataSize) const
{
    abData     = m_data;
    awDataSize = GetUsedDataSize();
}

void COFP13QueuePropExperimenter::SetExperimenter(ACE_UINT32 anExperimenter)
{
    m_experimenter = anExperimenter;
}

inline ACE_UINT16 COFP13QueuePropExperimenter::GetStreamLen()       const
{
    return m_wPropLen;
}

inline ACE_UINT16 COFP13QueuePropExperimenter::GetEncodeSpace()     const
{
    ACE_UINT16 streamLen = GetStreamLen();
    return (streamLen + 7) / 8 * 8;
}


class COFP10PacketQueue : public IOFPMsgParser
{
public:
    COFP10PacketQueue(ACE_UINT32 adwQueueId = 0)
    {
        ::memset(&m_tMember, 0, sizeof m_tMember);
        m_tMember.queue_id = adwQueueId;
        m_tMember.len = sizeof m_tMember;
    }
    virtual ~COFP10PacketQueue();

    ACE_UINT32 GetQueueId() const
    {
        return m_tMember.queue_id;
    }
    ACE_UINT16 GetQueueLen() const
    {
        return m_tMember.len;
    }

    void SetQueueId(ACE_UINT32 adwId)
    {
        m_tMember.queue_id = adwId;
    }

    CmResult AddPropList(std::list<COFPQueueProp *> &list);
    CmResult AppendProp(COFPQueueProp *Prop);
    CmResult GetPropList(std::list<COFPQueueProp *> &list);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return m_tMember.len;
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;
    
protected:
    struct member
    {
        ACE_UINT32 queue_id; /* id for the specific queue. */
        ACE_UINT16 len; /* Length in bytes of this queue desc. */
        ACE_CDR::Octet pad[2]; /* 64-bit alignment. */
    };

private:
    struct member m_tMember;
    std::list<COFPQueueProp *> m_prop_list;
};


class COFP13PacketQueue : public IOFPMsgParser
{
public:
    COFP13PacketQueue(ACE_UINT32 adwQueueId = 0, ACE_UINT32 adwPort = 0);
    virtual ~COFP13PacketQueue();

    inline ACE_UINT32 GetQueueId()                                  const;
    inline ACE_UINT32 GetQueuePort()                                const;
    inline ACE_UINT16 GetQueueLen()                                 const;
    inline void   SetQueueId(ACE_UINT32 adwQueueId);
    inline void   SetQueuePort(ACE_UINT32 adwPortNo);

    CmResult AddPropList(std::list<COFPQueueProp *> &list);
    CmResult AppendProp(COFPQueueProp *Prop);
    CmResult GetPropList(std::list<COFPQueueProp *> &list);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    ACE_UINT32 m_dwQueueId;
    ACE_UINT32 m_dwPort;
    ACE_UINT16 m_wQueueLen;
    std::list<COFPQueueProp *> m_prop_list;
};

ACE_UINT32 COFP13PacketQueue::GetQueueId()                          const
{
    return m_dwQueueId;
}

ACE_UINT32 COFP13PacketQueue::GetQueuePort()                        const
{
    return m_dwPort;
}

ACE_UINT16 COFP13PacketQueue::GetQueueLen()                         const
{
    return m_wQueueLen;
}

void COFP13PacketQueue::SetQueueId(ACE_UINT32 adwQueueId)
{
    m_dwQueueId = adwQueueId;
}

void COFP13PacketQueue::SetQueuePort(ACE_UINT32 adwPortNo)
{
    m_dwPort = adwPortNo;
}


#endif  // COFP_PACKET_QUEUE_H


