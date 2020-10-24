#!/bin/bash
gcc server.c ../common/common.c ../common/cJSON.c ../common/get_cjson_value.c -I ../common/ -D _D
