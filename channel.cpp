//////////////// Подпрограммы для работы с каналами обмена //////////////////

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

#include "channel.h"


#ifdef LINUX_D

#include <arpa/inet.h>
#include <termios.h>
#include <netinet/in.h>
#include <sys/socket.h>
//#include "define.h"

//#define SHOW_GETPUT_DATA

//----------------------------------------------------------
// Создание и открытие канала
//----------------------------------------------------------

int channel_open(TChannel *sock, // Создаваемый канал
                int a_proto,     // Протокол (PROTO_UDP или PROTO_RS232)
                int a_mode,      // Режим (READ_MODE, WRITE_MODE, READWRITE_MODE)
                int port,        // Порт (для UDP) или скорость (для RS232)
                char *addr,      // IP-Адрес или имя COM-порта
                TReader rdr,     // Процедура чтения из канала
                TWriter wr,      // Процедура записи в канал
                TEvent r_action, // Процедура, вызываемая после чтения
                TEvent w_action  // Процедура, вызываемая после записи
				)
{
    int opt=1;
	sock->port=port; strcpy(sock->addr, addr);
	sock->mode=a_mode; sock->proto=a_proto;
	sock->Reader=rdr; sock->onRead=r_action;
	sock->Writer=wr;  sock->onWrite=w_action;
	if(a_proto & PROTO_UDP)
	{
		struct sockaddr_in sa;
		sock->sd=socket(PF_INET, SOCK_DGRAM, 0);
        if(sock->sd == -1) perror("socket:");
		if(setsockopt(sock->sd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt))==-1)
			perror("setsockopt:");
        if(setsockopt(sock->sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))==-1)
            perror("setsockopt:");
		if(a_mode & READ_MODE)
		{
			memset(&sa, 0, sizeof(sa));
			sa.sin_family=PF_INET;
			sa.sin_port=htons(port);
			sa.sin_addr.s_addr=htonl(INADDR_ANY);
			if(bind(sock->sd, (struct sockaddr *)&sa, sizeof(sa))==-1)
				perror("bind:");
		}
		fcntl(sock->sd,F_SETFL,O_NONBLOCK);
	}
	return 1;
}


//int channel_stdget(TChannel *sock, void *buf, int length)
int channel_stdget(TChannel *sock, void *buf)
{
	int ret_val=0;
	//sigprocmask(SIG_BLOCK, &io_set, NULL);
	if(!(sock->mode & READ_MODE)) return 0;
	if(sock->proto & PROTO_UDP)
	{
		struct sockaddr_in sender_addr;
//		int sender_addr_len=sizeof(sender_addr);
		socklen_t sender_addr_len=sizeof(sender_addr);
		memset(&sender_addr, 0, sender_addr_len);
		ret_val=recvfrom(sock->sd, buf, MAX_BUFFER_SIZE, 0,
		(struct sockaddr *)&sender_addr, &sender_addr_len);
		if(ret_val==-1);//perror("channel_udp_get");
		else
		{// Декодируем и сохраняем адрес отправителя
			strcpy(sock->addr, inet_ntoa(sender_addr.sin_addr));
		}
	}
	//sigprocmask(SIG_UNBLOCK, &io_set, NULL);
	return ret_val;
}


int channel_stdput(TChannel *sock, void *buf, int length)
{
	int ret_val=0;
	if(!(sock->mode & WRITE_MODE)) return 0;
	if(sock->proto & PROTO_UDP)
	{
		struct sockaddr_in recv_addr;
		if(!ISSET_WRITE(*sock)) return -1;
		memset(&recv_addr, 0, sizeof(recv_addr));
		recv_addr.sin_family=PF_INET;
		recv_addr.sin_port=htons(sock->port);
		recv_addr.sin_addr.s_addr=inet_addr(sock->addr);
		ret_val=sendto(sock->sd, buf, length, 0, (struct sockaddr *)&recv_addr,
		sizeof(recv_addr));
		if(ret_val<0) perror("sendto");
	}
	return ret_val;
}


int channel_close(TChannel *sock)
{
	if(sock->sd!=-1) close(sock->sd);
	return 1;
}
#endif
