#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

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

	struct input_event event;
	while(1)
	{
		read(fd, &event, sizeof(struct input_event));
		if(event.type == EV_SYN)
		{
			printf("EV_SYN: ");
		}
		if(event.type == EV_KEY)
		{
			printf("EV_KEY: ");
		}
		if(event.type == EV_ABS)
		{
			printf("EV_ABS: ");
		}
		if(event.type == EV_REL)
		{
			printf("EV_REL: ");
		}
		printf("code(%d) value(%d)\n", event.code, event.value);	
	}

	close(fd);
#endif
	return 0;
}
