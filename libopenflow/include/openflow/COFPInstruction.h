/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPInstruction.h
 * Description:      OpenFlow instruction header file
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
#ifndef COFP_INSTRUCTION_H
#define COFP_INSTRUCTION_H

#include "IOFPMsgParser.h"
#include "COFPAction.h"
#include "ace/CDR_Stream.h"

enum ofp_instruction_type
{
    OFPIT_GOTO_TABLE     = 1, /* Setup the next table in the lookup pipeline */
    OFPIT_WRITE_METADATA = 2, /* Setup the metadata field for use later in pipeline */
    OFPIT_WRITE_ACTIONS  = 3, /* Write the action(s) onto the datapath action set */
    OFPIT_APPLY_ACTIONS  = 4, /* Applies the action(s) immediately */
    OFPIT_CLEAR_ACTIONS  = 5, /* Clears all actions from the datapath action set */
    OFPIT_METER          = 6, /* Apply meter (rate limiter) */
    OFPIT_EXPERIMENTER   = 0xFFFF /* Experimenter instruction */
};

#define OFP13_INSTRUCTION_HEADLEN (4)

class COFP13Instruction : public IOFPMsgParser
{
public:
    COFP13Instruction(ACE_UINT16 awType = 0, ACE_UINT16 awLen = 0);
    virtual ~COFP13Instruction()    {};

    inline ACE_UINT16 GetInstructType()                             const;
    inline ACE_UINT16 GetInstructLen()                              const;
    inline void   SetInstructType(ACE_UINT16 awType);
    inline void   SetInstructLen(ACE_UINT16 awLen);    

public:    
    static CmResult DecodeInstruction(ACE_InputCDR &is, COFP13Instruction * &aInstru);
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;    

protected:
    ACE_UINT16 m_wType;  /* Instruction type */
    ACE_UINT16 m_wLen;   /* Length of this struct in bytes. */
    
    /* The length includes the header and any padding used to make the instruction 64-bit aligned.
       NB: The length of an instruction *must* always be a multiple of eight. */
};

ACE_UINT16 COFP13Instruction::GetInstructType()                     const
{
    return m_wType;
}

ACE_UINT16 COFP13Instruction::GetInstructLen()                      const
{
    return m_wLen;
}

void COFP13Instruction::SetInstructType(ACE_UINT16 awType)
{
    m_wType = awType;
}

void COFP13Instruction::SetInstructLen(ACE_UINT16 awLen)
{
    m_wLen = awLen;
}

inline ACE_UINT16 COFP13Instruction::GetStreamLen() const
{
    return (sizeof(m_wType) + sizeof(m_wLen));
}

inline ACE_UINT16 COFP13Instruction::GetEncodeSpace() const
{
    return GetStreamLen();
}

class COFP13InstructGotoTable : public COFP13Instruction
{
public:
    COFP13InstructGotoTable(ACE_CDR::Octet abTableId = 0);
    virtual ~COFP13InstructGotoTable()  {};

    inline ACE_CDR::Octet GetTableId()                                    const;
    inline void SetTableId(ACE_CDR::Octet abTableId);
    
public:    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    ACE_CDR::Octet m_bTableId;
};

ACE_CDR::Octet COFP13InstructGotoTable::GetTableId()                      const
{
    return m_bTableId;
}

void COFP13InstructGotoTable::SetTableId(ACE_CDR::Octet abTableId)
{
    m_bTableId = abTableId;
}

inline ACE_UINT16 COFP13InstructGotoTable::GetStreamLen()           const
{
    return (COFP13Instruction::GetStreamLen() + sizeof(m_bTableId) + 3 * sizeof(ACE_CDR::Octet));
}

inline ACE_UINT16 COFP13InstructGotoTable::GetEncodeSpace()         const
{
    return GetStreamLen();
}

class COFP13InstructWriteMetadata : public COFP13Instruction
{
public:
    COFP13InstructWriteMetadata(ACE_UINT64 alMetadata = 0, ACE_UINT64 alMetaDataMask = 0);
    virtual ~COFP13InstructWriteMetadata()    {};

    inline ACE_UINT64 GetMetadata()                                 const;
    inline ACE_UINT64 GetMetadataMask()                             const;
    inline void   SetMetadata(ACE_UINT64 alData);
    inline void   SetMetadataMask(ACE_UINT64 alMask);
    
public:    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    
private:
    ACE_UINT64 m_lMetadata;
    ACE_UINT64 m_lMetaDataMask;
};

ACE_UINT64 COFP13InstructWriteMetadata::GetMetadata()                 const
{
    return m_lMetadata;
}

ACE_UINT64 COFP13InstructWriteMetadata::GetMetadataMask()             const
{
    return m_lMetaDataMask;
}

void COFP13InstructWriteMetadata::SetMetadata(ACE_UINT64 alData)
{
    m_lMetadata = alData;
}

void COFP13InstructWriteMetadata::SetMetadataMask(ACE_UINT64 alMask)
{
    m_lMetaDataMask = alMask;
}

inline ACE_UINT16 COFP13InstructWriteMetadata::GetStreamLen()         const
{
    return (COFP13Instruction::GetStreamLen() + 4*sizeof(ACE_CDR::Octet) + sizeof(m_lMetadata) + sizeof(m_lMetaDataMask));
}

inline ACE_UINT16 COFP13InstructWriteMetadata::GetEncodeSpace() const
{
    return GetStreamLen();
}

class COFP13Action;

/* The OFPIT_WRITE_ACTIONS, OFPIT_APPLY_ACTIONS, and OFPIT_CLEAR_ACTIONS instructions use the
   following structure and fields: */
class COFP13InstructAction : public COFP13Instruction
{
public:
    COFP13InstructAction(ACE_UINT16 awType);
    virtual ~COFP13InstructAction();

    CmResult AddActionList(std::list<COFP13Action *> &list);
    CmResult AppendAction(COFP13Action *action);
    CmResult GetActionList(std::list<COFP13Action *> &list);
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    
private:
    std::list<COFP13Action *> m_action_list;
};

class COFP13InstructionMeter : public COFP13Instruction
{
public:
    COFP13InstructionMeter(ACE_UINT32 anMeterId = 0);
    virtual ~COFP13InstructionMeter()   {};

    inline ACE_UINT32 GetMeterId()                                  const;
    inline void   SetMeterId(ACE_UINT32 adwId);
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    
private:
    ACE_UINT32 m_dwMeterId;
};

ACE_UINT32 COFP13InstructionMeter::GetMeterId()                     const
{
    return m_dwMeterId;
}

void COFP13InstructionMeter::SetMeterId(ACE_UINT32 adwId)
{
    m_dwMeterId = adwId;
}

inline ACE_UINT16 COFP13InstructionMeter::GetStreamLen()            const
{
    return (COFP13Instruction::GetStreamLen() + sizeof(m_dwMeterId));
}

inline ACE_UINT16 COFP13InstructionMeter::GetEncodeSpace()          const
{
    return GetStreamLen();
}

#endif  // COFP_INSTRUCTION_H


