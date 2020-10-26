#!/bin/bash
gcc server.c ../common/common.c ../common/cJSON.c ../common/get_cjson_value.c ../common/udp_epoll.c ../common/sub_reactor.c ../common/thread_pool.c ../common/heart_beat.c -I ../common/ -lpthread -o Server -D _D
