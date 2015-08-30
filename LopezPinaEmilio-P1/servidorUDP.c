/* Práctica 1-Redes
Emilio López Piña 15456963-C

Fichero servidor
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <time.h>
#define N 100


main()
{
	system("clear");
	printf("------------------SERVIDOR------------------\n");
	/*descriptor socket y buffer de datos*/
	int Socket_Servidor;
	struct sockaddr_in Servidor;
	char mensaje[N];
	
	/*información cliente*/
	struct sockaddr_in Cliente;
	socklen_t Longitud_Cliente;

	/*abrir el socket*/
	Socket_Servidor=socket(AF_INET,SOCK_DGRAM,0);
	if(Socket_Servidor==-1)
	{
		printf("No se puede abrir el socket servidor\n");
		exit(-1);
	}

	/*rellenar estructura servidor*/
	Servidor.sin_family=AF_INET;
	Servidor.sin_port=htons(3000);
	Servidor.sin_addr.s_addr=INADDR_ANY;

	if(bind(Socket_Servidor,(struct sockaddr*)&Servidor,sizeof(Servidor))==-1)
	{
		close(Socket_Servidor);
		exit(-1);
	}

	Longitud_Cliente=sizeof(Cliente);
	
	/*recibir mensaje*/
	while(1)
	{
		int recibir=recvfrom(Socket_Servidor,mensaje,sizeof(mensaje),0,(struct sockaddr *) &Cliente,&Longitud_Cliente);
		
		if(recibir>=0)
		{
			printf("Conectado con el cliente\n");
			time_t tiempo;
			struct tm *tmPtr;

			tiempo=time(NULL);
			tmPtr=localtime(&tiempo);

			int enviado;

			/*comprobamos si la información es correcta*/
			if(strcmp(mensaje,"DAY")==0)
			{
				strftime(mensaje,100,"%A, %d de %B de %Y",tmPtr);
			}
			
			if(strcmp(mensaje,"TIME")==0)
			{
				strftime(mensaje,100,"%H:%M:%S",tmPtr);
			}

			if(strcmp(mensaje,"DAYTIME")==0)
			{
				strftime(mensaje,100,"%A, %d de %B de %Y; %H:%M:%S",tmPtr);
			}

			enviado=sendto(Socket_Servidor,mensaje,sizeof(mensaje),0,(struct sockaddr *) &Cliente,Longitud_Cliente);
			printf("Esperando petición del cliente...\n");
		}

		else
		{
			printf("No se ha recibido información del cliente\n");
		}
	}		
	close(Socket_Servidor);
}
		
	
	
