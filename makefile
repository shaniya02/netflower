client.out: bclient.o cloginRegist.o dhomepage.o ecusvip.o\
 			fworker.o groot.o
	gcc *.o -o $@ -lpthread
%.o:%.c
	gcc -c $< -o $@
clean:
	rm *.o client.out