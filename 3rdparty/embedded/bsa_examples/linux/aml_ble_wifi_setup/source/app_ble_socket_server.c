#include "app_ble_socket_server.h"

/********************SERVER API***************************/
int app_ble_socket_server_create(tAPP_SOCKET *app_socket)
{
	unlink(app_socket->sock_path);
	if ((app_socket->server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("fail to create socket\n");
		perror("socket");
		return -1;
	}
	app_socket->server_address.sun_family = AF_UNIX;
	strcpy(app_socket->server_address.sun_path, app_socket->sock_path);
	app_socket->server_len = sizeof(app_socket->server_address);
	app_socket->client_len = sizeof(app_socket->client_address);
	if ((bind(app_socket->server_sockfd, (struct sockaddr *)&app_socket->server_address, app_socket->server_len)) < 0) {
		perror("bind");
		return -1;

	}
	if (listen(app_socket->server_sockfd, 10) < 0) {
		perror("listen");
		return -1;
	}
	printf("Server is ready for client connect...\n");

	return 0;
}

int accpet_client(tAPP_SOCKET *app_socket)
{
	app_socket->client_sockfd = accept(app_socket->server_sockfd, (struct sockaddr *)&app_socket->server_address, (socklen_t *)&app_socket->client_len);
	if (app_socket->client_sockfd == -1) {
		perror("accept");
		return -1;
	}
	return 0;
}

void teardown_socket_server(tAPP_SOCKET *app_socket)
{
	unlink(app_socket->sock_path);
	app_socket->server_sockfd = 0;
	app_socket->client_sockfd = 0;
}

/********************COMMON API***************************/
int socket_send(int sockfd, char *msg, int len)
{
	int bytes;

	if (sockfd < 0) {
		printf("%s: invalid sockfd\n", __func__);
		return -1;
	}
	if ((bytes = send(sockfd, msg, len, 0)) < 0) {
		perror("send");
	}
	return bytes;
}

int socket_recieve(int sockfd, char *msg, int len)
{
	int bytes;

	if (sockfd < 0) {
		printf("%s: invalid sockfd\n", __func__);
		return -1;
	}

	if ((bytes = recv(sockfd, msg, len, 0)) < 0)
	{
		perror("recv");
	}
	return bytes;
}
