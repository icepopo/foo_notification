#include "stdafx.h"
#include "Preferences.h"

namespace Config {
	// {5FA2C1B3-F8DA-4917-B8FE-D642B642F746}
	const GUID guid_preference_page = { 0x5fa2c1b3, 0xf8da, 0x4917,{ 0xb8, 0xfe, 0xd6, 0x42, 0xb6, 0x42, 0xf7, 0x46 } };

	// {497B1F30-30DD-49B3-A078-29F05722EB29} 
	const GUID guid_artist_checkbox = { 0x497b1f30, 0x30dd, 0x49b3,{ 0xa0, 0x78, 0x29, 0xf0, 0x57, 0x22, 0xeb, 0x29 } };
    const bool cfg_artist_checkbox_default = false;
	cfg_bool cfg_artist_checkbox(guid_artist_checkbox, cfg_artist_checkbox_default);

	// {2F7954C0-FEA5-4A26-ACF7-E69416E96B96}
	const GUID guid_album_checkbox = { 0x2f7954c0, 0xfea5, 0x4a26,{ 0xac, 0xf7, 0xe6, 0x94, 0x16, 0xe9, 0x6b, 0x96 } };
	const bool cfg_album_checkbox_default = false;
	cfg_bool cfg_album_checkbox(guid_album_checkbox, cfg_album_checkbox_default);

	// {8ECD4B74-407A-4948-9F04-4D7A90171B6F}
	const GUID guid_song_checkbox = { 0x8ecd4b74, 0x407a, 0x4948,{ 0x9f, 0x4, 0x4d, 0x7a, 0x90, 0x17, 0x1b, 0x6f } };
	const bool cfg_song_checkbox_default = true;
	cfg_bool cfg_song_checkbox(guid_song_checkbox, cfg_album_checkbox_default);

	// {41BFCF67-D99F-406A-B047-AF29CF1AAE72}
	const GUID guid_cfg_format1 = { 0x41bfcf67, 0xd99f, 0x406a,{ 0xb0, 0x47, 0xaf, 0x29, 0xcf, 0x1a, 0xae, 0x72 } };
	const char *cfg_format1_default = "%artist%";
	cfg_string cfg_format1(guid_cfg_format1, cfg_format1_default);

	// {531F5401-837B-400B-9AB4-EBEB01300E10}
	const GUID guid_cfg_format2 = { 0x531f5401, 0x837b, 0x400b,{ 0x9a, 0xb4, 0xeb, 0xeb, 0x1, 0x30, 0xe, 0x10 } };
	const char *cfg_format2_default = "%album%";
	cfg_string cfg_format2(guid_cfg_format2, cfg_format2_default);

	// {F79BF417-4C95-41CA-A935-E09F3BE1574C}
	const GUID guid_cfg_format3 = { 0xf79bf417, 0x4c95, 0x41ca,{ 0xa9, 0x35, 0xe0, 0x9f, 0x3b, 0xe1, 0x57, 0x4c } };
	const char *cfg_format3_default = "%title%";
	cfg_string cfg_format3(guid_cfg_format3, cfg_format3_default);

	bool shuffle_mode = false;
	bool playback_mode = false;
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
	return Config::guid_preference_page;
}

GUID preferences_page_myimpl::get_parent_guid() {
	return guid_tools;
}