#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "Cclientproto.h"
#include "benchapi.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <Csync_user.h>

void
sig_exit (int signo)
{
	exit (signo);
}

int deal_msg(char * msg,int msglen, Cclientproto *cp )
{
	uint32_t msg_id=*((uint32_t *)msg);
	char buf[14096];
	int ret;
	switch ( msg_id ){
		case 1:{
				   TUSER_FLAG *uf=(TUSER_FLAG*)msg;							
				   ret=cp->send_user_set_vip_flag_cmd(uf->userid, uf->flag);
				   DEBUG_LOG ("DEAL set vip flag: [%u],[%u] ret[%d]",
							   uf->userid, uf->flag , ret );
				   break;
			   }
		case 2:{
				   TATTIRE_ADD *aa=(TATTIRE_ADD*)msg;							
				   ret=cp->user_add_attire(aa->userid, aa->attiretype, 
						   aa->attireid, aa->addcount, 0xFFFFFFFF);
				   DEBUG_LOG ("DEAL add attire: [%u],[%u],[%u],[%u] ret[%d]", 
							   aa->userid, aa->attiretype,aa->attireid,aa->addcount , ret );
				   break;
			   }
		case DV_CHANGE_PASSWD_CMD:{
				   TDV_CHANGE_PASSWD * tcp =(TDV_CHANGE_PASSWD *)msg;							
				   ret=cp-> f_DV_CHANGE_PASSWD(tcp->userid ,tcp->newpasswd);
				   DEBUG_LOG ("DEAL dv change passwd : [%u],[%d]", 
							   tcp->userid, ret );
				   break;
		}
		case DV_REGISTER_CMD:{
				   TDV_REGISTER * tr =(TDV_REGISTER *)msg;							
				   ret=cp-> f_DV_REGISTER(tr->userid,tr->sex,tr->passwd,tr->nick,tr->email);
				   DEBUG_LOG ("DEAL dv change passwd : [%u],[%64.64s][%d]", 
							   tr->userid,tr->email,ret );
				   break;
		}
		case DV_CHANGE_NICK_CMD:{
				   TDV_CHANGE_NICK * tcp =(TDV_CHANGE_NICK *)msg;							
				   ret=cp-> f_DV_CHANGE_NICK(tcp->userid ,tcp->newnick);
				   DEBUG_LOG ("DEAL dv change passwd : [%u],[%d]", 
							   tcp->userid, ret );
				   break;
		}


		default:
			   DEBUG_LOG ("nodefine msg_id%d", msg_id );
			   ret=FAIL;
			   break;
	}	

	if (ret!=SUCC){
		ASC2HEX_3(buf,msg,msglen+4);
		ERROR_LOG ("NEEDDEAL:%s", buf );
	}
	return ret;
}

int 
main ( int argc, char *argv[] )
{
	int i;
	Cclientproto * cp ;
	char recvbuf[4096];
	if (argc!=4){
		printf("need serip serport , msglistkey \n");
		return 1;
	}
			
	
	cp=new Cclientproto( argv[1] ,atoi(argv[2]) );

	/*屏蔽信号*/
	for ( i = 0; i < 32; i ++ )
	{
		if(i == SIGALRM || i == SIGUSR1 )
			continue;
		signal( i, SIG_IGN );
	}
	sigset (SIGUSR2, &sig_exit);
	log_init("../log/" ,8,32000000, "sync_user");
	/*生成精灵进程, 使进程与终端脱节*/
	/*与终端脱节后,scanf等函数将无法使用*/
	switch (fork() )
	{
		case -1:
			return(31);
		case 0:
			break;
		default :
			exit(0);
	}

	int recvlen;
	int qid = msgget( atoi(argv[3]) , IPC_CREAT | 0600 );
	if (qid==-1){
		printf("get msg queue err %d \n",errno );
	}
	//deal
	while( true ) {
		if((recvlen=msgrcv( qid,recvbuf ,sizeof(recvbuf),0, 0 )) == -1){
			continue;
		}
		deal_msg(recvbuf,recvlen,cp );
	}
	delete cp;
}
