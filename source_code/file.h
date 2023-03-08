#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <libgen.h>
#include "user.h"
#include "define.h"

void errorMsg(char *str);
void upload_file(char *UserID,int clientSocket);
void write_file(int clientSocket);

void download_file(char *UserID,int clientSocket);

void remove_file(char *UserID,int sockfd);

void add_file_to_DB(MYSQL *con,char *UserID,char *filename);

void main_menu();
