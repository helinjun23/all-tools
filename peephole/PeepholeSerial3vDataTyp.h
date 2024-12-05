#ifndef _PEEPHOLE_SERIAL_3V_DATA_TYP_H_
#define _PEEPHOLE_SERIAL_3V_DATA_TYP_H_

#include <stdint.h>


#define PEEP_HOLE_3V_TAG        0x7B   
#define PEEP_HOLE_3V_RESERVE    0x00

#define PEEP_HOLE_3V_MANUFACTURER  0x03 //orbbec Manufacturer,get from DSM

typedef enum WIFI_3V_STATE{
    WIFI_3V_STATE_CONFIG_NET=0,
    WIFI_3V_STATE_CONNECT=1, //ssid and pwd success,connect wifi hot
    WIFI_3V_STATE_CONNECT_SUCCESS=2,
    WIFI_3V_STATE_CONFIG_FAIL=3,
    WIFI_3V_STATE_NO_CONFIG_NET=7,
    WIFI_3V_STATE_AIOT_CONNECT_SUCCESS=8,
    WIFI_3V_STATE_END
}WIFI_3V_STATE_ENUM;

typedef enum {
    IPC_3V_WAKEUP_POWERON=0,
    IPC_3V_WAKEUP_GPIO=1,
    IPC_3V_WAKEUP_PIR=2,
    IPC_3V_WAKEUP_RESET=3,
    IPC_3V_WAKEUP_LCD=4,
    IPC_3V_WAKEUP_OTHER=5,
    IPC_3V_WAKEUP_END
}IPC_3V_WAKEUP;

typedef enum{
    LOCK_3V_ALARM_NO=0x00,
    LOCK_3V_ALARM_TAMPER=0x01,
    LOCK_3V_ALARM_INTRUDER=0x02,
    LOCK_3V_ALARM_TEST_BAN=0x03,
    LOCK_3V_ALARM_DEFENCE =0x04,
    LOCK_3V_ALARM_END
}LOCK_3V_ALARM;

typedef enum{
    LOCK_3V_EVENT_NO=0x00,
    LOCK_3V_EVENT_DOOR_BELL=0x01,
    LOCK_3V_EVENT_DOOR_OPEN=0x02,
    LOCK_3V_EVENT_DOOR_CLOSE=0x03,
    LOCK_3V_EVENT_DOOR_LOCKED=0x04,
    LOCK_3V_EVENT_DOOR_LOCKED_CANCEL=0x05,
    LOCK_3V_EVENT_DOOR_RESET_FACTORY=0x06,
    LOCK_3V_EVENT_LOW_BATTERY_LEVEL=0x07,// low battery level
    LOCK_3V_EVENT_DOOR_TRY_ERR=0x08,
    LOCK_3V_EVENT_END
}LOCK_3V_EVNET;

enum PeepHole3VCmdResTyp{
    PeepHole3VCmdResTypFail=0x00,
    PeepHole3VCmdResTypSucc=0x01,
    PeepHole3VCmdResTypEnd
};

enum PeepHole3VCmdTyp{
    PeepHole3VCmdTypNoNetConfig =0x80,
    PeepHole3VCmdTypNetConfig   =0x82,
    PeepHole3VCmdTypUartPrepareNote =0x90,
    PeepHole3VCmdTypUartPrepareResp =0x91,
    PeepHole3VCmdTypWifiStaNote =0xc5,
    PeepHole3VCmdTypPirCtl      =0xc7,
    PeepHole3VCmdTypAlarmEvent  =0xca,
    PeepHole3VCmdTypPicDir      =0xd1,
    PeepHole3VCmdTypAliKey      =0xd5,
    PeepHole3VCmdTypWifiSta     =0xd6,
    PeepHole3VCmdTypMac         =0xd7,
    PeepHole3VCmdTypSsidPwd     =0xd8,
    PeepHole3VCmdTypDataTrans   =0xd9,
    PeepHole3VCmdTypDevName     =0xda,
    PeepHole3VCmdTypIP          =0xdb,
    PeepHole3VCmdTypIpcWakeNote =0xdc,
    PeepHole3VCmdTypSleep       =0xde,
    PeepHole3VCmdTypSendNetTime =0xc2,
    PeepHole3VCmdTypeGetNetTime =0xA0,
    PeepHole3VCmdTypeGetDeviceSn =0xA1,
    PeepHole3VCmdTypeSetPowerSaving =0xA3,
    PeepHole3VCmdTypeGetVersion =0xA4,
    PeepHole3VCmdTypeFacSsidPwd =0xA5,
    PeepHole3VCmdTypeOutRemind  =0xE0,
    PeepHole3VCmdTypeScreenDataPass=0xE1,
    PeepHole3VCmdTypeScreenTimeSet =0xE2,
    PeepHole3VCmdTypeFovSet        =0xE3,
    PeepHole3VCmdTypEnd
};

enum open_type_3V{
    open_typ_3v_none=0x00,
    open_typ_3v_finger=0x01,
    open_typ_3v_pwd   = 0x02,
    open_typ_3v_card  = 0x03,
    open_typ_3v_face  = 0x04,
    open_typ_3v_vein = 0x05,
    open_typ_3v_key         = 0x06,
    open_typ_3v_iris        = 0x07,
    open_typ_3v_elec_key    = 0x09,
    open_typ_3v_end
};

typedef struct SerialCmd3V{
    unsigned char head;
    unsigned char len;
    unsigned char comm;
    unsigned char data[255];
    uint8_t crc;
}SerialCmd3V;


typedef enum{
    LOCK_3V_WAKE_UP_NORMAL=0x00,
    LOCK_3V_WAKE_UP_TAMPER,
    LOCK_3V_WAKE_UP_INTRUDER,
    LOCK_3V_WAKE_UP_TEST_BAN,
    LOCK_3V_WAKE_UP_DOOR_BELL,
    LOCK_3V_WAKE_UP_END
}LOCK_3V_WAKEUP_CODE;



#endif
