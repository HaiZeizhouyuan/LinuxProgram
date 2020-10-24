#!/bin/bash
gcc server.c ../common/common.c ../common/chat.c ../common/thread_pool.test.c ../common/file.c -I ../common/ -lpthread -o server1
