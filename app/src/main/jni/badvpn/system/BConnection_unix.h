/**
 * @file BConnection_unix.h
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
 */

#include <misc/debugerror.h>
#include <base/DebugObject.h>

#define BCONNECTION_SEND_LIMIT 2
#define BCONNECTION_RECV_LIMIT 2
#define BCONNECTION_LISTEN_BACKLOG 128

struct BListener_s {
    BReactor *reactor;
    void *user;
    BListener_handler handler;
    char *unix_socket_path;
    int fd;
    BFileDescriptor bfd;
    BPending default_job;
    DebugObject d_obj;
};

struct BConnector_s {
    BReactor *reactor;
    void *user;
    BConnector_handler handler;
    BPending job;
    int fd;
    int connected;
    int have_bfd;
    BFileDescriptor bfd;
    DebugObject d_obj;
};

struct BConnection_s {
    BReactor *reactor;
    void *user;
    BConnection_handler handler;
    int fd;
    int close_fd;
    int is_hupd;
    BFileDescriptor bfd;
    int wait_events;
    struct {
        BReactorLimit limit;
        StreamPassInterface iface;
        BPending job;
        const uint8_t *busy_data;
        int busy_data_len;
        int state;
    } send;
    struct {
        BReactorLimit limit;
        StreamRecvInterface iface;
        BPending job;
        uint8_t *busy_data;
        int busy_data_avail;
        int state;
    } recv;
    DebugError d_err;
    DebugObject d_obj;
};
