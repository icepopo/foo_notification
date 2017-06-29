#include "stdafx.h"
#include "Contextmenu.h"
#include "foo_notification.h"
#include "Preferences.h"

// {1B26FC6E-6B60-43D5-9210-3EA23C5B7583}
static const GUID guid_contextmenu_group = { 0x1b26fc6e, 0x6b60, 0x43d5,{ 0x92, 0x10, 0x3e, 0xa2, 0x3c, 0x5b, 0x75, 0x83 } };

static contextmenu_group_popup_factory contextmenu_group(guid_contextmenu_group, contextmenu_groups::root, COMPONENT_NAME, 0);

// {D3F7C4B4-1984-4893-9F59-92033934628A}
static const GUID guid_toggle = { 0xd3f7c4b4, 0x1984, 0x4893,{ 0x9f, 0x59, 0x92, 0x3, 0x39, 0x34, 0x62, 0x8a } };

// {C437AAA5-8269-40FB-8EE4-631D3F895E31}
static const GUID guid_onetime = { 0xc437aaa5, 0x8269, 0x40fb,{ 0x8e, 0xe4, 0x63, 0x1d, 0x3f, 0x89, 0x5e, 0x31 } };

// {E69E0035-B8F2-4718-8FE9-8EA497FE990A}
static const GUID guid_playback_mode = { 0xe69e0035, 0xb8f2, 0x4718,{ 0x8f, 0xe9, 0x8e, 0xa4, 0x97, 0xfe, 0x99, 0xa } };

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
