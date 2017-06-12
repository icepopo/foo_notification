#include "stdafx.h"
#include "Preferences.h"

namespace Config {
	const GUID guid_artist_checkbox = { 0x9cd130f7, 0x5d80, 0x4804,{ 0x9b, 0x79, 0x11, 0xee, 0xcb, 0xd6 } };
    const bool cfg_artist_checkbox_default = false;
	cfg_bool cfg_artist_checkbox(guid_artist_checkbox, cfg_artist_checkbox_default);

	const GUID guid_album_checkbox = { 0x67ef4917, 0x9a23, 0x480a,{ 0x80, 0x59, 0x5d, 0xb9, 0xa4, 0x6b } };
	const bool cfg_album_checkbox_default = false;
	cfg_bool cfg_album_checkbox(guid_album_checkbox, cfg_album_checkbox_default);

	const GUID guid_song_checkbox = { 0x1b547b01, 0x17dd, 0x4cb6,{ 0xb9, 0x67, 0x84, 0x24, 0x39, 0x55 } };
	const bool cfg_song_checkbox_default = true;
	cfg_bool cfg_song_checkbox(guid_song_checkbox, cfg_album_checkbox_default);

	const GUID guid_cfg_format1 = { 0xa1b8a78d, 0x460f, 0x471a,{ 0xbf, 0x02, 0xfd, 0x89, 0x82, 0x8d } };
	const char *cfg_format1_default = "%artist%";
	cfg_string cfg_format1(guid_cfg_format1, cfg_format1_default);

	const GUID guid_cfg_format2 = { 0x723c6622, 0xdf03, 0x4cfa,{ 0xb4, 0x66, 0x2d, 0x88, 0x7d, 0xdd } };
	const char *cfg_format2_default = "%album%";
	cfg_string cfg_format2(guid_cfg_format2, cfg_format2_default);

	const GUID guid_cfg_format3 = { 0x583fef1c, 0xd048, 0x48ca,{ 0x8e, 0xe3, 0x2b, 0x1b, 0x03, 0xd2 } };
	const char *cfg_format3_default = "%title%";
	cfg_string cfg_format3(guid_cfg_format3, cfg_format3_default);

	bool shuffle_mode = false;
}

BOOL Preferences::OnInitDialog(CWindow, LPARAM) {
	artist_checkbox = GetDlgItem(IDC_CHECK1);
	album_checkbox = GetDlgItem(IDC_CHECK2);
	song_checkbox = GetDlgItem(IDC_CHECK3);
	format1 = GetDlgItem(IDC_EDIT1);
	format2 = GetDlgItem(IDC_EDIT2);
	format3 = GetDlgItem(IDC_EDIT3);

	artist_checkbox.SetCheck(Config::cfg_artist_checkbox.get_value());
	album_checkbox.SetCheck(Config::cfg_album_checkbox.get_value());
	song_checkbox.SetCheck(Config::cfg_song_checkbox.get_value());
	uSetWindowText(format1, Config::cfg_format1);
	uSetWindowText(format2, Config::cfg_format2);
	uSetWindowText(format3, Config::cfg_format3);

	return FALSE;
}

void Preferences::OnEditChange(UINT, int, CWindow) {
	OnChanged();
}

t_uint32 Preferences::get_state() {
	t_uint32 state = preferences_state::resettable;
	if (HasChanged()) state |= preferences_state::changed;
	return state;
}

void Preferences::reset() {
	artist_checkbox.SetCheck(Config::cfg_artist_checkbox_default);
	album_checkbox.SetCheck(Config::cfg_album_checkbox_default);
	song_checkbox.SetCheck(Config::cfg_song_checkbox_default);
	uSetWindowText(format1, Config::cfg_format1_default);
	uSetWindowText(format2, Config::cfg_format2_default);
	uSetWindowText(format3, Config::cfg_format3_default);

	OnChanged();
}

void Preferences::apply() {
	Config::cfg_artist_checkbox = artist_checkbox.IsChecked();
	Config::cfg_album_checkbox = album_checkbox.IsChecked();
	Config::cfg_song_checkbox = song_checkbox.IsChecked();

	uGetWindowText(format1, Config::cfg_format1);
	uGetWindowText(format2, Config::cfg_format2);
	uGetWindowText(format3, Config::cfg_format3);

	OnChanged();
}

bool Preferences::HasChanged() {
	if (Config::cfg_artist_checkbox.get_value() != artist_checkbox.IsChecked()) return true;
	if (Config::cfg_album_checkbox.get_value() != album_checkbox.IsChecked()) return true;
	if (Config::cfg_song_checkbox.get_value() != song_checkbox.IsChecked()) return true;

	pfc::string8 temp;
	uGetWindowText(format1, temp);
	if (Config::cfg_format1 != temp) return true;
	uGetWindowText(format2, temp);
	if (Config::cfg_format2 != temp) return true;
	uGetWindowText(format3, temp);
	if (Config::cfg_format3 != temp) return true;

	return false;
}
void Preferences::OnChanged() {
	m_callback->on_state_changed();
}

const char * preferences_page_myimpl::get_name() {
	return COMPONENT_NAME;
}

GUID preferences_page_myimpl::get_guid() {
	return guid_preference_page;
}

GUID preferences_page_myimpl::get_parent_guid() {
	return guid_tools;
}