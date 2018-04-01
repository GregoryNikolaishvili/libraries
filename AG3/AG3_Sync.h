#ifndef _AG3_SYNC_H
#define _AG3_SYNC_H

#include <Arduino.h>
#include <RS485_non_blocking.h>

class AG3SyncState
{
private:
	boolean isSyncOK;
	boolean isStatesDataOK;
	boolean isSettingsDataOK;

	unsigned long nextSyncRequestTicks;
	unsigned long nextSettingsRequestTicks;
	unsigned long nextStateRequestTicks;
protected:
	RS485* link;

	virtual void changed() {};
public:
	void begin(RS485* linkToAG);

	void resync() { isSyncOK = false; isSettingsDataOK = false; isStatesDataOK = false; changed(); };

	void setSyncOK(boolean value) { isSyncOK = value; if (!value) { isSettingsDataOK = false; isStatesDataOK = false; } changed(); }
	void setIsSettingsDataOK(boolean value) { isSettingsDataOK = value; changed(); }
	void setIsStatesDataOK(boolean value) { isStatesDataOK = value; changed(); }

	inline boolean getSyncOK() { return isSyncOK; }
	inline boolean getIsSettingsDataOK() { return isSettingsDataOK; }
	inline boolean getIsStatesDataOK() { return isStatesDataOK; }

	boolean getIsEverythingOK() { return isSyncOK || isSettingsDataOK || isStatesDataOK; }

	boolean checkSync(unsigned long ticks);
	boolean checkSettings(unsigned long ticks);
	boolean checkStates(unsigned long ticks);
};

#endif
