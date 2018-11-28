#pragma once
class game_status_0
{
public:
	void manage();
private:
	int g_role_index = 0;
	int g_role_count = 0;
	int g_role_max_count = 0;
	int get_role_index();
	int get_role_count();
	int get_role_max_count();
	void select_role();
	void create_role();
};

