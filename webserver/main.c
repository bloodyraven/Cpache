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
	char get[256];
 	char path[512];
 	int versionMaj = -1, versionMin = -1;
	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur \n";
	int serveur = creer_serveur(8080);
	FILE* file = NULL;

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
				/*ligneVide(buffer);
				fgets(buffer, sizeof(buffer), file);
				printf("%d\n", analyse(buffer, get, path, &versionMaj, &versionMin));*/
	 			while(fgets(buffer, sizeof(buffer), file) != NULL) {
					//fprintf(file, "<Cpache> %s", buffer);
					if(analyse(buffer, get, path, &versionMaj, &versionMin) == -1) {
						fprintf(file,"HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 16\r\n\r\n400 Bad request\n");
					} else {
						fprintf(file,"HTTP/1.1 200 Good Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n200 good request\n");
						fprintf(file,"Bienvenue sur Cpache ! \n");
					}
				}
				printf("Client deconnecté\n");
				fclose(file);
			}
 		}
	}
	return 0;
}

