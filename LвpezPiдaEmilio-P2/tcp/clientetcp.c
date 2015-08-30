#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define TAMBUFFER 256

int main(int argc, char *argv[])
{
	int ID_socket_cli, result;
	struct sockaddr_in address_ser, address_cli;
	struct hostent *h;
	char mensaje[] = "Esto es un mensaje al mundo.";

	if(argc<2) 
	{
		printf("uso : %s servidor \n", argv[0]);
		exit(1);
	}


	ID_socket_cli=socket(AF_INET, SOCK_STREAM,0);
	if (ID_socket_cli==-1)
	{
		printf("error\n");
		exit(-1);
	}
   

	address_cli.sin_family = AF_INET;
	address_cli.sin_addr.s_addr = htonl(INADDR_ANY);
	address_cli.sin_port = htons(0);

	result = bind(ID_socket_cli, (struct sockaddr *) &address_cli, sizeof(address_cli)); 
	
	if (result<0) 
	{
		printf("error\n");
		exit(-1);
   	}
   
	h = gethostbyname(argv[1]);
	printf("Escribe mensaje para %s\n",inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));
	address_ser.sin_family = h->h_addrtype;
	memcpy((char *) &address_ser.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	address_ser.sin_port = htons(6001);

	result = connect(ID_socket_cli, (struct sockaddr *) &address_ser, sizeof(address_ser));
	if (result<0) 
	{
		printf("error\n");
		exit(-1);
	}
   
	//manda hola
	strcpy(mensaje,"");
	strcpy(mensaje,"HELLO");
	result = send(ID_socket_cli, mensaje, strlen(mensaje)+1, 0);
      
	if (result<0) 
	{
        	printf("error\n");
        	exit(-1);
      	}
	
	//recibe hola
	result=recv(ID_socket_cli,mensaje,TAMBUFFER,0);
	if(result<0)
	{
		printf("error");
		exit(-1);
	}
		printf("%s\n",mensaje);

	do
	{
	//introduce opción
		printf("Introduzca el código de la butaca que desea ocupar\nPLA-Platea\nPI1-Piso1\nPI2-Piso2\nPI3-Piso3\nGAL-Gallinero:\n ");
		gets(mensaje);
	
		//envia opcion
		result=send(ID_socket_cli,mensaje,strlen(mensaje)+1,0);
		if(result<0)
		{
			printf("error\n");
			exit(-1);
		}
	
		//recibe opción
		result=recv(ID_socket_cli,mensaje,TAMBUFFER,0);
		
		if (result<0) {
        		 printf("error\n");
        		 exit(-1);
      		}
		puts(mensaje);
		printf("\nPresione Intro");
		getchar();
		system("clear");
   	}
   	while(strncmp(mensaje,"FIN",3)!=0);
   	close(ID_socket_cli);

   return 0;

}

