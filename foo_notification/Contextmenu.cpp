#include "stdafx.h"
#include "Contextmenu.h"
#include "foo_notification.h"
#include "Preferences.h"

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
		case cmd_toggle: p_out = "Toggle shuffle-mode"; break;
		case cmd_onetime: p_out = "Show notification"; break;
		default: uBugCheck();
	}
}

void Contextmenu::context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) {
	switch (p_index) {
	case cmd_toggle:
		//
		Config::shuffle_mode = !Config::shuffle_mode;
		break;
	case cmd_onetime:
		//
		g_notification.show_notification_by_hand();
		break;
	default:
		uBugCheck();
	}
}

GUID Contextmenu::get_item_guid(unsigned p_index) {
	static const GUID guid_toggle = { 0xa0f37238, 0x70c4, 0x4db2,{ 0xac, 0x7a, 0x29, 0x05, 0xaf, 0x3f } };
	static const GUID guid_onetime = { 0x19c4bd56, 0xb166, 0x4a79,{ 0xa9, 0x34, 0x33, 0x52, 0x8d, 0x72 } };

	switch (p_index) {
		case cmd_toggle: return guid_toggle;
		case cmd_onetime: return guid_onetime;
		default: uBugCheck();
	}
}

bool Contextmenu::get_item_description(unsigned p_index, pfc::string_base & p_out) {
	switch (p_index) {
		case cmd_toggle:
			p_out = "This command toggles shuffle mode. When active it enables showing of the notification after every song, overriding usual settings.";
			return true;
		case cmd_onetime:
			p_out = "Show a notification despite the settings.";
			return true;
		default:
			uBugCheck();
	}
}

contextmenu_item::t_enabled_state Contextmenu::get_enabled_state(unsigned p_index) {
	return contextmenu_item::DEFAULT_ON;
}
