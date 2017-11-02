
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h> 
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void  *req_handler(void *);
void *pro_handler(void *);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int n;
char buffer[256];
char record[50][3][20],temp[50][3][20],result[50][3][20];
char services[25][50] = {
	"AddTwoNumbers","SubtractTwoNumbers","MultiplyTwoNumbers","DivideTwoNumbers","SquarRoot","Square","Area","Volume","Perimeter",
	"Circumference","SurfaceArea","Integrate","Differentiate","Power","Logarithm","StringLength","Encrypt","Decrypt","EdgeDetection",
	"FFT","RayTracing","VolumRendering","ZBuffer","TextureMapping","MotionBlurr"}; 

//char *msg;
int counter = 0;
int a;


void error(char *msg)
{
    perror(msg);
    exit(1); 
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, requestor, provider;
     //char buffer[256];
     struct sockaddr_in brok_addr, requ_addr;
     int ab;
     //int a[15];

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     //creating socket

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &brok_addr, sizeof(brok_addr));

     portno = atoi(argv[1]);

     brok_addr.sin_family = AF_INET;
     brok_addr.sin_addr.s_addr = INADDR_ANY;
     brok_addr.sin_port = htons(portno);

     //bind address with socket
     if (bind(sockfd, (struct sockaddr *) &brok_addr,
              sizeof(brok_addr)) < 0) 
              error("ERROR on binding");

     // listening on socket
     listen(sockfd,5);

     //for accept
     requestor = sizeof(requ_addr);
     

     printf("Waiting for incoming connections\n");

     // Accept system call
     while(1){

     	newsockfd = accept(sockfd, (struct sockaddr *) &requ_addr, &requestor);
     		//close(sockfd);
     	if (newsockfd < 0) 
          error("ERROR on accept");

      	puts("connection Accepted");

      	// reading ab value and based on that call thread
      	read(newsockfd, &ab, sizeof(int));


      	pthread_t req_thrd, pro_thrd;
      	int *new_sock = malloc(1);
      	*new_sock = newsockfd;

      	if(ab == 1 )
      	{
      		if(pthread_create(&req_thrd, NULL, req_handler, (void*)new_sock) < 0)
      		{
      			puts("Could not creating thread");
      			return 1;
      		}
      			puts("connection1 Handler assigned");
      			close(newsockfd);
      		
      	}

      	else
      	{
      		if(pthread_create(&pro_thrd, NULL, pro_handler, (void*)new_sock) < 0)
      		{
      			puts("Could not creating thread");
      			return 1;
      		}
      			puts("connection2 Handler assigned");
      	}

     	bzero(buffer,256);
  		

 	}
     return 0; 
}

void *req_handler(void *sock)
{
	int req_sock = *(int *)sock;
	int c=0;
	n = read(req_sock, &a, sizeof(int));
	if(n < 0) puts("Error reading from socket");
	printf("%d\n",a );
	pthread_mutex_lock(&mutex);
	//printf("Index\tIP Address\tPort Number\n");
	for (int i = 0; i < counter; ++i) {
		if(strcmp(services[a-1],temp[i][2]) == 0) {
			strcpy(result[c][0],temp[i][0]);
			strcpy(result[c][1],temp[i][1]);
			strcpy(result[c][2],temp[i][2]);
			++c;
		}
	}
	write(req_sock, result, sizeof(char)*50*3*20);
	pthread_mutex_unlock(&mutex);


	free(sock);

}

void *pro_handler(void *sock)
{
	int pro_sock = *(int *)sock;
	int i,j,k,rows;
	//bzero(record,sizeof(char) * 50*3*20);
	while(1)
	{
	pthread_mutex_lock(&mutex);
	n = read(pro_sock, record,sizeof(char)*50*3*20);
	if (n < 0) error("ERROR reading from socket");
	for(i=0;i<counter;i++) {
      if(strcmp(record[0][1],temp[i][1])==0) {
          for(j=0;j<2;j++) {
              for(k=i;k<counter;k++) {
                  strcmp(temp[k][j],temp[k+1][j]);
              }
          --i;
          --counter;
          }
      }   
  }


      for ( i = 0; i < 50; ++i) {   
    	if (strcmp(record[i][0],"") == 0) {
	       	break;
	    } 	
        for (j = 0; j < 3; ++j) {
       		strcpy(temp[counter][j],record[i][j]);
        	//printf("%s\t",record[counter+i][j]);	       	
        }
        if(strcmp(record[i][0],"") != 0) {
        	counter = counter + 1;
        }
    }
	
    pthread_mutex_unlock(&mutex);
    
    bzero(record,sizeof(char)*50*3*20);

    for ( i = 0; i < counter; ++i) {   
        for (j = 0; j < 3; ++j) {
        	printf("%s\t",temp[i][j]);	       	
        }
        printf("\n");
    }
     n = write(pro_sock,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
    }	
     //free(sock);

}