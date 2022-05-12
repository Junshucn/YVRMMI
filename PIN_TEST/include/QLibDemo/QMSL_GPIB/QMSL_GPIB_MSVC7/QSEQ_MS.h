/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_GPIB/QMSL_GPIB_MSVC7/QSEQ_MS.h#5 $
 * $DateTime: 2016/04/04 17:51:51 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#ifndef __QSEQ_MS_H__
#define __QSEQ_MS_H__

#include "QSEQ.h"

//--------------------------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_MS_Set ( long nSegItem, QSEQ_Sequence_type   *SeqItem );

//--------------------------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_MS (   HANDLE            g_hQMSL, 
                           QSEQ_SubAction_Type   sub_action );

#endif
