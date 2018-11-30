#pragma once
namespace function
{
	void remoteMainThreadCall(byte * shell_code, size_t shell_code_size, PVOID param = 0, size_t paramSize = 0);
	int decrypt(int address);
	void encrypt(INT32 Address, INT32 Value);
	int getGameStatus();
	POS getCurrentRoomPos();
	POS getBossRoomPos();
	bool isBossRoom();
	bool isOpenDoor();
};

