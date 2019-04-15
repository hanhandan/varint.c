#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include<sys/wait.h>
#include <string.h>
#include "varint.h"

char * encode_test() {
  unsigned long long number = 23113293191329;
  char *str = calloc(7, sizeof(char));
  unsigned char bts;

  varint_encode(number, str, 7, &bts);
  return str;
}
/********************************************************************* 
*filename: tcpserver.c 
*purpose:tcp服务端程序 
********************************************************************/
int main(int argc, char ** argv) 
{
    int sockfd,new_fd;          /* 监听socket: sock_fd,数据传输socket: new_fd */ 
    struct sockaddr_in my_addr; /* 本机地址信息 */ 
    struct sockaddr_in their_addr; /* 客户地址信息 */ 
    unsigned int sin_size, myport, lisnum; 
 
    if(argv[1])  
        myport = atoi(argv[1]); 
    else 
        myport = 8800; 
 
    if(argv[2])  
        lisnum = atoi(argv[2]); 
    else 
        lisnum = 2; 
 
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("socket"); 
        exit(1); 
    } 
    printf("socket %d ok \n",myport);

    my_addr.sin_family=PF_INET; 
    my_addr.sin_port=htons(myport); 
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(my_addr.sin_zero), 0); 
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) { 
        perror("bind"); 
        exit(1); 
    } 
    
    printf("bind ok \n");
 
    if (listen(sockfd, lisnum) == -1) { 
        perror("listen"); 
        exit(1); 
    }
    printf("listen ok \n");
 
    sin_size = sizeof(struct sockaddr_in); 
    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) { 
        perror("accept"); 
        exit(0); 
    } 
    printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));

    {
        char  szSnd[63] = {0};
        sprintf(szSnd,"i am server\n");
        char * pStr = encode_test();
        if (send(new_fd, pStr, 7, 0) == -1) { 
            perror("send"); 
            
            if (pStr) free(pStr);
            close(new_fd); 
        }

        printf("send msg: %s \n",szSnd);
        if (pStr) free(pStr);

    }
    exit(0);
}

