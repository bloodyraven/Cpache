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
	char buffer[256];
	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur \n";
	int serveur = creer_serveur(8080);
	FILE* file;

	while(1) {
		initialiser_signaux();
		if((client = accept(serveur, NULL, NULL)) == -1)
			perror("accept");
		pid = fork();
		if(pid == -1) {
			perror("fork");
		} else {
			if(pid != 0) {
				if(close(client) == -1)
					perror("close");
			} else {
				printf("Client connecté\n");
				sleep(2);
					if(write(client, message_bienvenue, strlen(message_bienvenue)) == -1)
						perror("write");
				file = fdopen(client,"w+");
	 			while(fgets(buffer, sizeof(buffer), file) != NULL) {
					fprintf(file, "<Cpache> %s",buffer);
				}
				printf("Client deconnecté\n");
				fclose(file);
			}
 		}
	}
	return 0;
}

