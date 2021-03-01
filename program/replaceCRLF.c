#include <stdio.h>
int replaceCRLF(int t, char *buf, int size)
{
	while(buf[t] == '\0')
	{
	printf("%c", buf[t]);
		if(buf[t] != '\r' && buf[t + 1] == '\n'){
			for(int i = size - 2;i > t;i--){
				buf[i + 1] = buf[i];
			}
			buf[t + 1] = '\r';
		}
		t++;
	}
	return 0;
}
