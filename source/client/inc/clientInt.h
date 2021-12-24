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

#ifndef TDENGINE_CLIENTINT_H
#define TDENGINE_CLIENTINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "taos.h"
#include "common.h"
#include "tmsg.h"
#include "tdef.h"
#include "tep.h"
#include "thash.h"
#include "tlist.h"
#include "tmsgtype.h"
#include "trpc.h"

typedef struct SQueryExecMetric {
  int64_t      start;    // start timestamp
  int64_t      parsed;   // start to parse
  int64_t      send;     // start to send to server
  int64_t      rsp;      // receive response from server
} SQueryExecMetric;

typedef struct SInstanceActivity {
  uint64_t     numOfInsertsReq;
  uint64_t     numOfInsertRows;
  uint64_t     insertElapsedTime;
  uint64_t     insertBytes;         // submit to tsdb since launched.

  uint64_t     fetchBytes;
  uint64_t     queryElapsedTime;
  uint64_t     numOfSlowQueries;
  uint64_t     totalRequests;
  uint64_t     currentRequests;      // the number of SRequestObj
} SInstanceActivity;

typedef struct SHeartBeatInfo {
  void  *pTimer;   // timer, used to send request msg to mnode
} SHeartBeatInfo;

typedef struct SAppInstInfo {
  int64_t           numOfConns;
  SCorEpSet         mgmtEp;
  SInstanceActivity summary;
  SList            *pConnList;  // STscObj linked list
  uint32_t          clusterId;
  void             *pTransporter;
} SAppInstInfo;

typedef struct SAppInfo {
  int64_t        startTime;
  char           appName[TSDB_APP_NAME_LEN];
  char          *ep;
  int32_t        pid;
  int32_t        numOfThreads;
  SHeartBeatInfo hb;
  SHashObj      *pInstMap;
} SAppInfo;

typedef struct STscObj {
  char               user[TSDB_USER_LEN];
  char               pass[TSDB_PASSWORD_LEN];
  char               db[TSDB_ACCT_ID_LEN + TSDB_DB_NAME_LEN];
  int32_t            acctId;
  uint32_t           connId;
  uint64_t           id;       // ref ID returned by taosAddRef
  void              *pTransporter;
  pthread_mutex_t    mutex;     // used to protect the operation on db
  int32_t            numOfReqs; // number of sqlObj from this tscObj
  SAppInstInfo      *pAppInfo;
} STscObj;

typedef struct SReqResultInfo {
  const char  *pRspMsg;
  const char  *pData;
  TAOS_FIELD  *fields;
  uint32_t     numOfCols;

  int32_t     *length;
  TAOS_ROW     row;
  char       **pCol;

  uint32_t     numOfRows;
  uint32_t     current;
} SReqResultInfo;

typedef struct SReqMsg {
  void     *pMsg;
  uint32_t  len;
} SReqMsgInfo;

typedef struct SRequestSendRecvBody {
  tsem_t          rspSem;        // not used now
  void*           fp;
  int64_t         execId;        // showId/queryId
  SReqMsgInfo     requestMsg;
  SReqResultInfo  resInfo;
} SRequestSendRecvBody;

#define ERROR_MSG_BUF_DEFAULT_SIZE  512

typedef struct SRequestObj {
  uint64_t         requestId;
  int32_t          type;   // request type
  STscObj         *pTscObj;
  SQueryExecMetric metric;
  char            *sqlstr;  // sql string
  int32_t          sqlLen;
  SRequestSendRecvBody     body;
  int64_t          self;
  char            *msgBuf;
  int32_t          code;
  void            *pInfo;   // sql parse info, generated by parser module
} SRequestObj;

typedef struct SRequestMsgBody {
  int32_t     msgType;
  SReqMsgInfo msgInfo;
  uint64_t    requestId;
  uint64_t    requestObjRefId;
} SRequestMsgBody;

extern SAppInfo   appInfo;
extern int32_t    tscReqRef;
extern int32_t    tscConnRef;

SRequestMsgBody buildRequestMsgImpl(SRequestObj *pRequest);
extern int (*handleRequestRspFp[TDMT_MAX])(SRequestObj *pRequest, const char* pMsg, int32_t msgLen);

int   taos_init();

void* createTscObj(const char* user, const char* auth, const char *db, SAppInstInfo* pAppInfo);
void  destroyTscObj(void*pObj);

void *createRequest(STscObj* pObj, __taos_async_fn_t fp, void* param, int32_t type);
void  destroyRequest(SRequestObj* pRequest);

char *getConnectionDB(STscObj* pObj);
void  setConnectionDB(STscObj* pTscObj, const char* db);

void taos_init_imp(void);
int taos_options_imp(TSDB_OPTION option, const char *str);

void* openTransporter(const char *user, const char *auth, int32_t numOfThreads);

void processMsgFromServer(void* parent, SRpcMsg* pMsg, SEpSet* pEpSet);
void initMsgHandleFp();

TAOS *taos_connect_internal(const char *ip, const char *user, const char *pass, const char *auth, const char *db, uint16_t port);
TAOS_RES *taos_query_l(TAOS *taos, const char *sql, int sqlLen);

void* doFetchRow(SRequestObj* pRequest);
void setResultDataPtr(SReqResultInfo* pResultInfo, TAOS_FIELD* pFields, int32_t numOfCols, int32_t numOfRows);

#ifdef __cplusplus
}
#endif

#endif  // TDENGINE_CLIENTINT_H
