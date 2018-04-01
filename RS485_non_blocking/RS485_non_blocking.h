#ifndef _RS485_H
#define _RS485_H

/*
 RS485 protocol library - non-blocking.

 Devised and written by Nick Gammon.
 Date: 4 December 2012
 Version: 1.0

 Licence: Released for public use.

 */

#include "Arduino.h"
#include <AG3.h>

class RS485
{
private:
	typedef size_t(*WriteCallback)  (const byte what);    // send a byte to serial port
	typedef int(*AvailableCallback)  ();    // return number of bytes available
	typedef int(*ReadCallback)  (void);    // read a byte from serial port
	typedef void(*ErrorCallback)  (const char*);

	enum {
		STX = '\2',   // start of text
		ETX = '\3'    // end of text
	};

	// callback functions to do reading/writing
	ReadCallback fReadCallback;
	AvailableCallback fAvailableCallback;
	WriteCallback fWriteCallback;
	ErrorCallback fErrorCallback;

	byte * fData; // where we save incoming stuff

	// how much data is in the buffer
	int fBufferSize;

	// this is true once we have valid data in buf
	bool FAvailable;

	bool FHaveSTX; // an STX (start of text) signals a packet start

	// count of errors
	unsigned long FErrorCount;

	// variables below are set when we get an STX
	bool FHaveETX;
	byte FInputPos;
	byte FCurrentByte;
	bool FFirstNibble;
	unsigned long FStartTime;

	byte FCrc8;

	unsigned int syncCounterSend;
	unsigned int syncCounterRecv;

	// helper private functions
	byte crc8(const byte *addr, byte len);
	void sendComplemented(const byte what);

public:

	// constructor
	RS485(ReadCallback readCallback, AvailableCallback availableCallback, WriteCallback writeCallback, const byte bufferSize, ErrorCallback errorCallback = NULL)
	{
		fReadCallback = readCallback;
		fAvailableCallback = availableCallback;
		fWriteCallback = writeCallback;
		fBufferSize = bufferSize;
		fErrorCallback = errorCallback;
		fData = NULL;
	}

	// destructor - frees memory used
	~RS485() { stop(); }

	// allocate memory for buf_
	void begin();

	// free memory in buf_
	void stop();

	// handle incoming data, return true if packet ready
	bool update();

	// reset to no incoming data (eg. after a timeout)
	void reset();

	// send data
	//void sendMsg(const byte * data, const byte length);

	// returns true if packet available
	bool available() const { return FAvailable; };

	// once available, returns the address of the current message
	inline byte * getData()   const { return fData; }
	inline byte getLength() const { return FInputPos; }

	// return how many errors we have had
	unsigned long getErrorCount() const { return FErrorCount; }

	// return when last packet started
	unsigned long getPacketStartTime() const { return FStartTime; }

	// return true if a packet has started to be received
	bool isPacketStarted() const { return FHaveSTX; }

	void sendCommand(byte command) { beginTransmission();  write(command); endTransmission(); };
	void sendCommand(byte command, byte value) { beginTransmission(); write(command); write(value); endTransmission(); };
	void sendCommand(byte command, byte id, int value) { beginTransmission(); write(command); write(id); write(value >> 8); write(value & 0xFF); endTransmission(); };
	void sendTerminalCommand(byte socket, const char* data) { beginTransmission();  write(LINK_COMMAND_TERMINAL);  write(socket); write(data); endTransmission(); };
	void sendText(byte socket, Stream& stream) { beginTransmission();  write(LINK_COMMAND_TEXT); write(socket); write(stream); endTransmission(); };

	void beginTransmission();
	void write(byte data);
	void write(Stream& stream);
	void write(const char* data);
	void endTransmission();

	void sendSyncMsg(unsigned int settingsVersion, unsigned int statesVersion);

	//inline unsigned int getSyncCounterSend() { return syncCounterSend; }
	inline unsigned int getSyncCounterRecv() { return syncCounterRecv; }
	inline void setSyncCounterRecv(unsigned int value) { syncCounterRecv = value; }
};

#endif