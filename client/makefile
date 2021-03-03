#out:=../bin/a.out
out:=a.out
srcs:=$(wildcard *.c)
objs:=$(srcs:%.c=%.o)
$(out):$(objs)
	gcc $(objs) -o $@ -pthread -I../include
%.o:%.c
	gcc -c $^ -o $@	-g -I../include
rebuild:clean a.out
clean:
	$(RM) $(objs) $(out)

