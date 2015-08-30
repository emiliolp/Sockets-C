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

int main()
{
	int ID_socket_ser, ID_socket_ser2, result, long_cli;
   	struct sockaddr_in address_ser, address_cli;
   	char mensaje[TAMBUFFER];
   	int PLA=100,PI1=30,PI2=30,PI3=30,GAL=50;

  
   	ID_socket_ser=socket(AF_INET, SOCK_STREAM,0);
   	if (ID_socket_ser==-1)
   	{
   	   printf("error\n");
   	   exit(-1);
   	}
   
  	address_ser.sin_family = AF_INET;
   	address_ser.sin_addr.s_addr = htonl(INADDR_ANY);
   	address_ser.sin_port = htons(6001);

   	result = bind(ID_socket_ser, (struct sockaddr *) &address_ser, sizeof(address_ser));
   	if (result<0) 
	{
      		printf("error\n");
      		exit(-1);
   	}
   
	listen(ID_socket_ser,2);
	long_cli = sizeof(address_cli);
   
	while(1)
	{
		ID_socket_ser2 = accept(ID_socket_ser, (struct sockaddr *) &address_cli, &long_cli);
   		if (ID_socket_ser2<0) 
		{
      			printf("Error\n");
      			exit(-1);
   		}
   
		memset(mensaje,0x0,TAMBUFFER);
		system("clear");
		//recibe mensaje
		do
		{
	   		result = recv(ID_socket_ser2, mensaje, TAMBUFFER, 0);
	  		if (result<0) 
			{
		   		printf("error\n");
		   		exit(-1);
	  		}
		
			//recibe opcion
			if(strcmp(mensaje,"FIN")==0){}
			else if(strcmp(mensaje,"HELLO")==0){}
			else if(strcmp(mensaje,"PLA")==0)
			{
				if(PLA>0)
				{
					sprintf(mensaje,"PLA %d",PLA);	
					PLA=PLA-1;
				}
				else
					puts("No hay asientos disponibles en PLA");
			}
			else if(strcmp(mensaje,"PI1")==0)
			{
				if(PI1>0)
				{
					sprintf(mensaje,"PI1 %d",PI1);
					PI1=PI1-1;
				}
				else
					puts("No hay asientos disponibles en PI1");
			}
			else if(strcmp(mensaje,"PI2")==0)
			{
				if(PI2>0)
				{
					sprintf(mensaje,"PI2 %d",PI2);
					PI2=PI2-1;
				}
				else
					puts("No hay asientos disponibles en PI2");
			}
			else if(strcmp(mensaje,"PI3")==0)
			{
				if(PI3>0)
				{
					sprintf(mensaje,"PI3 %d",PI3);
					PI3=PI3-1;
				}
				else
					puts("No hay asientos disponibles en PI3");
			}	
			else if(strcmp(mensaje,"GAL")==0)
			{
				if(GAL>0)
				{
					sprintf(mensaje,"GAL %d",GAL);
					GAL=GAL-1;
				}
				else
					puts("No hay asientos disponibles en GAL");
			}
			else if(strcmp(mensaje,"DIS")==0)
			{
				sprintf(mensaje,"DIS 0:%d; 1:%d; 3:%d; 4:%d",PLA,PI1,PI2,PI3,GAL);
			}
			else
		 
				sprintf(mensaje,"%s ERROR",mensaje);
		
			puts(mensaje);
			//envia butaca asignada
			result=send(ID_socket_ser2,mensaje,strlen(mensaje)+1,0);
			if(result<0)
			{
				printf("error");
				exit(-1);
			}
		}
		while(strcmp(mensaje,"FIN")!=0);
		close(ID_socket_ser2);
		puts("Esperando nuevo cliente...");
   	}
   	close(ID_socket_ser);

   	return 0;
}

