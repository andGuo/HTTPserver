# myHttpServer

An interest project of mine

## Description

A HTTP 0.9 and HTTP 1.0 compliant web server featuring multi-threading with a thread pool. Includes a barebones client to connect to the server.

The main thread listens for client connections and will add them to a task queue. With condition variables, each thread then takes a client connection from the queue, receives the client's HTTP request, parses the necessary info, and responds accordingly. 

The HTTP request is parsed using POSIX regex. The server will send back an HTTP error code and reason phrase if an invalid request is received. Since there is no filesystem, a normal server GET response will include a generated header and static HTML ¯\\\_(ツ)_/¯. 

There is an articifial delay on the thread routine to emaphsize the concurrency of the threads. Feel free to remove it in the source code. Nonetheless, each request takes the server about one second to process. Thus, the formula for how long the server will take to run (assuming the max number of requests is received all at once) is about:

#requests/#threads = #seconds to finish running server

## Getting Started

### Dependencies

* Uses ANSI C and C POSIX libraries - WILL NOT RUN ON WINDOWS.
* Tested on: macOS Monterey (Clang), Ubuntu 20 (GCC)

### Installation and Execution

```
git clone https://github.com/andGuo/myHttpServer.git
cd myHttpServer
make
./server
```
By default, the server creates 10 threads and fulfills 100 requests before closing the socket and cleaning up the process.
You may choose to pass two command-line arguments to the server process, the first integer representing the number of threads, and the second integer representing the number of requests the server will handle. For example, the following creates 100 threads and will handle 1000 requests.
```
./server 100 1000
```
There are a number of ways to connect to a running server. In a seperate terminal type a combination of any of the following:
```
./client

./client simple

./spawnClients.sh [NUMBER OF CLIENT PROCESSES]
```
alternatively, enter http://localhost:8080 into a browser once the server is running.

## Version History

* 0.1
    * Initial Release

## License

This project is licensed under the MIT License - see the LICENSE file for details

## Acknowledgments

* "Beej's Guide to Network Programming" by: Brian “Beej Jorgensen” Hall
* "A Practical Guide to Writing Clients and Servers" by: James Marshall
* "HTTP Server: Everything you need to know to Build a simple HTTP server from scratch" by: Skrew Everything 
* Channel 'Code Vault'  on YouTube
* Channel 'Jacob Sorber'  on YouTube


