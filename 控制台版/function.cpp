#include "pch.h"
#include "function.h"

//解密
int function::decrypt(int address)
{
	int eax, esi, edx, i;
	eax = read<int>(address);
	esi = read<int>(__解密基址);
	edx = eax >> 16;
	edx = read<int>(esi + edx * 4 + 36);
	eax = eax & 65535;
	eax = read<int>(edx + eax * 4 + 8468);
	edx = eax & 65535;
	esi = edx << 16;
	esi = esi | edx;
	i = read<int>(address + 4);
	esi = esi ^ i;
	return esi;
}
//加密
void function::encrypt(INT32 Address, INT32 Value)
{
	INT32 EncryptId = 0;
	INT32 OffsetParam = 0;
	INT32 OffsetAddress = 0;
	INT32 Data = 0;
	INT32 AddressMask = 0;
	INT16 ax = 0;
	INT16 si = 0;
	EncryptId = read<int>(Address);
	OffsetParam = read<int>(read<int>(__解密基址) + (EncryptId >> 16) * 4 + 36);
	OffsetAddress = OffsetParam + (EncryptId & 0xFFFF) * 4 + 8468;
	OffsetParam = read<int>(OffsetAddress);
	Data = OffsetParam & 0xFFFF;
	Data += Data << 16;
	ax = OffsetParam & 0xFFFF;
	AddressMask = Address & 0xF;
	if (AddressMask == 0x0)
	{
		si = Value >> 16;
		si -= ax;
		si += Value;
	}
	else if (AddressMask == 0x4)
	{
		si = (Value & 0xFFFF) - (Value >> 16);
	}
	else if (AddressMask == 0x8)
	{
		si = Value >> 16;
		si *= Value;
	}
	else if (AddressMask == 0xC)
	{
		si = Value >> 16;
		si += Value;
		si += ax;
	}
	else
	{
		return;
	}
	ax ^= si;
	write<BYTE>(OffsetAddress + 2, (BYTE)ax);
	write<BYTE>(OffsetAddress + 3, (BYTE)(ax >> 8));
	write<int>(Address + 4, Data ^ Value);
}

int function::getGameStatus()
{
	return read<int>(__游戏状态);
}
POS function::getCurrentRoomPos()
{
	POS CurrentRoomPos;
	if (getGameStatus() == 1)
	{
		CurrentRoomPos.x = readOffset<int>(__遍历取值, { __大区域偏移 });
		CurrentRoomPos.y = readOffset<int>(__遍历取值, { __小区域偏移 });
	}
	else {
		DWORD OffsetAddress = readOffset<int>(__房间编号, { __时间基址 ,__门型偏移 });
		CurrentRoomPos.x = read<int>(OffsetAddress + __当前房间X);
		CurrentRoomPos.y = read<int>(OffsetAddress + __当前房间Y);
	}

	return CurrentRoomPos;
}

POS function::getBossRoomPos()
{
	POS BossRoomPos;
	DWORD OffsetAddress = readOffset<int>(__房间编号, { __时间基址 ,__门型偏移 });
	BossRoomPos.x = decrypt(OffsetAddress + __BOSS房间X);
	BossRoomPos.y = decrypt(OffsetAddress + __BOSS房间Y);
	return BossRoomPos;
}

bool function::isBossRoom()
{
	POS current_room_pos;
	POS boss_room_pos;

	current_room_pos = getCurrentRoomPos();
	boss_room_pos = getBossRoomPos();

	if (current_room_pos.x == boss_room_pos.x && current_room_pos.y == boss_room_pos.y)
	{
		return true;
	}
	return false;
}

bool function::isOpenDoor()
{
	if (decrypt(readOffset<int>(__人物基址, { __地图偏移 }) + __开门偏移) == 0)
	{
		return true;
	}
	else {
		return false;
	}
}