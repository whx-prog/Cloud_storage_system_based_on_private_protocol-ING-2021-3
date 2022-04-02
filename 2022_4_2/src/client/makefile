#out:=../bin/a.out
out:=../../bin/client/client
srcs:=$(wildcard *.c)
objs:=$(srcs:%.c=%.o)
$(out):$(objs)
	gcc $(objs) -o $@ -pthread -L /usr/include/mysql  -Wall -lmysqlclient -L ../../lib/ -lcjson -lcrypto 
%.o:%.c
	gcc -c $^ -o $@	-g -I ../../include/client
rebuild:clean a.out
clean:
	$(RM) $(objs) $(out)

