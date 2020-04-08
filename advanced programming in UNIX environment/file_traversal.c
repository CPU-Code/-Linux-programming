/*
 * cpucode@ubuntu:~/cpucodefile/environment/fileIO$ ./a.out ./
 * regular files =       5, 100.00 %
 * directories =       0,  0.00 %
 * block specal =       0,  0.00 %
 * char specal =       0,  0.00 %
 * FIFOS =       0,  0.00 %
 * symbolic links =       0,  0.00 %
 * sockets =       0,  0.00 %
 */
#include <dirent.h>
#include <limits.h>
#include "../apue.h"

/* function type that is called for each filename */
typedef int Myfunc(const char *, const struct stat*, int);
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;
/*
 * descend through the hierarchy, starting at "pathname"
 * the caller's func() is called for every file
 */
#define  FTW_F   1	    /* file other than directory */
#define  FTW_D   2	    /* directory */
#define  FTW_DNR 3	    /* directory that can't be read */
#define  FTW_NS  4	    /* file that we can't stat */

static char *fullpath;	    /* contains full pathname for every file */
static size_t pathlen;

int main(int argc, char *argv[])
{
	int ret;
	if(argc != 2)
	{
		err_quit("usage: ftw <starting-pathname>");
	}
	ret = myftw(argv[1], myfunc);
	ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
	
	if(0 == ntot)
	{
		/* avoid divide by 0, print 0 for all counts */
		ntot = 1;
	}
	printf("regular files = %7ld, %5.2f %%\n",nreg, nreg*100.0/ntot);
	printf("directories = %7ld, %5.2f %%\n",ndir, ndir*100.0/ntot);
	printf("block specal = %7ld, %5.2f %%\n",nblk, nblk*100.0/ntot);
	printf("char specal = %7ld, %5.2f %%\n",nchr, nchr*100.0/ntot);
	printf("FIFOS = %7ld, %5.2f %%\n",nfifo, nfifo*100.0/ntot);
	printf("symbolic links = %7ld, %5.2f %%\n",nslink, nslink*100.0/ntot);
	printf("sockets = %7ld, %5.2f %%\n",nsock, nsock*100.0/ntot);
	
	exit(ret);
}
/* we return whatever func() returns */
static int myftw(char *pathname, Myfunc *func)
{
	/* malloc PATH_MAX+1 byte */
	fullpath = path_alloc(&pathlen);
	
	if(pathlen <= strlen(pathname))
	{
		pathlen = strlen(pathname)*2;
		if((fullpath = realloc(fullpath, pathlen)) == NULL)
		{
			err_sys("realloc failed");
		}
	}
	strcpy(fullpath, pathname);
	return(dopath(func));
}

/*
 * descend through the hierarchy,starting at "fullpath"
 *  从“fullpath”下寻找
 * if "fullpath" is anything other than a directory,
 *   如果“fullpath”不是一个目录
 * we lstat() it,call func(), and return 
 *   我们lstat()它，调用func()，然后返回
 * for a directory ,we call ourself recursively for each name in the directory
 *   对于目录，我们为目录中的每个名称递归地调用自己
 */
static int dopath(Myfunc *func)
{
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	int ret;
	int n;
	
	if(lstat(fullpath, &statbuf) < 0)
	{
		return(func(fullpath, &statbuf, FTW_NS));
	}
	if(S_ISDIR(statbuf.st_mode) == 0)
	{
		return(func(fullpath, &statbuf, FTW_F));
	}
	
	n = strlen(fullpath);
	if(n + NAME_MAX + 2 > pathlen)
	{
		pathlen *= 2;
		if((fullpath = realloc(fullpath, pathlen)) == NULL)
		{
			err_sys("realloc failed");
		}
	}
	fullpath[n++] = '/';
	fullpath[n] = 0;
	
	if((dp = opendir(fullpath)) == NULL)
	{
		return(func(fullpath, &statbuf, FTW_DNR));
	}
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name, ".") == 0 || 
		   strcmp(dirp->d_name, "..") ==0)
		{
			continue;
		}
		strcpy(&fullpath[n], dirp -> d_name);
		if((ret = dopath(func)) != 0)
		{
			break;
		}
	}
	
	fullpath[n-1] = 0;
	if(closedir(dp) < 0)
	{
		err_ret("can't close directory %s",fullpath);
	}
	return(ret);
}

static int myfunc(const char *pathname, const struct stat*statptr, int type)
{
	switch(type)
	{
		case FTW_F:
			switch(statptr -> st_mode & S_IFMT)
			{
				case S_IFREG:
					nreg++;
					break;
				case S_IFBLK:
					nblk++;
					break;
				case S_IFCHR:
					nchr++;
					break;
				case S_IFIFO:
					nfifo++;
					break;
				case S_IFSOCK:
					nsock++;
					break;
				case S_IFDIR:
					/* directories should have type = FTW_D */
					err_dump("for S_IFDIR for %s\n",pathname);
			}
			break;
		case FTW_D:
			ndir++;
			break;
		case FTW_DNR:
			err_ret("can't read directory %s\n",pathname);
			break;
		case FTW_NS:
			err_ret("stat error for %s \n",pathname);
			break;
		default:
			err_dump("unknown type %d for pathname %s\n", type, pathname);
	}
	return 0;
}
