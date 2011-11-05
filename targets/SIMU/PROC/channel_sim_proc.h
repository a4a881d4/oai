/*
 * channel_sim_proc.h
 *
 *  Created on: Oct 20, 2011
 *      Author: root
 */

#ifndef CHANNEL_SIM_PROC_H_
#define CHANNEL_SIM_PROC_H_


#endif /* CHANNEL_SIM_PROC_H_ */


void init_mmap_channel(int id,LTE_DL_FRAME_PARMS *frame_parms, double ***s_re,double ***s_im,double ***r_re,double ***r_im,double ***r_re0,double ***r_im0);

void init_mmap(int id,LTE_DL_FRAME_PARMS *frame_parms, double ***s_re,double ***s_im,double ***r_re,double ***r_im,double ***r_re0,double ***r_im0);

void do_DL_sig_eNB(double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX], u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);

void do_DL_sig_ue(double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX], u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);

void clean_param(double **r_re,double **r_im,LTE_DL_FRAME_PARMS *frame_parms);

void do_DL_sig_channel(u8 eNB_i,u8 UE_i,double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX], u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);

void do_UL_sig_ue(double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);

void do_UL_sig_channel(u8 eNB_i,u8 UE_i,double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);


void do_UL_sig_eNB(double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);


void do_DL_sig_channel2(int eNB_idn,int UE_idn,double **r_re0,double **r_im0,double ***r_re,double ***r_im,double ***s_re,double ***s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],
node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX], u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);

void do_UL_sig_channel2(int eNB_idn,int UE_idn,double **r_re0,double **r_im0,double ***r_re,double ***r_im,double ***s_re,double ***s_im,channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],u16 next_slot,u8 abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms);


void do_DL_sig_channel_T(void *param);

void do_UL_sig_channel_T(void *param);

void init_rre(LTE_DL_FRAME_PARMS *frame_parms,double ***r_re0,double ***r_im0);

void channel_add(double **r_re,double **r_im,double **r_re0,double **r_im0,LTE_DL_FRAME_PARMS *frame_parms);
