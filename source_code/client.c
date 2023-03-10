#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "define.h"
#include "menu.h"
#include "file.h"
#include "fileRes.h"
#include "user.h"
#include "folder.h"
#include "showData.h"
#include "space.h"

int main(int argc, const char *argv[])
{
    // create a socket
    int network_socket;
    if (argc < 2)
    {
        printf("Enter port please \n");
    }
    int port = atoi(argv[1]);
    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    // check for connection_status
    if (connection_status == -1)
    {
        printf("The connection has error\n\n");
    }
    if (connection_status == 0)
    {
        // receive data from the server
        char mes_rev_first[100] = {0};
        recv(network_socket, mes_rev_first, 100, 0);
        printf("%s", mes_rev_first);
        memset(mes_rev_first, '\0', (strlen(mes_rev_first) + 1));
        int choice;
        do
        {
            char email[255] = {0};
            char password[255] = {0};
            loginPage();
            printf(COLOR_GREEN "Enter your choice : " COLOR_RESET);
            scanf("%d", &choice);
            getchar();
            switch (choice)
            {
            case 1:
            {
                system("clear");
                printf(COLOR_BLUE "\n______________________________________________________________________________\n");
                printf("\t\t\t\t SIGN IN\n\n");
                printf("______________________________________________________________________________\n" COLOR_RESET);
                // int state = 0;
                int read_len;
                char mes_rev[255] = {0};
                char *userID;
                printf(COLOR_GREEN "\nEnter Email\t\t: " COLOR_RESET);
                scanf("%s", email);
                int validateLogin(char *email, char *password);
                printf(COLOR_GREEN "\nEnter Password\t\t: " COLOR_RESET);
                scanf("%s", password);
                char message[BUFF_SIZE] = "";
                // char sign_in_code[10] = "0";
                strcat(message, SIGN_IN);
                strcat(message, "*");
                strcat(message, email);
                strcat(message, "*");
                strcat(message, password);
                send(network_socket, message, sizeof(message), 0);
                memset(message, '\0', (strlen(message) + 1));
                read_len = recv(network_socket, mes_rev, sizeof(mes_rev), 0);
                mes_rev[read_len] = '\0';
                if (strcmp(mes_rev, "0") == 0)
                {
                    printf(COLOR_RED "\n\t\t\t\tLogin failed, Try again!\n\n" COLOR_RESET);
                }
                else
                {
                    // printf(COLOR_GREEN"\n___________________________________________________________________________________\n");
                    // printf("\t\t\t\t Login successfully \n\n");
                    // printf("________________________________________________________________________________________\n"COLOR_RESET);
                    mes_rev[read_len] = '\0';
                    userID = strtok(mes_rev, "*");
                    char constUserID[10];
                    strcpy(constUserID, userID);
                    char *userName = strtok(NULL, "*");
                    system("clear");
                    printf(COLOR_GREEN "\n\t\t\t Login successfully : Hello %s\n" COLOR_RESET, userName);
                    int choose;
                    do
                    {

                        homePage();
                        printf(COLOR_GREEN "\nYour choice: " COLOR_RESET);
                        scanf("%d", &choose);
                        getchar();
                        switch (choose)
                        {
                        case 1:
                        {
                            

                            char message[SIZE] = {0};
                            sprintf(message, "%s*%s", PROFILE, constUserID);
                            send(network_socket, message, strlen(message), 0);

                            memset(message, 0, SIZE);
                            int bytes_received = recv(network_socket, message, SIZE - 1, 0);
                            message[bytes_received] = '\0';

                            char* userID = strtok(message, "*");
                            char* userName = strtok(NULL, "*");
                            char* email = strtok(NULL, "*");

                            printf(COLOR_BLUE "___________________________MY PROFILE_________________________\n" COLOR_RESET);
                            printf("\t\tUserID   : \t%s\n", userID);
                            printf("\t\tUserName : \t%s\n", userName);
                            printf("\t\tEmail    : \t%s\n\n", email);
                            printf(COLOR_BLUE "_________________________________________________________________\n" COLOR_RESET);


                            break;
                        }
                        
                        case 2:
                        {
                            char mes_send_showall[1024] = {0};
                            char mes_recv_showall[1024] = {0};
                            strcpy(mes_send_showall, LIST_ALL_USER);
                            strcat(mes_send_showall, "*");
                            strcat(mes_send_showall, constUserID);

                            send(network_socket, mes_send_showall, sizeof(mes_send_showall), 0);
                            memset(mes_send_showall, '\0', sizeof(mes_send_showall));
                            read_len = recv(network_socket, mes_recv_showall, sizeof(mes_recv_showall), 0);
                            mes_recv_showall[read_len] = '\0';
                            
                            if (strcmp(mes_recv_showall, "0") == 0)
                            {
                                printf(COLOR_RED "Have no one\n" COLOR_RESET);
                            }
                            else
                            {
                                userID = strtok(mes_recv_showall, "*");
                                char *length1 = strtok(NULL, "*");
                                int size = atoi(length1);
                                char userlist[size][255];
                                char *p;
                                int j = 0;
                                p = strtok(NULL, "*");
                                strcpy(&userlist[j][255], p);
                                while (p != NULL)
                                {
                                    j++;
                                    p = strtok(NULL, "*");
                                    if (p == NULL)
                                    {
                                        break;
                                    }
                                    strcpy(&(userlist[j][255]), p);
                                }
                                
                                printf(COLOR_BLUE "____________________________LIST USER______________________________\n");
                                printf("%-11s| %-26s | %-21s | %-3s \n", "STT", "UserID", "UserName", "Email");
                                printf("__________________________________________________________________________\n\n" COLOR_RESET);
                                
                                int k = 0;
                                for (int i = 0; i < (atoi(length1) * 3); i += 3)
                                {
                                    printf("%-11d| %-26s | %-21s | %-3s \n", k + 1, &userlist[i][255], &userlist[i + 1][255], &userlist[i + 2][255]);
                                    k++;
                                }
                                
                                printf(COLOR_BLUE "__________________________________________________________________________\n\n" COLOR_RESET);
                            }
                            break;
                        }

                        case 3:
                        {

                            char mes_send_mySpace[1024] = {0};
                            char mes_recv_mySpace[1024] = {0};
                            strcpy(mes_send_mySpace, MY_SPACE);
                            strcat(mes_send_mySpace, "*");
                            strcat(mes_send_mySpace, constUserID);

                            send(network_socket, mes_send_mySpace, sizeof(mes_send_mySpace), 0);
                            memset(mes_send_mySpace, '\0', (strlen(mes_send_mySpace) + 1));
                            read_len = recv(network_socket, mes_recv_mySpace, sizeof(mes_recv_mySpace), 0);
                            mes_recv_mySpace[read_len] = '\0';
                            if (strcmp(mes_recv_mySpace, "0") == 0)
                            {
                                printf(COLOR_RED "You have not any space\n" COLOR_RESET);
                            }
                            else
                            {
                                userID = strtok(mes_recv_mySpace, "*");
                                char *length1 = strtok(NULL, "*");
                                int size = atoi(length1);
                                char listSpace[size][255];
                                char *p;
                                int j = 0;
                                p = strtok(NULL, "*");
                                strcpy(&listSpace[j][255], p);
                                while (p != NULL)
                                {
                                    j++;
                                    p = strtok(NULL, "*");
                                    if (p == NULL)
                                    {
                                        break;
                                    }
                                    strcpy(&(listSpace[j][255]), p);
                                }

                                printf(COLOR_BLUE "____________________________MY SPACE______________________________\n");
                                // printf("\nSTT\t| Space Name\t\t\t| Description\t\t| Space ID\n");
                                printf("%-11s| %-26s | %-21s | %-3s \n", "STT", "SpaceName", "Description", "SpaceID");
                                printf("____________________________________________________________________\n\n" COLOR_RESET);
                                int k = 0;
                                for (int i = 0; i < (atoi(length1) * 3); i += 3)
                                {
                                    printf("%-11d| %-26s | %-21s | %-3s \n", k + 1, &listSpace[i][255], &listSpace[i + 1][255], &listSpace[i + 2][255]);
                                    k++;
                                }
                                printf(COLOR_BLUE "____________________________________________________________________\n\n" COLOR_RESET);
                                char input[2];
                                printf("Do you want to access the space, enter y(or Y)/ NO(enter any key) :");
                                scanf("%[^\n]", input);
                                getchar();
                                if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0)
                                {
                                    char spaceID[5];
                                    printf("Enter ID space : ");
                                    scanf("%[^\n]", spaceID);
                                    getchar();
                                    char mes_send_space_id[BUFF_SIZE];
                                    char mes_recv_space_id[BUFF_SIZE];
                                    strcpy(mes_send_space_id, ACCESS_SPACE);
                                    strcat(mes_send_space_id, "*");
                                    strcat(mes_send_space_id, spaceID);
                                    strcat(mes_send_space_id, "*");
                                    strcat(mes_send_space_id, constUserID);
                                    send(network_socket, mes_send_space_id, sizeof(mes_send_space_id), 0);
                                    memset(mes_send_space_id, '\0', (strlen(mes_send_space_id) + 1));
                                    read_len = recv(network_socket, mes_recv_space_id, sizeof(mes_recv_space_id), 0);
                                    mes_recv_mySpace[read_len] = '\0';
                                    int check = atoi(mes_recv_space_id);
                                    // printf("check result : %d\n", check);
                                    if (check == 0)
                                    {
                                        printf(COLOR_RED "Failed, ID space does not exist!, Try again.\n" COLOR_RESET);
                                    }
                                    else
                                    {
                                        // do nothing
                                        int choose1;
                                        do
                                        {

                                            adminPage();

                                            printf(COLOR_GREEN "Your choose : " COLOR_RESET);
                                            scanf("%d", &choose1);
                                            getchar();
                                            switch (choose1)
                                            {
                                            case 1:
                                            {

                                                char mes_send_list_member[BUFF_SIZE] = {0};
                                                char mes_recv_list_member[BUFF_SIZE] = {0};
                                                strcpy(mes_send_list_member, LIST_MEMBER);
                                                strcat(mes_send_list_member, "*");
                                                strcat(mes_send_list_member, spaceID);
                                                strcat(mes_send_list_member, "*");
                                                strcat(mes_send_list_member, constUserID);
                                                send(network_socket, mes_send_list_member, sizeof(mes_send_list_member), 0);
                                                memset(mes_recv_space_id, '\0', (strlen(mes_recv_space_id) + 1));
                                                read_len = recv(network_socket, mes_recv_list_member, sizeof(mes_recv_list_member), 0);
                                                mes_recv_list_member[read_len] = '\0';
                                                if (strcmp(mes_recv_list_member, "0") == 0)
                                                {
                                                    printf(COLOR_RED "Doesnt have member\n" COLOR_RESET);
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_member, "*");
                                                    char *length1 = strtok(NULL, "*");

                                                    int length = atoi(length1);

                                                    char listMember[255][255];
                                                    char *p;
                                                    int j = 0;
                                                    p = strtok(NULL, "*");
                                                    while (p != NULL)
                                                    {
                                                        strcpy(&listMember[j][255], p);
                                                        j++;
                                                        p = strtok(NULL, "*");
                                                    }
                                                    printf(COLOR_BLUE "_________________________________________________________LIST MEMBER_________________________________________________________\n" COLOR_RESET);
                                                    // printf("\nSTT\t| User ID\t\t| UserName\t\t| Role\n");
                                                    printf("%-11s| %-15s| %-18s| %-30s| %-3s \n", "STT", "UserID", "UserName", "Email", "Role");
                                                    printf(COLOR_BLUE "______________________________________________________________________________________________________________________________\n" COLOR_RESET);

                                                    int k = 0;

                                                    for (int i = 0; i < (length * 3); i += 3)
                                                    {
                                                        if (strcmp(constUserID, &(listMember[i][255])) == 0)
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-30s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], &listMember[i + 2][255], "Admin");
                                                            k++;
                                                        }
                                                        else
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-30s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], &listMember[i + 2][255], "Member");
                                                            k++;
                                                        }
                                                    }
                                                    printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                }
                                                break;
                                            }

                                            case 2:
                                            {
                                                char mes_send_list_folder[BUFF_SIZE] = {0};
                                                char mes_recv_list_folder[BUFF_SIZE] = {0};
                                                sprintf(mes_send_list_folder, "%s*%s*%s", LIST_FOLDER, spaceID, constUserID);
                                                send(network_socket, mes_send_list_folder, strlen(mes_send_list_folder), 0);
                                                memset(mes_send_list_folder, 0, BUFF_SIZE);
                                                ssize_t read_len = recv(network_socket, mes_recv_list_folder, BUFF_SIZE, 0);
                                                mes_recv_list_folder[read_len] = '\0';
                                                if (strcmp(mes_recv_list_folder, "0") == 0)
                                                {
                                                    printf(COLOR_RED "There are no folders in this space\n" COLOR_RESET);
                                                    break;
                                                }
                                                else
                                                {
                                                    char *token = strtok(mes_recv_list_folder, "*");
                                                    int length = atoi(strtok(NULL, "*"));
                                                    char listFolders[length][255];
                                                    for (int i = 0; i < length; i++)
                                                    {
                                                        strcpy(listFolders[i], strtok(NULL, "*"));
                                                        strtok(NULL, "*"); // Discard the owner information
                                                    }
                                                    printf(COLOR_BLUE "_______________________________________LIST FOLDERS__________________________________\n");
                                                    printf("%-11s| %-28s|  %-10s \n", "STT", "Folder Name", "Owner");
                                                    printf("___________________________________________________________________________________________\n\n" COLOR_RESET);
                                                    for (int i = 0; i < length; i++)
                                                    {
                                                        printf("%-11d| %-28s|  %-10s \n", i + 1, listFolders[i], constUserID);
                                                    }
                                                    printf(COLOR_BLUE "___________________________________________________________________________________________\n\n" COLOR_RESET);
                                                    break;
                                                }
                                            }

                                            case 3:
                                            {

                                                char mes_send_list_file[SIZE];
                                                char mes_recv_list_file[SIZE];
                                                int read_len;

                                                // Construct message to send
                                                sprintf(mes_send_list_file, "%s*%s*%s", LIST_FILE, spaceID, constUserID);

                                                // Send message to server
                                                send(network_socket, mes_send_list_file, strlen(mes_send_list_file), 0);

                                                // Receive response from server
                                                read_len = recv(network_socket, mes_recv_list_file, sizeof(mes_recv_list_file), 0);
                                                mes_recv_list_file[read_len] = '\0';

                                                // Check if there are files in folder
                                                if (strcmp(mes_recv_list_file, "0") == 0) {
                                                    printf(COLOR_RED "There are no files in this folder\n" COLOR_RESET);
                                                } else {
                                                    // Parse response to get list of files
                                                    strtok(mes_recv_list_file, "*");
                                                    char *length_str = strtok(NULL, "*");
                                                    int length = atoi(length_str);
                                                    if (length > 0) {
                                                        char listFile[length][255];
                                                        for (int i = 0; i < length; i++) {
                                                            char *file_name = strtok(NULL, "*");
                                                            char *owner = strtok(NULL, "*");
                                                            sprintf(listFile[i], "%s|%s", file_name, owner);
                                                        }

                                                        // Print list of files
                                                        printf(COLOR_BLUE "________________________________________LIST FILE__________________________________________\n");
                                                        printf("%-11s| %-28s|  %-10s \n", "STT", "FileName", "Owner");
                                                        printf("___________________________________________________________________________________________\n" COLOR_RESET);
                                                        for (int i = 0; i < length; i++) {
                                                            printf("%-11d| %-28s|  %-10s \n", i+1, strtok(listFile[i], "|"), strtok(NULL, "|"));
                                                        }
                                                        printf(COLOR_BLUE "___________________________________________________________________________________________\n" COLOR_RESET);
                                                    } else {
                                                        printf(COLOR_RED "There are no files in this folder\n" COLOR_RESET);
                                                    }
                                                }
                                                break;
                                            }

                                            case 4: // UPLOAD
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", UPLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                upload_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 5: // download
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", DOWNLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                download_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 6:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", REMOVE_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                remove_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 7: // makedir
                                            {
                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", CREATE_DIR);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                makedir(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 8: // removedir
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", REMOVE_DIR);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                removedir(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 9: 
                                            {
                                                char mes_send_add_member[255] = {0};
                                                char mes_recv_add_member[255] = {0};
                                                char email[255];
                                                printf("Enter email of member to add: ");
                                                scanf("%254[^\n]%*c", email); // using a field width specifier and *c to consume the newline character
                                                snprintf(mes_send_add_member, sizeof(mes_send_add_member), "%s*%s*%s", ADD_MEMBER, email, spaceID);
                                                send(network_socket, mes_send_add_member, strlen(mes_send_add_member), 0);
                                                memset(mes_send_add_member, '\0', sizeof(mes_send_add_member));
                                                ssize_t read_len = recv(network_socket, mes_recv_add_member, sizeof(mes_recv_add_member), 0);
                                                if (read_len == -1) {
                                                    perror("recv failed");
                                                    exit(EXIT_FAILURE);
                                                }
                                                mes_recv_add_member[read_len] = '\0';
                                                int check = atoi(mes_recv_add_member);
                                                if (check == 0) {
                                                    printf(COLOR_RED "Add member %s failed. Please try again.\n" COLOR_RESET, email);
                                                } else if (check == 2) {
                                                    printf(COLOR_RED "Add member %s failed. The member already exists.\n" COLOR_RESET, email);
                                                } else {
                                                    printf(COLOR_GREEN "Add member %s successful.\n" COLOR_RESET, email);
                                                }
                                                break;
                                            }

                                            case 10:
                                            {

                                                char mes_send_del_member[255] = {0};
                                                char mes_recv_del_member[255] = {0};
                                                char email[255];
                                                printf("Enter email of member to delete : ");
                                                scanf("%[^\n]", email);
                                                getchar();
                                                strcpy(mes_send_del_member, DELETE_MEMBER);
                                                strcat(mes_send_del_member, "*");
                                                strcat(mes_send_del_member, email);
                                                strcat(mes_send_del_member, "*");
                                                strcat(mes_send_del_member, spaceID);
                                                send(network_socket, mes_send_del_member, sizeof(mes_send_del_member), 0);
                                                memset(mes_send_del_member, '\0', (strlen(mes_send_del_member) + 1));
                                                read_len = recv(network_socket, mes_recv_del_member, sizeof(mes_recv_del_member), 0);
                                                mes_recv_del_member[read_len] = '\0';
                                                int check = atoi(mes_recv_del_member);
                                                // printf("check result : %d\n", check);
                                                if (check == 0)
                                                {
                                                    printf(COLOR_RED " Delete member %s failed, Try again.\n" COLOR_RESET, email);
                                                }
                                                else if (check == 2)
                                                {
                                                    printf(COLOR_RED " Delete member failed,  %s does not exist on space !!.\n" COLOR_RESET, email);
                                                }
                                                else
                                                {
                                                    printf(COLOR_GREEN " Delete member %s successfull !!.\n" COLOR_RESET, email);
                                                }
                                                break;
                                            }
                                            case 11: {
                                                char mes_send_del_space[255] = {0};
                                                char mes_recv_del_space[255] = {0};
                                                char sureDel[255];
                                                printf("Are you sure you want to delete this space?");
                                                printf("Enter y( or Y)/ NO (any key) : ");
                                                scanf("%[^\n]", sureDel);
                                                getchar();
                                                if (strcasecmp(sureDel, "y") == 0)
                                                {
                                                    strcpy(mes_send_del_space, DELETE_SPACE);
                                                    strcat(mes_send_del_space, "*");
                                                    strcat(mes_send_del_space, spaceID);
                                                    send(network_socket, mes_send_del_space, strlen(mes_send_del_space), 0);
                                                    memset(mes_send_del_space, '\0', sizeof(mes_send_del_space));
                                                    read_len = recv(network_socket, mes_recv_del_space, sizeof(mes_recv_del_space), 0);
                                                    mes_recv_del_space[read_len] = '\0';
                                                    int check = atoi(mes_recv_del_space);
                                                    printf("check result : %d\n", check);
                                                    if (check == 0)
                                                    {
                                                        printf(COLOR_RED " Delete space failed, Try again.\n" COLOR_RESET);
                                                    }
                                                    else
                                                    {
                                                        printf(COLOR_GREEN " Delete space successfull !!.\n" COLOR_RESET);
                                                        choose1 = 12;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                                break;
                                            }

                                            default:
                                            {
                                                break;
                                            }
                                            }

                                        } while (choose1 != 12);
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        case 4:
                        {
                            char mes_send_myShareSpace[1024] = {0};
                            char mes_recv_myShareSpace[1024] = {0};
                            strcpy(mes_send_myShareSpace, SHARE_SPACE);
                            strcat(mes_send_myShareSpace, "*");
                            strcat(mes_send_myShareSpace, constUserID);

                            send(network_socket, mes_send_myShareSpace, sizeof(mes_send_myShareSpace), 0);
                            memset(mes_send_myShareSpace, '\0', (strlen(mes_send_myShareSpace) + 1));

                            read_len = recv(network_socket, mes_recv_myShareSpace, sizeof(mes_recv_myShareSpace), 0);
                            //  printf("read_len : %d\n", read_len);
                            mes_recv_myShareSpace[read_len] = '\0';

                            if (strcmp(mes_recv_myShareSpace, "0") == 0) {
                                printf(COLOR_RED "No space found \n" COLOR_RESET);
                            } else {
                                // do nothing
                                userID = strtok(mes_recv_myShareSpace, "*");
                                char *length1 = strtok(NULL, "*");
                                int length = atoi(length1);
                                char listSpace[length][255];
                                char *p;
                                int j = 0;
                                p = strtok(NULL, "*");
                                strcpy(&listSpace[j][255], p);
                                while (p != NULL)
                                {
                                    j++;
                                    p = strtok(NULL, "*");
                                    if (p == NULL)
                                    {
                                        break;
                                    }
                                    strcpy(&(listSpace[j][255]), p);
                                }
                                printf(COLOR_BLUE "____________________________MY SHARE Space____________________________\n" COLOR_RESET);
                                // printf("STT\t|Space ID\t\t|\tSpaceName\t\n");
                                printf("%-11s|  %-13s |\t%-22s\n", "STT", "SpaceID", "SpaceName");
                                printf(COLOR_BLUE "__________________________________________________________________\n" COLOR_RESET);
                                int k = 0;
                                for (int i = 0; i < (length * 2); i += 2)
                                {

                                    printf("%-11d|  %-13s |\t%-22s\n", k + 1, &listSpace[i][255], &listSpace[i + 1][255]);
                                    k++;
                                }
                                printf(COLOR_BLUE "__________________________________________________________________\n" COLOR_RESET);
                                char input[2];
                                printf("Do you want to access the share space, enter y (or Y)/ NO(enter any key):");
                                scanf("%[^\n]", input);
                                getchar();
                                if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0)
                                {
                                    char spaceID[5];
                                    printf("Enter ID space: ");
                                    scanf("%[^\n]", spaceID);
                                    getchar();
                                    char mes_send_space_id[255];
                                    char mes_recv_space_id[255];
                                    strcpy(mes_send_space_id, ACCESS_SHARE_SPACE);
                                    strcat(mes_send_space_id, "*");
                                    strcat(mes_send_space_id, spaceID);
                                    strcat(mes_send_space_id, "*");
                                    strcat(mes_send_space_id, constUserID);
                                    // printf("check mes send : %s\n", mes_send_space_id);
                                    send(network_socket, mes_send_space_id, sizeof(mes_send_space_id), 0);
                                    memset(mes_send_space_id, '\0', (strlen(mes_send_space_id) + 1));
                                    read_len = recv(network_socket, mes_recv_space_id, sizeof(mes_recv_space_id), 0);
                                    mes_recv_myShareSpace[read_len] = '\0';
                                    int check = atoi(mes_recv_space_id);
                                    // printf("check result : %d\n", check);
                                    if (check == 0)
                                    {
                                        printf(COLOR_RED "Failed, ID space does not exist!, Try again.\n" COLOR_RESET);
                                    }
                                    else
                                    {
                                        int choose2;
                                        do
                                        {

                                            shareSpacePage();

                                            printf(COLOR_GREEN "Your choose : " COLOR_RESET);
                                            scanf("%d", &choose2);
                                            getchar();
                                            switch (choose2)
                                            {
                                            case 1:
                                            {
                                                // printf("userID : %s\n", constUserID);
                                                char mes_send_list_member[BUFF_SIZE] = {0};
                                                char mes_recv_list_member[BUFF_SIZE] = {0};
                                                strcpy(mes_send_list_member, LIST_MEMBER);
                                                strcat(mes_send_list_member, "*");
                                                strcat(mes_send_list_member, spaceID);
                                                strcat(mes_send_list_member, "*");
                                                strcat(mes_send_list_member, constUserID);
                                                send(network_socket, mes_send_list_member, sizeof(mes_send_list_member), 0);
                                                memset(mes_send_space_id, '\0', (strlen(mes_send_space_id) + 1));
                                                read_len = recv(network_socket, mes_recv_list_member, sizeof(mes_recv_list_member), 0);

                                                mes_recv_list_member[read_len] = '\0';
                                                printf("test chuoi : %s|||\n", mes_recv_list_member);
                                                if (strcmp(mes_recv_list_member, "0") == 0)
                                                {
                                                    printf(COLOR_RED "Not allow to get member info\n" COLOR_RESET);
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_member, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    char listMember[length][255];
                                                    char *p;
                                                    int j = 0;
                                                    p = strtok(NULL, "*");
                                                    strcpy(&listMember[j][255], p);
                                                    //  printf("list[0] : %s\n", &listMember[j][255]);
                                                    while (p != NULL)
                                                    {
                                                        j++;
                                                        p = strtok(NULL, "*");
                                                        // printf(" p : %s\n", p);
                                                        if (p == NULL)
                                                        {
                                                            break;
                                                        }
                                                        strcpy(&(listMember[j][255]), p);
                                                    }
                                                    printf(COLOR_BLUE "_________________________________________________________LIST MEMBER_________________________________________________________\n" COLOR_RESET);

                                                    printf("%-11s| %-15s| %-18s| %-30s| %-3s \n", "STT", "UserID", "UserName", "Email", "Role");
                                                    printf(COLOR_BLUE "______________________________________________________________________________________________________________________________\n" COLOR_RESET);
                                                    int k = 0;

                                                    for (int i = 0; i < (length * 3); i += 3)
                                                    {
                                                        if (i == 0)
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-30s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], &listMember[i + 2][255], "Admin");
                                                        }
                                                        else
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-30s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], &listMember[i + 2][255], "Member");
                                                        }
                                                        k++;
                                                    }
                                                    printf(COLOR_BLUE "______________________________________________________________________________________________________________________________\n" COLOR_RESET);
                                                }
                                                break;
                                            }
                                            case 2:
                                            {

                                                char mes_send_list_folder[1024] = {0};
                                                char mes_recv_list_folder[1024] = {0};
                                                strcpy(mes_send_list_folder, LIST_FOLDER);
                                                strcat(mes_send_list_folder, "*");
                                                strcat(mes_send_list_folder, spaceID);
                                                strcat(mes_send_list_folder, "*");
                                                strcat(mes_send_list_folder, constUserID);
                                                send(network_socket, mes_send_list_folder, sizeof(mes_send_list_folder), 0);
                                                memset(mes_send_list_folder, '\0', (strlen(mes_send_list_folder) + 1));
                                                read_len = recv(network_socket, mes_recv_list_folder, sizeof(mes_recv_list_folder), 0);
                                                mes_recv_list_folder[read_len] = '\0';
                                                // printf("check string : %s\n", mes_recv_list_folder);
                                                if (strcmp(mes_recv_list_folder, "0") == 0)
                                                {
                                                    printf(COLOR_RED "There are no folder in this space or not allow to get list folder!\n" COLOR_RESET);
                                                    break;
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_folder, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    char listFoler[length][255];
                                                    char *p;
                                                    int j = 0;
                                                    p = strtok(NULL, "*");
                                                    strcpy(&listFoler[j][255], p);
                                                    // printf("list[0] : %s\n", &listFoler[j][255]);
                                                    while (p != NULL)
                                                    {
                                                        j++;
                                                        p = strtok(NULL, "*");
                                                        //  printf(" p : %s\n", p);
                                                        if (p == NULL)
                                                        {
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            strcpy(&(listFoler[j][255]), p);
                                                        }
                                                    }
                                                    printf(COLOR_BLUE "________________________LIST FOLDER_________________________\n" COLOR_RESET);
                                                    // printf("\nSTT\t| Folder Name\t\t |  Own\n");
                                                    printf("%-11s| %-28s|  %-10s \n", "STT", "FolderName", "Owner");
                                                    printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                    int k = 0;
                                                    for (int i = 0; i < (length * 2); i += 2)
                                                    {

                                                        printf("%-11d| %-28s|  %-10s \n", k + 1, &listFoler[i][255], &listFoler[i + 1][255]);
                                                        k++;
                                                    }
                                                    printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                }
                                                break;
                                            }
                                            case 3:
                                            {

                                                char mes_send_list_file[1024] = {0};
                                                char mes_recv_list_file[1024] = {0};
                                                strcpy(mes_send_list_file, LIST_FILE);
                                                strcat(mes_send_list_file, "*");
                                                strcat(mes_send_list_file, spaceID);
                                                strcat(mes_send_list_file, "*");
                                                strcat(mes_send_list_file, constUserID);
                                                send(network_socket, mes_send_list_file, sizeof(mes_send_list_file), 0);
                                                memset(mes_send_list_file, '\0', (strlen(mes_send_list_file) + 1));
                                                read_len = recv(network_socket, mes_recv_list_file, sizeof(mes_recv_list_file), 0);
                                                mes_recv_list_file[read_len] = '\0';
                                                // printf("check string : %s\n", mes_recv_list_file);
                                                if (strcmp(mes_recv_list_file, "0") == 0)
                                                {
                                                    printf(COLOR_RED "There are no files in this folder or not allow to get list file\n" COLOR_RESET);
                                                    break;
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_file, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    // printf("check length : %d\n", length);
                                                    if (atoi(length1) > 0)
                                                    {
                                                        char listFile[length * 2][255];
                                                        char *p;
                                                        int j = 0;
                                                        p = strtok(NULL, "*");
                                                        strcpy(&listFile[j][255], p);
                                                        // printf("list[0] : %s\n", &listFile[j][255]);
                                                        while (p != NULL)
                                                        {
                                                            j++;
                                                            p = strtok(NULL, "*");
                                                            // printf(" p[%d] : %s\n", j, p);
                                                            if (p == NULL)
                                                            {
                                                                break;
                                                            }
                                                            else
                                                            {
                                                                strcpy(&(listFile[j][255]), p);
                                                                // printf("list[%d] : %s\n", j, &listFile[j][255]);
                                                            }

                                                            // printf(" listFile[%d] : %s\n",j, &listFile[j][255]);
                                                        }
                                                        for (int i = 0; i < 6; i++)
                                                        {
                                                            // printf(" listFile[%d] : %s\n", i, &listFile[i][255]);
                                                        }
                                                        printf(COLOR_BLUE "_________________________________LIST FILE__________________________________\n");
                                                        printf("\nSTT\t| File Name\t\t\t |  Own\n");
                                                        printf("___________________________________________________________________________\n" COLOR_RESET);

                                                        int k = 0;
                                                        for (int i = 0; i < (atoi(length1) * 2); i += 2)
                                                        {

                                                            printf("%-11d| %-28s|  %-10s \n", k + 1, &listFile[i][255], &listFile[i + 1][255]);
                                                            k++;
                                                        }
                                                        printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                    }
                                                    else
                                                    {
                                                        printf(COLOR_RED "There are no files in this folder\n" COLOR_RESET);
                                                        break;
                                                    }
                                                }
                                                break;
                                            }
                                            case 4:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", UPLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                upload_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 5:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", DOWNLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                download_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 6:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", REMOVE_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                remove_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 7:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", CREATE_DIR);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                makedir(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 8:
                                            {

                                                char mes_send_out_space[255] = {0};
                                                char mes_recv_out_space[255] = {0};
                                                char sureOut[255];
                                                printf("Do you want to leave this space?");
                                                printf("Enter y( or Y)/ NO (any key) : ");
                                                scanf("%[^\n]", sureOut);
                                                getchar();
                                                if (strcmp(sureOut, "y") == 0 || strcmp(sureOut, "Y") == 0)
                                                {
                                                    strcpy(mes_send_out_space, OUT_SPACE);
                                                    strcat(mes_send_out_space, "*");
                                                    strcat(mes_send_out_space, constUserID);
                                                    strcat(mes_send_out_space, "*");
                                                    strcat(mes_send_out_space, spaceID);
                                                    send(network_socket, mes_send_out_space, sizeof(mes_send_out_space), 0);
                                                    memset(mes_send_out_space, '\0', (strlen(mes_send_out_space) + 1));
                                                    read_len = recv(network_socket, mes_recv_out_space, sizeof(mes_recv_out_space), 0);
                                                    mes_recv_out_space[read_len] = '\0';
                                                    int check = atoi(mes_recv_out_space);
                                                    printf("check result : %d\n", check);
                                                    if (check == 0)
                                                    {
                                                        printf(" Out space failed, Try again.\n");
                                                    }
                                                    else
                                                    {
                                                        printf(" Leave space successfull !!.\n");
                                                        choose2 = 9;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                                break;
                                            }
                                            default:
                                            {
                                                break;
                                            }
                                            }
                                        } while (choose2 != 9);
                                    break;
                                    }
                                }
                            }
                            printf("choose5\n");
                            break;
                        }

                        case 5:
                        {
                            system("clear");
                            char mes_send_create_space[255] = {0};
                            char mes_recv_create_space[255] = {0};
                            char spaceName[255] = {0};
                            char space_descrp[255] = {0};
                            strcpy(mes_send_create_space, CREATE_SPACE);
                            strcat(mes_send_create_space, "*");
                            // printf("user id test  : %s\n", userID);
                            strcat(mes_send_create_space, constUserID);
                            strcat(mes_send_create_space, "*");

                            printf("Enter Space Name : ");
                            scanf("%[^\n]%*c", spaceName);
                            // ltrim(spaceName);
                            // rtrim(spaceName);
                            printf("Enter Space Description : ");
                            scanf("%[^\n]%*c", space_descrp);
                            // ltrim(space_descrp);
                            // rtrim(space_descrp);
                            strcat(mes_send_create_space, spaceName);
                            strcat(mes_send_create_space, "*");
                            strcat(mes_send_create_space, space_descrp);

                            send(network_socket, mes_send_create_space, sizeof(mes_send_create_space), 0);
                            memset(mes_send_create_space, '\0', (strlen(mes_send_create_space) + 1));
                            read_len = recv(network_socket, mes_recv_create_space, sizeof(mes_recv_create_space), 0);
                            // printf("read_len : %d\n", read_len);
                            mes_recv_create_space[read_len] = '\0';
                            // printf(" test chuoi nhan : %s\n", mes_recv_create_space);
                            int status = atoi(mes_recv_create_space);
                            // printf("status: %d",status);
                            if (status == 1)
                            {
                                printf(COLOR_GREEN "Create space \"%s\" successfull !\n" COLOR_RESET, spaceName);
                            }
                            else if (status == 2)
                            {
                                printf(COLOR_RED "Space name existed, please try again !\n" COLOR_RESET);
                            }
                            else
                            {
                                printf(COLOR_RED "Space creation failed, try again !\n" COLOR_RESET);
                            }
                            break;
                        }
                        case 6:
                        {
                            break;
                        }
                        default:
                            printf("Function does not exist, please try again \n");
                            break;
                        }
                    } while (choose != 6);
                }
                break;
            }
            case 2:
            {
                system("clear");
                char username[255];
                printf("___________________________________________________________________________\n");
                printf("\t\t\t SIGN UP \n");
                printf("___________________________________________________________________________\n");
                printf("\t\t EMAIL: ");
                scanf("%[^\n]", email);
                getchar();
                printf("\t\t USERNAME: ");
                scanf("%[^\n]", username);
                getchar();
                printf("\t\t PASSWORD: ");
                scanf("%[^\n]", password);
                getchar();
                int state_validate;
                int state = 0;
                state_validate = validateLogin(email, password);
                if (state_validate == 1)
                {
                    char message[BUFF_SIZE] = "";
                    char sign_up_code[10] = "1";
                    strcat(message, sign_up_code);
                    strcat(message, "*");
                    strcat(message, username);
                    strcat(message, "*");
                    strcat(message, email);
                    strcat(message, "*");
                    strcat(message, password);
                    send(network_socket, message, sizeof(message), 0);
                    memset(message, '\0', (strlen(message) + 1));
                    char mes_rev[255];
                    int read_len = recv(network_socket, mes_rev, sizeof(mes_rev), 0);
                    mes_rev[read_len] = '\0';
                    state = atoi(mes_rev);
                    memset(mes_rev, '\0', strlen(mes_rev) + 1);
                    // printf("%d\n", state);
                    if (state == 1)
                    {
                        printf(COLOR_GREEN "Register successfully ! Please login to user\n" COLOR_RESET);
                        break;
                    }
                    else if (state == 2)
                    {
                        printf(COLOR_RED "Failed, Email have already exist.\n" COLOR_RESET);
                        break;
                    }
                    else
                    {
                        // printf("%d\n", state);
                        printf(COLOR_RED "Register failed, Try again!\n" COLOR_RESET);
                        break;
                    }
                }
                else
                {
                    printf(COLOR_RED "Email or password not validated ! Please resgister " COLOR_RESET);
                    break;
                }

                // do everthing
                break;
            }
            case 3:
            {
                system("clear");
                printf("\n\n______________________Chuc ban A+______________________\n\n");
                exit(-1);
                break;
            }
            default:
            {
                printf("Function does not exist. Please try again!\n");
                break;
            }
            }
        } while (choice != 3);
    }
    close(network_socket);

    printf("\n________________See you again________________\n");
    return 0;
}
