#include "stdafx.h"
#include "resource.h"
#include "foo_notification.h"

static const GUID guid_artist_checkbox = { 0x9cd130f7, 0x5d80, 0x4804,{ 0x9b, 0x79, 0x11, 0xee, 0xcb, 0xd6 } };
static const bool cfg_artist_checkbox_default = false;
cfg_bool cfg_artist_checkbox(guid_artist_checkbox, cfg_artist_checkbox_default);

static const GUID guid_album_checkbox = { 0x67ef4917, 0x9a23, 0x480a,{ 0x80, 0x59, 0x5d, 0xb9, 0xa4, 0x6b } };
static const bool cfg_album_checkbox_default = false;
cfg_bool cfg_album_checkbox(guid_album_checkbox, cfg_album_checkbox_default);

static const GUID guid_song_checkbox = { 0x1b547b01, 0x17dd, 0x4cb6,{ 0xb9, 0x67, 0x84, 0x24, 0x39, 0x55 } };
static const bool cfg_song_checkbox_default = true;
cfg_bool cfg_song_checkbox(guid_song_checkbox, cfg_album_checkbox_default);

static const GUID guid_cfg_format1 = { 0xa1b8a78d, 0x460f, 0x471a,{ 0xbf, 0x02, 0xfd, 0x89, 0x82, 0x8d } };
static const char *cfg_format1_default = "%artist%";
cfg_string cfg_format1(guid_cfg_format1, cfg_format1_default);

static const GUID guid_cfg_format2 = { 0x723c6622, 0xdf03, 0x4cfa,{ 0xb4, 0x66, 0x2d, 0x88, 0x7d, 0xdd } };
static const char *cfg_format2_default = "%album%";
cfg_string cfg_format2(guid_cfg_format2, cfg_format2_default);

static const GUID guid_cfg_format3 = { 0x583fef1c, 0xd048, 0x48ca,{ 0x8e, 0xe3, 0x2b, 0x1b, 0x03, 0xd2 } };
static const char *cfg_format3_default = "%title%";
cfg_string cfg_format3(guid_cfg_format3, cfg_format3_default);

class Preferences : public CDialogImpl<Preferences>, public preferences_page_instance {
	CCheckBox artist_checkbox;;
	CCheckBox album_checkbox;
	CCheckBox song_checkbox;
	CEdit format1;
	CEdit format2;
	CEdit format3;
public:
	Preferences(preferences_page_callback::ptr callback) : m_callback(callback) {}
	enum { IDD = IDD_MYPREFERENCES };
	t_uint32 get_state();
	void apply();
	void reset();

	BEGIN_MSG_MAP(CMyPreferences)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_CHECK1, BN_CLICKED, OnEditChange)
		COMMAND_HANDLER_EX(IDC_CHECK2, BN_CLICKED, OnEditChange)
		COMMAND_HANDLER_EX(IDC_CHECK3, BN_CLICKED, OnEditChange)
		COMMAND_HANDLER_EX(IDC_EDIT1, EN_UPDATE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_EDIT2, EN_UPDATE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_EDIT3, EN_UPDATE, OnEditChange)

	END_MSG_MAP()
private:
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnEditChange(UINT, int, CWindow);
	bool HasChanged();
	void OnChanged();

	const preferences_page_callback::ptr m_callback;

};

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

class preferences_page_myimpl : public preferences_page_impl<Preferences> {
public:
	const char * get_name() { return COMPONENT_NAME; }
	GUID get_guid() {
		static const GUID guid = { 0x290cd872 , 0xcab3, 0x4756,{ 0x95, 0x40, 0xd2, 0x9b, 0xc3, 0xeb } };
		return guid;
	}
	GUID get_parent_guid() { return guid_tools; }
};

static preferences_page_factory_t<preferences_page_myimpl> g_preferences_page_myimpl_factory;