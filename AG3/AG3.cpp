#include <AG3.h>

void AG3StateBase::begin()
{
	version = random(0xFFFF);
	for (byte id = 0; id < AQUAGOD_STATES_SIZE; id++)
		values[id] = -1;  // not initialized
}

boolean AG3StateBase::set(byte id, int value)
{
	if ((id < AQUAGOD_STATES_SIZE) && (values[id] != value))
	{
		values[id] = value;
		version++;
		return true;
	}

	return false;
}


boolean AG3StateBase::getHaveErrors()
{
	return getErrorCode() || getSensorErrorCode();
}

void AG3StateBase::setErrorBit(unsigned int value)
{
	set(STATE_ERROR, get(STATE_ERROR) | value);
}

void AG3StateBase::unsetErrorBit(unsigned int value)
{
	set(STATE_ERROR, get(STATE_ERROR) & ~value);
}

void AG3StateBase::setSensorErrorBit(unsigned int value)
{
	set(STATE_SENSORS, get(STATE_SENSORS) | value);
}

void AG3StateBase::unsetSensorErrorBit(unsigned int value)
{
	set(STATE_SENSORS, get(STATE_SENSORS) & ~value);
}


void AG3State::begin()
{
	AG3StateBase::begin();

	addState(STATE_ERROR, F("System Err"));
	addState(STATE_SENSORS, F("Sensor Err"));

	addState(STATE_AQUA_T, F("Aqua T"));
	addState(STATE_AQUA_T1, F("Aqua T1"));
	addState(STATE_AQUA_T2, F("Aqua T2"));
	addState(STATE_AQUA_T3, F("Aqua T3"));
	addState(STATE_AQUA_PH, F("Aqua PH"));
	addState(STATE_AQUA_WL_PERCENT, F("Aqua WL"));
	addState(STATE_AQUA_WL_MM, F("Aqua WL(mm)"));
	addState(STATE_AQUA_WL_LOW, F("Aqua WL Low"));
	addState(STATE_AQUA_WL_HIGH, F("Aqua WL High"));

	addState(STATE_SUMP_T, F("Sump T"));
	addState(STATE_SUMP_PH, F("Sump PH"));
	addState(STATE_SUMP_WL_PERCENT, F("Sump WL"));
	addState(STATE_SUMP_WL_MM, F("Sump WL(mm)"));
	addState(STATE_SUMP_WL_LOW, F("Sump WL Low"));
	addState(STATE_SUMP_WL_HIGH, F("Sump WL High"));

	addState(STATE_HOSP_T, F("Hosp T"));
	addState(STATE_HOSP_WL_PERCENT, F("Hosp WL"));
	addState(STATE_HOSP_WL_MM, F("Hosp WL(mm)"));
	addState(STATE_HOSP_WL_LOW, F("Hosp WL Low"));
	addState(STATE_HOSP_WL_HIGH, F("Hosp WL High"));

	addState(STATE_ROOM_T, F("Room T"));
	addState(STATE_ROOM_H, F("Room Hum."));

	addState(STATE_BOARD_T, F("Board T"));
	addDevice(DEVICE_BOARD_FAN, F("Fan %"));
	addState(STATE_BOARD_FAN_RPM, F("Fan RPM"));

	addState(STATE_FLOOR_WET1, F("Floor Wet 1"));
	addState(STATE_FLOOR_WET2, F("Floor Wet 2"));

	addDevice(DEVICE_LIGHT_1, F("Light 1"));
	addDevice(DEVICE_LIGHT_2, F("Light 2"));
	addDevice(DEVICE_LIGHT_3, F("Light 3"));
	addDevice(DEVICE_LIGHT_4, F("Light 4"));
	addDevice(DEVICE_LIGHT_5, F("Light 5"));
	addDevice(DEVICE_LIGHT_6, F("Light 6"));
	addDevice(DEVICE_LIGHT_MOON, F("Moon Light"));
	addDevice(DEVICE_LIGHT_HOSPITAL, F("Hosp Light"));

	addDevice(DEVICE_EXHAUST_FAN, F("Exhaust Fan"));

	addDevice(DEVICE_O2, F("O2"));
	addDevice(DEVICE_CO2, F("CO2"));
	addDevice(DEVICE_AQUA_R_PUMP, F("Aqua R.Pump"));
	addDevice(DEVICE_WATER_DRAIN_PUMP, F("Drain Pump"));
	addDevice(DEVICE_WATER_FILL_PUMP, F("Fill Pump"));

	addDevice(DEVICE_DOSING_PUMP_MACRO, F("Dosing Macro"));
	addDevice(DEVICE_DOSING_PUMP_MICRO, F("Dosing Micro"));

	addDevice(DEVICE_SUMP_R_PUMP, F("Sump R.Pump"));
	addDevice(DEVICE_UV_LIGHT, F("UV Light"));
	addDevice(DEVICE_FILTER_1, F("Filter 1"));
	addDevice(DEVICE_FILTER_2, F("Filter 2"));

	addDevice(DEVICE_AQUA_HEATER, F("Aqua Heater"));
	addDevice(DEVICE_SUMP_HEATER, F("Sump Heater"));
	addDevice(DEVICE_HOSP_HEATER, F("Hosp Heater"));

	addDevice(DEVICE_SOLENOID, F("Solenoid"));
}

void AG3State::addState(byte id, const __FlashStringHelper* name)
{
	isDevices[id] = false;
	names[id] = name;
}

void AG3State::addDevice(byte id, const __FlashStringHelper* name)
{
	isDevices[id] = true;
	names[id] = name;
}

//////////////////////////////////

void AG3SettingBase::begin()
{
	version = random(0xFFFF);
	for (byte id = 0; id < AQUAGOD_SETTINGS_SIZE; id++)
		values[id] = -1;  // not initialized
}

void AG3SettingBase::set(byte id, int value)
{
	if ((id < AQUAGOD_SETTINGS_SIZE) && (values[id] != value))
	{
		values[id] = value;
		version++;
	}
}

void AG3Setting::add(byte id, const __FlashStringHelper* name, int defValue)
{
	set(id, defValue);
	names[id] = name;
}

void AG3Setting::begin()
{
	AG3SettingBase::begin();

	add(SETTING_LOG_LEVEL, F("Log Level"), 4); // #define LOG_LEVEL_VERBOSE 4
	add(SETTING_AQUA_T, F("Aqua T"), 3000);
	add(SETTING_AQUA_PH, F("Aqua PH"), 700);
	add(SETTING_HOSP_T, F("Hosp T"), 0);
	add(SETTING_BOARD_T, F("Board T"), 3100);

	add(SETTING_O2_ON_TIME, F("O2 On Time"), AlarmHM_Minutes(22, 0));
	add(SETTING_O2_OFF_TIME, F("O2 Off Time"), AlarmHM_Minutes(11, 0));
	add(SETTING_CO2_ON_TIME, F("CO2 On Time"), AlarmHM_Minutes(10, 0));
	add(SETTING_CO2_OFF_TIME, F("CO2 Off Time"), AlarmHM_Minutes(21, 0));

	add(SETTING_LIGHT_ON_TIME, F("Light On Time"), AlarmHM_Minutes(11, 0));
	add(SETTING_LIGHT_OFF_TIME, F("Light Off Time"), AlarmHM_Minutes(22, 0));
	add(SETTING_LIGHT_TRANSITION_DURATION_MIN, F("Light Transition Ducation (min)"), 5);
	add(SETTING_LIGHT_MOONLIGHT_DURATION_MIN, F("MoonLight Ducation (min)"), 60);

	add(SETTING_FEED_START_TIME_A, F("Feed Start Time A"), AlarmHM_Minutes(10, 30));
	add(SETTING_FEED_START_TIME_B, F("Feed Start Time B"), AlarmHM_Minutes(15, 0));
	add(SETTING_FEED_START_TIME_C, F("Feed Start Time C"), AlarmHM_Minutes(20, 0));
	add(SETTING_FEED_DOW, F("Feed DOW"), B01111110); // all except sunday

	add(SETTING_WATER_CHANGE_START_TIME_A, F("Water Change Start Time A"), AlarmHM_Minutes(22, 15));
	add(SETTING_WATER_CHANGE_START_TIME_B, F("Water Change Start Time B"), AlarmHM_Minutes(0, 0));
	add(SETTING_WATER_DRAIN_DURATION_SEC, F("Water Drain Duration (sec)"), 60 * 5);
	add(SETTING_WATER_FILL_DURATION_SEC, F("Water Fill Duration (sec)"), 60 * 3);
	add(SETTING_WATER_EXRA_DRAIN_DURATION_SEC, F("Water Exra Drain Duration (sec)"), 60 * 1);
	add(SETTING_WATER_DRAIN_LEVEL_PERCENT, F("Water Drain Level (%)"), 80); // % level after water drain
	add(SETTING_WATER_CHANGE_DOW, F("Water Change DOW"), B11111111);

	add(SETTING_DOSING_MACRO_START_TIME, F("Dosing Macro Start Time"), AlarmHM_Minutes(10, 45));
	add(SETTING_DOSING_MACRO_DURATION_SEC, F("Dosing Macro Duration (sec)"), 15);
	add(SETTING_DOSING_MACRO_DOW, F("Dosing Macro DOW"), B11111111);

	add(SETTING_DOSING_MICRO_START_TIME, F("Dosing Micro Start Time"), AlarmHM_Minutes(10, 55));
	add(SETTING_DOSING_MICRO_DURATION_SEC, F("Dosing Micro Duration (sec)"), 15);
	add(SETTING_DOSING_MICRO_DOW, F("Dosing Micro DOW"), B11111111);
}


///////////////////////////

void printDateTime(Print &server, time_t value)
{
	server.print(day(value));
	server.print(' ');
	server.print(monthShortStr(month(value)));
	server.print(' ');
	server.print(year(value));
	server.print(' ');

	if (hour(value) < 10)
		server.print('0');
	server.print(hour(value));
	server.print(':');
	if (minute(value) < 10)
		server.print('0');
	server.print(minute(value));
	server.print(':');
	if (second(value) < 10)
		server.print('0');
	server.print(second(value));
	server.print(' ');
}


