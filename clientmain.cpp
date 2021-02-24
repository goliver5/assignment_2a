#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* You will to add includes here */

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
// Enable if you want debugging to be printed, see examble below.
// Alternative, pass 
#define DEBUG


// Included to get the support library
#include <calcLib.h>

int main(int argc, char *argv[]){

  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */

  initCalcLib();

  //double f1,f2,fresult;
  //int i1,i2,iresult;//kan ändras

  if(argc != 2)
  {
    printf("NOT ENOUGH\n");//skriva hur man ska skriva sen
    exit(1);

  }


  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);

  //printf("Desthost: %s\n", Desthost);
  //printf("DestPort: %s\n", Destport);
  printf("Connected to %s:%s\n", Desthost,Destport);

  if(Desthost == NULL)
  {
    printf("ERROR Desthost\n");
    exit(1);
  }

  if(Destport == NULL)
  {
    printf("ERROR Destport\n");
    exit(1);
  }
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  /* Do magic */
  int port=atoi(Destport);
  char buffer[256];
  char ok[]="OK\n", *ptr, *value1, *value2;
  char buf2[256], buf3[256],buf4[256];
  char delim2[]=" ";

  struct sockaddr_in clientaddr;
  socklen_t len; 
  len = sizeof(clientaddr);
  

  int serversocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  struct sockaddr_in minAdress;
  //struct sockaddr_in client_addr;//...
  //socklen_t len;//...
  minAdress.sin_family = AF_INET;
  minAdress.sin_port=htons(port);
  minAdress.sin_addr.s_addr=inet_addr(Desthost);

  //printf("local: %s\n",inet_ntoa() )

  if(connect(serversocket, (struct sockaddr*)&minAdress, sizeof(minAdress))==-1)
  {
    printf("CONNECTION TIMED OUT\n");
    exit(1);
  }
  
  recv(serversocket,buffer, sizeof(buffer), 0);

  
  if(strcmp(buffer, "TEXT TCP 1.0\n\n")!=0)
  {
    printf("something wrong dunno\n");
    exit(1); 
  }

  //memset(buffer, 0, sizeof(buffer));

  if(send(serversocket,ok,strlen(ok), 0 )==-1)
  {
    printf("Send failed\n");
    exit(1);
  }
  recv(serversocket,buf2, 256, 0);


  getsockname(serversocket, (struct sockaddr *)&clientaddr, &len);
  printf("local: %s:%d\n", inet_ntoa(clientaddr.sin_addr),(int)ntohs(clientaddr.sin_port));

  ptr=strtok(buf2, delim2);
  value1=strtok(NULL, delim2);
  value2=strtok(NULL, "\n");

  printf("%s %s %s\n",ptr,value1,value2);

  //printf("Operator: %s\n", ptr);
  //printf("Value1: %s\n", value1);
  //printf("Value2: %s\n", value2);

  //printf("Received delim2: %s\n", delim2);


  if(ptr[0] == 'f')
  {
  double f1,f2,fresult;
  f1=atof(value1);
  f2=atof(value2);
  
  if(strcmp(ptr,"fadd")==0)
  {
    fresult=f1+f2;
    } else if (strcmp(ptr, "fsub")==0){
      fresult=f1-f2;
    } else if (strcmp(ptr, "fmul")==0){
      fresult=f1*f2;
    } else if (strcmp(ptr, "fdiv")==0){
      fresult=f1/f2;
  }

  printf("Calculated the result to %8.8g\n", fresult);

  sprintf(buf4, "%8.8g\n", fresult);

  send(serversocket,buf4,strlen(buf4), 0 );
  memset(&buf3, 0, sizeof(buf3));
  recv(serversocket,buf3, 256, 0);
  printf("Sent from server: %s", buf3);

  }
  else 
  {
  printf("its an int\n");
  int i1,i2,iresult;

  i1=atoi(value1);
  i2=atoi(value2);

  if(strcmp(ptr,"add")==0){
      iresult=i1+i2;
    } else if (strcmp(ptr, "sub")==0){
      iresult=i1-i2;
    } else if (strcmp(ptr, "mul")==0){
      iresult=i1*i2;
    } else if (strcmp(ptr, "div")==0){
      iresult=i1/i2;
    }

    printf("Calculated the result to = %d \n" ,iresult);

    sprintf(buf4, "%d\n", iresult);

  send(serversocket,buf4,strlen(buf4), 0 );

  memset(&buf3, 0, sizeof(buf3));
  recv(serversocket,buf3, sizeof(buf3), 0);
  printf("Sent from server: %s", buf3);
  }


#ifdef DEBUG 

  
  printf("Host %s, and port %d.\n",Desthost,port);
#endif

  
}