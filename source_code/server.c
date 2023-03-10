#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include "define.h"
#include "menu.h"
#include "file.h"
#include "fileRes.h"
#include "user.h"
#include "folder.h"
#include "showData.h"
#include "space.h"

void *connection_handler(void *client_socket)
{
    int socket = *(int *)client_socket;

    char server_message[100] = "_____________________________WELCOME TO MY PROJECT_____________________________\n";
    send(socket, server_message, sizeof(server_message), 0);

    while (1)
    {
        int read_len;
        char client_message[BUFF_SIZE] = "";
        read_len = recv(socket, client_message, BUFF_SIZE, 0);
        // printf("Check read_len : %d\n", read_len);
        if (read_len > 0)
        {
            client_message[read_len] = '\0';
            if (strlen(client_message) > 0)
            {
                char temp[read_len];
                strcpy(temp, client_message);

                char *func_code = strtok(temp, "*");
                // printf("Token get : %s %s\n", func_code, client_message);
                int check_code = atoi(func_code);

                switch (check_code)
                {
                case FN_LIST_FOLDER:
                {
                    printf("------------List folder --------------\n");
                    char *code = strtok(client_message, "*");
                    char *spaceID = strtok(NULL, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("spaceID : %s\n", spaceID);
                    printf("userID : %s\n", userID);
                    char *status = listFolder(userID, spaceID, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case FN_LIST_FILE:
                {
                    printf("------------List file --------------\n");
                    char *code = strtok(client_message, "*");
                    char *spaceID = strtok(NULL, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("spaceID : %s\n", spaceID);
                    printf("userID : %s\n", userID);
                    char *status = listFile(userID, spaceID, temp);
                    printf("status : %s, %ld\n", status, strlen(status));
                    if (strcmp(status, "0") == 0)
                    {
                        printf("gui cai nay 1\n");
                        if (send(socket, temp, strlen(temp), 0) < 0)
                        {
                            printf("send error 1\n");
                        }
                    }
                    else
                    {
                        printf("gui cai nay 2\n");
                        if (send(socket, status, strlen(status), 0) < 0)
                        {
                            printf("send error 2\n");
                        }
                        else
                        {
                            printf("gui thanh cong\n");
                        }
                    }
                    break;
                }
                case FN_SIGN_IN:
                {
                    char *code = strtok(client_message, "*");
                    char *email = strtok(NULL, "*");
                    char *password = strtok(NULL, "*");
                    char temp[255] = "";
                    printf("code : %s\n", code);
                    printf("email : %s\n", email);
                    printf("pass : %s\n", password);
                    char *status = sign_in(email, password, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        int send_mes_client = send(socket, temp, sizeof(temp), 0);
                        printf(" byte send 0 : %d\n", send_mes_client);
                    }
                    else
                    {
                        int send_mes_client = send(socket, status, strlen(status), 0);
                        printf(" byte send 1 : %d\n", send_mes_client);
                    }
                    break;
                }
                case FN_SIGN_UP:
                {
                    char *code = strtok(client_message, "*");
                    char *username = strtok(NULL, "*");
                    char *email = strtok(NULL, "*");
                    char *password = strtok(NULL, "*");
                    printf(" code : %s\n", code);
                    printf("username: %s\n ", username);
                    printf("email : %s\n", email);
                    printf("pass : %s\n", password);
                    int status = sign_up(username, email, password);
                    printf("status : %d\n", status);
                    if (status == 1)
                    {
                        char status[MIN_SIZE] = "1";
                        send(socket, status, sizeof(status), 0);
                    }
                    else if (status == 2)
                    {
                        char status[MIN_SIZE] = "2";
                        send(socket, status, sizeof(status), 0);
                    }
                    else
                    {
                        char status[MIN_SIZE] = "0";
                        send(socket, status, sizeof(status), 0);
                    }
                    break;
                }
                case FN_PROFILE:
                {
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = showProfile(userID, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case FN_LIST_ALL_USER:
                {
                    printf("------------List all user--------------\n");
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = showAllUser(temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case FN_MY_SPACE:
                {
                    printf("------------ MY SPACE --------------\n");
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = my_space(userID, temp);
                    printf("status : %s\n", status);
                    printf("temp : %s\n", temp);
                    if (strcmp(status, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case FN_ACCESS_SPACE:
                {
                    printf("------------ Access Space --------------\n");
                    char *code = strtok(client_message, "*");
                    char *spaceID = strtok(NULL, "*");
                    char *userID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("spaceID : %s\n", spaceID);
                    printf("userID : %s\n", userID);
                    int status = access_space(spaceID, userID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case FN_ACCESS_SHARE_SPACE:
                {
                    printf("------------ Access Share Space --------------\n");
                    char *code = strtok(client_message, "*");
                    char *spaceID = strtok(NULL, "*");
                    char *userID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("spaceID : %s\n", spaceID);
                    printf("userID : %s\n", userID);
                    int status = access_share_space(spaceID, userID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case FN_CREATE_SPACE:
                {
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char *spaceName = strtok(NULL, "*");
                    char *spaceDes = strtok(NULL, "*");
                    printf("--------------- Create Space ------------- \n");
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    printf("space name : %s\n", spaceName);
                    printf("space description : %s\n", spaceDes);
                    int status = create_space(userID, spaceName, spaceDes);
                    printf("status : %d\n", status);
                    if (status == 0)
                    {
                        char status[MIN_SIZE] = "0";
                        send(socket, status, sizeof(status), 0);
                    }
                    else if (status == 1)
                    {
                        char status[MIN_SIZE] = "1";
                        send(socket, status, sizeof(status), 0);
                    }
                    else
                    {
                        char status[MIN_SIZE] = "2";
                        send(socket, status, sizeof(status), 0);
                    }
                    break;
                }
                case FN_LIST_MEMBER:
                {
                    printf("------------List member --------------\n");
                    char *code = strtok(client_message, "*");
                    char *spaceID = strtok(NULL, "*");
                    char *userID = strtok(NULL, "*");
                    char *temp = (char*)malloc(1024*sizeof(char));
                    strcpy(temp, "");
                    printf("code : %s\n", code);
                    printf("spaceID : %s\n", spaceID);
                    printf("userID : %s\n", userID);
                    char *status = listMember(userID, spaceID, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }

                case FN_SHARE_SPACE:
                {
                    printf("------------ My Share Space --------------\n");
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = shareSpace(userID, temp);
                    printf("status : %s\n", status);
                    printf("temp : %s\n", temp);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case FN_ADD_MEMBER:
                {
                    char *code = strtok(client_message, "*");
                    char *email = strtok(NULL, "*");
                    char *spaceID = strtok(NULL, "*");
                    printf(" code : %s\n", code);
                    printf("email: %s\n ", email);
                    printf("spaceID: %s\n ", spaceID);
                    int status = add_member(email, spaceID);
                    printf("status : %d\n", status);
                    if (status == 1)
                    {
                        char status[MIN_SIZE] = "1";
                        send(socket, status, sizeof(status), 0);
                    }
                    else if (status == 2)
                    {
                        char status[MIN_SIZE] = "2";
                        send(socket, status, sizeof(status), 0);
                    }
                    else
                    {
                        char status[MIN_SIZE] = "0";
                        send(socket, status, sizeof(status), 0);
                    }
                    break;
                }
                case FN_DELETE_MEMBER:
                {
                    printf("------------ Delete Member --------------\n");
                    char *code = strtok(client_message, "*");
                    char *email = strtok(NULL, "*");
                    char *spaceID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("email : %s\n", email);
                    printf("spaceID : %s\n", spaceID);
                    int status = delete_member(email, spaceID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else if (status == 2)
                    {
                        char buff[2] = "2";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case FN_DELETE_SPACE:
                {
                    printf("------------ Delete Space --------------\n");
                    printf("check chuoi nhan dc : %s\n", client_message);
                    char *code = strtok(client_message, "*");
                    char *spaceID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("spaceID : %s\n", spaceID);
                    int status = delete_space(spaceID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case FN_OUT_SPACE:
                {
                    printf("------------ Out Space --------------\n");
                    printf("check chuoi nhan dc : %s\n", client_message);
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char *spaceID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    printf("spaceID : %s\n", spaceID);
                    int status = out_space(userID, spaceID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case FN_REMOVE_FILE:
                    remove_res(socket);
                    break;
                case FN_UPLOAD_FILE:
                    write_file(socket);
                    break;
                case FN_DOWNLOAD_FILE:
                    download_file_res(socket);
                    break;
                case FN_CREATE_DIR:
                    makedir_res(socket);
                    break;
                case FN_REMOVE_DIR:
                    removedir_res(socket);
                    break;

                default:
                    break;
                }
            }
        }
        else
        {
            printf("Client exit! !\n");
            break;
        }
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    if( argc < 2)
    {
        printf("Enter port please !!! \n");
    }
    int port = atoi(argv[1]);
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        perror("Socket initialisation failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Server socket created successfully\n");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully binded..\n");
    }

    if (listen(server_socket, 100) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
    {
        printf("Server listening..\n");
    }

    int no_threads = 0;
    pthread_t threads[100];
    while (1)
    {
        printf("Listening...\n");
        int client_socket = accept(server_socket, NULL, NULL);
        puts("Connection accepted");
        if (pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) < 0)
        {
            perror("Could not create thread");
            return 1;
        }
        if (client_socket < 0)
        {
            printf("server acccept failed...\n");
            exit(0);
        }
        else
        {
            printf("Server acccept the client...\n");
        }
        puts("Handler assigned");
        no_threads++;
    }
    int k = 0;
    for (k = 0; k < 100; k++)
    {
        pthread_join(threads[k], NULL);
    }
    close(server_socket);

    return 0;
}
