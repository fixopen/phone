/*
 * Copyright 2010, Lloyd Hilaiel.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 * 
 *  3. Neither the name of Lloyd Hilaiel nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */ 

#include "yajl_parse.h"
#include "yajl_gen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int reformat_null(void * ctx)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_null(g);
    return 1;
}

static int reformat_boolean(void * ctx, int boolean)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_bool(g, boolean);
    return 1;
}

static int reformat_number(void * ctx, const char * s, unsigned int l)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_number(g, s, l);
    return 1;
}

static int reformat_string(void * ctx, const unsigned char * stringVal,
                           unsigned int stringLen)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_string(g, stringVal, stringLen);
    return 1;
}

static int reformat_map_key(void * ctx, const unsigned char * stringVal,
                            unsigned int stringLen)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_string(g, stringVal, stringLen);
    return 1;
}

static int reformat_start_map(void * ctx)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_map_open(g);
    return 1;
}


static int reformat_end_map(void * ctx)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_map_close(g);
    return 1;
}

static int reformat_start_array(void * ctx)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_array_open(g);
    return 1;
}

static int reformat_end_array(void * ctx)
{
    yajl_gen g = (yajl_gen) ctx;
    yajl_gen_array_close(g);
    return 1;
}

static yajl_callbacks callbacks = {
    reformat_null,
    reformat_boolean,
    NULL,
    NULL,
    reformat_number,
    reformat_string,
    reformat_start_map,
    reformat_map_key,
    reformat_end_map,
    reformat_start_array,
    reformat_end_array
};

static void
usage(const char * progname)
{
    fprintf(stderr, "%s: reformat json from stdin\n"
            "usage:  json_reformat [options]\n"
            "    -m minimize json rather than beautify (default)\n"
            "    -u allow invalid UTF8 inside strings during parsing\n",
            progname);
    exit(1);

}

