/*
 * Copyright (c) 2010, 2011 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OPENVSWITCH_TYPES_H
#define OPENVSWITCH_TYPES_H 1

//#include <linux/types.h>
//#include <sys/types.h>
#include <stdint.h>

#ifdef __CHECKER__
#define WPL_BITWISE __attribute__((bitwise))
#define WPL_FORCE __attribute__((force))
#else
#define WPL_BITWISE
#define WPL_FORCE
#endif


namespace openflow
{

typedef ACE_UINT16 WPL_BITWISE ofp_port_t;
typedef ACE_UINT32 WPL_BITWISE odp_port_t;
typedef ACE_UINT32 WPL_BITWISE ofp11_port_t;

/* Macro functions that cast int types to ofp/odp/ofp11 types. */
#define OFP_PORT_C(X) ((WPL_FORCE ofp_port_t) (X))
#define ODP_PORT_C(X) ((WPL_FORCE odp_port_t) (X))
#define OFP11_PORT_C(X) ((WPL_FORCE ofp11_port_t) (X))

}  // namespace openflow

#endif /* openvswitch/types.h */
