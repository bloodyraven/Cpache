#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int creer_serveur(int port){
	int socket_serveur;
	int bind_serveur;

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	socket_serveur = socket (AF_INET, SOCK_STREAM, 0);
	if(socket_serveur == -1)
		perror("socket_serveur");

	if((bind_serveur = bind(socket_serveur, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) == -1)
		perror("bind socker_serveur");

	if(listen(socket_serveur, 10) == -1)
		perror("Listen socket_serveur");
	return socket_serveur;
}

int setsockopt(int fd, int SOL_SOCKET, int SO_REUSEADDR, const void *optval, socklen_t option_len) {
	int optval = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		perror("Can not set SO_REUSEADDR option");
}



