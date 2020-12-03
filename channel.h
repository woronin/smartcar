//////////////// Подпрограммы для работы с каналами обмена //////////////////

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "define.h"

#define MAX_BUFFER_SIZE 1024

#define WRITE_MODE 1
#define READ_MODE  2
#define READWRITE_MODE (READ_MODE | WRITE_MODE)
#define PROTO_UDP   1 
#define PROTO_RS232 2
#define STOPB_1     4
#define STOPB_2     8
#define PCON_OFF    16 
#define PCON_ON     32 

//#define PORT_DRONE 5556

#ifdef LINUX_D

struct channel_t;

typedef void (*TEvent)(struct channel_t *, void *, int);
typedef int (*TReader)(struct channel_t *, void *, int *);
typedef int (*TWriter)(struct channel_t *, void *, int);

typedef struct channel_t
{
	int port;
	char addr[16];
	int sd;
	unsigned mode;
	unsigned proto;
	TEvent onRead, onWrite;
	TReader Reader;
	TWriter Writer;
} TChannel;

#define ISSET_READ(sock) ((sock).mode & READ_MODE)
#define ISSET_WRITE(sock) ((sock).mode & WRITE_MODE)

extern int channel_open(TChannel *sock, int a_proto, int a_mode, int port,
						char *addr, TReader rdr, TWriter wr, TEvent r_action,
						TEvent w_action);
extern int channel_close(TChannel *sock);

extern int channel_stdget(TChannel *sock, void *buf, int bytes_to_read);
extern int channel_stdput(TChannel *sock, void *buf, int bytes_to_put);


#endif
#endif













