/*Practica 1-Redes
Emilio López Piña 15456963-C

Fichero cliente
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 100

void main(int argc, char *argv[])
{	
	system("clear");
	char direccion[N];
	int tiempo;
	if(argc<3)
	{
		printf("NOTA: No ha introducido valores de direccion ip y tiempo de espera, por defecto estos valores seran 127.0.0.1 y 5 segundos\n");
		strcpy(direccion,"127.0.0.1");
		tiempo=5;
	}
	else
	{
		strcpy(direccion,argv[1]);
		tiempo=atoi(argv[2]);
	}

	/*Descriptor del socket*/
	int Socket_Cliente;
	char Datos[N];
	int contador=0;
	fd_set readfds;

	printf("------------------CLIENTE------------------\n");
	/*Información servidor*/
	struct sockaddr_in Servidor;
	socklen_t Longitud_Servidor;

	/*Abrir socket*/
	Socket_Cliente=socket(AF_INET,SOCK_DGRAM,0);
	if (Socket_Cliente == -1)
	{
		printf ("No se puede abrir el socket cliente\n");
    		exit (-1);	
	}

	/*Rellenar estructura sockaddr_in*/
	Servidor.sin_family=AF_INET;
	Servidor.sin_port=htons(3000);
	Servidor.sin_addr.s_addr=inet_addr(direccion);
	
	Longitud_Servidor=sizeof(Servidor);

	/*mensaje mandar a servidor*/
	do
	{ 
		printf("Introduce variable DAY, TIME O DAYTIME: \n");
		scanf("%s",Datos);
	}
	while(strcmp(Datos,"DAY")!=0 && strcmp(Datos,"TIME")!=0 && strcmp(Datos,"DAYTIME")!=0);

	/*enviar mensaje a servidor*/
	
	int c=0;
	while(contador<3 && c<=0)
	{
		int enviado=sendto(Socket_Cliente, (char*) Datos, sizeof(Datos), 0,(struct sockaddr*) &Servidor, Longitud_Servidor);
		if(enviado<0)
		{
			printf("Error\n");
			exit(-1);
		}
		else
		{
			struct timeval timeout;
			timeout.tv_sec=tiempo;
			timeout.tv_usec=0;
			FD_ZERO(&readfds);
			FD_SET(Socket_Cliente,&readfds);
			printf("\tEsperando respuesta intento %d...\n",contador+1);
			c=select(FD_SETSIZE,&readfds,(fd_set*)NULL,(fd_set*)NULL,&timeout);/*Espera respuesta del servidor durante 5 segundos*/
		
			if(c==0) /*Si c es 0 no se recibe ninguna respuesta*/
			{
				contador++;
			}
		
			else /*Si c es menor que 0 se ha producido un error*/
			{
				if(c<0)
				{
					printf("Ha habido un error al acceder al servidor\n");
					exit(-1);
				}
			}
		}
	}
	if(contador==3)
	{
		printf("\nSin respuesta del servidor\n");
		exit(-1);
	}

	if(c>0) /*c>0 se ha recibido alguna respuesta*/
	{
		int recibido= recvfrom (Socket_Cliente, (char *) Datos, sizeof(Datos), 0,(struct sockaddr *) &Servidor, &Longitud_Servidor); /*Se recibe información del servidor*/
		if(recibido<0)
		{
			printf("No se puede recibir información del servidor\n");
		}
		else
			printf("\n%s\n",Datos);
	}
			
	close(Socket_Cliente);
}
	
