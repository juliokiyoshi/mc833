#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#define LISTENQ 10
#define MAXDATASIZE 100



int main (int argc, char **argv) {
   int    listenfd, connfd;
   struct sockaddr_in servaddr; // alocação de espaço para socket
   struct sockaddr_in addr2; 
   struct sockaddr_in clnt_addr; // aloca um socket para obter as informações do cliente
   char   buf[MAXDATASIZE];
    socklen_t clnt_addr_size;
   time_t ticks;


   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family      = AF_INET; //  aceita comunicacao do tipo ipv4
   servaddr.sin_addr.s_addr =htonl(INADDR_ANY); // coloca seu localhost como o endereço do socket
   servaddr.sin_port        = htons(13);   // adiciona a porta  que o socket irá começar a escultar


   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { // chamada para o sistema OP para criar o socket
      perror("socket");
      exit(1);
   }
   if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))< 0) { // bind das definições que setemos nas linhas 24 - 26
      perror("bind");
      exit(1);
   }


   if (listen(listenfd, LISTENQ) == -1) { // setar o socket para começar a escutar na porta setada
      perror("listen");
      exit(1);
   }
  

   for ( ; ; ) {
      if ((connfd = accept(listenfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size)) <0 ) {// aceita uma conexão de um cliente
         perror("accept");
         exit(1);
      }
       
      clnt_addr_size=sizeof(clnt_addr);  
      int clnt_peer_err = getpeername(connfd, (struct sockaddr *)&addr2, &clnt_addr_size); // usando a função para pegar as info do cliente que acessa o server
      fprintf(stderr,"Client socket's ip : %s\n", inet_ntoa(addr2.sin_addr)); // print o end do cliente
      fprintf(stderr,"client socket's port %d\n", ntohs(addr2.sin_port));
      fprintf(stderr,"value of clnt_peer_err  : %d\n",clnt_peer_err);
      ticks = time(NULL);
      snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks)); // printa o que vamos colocar no buffer
      write(connfd, buf, strlen(buf));// escreve o que está no buffer para enviar para o cliente

      close(connfd);
   }
   return(0);
}

