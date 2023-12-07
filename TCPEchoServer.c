

#include <stdio.h>	   /* for printf() and fprintf() */
#include <sys/socket.h>/* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>	   /* for atoi */
#include <string.h>	   /* for memset() */
#include <unistd.h>	   /* for close() */

#define MAXPENDING 5   /* Maximum outstanding connection requests */
#define RCVBUFSIZE 32  /* Size of receive buffer */
#define PORT 18650	   /* Port Number */

void DieWithError(char *errorMessage); /* Error handling function */
void HandleTCPClient(int clntSocket);  /* TCP client handling function */

int main()
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    int port;			             /* Server port */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure */

    /* Create socket for incoming connections */
    if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        DieWithError("socket() failed");
    }

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(PORT);      	      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed1");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (;;) /* Run forever */
    {
        printf("Waiting for Client to connect....\n");
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");

        /* clntSock is connected to a client! */
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        /* HandleTCPClient() receives the echo message and echoes it back to the client*/
        HandleTCPClient(clntSock);
        exit(1);
    }
}


void DieWithError(char * errorMessage)	/* Error handling function */
{
    perror(errorMessage);
    exit(1);
}


void HandleTCPClient(int clntSocket)
{
    int done = 0;
    char echoBuffer[RCVBUFSIZE];       /* Buffer for echo string */
    int recvMsgSize;                   /* Size of received message */

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0) /* recv() returns a positive value (indicating that something was received)*/
        DieWithError("recv() failed1");

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0 && done==0)      /* zero indicates end of transmission */
    {
        char returnBuffer[recvMsgSize];
        int j=recvMsgSize;
        int i=0;

        /* The return Buffer should have a reversed messages for the original messages received from Client:
        So that, the first arrary of returnBuffer[] has a value from the end arrary of echoBuffer[], you should use for-statement.
        */
        for(i=0 ; i<recvMsgSize ; i++)
        {
            returnBuffer[recvMsgSize - i - 1] = echoBuffer[i];

        }
        /* The server program should be terminated if it gets “end” message from a client
        after it replies “dne” message to the client; you should use "done=1" variable and send the reversed message back to client.
        */
        if(recvMsgSize == 3)
        {
            printf("dne");
            done = 1;
            for(int i = 0; i < recvMsgSize; i++){
                printf("%d", returnBuffer[i]);
            }
        }
        else
        {
            /* Echo message back to client */
            if (send(clntSocket, returnBuffer, recvMsgSize, 0) != recvMsgSize)
                DieWithError("send() failed");

            /* See if there is more data to receive */
            if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
                DieWithError("recv() failed2");
        }
    }

    close(clntSocket);   /* Close client socket */
    exit(1);
}
