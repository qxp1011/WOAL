/*
 * Copyright (c) 2013 Nanjing Weifeng Networks, Inc.
 *
 * Licensed under the FreeBSD License(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.freebsd.org/copyright/freebsd-license.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COF_CONNECTOR_H
#define COF_CONNECTOR_H

#include "ace/Synch_Traits.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"

#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

class COfConnector : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

public:
  //COfConnector () : notifier_ (0, this, ACE_Event_Handler::WRITE_MASK)
  //    {}
  COfConnector();
  virtual ~COfConnector();

  virtual int open (void * = 0);

  // Called when input is available from the client.
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  // Called when output is possible.
  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  // Called when a timer expires.
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);

private:
  enum { ITERATIONS = 5 };
  int m_iterations;
  //ACE_Reactor_Notification_Strategy notifier_;
};

#endif /* COF_CONNECTOR_H */
