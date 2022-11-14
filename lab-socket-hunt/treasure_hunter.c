#define USERID 1823699974

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

int SERVER = 1;
int PORT = 2;
int LEVEL = 3;
int SEED = 4;

int verbose = 0;
int INBOUND = 64;
int INITIAL = 8;
int TREASURE_SIZE = 1024;
int BUF = 4;

void print_bytes(unsigned char *bytes, int byteslen);

int main(int argc, char *argv[]) {
	unsigned char treasure[TREASURE_SIZE];
	unsigned int treasureIndex = 0;
	unsigned char initial[INITIAL];
	unsigned char inbound[INBOUND];
	
	if (argc < 4) {
		fprintf(stderr, "cmd line arguments\n");
		exit(1);
	}

	unsigned int id = htonl(USERID); //0x6cb3701b
	unsigned short seed = htons(*argv[SEED]);

	initial[0] = 0;
	initial[1] = atoi(argv[LEVEL]);
	memcpy(&initial[2], &id, 4);
	memcpy(&initial[6], &seed, 2);

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int socket_fd, s;
	int af = AF_INET;
	int chunkSize = 0;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = af;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	s = getaddrinfo(argv[SERVER], argv[PORT], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	}

		for (rp = result; rp != NULL; rp = rp->ai_next) {
		socket_fd = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol);
		if (socket_fd == -1)
			continue;

		if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;  /* Success */

		close(socket_fd);
	}

	if (rp == NULL) {   /* No address succeeded */
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}

	freeaddrinfo(result);   /* No longer needed */

	send(socket_fd, initial, INITIAL, 0);
	int nread = read(socket_fd, &inbound, INBOUND);

	if (nread == -1) {
		fprintf(stderr, "read");
		exit(1);
	}

	chunkSize = inbound[0];
	memcpy(&treasure[treasureIndex], &inbound[1], chunkSize);
	treasureIndex += chunkSize;

	unsigned short port = 1;
	char * portString = malloc(24);
	
	for(;;) {
		unsigned int nonce = 0;

		if (chunkSize == 0) {
			break;
		}
		if (chunkSize > 127) {
			fprintf(stderr, "error: %d\n", inbound[0]);
			exit(1);
		}

		int opCode = inbound[chunkSize + 1];

		if (opCode == 1) {
			memcpy(&port, &inbound[chunkSize + 2], 2);
			port = ntohs(port);

			snprintf(portString, 24, "%u", port);
			
			s = getaddrinfo(argv[SERVER], portString, &hints, &result);
			if (s != 0) {
				fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
				exit(1);
			}

			for (rp = result; rp != NULL; rp = rp->ai_next) {
				if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) != -1) {
					break;  // Success
				}
			}
		}
		else if (opCode == 2) {
			close(socket_fd);
			if (port != 1) {
				s = getaddrinfo(argv[SERVER], portString, &hints, &result);
			}
			else {
				s = getaddrinfo(argv[SERVER], argv[PORT], &hints, &result);
			}

				unsigned short localPort;
				memcpy(&localPort, &inbound[chunkSize + 2], 2);

				if (af == AF_INET6){
					struct sockaddr_in6 address;
					address.sin6_family = af;
					address.sin6_addr = in6addr_any;
					address.sin6_port = localPort;
					

					if (s != 0) {
						fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
						exit(1);
					}

						for (rp = result; rp != NULL; rp = rp->ai_next) {
						socket_fd = socket(rp->ai_family, rp->ai_socktype,
								rp->ai_protocol);
						if (socket_fd == -1)
							continue;

						bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
						if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) != -1)
							break;  /* Success */

						close(socket_fd);
					}
				}
				else {
					struct sockaddr_in address;
					address.sin_family = af;
					address.sin_addr.s_addr = INADDR_ANY;
					address.sin_port = localPort;
					

					if (s != 0) {
						fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
						exit(1);
					}

						for (rp = result; rp != NULL; rp = rp->ai_next) {
						socket_fd = socket(rp->ai_family, rp->ai_socktype,
								rp->ai_protocol);
						if (socket_fd == -1)
							continue;

						bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
						if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) != -1)
							break;  /* Success */

						close(socket_fd);
					}
				}

				if (rp == NULL) {   /* No address succeeded */
					fprintf(stderr, "Could not connect\n");
					exit(1);
				}

				freeaddrinfo(result);   /* No longer needed */
		}
		else if (opCode == 3) {
			if (af == AF_INET6) {
				struct sockaddr_in6 local;
				socklen_t len = sizeof(local);
				if (getsockname(socket_fd, (struct sockaddr *)&local, &len) == -1) {
					fprintf(stderr, "error getting port number\n");
					exit(1);
				}

				close(socket_fd);
				if (port != 1) {
					s = getaddrinfo(argv[SERVER], portString, &hints, &result);
				}
				else {
					s = getaddrinfo(argv[SERVER], argv[PORT], &hints, &result);
				}

				for (rp = result; rp != NULL; rp = rp->ai_next) {
						socket_fd = socket(rp->ai_family, rp->ai_socktype,
								rp->ai_protocol);
						if (socket_fd == -1)
							continue;

						bind(socket_fd, (struct sockaddr *)&local, sizeof(local));
						break;
				}
			}
			else {
				struct sockaddr_in local;
				socklen_t len = sizeof(local);
				if (getsockname(socket_fd, (struct sockaddr *)&local, &len) == -1) {
					fprintf(stderr, "error getting port number\n");
					exit(1);
				}

				close(socket_fd);
				if (port != 1) {
					s = getaddrinfo(argv[SERVER], portString, &hints, &result);
				}
				else {
					s = getaddrinfo(argv[SERVER], argv[PORT], &hints, &result);
				}

				for (rp = result; rp != NULL; rp = rp->ai_next) {
						socket_fd = socket(rp->ai_family, rp->ai_socktype,
								rp->ai_protocol);
						if (socket_fd == -1)
							continue;

						bind(socket_fd, (struct sockaddr *)&local, sizeof(local));
						break;
				}
			}


			unsigned short m;
			memcpy(&m, &inbound[chunkSize + 2], 2);
			m = ntohs(m);
			
			for (unsigned short i = 0; i < m; ++i) {
				struct sockaddr_in remote;
				socklen_t remoteLen = sizeof(remote);
				

				if(recvfrom(socket_fd, NULL, 0, 0, (struct sockaddr *) &remote, &remoteLen) == -1) {
					fprintf(stderr, "error with recvfrom\n");
					exit(1);
				}

				nonce += ntohs(remote.sin_port);
			}
			nonce += 1;
			nonce = htonl(nonce);
			
			if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) == -1) {
				fprintf(stderr, "error connecting\n");
				exit(1);
			}

			freeaddrinfo(result);   /* No longer needed */
		}
		else if (opCode == 4) {
			if (af == AF_INET) {
				af = AF_INET6;
			}
			else {
				af = AF_INET;
			}
			hints.ai_family = af;

			memcpy(&port, &inbound[chunkSize + 2], 2);
			port = ntohs(port);

			snprintf(portString, 24, "%u", port);
			
			s = getaddrinfo(argv[SERVER], portString, &hints, &result);

			if (s != 0) {
				fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
				exit(1);
			}

			close(socket_fd);
			for (rp = result; rp != NULL; rp = rp->ai_next) {
				socket_fd = socket(rp->ai_family, rp->ai_socktype,
						rp->ai_protocol);
				if (socket_fd == -1)
					continue;

				if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) != -1)
					break;  /* Success */

				close(socket_fd);
			}

			if (rp == NULL) {   /* No address succeeded */
				fprintf(stderr, "Could not connect\n");
				exit(1);
			}

			freeaddrinfo(result);   /* No longer needed */
		}

		if (opCode != 3) {
			memcpy(&nonce, &inbound[inbound[0] + 4], 4);
			nonce = ntohl(nonce) + 1;
			nonce = htonl(nonce);
		}

		unsigned char buf[BUF];
		memcpy(&buf, &nonce, 4);
		send(socket_fd, buf, BUF, 0);
		nread = read(socket_fd, &inbound, INBOUND);

		if (nread == -1) {
			fprintf(stderr, "read\n");
			exit(1);
		}

		chunkSize = inbound[0];
		memcpy(&treasure[treasureIndex], &inbound[1], chunkSize);
		treasureIndex += chunkSize;

	}
	treasure[treasureIndex] = '\0';
	printf("%s\n", treasure);
	//print_bytes(treasure, treasureIndex);
}

void print_bytes(unsigned char *bytes, int byteslen) {
	int i, j, byteslen_adjusted;

	if (byteslen % 8) {
		byteslen_adjusted = ((byteslen / 8) + 1) * 8;
	} else {
		byteslen_adjusted = byteslen;
	}
	for (i = 0; i < byteslen_adjusted + 1; i++) {
		if (!(i % 8)) {
			if (i > 0) {
				for (j = i - 8; j < i; j++) {
					if (j >= byteslen_adjusted) {
						printf("  ");
					} else if (j >= byteslen) {
						printf("  ");
					} else if (bytes[j] >= '!' && bytes[j] <= '~') {
						printf(" %c", bytes[j]);
					} else {
						printf(" .");
					}
				}
			}
			if (i < byteslen_adjusted) {
				printf("\n%02X: ", i);
			}
		} else if (!(i % 4)) {
			printf(" ");
		}
		if (i >= byteslen_adjusted) {
			continue;
		} else if (i >= byteslen) {
			printf("   ");
		} else {
			printf("%02X ", bytes[i]);
		}
	}
	printf("\n");
}