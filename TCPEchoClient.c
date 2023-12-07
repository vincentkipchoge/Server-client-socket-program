
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h>	/* for host name*/

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    char *servIP;                    /* Server IP address (dotted quad) */
    int echoServPort;                /* Echo server port */
    char echoString[RCVBUFSIZE];     /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */
    int done = 0;

    if (argc != 3)    /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server IP> <Port Number>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];             /* Second arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Thred arg: server port number */



    /* Create a reliable, stream socket using TCP */
    /* (1) => Fill out the blank if() */
    if (((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)){
        DieWithError("socket() failed");
    }


    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */

    /* If argv is numbers(0~9) go to echoServAddr, else go to struct hostent;
    in this case, you need to have a if condition for checking the first input argument value should be within 0~9 numbers
    so that is for checking Server IP address*/
    /* (2) => Fill out the blank if() */
    if(strlen((const char *) argv) > 9)	/* If argv is numbers(0~9) go to echoServAddr, else go to struct hostent */
    {
        echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
    }
    else
    {
        struct hostent *hname;
        if((hname=gethostbyname(argv[1])) == NULL)
        {
            printf("gethostbyname failed");
            exit(1);
        }
        echoServAddr.sin_addr = *((struct in_addr*)hname->h_addr); /* Server name of machine*/
    }
    echoServAddr.sin_port     = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    /* (3) => Fill out the blank if() */
    if (connect(sock, (struct  sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
        DieWithError("connect() failed");
    }


    while (done == 0)
    {
        scanf("%s", echoString);
        echoStringLen = strlen(echoString); /* Determine input length */

        /* Determine input length; if EchoClient sents "end" to server, first get the input-data "end" using scanf(),
        second change the varialbe "done" to make no more while-loop but in this case you need to use echoString[]
        for matching input string "end" */
        if(echoStringLen == 3)
        {
            scanf("%s", echoString);
            done = 1;
        }
        /* Send the string to the server */
        if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
            DieWithError("send() sent a different number of bytes than expected");

        /* Receive the same string back from the server */
        totalBytesRcvd = 0;
        printf("Received: ");                /* Setup to print the echoed string */
        while (totalBytesRcvd < echoStringLen)
        {
            /* Receive up to the buffer size (minus 1 to leave space for
               a null terminator) bytes from the sender */
            if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
                DieWithError("recv() failed or connection closed prematurely");
            totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
            echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
            printf("%s", echoBuffer);            /* Print the echo buffer */
        }

        printf("\n");    /* Print a final linefeed */

    }
    close(sock);
    exit(0);
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
