/*
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ ls -l    //see length and last modified time
 * total 28
 * -rwxr-xr-x 1 cpucode cpucode 13344 Apr  7 19:44 a.out
 * -rw-r--r-- 1 cpucode cpucode   546 Apr  7 19:44 funtimens.c
 * -rw-r--r-- 1 cpucode cpucode   158 Mar 25 22:29 lseek.c
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ ls -lu
 * total 28
 * -rwxr-xr-x 1 cpucode cpucode 13344 Apr  7 19:44 a.out
 * -rw-r--r-- 1 cpucode cpucode   546 Apr  7 19:44 funtimens.c
 * -rw-r--r-- 1 cpucode cpucode   158 Apr  4 05:24 lseek.c
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ date
 * Tue Apr  7 19:49:43 PDT 2020
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ ./a.out funtimens.c lseek.c 
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ ls -l
 * total 20
 * -rwxr-xr-x 1 cpucode cpucode 13344 Apr  7 19:44 a.out
 * -rw-r--r-- 1 cpucode cpucode     0 Apr  7 19:44 funtimens.c
 * -rw-r--r-- 1 cpucode cpucode     0 Mar 25 22:29 lseek.c
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ ls -lu
 * total 20
 * -rwxr-xr-x 1 cpucode cpucode 13344 Apr  7 19:44 a.out
 * -rw-r--r-- 1 cpucode cpucode     0 Apr  7 19:44 funtimens.c
 * -rw-r--r-- 1 cpucode cpucode     0 Apr  4 05:24 lseek.c
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ ls -lc
 * total 20
 * -rwxr-xr-x 1 cpucode cpucode 13344 Apr  7 19:44 a.out
 * -rw-r--r-- 1 cpucode cpucode     0 Apr  7 19:52 funtimens.c
 * -rw-r--r-- 1 cpucode cpucode     0 Apr  7 19:52 lseek.c
 */
#include <fcntl.h>
#include "../apue.h"

int main(int argc, char *argv[])
{
	int i;
	int fd;
	struct stat statbuf;
	struct timespec times[2];
	
	for(i = 1; i < argc; i++)
	{
        /* fetch current times */
		if(stat(argv[i], &statbuf) < 0)
		{
			err_ret("%s:stat error",argv[i]);
			continue;
		}
        /* truncate */
		if((fd = open(argv[i], O_RDWR | O_TRUNC	)) < 0)
		{
			err_ret("%s:open error",argv[i]);
			continue;
		}
		times[0] = statbuf.st_atim;
		times[1] = statbuf.st_mtim;
		/* reset times */
		if(futimens(fd, times) < 0)
		{
			err_ret("%s:futimens error\n",argv[i]);
		}
		close(fd);
	}
	exit(0);
}
