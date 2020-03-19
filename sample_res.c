/*
  * If not stated otherwise in this file or this component's Licenses.txt file
  * the following copyright and licenses apply:
  *
  * Copyright 2019 RDK Management
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
*/
#include "rtConnection.h"
#include "rtLog.h"
#include "rtMessage.h"

#include <stdlib.h>
#include <unistd.h>

void onMessage(rtMessageHeader const* hdr, rtMessage req, void* closure)
{
  rtConnection con = (rtConnection) closure;

  if (rtMessageHeader_IsRequest(hdr))
  {
    char* buff = NULL;
    uint32_t buff_length = 0;
    
    rtMessage_ToString(req, &buff, &buff_length);
    rtLog_Info("req:%.*s", buff_length, buff);
    free(buff);

    // create response
    rtMessage res;
    rtMessage_Create(&res);
    rtMessage_SetString(res, "reply", "reply -- Hello World");
    rtConnection_SendResponse(con, hdr, res, 1000);
    rtMessage_Release(res);
  }

  rtLog_Info("flags     :%d", hdr->flags);
  rtLog_Info("is_request:%d", rtMessageHeader_IsRequest(hdr));
}

int main()
{
  rtLog_SetLevel(RT_LOG_DEBUG);

  rtConnection con;
  rtConnection_Create(&con, "PROVIDER1", "unix:///tmp/rtrouted");
  rtConnection_AddListener(con, "RDK.MODEL.PROVIDER1", onMessage, con);

  pause();
  rtConnection_Destroy(con);
  return 0;
}

