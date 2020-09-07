/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-09-07 16:38:52
 * @LastEditTime: 2020-09-07 17:03:20
 * @FilePath: \-Linux-programming\my_cp.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

// 使用系统调用实现 cp 命令
// 传给可执行程序的参数个数存放在 main 函数的 argc 中， 参数首地址存放在指针数组 argv 中
int main(int argc, char *argv[])
{
	int fd, dest_fd;
	int count;
	int err;
	char *path = "./test.txt";
	char str[100];

	if(argc != 3)
	{
		perror("argc != 3");
		return -1;
	}

	/**
	 * @function: 打开文件
	 * @parameter: 
	 * 		pathname： 文件的路径及文件名。
	 * 		flags： open 函数的行为标志。
	 * 		mode： 文件权限(可读、 可写、 可执行)的设置
	 * 		O_RDONLY 以只读的方式打开
	 * @return {type} 
	 *     success: 打开的文件描述符
	 *     error: -1
	 * @note: 
	 */
	fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		perror("open");

		return 0;
	}

	/**
	 * @function: 打开文件
	 * @parameter: 
	 * 		O_WRONLY 以只写的方式打开
	 * 		O_CREAT 文件不存在则创建文件
	 * @return {type} 
	 *     success: 
	 *     error: 
	 * @note: 
	 */
	dest_fd = open(argv[2], O_WRONLY | O_CREAT, 0666);
	if(dest_fd == -1)
	{
		perror("open");
		return 0;
	}

	while(1)
	{
		memset(str, 0, sizeof(str));

		/**
		 * @function: 把指定数目的数据读到内存
		 * @parameter: 
		 * 		fd： 文件描述符。
		 * 		addr： 内存首地址。
		 * 		count： 读取的字节个数
		 * @return {type} 
		 *     success: 实际读取到的字节个数
		 *     error: -1
		 * @note: 
		 */
		count = read(fd, str, 99);
		if(count <= 0)
		{
			perror("read");
			break;
		}

		/**
		 * @function: 把指定数目的数据写到文件
		 * @parameter: 
		 * 		fd： 文件描述符。
		 * 		addr： 数据首地址。
		 * 		count： 写入数据的字节个数。
		 * @return {type} 
		 *     success: 实际写入数据的字节个数
		 *     error: -1
		 * @note: 
		 */
		count = write(dest_fd, str, count);
		if(count == -1)
		{
			perror("write");
			break;
		}
	}

	/**
	 * @function: 关闭文件
	 * @parameter: 
	 * 		fd : 调用 open 打开文件返回的文件描述符
	 * @return {type} 
	 *     success: 0
	 *     error: -1
	 * @note: 
	 */
	err = close(fd);
	if(err != 0)
	{
		perror("close");
		return 0;
	}

	err = close(dest_fd);
	if(err != 0)
	{
		perror("close");
		return 0;
	}

	return 0;
}

/*
cpucode@ubuntu:~/cpucode/code/linux_code$ vim text.txt 
  1 cpucodecpucodecpucodecpucodecpucodecpucode
  2 cpucodecpucodecpucode
  3 cpucodecpucodecpucode
  4 

cpucode@ubuntu:~/cpucode/code/linux_code$ ./my_cp text.txt cpucode.txt
read: Success
cpucode@ubuntu:~/cpucode/code/linux_code$ cat cpucode.txt 
cpucodecpucodecpucodecpucodecpucodecpucode
cpucodecpucodecpucode
cpucodecpucodecpucode
*/





