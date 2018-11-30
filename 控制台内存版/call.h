#pragma once
namespace call
{
	void 技能Call(int pointer, int code, int damage, int x, int y, int z);
	void 释放Call(int pointer, int code, int damage, int x, int y, int z);
	void 坐标Call(int pointer, int x, int y, int z);
	void 区域Call(PCITY_INFO city_info, int copy_id);
	void 公告CALL(std::wstring buffer, int type = 37, int color = 0xff0078ff);
};

