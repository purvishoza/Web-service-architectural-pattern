#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

    char services[25][50] = {
        "AddTwoNumbers","SubtractTwoNumbers","MultiplyTwoNumbers","DivideTwoNumbers","SquarRoot","Square","Area","Volume","Perimeter",
        "Circumference","SurfaceArea","Integrate","Differentiate","Power","Logarithm","StringLength","Encrypt","Decrypt","EdgeDetection",
        "FFT","RayTracing","VolumRendering","ZBuffer","TextureMapping","MotionBlurr"}; 

    char  record[50][3][20];

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    //char *msg;
    int ab = 1;
    int a;

    char buffer[256];

    // Checking Comand-line Argument
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    //Covenvert int port to string fromate
    portno = atoi(argv[2]);

    //Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Getting server name 
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //clear the sery_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    // copy the server name to serv_addr format
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);

    // connecting to server
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    write(sockfd, &ab, sizeof(int));

    printf("Service ID\tService Name\n");
    for (int i = 0; i < 25; ++i) {
        printf("%d\t\t : %s\n",i+1,services[i]);
    }

    printf("Please enter the service id: ");
    scanf("%d", &a);
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    
    n = write(sockfd, &a, sizeof(int));
    if (n < 0)  
         error("ERROR writing to socket");


    n = read(sockfd, record, sizeof(char)*50*3*20);
    if (n < 0) 
         error("ERROR reading from socket");
     printf("this is list of services\n");
     for (int i = 0; i < 25; i++) {
        if (strcmp(record[i][0],"") == 0) {
            break;
        }   
        for(int j = 0; j<2; j++) {
            printf("%s\t", record[i][j]);    
        }
        printf("\n");
     }

    bzero(buffer,256);
    n = read(sockfd,buffer,256);
        if (n < 0) 
         error("ERROR reading from socket");

     
    
    return 0;
}