#out:=../bin/a.out
out:=../../bin/sever/sever
srcs:=$(wildcard *.c)
objs:=$(srcs:%.c=%.o)
$(out):$(objs)
	gcc $(objs) -o $@ -pthread -Wall -I /usr/include/mysql/ -lmysqlclient -lcrypto

%.o:%.c
	gcc -c $^ -o $@	-g -Wall -I ../../include/sever/
rebuild:clean a.out
clean:
	$(RM) $(objs) $(out)

