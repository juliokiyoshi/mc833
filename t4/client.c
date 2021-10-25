#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>  
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXLINE 4096
#define MAXDATASIZE 100

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    char error[MAXLINE + 1];
    struct sockaddr_in servaddr;
    struct sockaddr_in bound_address;
    struct hostent *host_address;

    if (argc != 3) {
        strcpy(error,"uso: ");
        strcat(error,argv[0]);
        strcat(error," ");
        perror(error);
        exit(1);
    }

    host_address = gethostbyname(argv[1]); // pega o ip_address quando é passando como argumento
    int port=atoi(argv[2]); // pega o port
    struct in_addr **address_list = (struct in_addr **)host_address->h_addr_list;
    printf("Client IP ARE: %s\n", inet_ntoa(*address_list[0]));
    printf("Port: %d\n", port);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr = *address_list[0]; // seta o ip em que está o cliente
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // cria o socket
        perror("socket error");
        exit(1);
    }
 
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { // se connecta ao servidor
        perror("connect error");
        exit(1);
    }
    bzero(&bound_address, sizeof(bound_address));// irá guardar os dados do servidor

    // printa o ip'do servidor em que está rodando e o local port em que está ouvindo 
    printf("Connected to server of IP: %s, on local port: %d\n", inet_ntoa(*address_list[0]),ntohs(bound_address.sin_port));
   
    
    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) { // le o que recebeu do servidor
        recvline[n] = 0;
        printf("O comando recebido pelo servidor foi:\n %s\n",recvline);
        if (strcmp(recvline, "bye") == 0) { // printa o que recebeu do servidor
            printf("o comando recebido foi bye então é para desligar a conexão\n");
            close(sockfd);
            exit(0);
        }// copia o recvline para o buffer
        FILE* result;
        result =popen(recvline, "r");
        if(result == NULL){
            printf("ERROR NO popen\n");
        }
        char result_str[MAXLINE];
        char fgets_buffer[MAXLINE];
        memset(result_str, 0, MAXLINE);
        while(fgets(fgets_buffer, MAXLINE, result)!=NULL){
            strcat(result_str, fgets_buffer);
        }
        strcat(result_str,"\0");

        write(sockfd, result_str, strlen(result_str));
        //printf("comando foi enviado com sucesso!\n");
    
        if (n < 0) {
            perror("read error");
            exit(1);
        }
        exit(0);
    }
}