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
#include "user.h"
#include "locationAndTime.h"

#define PORT 9999

void *connection_handler(void *);
bool upToString(char* client_message,char* server_message);
void finish_with_error(MYSQL *con);

User* addUser(char inforUserMessage[5000]);
LocationAndTime* addLocationAndTine(char inforLocationandTime[5000]);

char* createInfor(User* user, char* str, char inforUserMessage[5000]);
void createInforBirthDay(User* user, char* str, char inforUserMessage[5000]);

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
	    printf("Mysql connect success!\n");
    }
    MYSQL_RES *result = mysql_store_result(con);
    int socket = *(int*) newSocket;
	int read_len, reader;
	char server_message[100]="Hello from server\n";
	char server_message_wrong[100] = "Logic error - Invalid UserName or Password\n";
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
        // printf("query: %s", account_message);
        accountName = strtok(account_message,"_");
        printf("Name: %s\n", accountName);
        accountName = strtok(NULL,"_");
        accountPassword = strtok(accountName,"_");
        accountPassword = removeEnterCharacterFromString(accountPassword);
        printf("password:%s\n", accountPassword);
        accountName = strtok(account_message,"_");
        if(strcmp(client_message,"1") == 0){
            bzero(client_message,sizeof(client_message));
            char* idaccount = checkNameAccount(accountName,accountPassword,con,result);
            if(strcmp(idaccount,"false") != 0){
                char server_send_message[100] = "Login successfully: Welcome to PC COVID\n"; 
                write(socket, server_send_message, sizeof(server_send_message));
                bzero(server_send_message,sizeof(server_send_message));
                User *user = createUserEmpty(); 
                addInforUser(user,idaccount,con,result);
                char inforUserToClient[1000];
                char inforUserFromClientUpdate[1000];
                char messageTimeFromClient[1000];
                char messageHistory[1000] = "";
                LocationAndTime *locationAndTimeHistory = createLTEmpty();
                int reader_len_timeFromClient;
                int reader_len_inforUpdate;
                int checkLogOut = 0;
                int setTime = 0;
                while (checkLogOut==0){
                    setTime += 1;
                    char choiceClientMessage[100];
                    int choiceMenuClient = recv(socket,choiceClientMessage,100,0);
                    if(choiceMenuClient > 0){
                        int choice = atoi(choiceClientMessage);
                        switch(choice){
                            case 1:
                                addInforUser(user,idaccount,con,result);
                                sprintf(inforUserToClient,"%s_%s_%s_%s_%s_%s_%s_%s_%s_%s\n", user->idUser, user->firstName, user->lastName, user->cardId, user->birthday, user->gender, user->numberPhone, user->address, user->email, user->state);
                                write(socket,inforUserToClient,sizeof(inforUserToClient));
                                printf("gui thanh cong %s", inforUserToClient);
                                bzero(inforUserToClient,sizeof(inforUserToClient));                
                                break;
                            case 2:
                                reader_len_timeFromClient = recv(socket,messageTimeFromClient,1000,0);
                                if(reader_len_timeFromClient>0){
                                    sprintf(messageTimeFromClient,"%s",removeEnterCharacterFromString(messageTimeFromClient));
                                    LocationAndTime *locationAndTime = addLocationAndTine(messageTimeFromClient);
                                    int codeLocation = getLocation(locationAndTime->location,con,result);
                                    bool checkTime = insertLocationAndTime(locationAndTime, user,codeLocation,con,result);
                                    if(checkTime){
                                        printf("INSERT LOCATION AND TIME SUCCESSFUL\n");
                                        char sendSuccessfulMessageTime[100] = "Khai bao thanh cong!\n";
                                        write(socket,sendSuccessfulMessageTime,sizeof(sendSuccessfulMessageTime));
                                        printf("Gui thanh cong: %s\n",sendSuccessfulMessageTime);
                                        bzero(sendSuccessfulMessageTime,sizeof(sendSuccessfulMessageTime));
                                    }else{
                                        printf("INSERT LOCATION AND TIME ERROR\n");
                                    }
                                }else{
                                    char sendErrorMessageTime[100] = "Can't read time and location!\n";
                                    write(socket,sendErrorMessageTime,sizeof(sendErrorMessageTime));
                                    bzero(sendErrorMessageTime,sizeof(sendErrorMessageTime));
                                }
                                break;
                            case 3:
                                locationAndTimeHistory = getHistory(user,con,result);
                                sprintf(messageHistory,"%s",locationAndTimeHistory->str);
                                // printf("%s",messageHistory);
                                if(messageHistory!=(NULL)){
                                    strcat(messageHistory,"\n");                                
                                    // printf("chuoi history: %s\n",messageHistory);
                                    write(socket,messageHistory,sizeof(messageHistory));
                                    bzero(messageHistory,sizeof(messageHistory));
                                }else{
                                    char messageNULL[100] = "Hien tai khong co lich trinh di chuyen\n";
                                    write(socket,messageNULL,sizeof(messageNULL));
                                    bzero(messageNULL,sizeof(messageNULL));
                                }
                                break;
                            case 4:
                                reader_len_inforUpdate = recv(socket,inforUserFromClientUpdate,1000,0);
                                if(reader_len_inforUpdate>0){
                                    sprintf(inforUserFromClientUpdate,"%s",removeEnterCharacterFromString(inforUserFromClientUpdate));
                                    // printf("chuoi nhan: %s\n", inforUserMessage);
                                    User *user_index = addUser(inforUserFromClientUpdate);
                                    bool checkUpdateInfor = updateInforAccount(user_index,con,result);
                                    if(checkUpdateInfor==true){
                                        char inforUpdateSuccessMessage[1000] = "Update your information successfully\n";
                                        write(socket,inforUpdateSuccessMessage, sizeof(inforUpdateSuccessMessage));
                                        bzero(inforUpdateSuccessMessage,sizeof(inforUpdateSuccessMessage));
                                        free(user_index);
                                    }  
                                }else{
                                    char sendErrorMessageUpdate[100] = "Can't read your update information!\n";
                                    write(socket,sendErrorMessageUpdate,sizeof(sendErrorMessageUpdate));
                                    bzero(sendErrorMessageUpdate,sizeof(sendErrorMessageUpdate));
                                }
                                break;
                            case 5:
                                checkLogOut = 1;
                                break;
                        }
                        bzero(choiceClientMessage,sizeof(choiceClientMessage));
                    }
                    if(setTime > 10000){
                        break;
                    }
                }

                if(checkLogOut == 1){
                    bzero(account_message,sizeof(account_message));
                    free(user);
                    printf("CheckLogOut: %d\n", checkLogOut);
                    continue;
                }

                // send_status=send(socket , server_send_message , strlen(server_send_message),0);
            }else{
                write(socket, server_message_wrong, sizeof(server_message_wrong));
                bzero(server_message_wrong,sizeof(server_message_wrong));
                // send_status=send(socket , server_message_wrong , strlen(server_message_wrong),0);
            }
        }else{
            bzero(client_message,sizeof(client_message));
            char* idaccount = checkNameAccount(accountName,accountPassword,con,result);
            if(strcmp(idaccount,"false") != 0){
                char accountErrorMessage[1000] = "Create a account error: Account already exists\n";
                // printf("check account error\n");
                write(socket, accountErrorMessage, sizeof(accountErrorMessage));
                bzero(accountErrorMessage,sizeof(accountErrorMessage));
                // send_status=send(socket , server_send_message , strlen(server_send_message),0);
            }else{
                int idUser = createNewAccount(accountName,accountPassword,con,result);
                char accountSuccessMessage[1000] = "Create a account successfully: Please enter personal information\n";
                printf("Insert account ok\n");
                char idser[10];
                sprintf(idser, "%d\n",idUser);
                write(socket, accountSuccessMessage, sizeof(accountSuccessMessage));
                write(socket, idser, sizeof(idser));
                bzero(accountSuccessMessage,sizeof(accountSuccessMessage));
                bzero(idser,sizeof(idser));
                char inforUserMessage[5000];
                int reader_len = recv(socket, inforUserMessage, sizeof(inforUserMessage),0);
                if(reader_len < 0){
                    char errorMessage[100] = "Can't receive your infor user message";
                    write(socket,errorMessage, sizeof(errorMessage));
                    bzero(errorMessage,sizeof(errorMessage));
                }else{
                    sprintf(inforUserMessage,"%s",removeEnterCharacterFromString(inforUserMessage));
                    // printf("chuoi nhan: %s\n", inforUserMessage);
                    User *user = addUser(inforUserMessage);
                    bool checkInsertInfor = insertInforAccount(user,con,result);
                    if(checkInsertInfor==true){
                        char inforSuccessMessage[1000] = "Create information successfully -> Let login to PC COVID\n";
                        write(socket,inforSuccessMessage, sizeof(inforSuccessMessage));
                        bzero(inforSuccessMessage,sizeof(inforSuccessMessage));
                    }
                }
            }
        }
        bzero(account_message,sizeof(account_message));
        printf("ket vong\n");
	}
	return 0;
}


User* addUser(char inforUserMessage[5000]){
    int i = 0;
    User *user = createUserEmpty();
    char* token = strtok(inforUserMessage,"_");
    while(token != NULL){
        if(i==0){
            sprintf(user->idUser,"%s",token);
        }else if(i==1){
            sprintf(user->firstName,"%s",token);
        }else if(i==2){
            sprintf(user->lastName,"%s",token);
        }else if(i==3){
            sprintf(user->cardId,"%s",token);
        }else if(i==4){
            sprintf(user->birthday,"%s",token);
        }else if(i==5){
            sprintf(user->gender,"%s",token);
        }else if(i==6){
            sprintf(user->numberPhone,"%s",token);
        }else if(i==7){
            sprintf(user->address,"%s",token);
        }else if(i==8){
            sprintf(user->email,"%s",token);
        }else if(i==9){
            sprintf(user->state,"%s",token);
        }
        // printf("%s\n", token);
        token = strtok(NULL, "_");
        i++;
    }
    return user;
}


LocationAndTime* addLocationAndTine(char inforLocationandTime[5000]){
    int i = 0;
    LocationAndTime *locationAndTime = createLTEmpty();
    char* token = strtok(inforLocationandTime,"_");
    while(token != NULL){
        if(i==0){
            sprintf(locationAndTime->location,"%s",token);
        }else if(i==1){
            sprintf(locationAndTime->time,"%s",token);
        }
        // printf("%s\n", token);
        token = strtok(NULL, "_");
        i++;
    }
    return locationAndTime;
}


