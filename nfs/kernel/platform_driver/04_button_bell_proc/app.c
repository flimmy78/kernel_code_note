#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
#if 0
	while(1)
		;
#endif

#if 1
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s dev_file\n", argv[0]);
		return -1;
	}

	int fd;
	fd = open(argv[1], O_RDWR);	//open -> sys_open -> test_open 
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	char buf[4] = {};
	while(1)
	{
		read(fd, buf, 4);
		printf("%c %c %c %c\n", buf[0], buf[1], buf[2], buf[3]);	
	}

	close(fd);
#endif
	return 0;
}
