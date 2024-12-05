/***********************************************************************
** Copyright (C) ORBBEC Technology Group Co., Ltd. All rights reserved.
** Author		: chenjialuo
** Date			: 2021-02-02
** Name			: OrbDataTyp.h
** Version		: 1.0
** Description 	: 
** Modify Record:
1: creat on 2021.02.02. 
   
***********************************************************************/

#ifndef _ORB_DATA_TYP_H_
#define _ORB_DATA_TYP_H_

//different serial protocol macro
//#define PEEPHOLE_PROTOCOL_TYP_3V 
#define PEEPHOLE_PROTOCOL_TYP_K1S 
//#define PEEPHOLE_PROTOCOL_TYP_ORB


typedef enum PeepholeProtocolTyp{
    PeepholeProtocolTyp3V  = 0x00,
    PeepholeProtocolTypK1S = 0x01, //senseTime
    PeepholeProtocolTypOrb = 0x02, //orbbec
    PeepholeProtocolTypOth = 0x03, 
    PeepholeProtocolTypEnd
}SerialProtocolTyp;


typedef enum SerialCmdParseSta{
    SerialCmdParseStaClientBegin=0,
    SerialCmdParseStaClientRun,
    SerialCmdParseStaClientFinish,
    SerialCmdParseStaOrbBegin,
    SerialCmdParseStaOrbRun,
    SerialCmdParseStaOrbFinish,
    SerialCmdParseStaEnd
}SerialCdmParseSta;

typedef enum{
    MASTER_WAKE_UP_NORMAL = 0x00,
    MASTER_WAKE_UP_PIR,
    MASTER_WAKE_UP_NETWORKING,
    /*GPIO START*/
    MASTER_WAKE_UP_DOOR_BELL,
    MASTER_WAKE_UP_ALARM_TAMPER,
    MASTER_WAKE_UP_TRY_ERR,
    MASTER_WAKE_UP_FACTORYRESET,
    MASTER_WAKE_UP_ALARM_DEFENCE,
    /*GPIO END*/

    MASTER_WAKE_UP_NUMBER,
    MASTER_WAKE_UP_END
}MASTER_WAKE_UP_CODE;

typedef enum {
    wake_up_typ_normal=0,//power on
    wake_up_typ_pir,
    wake_up_typ_networking,
    wake_up_typ_gpio,
    wake_up_typ_lcd,
    wake_up_typ_door_bell,
    wake_up_typ_alarm_tamper,
    wake_up_typ_try_err,
    wake_up_typ_reset,
    wake_up_typ_alarm_defence,
    wake_up_typ_number, 
    wake_up_typ_end
}wake_up_typ;

typedef enum {
    wifi_action_typ_live=0,
    wifi_action_typ_sleep,
    wifi_action_typ_standard,
    wifi_action_typ_end
}wifi_action_typ;

typedef enum {
    lock_event_typ_no=0,
    lock_event_typ_door_bell,
    lock_event_typ_door_open,
    lock_event_typ_door_close,
    lock_event_typ_door_locked,
    lock_event_typ_door_locked_cancel,
    lock_event_typ_door_reset,
    lock_event_typ_low_battery,
    lock_event_typ_door_try_err,
    lock_event_typ_end
}lock_event_typ;


typedef enum {
    lock_alarm_typ_no=0,
    lock_alarm_typ_tamper,
    lock_alarm_typ_intruder,
    lock_alarm_typ_ban_test,
    lock_alarm_typ_defence,
    lock_alarm_typ_fake_lock,
    lock_alarm_typ_end
}lock_alarm_typ;

//
typedef enum{
    net_sta_err_typ_none=0,
    net_sta_err_typ_wifi_connection_fail,
    net_sta_err_typ_cloud_connection_fail,
    net_sta_err_typ_cloud_timeout,
    net_sta_err_typ_end
}net_sta_err_typ;

typedef enum {
    net_sta_typ_normal=0,
    net_sta_typ_config_ssid,
    net_sta_typ_wifi_succ,
    net_sta_typ_config_fail,
    net_sta_typ_cloud_connect,
    net_sta_typ_cloud_disconnect,
    net_sta_typ_err,
    net_sta_typ_end
}net_sta_typ;

typedef enum {
    media_rec_sta_typ_video=0,
    media_rec_sta_typ_audio,
    media_rec_sta_typ_working,
    media_rec_sta_typ_finish,
    media_rec_sta_typ_end
}media_rec_sta_typ;

typedef enum {
    media_sta_err_typ_none=0,
    media_sta_err_typ_videocall_fail,
    media_sta_err_typ_videocall_rejected,
    media_sta_err_typ_cap_notify_cloud_failed,
    media_sta_err_typ_passthrough_failed,
    media_sta_err_typ_capture_video_failed,
    media_sta_err_typ_capture_audio_failed,
    media_sta_err_typ_open_cam_failed,
    media_sta_err_typ_open_audio_failed,
    media_sta_err_typ_unknown,
    media_sta_err_typ_end
}media_sta_err_typ;

typedef enum {
    media_sta_typ_idle=0,
    media_sta_typ_waiting,
    media_sta_typ_working,
    media_sta_typ_error,
    media_sta_typ_end
}media_sta_typ;

typedef enum{
    ota_sta_err_typ_none=0,
    ota_sta_err_typ_ver,
    ota_sta_err_typ_query_timeout,
    ota_sta_err_typ_check_fail,
    ota_sta_err_typ_update_fail,
    ota_sta_err_typ_end
}ota_sta_err_typ;

typedef enum {
    ota_sta_typ_idle=0,
    ota_sta_typ_begin,
    ota_sta_typ_download,
    ota_sta_typ_download_finish,
    ota_sta_typ_unzip,
    ota_sta_typ_wifi,
    ota_sta_typ_3d_cam,
    ota_sta_typ_3d_alg,
    ota_sta_typ_ipc,
    ota_sta_typ_doing,
    ota_sta_typ_finish,
    ota_sta_typ_err,
    ota_sta_typ_end
}ota_sta_typ;

typedef enum {
    rec_stream_typ_video=0,
    //rec_stream_typ_sub_video,
    rec_stream_typ_audio,
    rec_stream_typ_av,
    rec_stream_typ_end,
}rec_stream_typ;

typedef enum {
    sub_sta_typ_none=0,
    sub_sta_typ_doing,
    sub_sta_typ_finish,
    sub_sta_typ_err,
    sub_sta_end,
}sub_sta_typ;

typedef enum {
    device_sta_sub_typ_none=0x00,
    device_sta_sub_typ_net,
    device_sta_sub_typ_ota,
    device_sta_sub_typ_stream,
    device_sta_sub_typ_rec,
    device_sta_sub_typ_playback,
    device_sta_sub_typ_cloud,
    device_sta_sub_typ_sleep,
    device_sta_sub_typ_wake_up,
    device_sta_sub_typ_time,
    device_sta_sub_typ_end
}device_sta_sub_typ;

typedef enum {
    device_sta_typ_none=0,
    device_sta_typ_idle,
    device_sta_typ_working,
    device_sta_typ_waiting,
    device_sta_typ_error,
    device_sta_typ_end
}device_sta_typ;

typedef enum {
    cmd_msg_resp_typ_data_through=0,
    cmd_msg_resp_typ_snap,
    cmd_msg_resp_typ_end,
}device_msg_typ;

typedef struct {
    uint8_t sta; //ota_sta_typ
    uint8_t err; //ota_sta_err_typ
    uint8_t progress; //[0-100]
    uint8_t reserved[5];
}ota_state_struct;

typedef struct SerialProtocolCmdData
{
    uint8_t data[2048];
    uint32_t dataLen;
    uint32_t cmdlen; //parse cmd len
}SerialProtocolCmdData;

typedef struct 
{
    uint8_t sta;
    uint8_t err;
    uint32_t progress;
    uint32_t time;
    uint8_t  reserved[2];
}device_sta_data;

typedef struct device_state_param
{
    uint8_t cursta;
    bool bSendWait;
    uint32_t curWaitTime;
}device_state_param;


typedef struct wifi_event_data{
    bool bEvent;
    int event;
}wifi_event_data;

typedef enum event_media_typ{
    event_media_typ_video=0,
    event_media_typ_audio=1,
    event_media_typ_av =2,
    event_media_typ_end
}event_media_typ;
typedef enum event_alarm_action{
    event_alarm_action_none=0,
    event_alarm_action_snap,
    event_alarm_action_rec_audio,
    event_alarm_action_rec_video,
    event_alarm_action_rec_av,
    event_alarm_action_end
}event_alarm_action;

#endif 
