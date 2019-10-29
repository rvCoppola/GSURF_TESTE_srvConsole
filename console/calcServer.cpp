#pragma hdrstop
#pragma argsused

#define WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int _tmain(int argc, _TCHAR* argv[])
{
	char buf[128];

	int addr_len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	unsigned short port = 175;

	WSADATA wsaData;
	SOCKET listener;
	SOCKET talker;

	int rslt;

	printf("\nServidor de echo TCP/IP - Teste GSURF\n");
	printf("=====================================\n");
	printf("WSAStartup()... ");

	// Requisita Winsock versão 2.2
	if ((rslt = WSAStartup(0x202, &wsaData)) == 0) {
		printf("ok.\n");

		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(port);

		listener = socket(AF_INET, SOCK_STREAM, 0);

		printf("socket()... ");
		if (listener != INVALID_SOCKET) {
			printf("ok.\n");

			rslt = bind(listener, (struct sockaddr *) &server_addr,
				sizeof(server_addr));

			printf("bind()... ");
			if (rslt != SOCKET_ERROR) {
				printf("ok.\n");

				printf("Escutando na porta %d e aguardando conexao... ", port);
				if (listen(listener, 5) != SOCKET_ERROR) {
					printf("ok.\n");

					while(1) {
						printf(
						  "\nPressione Ctrl+C para encerrar o servidor.\n\n");

						printf("accept()... ");
						addr_len = sizeof(client_addr);
						talker = accept(listener,
							(struct sockaddr *) &client_addr, &addr_len);

						if (talker == INVALID_SOCKET) {
							fprintf(stderr,
								"falha!\naccept(): erro %d\n",
								WSAGetLastError());

							WSACleanup();

							return -1;
						} // if (talker == INVALID_SOCKET)
						else
							printf("ok.\n");

						printf("Conexao aceita de %s na porta %d.\n",
							inet_ntoa(client_addr.sin_addr),
							htons(client_addr.sin_port));

						printf("recv()... ");
						rslt = recv(talker, buf, sizeof(buf), 0);

						if (rslt != SOCKET_ERROR) {
							printf("ok.\n");

							if (rslt != 0) {
								buf[rslt] = 0;
								printf("Recebido%s%d byte%s",
									(rslt > 1)?"s " : " ", rslt,
									(rslt > 1)?"s\n":"\n");

								printf("Dados: \"%s\"\n\n", buf);
								printf("Enviando os mesmos dados de "
									"volta para o cliente... ");

								rslt = send(talker, buf, rslt, 0);

								if (rslt == SOCKET_ERROR)
									fprintf(stderr, "falha!\nsend(): erro %d\n",
										WSAGetLastError());
								else
									printf("ok.\n");
							} // if (rslt != 0)
							else
								printf("Conexao encerrada pelo Cliente.\n");
						} // if (rslt != SOCKET_ERROR)
					   else
							fprintf(stderr, "falha!\nrecv(): erro %d\n",
								WSAGetLastError());

						closesocket(talker);
					} // while(1)

					return 0;
				} // if (listen(listener, 5) != SOCKET_ERROR)
			   else
					fprintf(stderr, "falha!\nlisten(): erro %d\n",
					  WSAGetLastError());
			} // if (rslt != SOCKET_ERROR)
			else
				fprintf(stderr, "falha!\nbind(): erro %d\n",
				  WSAGetLastError());

			closesocket(listener);
		} // if (listener != INVALID_SOCKET)
		else
			fprintf(stderr, "falha!\nsocket(): erro %d\n",
				WSAGetLastError());
	} // if ((rslt = WSAStartup(0x202, &wsaData)) == 0)
	else
		fprintf(stderr, "falha!\nWSAStartup(): erro %d\n", rslt);

	WSACleanup();

	return -1;
} // _tmain

