#include "stdio.h"
#include <string>
#include <sys/types.h>
#include <openssl/md5.h>
#include <fcntl.h>
#include "unistd.h"
#include "stdlib.h"

void stream(char *pphrase, int length);
