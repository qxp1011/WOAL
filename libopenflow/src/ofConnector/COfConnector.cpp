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

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "COfConnector.h"

COfConnector::COfConnector()
    : m_iterations(0)
{
}

COfConnector::~COfConnector()
{
}

int COfConnector::open (void *p)
{
    //ACE_Time_Value iter_delay (2);   // Two seconds
    if (super::open (p) == -1)
    {
        return -1;
    }
    //this->notifier_.reactor (this->reactor ());
    //this->msg_queue ()->notification_strategy (&this->notifier_);
    //this->iterations_ = 0;
    //return this->reactor ()->schedule_timer
    //(this, 0, ACE_Time_Value::zero, iter_delay);

	return 0;
}

int COfConnector::handle_input (ACE_HANDLE)
{
    char buf[64];
    ssize_t recv_cnt = this->peer ().recv (buf, sizeof (buf) - 1);
    if (recv_cnt > 0)
    {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%.*C"),
                  static_cast<int> (recv_cnt),
                  buf));
        return 0;
    }

    if (recv_cnt == 0 || ACE_OS::last_error () != EWOULDBLOCK)
    {
        //this->reactor ()->end_reactor_event_loop ();
        return -1;
    }
    return 0;
}

int COfConnector::handle_timeout(const ACE_Time_Value &, const void *)
{
    if (++this->m_iterations >= ITERATIONS)
    {
        this->peer ().close_writer ();
        return 0;
    }

    ACE_Message_Block *mb = 0;
    ACE_NEW_RETURN (mb, ACE_Message_Block (128), -1);
    int nbytes = ACE_OS::sprintf(mb->wr_ptr (), "Iteration %d\n", this->m_iterations);
    ACE_ASSERT (nbytes > 0);
    mb->wr_ptr (static_cast<size_t> (nbytes));
    this->putq (mb);
    return 0;
}

int COfConnector::handle_output (ACE_HANDLE)
{
    ACE_Message_Block *mb = 0;
    ACE_Time_Value nowait (ACE_OS::gettimeofday ());
    
    while (-1 != this->getq (mb, &nowait))
    {
        ssize_t send_cnt =
        this->peer ().send (mb->rd_ptr (), mb->length ());
        if (send_cnt == -1)
        {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("send")));
        }
        else
        {
            mb->rd_ptr (static_cast<size_t> (send_cnt));
        }
        
        if (mb->length () > 0)
        {
            this->ungetq (mb);
            break;
        }
        mb->release ();
    }
    
    if (this->msg_queue ()->is_empty ())
    {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("COfConnector::handle_output, msg queue is empty.\n")));
    //this->reactor ()->cancel_wakeup
    //  (this, ACE_Event_Handler::WRITE_MASK);
    }
//    else
//    this->reactor ()->schedule_wakeup
//      (this, ACE_Event_Handler::WRITE_MASK);

    return 0;
}


