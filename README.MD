# WebServ
Webserv, a project from *42 school*, we have to create a serverWeb inspired by NGINX, in  ```C++```

## Socket Programming

### Initialization

Socket programming is a domain in programming, it's specialized in computer interconnection via a network, here we use the specialized library ```socket.h```

First, to initialize a socket i have created a Server class, the constructor initialize the socket with 

```cpp
this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
```
where the parameters are :
1. domain : AF_INET, to use IPV4
2. type : SOCK_STREAM is udp connection type [see udp diagram](https://media.geeksforgeeks.org/wp-content/uploads/20220330131350/StatediagramforserverandclientmodelofSocketdrawio2-448x660.png)

After, we have to set the socket option, it's an optionnal function.

```cpp
setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt, sizeof(this->opt));
```

A structure to save our data and link our ports to the socket is included in ```socket.h```, called ```sockadrr_in``` we have to fill it with our data in order to bind the socket to the network.

```cpp
this->address.sin_family = AF_INET;
this->address.sin_addr.s_addr = INADDR_ANY;
this->address.sin_port = htons(PORT);
```

here we fill it with the type of IP Address we want to utilize, AF_INET for IPV4, INADDR_ANY is to use any IP Address we want, and we provide the port we want too.

Finally we have to bind the socket we created with our port on our computer with the function bind.

```cpp
bind(this->sockfd, (struct sockaddr*)&this->address, sizeof(this->address))
```

Here we are the server is set up now we have to make it listen for new connection and accept new host to communicate with it.

### Listening - Accepting

```cpp
listen(this->sockfd, 5);
```
this function marks the socket referred to by _sockfd_ as a passive
socket, that is, as a socket that will be used to accept incoming
connection.

The second argument is the backlog, it defines the **maximum** length to which the queue of **pending** connection for sockfd may grow.

```cpp
int newSocket = accept(this->sockfd, (struct sockaddr *)&this->address, (socklen_t *)&this->addrLen);
```
This function extract the first connection of the pending queue, and create a newSocket we can manipulate in our code.
If no pending connection are present on the queue, and the socket is not market as non blocking, accept block the caller until a connection is present.
In order to have our server working the accept function have to be non-blocking because we can have host trying to connect with us and in the same time another host communicating with us.

### Non-Blocking socket.
```cpp
fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
```
this function is simple and permit to set sockfd to non-blocking so when accept is waiting for a new connection the server can continue to communicate with client already connected.