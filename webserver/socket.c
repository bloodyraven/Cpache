#include "socket.h"
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
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

void ligneVide(char *str) {	
	while(*str != '\0') {
		if(*str == '\r' || *str == '\n') {
			*str ='\0';
			break;
		}
		str++;
	}
}

char *fgets_or_exit(char *buffer, int size, FILE *file){
 	if(fgets(buffer, size, file) == NULL){
 		exit(1);
 	}
 	return buffer;	
}

int parse_http_request(const char *request_line, http_request *request) {
 	char get[50];

 	if((sscanf(request_line, "%s %s HTTP/%d.%d", get, request->url, &request->major_version, &request->minor_version)) != 4) {
 		perror("scanf");
 		return 0;
 	}

 	if(strcmp(get, "GET")==0) {
 		request->method = HTTP_GET;
 	} else {
 		return 0;
 	}
 		
 	if(strcmp(request->url,"/")! = 0)
 		return 0;
 
 	if (request->major_version != 1) {
 		request->method = HTTP_UNSUPPORTED;
 		return 0;
 	}
 	
 	if (!(request->minor_version != -1 && (request->minor_version == 1 || request->minor_version == 0))) {
 		request->method = HTTP_UNSUPPORTED;
 		return 0;
 	}
 
 	return 1;
 }

void skip_headers (FILE * fichier_client) {
 	char buffer[256];
 	while(fgets_or_exit(buffer, sizeof(buffer), fichier_client)) {
 		ligneVide((char*)buffer);
 		if(strlen(buffer) == 0)
 			break;
 	}
 }

void send_status(FILE * fichier_client, int code, const char * reason_phrase) {
	fprintf(fichier_client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
}
 
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body) {
	send_status(client, code, reason_phrase);
	fprintf(client, "Connection: close\r\nContent-Length: %d\r\n\r\n%s", (int) strlen(message_body), message_body);
}

