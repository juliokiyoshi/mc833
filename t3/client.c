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

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    char error[MAXLINE + 1];
    struct sockaddr_in servaddr;
    struct sockaddr_in bound_address;
    struct hostent *host_address;
    if (argc != 2) {
        strcpy(error,"uso: ");
        strcat(error,argv[0]);
        strcat(error," ");
        perror(error);
        exit(1);
    }

    host_address = gethostbyname(argv[1]); // pega o ip_address quando é passando como argumento
    struct in_addr **address_list = (struct in_addr **)host_address->h_addr_list;
    printf("Server IP: %s\n", inet_ntoa(*address_list[0]));
    bzero(&bound_address, sizeof(bound_address));// irá guardar os dados do servidor
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    servaddr.sin_addr = *address_list[0]; // seta o ip em que está o cliente
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // cria o socket
        perror("socket error");
        exit(1);
    }
 
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { // se connecta ao servidor
        perror("connect error");
        exit(1);
    }
    // printa o ip'do servidor em que está rodando e o local port em que está ouvindo 
        printf("Connected to server of IP: %s, on local port: %d\n", 
           inet_ntoa(*address_list[0]),
           ntohs(bound_address.sin_port));
    
    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) { // le o que recebeu do servidor
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF) { // printa o que recebeu do servidor
            perror("fputs error");
            exit(1);
        }
    }
    if (n < 0) {
        perror("read error");
        exit(1);
    }
    exit(0);
}