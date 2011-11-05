#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


#include "SIMULATION/TOOLS/defs.h"
#include "SIMULATION/RF/defs.h"
#include "PHY/types.h"
#include "PHY/defs.h"
#include "PHY/extern.h"
#include "MAC_INTERFACE/extern.h"

#ifdef OPENAIR2
#include "LAYER2/MAC/defs.h"
#include "LAYER2/MAC/extern.h"
#include "UTIL/LOG/log_if.h"
#include "RRC/LITE/extern.h"
#include "PHY_INTERFACE/extern.h"
#include "UTIL/OCG/OCG.h"
#include "UTIL/OPT/opt.h" // to test OPT
#endif

#include "ARCH/CBMIMO1/DEVICE_DRIVER/extern.h"
#ifdef IFFT_FPGA
#include "PHY/LTE_REFSIG/mod_table.h"
#endif

#include "SCHED/defs.h"
#include "SCHED/extern.h"

#ifdef XFORMS
#include "forms.h"
#include "phy_procedures_sim_form.h"
#endif

#include "../USER/oaisim.h"

#include "channel_sim_proc.h"
#include "interface.h"
#include "Tsync.h"
#define RF
#define FILENAMEMAX 255
int fd_UE[8];
int fd_NB[11];
char toto[FILENAMEMAX];
int fd_channel;
//#define DEBUG_SIM
void Process_Func(int node_id,int port,double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
		channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms){

if(node_id<MAX_eNB)
	eNB_Inst(node_id,port,r_re0,r_im0,r_re,r_im,s_re,s_im,eNB2UE,UE2eNB,enb_data,ue_data,abstraction_flag,frame_parms);
else
	UE_Inst(node_id,port,r_re0,r_im0,r_re,r_im,s_re,s_im,eNB2UE,UE2eNB,enb_data,ue_data,abstraction_flag,frame_parms);


}
void UE_Inst(int node_id,int port,double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
		channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms){


	int next_slot,last_slot,slot=0,UE_id=0,eNB_id=0;
	lte_subframe_t direction;
	 printf("UE_Inst Begins\n");
	 init_mmap(node_id,frame_parms, &s_re, &s_im, &r_re, &r_im, &r_re0, &r_im0);
	 Interface_init(port,node_id);
	 char p_input[FILENAMEMAX];
	 int fd_pipe;
	 int fd_channel;
	 fd_channel=open("/tmp/channel",O_RDWR|O_APPEND|O_APPEND,0);
	 sprintf(p_input,"/tmp/pipe_%d",node_id);
	// fprintf(stderr,p_input);
	 fd_pipe=open(p_input,O_RDWR|O_APPEND|O_APPEND,0);
	 IntInitAll();

	  while(1){
	      wait_4slot(&slot,&mac_xface->frame);
		  //read(fd_pipe,&slot,sizeof(slot));
		  //read(fd_pipe,&frame,sizeof(frame));
		 // mac_xface->frame=frame;
	       last_slot = (slot - 1)%20;
	       if (last_slot <0)
	 	last_slot+=20;
	       next_slot = (slot + 1)%20;

	       direction = subframe_select(frame_parms,next_slot>>1);

	       if((next_slot %2) ==0)
	 	clear_eNB_transport_info(1);

	       UE_id=0;
	     //  for (UE_id = oai_emulation.info.first_ue_local; UE_id < (oai_emulation.info.first_ue_local + oai_emulation.info.nb_ue_local); UE_id++)
	        //  	if ((mac_xface->frame) >= (UE_id * 10)) {	// activate UE only after 10*UE_id frames so that different UEs turn on separately
	          	  printf("\n\n[SIM] EMU PHY procedures UE %d for frame %d, slot %d (subframe %d)\n",
	          	     UE_id,mac_xface->frame, slot, (next_slot >> 1));

	          	  if (PHY_vars_UE_g[UE_id]->UE_mode[0] != NOT_SYNCHED) {
	          	    if ((mac_xface->frame)>0) {
	          	      phy_procedures_UE_lte ((last_slot),(next_slot), PHY_vars_UE_g[UE_id], 0, abstraction_flag);
	          	    }
	          	  }
	          	  else {
	          	    if (abstraction_flag==1){
	          	      LOG_E(EMU, "sync not supported in abstraction mode (UE%d,mode%d)\n", UE_id, PHY_vars_UE_g[UE_id]->UE_mode[0]);
	          	      exit(-1);
	          	    }
	          	    if (((mac_xface->frame)>0) && ((last_slot) == (SLOTS_PER_FRAME-1))) {
	          	      initial_sync(PHY_vars_UE_g[UE_id]);
	          	    }
	           	  }
	       //   	}


	       emu_transport (mac_xface->frame, last_slot, next_slot, direction, 0);

	       if ((next_slot % 2) == 0)
	 	clear_UE_transport_info (1);
	       emu_transport (mac_xface->frame, last_slot, next_slot,direction, 0);

	       if (direction  == SF_DL) {
	        	  do_DL_sig_ue(r_re0,r_im0,r_re,r_im,s_re,s_im,eNB2UE,enb_data, ue_data,next_slot,abstraction_flag,frame_parms);
	       }
	       else if (direction  == SF_UL) {
	     	  do_UL_sig_ue(r_re0,r_im0,r_re,r_im,s_re,s_im,UE2eNB,next_slot,abstraction_flag,frame_parms);      }
	       else {
	 	if (next_slot%2==0) {//DL part
	      	  do_DL_sig_ue(r_re0,r_im0,r_re,r_im,s_re,s_im,eNB2UE,enb_data, ue_data,next_slot,abstraction_flag,frame_parms);

	 	}
	 	else {// UL part
	 		 do_UL_sig_ue(r_re0,r_im0,r_re,r_im,s_re,s_im,UE2eNB,next_slot,abstraction_flag,frame_parms);
	 	}
	       }
           //write(fd_channel, "1", 1);
           //fprintf(stderr,"Wrote ue\n");
	       send_exec_complete(CHANNEL_PORT);

	     }

}

void eNB_Inst(int node_id,int port,double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
		channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms){
	int next_slot,last_slot,slot=0,UE_id=0,eNB_id=0;
	lte_subframe_t direction;
	printf("eNB_Inst Begins\n");

	 init_mmap(node_id,frame_parms, &s_re, &s_im, &r_re, &r_im, &r_re0, &r_im0);
     Interface_init(port,node_id);
	 IntInitAll();
	 char p_input[FILENAMEMAX];
	 int fd_pipe;
	 int fd_channel;
	 fd_channel=open("/tmp/channel",O_RDWR|O_APPEND,0);
	 sprintf(p_input,"/tmp/pipe_%d",node_id);
	// fprintf(stderr,p_input);

	 fd_pipe=open(p_input,O_RDWR|O_APPEND,0);

	  while(1){
	      wait_4slot(&slot,&mac_xface->frame);
		  //read(fd_pipe,&slot,sizeof(slot));
		  //read(fd_pipe,&frame,sizeof(frame));
		//  mac_xface->frame=frame;

	  //wait_4slot(&slot,&mac_xface->frame);
	  last_slot = (slot - 1)%20;
	        if (last_slot <0)
	  	last_slot+=20;
	        next_slot = (slot + 1)%20;

	        direction = subframe_select(frame_parms,next_slot>>1);

	        if((next_slot %2) ==0)
	  	clear_eNB_transport_info(1);

	  	printf
	  	  ("\n\n [SIM]EMU PHY procedures eNB %d for frame %d, slot %d (subframe %d) (rxdataF_ext %p) Nid_cell %d\n",
	  	   eNB_id, mac_xface->frame, slot, next_slot >> 1,
	  	   PHY_vars_eNB_g[0]->lte_eNB_ulsch_vars[0]->rxdataF_ext, PHY_vars_eNB_g[eNB_id]->lte_frame_parms.Nid_cell);
	  	phy_procedures_eNB_lte (last_slot, next_slot, PHY_vars_eNB_g[eNB_id], abstraction_flag);

	        emu_transport (frame, last_slot, next_slot, direction, 0);

	        if ((next_slot % 2) == 0)
	  	clear_UE_transport_info (1);

	        emu_transport (frame, last_slot, next_slot,direction, 0);

	        if (direction  == SF_DL) {
	      	  do_DL_sig_eNB(r_re0,r_im0,r_re,r_im,s_re,s_im,eNB2UE,enb_data, ue_data,next_slot,abstraction_flag,frame_parms);

	        }
	        else if (direction  == SF_UL) {
	      	  do_UL_sig_eNB(r_re0,r_im0,r_re,r_im,s_re,s_im,UE2eNB,next_slot,abstraction_flag,frame_parms);      }
	        else {
	  	if (next_slot%2==0) {
	  		  do_DL_sig_eNB(r_re0,r_im0,r_re,r_im,s_re,s_im,eNB2UE,enb_data, ue_data,next_slot,abstraction_flag,frame_parms);
	  	}
	  	else {
	  		do_UL_sig_eNB(r_re0,r_im0,r_re,r_im,s_re,s_im,UE2eNB,next_slot,abstraction_flag,frame_parms);
	  	}
	        }
	           write(fd_channel, "1", 1);
	          // fprintf(stderr,"Wrote enb\n");

	        send_exec_complete(CHANNEL_PORT);
	      }


}


void Channel_Inst(int node_id,int port,double **s_re[10],double **s_im[10],double **r_re[10],double **r_im[10],double **r_re0,double **r_im0,
double **r_re0_d[8][3],double **r_im0_d[8][3],double **r_re0_u[3][8],double **r_im0_u[3][8],channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms){

	 Interface_init(port,node_id);
	 IntInitAll();

	// fd_channel=open("/tmp/channel",O_RDWR|O_APPEND,0);

	int next_slot,last_slot,slot=0,UE_id=0,eNB_id=0;
	lte_subframe_t direction;


	  int ci,ji=0;
	  for(ci=0;ci<NB_eNB_INST;ci++)
	  {
	  init_mmap_channel(ji,frame_parms, &(s_re[ci]), &(s_im[ci]), &(r_re[ci]), &(r_im[ci]), &(r_re0), &(r_im0));
	//  sprintf(toto, "/tmp/pipe_%d",ji);
		// fprintf(stderr,toto);

	  //fd_NB[ci]=open(toto,O_RDWR|O_APPEND,0);
	  ji++;
	  }
	  ji=0;
	  for(ci=NB_eNB_INST;ci<(NB_eNB_INST+NB_UE_INST);ci++)
	  {
	  init_mmap_channel(MAX_eNB+ji,frame_parms, &(s_re[ci]), &(s_im[ci]), &(r_re[ci]), &(r_im[ci]), &(r_re0), &(r_im0));
	  sprintf(toto, "/tmp/pipe_%d",ji);
		// fprintf(stderr,toto);

	  //	  fd_NB[ci]=open(toto,O_RDWR|O_APPEND,0);
	  ji++;
	  }

	  for (eNB_id = 0; eNB_id < NB_eNB_INST; eNB_id++) {
	  for (UE_id = 0; UE_id < NB_UE_INST; UE_id++) {
	  init_rre(frame_parms,&(r_re0_u[eNB_id][UE_id]),&(r_im0_u[eNB_id][UE_id]));
	  init_rre(frame_parms,&(r_re0_d[UE_id][eNB_id]),&(r_im0_d[UE_id][eNB_id]));
	  }
	  }


	 for(eNB_id=0;eNB_id<NB_eNB_INST;eNB_id++){
	 for(UE_id=0;UE_id<NB_UE_INST;UE_id++){
	 e2u_t[eNB_id][UE_id]=(ch_thread*)calloc(1,sizeof(ch_thread));
	 }}

	 for(UE_id=0;UE_id<NB_UE_INST;UE_id++){
	 for(eNB_id=0;eNB_id<NB_eNB_INST;eNB_id++){
	 u2e_t[UE_id][eNB_id]=(ch_thread*)calloc(1,sizeof(ch_thread));
	 }}

	 pthread_t cthr_u[NB_eNB_INST][NB_UE_INST];
	 pthread_t cthr_d[NB_UE_INST][NB_eNB_INST];

	    //pthread_cond_init (&downlink_cond, NULL);
	    //pthread_cond_init (&uplink_cond, NULL);
	    if (pthread_cond_init (&downlink_cond_channel, NULL)) exit(1);
	    if (pthread_cond_init (&uplink_cond_channel, NULL)) exit(1);
	 	//if (   pthread_mutex_lock(&downlink_mutex_channel)) exit(1);
	 	//if (   pthread_mutex_lock(&uplink_mutex_channel)) exit(1);

	     NUM_THREAD_DOWNLINK=0;
	    for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++){
	  	  for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
		  	u2e_t[UE_id][eNB_id]->thread_id=NUM_THREAD_DOWNLINK;
	  		u2e_t[UE_id][eNB_id]->eNB_id=eNB_id;
	  		u2e_t[UE_id][eNB_id]->UE_id=UE_id;
	  		u2e_t[UE_id][eNB_id]->r_re0=r_re0_d[UE_id][eNB_id];
	  		u2e_t[UE_id][eNB_id]->r_im0=r_im0_d[UE_id][eNB_id];
	  		u2e_t[UE_id][eNB_id]->r_re=r_re[NB_eNB_INST+UE_id];
	  		u2e_t[UE_id][eNB_id]->r_im=r_im[NB_eNB_INST+UE_id];
	  		u2e_t[UE_id][eNB_id]->s_im=s_im[eNB_id];
	  		u2e_t[UE_id][eNB_id]->s_re=s_re[eNB_id];
	  		u2e_t[UE_id][eNB_id]->eNB2UE=eNB2UE[eNB_id][UE_id];
	  		u2e_t[UE_id][eNB_id]->UE2eNB=UE2eNB[UE_id][eNB_id];
	  		u2e_t[UE_id][eNB_id]->enb_data=enb_data[eNB_id];
	  		u2e_t[UE_id][eNB_id]->ue_data=ue_data[UE_id];
	  		u2e_t[UE_id][eNB_id]->next_slot=&next_slot;
	  		u2e_t[UE_id][eNB_id]->abstraction_flag=&abstraction_flag;
	  		u2e_t[UE_id][eNB_id]->frame_parms=frame_parms;
	  	  if(pthread_cond_init (&downlink_cond[eNB_id][UE_id], NULL)) exit(1);
	  		 	   //if(pthread_mutex_lock(&downlink_mutex[eNB_id][UE_id])) exit(1);
	  	pthread_create (&cthr_d[UE_id][eNB_id], NULL, do_DL_sig_channel_T,(void*)(u2e_t[UE_id][eNB_id]));
	  	NUM_THREAD_DOWNLINK++;
	    }
	    }

	     NUM_THREAD_UPLINK=0;
	  	  for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
	  	    for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++){
			e2u_t[eNB_id][UE_id]->thread_id=NUM_THREAD_UPLINK;
			e2u_t[eNB_id][UE_id]->eNB_id=eNB_id;
     		e2u_t[eNB_id][UE_id]->UE_id=UE_id;
     		e2u_t[eNB_id][UE_id]->r_re=r_re[eNB_id];
     		e2u_t[eNB_id][UE_id]->r_im=r_im[eNB_id];
     		e2u_t[eNB_id][UE_id]->r_re0=r_re0_u[eNB_id][UE_id];
     		e2u_t[eNB_id][UE_id]->r_im0=r_im0_u[eNB_id][UE_id];
     		e2u_t[eNB_id][UE_id]->s_im=s_im[NB_eNB_INST+UE_id];
     		e2u_t[eNB_id][UE_id]->s_re=s_re[NB_eNB_INST+UE_id];
     		e2u_t[eNB_id][UE_id]->eNB2UE=eNB2UE[eNB_id][UE_id];
     		e2u_t[eNB_id][UE_id]->UE2eNB=UE2eNB[UE_id][eNB_id];
     		e2u_t[eNB_id][UE_id]->enb_data=enb_data[eNB_id];
     		e2u_t[eNB_id][UE_id]->ue_data=ue_data[UE_id];
     		e2u_t[eNB_id][UE_id]->next_slot=&next_slot;
     		e2u_t[eNB_id][UE_id]->abstraction_flag=&abstraction_flag;
     		e2u_t[eNB_id][UE_id]->frame_parms=frame_parms;
     		if(pthread_cond_init (&uplink_cond[UE_id][eNB_id], NULL)) exit(1);
     		//if(pthread_mutex_lock(&uplink_mutex[UE_id][eNB_id])) exit(1);
	        pthread_create (&cthr_u[eNB_id][UE_id], NULL, do_UL_sig_channel_T,(void*)e2u_t[eNB_id][UE_id]);
	        NUM_THREAD_UPLINK++;
       }
          }


}

void Channel_DL(double **s_re[10],double **s_im[10],double **r_re[10],double **r_im[10],double **r_re0,double **r_im0,
double **r_re0_d[8][3],double **r_im0_d[8][3],double **r_re0_u[3][8],double **r_im0_u[3][8],channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms,int slot){

	  int count=0;
	  int next_slot,last_slot,UE_id=0,eNB_id=0;
	  lte_subframe_t direction;

  	  for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
  		  //write(fd_NB[eNB_id],&slot,sizeof(slot));
  		//write(fd_NB[eNB_id],&(mac_xface->frame),sizeof(mac_xface->frame));
	  send_exec_msg(mac_xface->frame,slot,eNB_PORT+eNB_id);
	  }

	  count=0;
  	  while(count<NB_eNB_INST){
	//  char titi;
	  //read(fd_channel,&titi,1);
  	  wait_4Msg();
	  count++;
	  }

       _COT=0;
  	   for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++){
  	   for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
  	   if(pthread_cond_signal(&downlink_cond[eNB_id][UE_id])) exit(1);
  	  // if(pthread_mutex_unlock(&downlink_mutex[eNB_id][UE_id])) exit(1);
  	   }
  	   }

  	    //pthread_mutex_lock(&downlink_mutex_channel);

  	   if(pthread_cond_wait(&downlink_cond_channel, &downlink_mutex_channel)) exit(1);

 	    //pthread_mutex_unlock(&downlink_mutex_channel);


for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++){
clean_param(r_re[NB_eNB_INST+UE_id],r_im[NB_eNB_INST+UE_id],frame_parms);
for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
//channel_add(r_re[NB_eNB_INST+UE_id],r_im[NB_eNB_INST+UE_id],r_re0_d[UE_id][eNB_id],r_im0_d[UE_id][eNB_id],frame_parms);
alpha_channel(eNB_id,UE_id,r_re[NB_eNB_INST+UE_id],r_im[NB_eNB_INST+UE_id],r_re0_d[UE_id][eNB_id],r_im0_d[UE_id][eNB_id],frame_parms);
}
}

//Just to check whether it works without interference , pls delete it after use
//for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++){
//clean_param(r_re[NB_eNB_INST+UE_id],r_im[NB_eNB_INST+UE_id],frame_parms);
//channel_add(r_re[NB_eNB_INST+UE_id],r_im[NB_eNB_INST+UE_id],r_re0_d[UE_id][UE_id],r_im0_d[UE_id][UE_id],frame_parms);
//}

count=0;
for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++)
if (mac_xface->frame >= (UE_id * 10)) {
	//  write(fd_NB[NB_eNB_INST+UE_id],&slot,sizeof(slot));
//	write(fd_NB[NB_eNB_INST+UE_id],&(mac_xface->frame),sizeof(mac_xface->frame));
send_exec_msg(mac_xface->frame,slot,UE_PORT+UE_id);
	count++;
}

while(count--){
	 // char titi;
	//  read(fd_channel,&titi,1);
	wait_4Msg();
}

}


void Channel_UL(double **s_re[10],double **s_im[10],double **r_re[10],double **r_im[10],double **r_re0,double **r_im0,
double **r_re0_d[8][3],double **r_im0_d[8][3],double **r_re0_u[3][8],double **r_im0_u[3][8],channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms,int slot){

	  int count=0;
	  int next_slot,last_slot,UE_id=0,eNB_id=0;
	  lte_subframe_t direction;
	  for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++)
	  if (mac_xface->frame >= (UE_id * 10)) {
		//  write(fd_NB[NB_eNB_INST+UE_id],&slot,sizeof(slot));
		//write(fd_NB[NB_eNB_INST+UE_id],&(mac_xface->frame),sizeof(mac_xface->frame));
	  send_exec_msg(mac_xface->frame,slot,UE_PORT+UE_id);
	  }
	  else{
	  count++;
	  }
	  while(count<NB_UE_INST){
		  char titi;
		//  read(fd_channel,&titi,1);
	  wait_4Msg();
	  count++;
	  }

	  _COT_U=0;

	  for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
	  for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++){
	  if ( pthread_cond_signal(&uplink_cond[UE_id][eNB_id])) exit(1);
	//  if ( pthread_mutex_unlock(&uplink_mutex[UE_id][eNB_id])) exit(1);
	  }}
	  if ( pthread_cond_wait(&uplink_cond_channel, &uplink_mutex_channel)) exit(1);

	  for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
	  clean_param(r_re[eNB_id],r_im[eNB_id],frame_parms);
	  for (UE_id = 0; UE_id < (NB_UE_INST); UE_id++){
	  //channel_add(r_re[eNB_id],r_im[eNB_id],r_re0_u[eNB_id][UE_id],r_im0_u[eNB_id][UE_id],frame_parms);
	  alpha_channel(eNB_id,UE_id,r_re[eNB_id],r_im[eNB_id],r_re0_u[eNB_id][UE_id],r_im0_u[eNB_id][UE_id],frame_parms);
	  }
	  }

	  //Just to check whether it works without interference , pls delete it after use
//	  for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
//	  clean_param(r_re[eNB_id],r_im[eNB_id],frame_parms);
//	  channel_add(r_re[eNB_id],r_im[eNB_id],r_re0_u[eNB_id][eNB_id],r_im0_u[eNB_id][eNB_id],frame_parms);
//	  }

	    count=0;
		for (eNB_id=0;eNB_id<(NB_eNB_INST);eNB_id++) {
	  		//  write(fd_NB[eNB_id],&slot,sizeof(slot));
	  	//	write(fd_NB[eNB_id],&(mac_xface->frame),sizeof(mac_xface->frame));
	  	send_exec_msg(mac_xface->frame,slot,eNB_PORT+eNB_id);
		}

	  	while(count<NB_eNB_INST){
	  //	  char titi;
	  	//  read(fd_channel,&titi,1);
	  	wait_4Msg();
	  	count++;
	  	}

}

void Channel_Func(double **s_re[10],double **s_im[10],double **r_re[10],double **r_im[10],double **r_re0,double **r_im0,
double **r_re0_d[8][3],double **r_im0_d[8][3],double **r_re0_u[3][8],double **r_im0_u[3][8],channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms,int slot){
	int next_slot,last_slot,UE_id=0,eNB_id=0,dir_flag;
	lte_subframe_t direction;

	last_slot = (slot - 1)%20;
	if (last_slot <0)
	last_slot+=20;
	next_slot = (slot + 1)%20;

	direction = subframe_select(frame_parms,next_slot>>1);

	 if (direction  == SF_DL) {
	 dir_flag=1;
	 }
	 else if (direction  == SF_UL) {
	 dir_flag=2;
	 }
	 else {
	 if (next_slot%2==0) {
	 dir_flag=1;
	 }
	 else {
	 dir_flag=2;
	 }
	 }

	 if(dir_flag==1)
	 {
	 Channel_DL(s_re,s_im,r_re,r_im,r_re0,r_im0,r_re0_d,r_im0_d,r_re0_u,r_im0_u,eNB2UE,UE2eNB,enb_data,ue_data,abstraction_flag,frame_parms,slot);
	 }

	 if(dir_flag==2)
	 {
	 Channel_UL(s_re,s_im,r_re,r_im,r_re0,r_im0,r_re0_d,r_im0_d,r_re0_u,r_im0_u,eNB2UE,UE2eNB,enb_data,ue_data,abstraction_flag,frame_parms,slot);
	 }

}
