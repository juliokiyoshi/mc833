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
#define MAXLINE 4096



// 0=48
// 9=57
char* convertToString(char* result,int item){
   int rest,i=0;
   while(item!=0){
      rest=item%10;
      item=item/10;
      result[i]=rest+48;
      i++;
   }
   return result;
}
void revstr(char *str1)  
{  
    // declare variable  
    int i, len, temp;  
    len = strlen(str1); // use strlen() to get the length of str string  
      
    // use for loop to iterate the string   
    for (i = 0; i < len/2; i++)  
    {  
        // temp variable use to temporary hold the string  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

int main (int argc, char **argv) {
   int    listenfd, connfd;
   struct sockaddr_in servaddr; // alocação de espaço para socket
   struct sockaddr_in addr2; 
   struct sockaddr_in clnt_addr; // aloca um socket para obter as informações do cliente
   char   buf[MAXDATASIZE];
   //char buf2[]="bye";
   int pid,len;
   socklen_t clnt_addr_size;
   time_t ticks;
   struct hostent *host_address;



   host_address = gethostbyname(argv[1]); // pega o ip_address quando é passando como argumento
   struct in_addr **address_list = (struct in_addr **)host_address->h_addr_list;
   int port=atoi(argv[2]);
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family      = AF_INET; //  aceita comunicacao do tipo ipv4
   servaddr.sin_addr        =  *address_list[0];
   servaddr.sin_port        = htons(port);   // adiciona a porta  que o socket irá começar a escultar


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
      len = sizeof(clnt_addr_size);
       
      clnt_addr_size=sizeof(clnt_addr);  
      getpeername(connfd, (struct sockaddr *)&addr2, &clnt_addr_size); // usando a função para pegar as info do cliente que acessa o server
      //fprintf(stderr,"Client socket's ip : %s\n", inet_ntoa(addr2.sin_addr)); // print o end do cliente
      //fprintf(stderr,"client socket's port %d\n", ntohs(addr2.sin_port));
      //fprintf(stderr,"value of clnt_peer_err  : %d\n",clnt_peer_err);
      int a =ntohs(addr2.sin_port);
      char buffer[33];
      FILE* out=fopen("out_server.txt","w");
      fputs("\nPorta que o cliente esta usando eh: ",out);
      fputs(convertToString(buffer,a),out);
      fputs("\nIp do cliente eh este: ",out);
      fputs(inet_ntoa(addr2.sin_addr),out);
      fclose(out);
      ticks = time(NULL);
      snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks)); // printa o que vamos colocar no buffer
      write(connfd, buf, strlen(buf));// escreve o que está no buffer para enviar para o cliente
      //printf("server: received connection\n");
      printf ("\n");
      pid = fork();
      if (pid == 0) {
         //printf("you are in child process\n");
			close(listenfd);
			while (len != recv(connfd, buf, sizeof(buf), 0)) {
           revstr(buf);
           printf("\nstring reversa é : %s\n", buf);
         printf ("\n");

				//fputs(buf, stdout);
            //printf("you are send a BYE FLAG");
				send(connfd, buf, len, 0);
			}
			close(connfd);
			exit(0);
		}
            printf ("\n");

     
      close(connfd);
   }
   return(0);
}

