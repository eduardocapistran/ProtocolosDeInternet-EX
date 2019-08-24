#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PUERTO 50000  /* Puerto donde reside nuestro servidor daytime */
#define TAMBUF 32     /* Tamaño del buffer de reception de datos */

int main(void) {
		int s, n, m; /* Socket, numero de bytes recibidos, bytes rellenados en sa por rcvfrom */
		struct sockaddr_in sa; /* Informacion de la connexion */
		char buf[TAMBUF];     /* Buffer de recepcion de datos */
		time_t t;             /* Fecha actual en segundos */

		if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
				perror("Socket");
				exit(1);
		}
		sa.sin_family = AF_INET;
		sa.sin_port = htons(PUERTO);
		sa.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
				perror("Bind");
				exit(1);
		}

		do {
				if ((n = recvfrom(s, buf, TAMBUF, 0, &sa, &m)) < 0) {
						perror("recvfrom");
						exit(1);
				}

				t = time(NULL);
				n = sprintf(buf, "%s", ctime(&t));

				if (sendto(s, buf, n, 0, (struct sockaddr *)&sa, m) != n) {
						perror("sendto");
						exit(1);
				}
		} while(1);
}

