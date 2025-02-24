# driver-API-12c4bf1-cJSON_PrintBuffered-stack_overflow
Stack overflow when calling API `cJSON_PrintBuffered`

## Summary
Stack overflow when calling API `cJSON_PrintBuffered`

## Test Environment
Ubuntu 24.04.1, 64bit  
cjson (master 12c4bf1)

## How to trigger
Compile the PoC test program and run:
```C++
// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithLengthOpts at cJSON.c:1115:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2077:26 in cJSON.h
// cJSON_CreateArrayReference at cJSON.c:2529:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1285:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>

int main(void) {
    char *Data =
        "[51,1111111115111117]13,111111111111117111,11111,11111111111111,"
        "111111111111111111111111111111111111111111111111111211111111117111,6{"
        "\"1111^1";
    size_t Size = 142;

    cJSON *root = cJSON_ParseWithLengthOpts((const char*)Data, Size, NULL, false);
    cJSON *array = cJSON_CreateArrayReference(root);
    cJSON_AddItemToObject(root, "array_ref", array);
    char *json_str = cJSON_PrintBuffered(root, 1024, true);

    free(json_str);
    cJSON_Delete(root);
    cJSON_Delete(array);
    return 0;
}
```
The compile command is:
```bash
$ clang++ ./driver-API-12c4bf1-cJSON_PrintBuffered-stack_overflow.cc -o ./driver-API-12c4bf1-cJSON_PrintBuffered-stack_overflow -fsanitize=address,undefined -g -I./code ./bin_asan/lib/libcjson.a
```

## ASan report
```
$ ./driver-API-12c4bf1-cJSON_PrintBuffered-stack_overflow
AddressSanitizer:DEADLYSIGNAL
=================================================================
==1136697==ERROR: AddressSanitizer: stack-overflow on address 0x7ffee5d11fc8 (pc 0x7f76a7e599a8 bp 0x7ffee5d12050 sp 0x7ffee5d11fd0 T0)
    #0 0x7f76a7e599a8 in __mpn_mul stdlib/mul.c:81:12
    #1 0x7f76a7e614a7 in __printf_fp_buffer_1 stdio-common/printf_fp.c:375:10
    #2 0x7f76a7e6335b in __printf_fp_l_buffer stdio-common/printf_fp.c:1122:7
    #3 0x7f76a7e6a6c4 in __printf_fp_spec stdio-common/vfprintf-internal.c:266:5
    #4 0x7f76a7e6a6c4 in __printf_buffer stdio-common/vfprintf-internal.c:999:6
    #5 0x7f76a7e88a4d in __vsprintf_internal libio/iovsprintf.c:62:3
    #6 0x7f76a7e88a4d in vsprintf libio/iovsprintf.c:76:10
    #7 0x5590c7599fd5 in vsprintf (/home/yuwei/afgen/afgenllm/database/cjson/latest/ckg/driver-API-12c4bf1-cJSON_PrintBuffered-stack_overflow+0x56fd5) (BuildId: 6b3fd69155a6dfcd974aad4759a177491a2fd93a)
    #8 0x5590c759b2be in sprintf (/home/yuwei/afgen/afgenllm/database/cjson/latest/ckg/driver-API-12c4bf1-cJSON_PrintBuffered-stack_overflow+0x582be) (BuildId: 6b3fd69155a6dfcd974aad4759a177491a2fd93a)
    #9 0x5590c766295b in print_number /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:591:18
    #10 0x5590c7654107 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1430:20
    #11 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #12 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #13 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #14 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #15 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #16 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #17 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #18 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #19 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #20 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #21 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #22 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #23 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #24 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #25 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #26 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #27 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #28 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #29 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #30 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #31 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #32 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #33 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #34 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #35 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #36 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #37 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #38 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #39 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #40 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #41 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #42 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #43 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #44 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #45 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #46 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #47 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #48 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #49 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #50 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #51 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #52 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #53 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #54 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #55 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #56 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #57 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #58 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #59 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #60 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #61 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #62 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #63 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #64 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #65 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #66 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #67 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #68 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #69 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #70 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #71 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #72 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #73 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #74 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #75 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #76 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #77 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #78 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #79 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #80 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #81 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #82 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #83 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #84 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #85 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #86 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #87 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #88 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #89 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #90 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #91 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #92 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #93 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #94 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #95 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #96 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #97 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #98 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #99 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #100 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #101 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #102 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #103 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #104 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #105 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #106 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #107 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #108 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #109 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #110 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #111 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #112 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #113 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #114 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #115 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #116 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #117 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #118 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #119 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #120 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #121 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #122 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #123 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #124 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #125 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #126 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #127 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #128 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #129 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #130 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #131 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #132 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #133 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #134 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #135 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #136 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #137 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #138 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #139 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #140 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #141 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #142 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #143 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #144 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #145 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #146 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #147 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #148 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #149 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #150 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #151 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #152 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #153 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #154 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #155 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #156 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #157 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #158 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #159 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #160 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #161 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #162 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #163 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #164 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #165 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #166 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #167 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #168 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #169 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #170 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #171 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #172 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #173 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #174 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #175 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #176 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #177 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #178 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #179 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #180 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #181 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #182 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #183 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #184 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #185 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #186 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #187 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #188 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #189 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #190 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #191 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #192 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #193 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #194 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #195 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #196 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #197 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #198 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #199 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #200 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #201 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #202 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #203 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #204 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #205 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #206 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #207 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #208 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #209 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #210 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #211 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #212 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #213 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #214 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #215 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #216 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #217 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #218 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #219 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #220 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #221 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #222 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #223 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #224 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #225 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #226 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #227 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #228 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #229 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #230 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #231 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #232 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #233 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #234 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #235 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #236 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #237 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #238 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #239 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #240 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #241 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #242 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #243 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #244 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #245 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #246 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20
    #247 0x5590c7663095 in print_array /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1588:14
    #248 0x5590c7654292 in print_value /home/yuwei/afgen/afgenllm/database/cjson/latest/code/cJSON.c:1454:20

SUMMARY: AddressSanitizer: stack-overflow stdlib/mul.c:81:12 in __mpn_mul
==1136697==ABORTING
```