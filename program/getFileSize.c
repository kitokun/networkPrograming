#include <sys/stat.h>
long long getFileSize(char *pathName)
{
	struct stat sb;

	if(stat(pathName, &sb) == -1){
		return -1;
	}

	return sb.st_size;
}

long long getFileSize_fd(int fd)
{
	struct stat sb;
	if(fstat(fd, &sb) == -1){
		return -1;
	}
	return sb.st_size;
}
