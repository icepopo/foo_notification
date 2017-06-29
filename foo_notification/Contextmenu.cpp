#include "stdafx.h"
#include "Contextmenu.h"
#include "foo_notification.h"
#include "Preferences.h"

static const GUID guid_contextmenu_group = { 0x2afa0ba8 , 0xdb1e, 0x4ee5,{ 0xb7, 0xa8, 0xb3, 0x56, 0xc3, 0x2e } };
static contextmenu_group_popup_factory contextmenu_group(guid_contextmenu_group, contextmenu_groups::root, COMPONENT_NAME, 0);

static const GUID guid_toggle = { 0xa0f37238, 0x70c4, 0x4db2,{ 0xac, 0x7a, 0x29, 0x05, 0xaf, 0x3f } };
static const GUID guid_onetime = { 0x19c4bd56, 0xb166, 0x4a79,{ 0xa9, 0x34, 0x33, 0x52, 0x8d, 0x72 } };
static const GUID guid_playback_mode = { 0x7822f302, 0x9b84, 0x4f43,{ 0xa1, 0x98, 0x1, 0x72, 0xd4, 0x6e, 0x5f, 0x3f } };

GUID Contextmenu::get_parent() {
	return guid_contextmenu_group;
}

unsigned Contextmenu::get_num_items() {
	return cmd_total;
}

void Contextmenu::get_item_name(unsigned p_index, pfc::string_base & p_out) {
	switch (p_index) {
		case cmd_toggle: p_out = "Toggle shuffle mode"; break;
		case cmd_onetime: p_out = "Show notification"; break;
		case cmd_playback_mode: p_out = "Switch playback mode"; break;
		default: uBugCheck();
	}
}

void Contextmenu::context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) {
	switch (p_index) {
	case cmd_toggle:
		Config::shuffle_mode = !Config::shuffle_mode;
		console::formatter() << "Changed shuffle mode to: " << (Config::shuffle_mode ? "true" : "false");
		g_notification.changedRandomMode();
		break;
	case cmd_onetime:
		g_notification.show_notification_by_hand();
		console::formatter() << "Shown a notification by hand";
		break;
	case cmd_playback_mode:
		g_notification.changePlaybackMode();
		console::formatter() << "Changed playback mode to: " << (Config::playback_mode ? "random" : "default");
		break;
	default:
		uBugCheck();
	}
}

GUID Contextmenu::get_item_guid(unsigned p_index) {
	switch (p_index) {
		case cmd_toggle: return guid_toggle;
		case cmd_onetime: return guid_onetime;
		case cmd_playback_mode: return guid_playback_mode;
		default: uBugCheck();
	}
}

bool Contextmenu::get_item_description(unsigned p_index, pfc::string_base & p_out) {
	switch (p_index) {
		case cmd_toggle:
			p_out = "When active, it overrides the settings and shows the notification after every song change.";
			return true;
		case cmd_onetime:
			p_out = "Show a notification despite the settings.";
			return true;
		case cmd_playback_mode:
			p_out = "Switch beetween normal and random playback mode.";
			return true;
		default:
			uBugCheck();
	}
}

contextmenu_item::t_enabled_state Contextmenu::get_enabled_state(unsigned p_index) {
	return contextmenu_item::DEFAULT_ON;
}
