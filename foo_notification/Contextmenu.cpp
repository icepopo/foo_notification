#include "stdafx.h"
#include "Contextmenu.h"
#include "foo_notification.h"

static const GUID guid_contextmenu_group = { 0x2afa0ba8 , 0xdb1e, 0x4ee5,{ 0xb7, 0xa8, 0xb3, 0x56, 0xc3, 0x2e } };
static contextmenu_group_popup_factory contextmenu_group(guid_contextmenu_group, contextmenu_groups::root, COMPONENT_NAME, 0);

GUID Contextmenu::get_parent() {
	return guid_contextmenu_group;
}

unsigned Contextmenu::get_num_items() {
	return cmd_total;
}

void Contextmenu::get_item_name(unsigned p_index, pfc::string_base & p_out) {
	switch (p_index) {
		case cmd_toggle: p_out = "Toggle Shuffle Mode"; break;
		case cmd_test2: p_out = "dadadad"; break;
		default: uBugCheck();
	}
}

void Contextmenu::context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) {
	switch (p_index) {
	case cmd_toggle:
		//
		break;
	case cmd_test2:
		//
		break;
	default:
		uBugCheck();
	}
}

GUID Contextmenu::get_item_guid(unsigned p_index) {
	static const GUID guid_toggle = { 0xa0f37238, 0x70c4, 0x4db2,{ 0xac, 0x7a, 0x29, 0x05, 0xaf, 0x3f } };
	static const GUID guid_test2 = { 0x19c4bd56, 0xb166, 0x4a79,{ 0xa9, 0x34, 0x33, 0x52, 0x8d, 0x72 } };

	switch (p_index) {
		case cmd_toggle: return guid_toggle;
		case cmd_test2: return guid_test2;
		default: uBugCheck();
	}
}

bool Contextmenu::get_item_description(unsigned p_index, pfc::string_base & p_out) {
	switch (p_index) {
		case cmd_toggle:
			p_out = "This is a sample command.";
			return true;
		case cmd_test2:
			p_out = "This is a sample command that decodes the selected tracks and reports the peak sample value.";
			return true;
		default:
			uBugCheck();
	}
}

contextmenu_item::t_enabled_state Contextmenu::get_enabled_state(unsigned p_index) {
	return contextmenu_item::DEFAULT_ON;
}
