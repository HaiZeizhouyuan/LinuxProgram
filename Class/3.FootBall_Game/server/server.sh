#!/bin/bash
gcc server.c ../common/common.c ../common/cJSON.c ../common/get_cjson_value.c ../common/udp_epoll.c ../common/sub_reactor.c ../common/thread_pool.c ../common/heart_beat.c ../common/game_ui.c ../common/server_exit.c ../common/server_send_all.c ../common/ball_status.c ../common/can_access.c -I ../common/ -lpthread -lcurses -o Server
