#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(/*int argc, char ** argv*/ void)
{
	int client;
	int serveur = creer_serveur(8080);
	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur \n";

	/* Arnold Robbins in the LJ of February ’95 , describing RCS
	if ( argc > 1 && strcmp ( argv [1] , "-advice" ) == 0) {
		printf ("Don't Panic !\n");
	return 42;
	}
	printf ("Need an advice ?\n");
	return 0;*/


	while(1) {
		client = accept(serveur, NULL, NULL );
		if (client == -1)
			perror("accept");
		sleep(1000);
		write(client, message_bienvenue, strlen(message_bienvenue));
	}
	return 0;
}

