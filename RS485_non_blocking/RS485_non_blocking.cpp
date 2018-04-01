/*
 RS485 protocol library - non-blocking.

 Devised and written by Nick Gammon.
 Date: 4 December 2012
 Version: 1.0

 Licence: Released for public use.


 Can send from 1 to 255 bytes from one node to another with:

 * Packet start indicator (STX)
 * Each data byte is doubled and inverted to check validity
 * Packet end indicator (ETX)
 * Packet CRC (checksum)

 */


#include <RS485_non_blocking.h>
#include <Time.h>

// allocate the requested buffer size
void RS485::begin()
{
	fData = (byte *)malloc(fBufferSize);
	reset();
	FErrorCount = 0;
	syncCounterSend = 0;
	syncCounterRecv = 0;
}

// get rid of the buffer
void RS485::stop()
{
	reset();
	free(fData);
	fData = NULL;
}

// called after an error to return to "not in a packet"
void RS485::reset()
{
	FHaveSTX = false;
	FAvailable = false;
	FInputPos = 0;
	FStartTime = 0;
	FCrc8 = 0;
}


// calculate 8-bit CRC
byte RS485::crc8(const byte *addr, byte len)
{
	byte crc = 0;
	while (len--)
	{
		byte inbyte = *addr++;
		for (byte i = 8; i; i--)
		{
			byte mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix)
				crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

// send a byte complemented, repeated only values sent would be (in hex):  0F, 1E, 2D, 3C, 4B, 5A, 69, 78, 87, 96, A5, B4, C3, D2, E1, F0
void RS485::sendComplemented(const byte what)
{
	byte c;

	// first nibble
	c = what >> 4;
	fWriteCallback((c << 4) | (c ^ 0x0F));

	// second nibble
	c = what & 0x0F;
	fWriteCallback((c << 4) | (c ^ 0x0F));

}

void RS485::write(Stream& stream)
{
	while (stream.available())
	{
		write(stream.read());
	}
	
	stream.flush();
}

void RS485::write(const char * data)
{
	do
	{
		write(*data);
	} while (*data++);
}


void RS485::beginTransmission()
{
	FCrc8 = 0;
	fWriteCallback(STX);  // STX
}

void RS485::write(byte inbyte)
{
	sendComplemented(inbyte);

	for (byte i = 8; i; i--)
	{
		byte mix = (FCrc8 ^ inbyte) & 0x01;
		FCrc8 >>= 1;
		if (mix)
			FCrc8 ^= 0x8C;
		inbyte >>= 1;
	}
}

void RS485::endTransmission()
{
	fWriteCallback(ETX);  // ETX
	sendComplemented(FCrc8);
	syncCounterSend++;
}

void RS485::sendSyncMsg(unsigned int settingsVersion, unsigned int statesVersion)
{
	beginTransmission();
	write(byte(LINK_COMMAND_SYNC));

	write(highByte(syncCounterSend));
	write(lowByte(syncCounterSend));

	write(highByte(settingsVersion));
	write(lowByte(settingsVersion));

	write(highByte(statesVersion));
	write(lowByte(statesVersion));

	time_t T = now();// unsigned long

	unsigned int W = T >> 16;
	write(highByte(W));
	write(lowByte(W));

	W = T & 0xFFFF;
	write(highByte(W));
	write(lowByte(W));

	endTransmission();
}

// called periodically from main loop to process data and 
// assemble the finished packet in 'fData'

// returns true if packet received.

// You could implement a timeout by seeing if isPacketStarted() returns true, and if too much time has passed since getPacketStartTime() time.
bool RS485::update()
{
	// no data? can't go ahead (eg. begin() not called)
	if (fData == NULL)
		return false;

	// no callbacks? Can't read
	if (fAvailableCallback == NULL || fReadCallback == NULL)
		return false;

	while (fAvailableCallback() > 0)
	{
		byte inByte = fReadCallback();
		switch (inByte)
		{

		case STX:   // start of text
			FHaveSTX = true;
			FHaveETX = false;
			FInputPos = 0;
			FFirstNibble = true;
			FStartTime = millis();
			break;

		case ETX:   // end of text (now expect the CRC check)
			FHaveETX = true;
			break;

		default:
			// wait until packet officially starts
			if (!FHaveSTX)
				break;

			// check byte is in valid form (4 bits followed by 4 bits complemented)
			if ((inByte >> 4) != ((inByte & 0x0F) ^ 0x0F))
			{
				reset();
				FErrorCount++;

				if (fErrorCallback)
					ErrorCallback(F("a <> A"));
				break;  // bad character
			}

			// convert back 
			inByte >>= 4;

			// high-order nibble?
			if (FFirstNibble)
			{
				FCurrentByte = inByte;
				FFirstNibble = false;
				break;
			}

			// low-order nibble
			FCurrentByte <<= 4;
			FCurrentByte |= inByte;
			FFirstNibble = true;

			// if we have the ETX this must be the CRC
			if (FHaveETX)
			{
				if (crc8(fData, FInputPos) != FCurrentByte)
				{
					reset();
					FErrorCount++;
					if (fErrorCallback)
						fErrorCallback("CRC");
					break;  // bad crc  
				}

				FAvailable = true;
				syncCounterRecv++;
				return true;  // show data ready
			}

			// keep adding if not full
			if (FInputPos < fBufferSize)
				fData[FInputPos++] = FCurrentByte;
			else
			{
				reset(); // overflow, start again
				FErrorCount++;
				if (fErrorCallback)
					fErrorCallback("Overflow");
			}

			break;
		}
	}

	return false;  // not ready yet
}

