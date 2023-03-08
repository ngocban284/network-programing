#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <libgen.h>
#include "user.h"
#include "define.h"

void download_file_res(int clientSocket);

void remove_file(char *UserID,int sockfd);
void remove_res(int clientSocket);

