#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include "account_mysql.h"

#define PORT 9999

void *connection_handler(void *);
bool upToString(char* client_message,char* server_message);
void finish_with_error(MYSQL *con);

int main(){
    MYSQL *con = mysql_init(NULL);
    int socketfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];

    socketfd = socket(AF_INET,SOCK_STREAM,0);

    if(socketfd < 0){
        printf("\n error in socket creation");
        return -1;
    }
    printf("\n Server socket is created\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if(ret < 0){
        printf("Error in binding\n");
        return -1;
    }
    printf("[*]Bind to port %d\n", PORT);

    if(listen(socketfd, 10) == 0){
        printf("Listening...\n");
    }else{
        printf("Error in binding\n");
    }


    int no_threads=0;
    pthread_t threads[3];
    while (no_threads<3){
        newSocket = accept(socketfd, (struct sockaddr*)&newAddr, &addr_size);
        if( newSocket < 0){
            printf("No socket\n");
            exit(1);
        }
        if( pthread_create( &threads[no_threads], NULL ,  connection_handler , &newSocket) < 0){
            printf("wrong");	
            perror("Could not create thread");
            return 1;
        }
        if (newSocket < 0) { 
        	printf("server acccept failed...\n"); 
        	exit(0); 
    	}else{
        	printf("Server acccept the client...\n");
        }
	puts("Handler assigned");
    no_threads++;
	}
    int k=0;
    for (k=0;k<3;k++){
	    pthread_join(threads[k],NULL);
	}
    close(socketfd);
    printf("close server: %d\n", no_threads);
    mysql_close(con);
    return 0;
}

void *connection_handler(void *newSocket){
    MYSQL *con = mysql_init(NULL);
    if (con == NULL){
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }

    if (mysql_real_connect(con, "localhost", "root", "password","mydb", 0, NULL, 0) == NULL){
        finish_with_error(con);
    }else{
	    printf("Mysql connect success!!\n");
    }
    MYSQL_RES *result = mysql_store_result(con);
    int socket = *(int*) newSocket;
    int socket2 = *(int*) newSocket;
	int read_len, reader;
	char server_message[100]="Hello from server\n";
	char server_message_wrong[100] = "Logic error - Invalid UserName or Password\n";
    char server_send_message[100] = "Login successfully: Welcome to PC COVID\n"; 
	int send_status;
    // send_status=send(socket, server_message, sizeof(server_message), 0);
	char client_message[1];
    char index[100];
    char account_message[100];
    char* accountName;
    char* accountPassword;
    char query[1024];
	while((read_len=recv(socket,client_message, 100,0))>0)
	{
		client_message[read_len-1] = '\0';
		printf("Chuoi string nhan la: %s\n",client_message);
        reader = recv(socket,account_message,1024,0);
        printf("query: %s", account_message);
        accountName = strtok(account_message,"_");
        printf("Name: %s\n", accountName);
        accountName = strtok(NULL,"_");
        accountPassword = strtok(accountName,"_");
        accountPassword = removeEnterCharacterFromString(accountPassword);
        printf("password:%s\n", accountPassword);
        accountName = strtok(account_message,"_");
        if(strcmp(client_message,"1") == 0){
            bool checkName = checkNameAccount(accountName,accountPassword,con,result);
            if(checkName == true){
                write(socket, server_send_message, sizeof(server_send_message));
                bzero(server_send_message,sizeof(server_send_message));
                // send_status=send(socket , server_send_message , strlen(server_send_message),0);
            }else{
                write(socket, server_message_wrong, sizeof(server_send_message));
                bzero(server_message_wrong,sizeof(server_send_message));
                // send_status=send(socket , server_message_wrong , strlen(server_message_wrong),0);
            }
        }else{
            bool checkName = checkNameAccount(accountName,accountPassword,con,result);
            bool checkCreate = createNewAccount(accountName,accountPassword,con,result);
            if(checkCreate == false){
                printf("check create false");
            }
            if((checkName == true) || (checkCreate == false)){
                char accountErrorMessage[1000] = "Create a account error: Account already exists\n";
                printf("check account error\n");
                write(socket, accountErrorMessage, sizeof(accountErrorMessage));
                bzero(accountErrorMessage,sizeof(accountErrorMessage));
                // send_status=send(socket , server_send_message , strlen(server_send_message),0);
            }else if((checkName == false) && (checkName == true)){
                char accountSuccessMessage[1000] = "Create a account successfully: Please enter personal information\n";
                printf("check account ok\n");
                write(socket, accountSuccessMessage, sizeof(accountSuccessMessage));
                bzero(accountSuccessMessage,sizeof(accountSuccessMessage));
                // send_status=send(socket , server_message_wrong , strlen(server_message_wrong),0);
            }
        }
        bzero(client_message,sizeof(client_message));
	}


	return 0;
}

