#include <xinu.h>
#include <custom.h>

/*
	Exercise4 of Lab1
	Design a programm to send "Hello world" to main process and let main
	process echo that message.
 */

char* msg = "Hello world!";

void E4L1_client(){
	kprintf("Client: Ready to receive msg.\n");
	char c = 1;
	while(c){
		c = (char)receive();
		kprintf("%c", c);
	}
	kprintf("Client: Finished.\n");
}

void E4L1_server(pid32 pid, char* msg){
	kprintf("Server: Ready to send msg \"%s\"\n",msg);
	int len = strlen(msg);
	int i = 0;
	for (i = 0; i < len + 1; i++){ 
		// send string including the following '\0'
		while(send(pid, (umsg32)(msg[i])) == SYSERR)
			sleep(1);
	}
	kprintf("Server: Finished.\n");
	return;
}

void E4L1(){
	kprintf("Main Process: Starting Server...\n");
	resume(create(E4L1_server, 4096, 60, "msg server", 2, getpid(), msg));
	kprintf("Main Process: Starting Client...\n");
	E4L1_client();
	kprintf("Main Process: Done!\n");
}
