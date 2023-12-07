# Server-client-socket-program

Reverse Echo Server:
Simple reverse echo server and a client using TCP socket. The server and client run on different machines
<img width="506" alt="Screenshot 2023-12-07 at 4 03 11 PM" src="https://github.com/vincentkipchoge/Server-client-socket-program/assets/100973751/03247812-03a1-4bc7-a312-9c2ce0cc3479">

The reverse echo server receives a message from a client over TCP socket and replies the same message back to the source in reverse order.The server program is terminated if it gets “end” message from a client after it replies “dne” message to the client. Note: Port number used should be over 5000.

A reverse echo server begins by a user at any machine by a command line:
prompt>./my-oche-server



Echo Client:
The echo client gets a message from a user and sends the message to the connected reverse echo server. When the reversed message is arrived from the server, it displays the message to the users.
If a user wants to stop the client program, the user types “end” to the client. The client sends the message to the reverse echo serve,r and waits for the message “dne” from the server. If the client gets the message “dne”, it terminates itself with displaying “dne” message.
The echo client is started by the following command line:
Prompt> ./my-echo-client [IP or name of machine where the my reverse echo server is running] [port-number]
