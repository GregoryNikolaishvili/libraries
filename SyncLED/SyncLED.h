#ifndef _SYNCLED_H_
#define _SYNCLED_H_
#include <Arduino.h>

class SyncLED {

public:
	void begin(byte pin, byte initialState = LOW, boolean usePWM = false, unsigned long pwmMS = 500);

	// Set the pattern (if desired) in binary ex: 0B1000010001
	// Note that the max length is 32 bits...
	// Rate represents the delay between bits
	void blinkPattern(unsigned long pattern = 0B10101010101010101010101010101010, unsigned long delay = 1000, byte pattern_length = 32);
	inline void setRate(unsigned long delay) { m_delay = delay; };
	void setPattern(unsigned long pattern, byte pattern_length = 32);
	inline void blinkPattern(uint8_t blinks, unsigned long delay = 1000);

	// The following functions stop the current pattern and set the LED state explicitly
	void On(unsigned long duration = 0UL);
	void Off();

	// Resume pattern once stopped
	void resumePattern(boolean reset = false);

	// Return true if the light is currently on
	inline boolean isOn() { return(m_state == HIGH); }

	// Call one of these to update LED State
	void update();
	void update(unsigned long time_ms);

private: // methods
	void advanceState(unsigned long time_ms);

private: // variables
	byte m_pin;
	unsigned long m_pattern;
	unsigned long m_delay;
	unsigned long m_lastChange;
	unsigned long m_pwmFadeMS;
	unsigned long m_duration;

	byte m_patternLength;
	byte m_inPattern;
	byte m_patternPosition;
	byte m_state;
	byte m_lastState;
	byte m_usePWM;

};

#endif