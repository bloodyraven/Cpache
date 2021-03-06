#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "socket.h"

int main(/*int argc, char ** argv*/ void)
{
	int pid;
	int client;
	char buffer[256];
	//char get[256];
 	//char path[512];
 	http_request request;
	int serveur = creer_serveur(8080);
	FILE * file = NULL;

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
				file = fdopen(client,"w+");
	 			fgets_or_exit(buffer, sizeof(buffer), file);
				skip_headers(file);
				if(parse_http_request(buffer, &request) == -1) {
					fprintf(file,"HTTP/1.1 404 Not found\r\nConnection: close\r\nContent-Length: 16\r\n");
				} else {
					fprintf(file,"HTTP/1.1 200 Good Request\r\nConnection: close\r\nContent-Length: 17\r\n");
					fprintf(file,"Bienvenue sur Cpache ! \n");
				}
			}
				printf("Client deconnecté\n");
				fclose(file);
		
 		}
	}
	return 0;
}

