/***********************************************************************
 * Copyright (C) 2013, Nanjing WFN Technology Co., Ltd 
 * Description: 
 * Others:
 * Version:          V1.0
 * Author:           Yi Jian
 * Date:             2013-09-12
 *
 * History 1:
 *     Date:          
 *     Version:       
 *     Author:       
 *     Modification: 
**********************************************************************/

#ifndef CMERROR_H
#define CMERROR_H

/*
 * To add error code to your module, you need to do the following:
 *
 * 1) Add a module offset code.  Add yours to the bottom of the list 
 *    right below this comment, adding 10000.
 *
 * 2) In your module, define a header file which uses addition.  An examples below:
 *
 *    #define CM_ERROR_MYMODULE_MYERROR1 (CM_ERROR_MODULE_MYMODULE + 1)
 *
 */

#define CM_ERROR_MODULE_BASE      10000
#define CM_ERROR_MODULE_NETWORK   20000
#define CM_ERROR_MODULE_DB        30000
#define CM_ERROR_MODULE_OPENFLOW_ENGINE 40000

#define CM_FAILED(rv) (rv != 0)
#define CM_SUCCEEDED(rv) (rv == 0)

/* Standard "it worked" return value */
#define CM_OK                              0

/* Returned when a function fails */
#define CM_ERROR_FAILURE                   (CmResult)(CM_ERROR_MODULE_BASE + 1)

/* Returned when an instance is not initialized */
#define CM_ERROR_NOT_INITIALIZED           (CmResult)(CM_ERROR_MODULE_BASE + 2)

/* Returned when an instance is already initialized */
#define CM_ERROR_ALREADY_INITIALIZED       (CmResult)(CM_ERROR_MODULE_BASE + 3)

/* Returned by a not implemented function */
#define CM_ERROR_NOT_IMPLEMENTED           (CmResult)(CM_ERROR_MODULE_BASE + 4)

#define CM_ERROR_NULL_POINTER              (CmResult)(CM_ERROR_MODULE_BASE + 5)

/* Returned when an unexpected error occurs */
#define CM_ERROR_UNEXPECTED                (CmResult)(CM_ERROR_MODULE_BASE + 6)

/* Returned when a memory allocation fails */
#define CM_ERROR_OUT_OF_MEMORY             (CmResult)(CM_ERROR_MODULE_BASE + 7)

/* Returned when an illegal value is passed */
#define CM_ERROR_INVALID_ARG               (CmResult)(CM_ERROR_MODULE_BASE + 8)

/* Returned when an operation can't complete due to an unavailable resource */
#define CM_ERROR_NOT_AVAILABLE             (CmResult)(CM_ERROR_MODULE_BASE + 9)

#define CM_ERROR_WOULD_BLOCK               (CmResult)(CM_ERROR_MODULE_BASE + 10)

#define CM_ERROR_NOT_FOUND                 (CmResult)(CM_ERROR_MODULE_BASE + 11)

#define CM_ERROR_FOUND                     (CmResult)(CM_ERROR_MODULE_BASE + 12)

#define CM_ERROR_PARTIAL_DATA              (CmResult)(CM_ERROR_MODULE_BASE + 13)

#define CM_ERROR_TIMEOUT                   (CmResult)(CM_ERROR_MODULE_BASE + 14)

#endif // CMERROR_H

