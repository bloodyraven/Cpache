#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int creer_serveur(int port) {
	int socket_serveur;
	int bind_serveur;
	int optval = 1;

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	socket_serveur = socket (AF_INET, SOCK_STREAM, 0);

	if(socket_serveur == -1)
		perror("socket_serveur");

	if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		perror("setsockopt");

	if((bind_serveur = bind(socket_serveur, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) == -1)
		perror("bind socket_serveur");

	if(listen(socket_serveur, 10) == -1)
		perror("Listen socket_serveur");


	return socket_serveur;
}

void traitement_signal(int sig) {
	printf("Signal %d reçu\n", sig);
	waitpid(-1,NULL,WNOHANG);
}

void initialiser_signaux(void) {
	struct sigaction sa;
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD, &sa, NULL) == -1)
		perror("sigaction (SIGCHLD)");
}

int analyse(char buffer[],char *get, char *path,int *versionMaj,int *versionMin) {
	if((sscanf(buffer, "%s %s HTTP/%d.%d", get, path, versionMaj, versionMin))==-1) {
 		perror("scanf");
 		return -1;
 	}
 		
 	if(strcmp(get, "GET")!=0) {
 		return -1;
 	}
 
 	/*if (strcmp(path,"/")!=0){
 		tmp = -1;
 	}*/
 
 	if(*versionMaj == 1) {
		*versionMaj =-1;
 	} else {
 		return -1;
 	}
 
 	if(*versionMin != -1 && (*versionMin == 1 || *versionMin == 0)) {
		*versionMin =-1;
 	} else {
 		return -1;
 	}
 	return 0;
}

void ligneVide(char *str) {	
	while(*str != '\0') {
		if(*str == '\r' || *str == '\n') {
			*str ='\0';
			break;
		}
		str++;
	}
}

