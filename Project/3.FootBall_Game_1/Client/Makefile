all = ./src/client.o ./src/cJSON.o ./src/common.o ./src/get_cjson_value.o ./src/client_recv.o\
	  ./src/client_exit.o ./src/send_chat.o ./src/send_ctl.o  ./src/game_ui.o ./src/show_data_stream.o\
	  ./src/show_strength.o ./src/parse_spirit.o ./src/get_map_value.o ./src/client_re_draw.o ./src/court_draw.o\
		./src/game_over.o


CFLAGS= -lpthread -lncursesw -lm -I ../include/

SRC=./src/
vpath %.h ../include/
vpath %.c ./clientFile/
vpath %.c ../common/
vpath %.c ../Server/serverFile/

CLIENT:$(all)
		gcc -o $@ $(all) $(CFLAGS)

$(SRC)%.o: %.c head.h
		gcc -c -I ./ -o  $@ $< $(CFLAGS)

.PHONY:clean
clean:
	rm $(all)
	rm CLIENT

