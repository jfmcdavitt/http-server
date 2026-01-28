#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

/* Error Reporting */
void error(char *msg)
{
  perror(msg);
  exit(1);
}

/* Main function for main server functionality */
int main(int argc, char *argv[]) 
{
  /* 
  *  -- Declaring -- 
  *
  *  sockfd, newsockfd: the file descriptors for the sockets
  *  portno: the port number for the server 
  *  clilen: The size of the address of the client
  *  n: Return value for read and write system calls
  *
  */
  int sockfd, newsockfd, portno, clilen, n;
  
  /* Buffer to store read messages from the client */
  char buffer[256];

  /* Structure for storing internet addresses */ 
  struct sockaddr_in serv_addr, cli_addr;
  
  /* Make sure a port is provided when setting up the server */
  if (argc < 2)
  {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  /* Set up Server Socket File Descriptor */ 
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  /* If the value returned is less than 0, there was an error */
  if (sockfd < 0) {
    error("ERROR opening socket\n");
  }
  
  /* Initialize the server address struct to zeros */ 
  bzero((char *) &serv_addr, sizeof(serv_addr));

  /* get the port number supplied by the command line */ 
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;

  serv_addr.sin_port = htons(portno);

  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
  {
    error("ERROR on binding\n");
  }

  listen(sockfd, 5);

  clilen = sizeof(cli_addr);
  
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  
  if (newsockfd < 0) {
    error("ERROR on accept\n");
  }

  bzero(buffer, 256);

  n = read(newsockfd, buffer, 255);

  if (n < 0) 
  {
     error("ERROR reading from socket\n");
  }

  printf("Here is the message\n---\n%s\n---\n", buffer);

  n = write(newsockfd, "I got your message", 18);
  if (n < 0) {
    error("ERROR writing to socket\n");
  }

  return 0;
}
