#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* You will to add includes here */

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
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
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  //printf("Connected to %s:%s\n", Desthost,Destport);
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  initCalcLib();
  char *ptr;

  double f1,f2,fresult;
  int i1,i2,iresult;

  /* Do magic */
  int port=atoi(Destport);
  int sockfd;
  int clientbertfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
  char buf[256], answer[256];
  char errormsg[]="ERROR TO\n";
  char ok[]="OK\n";
  char confirm[]="TEXT TCP 1.0\n\n";
  memset(&buf, 0,sizeof(buf));
  memset(&answer,0,sizeof(answer));

  struct sockaddr_in clientaddr;
  socklen_t len; 
  len = sizeof(clientaddr);

  memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

  struct timeval time;
  time.tv_sec = 5;
  time.tv_usec = 0;

if ((rv = getaddrinfo(Desthost, Destport, &hints, &servinfo)) != 0) 
  {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }
  for(p = servinfo; p != NULL; p = p->ai_next)
   {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
    {
			//close(sockfd);
			perror("listener: bind");
			continue;
		}
   }
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time));


    while (1)
    {

      rv = listen(sockfd,5);
      if(rv == -1)
      {
        printf("in a bit of a pickle with listen");
        close(sockfd);
        exit(1);
      }
      clientbertfd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);
      if(clientbertfd ==-1)
      {
        continue;
      }
      
      if(send(clientbertfd,confirm, strlen(confirm),0) == -1)
      {
        printf("send tcp error\n");
      }
      else
      {
        printf("send CONFRIM SUCCESS\n");
      }
      for(int i =0; i<3; i++)
        {
        if(recv(clientbertfd,buf, sizeof(buf), 0) ==-1)
          {
            printf("recv error \n");
            continue;
          }
          else
          {
            printf("%s",buf);
            break;
          }
        }

        ptr=randomType();

    if(ptr[0]=='f')
    {
      printf("Float\t");
      f1=randomFloat();
      f2=randomFloat();
      
    /* At this point, ptr holds operator, f1 and f2 the operands. Now we work to determine the reference result. */
   
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
    printf("%s %8.8g %8.8g = %8.8g\n",ptr,f1,f2,fresult);
    memset(&buf, 0,sizeof(buf));
    sprintf(buf,"%s %8.8g %8.8g\n", ptr, f1, f2);
    sprintf(answer,"%8.8g",fresult);

    if(send(clientbertfd,buf, strlen(buf),0) == -1)
      {
        printf("send tcp error 2\n");
      }
      else
      {
        printf("sent: %s", buf);
        memset(&buf,0,sizeof(buf));
      }
        
      if(recv(clientbertfd,buf, sizeof(buf), 0) ==-1)
        {
          printf("recv error \n");
          send(clientbertfd,errormsg, strlen(errormsg),0);
          continue;
        }
      else
        {
          printf("recieved: %s",buf);
          
          
        }

      if(strcmp(answer,buf))
       {
         printf("Correct answer\n");
         if(send(clientbertfd,ok, strlen(ok),0) == -1)
          {
             printf("send tcp error 2\n");
          }
          else
          {
            printf("sent: %s", ok);
            memset(&buf,0,sizeof(buf));
          }
        }
       else
       {
         printf("WRONG ANSWER\n");
         if(send(clientbertfd,errormsg, strlen(errormsg),0) == -1)
          {
             printf("send tcp error 2\n");
          }
          else
          {
            printf("sent: %s", errormsg);
            memset(&buf,0,sizeof(buf));
          }
       }


  } 
  else 
    {
    printf("Int\t");
    i1=randomInt();
    i2=randomInt();

    if(strcmp(ptr,"add")==0){
      iresult=i1+i2;
    } else if (strcmp(ptr, "sub")==0){
      iresult=i1-i2;
    } else if (strcmp(ptr, "mul")==0){
      iresult=i1*i2;
    } else if (strcmp(ptr, "div")==0){
      iresult=i1/i2;
    }

    printf("%s %d %d = %d \n",ptr,i1,i2,iresult);
    memset(&buf, 0,sizeof(buf));
    sprintf(buf,"%s %d %d\n", ptr, i1, i2);
    sprintf(answer,"%d",iresult);



    if(send(clientbertfd,buf, strlen(buf),0) == -1)
      {
        printf("send tcp error 2\n");
      }
      else
      {
        printf("sent: %s", buf);
        memset(&buf,0,sizeof(buf));
      }
      if(recv(clientbertfd,buf, sizeof(buf), 0) ==-1)
        {
          printf("recv error \n");
          continue;
        }
      else
        {
          printf("recieved: %s",buf);
          
         
        }
       
       if(strcmp(answer,buf))
       {
         printf("Correct answer\n");
         if(send(clientbertfd,ok, strlen(ok),0) == -1)
          {
             printf("send tcp error 2\n");
          }
          else
          {
            printf("sent: %s", ok);
            memset(&buf,0,sizeof(buf));
          }
        }
       else
       {
         printf("WRONG ANSWER\n");
         if(send(clientbertfd,errormsg, strlen(errormsg),0) == -1)
          {
             printf("send tcp error 2\n");
          }
          else
          {
            printf("sent: %s", errormsg);
            memset(&buf,0,sizeof(buf));
          }
       }

    
    }
    
    

  }//close sockfd efter loopen
  close(sockfd);

#ifdef DEBUG 
  printf("Host %s, and port %d.\n",Desthost,port);
#endif

  
}