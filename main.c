#include <stdio.h>
#include <fcntl.h>

int main()
int n;
void buffer= "Hello world";
scanf("%d", &n);
{
for(int i=0;i < n; i++)
	{
	int fd = open("\\wsl$\ubuntu\home\marianna\File_<i-th>", O_RDWR|O_CREAT|O_TRUNC, S_IRUS|S_IWUSR);//ete nuyn anunov baci anyndhat nuyn texum kpahi
	ssize_t write(fd, void *buffer, ssize_t 50);
			


	}



}
