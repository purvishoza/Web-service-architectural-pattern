Implement a sequential service requester, a multithreaded service broker, and a multiprocessor service provider.
The implementation must be done in the C programming language on the Linux Machines and on Sapphire.rocks.uhcl.edu.

The Service Requester:
This program should be a sequential program that, via a socket connection first asks the broker for the location of a specific service, the broker will respond with an IP address and a port number directing the requester to a particular service provider or a few providers if the service is available at multiple providers. 
Upon knowing the service provider the requester will ask for the specific service from the provider by sending it a message via a socket connection, and receiving a result back. 

This program must be interactive and ask the user to input a service that it needs performed. 
As a result, it will communicate with the broker, and print to the screen the information received from the broker in a clearly formatted way.
Upon contacting the provider, the program must print the message received from it.

The Service Broker:
This program will be multithreaded where one thread will respond to any number of service requesters asking for specific service. 
The other thread will listen and wait for service providers sending it via socket connections their available services along with information about how to connect to them. Upon receiving the information it will update its database (implement this in any way you’d like). This database is shared between the two threads and therefore concurrency control must take place. The broker may handle any number of service providers passing it their available services.

A broker may actually have multiple entries in its tables for any service if more than one provider provides this service.
In that case, it will send the service requester a message indicating that there are multiple entries and it is up to the service requester to decide which provider to use. 

Once updates are received from all providers, the broker should display its table of services and their providers to the screen.

The Service Provider:
Service providers will be a multiprocessor based system and therefore, they will first communicate with the service broker their available services. 
The other process will support handling requests from the service requesters. 

Upon receiving a request from the requester, the service provider will read the received message and basically respond back with a message saying that the specific requested service is completed (no need to actually perform the service but if you’d like to practice, then go ahead and implement a few if you’d like, but it’s not required). A message will be sent back to the requester indicating that, and in turn the requester will print that message to the screen. 
