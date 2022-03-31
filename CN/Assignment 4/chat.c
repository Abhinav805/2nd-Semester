#include <stdio.h>// input output // printf() scanf()
#include <stdlib.h>// miscellaneous symbolic constants and types
#include "chat.h"
#include <sys/socket.h>// for number of strcutures for socket address
#include <sys/types.h>// number of data type  used in system call
#include <pthread.h>// Mutithreading
#include <unistd.h>// read, write, close
#include <string.h>
#include <arpa/inet.h>// definitions for internet operations

#include<pthread.h>
#define MAXSIZE 256
#define EXITCODE 1
/*
*  Here is the starting point for your netster part.1 definitions. Add the 
*  appropriate comment header as defined in the code formatting guidelines
*/

/* Add function definitions */

// Declaration of the threading function
typedef struct parametersThread
{
    pthread_t *thread_id;
    struct sockaddr_in address;
    int socket;

} parametersThread;

//Defination of the threading function
void *tcp_multithread(void *vparams);

// Multithreading 
void *tcp_multithread(void *vparams)
{
	parametersThread* par;
    
    par = (parametersThread *)vparams;
    int clientsocket = par->socket;
    struct sockaddr_in clie_address = par->address;
    
	// Loop till the break
    while (1)
    {
        char char_buffer[MAXSIZE]; // Making buffer 
        if (recv(clientsocket, char_buffer, MAXSIZE, 0) > 0) // If receives data from a connected socket  
        {
           
            printf("got message from ('%s', %d)\n", inet_ntoa(clie_address.sin_addr), ntohs(clie_address.sin_port));
			
			// hello condition 
            if (strcmp(char_buffer, "hello") == 0)
            {   
                send(clientsocket, "world", 6, 0);// send world to the client
            }// goodbye condition
            else if (strcmp(char_buffer, "goodbye") == 0)
            {    
                send(clientsocket, "farewell", 9, 0);// send the farewell
                close(clientsocket);// close that existing client 
            }//exit
            else if (strcmp(char_buffer, "exit") == 0)
            {   
                send(clientsocket, "ok", 3, 0);//Send ok to the client
                close(clientsocket);// Close the client 
                exit(0);// Close the connection
            }//This the echo part 
            else
            {   
                send(clientsocket, char_buffer, sizeof(char_buffer), 0);// Echo the same to the client
            }
            bzero(char_buffer, sizeof(char_buffer));// Buffer cleared
        }
        else
        {
            break;// Breaking the connection 
        }
    }
    
    return NULL;
}



// For Server
void chat_server(char *iface, long port, int use_udp)
{
    
    int socketserver;
    
    if (use_udp)
    {
		socketserver = socket(AF_INET, SOCK_DGRAM, 0);//udp
	}
    else
    { 
		socketserver = socket(AF_INET, SOCK_STREAM, 0);//tcp
	} 
    if (socketserver < 0)// For error
    {
        perror("Socket failed");
        exit(EXITCODE);
    }
	
	// Creating struct
    struct sockaddr_in sv_addr;
    
    sv_addr.sin_port = htons(port);
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = INADDR_ANY;
    
	// Binding
    if (bind(socketserver, (const struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0)
    {
        perror("Bind Failed\n");
        exit(EXITCODE);
    }

    
    int listentcp;
    switch (use_udp)
    {
        case 0://TCP
            
            listentcp = listen(socketserver, 10);

            if(listentcp < 0)
            {
                perror("Listen Failed");
                exit(EXITCODE);
            }
            
            int count = -1; 
            
            while (1)
            {
                int clientsocket;
                struct sockaddr_in clie_address;// Struct
                socklen_t addr_len = sizeof(clie_address);

               
                clientsocket = accept(socketserver, (struct sockaddr *)&clie_address, &addr_len);

                if (clientsocket < 0)
                {
                    perror("Accept failed\n");
                    exit(EXITCODE);
                }
                count + = 1;
                
                printf("connection %d from ('%s', %d)\n", count, inet_ntoa(clie_address.sin_addr), ntohs(clie_address.sin_port));

                
				parametersThread *par = malloc(sizeof(parametersThread));// malloc
            
				pthread_t *thread_id = malloc(sizeof(pthread_t));

				par->socket = clientsocket;
				
				par->thread_id = thread_id;
				par->address = clie_address;

				pthread_create(thread_id, NULL, tcp_multithread, (void *)par);
            
                
            }
        case 1: // For UDP
            
            count = -1;
            while (1)
            {
                char buffer_cli[MAXSIZE];// buffer 
                struct sockaddr_in clie_address; // struct
                socklen_t address_length = sizeof(clie_address);

                //
                if ((recvfrom(socketserver, buffer_cli, MAXSIZE, 0, (struct sockaddr *)&clie_address, &address_length)))
                {
                    
                    count + =1;
                    printf("got message from ('%s', %d)\n", inet_ntoa(clie_address.sin_addr), ntohs(clie_address.sin_port));
                    
                    //hello condition
                    if (strcmp(buffer_cli, "hello") == 0)
                    {
                     
                        sendto(socketserver, "world", MAXSIZE, 0, (struct sockaddr *)&clie_address, address_length);
                    }// goodbye condition
                    else if (strcmp(buffer_cli, "goodbye") == 0)
                    {
                        sendto(socketserver, "farewell", MAXSIZE, 0, (struct sockaddr *)&clie_address, address_length);
                    }// exit condition
                    else if (strcmp(buffer_cli, "exit") == 0)
                    {
                        sendto(socketserver, "ok", MAXSIZE, 0, (struct sockaddr *)&clie_address, address_length);
                        exit(0);
                    }// For echo
                    else
                    {
                        sendto(socketserver, buffer_cli, MAXSIZE, 0, (struct sockaddr *)&clie_address, address_length);
                    }
                    bzero(buffer_cli, sizeof(buffer_cli));// Buffer cleared
                }
            }
        default:
            break;
    }
}



// For client

void chat_client(char *host, long port, int use_udp)
{
	//for Local host 
    if (strcmp(host, "localhost") == 0)
    {
        host = "127.0.0.1";
    }
    int cli_socket;
    struct sockaddr_in sv_addr; // Struct

    
    if (use_udp)//UDP
    {
		cli_socket = socket(AF_INET, SOCK_DGRAM, 0); //UDP
	}
    else//TCP
    { 
		cli_socket = socket(AF_INET, SOCK_STREAM, 0);// TCP
	}
    if (cli_socket < 0)
    {
        perror("Failure in socket connection");
        exit(EXITCODE);
    }

    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(port);
    sv_addr.sin_addr.s_addr = inet_addr(host);

    

    if (use_udp == 0)
    {
        int conn = connect(cli_socket, (struct sockaddr *)&sv_addr, sizeof(sv_addr));
        if (conn < 0)
        {
            perror("Connection failure");
            exit(EXITCODE);
        }
    }


    switch (use_udp)
    {
        case 0:// For TCP
            while (1)
            {

                char buffer_cli[MAXSIZE]; // creating 
                char server_message[MAXSIZE];
                int inp = scanf("%s", &buffer_cli[0]);
                if (inp)
                {
                    send(cli_socket, buffer_cli, sizeof(buffer_cli), 0);
                    bzero(buffer_cli, sizeof(buffer_cli));
                }
				
				// Receive
                if (recv(cli_socket, server_message, sizeof(server_message), 0))
                {

                    printf("%s\n", server_message);
                    if (strcmp(server_message, "farewell") == 0 || strcmp(server_message, "ok") == 0)
                    {
                        close(cli_socket);// close
                        exit(0);
                    }
                    bzero(server_message, sizeof(server_message));// buffer cleared
                }
                else
                {
                    break;
                }
            }

        case 1: // UDP
            while (1)
            {
                char buffer_cli[MAXSIZE]; // buffer 
                char server_message[MAXSIZE];
                socklen_t addr_len = sizeof(sv_addr);


                int inp = scanf("%s", &buffer_cli[0]);
                if (inp)
                {
                    sendto(cli_socket, buffer_cli, MAXSIZE, 0, (struct sockaddr *)&sv_addr, addr_len);
                    bzero(buffer_cli, sizeof(buffer_cli));// Clear the buffer
                }
                if (recvfrom(cli_socket, server_message, MAXSIZE, 0, (struct sockaddr *)&sv_addr, &addr_len))
                {
                    printf("%s\n", server_message);
                    if (strcmp(server_message, "farewell") == 0 || strcmp(server_message, "ok") == 0)
                    {
                        close(cli_socket);
                        exit(0);// Close the connection 
                    }
                    bzero(server_message, sizeof(server_message));// Clear the 
                }
                else
                {
                    break;
                }
            }
        default:
            break;
    }
}
