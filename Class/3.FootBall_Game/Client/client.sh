#!/bin/bash
gcc ./client.c -I ../common/ ../common/cJSON.c ../common/common.c ../common/get_cjson_value.c ../common/client_recv.c ../common/client_exit.c ../common/send_chat.c ../common/send_ctl.c  ../common/game_ui.c ../common/show_strength.c -lpthread -lcurses

