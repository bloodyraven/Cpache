#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(/*int argc, char ** argv*/ void)
{
	int pid;
	int client;
	int tmp;
	char buffer[256];
	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur \n";
	int serveur = creer_serveur(8080);


	initialiser_signaux();

	while(1) {
		if((client = accept(serveur, NULL, NULL)) == -1)
			perror("accept");
		printf("Client connecté\n");
		sleep(2);
		if(write(client, message_bienvenue, strlen(message_bienvenue)) == -1)
			perror("write");
		pid = fork();
		if(pid == -1) {
			perror("fork");
		} else {
			if(pid != 0) {
				if(close(client) == -1)
					perror("close");
			} else {
 				while((tmp = read(client, buffer, sizeof(buffer)))) {

 					if(write(client,buffer,strlen(buffer)) == -1 )
 						perror("write");
				}
 			
 				if(tmp == -1)
 					perror("read");

				printf("Client deconnecté\n");
			}
 		}
	}
	return 0;
}

