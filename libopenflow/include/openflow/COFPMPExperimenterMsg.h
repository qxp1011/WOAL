/***********************************************************************
 * Copyright (C) 2013, Nanjing Weifeng Networks, Inc.
 *
 * File Name:        COFPMPExperimenterMsg.h
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
#ifndef COFPMPEXPERIMENTERMSG_H
#define COFPMPEXPERIMENTERMSG_H

#include "COFPMultipartMsg.h"

template<class ADD_ADD_TYPE>
class COFP13MPExpRequestAndReplyMsg
    : public COFP13MultipartRequestMsg
{
public:
    /* Body for ofp_multipart_request/reply of type OFPMP_EXPERIMENTER. */
    struct ofp_experimenter_multipart_header {
        ACE_UINT32 experimenter; /* Experimenter ID which takes the same form*/
        ACE_UINT32 exp_type; /* Experimenter defined. */
        /* Experimenter-defined arbitrary additional data. */
    };
    static_assert((sizeof(struct ofp_experimenter_multipart_header) == 8), "must be 8");

    COFP13MPExpRequestAndReplyMsg()
        :COFP13MultipartRequestMsg(OFPMP_EXPERIMENTER)
    {
    }
    virtual ~COFP13MPExpRequestAndReplyMsg(){}

    // for interface IOFPMsgParser
    virtual CmResult StreamFrom(ACE_InputCDR &is)
    {
        if (COFP13MultipartMsg::StreamFrom(is) != CM_OK)
        {
            return CM_ERROR_FAILURE;
        }
        
        is>>m_headerdata.experimenter;
        is>>m_headerdata.exp_type;
        //is.Read(&m_addtional_data,sizeof(m_addtional_data));
        is.read_char_array(reinterpret_cast<char *>(&m_addtional_data), sizeof(m_addtional_data));

        bool bGood = is.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        
        return CM_OK;

    }
    virtual CmResult StreamTo(ACE_OutputCDR &os)
    {
        if (COFP13MultipartMsg::StreamTo(os) != CM_OK)
        {
            return CM_ERROR_FAILURE;
        }
        
        os<<m_headerdata.experimenter;
        os<<m_headerdata.exp_type;
        os.write_char_array(reinterpret_cast<const char *>(&m_addtional_data), sizeof(m_addtional_data));

        bool bGood = os.good_bit();
        CM_ASSERT_RETURN(bGood, CM_ERROR_FAILURE);
        
        return CM_OK;
    }
    virtual ACE_UINT16 GetStreamLen() const
    {
        return COFP13MultipartMsg::GetStreamLen()+sizeof(m_headerdata)+sizeof(m_addtional_data);
    }
    virtual ACE_UINT16   GetEncodeSpace() const
    {
        return GetStreamLen();
    }
    virtual void     Dump() const
    {
    }

    CmResult GetMeterFeatures(ofp_experimenter_multipart_header &headdata)
    {
        ::memcpy(&headdata, m_headerdata, sizeof(m_headerdata));
    }
    
    void SetMeterFeatures(ofp_experimenter_multipart_header &headdata)
    {
        ::memcpy(&m_headerdata, headdata, sizeof(headdata));
    }

    CmResult GetAddtionalData(ADD_ADD_TYPE &adddata)
    {
        ::memcpy(&adddata, m_addtional_data, sizeof(m_addtional_data));
    }
    
    void SetAddtionalData(ADD_ADD_TYPE &adddata)
    {
        ::memcpy(&m_addtional_data, adddata, sizeof(adddata));
    }

private:
    ofp_experimenter_multipart_header m_headerdata;
    ADD_ADD_TYPE m_addtional_data;
};

#endif

