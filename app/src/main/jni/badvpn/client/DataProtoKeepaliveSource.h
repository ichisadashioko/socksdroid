/**
 * @file DataProtoKeepaliveSource.h
 * @author Ambroz Bizjak <ambrop7@gmail.com>
 *
 * @section LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 *
 * A {@link PacketRecvInterface} source which provides DataProto keepalive packets.
 */

#ifndef BADVPN_DATAPROTOKEEPALIVESOURCE_H
#define BADVPN_DATAPROTOKEEPALIVESOURCE_H

#include <base/DebugObject.h>
#include <flow/PacketRecvInterface.h>

/**
 * A {@link PacketRecvInterface} source which provides DataProto keepalive packets.
 * These packets have no payload, no destination peers and flags zero.
 */
typedef struct {
    DebugObject d_obj;
    PacketRecvInterface output;
} DataProtoKeepaliveSource;

/**
 * Initializes the object.
 *
 * @param o the object
 * @param pg pending group
 */
void DataProtoKeepaliveSource_Init (DataProtoKeepaliveSource *o, BPendingGroup *pg);

/**
 * Frees the object.
 *
 * @param o the object
 */
void DataProtoKeepaliveSource_Free (DataProtoKeepaliveSource *o);

/**
 * Returns the output interface.
 * The MTU of the output interface will be sizeof(struct dataproto_header).
 *
 * @param o the object
 * @return output interface
 */
PacketRecvInterface * DataProtoKeepaliveSource_GetOutput (DataProtoKeepaliveSource *o);

#endif
