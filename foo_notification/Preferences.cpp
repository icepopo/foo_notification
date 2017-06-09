#include "stdafx.h"
#include "Preferences.h"

BOOL Preferences::OnInitDialog(CWindow, LPARAM) {
	artist_checkbox = GetDlgItem(IDC_CHECK1);
	album_checkbox = GetDlgItem(IDC_CHECK2);
	song_checkbox = GetDlgItem(IDC_CHECK3);
	format1 = GetDlgItem(IDC_EDIT1);
	format2 = GetDlgItem(IDC_EDIT2);
	format3 = GetDlgItem(IDC_EDIT3);

	artist_checkbox.SetCheck(cfg_artist_checkbox.get_value());
	album_checkbox.SetCheck(cfg_album_checkbox.get_value());
	song_checkbox.SetCheck(cfg_song_checkbox.get_value());
	uSetWindowText(format1, cfg_format1);
	uSetWindowText(format2, cfg_format2);
	uSetWindowText(format3, cfg_format3);

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
	artist_checkbox.SetCheck(false);
	album_checkbox.SetCheck(false);
	song_checkbox.SetCheck(true);
	uSetWindowText(format1, cfg_format1_default);
	uSetWindowText(format2, cfg_format2_default);
	uSetWindowText(format3, cfg_format3_default);

	OnChanged();
}

void Preferences::apply() {
	cfg_artist_checkbox = artist_checkbox.IsChecked();
	cfg_album_checkbox = album_checkbox.IsChecked();
	cfg_song_checkbox = song_checkbox.IsChecked();

	uGetWindowText(format1, cfg_format1);
	uGetWindowText(format2, cfg_format2);
	uGetWindowText(format3, cfg_format3);

	OnChanged();
}

bool Preferences::HasChanged() {
	if (cfg_artist_checkbox.get_value() != artist_checkbox.IsChecked()) return true;
	if (cfg_album_checkbox.get_value() != album_checkbox.IsChecked()) return true;
	if (cfg_song_checkbox.get_value() != song_checkbox.IsChecked()) return true;

	pfc::string8 temp;
	uGetWindowText(format1, temp);
	if (cfg_format1 != temp) return true;
	uGetWindowText(format2, temp);
	if (cfg_format2 != temp) return true;
	uGetWindowText(format3, temp);
	if (cfg_format3 != temp) return true;

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