#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include "replaceCRLF.h"
#include "getFileSize.h"

int main()
{
	int sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	int sock;
	long long headerSize = 44;
	long long fileSize;

	char *buf;
	char inbuf[2048];

	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	if(sock0 < 0){
		perror("socket");
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sock0, (struct sockaddr *) &addr,sizeof(addr)) != 0){
		perror("bind");
		return 1;
	}

	if(listen(sock0, 5) != 0){
		perror("listen");
		return 1;
	}

	fileSize = getFileSize("index.html");
	if(fileSize == (long long)(-1)){
		fprintf(stderr, "getFileSize() error\n");
		return 1;
	}
	printf("%lld\n", fileSize);

	buf =(char*)malloc(sizeof(char) * (fileSize + headerSize));
	if(buf == NULL){
		fprintf(stderr, "malloc() error\n");
		return 1;
	}
	printf("%d\n", sizeof(buf));

	memset(buf, 0, sizeof(char) * (fileSize + headerSize));
	snprintf(buf, sizeof(char) * (fileSize + headerSize), "HTTP/1.0 200 OK\r\n"
	                                                      "Content-Type: text/html\r\n"
	                                                      "\r\n");

	FILE *f;
	f = fopen("index.html", "r");
	int t = 44;
	while(fgets(buf + t,sizeof(char) * (fileSize + headerSize),f) != NULL){
		while(buf[t] != '\0'){
			t++;
		}
	}

	fclose(f);

	if(replaceCRLF(0, buf, sizeof(char) * (fileSize + headerSize)) != 0){
		printf("e");
		return -1;
	}

	printf("%s\n", buf);

	while(1){
		len = sizeof(client);
		sock = accept(sock0, (struct sockaddr *)&client, &len);
		if(sock < 0){
			perror("accept");
			break;
		}
		printf("accepted connection from %s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		fileSize = getFileSize_fd(sock);
		if(fileSize == -1){
			fprintf(stderr, "getFileSize_fd() error\n");
			return 1;
		}
		printf("filesize = %lld\n", fileSize);

		memset(inbuf, 0, sizeof(inbuf));
		recv(sock, inbuf, sizeof(inbuf), 0);

		printf("%s\n", inbuf);

		send(sock, buf, (int)strlen(buf), 0);

		close(sock);
	}

	close(sock0);

	return 0;
}
