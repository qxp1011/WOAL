/* Copyright (c) 2008, 2011, 2012 The Board of Trustees of The Leland Stanford
 * Junior University
 *
 * We are making the OpenFlow specification and associated documentation
 * (Software) available for public use and benefit with the expectation
 * that others will use, modify and enhance the Software and contribute
 * those enhancements back to the community. However, since we would
 * like to make the Software available for broadest use, with as few
 * restrictions as possible permission is hereby granted, free of
 * charge, to any person obtaining a copy of this Software to deal in
 * the Software under the copyrights without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * The name and trademarks of copyright holder(s) may NOT be used in
 * advertising or publicity pertaining to the Software or any
 * derivatives without specific, written prior permission.
 */

/*
 * Copyright (c) 2008, 2009, 2010, 2011 Nicira, Inc.
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

/* OpenFlow: protocol between controller and datapath. */

#ifndef OPENFLOW_11_H
#define OPENFLOW_11_H 1

#include "openflow-common.h"

/* OpenFlow 1.1 uses 32-bit port numbers.  Open vSwitch, for now, uses OpenFlow
 * 1.0 port numbers internally.  We map them to OpenFlow 1.0 as follows:
 *
 * OF1.1                    <=>  OF1.0
 * -----------------------       ---------------
 * 0x00000000...0x0000feff  <=>  0x0000...0xfeff  "physical" ports
 * 0x0000ff00...0xfffffeff  <=>  not supported
 * 0xffffff00...0xffffffff  <=>  0xff00...0xffff  "reserved" OFPP_* ports
 *
 * OFPP11_OFFSET is the value that must be added or subtracted to convert
 * an OpenFlow 1.0 reserved port number to or from, respectively, the
 * corresponding OpenFlow 1.1 reserved port number.
 */
#define OFPP11_MAX    OFP11_PORT_C(0xffffff00)
#define OFPP11_OFFSET 0xffff0000    /* OFPP11_MAX - OFPP_MAX */

/* Reserved wildcard port used only for flow mod (delete) and flow stats
 * requests. Selects all flows regardless of output port
 * (including flows with no output port)
 *
 * Define it via OFPP_NONE (0xFFFF) so that OFPP_ANY is still an enum ofp_port
 */
//#define OFPP_ANY OFPP_NONE

/* OpenFlow 1.1 port config flags are just the common flags. */
#define OFPPC11_ALL \
    (OFPPC_PORT_DOWN | OFPPC_NO_RECV | OFPPC_NO_FWD | OFPPC_NO_PACKET_IN)

/* OpenFlow 1.1 specific current state of the physical port.  These are not
 * configurable from the controller.
 */
enum ofp11_port_state {
    OFPPS11_BLOCKED      = 1 << 1,  /* Port is blocked */
    OFPPS11_LIVE         = 1 << 2,  /* Live for Fast Failover Group. */
#define OFPPS11_ALL (OFPPS_LINK_DOWN | OFPPS11_BLOCKED | OFPPS11_LIVE)
};

/* OpenFlow 1.1 specific features of ports available in a datapath. */
enum ofp11_port_features {
    OFPPF11_40GB_FD    = 1 << 7,  /* 40 Gb full-duplex rate support. */
    OFPPF11_100GB_FD   = 1 << 8,  /* 100 Gb full-duplex rate support. */
    OFPPF11_1TB_FD     = 1 << 9,  /* 1 Tb full-duplex rate support. */
    OFPPF11_OTHER      = 1 << 10, /* Other rate, not in the list. */

    OFPPF11_COPPER     = 1 << 11, /* Copper medium. */
    OFPPF11_FIBER      = 1 << 12, /* Fiber medium. */
    OFPPF11_AUTONEG    = 1 << 13, /* Auto-negotiation. */
    OFPPF11_PAUSE      = 1 << 14, /* Pause. */
    OFPPF11_PAUSE_ASYM = 1 << 15  /* Asymmetric pause. */
#define OFPPF11_ALL ((1 << 16) - 1)
};

/* Description of a port */
struct ofp11_port {
    ACE_UINT32 port_no;
    ACE_CDR::Octet pad[4];
    ACE_CDR::Octet hw_addr[OFP_ETH_ALEN];
    ACE_CDR::Octet pad2[2];                  /* Align to 64 bits. */
    char name[OFP_MAX_PORT_NAME_LEN]; /* Null-terminated */

    ACE_UINT32 config;        /* Bitmap of OFPPC_* flags. */
    ACE_UINT32 state;         /* Bitmap of OFPPS_* and OFPPS11_* flags. */

    /* Bitmaps of OFPPF_* and OFPPF11_* that describe features.  All bits
     * zeroed if unsupported or unavailable. */
    ACE_UINT32 curr;          /* Current features. */
    ACE_UINT32 advertised;    /* Features being advertised by the port. */
    ACE_UINT32 supported;     /* Features supported by the port. */
    ACE_UINT32 peer;          /* Features advertised by peer. */

    ACE_UINT32 curr_speed;    /* Current port bitrate in kbps. */
    ACE_UINT32 max_speed;     /* Max port bitrate in kbps */
};
OFP_ASSERT(sizeof(struct ofp11_port) == 64);

/* Modify behavior of the physical port */
struct ofp11_port_mod {
    ACE_UINT32 port_no;
    ACE_CDR::Octet pad[4];
    ACE_CDR::Octet hw_addr[OFP_ETH_ALEN]; /* The hardware address is not
                                      configurable.  This is used to
                                      sanity-check the request, so it must
                                      be the same as returned in an
                                      ofp11_port struct. */
    ACE_CDR::Octet pad2[2];        /* Pad to 64 bits. */
    ACE_UINT32 config;        /* Bitmap of OFPPC_* flags. */
    ACE_UINT32 mask;          /* Bitmap of OFPPC_* flags to be changed. */

    ACE_UINT32 advertise;     /* Bitmap of OFPPF_* and OFPPF11_*.  Zero all bits
                               to prevent any action taking place. */
    ACE_CDR::Octet pad3[4];        /* Pad to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp11_port_mod) == 32);

/* Group setup and teardown (controller -> datapath). */
struct ofp11_group_mod {
    ACE_UINT16 command;             /* One of OFPGC_*. */
    ACE_CDR::Octet type;                 /* One of OFPGT_*. */
    ACE_CDR::Octet pad;                  /* Pad to 64 bits. */
    ACE_UINT32 group_id;            /* Group identifier. */
    /* struct ofp11_bucket buckets[0]; The bucket length is inferred from the
                                       length field in the header. */
};
OFP_ASSERT(sizeof(struct ofp11_group_mod) == 8);

/* Query for port queue configuration. */
struct ofp11_queue_get_config_request {
    ACE_UINT32 port;
    /* Port to be queried. Should refer
       to a valid physical port (i.e. < OFPP_MAX) */
    ACE_CDR::Octet pad[4];
};
OFP_ASSERT(sizeof(struct ofp11_queue_get_config_request) == 8);

/* Group commands */
enum ofp11_group_mod_command {
    OFPGC11_ADD,          /* New group. */
    OFPGC11_MODIFY,       /* Modify all matching groups. */
    OFPGC11_DELETE,       /* Delete all matching groups. */
};

/* OpenFlow 1.1 specific capabilities supported by the datapath (struct
 * ofp_switch_features, member capabilities). */
enum ofp11_capabilities {
    OFPC11_GROUP_STATS    = 1 << 3,  /* Group statistics. */
};

enum ofp11_action_type {
    OFPAT11_OUTPUT,           /* Output to switch port. */
    OFPAT11_SET_VLAN_VID,     /* Set the 802.1q VLAN id. */
    OFPAT11_SET_VLAN_PCP,     /* Set the 802.1q priority. */
    OFPAT11_SET_DL_SRC,       /* Ethernet source address. */
    OFPAT11_SET_DL_DST,       /* Ethernet destination address. */
    OFPAT11_SET_NW_SRC,       /* IP source address. */
    OFPAT11_SET_NW_DST,       /* IP destination address. */
    OFPAT11_SET_NW_TOS,       /* IP ToS (DSCP field, 6 bits). */
    OFPAT11_SET_NW_ECN,       /* IP ECN (2 bits). */
    OFPAT11_SET_TP_SRC,       /* TCP/UDP/SCTP source port. */
    OFPAT11_SET_TP_DST,       /* TCP/UDP/SCTP destination port. */
    OFPAT11_COPY_TTL_OUT,     /* Copy TTL "outwards" -- from next-to-outermost
                                 to outermost */
    OFPAT11_COPY_TTL_IN,      /* Copy TTL "inwards" -- from outermost to
                               next-to-outermost */
    OFPAT11_SET_MPLS_LABEL,   /* MPLS label */
    OFPAT11_SET_MPLS_TC,      /* MPLS TC */
    OFPAT11_SET_MPLS_TTL,     /* MPLS TTL */
    OFPAT11_DEC_MPLS_TTL,     /* Decrement MPLS TTL */

    OFPAT11_PUSH_VLAN,        /* Push a new VLAN tag */
    OFPAT11_POP_VLAN,         /* Pop the outer VLAN tag */
    OFPAT11_PUSH_MPLS,        /* Push a new MPLS Label Stack Entry */
    OFPAT11_POP_MPLS,         /* Pop the outer MPLS Label Stack Entry */
    OFPAT11_SET_QUEUE,        /* Set queue id when outputting to a port */
    OFPAT11_GROUP,            /* Apply group. */
    OFPAT11_SET_NW_TTL,       /* IP TTL. */
    OFPAT11_DEC_NW_TTL,       /* Decrement IP TTL. */
    OFPAT11_EXPERIMENTER = 0xffff
};

#define OFPMT11_STANDARD_LENGTH 88

struct ofp11_match_header {
    ACE_UINT16 type;             /* One of OFPMT_* */
    ACE_UINT16 length;           /* Length of match */
};
OFP_ASSERT(sizeof(struct ofp11_match_header) == 4);

/* Fields to match against flows */
struct ofp11_match {
    struct ofp11_match_header omh;
    ACE_UINT32 in_port;          /* Input switch port. */
    ACE_UINT32 wildcards;        /* Wildcard fields. */
    ACE_CDR::Octet dl_src[OFP_ETH_ALEN]; /* Ethernet source address. */
    ACE_CDR::Octet dl_src_mask[OFP_ETH_ALEN]; /* Ethernet source address mask.  */
    ACE_CDR::Octet dl_dst[OFP_ETH_ALEN]; /* Ethernet destination address. */
    ACE_CDR::Octet dl_dst_mask[OFP_ETH_ALEN]; /* Ethernet destination address mask. */
    ACE_UINT16 dl_vlan;          /* Input VLAN id. */
    ACE_CDR::Octet dl_vlan_pcp;       /* Input VLAN priority. */
    ACE_CDR::Octet pad1[1];           /* Align to 32-bits */
    ACE_UINT16 dl_type;          /* Ethernet frame type. */
    ACE_CDR::Octet nw_tos;            /* IP ToS (actually DSCP field, 6 bits). */
    ACE_CDR::Octet nw_proto;          /* IP protocol or lower 8 bits of ARP opcode. */
    ACE_UINT32 nw_src;           /* IP source address. */
    ACE_UINT32 nw_src_mask;      /* IP source address mask. */
    ACE_UINT32 nw_dst;           /* IP destination address. */
    ACE_UINT32 nw_dst_mask;      /* IP destination address mask. */
    ACE_UINT16 tp_src;           /* TCP/UDP/SCTP source port. */
    ACE_UINT16 tp_dst;           /* TCP/UDP/SCTP destination port. */
    ACE_UINT32 mpls_label;       /* MPLS label. */
    ACE_CDR::Octet mpls_tc;           /* MPLS TC. */
    ACE_CDR::Octet pad2[3];           /* Align to 64-bits */
    ACE_UINT64 metadata;         /* Metadata passed between tables. */
    ACE_UINT64 metadata_mask;    /* Mask for metadata. */
};
OFP_ASSERT(sizeof(struct ofp11_match) == OFPMT11_STANDARD_LENGTH);

/* Flow wildcards. */
enum ofp11_flow_wildcards {
    OFPFW11_IN_PORT     = 1 << 0,  /* Switch input port. */
    OFPFW11_DL_VLAN     = 1 << 1,  /* VLAN id. */
    OFPFW11_DL_VLAN_PCP = 1 << 2,  /* VLAN priority. */
    OFPFW11_DL_TYPE     = 1 << 3,  /* Ethernet frame type. */
    OFPFW11_NW_TOS      = 1 << 4,  /* IP ToS (DSCP field, 6 bits). */
    OFPFW11_NW_PROTO    = 1 << 5,  /* IP protocol. */
    OFPFW11_TP_SRC      = 1 << 6,  /* TCP/UDP/SCTP source port. */
    OFPFW11_TP_DST      = 1 << 7,  /* TCP/UDP/SCTP destination port. */
    OFPFW11_MPLS_LABEL  = 1 << 8,  /* MPLS label. */
    OFPFW11_MPLS_TC     = 1 << 9,  /* MPLS TC. */

    /* Wildcard all fields. */
    OFPFW11_ALL           = ((1 << 10) - 1)
};

/* The VLAN id is 12-bits, so we can use the entire 16 bits to indicate
 * special conditions.
 */
enum ofp11_vlan_id {
    OFPVID11_ANY = 0xfffe,  /* Indicate that a VLAN id is set but don't care
                               about it's value. Note: only valid when
                               specifying the VLAN id in a match */
    OFPVID11_NONE = 0xffff, /* No VLAN id was set. */
};

enum ofp11_instruction_type {
    OFPIT11_GOTO_TABLE = 1,        /* Setup the next table in the lookup
                                      pipeline */
    OFPIT11_WRITE_METADATA = 2,    /* Setup the metadata field for use later
                                      in pipeline */
    OFPIT11_WRITE_ACTIONS = 3,     /* Write the action(s) onto the datapath
                                      action set */
    OFPIT11_APPLY_ACTIONS = 4,     /* Applies the action(s) immediately */
    OFPIT11_CLEAR_ACTIONS = 5,     /* Clears all actions from the datapath
                                      action set */
    OFPIT11_EXPERIMENTER = 0xFFFF  /* Experimenter instruction */
};

#define OFPIT11_ALL (OFPIT11_GOTO_TABLE | OFPIT11_WRITE_METADATA |      \
                     OFPIT11_WRITE_ACTIONS | OFPIT11_APPLY_ACTIONS |    \
                     OFPIT11_CLEAR_ACTIONS)

#define OFP11_INSTRUCTION_ALIGN 8

/* Generic ofp_instruction structure. */
struct ofp11_instruction {
    ACE_UINT16 type;              /* Instruction type */
    ACE_UINT16 len;               /* Length of this struct in bytes. */
    ACE_CDR::Octet pad[4];             /* Align to 64-bits */
};
OFP_ASSERT(sizeof(struct ofp11_instruction) == 8);

/* Instruction structure for OFPIT_GOTO_TABLE */
struct ofp11_instruction_goto_table {
    ACE_UINT16 type;                 /* OFPIT_GOTO_TABLE */
    ACE_UINT16 len;                  /* Length of this struct in bytes. */
    ACE_CDR::Octet table_id;              /* Set next table in the lookup pipeline */
    ACE_CDR::Octet pad[3];                /* Pad to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp11_instruction_goto_table) == 8);

/* Instruction structure for OFPIT_WRITE_METADATA */
struct ofp11_instruction_write_metadata {
    ACE_UINT16 type;              /* OFPIT_WRITE_METADATA */
    ACE_UINT16 len;               /* Length of this struct in bytes. */
    ACE_CDR::Octet pad[4];             /* Align to 64-bits */
    ACE_UINT64 metadata;          /* Metadata value to write */
    ACE_UINT64 metadata_mask;     /* Metadata write bitmask */
};
OFP_ASSERT(sizeof(struct ofp11_instruction_write_metadata) == 24);

/* Instruction structure for OFPIT_WRITE/APPLY/CLEAR_ACTIONS */
struct ofp11_instruction_actions {
    ACE_UINT16 type;              /* One of OFPIT_*_ACTIONS */
    ACE_UINT16 len;               /* Length of this struct in bytes. */
    ACE_CDR::Octet pad[4];             /* Align to 64-bits */
    /* struct ofp_action_header actions[0];  Actions associated with
                                             OFPIT_WRITE_ACTIONS and
                                             OFPIT_APPLY_ACTIONS */
};
OFP_ASSERT(sizeof(struct ofp11_instruction_actions) == 8);

/* Instruction structure for experimental instructions */
struct ofp11_instruction_experimenter {
    ACE_UINT16 type;              /* OFPIT11_EXPERIMENTER */
    ACE_UINT16 len;               /* Length of this struct in bytes */
    ACE_UINT32 experimenter;      /* Experimenter ID which takes the same form
                                   as in struct ofp_vendor_header. */
    /* Experimenter-defined arbitrary additional data. */
};
OFP_ASSERT(sizeof(struct ofp11_instruction_experimenter) == 8);

/* Action structure for OFPAT_OUTPUT, which sends packets out 'port'.
   * When the 'port' is the OFPP_CONTROLLER, 'max_len' indicates the max
   * number of bytes to send. A 'max_len' of zero means no bytes of the
   * packet should be sent.*/
struct ofp11_action_output {
    ACE_UINT16 type;                    /* OFPAT11_OUTPUT. */
    ACE_UINT16 len;                     /* Length is 16. */
    ACE_UINT32 port;                    /* Output port. */
    ACE_UINT16 max_len;                 /* Max length to send to controller. */
    ACE_CDR::Octet pad[6];                   /* Pad to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp11_action_output) == 16);

/* Action structure for OFPAT_GROUP. */
struct ofp11_action_group {
    ACE_UINT16 type;                    /* OFPAT11_GROUP. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_UINT32 group_id;                /* Group identifier. */
};
OFP_ASSERT(sizeof(struct ofp11_action_group) == 8);

/* OFPAT_SET_QUEUE action struct: send packets to given queue on port. */
struct ofp11_action_set_queue {
    ACE_UINT16 type;                    /* OFPAT11_SET_QUEUE. */
    ACE_UINT16 len;                     /* Len is 8. */
    ACE_UINT32 queue_id;                /* Queue id for the packets. */
};
OFP_ASSERT(sizeof(struct ofp11_action_set_queue) == 8);

/* Action structure for OFPAT11_SET_MPLS_LABEL. */
struct ofp11_action_mpls_label {
    ACE_UINT16 type;                    /* OFPAT11_SET_MPLS_LABEL. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_UINT32 mpls_label;              /* MPLS label */
};
OFP_ASSERT(sizeof(struct ofp11_action_mpls_label) == 8);

/* Action structure for OFPAT11_SET_MPLS_TC. */
struct ofp11_action_mpls_tc {
    ACE_UINT16 type;                    /* OFPAT11_SET_MPLS_TC. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_CDR::Octet mpls_tc;                  /* MPLS TC */
    ACE_CDR::Octet pad[3];
};
OFP_ASSERT(sizeof(struct ofp11_action_mpls_tc) == 8);

/* Action structure for OFPAT11_SET_MPLS_TTL. */
struct ofp11_action_mpls_ttl {
    ACE_UINT16 type;                    /* OFPAT11_SET_MPLS_TTL. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_CDR::Octet mpls_ttl;                 /* MPLS TTL */
    ACE_CDR::Octet pad[3];
};
OFP_ASSERT(sizeof(struct ofp11_action_mpls_ttl) == 8);

/* Action structure for OFPAT11_SET_NW_ECN. */
struct ofp11_action_nw_ecn {
    ACE_UINT16 type;                    /* OFPAT11_SET_TW_SRC/DST. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_CDR::Octet nw_ecn;                   /* IP ECN (2 bits). */
    ACE_CDR::Octet pad[3];
};
OFP_ASSERT(sizeof(struct ofp11_action_nw_ecn) == 8);

/* Action structure for OFPAT11_SET_NW_TTL. */
struct ofp11_action_nw_ttl {
    ACE_UINT16 type;                    /* OFPAT11_SET_NW_TTL. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_CDR::Octet nw_ttl;                   /* IP TTL */
    ACE_CDR::Octet pad[3];
};
OFP_ASSERT(sizeof(struct ofp11_action_nw_ttl) == 8);

/* Action structure for OFPAT11_PUSH_VLAN/MPLS. */
struct ofp11_action_push {
    ACE_UINT16 type;                    /* OFPAT11_PUSH_VLAN/MPLS. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_UINT16 ethertype;               /* Ethertype */
    ACE_CDR::Octet pad[2];
};
OFP_ASSERT(sizeof(struct ofp11_action_push) == 8);

/* Action structure for OFPAT11_POP_MPLS. */
struct ofp11_action_pop_mpls {
    ACE_UINT16 type;                    /* OFPAT11_POP_MPLS. */
    ACE_UINT16 len;                     /* Length is 8. */
    ACE_UINT16 ethertype;               /* Ethertype */
    ACE_CDR::Octet pad[2];
};
OFP_ASSERT(sizeof(struct ofp11_action_pop_mpls) == 8);

/* Configure/Modify behavior of a flow table */
struct ofp11_table_mod {
    ACE_CDR::Octet table_id;       /* ID of the table, 0xFF indicates all tables */
    ACE_CDR::Octet pad[3];         /* Pad to 32 bits */
    ACE_UINT32 config;        /* Bitmap of OFPTC_* flags */
};
OFP_ASSERT(sizeof(struct ofp11_table_mod) == 8);

/* Flags to indicate behavior of the flow table for unmatched packets.
   These flags are used in ofp_table_stats messages to describe the current
   configuration and in ofp_table_mod messages to configure table behavior.  */
enum ofp11_table_config {
    OFPTC11_TABLE_MISS_CONTROLLER = 0,    /* Send to controller. */
    OFPTC11_TABLE_MISS_CONTINUE = 1 << 0, /* Continue to the next table in the
                                             pipeline (OpenFlow 1.0
                                             behavior). */
    OFPTC11_TABLE_MISS_DROP = 1 << 1,     /* Drop the packet. */
    OFPTC11_TABLE_MISS_MASK = 3
};

/* Flow setup and teardown (controller -> datapath). */
struct ofp11_flow_mod {
    ACE_UINT64 cookie;             /* Opaque controller-issued identifier. */
    ACE_UINT64 cookie_mask;        /* Mask used to restrict the cookie bits
                                    that must match when the command is
                                    OFPFC_MODIFY* or OFPFC_DELETE*. A value
                                    of 0 indicates no restriction. */
    /* Flow actions. */
    ACE_CDR::Octet table_id;            /* ID of the table to put the flow in */
    ACE_CDR::Octet command;             /* One of OFPFC_*. */
    ACE_UINT16 idle_timeout;       /* Idle time before discarding (seconds). */
    ACE_UINT16 hard_timeout;       /* Max time before discarding (seconds). */
    ACE_UINT16 priority;           /* Priority level of flow entry. */
    ACE_UINT32 buffer_id;          /* Buffered packet to apply to (or -1).
                                    Not meaningful for OFPFC_DELETE*. */
    ACE_UINT32 out_port;           /* For OFPFC_DELETE* commands, require
                                    matching entries to include this as an
                                    output port. A value of OFPP_ANY
                                    indicates no restriction. */
    ACE_UINT32 out_group;          /* For OFPFC_DELETE* commands, require
                                    matching entries to include this as an
                                    output group. A value of OFPG11_ANY
                                    indicates no restriction. */
    ACE_UINT16 flags;              /* One of OFPFF_*. */
    ACE_CDR::Octet pad[2];
    /* Followed by an ofp11_match structure. */
    /* Followed by an instruction set. */
};
OFP_ASSERT(sizeof(struct ofp11_flow_mod) == 40);

/* Group types. Values in the range [128, 255] are reserved for experimental
 * use. */
enum ofp11_group_type {
    OFPGT11_ALL,      /* All (multicast/broadcast) group. */
    OFPGT11_SELECT,   /* Select group. */
    OFPGT11_INDIRECT, /* Indirect group. */
    OFPGT11_FF        /* Fast failover group. */
};

/* Group numbering. Groups can use any number up to OFPG_MAX. */
enum ofp11_group {
    /* Last usable group number. */
    OFPG11_MAX        = 0xffffff00,

    /* Fake groups. */
    OFPG11_ALL        = 0xfffffffc,  /* Represents all groups for group delete
                                        commands. */
    OFPG11_ANY        = 0xffffffff   /* Wildcard group used only for flow stats
                                        requests. Selects all flows regardless
                                        of group (including flows with no
                                        group). */
};

/* Bucket for use in groups. */
struct ofp11_bucket {
    ACE_UINT16 len;                    /* Length the bucket in bytes, including
                                        this header and any padding to make it
                                        64-bit aligned. */
    ACE_UINT16 weight;                 /* Relative weight of bucket. Only
                                        defined for select groups. */
    ACE_UINT32 watch_port;             /* Port whose state affects whether this
                                        bucket is live. Only required for fast
                                        failover groups. */
    ACE_UINT32 watch_group;            /* Group whose state affects whether this
                                        bucket is live. Only required for fast
                                        failover groups. */
    ACE_CDR::Octet pad[4];
    /* struct ofp_action_header actions[0]; The action length is inferred
                                            from the length field in the
                                            header. */
};
OFP_ASSERT(sizeof(struct ofp11_bucket) == 16);

/* Queue configuration for a given port. */
struct ofp11_queue_get_config_reply {
    ACE_UINT32 port;
    ACE_CDR::Octet pad[4];
    /* struct ofp_packet_queue queues[0];  List of configured queues. */
};
OFP_ASSERT(sizeof(struct ofp11_queue_get_config_reply) == 8);

struct ofp11_stats_msg {
    struct ofp_header header;
    ACE_UINT16 type;              /* One of the OFPST_* constants. */
    ACE_UINT16 flags;             /* OFPSF_REQ_* flags (none yet defined). */
    ACE_CDR::Octet pad[4];
    /* Followed by the body of the request. */
};
OFP_ASSERT(sizeof(struct ofp11_stats_msg) == 16);

/* Vendor extension stats message. */
struct ofp11_vendor_stats_msg {
    struct ofp11_stats_msg osm; /* Type OFPST_VENDOR. */
    ACE_UINT32 vendor;            /* Vendor ID:
                                 * - MSB 0: low-order bytes are IEEE OUI.
                                 * - MSB != 0: defined by OpenFlow
                                 *   consortium. */
    /* Followed by vendor-defined arbitrary additional data. */
};
OFP_ASSERT(sizeof(struct ofp11_vendor_stats_msg) == 20);

/* Stats request of type OFPST_FLOW. */
struct ofp11_flow_stats_request {
    ACE_CDR::Octet table_id;         /* ID of table to read (from ofp_table_stats),
                                 0xff for all tables. */
    ACE_CDR::Octet pad[3];           /* Align to 64 bits. */
    ACE_UINT32 out_port;        /* Require matching entries to include this
                                 as an output port. A value of OFPP_ANY
                                 indicates no restriction. */
    ACE_UINT32 out_group;       /* Require matching entries to include this
                                 as an output group. A value of OFPG11_ANY
                                 indicates no restriction. */
    ACE_CDR::Octet pad2[4];          /* Align to 64 bits. */
    ACE_UINT64 cookie;          /* Require matching entries to contain this
                                 cookie value */
    ACE_UINT64 cookie_mask;     /* Mask used to restrict the cookie bits that
                                 must match. A value of 0 indicates
                                 no restriction. */
    /* Followed by an ofp11_match structure. */
};
OFP_ASSERT(sizeof(struct ofp11_flow_stats_request) == 32);

/* Body of reply to OFPST_FLOW request. */
struct ofp11_flow_stats {
    ACE_UINT16 length;           /* Length of this entry. */
    ACE_CDR::Octet table_id;          /* ID of table flow came from. */
    ACE_CDR::Octet pad;
    ACE_UINT32 duration_sec;     /* Time flow has been alive in seconds. */
    ACE_UINT32 duration_nsec;    /* Time flow has been alive in nanoseconds beyond
                                  duration_sec. */
    ACE_UINT16 priority;         /* Priority of the entry. Only meaningful
                                  when this is not an exact-match entry. */
    ACE_UINT16 idle_timeout;     /* Number of seconds idle before expiration. */
    ACE_UINT16 hard_timeout;     /* Number of seconds before expiration. */
    ACE_UINT16 flags;            /* OF 1.3: Set of OFPFF*. */
    ACE_CDR::Octet  pad2[4];          /* Align to 64-bits. */
    ACE_UINT64 cookie;           /* Opaque controller-issued identifier. */
    ACE_UINT64 packet_count;     /* Number of packets in flow. */
    ACE_UINT64 byte_count;       /* Number of bytes in flow. */
    /* Open Flow version specific match */
    /* struct ofp11_instruction instructions[0];  Instruction set. */
};
OFP_ASSERT(sizeof(struct ofp11_flow_stats) == 48);

/* Body for ofp_stats_request of type OFPST_AGGREGATE. */
/* Identical to ofp11_flow_stats_request */

/* Flow match fields. */
enum ofp11_flow_match_fields {
    OFPFMF11_IN_PORT     = 1 << 0,  /* Switch input port. */
    OFPFMF11_DL_VLAN     = 1 << 1,  /* VLAN id. */
    OFPFMF11_DL_VLAN_PCP = 1 << 2,  /* VLAN priority. */
    OFPFMF11_DL_TYPE     = 1 << 3,  /* Ethernet frame type. */
    OFPFMF11_NW_TOS      = 1 << 4,  /* IP ToS (DSCP field, 6 bits). */
    OFPFMF11_NW_PROTO    = 1 << 5,  /* IP protocol. */
    OFPFMF11_TP_SRC      = 1 << 6,  /* TCP/UDP/SCTP source port. */
    OFPFMF11_TP_DST      = 1 << 7,  /* TCP/UDP/SCTP destination port. */
    OFPFMF11_MPLS_LABEL  = 1 << 8,  /* MPLS label. */
    OFPFMF11_MPLS_TC     = 1 << 9,  /* MPLS TC. */
    OFPFMF11_TYPE        = 1 << 10, /* Match type. */
    OFPFMF11_DL_SRC      = 1 << 11, /* Ethernet source address. */
    OFPFMF11_DL_DST      = 1 << 12, /* Ethernet destination address. */
    OFPFMF11_NW_SRC      = 1 << 13, /* IP source address. */
    OFPFMF11_NW_DST      = 1 << 14, /* IP destination address. */
    OFPFMF11_METADATA    = 1 << 15, /* Metadata passed between tables. */
};

/* Body of reply to OFPST_TABLE request. */
struct ofp11_table_stats {
    ACE_CDR::Octet table_id;        /* Identifier of table. Lower numbered tables
                                are consulted first. */
    ACE_CDR::Octet pad[7];          /* Align to 64-bits. */
    char name[OFP_MAX_TABLE_NAME_LEN];
    ACE_UINT32 wildcards;      /* Bitmap of OFPFMF_* wildcards that are
                                supported by the table. */
    ACE_UINT32 match;          /* Bitmap of OFPFMF_* that indicate the fields
                                the table can match on. */
    ACE_UINT32 instructions;   /* Bitmap of OFPIT_* values supported. */
    ACE_UINT32 write_actions;  /* Bitmap of OFPAT_* that are supported
                                by the table with OFPIT_WRITE_ACTIONS.  */
    ACE_UINT32 apply_actions;  /* Bitmap of OFPAT_* that are supported
                                by the table with OFPIT_APPLY_ACTIONS. */
    ACE_UINT32 config;         /* Bitmap of OFPTC_* values */
    ACE_UINT32 max_entries;    /* Max number of entries supported. */
    ACE_UINT32 active_count;   /* Number of active entries. */
    ACE_UINT64 lookup_count;   /* Number of packets looked up in table. */
    ACE_UINT64 matched_count;  /* Number of packets that hit table. */
};
OFP_ASSERT(sizeof(struct ofp11_table_stats) == 88);

/* Body for ofp_stats_request of type OFPST_PORT. */
struct ofp11_port_stats_request {
    ACE_UINT32 port_no;        /* OFPST_PORT message must request statistics
                              * either for a single port (specified in
                              * port_no) or for all ports (if port_no ==
                              * OFPP_ANY). */
    ACE_CDR::Octet pad[4];
};
OFP_ASSERT(sizeof(struct ofp11_port_stats_request) == 8);

/* Body of reply to OFPST_PORT request. If a counter is unsupported, set
 * the field to all ones. */
struct ofp11_port_stats {
    ACE_UINT32 port_no;
    ACE_CDR::Octet pad[4];           /* Align to 64-bits. */
    ACE_UINT64 rx_packets;      /* Number of received packets. */
    ACE_UINT64 tx_packets;      /* Number of transmitted packets. */
    ACE_UINT64 rx_bytes;        /* Number of received bytes. */
    ACE_UINT64 tx_bytes;        /* Number of transmitted bytes. */
    ACE_UINT64 rx_dropped;      /* Number of packets dropped by RX. */
    ACE_UINT64 tx_dropped;      /* Number of packets dropped by TX. */
    ACE_UINT64 rx_errors;       /* Number of receive errors.  This is a
                                 super-set of receive errors and should be
                                 great than or equal to the sum of all
                                 rx_*_err values. */
    ACE_UINT64 tx_errors;       /* Number of transmit errors.  This is a
                                 super-set of transmit errors. */
    ACE_UINT64 rx_frame_err;    /* Number of frame alignment errors. */
    ACE_UINT64 rx_over_err;     /* Number of packets with RX overrun. */
    ACE_UINT64 rx_crc_err;      /* Number of CRC errors. */
    ACE_UINT64 collisions;      /* Number of collisions. */
};
OFP_ASSERT(sizeof(struct ofp11_port_stats) == 104);

struct ofp11_queue_stats_request {
    ACE_UINT32 port_no;         /* All ports if OFPP_ANY. */
    ACE_UINT32 queue_id;        /* All queues if OFPQ_ALL. */
};
OFP_ASSERT(sizeof(struct ofp11_queue_stats_request) == 8);

struct ofp11_queue_stats {
    ACE_UINT32 port_no;
    ACE_UINT32 queue_id;         /* Queue id. */
    ACE_UINT64 tx_bytes;         /* Number of transmitted bytes. */
    ACE_UINT64 tx_packets;       /* Number of transmitted packets. */
    ACE_UINT64 tx_errors;        /* # of packets dropped due to overrun. */
};
OFP_ASSERT(sizeof(struct ofp11_queue_stats) == 32);

struct ofp11_group_stats_request {
    ACE_UINT32 group_id;         /* All groups if OFPG_ALL. */
    ACE_CDR::Octet pad[4];            /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp11_group_stats_request) == 8);

/* Body of reply to OFPST11_GROUP request */
struct ofp11_group_stats {
    ACE_UINT16 length;           /* Length of this entry. */
    ACE_CDR::Octet pad[2];            /* Align to 64 bits. */
    ACE_UINT32 group_id;         /* Group identifier. */
    ACE_UINT32 ref_count;        /* Number of flows or groups that
                                  directly forward to this group. */
    ACE_CDR::Octet pad2[4];           /* Align to 64 bits. */
    ACE_UINT64 packet_count;     /* Number of packets processed by group. */
    ACE_UINT64 byte_count;       /* Number of bytes processed by group. */
    /* struct ofp11_bucket_counter bucket_stats[0]; */

};
OFP_ASSERT(sizeof(struct ofp11_group_stats) == 32);

/* Used in group stats replies. */
struct ofp11_bucket_counter {
    ACE_UINT64 packet_count;   /* Number of packets processed by bucket. */
    ACE_UINT64 byte_count;     /* Number of bytes processed by bucket. */
};
OFP_ASSERT(sizeof(struct ofp11_bucket_counter) == 16);

/* Body of reply to OFPST11_GROUP_DESC request. */
struct ofp11_group_desc_stats {
    ACE_UINT16 length;            /* Length of this entry. */
    ACE_CDR::Octet type;               /* One of OFPGT_*. */
    ACE_CDR::Octet pad;                /* Pad to 64 bits. */
    ACE_UINT32 group_id;          /* Group identifier. */
    /* struct ofp11_bucket buckets[0]; */
};
OFP_ASSERT(sizeof(struct ofp11_group_desc_stats) == 8);

/* Send packet (controller -> datapath). */
struct ofp11_packet_out {
    ACE_UINT32 buffer_id;       /* ID assigned by datapath (-1 if none). */
    ACE_UINT32 in_port;         /* Packet's input port or OFPP_CONTROLLER. */
    ACE_UINT16 actions_len;     /* Size of action array in bytes. */
    ACE_CDR::Octet pad[6];
    /* struct ofp_action_header actions[0];  Action list. */
    /* uint8_t data[0]; */    /* Packet data. The length is inferred
                                 from the length field in the header.
                                 (Only meaningful if buffer_id == -1.) */
};
OFP_ASSERT(sizeof(struct ofp11_packet_out) == 16);

/* Packet received on port (datapath -> controller). */
struct ofp11_packet_in {
    ACE_UINT32 buffer_id;     /* ID assigned by datapath. */
    ACE_UINT32 in_port;       /* Port on which frame was received. */
    ACE_UINT32 in_phy_port;   /* Physical Port on which frame was received. */
    ACE_UINT16 total_len;     /* Full length of frame. */
    ACE_CDR::Octet reason;         /* Reason packet is being sent (one of OFPR_*) */
    ACE_CDR::Octet table_id;       /* ID of the table that was looked up */
    /* uint8_t data[0];        Ethernet frame, halfway through 32-bit word,
                               so the IP header is 32-bit aligned. The
                               amount of data is inferred from the length
                               field in the header. Because of padding,
                               offsetof(struct ofp_packet_in, data) ==
                               sizeof(struct ofp_packet_in) - 2. */
};
OFP_ASSERT(sizeof(struct ofp11_packet_in) == 16);

/* Flow removed (datapath -> controller). */
struct ofp11_flow_removed {
    ACE_UINT64 cookie;          /* Opaque controller-issued identifier. */

    ACE_UINT16 priority;        /* Priority level of flow entry. */
    ACE_CDR::Octet reason;           /* One of OFPRR_*. */
    ACE_CDR::Octet table_id;         /* ID of the table */

    ACE_UINT32 duration_sec;    /* Time flow was alive in seconds. */
    ACE_UINT32 duration_nsec;   /* Time flow was alive in nanoseconds beyond
                                 duration_sec. */
    ACE_UINT16 idle_timeout;    /* Idle timeout from original flow mod. */
    ACE_CDR::Octet pad2[2];          /* Align to 64-bits. */
    ACE_UINT64 packet_count;
    ACE_UINT64 byte_count;
    /* Followed by an ofp11_match structure. */
};
OFP_ASSERT(sizeof(struct ofp11_flow_removed) == 40);

#endif /* openflow/openflow-1.1.h */
