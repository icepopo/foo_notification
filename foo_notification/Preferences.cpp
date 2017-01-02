#include "stdafx.h"
#include "resource.h"
#include "foo_notification.h"

template <class T, class T2, class T3>
class Pref {
public:
	GUID guid;
	T default_value;
	T2 cfg;
	T3 elem;

	Pref(GUID guid, T default_value, T2 cfg, int id) {
		this->guid = guid;
		this->default_value = default_value;
		this->cfg = T2(guid, default_value);
		this->elem = GetDlgItem(id);
	}
};


static const GUID guid_artist_checkbox = { 0x9cd130f7, 0x5d80, 0x4804, { 0x9b, 0x79, 0x11, 0xee, 0xcb, 0xd6 } };
static const bool cfg_artist_checkbox_default = false;
cfg_bool cfg_artist_checkbox(guid_artist_checkbox, cfg_artist_checkbox_default);

static const GUID guid_album_checkbox = { 0x67ef4917, 0x9a23, 0x480a, { 0x80, 0x59, 0x5d, 0xb9, 0xa4, 0x6b } };
static const bool cfg_album_checkbox_default = false;
cfg_bool cfg_album_checkbox(guid_album_checkbox, cfg_album_checkbox_default);

static const GUID guid_song_checkbox = { 0x1b547b01, 0x17dd, 0x4cb6, { 0xb9, 0x67, 0x84, 0x24, 0x39, 0x55 } };
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





//static cfg_string cfg_format1(guid_cfg_format1, default_cfg_format1), cfg_format2(guid_cfg_format2, default_cfg_format2), cfg_format3(guid_cfg_format3, default_cfg_format3);

class Preferences : public CDialogImpl<Preferences>, public preferences_page_instance {
	CCheckBox artist_checkbox;;
	CCheckBox album_checkbox;
	CCheckBox song_checkbox;
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


	artist_checkbox.SetCheck(cfg_artist_checkbox.get_value());
	album_checkbox.SetCheck(cfg_album_checkbox.get_value());
	song_checkbox.SetCheck(cfg_song_checkbox.get_value());

	SetDlgItemText(IDC_EDIT1, (LPCTSTR)cfg_format1_default);
	SetDlgItemText(IDC_EDIT2, (LPCTSTR)cfg_format2_default);
	SetDlgItemText(IDC_EDIT3, (LPCTSTR)cfg_format3_default);

	return FALSE;
}

void Preferences::OnEditChange(UINT, int, CWindow) {
	// not much to do here
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

	OnChanged();
}

void Preferences::apply() {
	cfg_artist_checkbox = artist_checkbox.IsChecked();
	cfg_album_checkbox = album_checkbox.IsChecked();
	cfg_song_checkbox = song_checkbox.IsChecked();

	//OnChanged(); //our dialog content has not changed but the flags have - our currently shown values now match the settings so the apply button can be disabled
}

bool Preferences::HasChanged() {
	if (cfg_artist_checkbox.get_value() != artist_checkbox.IsChecked()) return true;
	if (cfg_album_checkbox.get_value() != album_checkbox.IsChecked()) return true;
	if (cfg_song_checkbox.get_value() != song_checkbox.IsChecked()) return true;

	return false;
}
void Preferences::OnChanged() {
	//tell the host that our state has changed to enable/disable the apply button appropriately.
	m_callback->on_state_changed();
}

class preferences_page_myimpl : public preferences_page_impl<Preferences> {
public:
	const char * get_name() { return COMPONENT_NAME; }
	GUID get_guid() {
		static const GUID guid = { 0xd6ac2612 , 0x4e0b, 0x48d4,{ 0x86, 0x2a, 0x41, 0xc8, 0x65, 0xce } };
		return guid;
	}
	GUID get_parent_guid() { return guid_tools; }
};

static preferences_page_factory_t<preferences_page_myimpl> g_preferences_page_myimpl_factory;