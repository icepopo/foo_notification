#pragma once
#include "stdafx.h"

class Contextmenu : public contextmenu_item_simple {
public:
	enum {
		cmd_toggle = 0,
		cmd_onetime,
		cmd_total
	};

    GUID get_parent() override;
	unsigned get_num_items() override;
	void get_item_name(unsigned p_index, pfc::string_base & p_out) override;
	void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) override;
	GUID get_item_guid(unsigned p_index) override;
	bool get_item_description(unsigned p_index, pfc::string_base & p_out) override;
	t_enabled_state get_enabled_state(unsigned p_index) override;
};
