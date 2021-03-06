#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	int sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	int sock;
	int n;

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

	while(1){
		len = sizeof(client);
		sock = accept(sock0, (struct sockaddr *)&client, &len);
		if(sock < 0){
			perror("accept");
			break;
		}
		printf("accepted connection from %s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		n = write(sock, "HELLO\n", 6);
		if(n < 0){
			perror("write");
		}

		close(sock);
	}

	close(sock0);

	return 0;
}
