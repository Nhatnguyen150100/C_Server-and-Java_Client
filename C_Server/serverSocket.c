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
#include "locationOftude.h"
#include "formdeclare.h"

#define PORT 9999
#define HOST_MYSQL "localhost"
#define USER_MYSQL "root"
#define PASSWORD_MYSQL "password"
#define DB_MYSQL "mydb"

void *connection_handler(void *);
void autoTruyVetF1(User* listUserF1, MYSQL *con, MYSQL_RES *result);
void finish_with_error(MYSQL *con);

char* createInfor(User* user, char* str, char inforUserMessage[5000]);
void createInforBirthDay(User* user, char* str, char inforUserMessage[5000]);

int KiemTraKyTuSo(char *s);

bool upToString(char* client_message,char* server_message);

User* addUser(char inforUserMessage[5000], int idUser);
Account* addAccount(char acccount_message[5000]);
LocationAndTime* addLocationAndTine(char inforLocationandTime[5000]);
Formdeclare* addFormdeclare(char formdeclareInfor[5000], int idUser);


User* listUserF1;

int main(){
    MYSQL *con = mysql_init(NULL);
    if (con == NULL){
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }

    if (mysql_real_connect(con, HOST_MYSQL, USER_MYSQL, PASSWORD_MYSQL,DB_MYSQL, 0, NULL, 0) == NULL){
        finish_with_error(con);
    }else{
	    printf("Mysql connect success!\n");
    }
    MYSQL_RES *result = mysql_store_result(con);
    autoTruyVetF1(listUserF1,con,result);
    int choice = 0;
    printf("Thuc hien chuc nang Server chon 1( bo qua chon 0):");
    scanf("%d", &choice);
    if(choice == 1){
        bool check = true;
        do{
            printf("------------Menu------------\n");
            printf("1. Xem thong tin nguoi dung\n");
            printf("2. Cap nhat trang thai nguoi dung\n");
            printf("3. Exit\n");
            int index = 0;
            printf("Lua chon: ");
            scanf("%d", &index);
            switch(index){
                case 1:
                    showAllOfInformationsUser(con,result);
                    break;
                case 2:
                    printf("1. F0\n");
                    printf("2. normal\n");
                    printf("Lua chon: ");
                    int choice2;
                    scanf("%d", &choice2);
                    int idUser;
                    printf("Nhap id nguoi dung: ");
                    scanf("%d", &idUser);
                    if(choice2==1){
                        bool checkUpdate = updateStateF0forUser(idUser,con,result);
                        if(checkUpdate==true){
                            printf("Update thanh cong\n");
                            autoTruyVetF1(listUserF1,con,result);
                        }else{
                            printf("Update khong thanh cong\n");
                        }
                    }else{
                        bool checkUpdate = updateStateNormalforUser(idUser,con,result);
                        if(checkUpdate==true){
                            printf("Update thanh cong\n");
                        }else{
                            printf("Update khong thanh cong\n");
                        }
                    }
                    break;
                case 3:
                    check = false;
                    break;
            }
        }while(check);       
    }



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
    pthread_t threads[1000];
    while (no_threads<1000){
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
    for (k=0;k<1000;k++){
	    pthread_join(threads[k],NULL);
	}
    close(socketfd);
    printf("close server: %d\n", no_threads);
    mysql_close(con);
    free(listUserF1);
    return 0;
}

void *connection_handler(void *newSocket){
    MYSQL *con = mysql_init(NULL);
    if (con == NULL){
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }
    if (mysql_real_connect(con, HOST_MYSQL, USER_MYSQL, PASSWORD_MYSQL,DB_MYSQL, 0, NULL, 0) == NULL){
        finish_with_error(con);
    }else{
	    printf("Mysql connect success!\n");
    }

    MYSQL_RES *result = mysql_store_result(con);
    int socket = *(int*) newSocket;
	int read_len, reader;
	char server_message[100]="Hello from server\n";
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
        if(strcmp(client_message,"login") == 0){
            reader = recv(socket,account_message,1024,0);
            // printf("query: %s", account_message);
            if(strcmp(account_message,"")==0){
                continue;
            }
            accountName = strtok(account_message,"_");
            printf("Name: %s\n", accountName);
            accountName = strtok(NULL,"_");
            accountPassword = strtok(accountName,"_");
            accountPassword = removeEnterCharacterFromString(accountPassword);
            printf("password:%s\n", accountPassword);
            accountName = strtok(account_message,"_");
            bzero(client_message,sizeof(client_message));
            char* idaccount = checkNamePasswordAccount(accountName,accountPassword,con,result);
            if(strcmp(idaccount,"false") != 0){
                char server_send_message[100] = "Login successfully: Welcome to PC COVID\n"; 
                write(socket, server_send_message, sizeof(server_send_message));
                bzero(server_send_message,sizeof(server_send_message));
                User *user = createUserEmpty(); 
                addInforUser(user,idaccount,con,result);
                char inforUserToClient[1000];
                sprintf(inforUserToClient,"%s_%s_%s_%s_%s_%s_%s_%s_%s_%s\n", user->idUser, user->firstName, user->lastName, user->cardId, user->birthday, user->gender, user->numberPhone, user->address, user->email, user->state);
                int check = write(socket,inforUserToClient,sizeof(inforUserToClient));
                printf("gui thanh cong first %d\n", check);
                bzero(inforUserToClient,sizeof(inforUserToClient));
                char stringLocation[1000]; 
                char inforUserFromClientUpdate[1000];
                char formdeclare[1000];
                char messageTimeFromClient[1000];
                char messageHistory[1000] = "";
                char* indexMessage;
                LocationAndTime *locationAndTimeHistory = createLTEmpty();
                Formdeclare* formDeclare = createFormdeclareEmpty();
                int reader_len_timeFromClient;
                int reader_len_inforUpdate;
                int reader_len_formdeclare;
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
                                    int idUser_index = atoi(user->idUser);
                                    User *user_index = addUser(inforUserFromClientUpdate,idUser_index);
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
                            case 6:
                                if(strcmp(user->state,"F1")==0){
                                    char waringMessage[300] = "Hien tai ban dang la F1 do co lich trinh di chuyen trung voi F0. XIN HAY TU CACH LY VA THONG BAO DEN CO SO Y TE GAN NHAT. TRAN TRONG!\n";
                                    write(socket,waringMessage, sizeof(waringMessage));
                                    printf("gui thanh cong 2: %s\n", waringMessage);
                                    bzero(waringMessage,sizeof(waringMessage));
                                }else if(strcmp(user->state,"F0")==0){
                                    char waringMessage[300] = "Hien tai ban dang la F0. YEU CAU KHONG DI CHUYEN KHOI NOI CU TRU VA LIEN HE VOI CO SO Y TE GAN NHAT DE DUOC HUONG DAN. TRAN TRONG!\n";
                                    write(socket,waringMessage, sizeof(waringMessage));
                                    printf("gui thanh cong 2: %s\n", waringMessage);
                                    bzero(waringMessage,sizeof(waringMessage));
                                }else{
                                    char waringMessage[300] = "Khong co thong bao\n";
                                    write(socket,waringMessage, sizeof(waringMessage));
                                    printf("gui thanh cong 2: %s\n", waringMessage);
                                    bzero(waringMessage,sizeof(waringMessage));
                                }
                                break;
                            case 7:
                                indexMessage = getLocationOfF0(con,result);
                                // printf("indexmessage %s\n", indexMessage);
                                strcpy(stringLocation,indexMessage);
                                bzero(indexMessage,sizeof(indexMessage));
                                strcat(stringLocation,"\n");
                                // printf("stringLocation: %s",stringLocation);
                                if(!strcmp(stringLocation,"")==0){
                                    int check = write(socket,stringLocation, sizeof(stringLocation));
                                    if(check>0){
                                        printf("gui thanh cong location: %s", stringLocation);
                                    }
                                    bzero(stringLocation,sizeof(stringLocation));
                                }else{
                                    char stringLocationEmpty[1000] = "khong co dia diem\n";
                                    int check = write(socket,stringLocationEmpty, sizeof(stringLocationEmpty));
                                    if(check>0){
                                        printf("gui thanh cong 0");
                                        bzero(stringLocationEmpty,sizeof(stringLocationEmpty));
                                    }
                                }
                                break;
                            case 8:
                                reader_len_formdeclare = recv(socket,formdeclare,1000,0);
                                if(reader_len_formdeclare>0){
                                    sprintf(formdeclare,"%s",removeEnterCharacterFromString(formdeclare));
                                    int idUser_index = atoi(user->idUser);
                                    formDeclare = addFormdeclare(formdeclare,idUser_index);
                                    bool checkInsertFormDeclare = insertFormDecalre(formDeclare,con,result);
                                    if(checkInsertFormDeclare==true){
                                        char inforUpdateSuccessMessage[1000] = "Khai bao thanh cong\n";
                                        write(socket,inforUpdateSuccessMessage, sizeof(inforUpdateSuccessMessage));
                                        bzero(inforUpdateSuccessMessage,sizeof(inforUpdateSuccessMessage));
                                        free(formDeclare);
                                    }
                                }
                                break;
                            default:
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
                char server_message_wrong[100] = "Login error - Invalid UserName or Password\n";
                write(socket, server_message_wrong, sizeof(server_message_wrong));
                bzero(server_message_wrong,sizeof(server_message_wrong));
                // send_status=send(socket , server_message_wrong , strlen(server_message_wrong),0);
            }
        }else{
            reader = recv(socket,account_message,1024,0);
            printf("query: %s", account_message);
            if(strcmp(account_message,"")==0){
                continue;
            }
            accountName = strtok(account_message,"_");
            printf("Name: %s\n", accountName);
            accountName = strtok(NULL,"_");
            accountPassword = strtok(accountName,"_");
            accountPassword = removeEnterCharacterFromString(accountPassword);
            printf("password:%s\n", accountPassword);
            accountName = strtok(account_message,"_");
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
                // char idser[10];
                // sprintf(idser, "%d\n",idUser);
                write(socket, accountSuccessMessage, sizeof(accountSuccessMessage));
                // write(socket, idser, sizeof(idser));
                bzero(accountSuccessMessage,sizeof(accountSuccessMessage));
                // bzero(idser,sizeof(idser));
                char inforUserMessage[5000];
                int reader_len = recv(socket, inforUserMessage, sizeof(inforUserMessage),0);
                if(reader_len < 0){
                    char errorMessage[100] = "Can't receive your infor user message";
                    write(socket,errorMessage, sizeof(errorMessage));
                    bzero(errorMessage,sizeof(errorMessage));
                    continue;
                }else{
                    sprintf(inforUserMessage,"%s",removeEnterCharacterFromString(inforUserMessage));
                    printf("chuoi nhan: %s\n", inforUserMessage);
                    User *user = addUser(inforUserMessage, idUser);
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
        printf("done!----\n");
	}
	return 0;
}


User* addUser(char inforUserMessage[5000], int idUser){
    int i = 0;
    User *user = createUserEmpty();
    sprintf(user->idUser,"%d",idUser);
    char* token = strtok(inforUserMessage,"_");
    while(token != NULL){
        if(i==0){
            // sprintf(user->idUser,"%s",token);
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


Formdeclare* addFormdeclare(char formdeclareInfor[5000], int idUser){
    int i = 0;
    Formdeclare *formdeclare = createFormdeclareEmpty();
    sprintf(formdeclare->idUser,"%d",idUser);
    char* token = strtok(formdeclareInfor,"_");
    while(token != NULL){
        if(i==0){
            sprintf(formdeclare->numberOfVehicle,"%s",token);
        }else if(i==1){
            sprintf(formdeclare->addressCome,"%s",token);
        }else if(i==2){
            sprintf(formdeclare->addressTo,"%s",token);
        }else if(i==3){
            sprintf(formdeclare->localDate,"%s",token);
        }else if(i==4){
            sprintf(formdeclare->cityCome,"%s",token);
        }else if(i==5){
            sprintf(formdeclare->cityTo,"%s",token);
        }else if(i==6){
            sprintf(formdeclare->districtCome,"%s",token);
        }else if(i==7){
            sprintf(formdeclare->districtTo,"%s",token);
        }else if(i==8){
            sprintf(formdeclare->vehicle,"%s",token);
        }
        // printf("%s\n", token);
        token = strtok(NULL, "_");
        i++;
    }
    return formdeclare;
}

Account* addAccount(char acccount_message[5000]){
    int i =0;
    Account* account = createAccountEmpty();
    char* token = strtok(acccount_message,"_");
        while(token != NULL){
        if(i==0){
            sprintf(account->name,"%s",token);
        }else{
            sprintf(account->password,"%s",token);
        }
        // printf("%s\n", token);
        token = strtok(NULL, "_");
        i++;
    }
    return account;
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

void autoTruyVetF1(User* listUserF1, MYSQL *con, MYSQL_RES *result){  
    listUserF1 = (User*)malloc(500*sizeof(User*));
    listUserF1 = listIdOfF0(con,result);
    if(!strcmp(listUserF1[0].idUser,"")==0){
            for(int i = 0; i < 500; i++){
                if(!(strcmp(listUserF1[i].idUser,"")==0)&& strlen(listUserF1[i].idUser)<5){      
                    if(KiemTraKyTuSo(listUserF1[i].idUser)==0){
                        // printf("idUser F1:%s\n",listUserF1[i].idUser);
                        updateState(listUserF1[i].idUser,con,result);
                    }                                            
                }else{
                    break;
                }       
            }
    }
}

int KiemTraKyTuSo(char *s){
            int i,ok;
            for(i=0; i<strlen(s); i++) {
                        if(s[i] >= '0' && s[i] <= '9') {
                                    ok = 1;
                        } else {
                                    ok = 0;
                                    break;
                        }
            }
            if(ok == 1){
                return 0;
            }else{
                return -1;
            }
}


