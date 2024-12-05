#ifndef _PEEP_HOLE_SERIAL_K1S_DATA_TYP_H_
#define _PEEP_HOLE_SERIAL_K1S_DATA_TYP_H_

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

#define PEEPHOLE_K1S_TAG1 (0xEF)
#define PEEPHOLE_K1S_TAG2 (0xAA)

typedef enum K1SMsgTyp{
    K1SMsgTypReply=0x00,
    K1SMsgTypNote =0x01,
    K1SMsgTypImg  =0x02,
    K1SMsgTypEnd
}K1SMsgTyp;

typedef enum K1SMid{
    K1S_MID_INIT=0x25,
    K1S_MID_INIT_RESP=0x50,
    K1S_MID_ENCKEY=0x53,
    K1S_MID_TRIGGER_CAPTURE=0x19,
    K1S_MID_DELETE_MEDIA_FILE=0x27,
    K1S_MID_SET_VOLUME=0x1F,
    K1S_MID_GET_VERSION=0x30,
    K1S_MID_START_OTA=0x40,
    K1S_MID_STOP_OTA=0x41,
    K1S_MID_CONFIG_BAUDRATE=0x51,
    K1S_MID_CONFIG_WIFI=0x70,
    K1S_MID_GET_WIFIMAC=0x71,
    K1S_MID_GET_WIFISTATUS=0x72,
    K1S_MID_SET_LINKKEY=0x82,
    K1S_MID_DATA_PASSTHROUGH=0x83,
    K1S_MID_TRIGGER_VIDEOCALL=0x84,
    K1S_MID_GET_DEVICENAME=0x85,
    K1S_MID_GET_MODULE_STATUS=0x86,
    K1S_MID_POWERDOWN=0xED,
    K1S_MID_DEBUG_MODE=0xF0,
    K1S_MID_GET_DEBUG_INFO=0xF1,
    K1S_MID_UPLOAD_DEBUG_INFO=0xF2,
    K1S_MID_HIJACK = 0xB2,
    K1S_MID_END
}K1SMid;

typedef enum K1SNid{
    K1S_NID_READY=0x00,//orbbec code
    K1S_NID_FACE_STATE=1,
    K1S_NID_UNKNOWNERROR=2,
    K1S_NID_OTA_STATE=3,
    K1S_NID_EYE_STATE=4,
    K1S_NID_MEDIA_CAP_STATE=5,
    K1S_NID_MEDIA_STATE=6,
    K1S_NID_MEDIA_PLAYBACK_STATE = 7,
    K1S_NID_END
}K1SNid;

typedef enum{
    K1S_NOTE_OTA_STATE_IDLE=0,
    K1S_NOTE_OTA_STATE_WORKING=1,
    K1S_NOTE_OTA_STATE_DONE=2,
    K1S_NOTE_OTA_STATE_ERROR=3,
    K1S_NOTE_OTA_STATE_END, 
}K1SNoteOtaState;

typedef enum{
   K1S_NOTE_OTA_ERROR_NONE             = 0,
   K1S_NOTE_OTA_ERROR_NO_SUCH_VERSION  = 1,
   K1S_NOTE_OTA_ERROR_QUERY_TIMEOUT    = 2,
   K1S_NOTE_OTA_ERROR_MD5_CHECK_FAILED = 3,
   K1S_NOTE_OTA_ERROR_UPDATE_FAILED    = 4,
   K1S_NOTE_OTA_ERROR_END  
}K1SNoteOtaError;

typedef enum{
    K1S_NOTE_MEDIA_STATE_IDLE      = 0,    // Module is idle, can be powered off immediately
    K1S_NOTE_MEDIA_STATE_WAITING   = 1,    // Module is waiting for the next command
    K1S_NOTE_MEDIA_STATE_WORKING   = 2,    // Module is working on media business
    K1S_NOTE_MEDIA_STATE_ERROR     = 3,    // An error occurred in the Module 
    K1S_NOTE_MEDIA_STATE_END
}K1SNoteMediaState;

typedef enum{
    K1S_NOTE_MEDIA_ERROR_NONE                      = 0,   // no error message
    K1S_NOTE_MEDIA_ERROR_WIFI_CONNECTION_FAILED    = 1,    // failed to connect wifi
    K1S_NOTE_MEDIA_ERROR_CLOUD_CONNECTION_FAILED   = 2,    // failed to connect cloud
    K1S_NOTE_MEDIA_ERROR_CLOUD_TIMEOUT             = 3,    // cloud timeout, given in VideoCall timeout or trigger cloud storage timeout 
    K1S_NOTE_MEDIA_ERROR_VIDEOCALL_FAILED          = 11,   // notify cloud failed when trigger video call
    K1S_NOTE_MEDIA_ERROR_VIDEOCALL_REJECTED        = 12,   // video call success but user rejected this command
    K1S_NOTE_MEDIA_ERROR_CAP_NOTIFY_CLOUD_FAILED   = 13,   // notify cloud failed after video capture
    K1S_NOTE_MEDIA_ERROR_PASSTHROUGH_FAILED        = 14,   // failed to pass through data
    K1S_NOTE_MEDIA_ERROR_CAPTURE_VIDEO_FAILED      = 21,   // failed to capture video
    K1S_NOTE_MEDIA_ERROR_CAPTURE_AUDIO_FAILED      = 22,   // failed to capture audio
    K1S_NOTE_MEDIA_ERROR_OPEN_CAMERA_FAILED        = 23,   // failed to open camera
    K1S_NOTE_MEDIA_ERROR_OPEN_AUDIO_DEVICE_FAILED  = 24,   // failed to open audio device
    K1S_NOTE_MEDIA_ERROR_UNKNOWNREASON             = 50,   // failed with unknown reason
    K1S_NOTE_MEDIA_ERROR_End
}K1SNoteMediaError;


typedef enum{
    K1S_MR_SUCCESS    = 0,     // success
    K1S_MR_REJECTED   = 1,     // module rejected this command
    K1S_MR_ABORTED    = 2,     // algo aborted
    K1S_MR_FAILED4_CAMERA = 4, // camera open failed
    K1S_MR_FAILED4_UNKNOWNREASON = 5, // UNKNOWN_ERROR
    K1S_MR_FAILED4_INVALIDPARAM = 6,  // invalid param
    K1S_MR_FAILED4_NOMEMORY = 7,      // no enough memory
    K1S_MR_FAILED4_UNKNOWNUSER = 8,   // no user enrolled
    K1S_MR_FAILED4_MAXUSER = 9,       // exceed maximum user number
    K1S_MR_FAILED4_FACEENROLLED = 10,// this face has been enrolled
    K1S_MR_FAILED4_LIVENESSCHECK = 12,// liveness check failed
    K1S_MR_FAILED4_TIMEOUT = 13,     // exceed the time limit
    K1S_MR_FAILED4_AUTHORIZATION = 14,// authorization failed
    K1S_MR_FAILED4_CAMERAFOV = 15,    // camera fov test failed
    K1S_MR_FAILED4_CAMERAQUA = 16,   // camera quality test failed
    K1S_MR_FAILED4_CAMERASTRU = 17,   // camera structure test failed
    K1S_MR_FAILED4_BOOT_TIMEOUT = 18, // boot up timeout
    K1S_MR_FAILED4_READ_FILE = 19,   // read file failed
    K1S_MR_FAILED4_WRITE_FILE = 20,   // write file failed
    K1S_MR_FAILED4_NO_ENCRYPT = 21,   // encrypt must be set
    K1S_MR_FAILED4_NO_RGBIMAGE= 23,   // rgb image is not ready
    K1S_MR_FAILED4_NO_WLAN                = 24, // wlan is not connected
    K1S_MR_FAILED4_MQTT_CONNECT           = 25, // mqtt connect fail
    K1S_MR_FAILED4_DM_CONNECT             = 26, // dm connect fail
    K1S_MR_FAILED4_LOWPOWER_CONNECT       = 27, // lowpower connect fail
    K1S_MR_FAILED4_GENERATE_WAKEUP_CMD    = 28, // generate wakeup command fail
    K1S_MR_FAILED4_GENERATE_HEARTBEAT_CMD = 29, // generate heartbeat command fail
    K1S_MR_FAILED4_MAKE_CONFIG_FILE       = 30, // make config file fail
}K1SResCode;

typedef enum {
    K1S_EVENT_TYP_ANTI_PRIZING=1,
    K1S_EVENT_TYP_BAN_TRY=3,
    K1S_EVENT_TYP_PROTECTION=4,
    K1S_EVENT_TYP_LOCK_ERR=5,
    K1S_EVENT_TYP_END
}k1s_event_typ;

typedef struct{
    uint8_t state; //K1SNoteOtaState
    uint8_t err;   //K1SNoteOtaError
    uint8_t progress; //value in [0,100]
    uint8_t reserved[5];
}k1s_note_ota_data;

typedef struct {
    uint8_t media_type;        //0:video;1:audio
    uint8_t recorded_time_s[4]; // capture progress
    uint8_t is_finished;        // 1 is_finished
    uint8_t capture_state;
} k1s_note_media_cap_data;

typedef struct {
    uint8_t msg_state;          // reference s_mstate
    uint8_t media_state;        // reference MEDIA_STATE_*, if media_state is MEDIA_STATE_ERROR reference media_error to find the reason
    uint8_t media_error;        // reference MEDIA_ERROR_*
    uint8_t unlock_state;       // 1: unlock business is working, 0: unlock business is not working
    uint8_t reserved[4];        // reserved feild
} k1s_msg_reply_get_module_status;

typedef struct {
    uint8_t media_type;
    uint8_t capture_time_s[4]; // int = (a[0] << 24) + (a[1] << 16) + (a[2] << 8) + a[3];
    uint8_t enable_cloud_storage;     // upload the captured video to cloud, 1:enable 0:disable
    uint8_t video_capture_type;       // video capture type, effective when media_type is ST_MEDIA_TYPE_VIDEO
    uint8_t reserved[1];    // reservie some field
}k1s_msg_trigger_capture_data;

typedef struct {
    uint8_t connect_status;  // 1: wifi is connected; other: not connected
    int8_t signal_level;    //一般信号强度在-30~-120之间。正常信号强度应该是-40 dbm ~ -85 dbm之间。
    int8_t noise; //snr
}k1s_msg_reply_get_wifistatus_data;

typedef struct {
    uint8_t seed[4]; // random data as a seed
    uint8_t mode;    // reserved for selecting encrytion mode
    uint8_t crttime[4]; // current time - this is the interface that module sync time with host
    uint8_t enable_network;// enable network
    uint8_t waiting_time;  // waiting time after process media business
    uint8_t reserved[5]; // reservie some field
} k1s_msg_init_data;

typedef struct {
    uint8_t tag1;
    uint8_t tag2;
    uint8_t msgId;
    uint16_t size;
    uint8_t  data[2048];
    uint8_t  parity;
}k1s_cmd_msg;

typedef struct {
    uint8_t tag1;
    uint8_t tag2;
    uint8_t typ;
    uint16_t size;
    uint8_t rid;
    uint8_t result;
    uint8_t data[1024];
    uint8_t parity;
}k1s_reply_msg;

typedef struct {
    uint8_t tag1;
    uint8_t tag2;
    uint8_t typ;
    uint16_t size;
    uint8_t nid;
    uint8_t data[1024];
    uint8_t parity;   
}k1s_note_msg;

typedef struct{
    uint8_t tag1;
    uint8_t tag2;
    uint16_t size;
    uint8_t  encData[1024];
    uint8_t parity;
}k1s_encryp_msg;

typedef struct{
    //uint8_t encryp;//0:no encrypt,1:encrypt
    uint8_t algTyp; //0:no,1;aes
    uint8_t key[32]; 
    uint8_t keylen;
}k1s_encryp_info;

#endif //
