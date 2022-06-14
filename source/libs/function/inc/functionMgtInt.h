/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_FUNCTION_MGT_INT_H_
#define _TD_FUNCTION_MGT_INT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "functionMgt.h"

#define FUNCTION_NAME_MAX_LENGTH 32

#define FUNC_MGT_FUNC_CLASSIFICATION_MASK(n) (1 << n)

#define FUNC_MGT_AGG_FUNC               FUNC_MGT_FUNC_CLASSIFICATION_MASK(0)
#define FUNC_MGT_SCALAR_FUNC            FUNC_MGT_FUNC_CLASSIFICATION_MASK(1)
#define FUNC_MGT_INDEFINITE_ROWS_FUNC   FUNC_MGT_FUNC_CLASSIFICATION_MASK(2)
#define FUNC_MGT_STRING_FUNC            FUNC_MGT_FUNC_CLASSIFICATION_MASK(3)
#define FUNC_MGT_DATETIME_FUNC          FUNC_MGT_FUNC_CLASSIFICATION_MASK(4)
#define FUNC_MGT_TIMELINE_FUNC          FUNC_MGT_FUNC_CLASSIFICATION_MASK(5)
#define FUNC_MGT_TIMEORDER_FUNC         FUNC_MGT_FUNC_CLASSIFICATION_MASK(6)
#define FUNC_MGT_PSEUDO_COLUMN_FUNC     FUNC_MGT_FUNC_CLASSIFICATION_MASK(7)
#define FUNC_MGT_WINDOW_PC_FUNC         FUNC_MGT_FUNC_CLASSIFICATION_MASK(8)
#define FUNC_MGT_SPECIAL_DATA_REQUIRED  FUNC_MGT_FUNC_CLASSIFICATION_MASK(9)
#define FUNC_MGT_DYNAMIC_SCAN_OPTIMIZED FUNC_MGT_FUNC_CLASSIFICATION_MASK(10)
#define FUNC_MGT_MULTI_RES_FUNC         FUNC_MGT_FUNC_CLASSIFICATION_MASK(11)
#define FUNC_MGT_SCAN_PC_FUNC           FUNC_MGT_FUNC_CLASSIFICATION_MASK(12)
#define FUNC_MGT_SELECT_FUNC            FUNC_MGT_FUNC_CLASSIFICATION_MASK(13)
#define FUNC_MGT_REPEAT_SCAN_FUNC       FUNC_MGT_FUNC_CLASSIFICATION_MASK(14)
#define FUNC_MGT_FORBID_FILL_FUNC       FUNC_MGT_FUNC_CLASSIFICATION_MASK(15)
#define FUNC_MGT_INTERVAL_INTERPO_FUNC  FUNC_MGT_FUNC_CLASSIFICATION_MASK(16)

#define FUNC_MGT_TEST_MASK(val, mask) (((val) & (mask)) != 0)

#define FUNC_UDF_ID_START 5000

extern const int funcMgtUdfNum;

#ifdef __cplusplus
}
#endif

#endif  // _TD_FUNCTION_MGT_INT_H_
