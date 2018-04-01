#ifndef _AG3_H
#define _AG3_H

#include <Arduino.h>
#include <TimeLib.h>

void printDateTime(Print &server, time_t value);

#define AlarmHM_Minutes(_hr_, _min_) (_hr_ * 60 + _min_)

//const byte OP_SOCKET_NUM = 100;
const byte MAX_SOCKET_NUM = 4; // MAX_SOCK_NUM

const byte LINK_COMMAND_SYNC = '*';  // 2 params (2 x byte = int)
const byte LINK_COMMAND_HEART_BEAT = 'H';  // no params

// Two way commands
const byte LINK_COMMAND_ALL_SETTINGS_CHANGE_BEGIN = 'T';  // no params
const byte LINK_COMMAND_SET_SETTING = '$';  // 3 params for I2C, 3 x N params for Serial
const byte LINK_COMMAND_ALL_SETTINGS_CHANGE_END = 't';  // 3 params (0 + version)

const byte LINK_COMMAND_ALL_STATES_CHANGE_BEGIN = 'S'; // no params
const byte LINK_COMMAND_SET_STATE = '#';  // 3 params for I2C, 3 x N params for Serial
const byte LINK_COMMAND_ALL_STATES_CHANGE_END = 's';  // 3 params (0 + version)


// from OP and NET to AG3
const byte LINK_COMMAND_TERMINAL = 'X';

// from AG3 to OP and NET

const byte LINK_COMMAND_BUZZER = 'B'; // 1 param, buzz type
const byte LINK_COMMAND_SEND_KEYS = 'K';
const byte LINK_COMMAND_TEXT = '.'; // string param

// Error Code Bits (in STATE_ERROR)
const int ERR_SYSTEM = 0x0001;
const int ERR_TIME = 0x0002;
const int ERR_INTERNET = 0x0004;
//const int ERR_RESERV = 0x0008;

// Error Code Bits (in STATE_SENSORS)
const int ERR_TEMP_SENSOR_AQUA1 = 0x0001;
const int ERR_TEMP_SENSOR_AQUA2 = 0x0002;
const int ERR_TEMP_SENSOR_AQUA3 = 0x0004;
const int ERR_TEMP_SENSOR_SUMP = 0x0008;
const int ERR_TEMP_SENSOR_HOSP = 0x0010;
const int ERR_TEMP_SENSOR_ROOM = 0x0020;
const int ERR_TEMP_SENSOR_RESERVE1 = 0x0040;
const int ERR_TEMP_SENSOR_RESERVE2 = 0x0080;

const int ERR_TEMP_SENSOR_BOARD = 0x0100;
const int ERR_TEMP_SENSOR_DHT = 0x0200;
const int ERR_HUM_SENSOR_DHT = 0x0400;

const int ERR_ULTRASONIC_AQUA = 0x1000;
const int ERR_ULTRASONIC_SUMP = 0x2000;
const int ERR_ULTRASONIC_HOSP = 0x4000;

#define ERR_ALL_DS18B20_SENSORS (ERR_TEMP_SENSOR_AQUA1 | ERR_TEMP_SENSOR_AQUA2 | ERR_TEMP_SENSOR_AQUA3 | ERR_TEMP_SENSOR_SUMP | ERR_TEMP_SENSOR_HOSP | ERR_TEMP_SENSOR_ROOM | ERR_TEMP_SENSOR_RESERVE1 | ERR_TEMP_SENSOR_RESERVE2)
#define ERR_ALL_TEMP_SENSORS (ERR_ALL_DS18B20_SENSORS | ERR_TEMP_SENSOR_BOARD | ERR_TEMP_SENSOR_DHT)
// End of Error Code Bits (in STATE_SENSORS)

enum AquaGodStateId {
	STATE_ERROR = 0,
	STATE_SENSORS,

	STATE_AQUA_T, // mean value
	STATE_AQUA_T1,
	STATE_AQUA_T2,
	STATE_AQUA_T3,
	STATE_AQUA_PH,
	STATE_AQUA_WL_PERCENT,
	STATE_AQUA_WL_MM,
	STATE_AQUA_WL_LOW,
	STATE_AQUA_WL_HIGH,
	DEVICE_AQUA_HEATER,
	DEVICE_FILTER_1,
	DEVICE_FILTER_2,
	DEVICE_O2,
	DEVICE_CO2,
	DEVICE_AQUA_R_PUMP,

	STATE_SUMP_T,
	STATE_SUMP_PH,
	STATE_SUMP_WL_PERCENT,
	STATE_SUMP_WL_MM,
	STATE_SUMP_WL_LOW,
	STATE_SUMP_WL_HIGH,
	DEVICE_SUMP_HEATER,
	DEVICE_SUMP_R_PUMP,

	STATE_HOSP_T,
	STATE_HOSP_WL_PERCENT,
	STATE_HOSP_WL_MM,
	STATE_HOSP_WL_LOW,
	STATE_HOSP_WL_HIGH,
	DEVICE_HOSP_HEATER,

	STATE_ROOM_T,
	STATE_ROOM_H,

	STATE_BOARD_T,
	DEVICE_BOARD_FAN,
	STATE_BOARD_FAN_RPM,

	STATE_FLOOR_WET1,
	STATE_FLOOR_WET2,

	/////////////////////

	DEVICE_LIGHT_1,
	DEVICE_LIGHT_2,
	DEVICE_LIGHT_3,
	DEVICE_LIGHT_4,
	DEVICE_LIGHT_5, // White front lamps (3)
	DEVICE_LIGHT_6, // dim light white
	DEVICE_LIGHT_MOON, // moon light blue
	DEVICE_LIGHT_HOSPITAL,

	DEVICE_EXHAUST_FAN,

	DEVICE_WATER_DRAIN_PUMP,
	DEVICE_WATER_FILL_PUMP,

	DEVICE_DOSING_PUMP_MACRO,
	DEVICE_DOSING_PUMP_MICRO,

	DEVICE_UV_LIGHT,

	DEVICE_SOLENOID,

	AQUAGOD_STATES_SIZE // should be last
};

enum AquaGodSettingId {
	SETTING_LOG_LEVEL,
	SETTING_AQUA_T,
	SETTING_AQUA_PH,
	SETTING_HOSP_T,
	SETTING_BOARD_T,

	SETTING_O2_ON_TIME,
	SETTING_O2_OFF_TIME,
	SETTING_CO2_ON_TIME,
	SETTING_CO2_OFF_TIME,

	SETTING_LIGHT_ON_TIME,
	SETTING_LIGHT_OFF_TIME,
	SETTING_LIGHT_TRANSITION_DURATION_MIN,
	SETTING_LIGHT_MOONLIGHT_DURATION_MIN,

	SETTING_FEED_START_TIME_A,
	SETTING_FEED_START_TIME_B,
	SETTING_FEED_START_TIME_C,
	SETTING_FEED_DOW,

	SETTING_WATER_CHANGE_START_TIME_A,
	SETTING_WATER_CHANGE_START_TIME_B,
	SETTING_WATER_DRAIN_DURATION_SEC,
	SETTING_WATER_FILL_DURATION_SEC,
	SETTING_WATER_EXRA_DRAIN_DURATION_SEC,
	SETTING_WATER_DRAIN_LEVEL_PERCENT,
	SETTING_WATER_CHANGE_DOW,

	SETTING_DOSING_MACRO_START_TIME,
	SETTING_DOSING_MACRO_DURATION_SEC,
	SETTING_DOSING_MACRO_DOW,

	SETTING_DOSING_MICRO_START_TIME,
	SETTING_DOSING_MICRO_DURATION_SEC,
	SETTING_DOSING_MICRO_DOW,

	AQUAGOD_SETTINGS_SIZE // should be last
};


class AG3StateBase
{
private:
	int values[AQUAGOD_STATES_SIZE];
protected:
	unsigned int version;
public:
	void begin();

	inline int get(byte id) { return values[id]; }
	boolean set(byte id, int value);

	void setErrorBit(unsigned int value);
	void unsetErrorBit(unsigned int value);
	inline int getErrorCode() { return get(STATE_ERROR); }

	void setSensorErrorBit(unsigned int value);
	void unsetSensorErrorBit(unsigned int value);
	inline int getSensorErrorCode() { return get(STATE_SENSORS); }

	boolean getHaveErrors();

	inline unsigned int getVersion() { return version; }
	inline void setVersion(unsigned int value) { version = value; }
};

class AG3SettingBase
{
private:
	int values[AQUAGOD_SETTINGS_SIZE];
protected:
	unsigned int version;
public:
	void begin();

	inline int get(byte id) { return values[id]; }
	void set(byte id, int value);

	inline unsigned int getVersion() { return version; }
	inline void setVersion(unsigned int value) { version = value; }
};


class AG3State : public AG3StateBase
{
private:
	const __FlashStringHelper* names[AQUAGOD_STATES_SIZE];
	boolean isDevices[AQUAGOD_STATES_SIZE];
public:
	void begin();

	inline const __FlashStringHelper* getName(byte id) { return names[id]; }
	inline boolean getIsDevice(byte id) { return isDevices[id]; }

	void addState(byte id, const __FlashStringHelper* name);
	void addDevice(byte id, const __FlashStringHelper* name);
};


class AG3Setting : public AG3SettingBase
{
private:
	const __FlashStringHelper* names[AQUAGOD_SETTINGS_SIZE];
public:
	void begin();

	inline const __FlashStringHelper* getName(byte id) { return names[id]; }

	void add(byte id, const __FlashStringHelper* name, int defValue);
};

#endif
