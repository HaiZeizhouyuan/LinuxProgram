#!/bin/bash
gcc ./client.c -I ../common/ ../common/cJSON.c ../common/common.c ../common/get_cjson_value.c -D _D
