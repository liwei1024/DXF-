#pragma once

typedef struct _ROLE_POS
{
	POS room = {38,2};
	int x = 0;
	int y = 0;
	int z = 0;
} CITY_INFO,*PCITY_INFO,ROLE_POS,*PROLE_POS;
namespace role {
	int getRoleLevel();
	std::wstring getRoleJobName();
	int getCurrentRoleFatigueValue();
	ROLE_POS getRolePos();
	int getRoleStatus();
	void releaseSkillByKey(int keyCode, int s);
}

