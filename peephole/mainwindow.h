#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QMainWindow>


#include "dataproc.h"
#include "SerialProtocolBase.h"
#include "SerialProtocolOrbFac.h"

#pragma execution_character_set("utf-8")

//#define ORBBEC_CLIENT_VER

typedef enum FUNC_TAB_TYPE
{
	FUNC_TAB_TYPE_NET = 0,
	FUNC_TAB_TYPE_CLOUD,
	FUNC_TAB_TYPE_FUNC,
	FUNC_TAB_TYPE_ALARM,
	FUNC_TAB_TYPE_EVENT,
	FUNC_TAB_TYPE_FAC1,//hide,in client ver
	FUNC_TAB_TYPE_FAC2,//hide,in client ver
	FUNC_TAB_TYPE_END
}FUNC_TAB_TYPE;

namespace Ui {
	class MainWindow;
	class eventwidget;
	class userwidget;
	class netwidget;
	class cloudwidget;
	class miscwidget;
	class factory1widget;
	class factory2widget;
}

class QTaskPane;
class QComboBox;
class MainWindow : public QMainWindow
{
	Q_OBJECT
		enum BkColorTyp {
		BkColorTypNo = 0,
		BkColorTypRed,
		BkColorTypGreen,
		BkColorTypBlue
	};
	enum ProductNumTyp
	{
		//1:k1t，2:k1a,3:c1d,4:c1a
		ProductNumTypNone = 0,
		ProductNumTypK1t = 1,
		ProductNumTypK1a = 2,
		ProductNumTypC1d = 3,
		ProductNumTypC1a = 4,
		ProductNumTypEnd
	};
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
	void ClearNoteListRes(bool bDel = false);

	void on_pushButton_orbbec_sn_get_clicked();
	void ReadConfig();

public slots:
	void ShowNoteInfo(QString noteStr, int bkColor, bool bShow);
	void ShowInfoDialog(QString info);
	void ShowDevInfo(int infoTyp, QString info);

private slots:
	void on_pushButton_wifi_ssid_pwd_set_clicked();
	void on_pushButton_wifi_mac_get_clicked();
	void on_pushButton_wifi_sn_get_clicked();
	void on_pushButton_wifi_ip_info_get_clicked();
	void on_pushButton_wifi_state_get_clicked();

	void on_checkBox_pir_stateChanged(int arg1);
	void on_pushButton_key_set_clicked();
	void on_groupBox_pic_dir_clicked();

	void on_groupBox_event_clicked();

	void on_pushButton_peephole_coms_clicked();
	void on_pushButton_peephole_open_clicked();

	void on_pushButton_peephole_sleep_clicked();
	//orbbec factory
	void on_pushButton_version_system_get_clicked();
	void on_pushButton_version_wifi_get_clicked();
	void on_pushButton_peephole_reset_clicked();
	void on_pushButton_pir_state_get_clicked();
	void on_pushButton_pir_state_set_clicked();
	void on_pushButton_pir_thresh_get_clicked();
	void on_pushButton_pir_thresh_set_clicked();
	void on_pushButton_pir_time_get_clicked();
	void on_pushButton_pir_time_set_clicked();

	void on_pushButton_wifi_state_get_2_clicked();
	void on_pushButton_wifi_state_set_clicked();
	void on_pushButton_wifi_ap_get_clicked();
	void on_pushButton_wifi_ap_set_clicked();

	void on_pushButton_media_audio_record_clicked();
	void on_pushButton_audio_rec_play_clicked();
	void on_pushButton_media_start_clicked();
	void on_pushButton_media_url_get_clicked();

	void on_pushButton_ircut_state_get_clicked();
	void on_pushButton_ircut_state_set_clicked();
	void on_pushButton_light_sensitive_get_clicked();
	void on_pushButton_light_sensitive_open_clicked();

	void on_checkBox_factory_mode_stateChanged(int arg1);
	void on_pushButton_sdcard_get_clicked();
	void on_checkBox_led_stateChanged(int arg1);
	void on_pushButton_sensor_sta_get_clicked();
	void on_pushButton_media_play_clicked();
	void on_pushButton_wire_ip_set_clicked();
	void on_pushButton_media_snap_clicked();
	void on_pushButton_media_record_clicked();
	void on_pushButton_media_cur_frm_get_clicked();
	void on_checkBox_msg_encrypt_stateChanged(int arg1);
	void on_groupBox_protocol_clicked();
	void on_pushButton_orb_sn_get_clicked();
	void on_pushButton_orb_sn_set_clicked();
	void on_pushButton_data_pass_clicked();
	void on_checkBox_ota_stateChanged(int arg1);
	void on_pushButton_version_get_clicked();
	void on_pushButton_net_time_get_clicked();
	void on_groupBox_wifi_action_typ_clicked();
	void on_groupBox_event_param_clicked();
	void on_pushButton_peephole_reset_2_clicked();
	void on_pushButton_dev_state_get_clicked();
	void on_checkBox_wifi_reserved_stateChanged(int arg1);
	void on_checkBox_scan_conf_stateChanged(int arg1);
	void on_pushButton_protocol_extend_send_clicked();
	void on_pushButton_alarm_send_clicked();

	void on_pushButton_event_send_clicked();

	void on_pushButton_person_detect_state_get_clicked();

	void on_pushButton_mask_detect_state_get_clicked();

	void on_pushButton_look_up_remind_clicked();

	void on_groupBox_wifi_live_face_clicked();

	void on_pushButton_device_sn_get_clicked();

	void on_pushButton_fov_set_clicked();

	void on_pushButton_battery_level_send_clicked();

	void on_pushButton_key_get_clicked();

	void on_groupBox_event_type_clicked();

	void on_pushButton_user_data_send_clicked();

	void on_comboBox_event_type_currentIndexChanged(int arg1);

	void on_pushButton_send_cmd_clicked();
protected:
	void InitAlarmParam(void);
	void InitEventParam(void);
	void InitFreezeParam();
	void InitRemindParam(void);
	void InitBatteryParam(void);
	void InitUserDataParam(void);

	void ClearEventCheck(uint8_t id);
	void EnableProtocolFunc(bool bEnable);
	void EnableRecordParam(bool bEnable);
	void EnableSnapParam(bool bEnable);
	void closeEvent(QCloseEvent* event);
	void ShowPic(unsigned char* data, int datalen, int width, int height);
	void ReadConfig1(void);
	void CheckDirect(char* directPath);
	//function
	bool atoi2(const char* str, int* pOut);
	int  Char2Hex(uint8_t* buf, int buflen, char* out, int& outlen);
	int  ParseCommStr(QString commStr, uint8_t* pComm, int bufLen);

	void InitComboBoxItems(QComboBox* combo, const QVector<QPair<QString, qint8>>& items_custom);
private:
	Ui::MainWindow* ui;
	Ui::eventwidget* ui_event;
	Ui::userwidget* ui_user;
	Ui::netwidget* ui_net;
	Ui::cloudwidget* ui_cloud;
	Ui::miscwidget* ui_misc;
	Ui::factory1widget* ui_fac1;
	Ui::factory2widget* ui_fac2;


	QTaskPane* m_paramTaskPane;
	CRecFile m_logWrite;
	char     m_logFilePath[128]; //log path
	char     m_savePicPath[128]; //save pic path
	//record
	bool m_bRec;
	//
	int m_baudRate[17];
	//list table
	QStandardItemModel* m_pNoteListModel;
	//relay param
	QButtonGroup m_picCtlGp;
	QButtonGroup m_wifiActionTypGp;
	QButtonGroup m_wifiActionFaceTypGp;
	//event param
	QButtonGroup m_eventParamGp;
	//event typ param
	QButtonGroup m_alarmEventGp;

	//serial  protocol type
	int m_protocolTyp;
	QButtonGroup m_protocolTypGp;
	//common event type and freeze event type
	QButtonGroup m_eventTypGp;
	int m_eventTyp = 0;
	//
	CSerialProtocolBase* m_pSerialProtocol;
	CSerialProcotolOrbFac m_orbFac;

	bool m_bDevOpen;
	bool m_bPicSave;
	CDataProc* m_pDataProc;
	//media control
	bool m_bAudioRec;
	bool m_bAudioPly;
	//stream
	bool m_bWifiConnect;
	bool m_bStreamStart;
	bool m_bLightSensitiveOpen;
	char m_streamUrl[256]{ 0 };

	//client play
	bool m_bPlay;
	int m_snapPicNum;

	//main and sub stream param
	int m_mainWidth;
	int m_mainHeight;
	int m_subWidth;
	int m_subHeight;
	int m_productNum;
	char m_ssid[128]{ 0 };
	char m_pwd[128]{ 0 };
};

#endif // MAINWINDOW_H
