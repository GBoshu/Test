#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main( int argc, char *argv[] ) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n, status;
    char *pIP, *pPort;
    struct addrinfo hints, *res , *p;

    if (argc != 3) {
        fprintf(stderr, "usage: listen ip port\n");
        exit(1);
    }

    pIP = argv[1];
    pPort = argv[2];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(pIP, pPort, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s", gai_strerror(status));
        exit(1);
    }
   
    /* First call to socket() function */
    sockfd = socket(res->ai_family, res->ai_socktype, 0);
   
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(pPort);
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(pIP);
    serv_addr.sin_port = htons(portno);
   

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
      
    /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd,5);

    printf("listened on %s:%s\n", pIP, pPort);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    /* If connection is established then start communicating */
    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("Here is the message: %s\n",buffer);

    /* Write a response to the client */
    n = write(newsockfd,"I got your message",18);

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
      
    return 0;
}

