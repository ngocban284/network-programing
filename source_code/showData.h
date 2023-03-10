#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include "user.h"
#include "define.h"

char *showProfile(char *userID, char *temp);
char *showAllUser(char *temp);
char *listMember(char* userID, char *spaceID, char *temp);
char *listFolder(char *userID, char *spaceID, char *temp);
char *listFile(char *userID, char *spaceID, char *temp);
