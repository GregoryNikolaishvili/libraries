#include <AG3.h>
#include <AG3_Sync.h>


void AG3SyncState::begin(RS485* linkToAG)
{
	link = linkToAG;

	nextSyncRequestTicks = 1;
	nextSettingsRequestTicks = 2;
	nextStateRequestTicks = 3;
}


boolean AG3SyncState::checkSync(unsigned long ticks)
{
	if (!isSyncOK && ((ticks > nextSyncRequestTicks) || (ticks + 20 < nextSyncRequestTicks))) // overflow protection
	{
		nextSyncRequestTicks = ticks + 10; // 5 sec
		link->sendTerminalCommand(MAX_SOCKET_NUM, "sync");
	}
	return !isSyncOK;
}

boolean AG3SyncState::checkSettings(unsigned long ticks)
{
	if (!isSettingsDataOK && ((ticks > nextSettingsRequestTicks) || (ticks + 20 < nextSettingsRequestTicks))) // overflow protection
	{
		nextSettingsRequestTicks = ticks + 10; // 5 sec
		link->sendTerminalCommand(MAX_SOCKET_NUM, "refresh settings");
	}
	return !isSettingsDataOK;
}

boolean AG3SyncState::checkStates(unsigned long ticks)
{
	if (!isStatesDataOK && ((ticks > nextStateRequestTicks) || (ticks + 20 < nextStateRequestTicks))) // overflow protection
	{
		nextStateRequestTicks = ticks + 10; // 5 sec
		link->sendTerminalCommand(MAX_SOCKET_NUM, "refresh states");
	}
	return !isStatesDataOK;
}

