/* Simple program to demonstrate a basic UDP client.
 * Repeatedly asks for words from stdin, and sends each word to the server.
 * If a response is received, then print out the length of the word.
 *
 * Compile using "cc -o wordlen-UDPclient wordlen-UDPclient.c"
 */

/* Include files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Manifest constants */
#define MAX_BUFFER_SIZE 40

// Configure IP address and port of server here
/* #define SERVER_IP "127.0.0.1"   /* loopback interface */
#define SERVER_IP "192.168.1.71"  /* csx.cpsc.ucalgary.ca */
#define SERVER_PORT 50   /* must match the server's actual port!! */

int main(void)
  {
    struct sockaddr_in si_server;
    struct sockaddr *server;
    int s, i, len = sizeof(si_server);
    char buf[MAX_BUFFER_SIZE];
    int readBytes;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not set up a socket!\n");
	return 1;
      }

    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(SERVER_PORT);
    server = (struct sockaddr *) &si_server;

    if (inet_pton(AF_INET, SERVER_IP, &si_server.sin_addr)==0)
      {
	printf("inet_pton() failed\n");
	return 1;
      }

    fprintf(stderr, "Welcome! I am the UDP version of the word length client!!\n");

    /* loop until the user enters "bye" */
    for( ; ; )
      {
	bzero(buf, MAX_BUFFER_SIZE);

 	printf("Enter a word to send to the server (or \"bye\" to exit): ");
	fgets(buf, MAX_BUFFER_SIZE, stdin);

	if(strncmp(buf, "bye", 3) == 0)
	  break;

	if (sendto(s, buf, strlen(buf), 0, server, sizeof(si_server)) == -1)
	  {
	    printf("sendto failed\n");
	    return 1;
	  }

	if ((readBytes=recvfrom(s, buf, MAX_BUFFER_SIZE, 0, server, &len))==-1)
	  {
	    printf("Read error!\n");
	    return -1;
	  }
	buf[readBytes] = '\0';    // proper null-termination of string

	printf("Server Response: %s\n", buf);
      }
    close(s);
    return 0;
  }
