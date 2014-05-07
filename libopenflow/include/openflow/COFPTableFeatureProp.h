/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPTableFeatureProp.h
 * Description:      OpenFlow table feature properties header file
 * Others:
 * Version:          V1.0
 * Author:           mazhh@wfnex.com
 * Date:             2013-07-16
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/
#ifndef COFP_TABLE_FEATURE_PROP_H
#define COFP_TABLE_FEATURE_PROP_H

#include "COFPMessage.h"
#include "IOFPMsgParser.h"
#include "COFPInstruction.h"

class COFP13TableFeatureProp : public IOFPMsgParser
{
public:
    /* Table Feature property types.
     * Low order bit cleared indicates a property for a regular Flow Entry.
     * Low order bit set indicates a property for the Table-Miss Flow Entry.
     */
    enum ofp_table_feature_prop_type {
        OFPTFPT_INSTRUCTIONS        = 0, /* Instructions property. */
        OFPTFPT_INSTRUCTIONS_MISS   = 1, /* Instructions for table-miss. */
        OFPTFPT_NEXT_TABLES         = 2, /* Next Table property. */
        OFPTFPT_NEXT_TABLES_MISS    = 3, /* Next Table for table-miss. */
        OFPTFPT_WRITE_ACTIONS       = 4, /* Write Actions property. */
        OFPTFPT_WRITE_ACTIONS_MISS  = 5, /* Write Actions for table-miss. */
        OFPTFPT_APPLY_ACTIONS       = 6, /* Apply Actions property. */
        OFPTFPT_APPLY_ACTIONS_MISS  = 7, /* Apply Actions for table-miss. */
        OFPTFPT_MATCH               = 8, /* Match property. */
        OFPTFPT_WILDCARDS           = 10, /* Wildcards property. */
        OFPTFPT_WRITE_SETFIELD      = 12, /* Write Set-Field property. */
        OFPTFPT_WRITE_SETFIELD_MISS = 13, /* Write Set-Field for table-miss. */
        OFPTFPT_APPLY_SETFIELD      = 14, /* Apply Set-Field property. */
        OFPTFPT_APPLY_SETFIELD_MISS = 15, /* Apply Set-Field for table-miss. */
        OFPTFPT_EXPERIMENTER        = 0xFFFE, /* Experimenter property. */
        OFPTFPT_EXPERIMENTER_MISS   = 0xFFFF, /* Experimenter for table-miss. */
    };
    /* Common header for all Table Feature Properties */
    struct ofp_table_feature_prop_header {
        ACE_UINT16 type;   /* One of OFPTFPT_*. */
        ACE_UINT16 length; /* Length in bytes of this property. */
    };
    static_assert((sizeof(struct ofp_table_feature_prop_header) == 4), "must be 4");

    COFP13TableFeatureProp(ACE_UINT16 aType = 0)
    {
        ::memset(&m_header, 0, sizeof(m_header));
        m_header.type = aType;
        m_header.length = sizeof(m_header);
    }
    virtual ~COFP13TableFeatureProp()   {};

    inline ACE_UINT16 GetType()                   const { return m_header.type;}
    inline ACE_UINT16 GetLength()                 const { return m_header.length;}
    inline void     SetType(ACE_UINT16 awType)          { m_header.type = awType; }
    
    static CmResult DecodeMPTFP(ACE_InputCDR &is, COFP13TableFeatureProp * &prop);
    void AddPropData(IOFPMsgParser *apropData);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    
protected:
    ofp_table_feature_prop_header m_header;
};

/* The OFPTFPT_INSTRUCTIONS and OFPTFPT_INSTRUCTIONS_MISS properties use the following structure
   and fields: */
class COFP13TableFeaturePropInstruct : public COFP13TableFeatureProp
{
public:
    COFP13TableFeaturePropInstruct(ACE_UINT16 aType)
        :   COFP13TableFeatureProp(0)
    {
        CM_ASSERT_RETURN_VOID((OFPTFPT_INSTRUCTIONS == aType) || (OFPTFPT_INSTRUCTIONS_MISS == aType));
        SetType(aType);
    }
    virtual ~COFP13TableFeaturePropInstruct();

    void AddInstruction(COFP13Instruction *inst); 
    void GetInstructionList(std::list<COFP13Instruction *> &list);
    void SetInstructionList(std::list<COFP13Instruction *> &list);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;
    
private:
    std::list<COFP13Instruction *> m_instruction_list;
};

/* The OFPTFPT_NEXT_TABLES and OFPTFPT_NEXT_TABLES_MISS properties use the following structure and
   fields: */
class COFP13TableFeaturePropNextTables : public COFP13TableFeatureProp
{
public:
    COFP13TableFeaturePropNextTables(ACE_UINT16 aType)
        :COFP13TableFeatureProp(0)
    {
        CM_ASSERT_RETURN_VOID((OFPTFPT_NEXT_TABLES == aType) || (OFPTFPT_NEXT_TABLES_MISS == aType));
        SetType(aType);
    }
    virtual ~COFP13TableFeaturePropNextTables() {}

    void AddNextTableID(ACE_CDR::Octet id);
    void GetNextTableIDList(std::list<ACE_CDR::Octet> &list);
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    std::list<ACE_CDR::Octet> m_next_table_id_list;
};

/* The OFPTFPT_WRITE_ACTIONS, OFPTFPT_WRITE_ACTIONS_MISS, OFPTFPT_APPLY_ACTIONS and
   OFPTFPT_APPLY_ACTIONS_MISS properties use the following structure and fields:    */
class COFP13TableFeaturePropActions : public COFP13TableFeatureProp
{
public:
    COFP13TableFeaturePropActions(ACE_UINT16 aType)
        :   COFP13TableFeatureProp(0)
    {
        CM_ASSERT_RETURN_VOID((OFPTFPT_WRITE_ACTIONS == aType) || (OFPTFPT_WRITE_ACTIONS_MISS == aType)
                               || (OFPTFPT_APPLY_ACTIONS == aType) || (OFPTFPT_APPLY_ACTIONS_MISS == aType));
        SetType(aType);
    }
    virtual ~COFP13TableFeaturePropActions();
    
    void AddAction(COFP13Action *action);
    void SetActionList(std::list<COFP13Action *> &list);
    void GetActionList(std::list<COFP13Action *> &list);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    std::list<COFP13Action *> m_action_list;
};

/* The OFPTFPT_MATCH, OFPTFPT_WILDCARDS, OFPTFPT_WRITE_SETFIELD, OFPTFPT_WRITE_SETFIELD_MISS, 
   OFPTFPT_APPLY_SETFIELD and OFPTFPT_APPLY_SETFIELD_MISS properties use the following structure and fields: */
class COFP13TableFeaturePropOXM : public COFP13TableFeatureProp
{
public:
    COFP13TableFeaturePropOXM(ACE_UINT16 aType)
        : COFP13TableFeatureProp(0)
    {
        CM_ASSERT_RETURN_VOID((OFPTFPT_MATCH == aType) || (OFPTFPT_WILDCARDS == aType) 
                               || (OFPTFPT_WRITE_SETFIELD == aType) || (OFPTFPT_WRITE_SETFIELD_MISS == aType)
                               || (OFPTFPT_APPLY_SETFIELD == aType) || (OFPTFPT_APPLY_SETFIELD_MISS == aType));
        SetType(aType);
    }
    virtual ~COFP13TableFeaturePropOXM();

    void AddOXM(COFP13OxmTlv *oxm);
    void SetOXMList(std::list<COFP13OxmTlv *> &list);
    void GetOXMList(std::list<COFP13OxmTlv *> &list);

    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    std::list<COFP13OxmTlv *> m_oxm_list;
};


/* The OFPTFPT_EXPERIMENTER and OFPTFPT_EXPERIMENTER_MISS properties uses the following structure
   and fieds: */
class COFP13TableFeaturePropExperimenter : public COFP13TableFeatureProp
{
public:
    COFP13TableFeaturePropExperimenter(ACE_UINT16 aType,
                                       ACE_UINT32 aexperimenter = 0,
                                       ACE_UINT32 aexp_type     = 0)
        :   COFP13TableFeatureProp(0)
        ,   m_experimenter(aexperimenter)
        ,   m_exp_type(aexp_type)
    {
        CM_ASSERT_RETURN_VOID((OFPTFPT_EXPERIMENTER == aType) || (OFPTFPT_EXPERIMENTER_MISS == aType));
        SetType(aType);
    }
    virtual ~COFP13TableFeaturePropExperimenter(){}

    void AddExpData(ACE_UINT32 data);
    void GetExpDataList(std::list<ACE_UINT32> &list);
    
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR &os);
    virtual ACE_UINT16   GetStreamLen()                             const;
    virtual ACE_UINT16   GetEncodeSpace()                           const;
    virtual void         Dump()                                     const;

private:
    ACE_UINT32 m_experimenter;
    ACE_UINT32 m_exp_type; /* Experimenter defined. */
    std::list<ACE_UINT32> m_experimenter_data_list;
};


#endif

