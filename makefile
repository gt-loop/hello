
VPATH = ./include:./src:./src/comm/:./src/other:./src/ptc/gat1400:./src/net
objs=main.o file.o global.o cJSON.o object.o  _http.o digest.o base64.o

 pu:$(objs)
	g++ -g -o pu $(objs) -Iinclude  -lzlog  -L ./lib -lghttp -lpthread

main.o:global.hpp object.hpp
file.o:zlog.h
global.o:zlog.h global.hpp
cJSON.o:cJSON.h
object.o:cJSON.h zlog.h global.hpp
_http.o:ghttp.h zlog.h digest.h
digest.o:digest.h
base64.o:b64.h

.PHONY:clean
clean:
	rm  $(objs)
