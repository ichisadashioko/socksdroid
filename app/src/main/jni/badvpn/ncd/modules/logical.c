/**
 * @file logical.c
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
 * Module for logical operators.
 *
 * Synopsis: not(string val)
 * Variables:
 *   string (empty) - "true" if val does not equal "true", "false" otherwise
 *
 * Synopsis: or([string val1, ...])
 * Variables:
 *   string (empty) - "true" if at least one of the values equals "true", "false" otherwise
 *
 * Synopsis: and([string val1, ...])
 * Variables:
 *   string (empty) - "true" if all of the values equal "true", "false" otherwise
 */

#include <stdlib.h>
#include <string.h>

#include <ncd/NCDModule.h>
#include <ncd/static_strings.h>
#include <ncd/extra/value_utils.h>

#include <generated/blog_channel_ncd_logical.h>

#define ModuleLog(i, ...) NCDModuleInst_Backend_Log((i), BLOG_CURRENT_CHANNEL, __VA_ARGS__)

struct instance {
    NCDModuleInst *i;
    int value;
};

static void func_new (void *vo, NCDModuleInst *i, const struct NCDModuleInst_new_params *params, int is_not, int is_or)
{
    struct instance *o = vo;
    o->i = i;

    // compute value from arguments
    if (is_not) {
        NCDValRef arg;
        if (!NCDVal_ListRead(params->args, 1, &arg)) {
            ModuleLog(o->i, BLOG_ERROR, "wrong arity");
            goto fail0;
        }
        if (!NCDVal_IsString(arg)) {
            ModuleLog(o->i, BLOG_ERROR, "wrong type");
            goto fail0;
        }

        o->value = !ncd_read_boolean(arg);
    } else {
        o->value = (is_or ? 0 : 1);

        size_t count = NCDVal_ListCount(params->args);

        for (size_t j = 0; j < count; j++) {
            NCDValRef arg = NCDVal_ListGet(params->args, j);

            if (!NCDVal_IsString(arg)) {
                ModuleLog(o->i, BLOG_ERROR, "wrong type");
                goto fail0;
            }

            int this_value = ncd_read_boolean(arg);
            if (is_or) {
                o->value = o->value || this_value;
            } else {
                o->value = o->value && this_value;
            }
        }
    }

    // signal up
    NCDModuleInst_Backend_Up(o->i);
    return;

fail0:
    NCDModuleInst_Backend_DeadError(i);
}

static void func_new_not (void *vo, NCDModuleInst *i, const struct NCDModuleInst_new_params *params)
{
    func_new(vo, i, params, 1, 0);
}

static void func_new_or (void *vo, NCDModuleInst *i, const struct NCDModuleInst_new_params *params)
{
    func_new(vo, i, params, 0, 1);
}

static void func_new_and (void *vo, NCDModuleInst *i, const struct NCDModuleInst_new_params *params)
{
    func_new(vo, i, params, 0, 0);
}

static int func_getvar2 (void *vo, NCD_string_id_t name, NCDValMem *mem, NCDValRef *out)
{
    struct instance *o = vo;

    if (name == NCD_STRING_EMPTY) {
        *out = ncd_make_boolean(mem, o->value, o->i->params->iparams->string_index);
        return 1;
    }

    return 0;
}

static struct NCDModule modules[] = {
    {
        .type = "not",
        .func_new2 = func_new_not,
        .func_getvar2 = func_getvar2,
        .alloc_size = sizeof(struct instance)
    }, {
        .type = "or",
        .func_new2 = func_new_or,
        .func_getvar2 = func_getvar2,
        .alloc_size = sizeof(struct instance)
    }, {
        .type = "and",
        .func_new2 = func_new_and,
        .func_getvar2 = func_getvar2,
        .alloc_size = sizeof(struct instance)
    }, {
        .type = NULL
    }
};

const struct NCDModuleGroup ncdmodule_logical = {
    .modules = modules
};
