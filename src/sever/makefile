#out:=../bin/a.out
out:=a.out
srcs:=$(wildcard *.c)
objs:=$(srcs:%.c=%.o)
$(out):$(objs)
	gcc $(objs) -o $@ -pthread -Wall -I /usr/include/mysql/ -lmysqlclient
%.o:%.c
	gcc -c $^ -o $@	-g -Wall -I../../include/server
rebuild:clean a.out
clean:
	$(RM) $(objs) $(out)

