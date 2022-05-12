/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo/QMSL_GPIB/QMSL_GPIB_MSVC7/QSEQ_EQ.h#5 $
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
#ifndef __QSEQ_EQ_H__
#define __QSEQ_EQ_H__

#include "QSEQ.h"  // <- consider splitting .h   a) Basic TYPES    b) FunctionPrototypes  (?)

//--------------------------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_EQ_Set ( long nSegItem, QSEQ_Sequence_type   SeqItem[] );

//--------------------------------------------------------------------------------------------------------------------------------------------
unsigned char QSEQ_Sequence_EQ (   const int               g_hInst,      // encapsulate instr handle, change to "InstanceId"
                           const QSEQ_SubAction_Type   sub_action );

#endif
