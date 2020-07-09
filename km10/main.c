#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>

#include <lzo/lzo1x.h>
#include <lzo/lzoconf.h>
#include <lzo/lzoutil.h>

#define DPATH "/dev/net/tun"

#define IN_MEM (1500)
#define OUT_MEM (IN_MEM + IN_MEM/16 + 64 + 3)


int sock, gfd;

lzo_bytep in;
lzo_bytep out;
lzo_bytep wmem;

int open_sck(char *addr)
{
	int desc;
	struct sockaddr_in6 sck_addr;

	/* create socket v6 */
	desc = socket(PF_INET6, SOCK_DGRAM, 0);
	if(desc < 0) {
		perror("socket");
		return desc;
	}

	memset(&sck_addr, 0, sizeof(struct sockaddr_in6));

	sck_addr.sin6_family = AF_INET6;
	sck_addr.sin6_port = htons(12345);

	if((bind(desc, (struct sockaddr *) &sck_addr, sizeof(struct sockaddr_in6))) == -1) {
		perror("bind");
		return -1;
	}

	/* convert `xxx::xxx` format to bin format */
	if((inet_pton(AF_INET6, addr, &sck_addr.sin6_addr)) < 0) {
		perror("inet6_pton");
		return -1;
	}

	if((connect(desc, (struct sockaddr *) &sck_addr, sizeof(struct sockaddr_in6))) == -1) {
		perror("connect");
		return -1;
	}

	return desc;
}

/* 
	configure tun interface
*/

int open_tun(void)
{
	int fd;
	struct ifreq ifr;

	if((fd = open(DPATH, O_RDWR, 0)) == -1) {
		perror("open");
		return -1;
	}

	memset(&ifr, 0, sizeof(struct ifreq));

	ifr.ifr_flags = IFF_TUN|IFF_NO_PI;
	ioctl(fd, TUNSETIFF, (void *) &ifr);

	return fd;
}

void *socket_routine(void * a)
{
	int d, r;
	lzo_uint len;
	char buf[1500];

	len = OUT_MEM;

	while(1) {
		d = recv(sock, buf, sizeof(char)*1500, 0);
		r = lzo1x_decompress((lzo_bytep) buf, (lzo_uint) d, out, &len, NULL);
		if(r != LZO_E_OK)
			printf("lzo1x_decompress FAILED!!\n");
		if(d < 0)
			perror("recv");
		printf("RECEIVE: %d bytes\n", d);
		write(gfd, out, len);
	}

}

void *tun_routine(void *a)
{
	int d, r;
	lzo_uint len;
	char buf[1500];

	/* len = IN_MEM; */

	while(1) {
		d = read(gfd, buf, 1500);
		r = lzo1x_1_compress((lzo_bytep) buf, (lzo_uint) d, in, &len, wmem);
		if(r != LZO_E_OK)
			printf("lzo1x_1_compress FAILED!!\n");
		printf("READ: %d bytes\n", d);
		d = send(sock, in, len, 0);
		if(d < 0)
			perror("send");
	}
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t p_id1, p_id2;

	if(argc  < 2 || argc > 3) {
		printf("Usage: %s [IPv6 destination]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if((sock = open_sck(argv[1])) == -1)
		exit(EXIT_FAILURE);

	if((gfd = open_tun()) == -1)
		exit(EXIT_FAILURE);

	if(lzo_init() != LZO_E_OK) {
		printf("lzo_init FAIL!!\n");
		exit(EXIT_FAILURE);
	}

	in = (lzo_bytep) lzo_malloc(IN_MEM);
	out = (lzo_bytep) lzo_malloc(OUT_MEM);
	wmem = (lzo_bytep) lzo_malloc(LZO1X_1_MEM_COMPRESS);

	if(in == NULL || out == NULL || wmem == NULL) {
		printf("lzo_malloc FAILED!!\n");
		exit(EXIT_FAILURE);
	}

	lzo_memset(in, 0, IN_MEM);

	pthread_create(&p_id1, NULL, socket_routine, NULL);
	pthread_create(&p_id2, NULL, tun_routine, NULL);
	pthread_join(p_id1, NULL);
	
	close(gfd);
	lzo_free(in);
	lzo_free(out);
	lzo_free(wmem);

	return 0;
}

