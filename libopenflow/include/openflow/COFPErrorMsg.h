/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPErrorMsg.h
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
#ifndef COFPERRORMSG_H
#define COFPERRORMSG_H

#include "COFPMessage.h"
#include "OFPMessageUtils.h"

class COFP10ErrorMsg : public COFPMessage
{
public:
    /* Values for 'type' in ofp_error_message. These values are immutable: they
        * will not change in future versions of the protocol (although new values may
        * be added). */
    enum ofp_error_type {
        OFPET_HELLO_FAILED, /* Hello protocol failed. */
        OFPET_BAD_REQUEST, /* Request was not understood. */
        OFPET_BAD_ACTION, /* Error in action description. */
        OFPET_FLOW_MOD_FAILED, /* Problem modifying flow entry. */
        OFPET_PORT_MOD_FAILED, /* Port mod request failed. */
        OFPET_QUEUE_OP_FAILED /* Queue operation failed. */
    };

    /* ofp_error_msg 'code' values for OFPET_HELLO_FAILED. 'data' contains an
        * ASCII text string that may give failure details. */
    enum ofp_hello_failed_code {
        OFPHFC_INCOMPATIBLE, /* No compatible version. */
        OFPHFC_EPERM /* Permissions error. */
    };

    /* ofp_error_msg 'code' values for OFPET_BAD_REQUEST. 'data' contains at least
        * the first 64 bytes of the failed request. */
    enum ofp_bad_request_code {
        OFPBRC_BAD_VERSION, /* ofp_header.version not supported. */
        OFPBRC_BAD_TYPE, /* ofp_header.type not supported. */
        OFPBRC_BAD_STAT, /* ofp_stats_request.type not supported. */
        OFPBRC_BAD_VENDOR, /* Vendor not supported (in ofp_vendor_header or ofp_stats_request or ofp_stats_reply). */
        OFPBRC_BAD_SUBTYPE, /* Vendor subtype not supported. */
        OFPBRC_EPERM, /* Permissions error. */
        OFPBRC_BAD_LEN, /* Wrong request length for type. */
        OFPBRC_BUFFER_EMPTY, /* Specified buffer has already been used. */
        OFPBRC_BUFFER_UNKNOWN /* Specified buffer does not exist. */
    };

    /* ofp_error_msg 'code' values for OFPET_BAD_ACTION. 'data' contains at least
        * the first 64 bytes of the failed request. */
    enum ofp_bad_action_code {
        OFPBAC_BAD_TYPE, /* Unknown action type. */
        OFPBAC_BAD_LEN, /* Length problem in actions. */
        OFPBAC_BAD_VENDOR, /* Unknown vendor id specified. */
        OFPBAC_BAD_VENDOR_TYPE, /* Unknown action type for vendor id. */
        OFPBAC_BAD_OUT_PORT, /* Problem validating output action. */
        OFPBAC_BAD_ARGUMENT, /* Bad action argument. */
        OFPBAC_EPERM, /* Permissions error. */
        OFPBAC_TOO_MANY, /* Can't handle this many actions. */
        OFPBAC_BAD_QUEUE /* Problem validating output queue. */
    };

    /* ofp_error_msg 'code' values for OFPET_FLOW_MOD_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_flow_mod_failed_code {
        OFPFMFC_ALL_TABLES_FULL, /* Flow not added because of full tables. */
        OFPFMFC_OVERLAP, /* Attempted to add overlapping flow with CHECK_OVERLAP flag set. */
        OFPFMFC_EPERM, /* Permissions error. */
        OFPFMFC_BAD_EMERG_TIMEOUT, /* Flow not added because of non-zero idle/hard timeout. */
        OFPFMFC_BAD_COMMAND, /* Unknown command. */
        OFPFMFC_UNSUPPORTED /* Unsupported action list - cannot process in the order specified. */
    };

    /* ofp_error_msg 'code' values for OFPET_PORT_MOD_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_port_mod_failed_code {
        OFPPMFC_BAD_PORT, /* Specified port does not exist. */
        OFPPMFC_BAD_HW_ADDR, /* Specified hardware address is wrong. */
    };

    /* ofp_error msg 'code' values for OFPET_QUEUE_OP_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request */
    enum ofp_queue_op_failed_code {
        OFPQOFC_BAD_PORT, /* Invalid port (or port does not exist). */
        OFPQOFC_BAD_QUEUE, /* Queue does not exist. */
        OFPQOFC_EPERM /* Permissions error. */
    };
    
public:
    COFP10ErrorMsg(ACE_CDR::Octet abVersion = OFP10_VERSION, ACE_UINT16 awSrcLen = 0, 
                           const ACE_CDR::Octet *apSrc = NULL, ACE_UINT16 awErrType = 0, ACE_UINT16 awErrCode = 0);
    virtual ~COFP10ErrorMsg();

    inline ACE_UINT16 GetErrType()              const;
    inline ACE_UINT16 GetErrCode()              const;
    inline const ACE_CDR::Octet *GetErrData()         const;
    inline ACE_UINT16 GetErrDataLen()           const;
    inline void SetErrType(ACE_UINT16 awType);
    inline void SetErrCode(ACE_UINT16 awCode);
    CmResult SetErrData(const ACE_CDR::Octet *apData, ACE_UINT16 awDataLen);
    friend CmResult COFPMsgMemoryMgr::Copy(
        std::allocator<ACE_CDR::Octet> &aAlloc, ACE_CDR::Octet *&apDes, ACE_UINT16 &awDesMemSize, ACE_UINT16 &awDesUsedMemSize,
        const ACE_CDR::Octet *apSrc, ACE_UINT16 awSrcSize);
    
public:
    // for interface IOFPMsgParser
    virtual CmResult     StreamFrom(ACE_InputCDR &is);
    virtual CmResult     StreamTo(ACE_OutputCDR& os);
    virtual ACE_UINT16   GetStreamLen() const
    {
        return (m_wDataLen + 4 + COFPMessage::GetStreamLen());
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void         Dump() const;

private:
    ACE_UINT16 m_wErrType;
    ACE_UINT16 m_wErrCode;
    static std::allocator<ACE_CDR::Octet> m_alloc;
    ACE_UINT16 m_wDataLen;      // used memory size
    ACE_UINT16 m_wDataMemSize;  // allocated memory size
    ACE_CDR::Octet *m_pData;
};

inline ACE_UINT16 COFP10ErrorMsg::GetErrType() const
{
    return m_wErrType;
}


inline ACE_UINT16 COFP10ErrorMsg::GetErrCode() const
{
    return m_wErrCode;
}


inline const ACE_CDR::Octet *COFP10ErrorMsg::GetErrData() const
{
    return m_pData;
}

inline ACE_UINT16 COFP10ErrorMsg::GetErrDataLen() const
{
    return m_wDataLen;
}


inline void COFP10ErrorMsg::SetErrType(ACE_UINT16 awType)
{
    m_wErrType = awType;
}


inline void COFP10ErrorMsg::SetErrCode(ACE_UINT16 awCode)
{
    m_wErrCode = awCode;
}



class COFP13ErrorMsg : public COFP10ErrorMsg
{
public:
        /* Values for 'type' in ofp_error_message. These values are immutable: they
              * will not change in future versions of the protocol (although new values may
              * be added). */
    enum ofp_error_type
    {
        OFPET_HELLO_FAILED = 0, /* Hello protocol failed. */
        OFPET_BAD_REQUEST = 1, /* Request was not understood. */
        OFPET_BAD_ACTION = 2, /* Error in action description. */
        OFPET_BAD_INSTRUCTION = 3, /* Error in instruction list. */
        OFPET_BAD_MATCH = 4, /* Error in match. */
        OFPET_FLOW_MOD_FAILED = 5, /* Problem modifying flow entry. */
        OFPET_GROUP_MOD_FAILED = 6, /* Problem modifying group entry. */
        OFPET_PORT_MOD_FAILED = 7, /* Port mod request failed. */
        OFPET_TABLE_MOD_FAILED = 8, /* Table mod request failed. */
        OFPET_QUEUE_OP_FAILED = 9, /* Queue operation failed. */
        OFPET_SWITCH_CONFIG_FAILED = 10, /* Switch config request failed. */
        OFPET_ROLE_REQUEST_FAILED = 11, /* Controller Role request failed. */
        OFPET_METER_MOD_FAILED = 12, /* Error in meter. */
        OFPET_TABLE_FEATURES_FAILED = 13, /* Setting table features failed. */
        OFPET_EXPERIMENTER = 0xffff /* Experimenter error messages. */
    };

    /* ofp_error_msg 'code' values for OFPET_HELLO_FAILED. 'data' contains an
        * ASCII text string that may give failure details. */
    enum ofp_hello_failed_code
    {
        OFPHFC_INCOMPATIBLE = 0, /* No compatible version. */
        OFPHFC_EPERM = 1, /* Permissions error. */
    };

    /* ofp_error_msg 'code' values for OFPET_BAD_REQUEST. 'data' contains at least
        * the first 64 bytes of the failed request. */
    enum ofp_bad_request_code
    {
        OFPBRC_BAD_VERSION = 0, /* ofp_header.version not supported. */
        OFPBRC_BAD_TYPE = 1, /* ofp_header.type not supported. */
        OFPBRC_BAD_MULTIPART = 2, /* ofp_multipart_request.type not supported. */
        OFPBRC_BAD_EXPERIMENTER = 3, /* Experimenter id not supported (in ofp_experimenter_header or
                                                            * ofp_multipart_request or ofp_multipart_reply). */
        OFPBRC_BAD_EXP_TYPE = 4, /* Experimenter type not supported. */
        OFPBRC_EPERM = 5, /* Permissions error. */
        OFPBRC_BAD_LEN = 6, /* Wrong request length for type. */
        OFPBRC_BUFFER_EMPTY = 7, /* Specified buffer has already been used. */
        OFPBRC_BUFFER_UNKNOWN = 8, /* Specified buffer does not exist. */
        OFPBRC_BAD_TABLE_ID = 9, /* Specified table-id invalid or does not exist. */
        OFPBRC_IS_SLAVE = 10, /* Denied because controller is slave. */
        OFPBRC_BAD_PORT = 11, /* Invalid port. */
        OFPBRC_BAD_PACKET = 12, /* Invalid packet in packet-out. */
        OFPBRC_MULTIPART_BUFFER_OVERFLOW = 13, /* ofp_multipart_request overflowed the assigned buffer. */
    };

    /* ofp_error_msg 'code' values for OFPET_BAD_ACTION. 'data' contains at least
        * the first 64 bytes of the failed request. */
    enum ofp_bad_action_code
    {
        OFPBAC_BAD_TYPE = 0, /* Unknown action type. */
        OFPBAC_BAD_LEN = 1, /* Length problem in actions. */
        OFPBAC_BAD_EXPERIMENTER = 2, /* Unknown experimenter id specified. */
        OFPBAC_BAD_EXP_TYPE = 3, /* Unknown action for experimenter id. */
        OFPBAC_BAD_OUT_PORT = 4, /* Problem validating output port. */
        OFPBAC_BAD_ARGUMENT = 5, /* Bad action argument. */
        OFPBAC_EPERM = 6, /* Permissions error. */
        OFPBAC_TOO_MANY = 7, /* Can't handle this many actions. */
        OFPBAC_BAD_QUEUE = 8, /* Problem validating output queue. */
        OFPBAC_BAD_OUT_GROUP = 9, /* Invalid group id in forward action. */
        OFPBAC_MATCH_INCONSISTENT = 10, /* Action can't apply for this match, or Set-Field missing prerequisite. */
        OFPBAC_UNSUPPORTED_ORDER = 11, /* Action order is unsupported for the action list in an Apply-Actions instruction */
        OFPBAC_BAD_TAG = 12, /* Actions uses an unsupported tag/encap. */
        OFPBAC_BAD_SET_TYPE = 13, /* Unsupported type in SET_FIELD action. */
        OFPBAC_BAD_SET_LEN = 14, /* Length problem in SET_FIELD action. */
        OFPBAC_BAD_SET_ARGUMENT = 15, /* Bad argument in SET_FIELD action. */
    };

    /* ofp_error_msg 'code' values for OFPET_BAD_INSTRUCTION. 'data' contains at 
        * least the first 64 bytes of the failed request. */
    enum ofp_bad_instruction_code
    {
        OFPBIC_UNKNOWN_INST = 0, /* Unknown instruction. */
        OFPBIC_UNSUP_INST = 1, /* Switch or table does not support the instruction. */
        OFPBIC_BAD_TABLE_ID = 2, /* Invalid Table-ID specified. */
        OFPBIC_UNSUP_METADATA = 3, /* Metadata value unsupported by datapath. */
        OFPBIC_UNSUP_METADATA_MASK = 4, /* Metadata mask value unsupported by datapath. */
        OFPBIC_BAD_EXPERIMENTER = 5, /* Unknown experimenter id specified. */
        OFPBIC_BAD_EXP_TYPE = 6, /* Unknown instruction for experimenter id. */
        OFPBIC_BAD_LEN = 7, /* Length problem in instructions. */
        OFPBIC_EPERM = 8, /* Permissions error. */
    };

    /* ofp_error_msg 'code' values for OFPET_BAD_MATCH. 'data' contains at least
        * the first 64 bytes of the failed request. */
    enum ofp_bad_match_code
    {
        OFPBMC_BAD_TYPE = 0, /* Unsupported match type specified by the match */
        OFPBMC_BAD_LEN = 1, /* Length problem in match. */
        OFPBMC_BAD_TAG = 2, /* Match uses an unsupported tag/encap. */
        OFPBMC_BAD_DL_ADDR_MASK = 3, /* Unsupported datalink addr mask - switch
                                                            does not support arbitrary datalink address mask. */
        OFPBMC_BAD_NW_ADDR_MASK = 4, /* Unsupported network addr mask - switch
                                                            does not support arbitrary network address mask. */
        OFPBMC_BAD_WILDCARDS = 5, /* Unsupported combination of fields masked or omitted in the match. */
        OFPBMC_BAD_FIELD = 6, /* Unsupported field type in the match. */
        OFPBMC_BAD_VALUE = 7, /* Unsupported value in a match field. */
        OFPBMC_BAD_MASK = 8, /* Unsupported mask specified in the match,field is not dl-address or nw-address. */
        OFPBMC_BAD_PREREQ = 9, /* A prerequisite was not met. */
        OFPBMC_DUP_FIELD = 10, /* A field type was duplicated. */
        OFPBMC_EPERM = 11, /* Permissions error. */
    };

    /* ofp_error_msg 'code' values for OFPET_FLOW_MOD_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_flow_mod_failed_code
    {
        OFPFMFC_UNKNOWN = 0, /* Unspecified error. */
        OFPFMFC_TABLE_FULL = 1, /* Flow not added because table was full. */
        OFPFMFC_BAD_TABLE_ID = 2, /* Table does not exist */
        OFPFMFC_OVERLAP = 3, /* Attempted to add overlapping flow with CHECK_OVERLAP flag set. */
        OFPFMFC_EPERM = 4, /* Permissions error. */
        OFPFMFC_BAD_TIMEOUT = 5, /* Flow not added because of unsupported idle/hard timeout. */
        OFPFMFC_BAD_COMMAND = 6, /* Unsupported or unknown command. */
        OFPFMFC_BAD_FLAGS = 7, /* Unsupported or unknown flags. */
    };

    /* ofp_error_msg 'code' values for OFPET_GROUP_MOD_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_group_mod_failed_code
    {
        OFPGMFC_GROUP_EXISTS = 0, /* Group not added because a group ADD attempted to
                                                          replace an already-present group. */
        OFPGMFC_INVALID_GROUP = 1, /* Group not added because Group specified is invalid. */
        OFPGMFC_WEIGHT_UNSUPPORTED = 2, /* Switch does not support unequal load sharing with select groups. */
        OFPGMFC_OUT_OF_GROUPS = 3, /* The group table is full. */
        OFPGMFC_OUT_OF_BUCKETS = 4, /* The maximum number of action buckets for a group has been exceeded. */
        OFPGMFC_CHAINING_UNSUPPORTED = 5, /* Switch does not support groups that forward to groups. */
        OFPGMFC_WATCH_UNSUPPORTED = 6, /* This group cannot watch the watch_port or watch_group specified. */
        OFPGMFC_LOOP = 7, /* Group entry would cause a loop. */
        OFPGMFC_UNKNOWN_GROUP = 8, /* Group not modified because a group MODIFY attempted to modify a non-existent group. */
        OFPGMFC_CHAINED_GROUP = 9, /* Group not deleted because another group is forwarding to it. */
        OFPGMFC_BAD_TYPE = 10, /* Unsupported or unknown group type. */
        OFPGMFC_BAD_COMMAND = 11, /* Unsupported or unknown command. */
        OFPGMFC_BAD_BUCKET = 12, /* Error in bucket. */
        OFPGMFC_BAD_WATCH = 13, /* Error in watch port/group. */
        OFPGMFC_EPERM = 14, /* Permissions error. */
    };
    
    /* ofp_error_msg 'code' values for OFPET_PORT_MOD_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_port_mod_failed_code
    {
        OFPPMFC_BAD_PORT = 0, /* Specified port number does not exist. */
        OFPPMFC_BAD_HW_ADDR = 1, /* Specified hardware address does not match the port number. */
        OFPPMFC_BAD_CONFIG = 2, /* Specified config is invalid. */
        OFPPMFC_BAD_ADVERTISE = 3, /* Specified advertise is invalid. */
        OFPPMFC_EPERM = 4, /* Permissions error. */
    };

    /* ofp_error_msg 'code' values for OFPET_TABLE_MOD_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_table_mod_failed_code
    {
        OFPTMFC_BAD_TABLE = 0, /* Specified table does not exist. */
        OFPTMFC_BAD_CONFIG = 1, /* Specified config is invalid. */
        OFPTMFC_EPERM = 2, /* Permissions error. */
    };

    /* ofp_error msg 'code' values for OFPET_QUEUE_OP_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request */
    enum ofp_queue_op_failed_code
    {
        OFPQOFC_BAD_PORT = 0, /* Invalid port (or port does not exist). */
        OFPQOFC_BAD_QUEUE = 1, /* Queue does not exist. */
        OFPQOFC_EPERM = 2, /* Permissions error. */
    };

    /* ofp_error_msg 'code' values for OFPET_SWITCH_CONFIG_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_switch_config_failed_code
    {
        OFPSCFC_BAD_FLAGS = 0, /* Specified flags is invalid. */
        OFPSCFC_BAD_LEN = 1, /* Specified len is invalid. */
        OFPSCFC_EPERM = 2, /* Permissions error. */
    };
    
    /* ofp_error_msg 'code' values for OFPET_ROLE_REQUEST_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_role_request_failed_code
    {
        OFPRRFC_STALE = 0, /* Stale Message: old generation_id. */
        OFPRRFC_UNSUP = 1, /* Controller role change unsupported. */
        OFPRRFC_BAD_ROLE = 2, /* Invalid role. */
    };

    /* ofp_error_msg 'code' values for OFPET_METER_MOD_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_meter_mod_failed_code
    {
        OFPMMFC_UNKNOWN = 0, /* Unspecified error. */
        OFPMMFC_METER_EXISTS = 1, /* Meter not added because a Meter ADD attempted to replace an existing Meter. */
        OFPMMFC_INVALID_METER = 2, /* Meter not added because Meter specified is invalid. */
        OFPMMFC_UNKNOWN_METER = 3, /* Meter not modified because a Meter
                                                            MODIFY attempted to modify a non-existent Meter. */
        OFPMMFC_BAD_COMMAND = 4, /* Unsupported or unknown command. */
        OFPMMFC_BAD_FLAGS = 5, /* Flag configuration unsupported. */
        OFPMMFC_BAD_RATE = 6, /* Rate unsupported. */
        OFPMMFC_BAD_BURST = 7, /* Burst size unsupported. */
        OFPMMFC_BAD_BAND = 8, /* Band unsupported. */
        OFPMMFC_BAD_BAND_VALUE = 9, /* Band value unsupported. */
        OFPMMFC_OUT_OF_METERS = 10, /* No more meters available. */
        OFPMMFC_OUT_OF_BANDS = 11, /* The maximum number of properties for a meter has been exceeded. */
    };
    
    /* ofp_error_msg 'code' values for OFPET_TABLE_FEATURES_FAILED. 'data' contains
        * at least the first 64 bytes of the failed request. */
    enum ofp_table_features_failed_code
    {
        OFPTFFC_BAD_TABLE = 0, /* Specified table does not exist. */
        OFPTFFC_BAD_METADATA = 1, /* Invalid metadata mask. */
        OFPTFFC_BAD_TYPE = 2, /* Unknown property type. */
        OFPTFFC_BAD_LEN = 3, /* Length problem in properties. */
        OFPTFFC_BAD_ARGUMENT = 4, /* Unsupported property value. */
        OFPTFFC_EPERM = 5, /* Permissions error. */
    };

public:
    COFP13ErrorMsg(ACE_CDR::Octet abVersion = OFP13_VERSION, ACE_UINT16 awSrcLen = 0, 
                           const ACE_CDR::Octet *apSrc = NULL, ACE_UINT16 awErrType = 0, ACE_UINT16 awErrCode = 0);
    virtual ~COFP13ErrorMsg(){}
};


#endif

