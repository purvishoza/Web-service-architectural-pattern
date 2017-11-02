#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>



int * generate_random(int num,int limit);
void AsAServer(int sock, int pno);

    int total_service;
    int *name_of_service;
    char buffer[256];

void error(char *msg)
{
    perror(msg);
    exit(0);
}

char services[25][50] = {
        "AddTwoNumbers","SubtractTwoNumbers","MultiplyTwoNumbers","DivideTwoNumbers","SquarRoot","Square","Area","Volume","Perimeter",
        "Circumference","SurfaceArea","Integrate","Differentiate","Power","Logarithm","StringLength","Encrypt","Decrypt","EdgeDetection",
        "FFT","RayTracing","VolumRendering","ZBuffer","TextureMapping","MotionBlurr"}; 

int main(int argc, char *argv[])
{
    int sockfd, portno, n, ab = 26, i, j, portno2;
    int a[15];
    char record[50][3][20];

    struct sockaddr_in serv_addr;
    struct hostent *server;

    
    if (argc < 4) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno2 = atoi(argv[3]);
    printf("%d\n",portno2 );
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    pid_t p_id;
    p_id = fork();


    if (p_id < 0)
    {
        perror("Fork is failed");
    }

    if (p_id == 0)  {
             //close(sockfd);
            printf("child process\n");
             AsaServer(sockfd,portno2);
             exit(0);
         }
    else
    {
            printf("parent process\n");
         
        
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

        n = write(sockfd, &ab, sizeof(int));
        if (n<0) puts("Error in writing ab");

        bzero(record,sizeof(char)*50*3*20);

        
       while(1)
        {
    // Random number generator
    total_service = generate_random(1,15);
    //total_service = 3;
    printf("Total Service : %d\n",total_service);
    name_of_service = generate_random(total_service,25);
    for (i = 0; i < total_service; ++i) {
        printf("%d\n",name_of_service[i]);
    }
    
    //Creating array that is passing to broker
    for (i = 0; i < total_service; ++i) {
        strcpy(record[i][0],inet_ntoa(serv_addr.sin_addr));
        strcpy(record[i][1],argv[3]);
        strcpy(record[i][2],services[name_of_service[i]-1]);
    } 
    for ( i = 0; i < total_service; ++i) {
        for (j = 0; j < 3; ++j)
        {
        printf("%s\t",record[i][j] );
        }
        printf("\n");
    }
    n = write(sockfd, record, sizeof(char)*total_service*3*20);
    if (n < 0)  
         error("ERROR writing to socket");
    bzero(buffer,256);
      n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer); 

    sleep(60);
    } //End of while loop

    } //End of else block

    return 0;
}

void AsaServer(int sock, int pno)
{
    int sockfd, newsockfd,n;
    struct sockaddr_in serv_addr, requ_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(pno);

    //bind address with socket 
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     // listening on socket
     listen(sockfd,5);

     //for accept
     int requestor = sizeof(requ_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &requ_addr, &requestor);
     if(newsockfd < 0) error("ERROR in accept");

         n = write(newsockfd, "You have been provided",22);
        if (n < 0)  error("ERROR writing to socket");
        close(newsockfd);

}

int *generate_random(int num,int limit) {
    static int random[15];
    int temp[30],single_num;
    int r,i;
    time_t t;
    bzero(random,15);
    bzero(temp,30);
    srand((unsigned) time(&t));
    for (i = 0; i < num; ++i) {
        r = rand()%limit+1;
        //printf("%d\n",r);
        if(!temp[r]) {
            random[i] = r;
        } else {
            i--;
        }
        temp[r] = 1;
    }
    if(limit!=25) {
        single_num = 2;
        return random[0];
    } else {
        return random;
    } 

}