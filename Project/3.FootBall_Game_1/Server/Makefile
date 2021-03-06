all = ./src/server.o ./src/common.o ./src/cJSON.o ./src/get_cjson_value.o ./src/udp_epoll.o ./src/sub_reactor.o ./src/thread_pool.o \
	./src/heart_beat.o ./src/game_ui.o ./src/server_exit.o ./src/server_send_all.o ./src/ball_status.o \
	./src/can_access.o ./src/server_re_draw.o ./src/create_spirit.o ./src/show_data_stream.o ./src/court_draw.o

CFLAGS= -lpthread -lncursesw -lm -Wall -Werror -I ../include/

SRC=./src/
vpath %.h ../include/
vpath %.c ./serverFile/
vpath %.c ../common/


SERVER:$(all)
	gcc -o $@ $(all) $(CFLAGS)


$(SRC)%.o: %.c head.h
	gcc -c -I ./ -o $@ $< $(CFLAGS) 

.PHONY:clean
clean:
	rm $(all)
	rm SERVER

