#pragma once
#include "stdafx.h"
#include "resource.h"
#include "foo_notification.h"

namespace Config {
	extern const GUID guid_preference_page;

	extern const GUID guid_artist_checkbox;
	extern const bool cfg_artist_checkbox_default;
	extern cfg_bool cfg_artist_checkbox;

	extern const GUID guid_album_checkbox;
	extern const bool cfg_album_checkbox_default;
	extern cfg_bool cfg_album_checkbox;

	extern const GUID guid_song_checkbox;
	extern const bool cfg_song_checkbox_default;
	extern cfg_bool cfg_song_checkbox;

	extern const GUID guid_cfg_format1;
	extern const char *cfg_format1_default;
	extern cfg_string cfg_format1;

	extern const GUID guid_cfg_format2;
	extern const char *cfg_format2_default;
	extern cfg_string cfg_format2;

	extern const GUID guid_cfg_format3;
	extern const char *cfg_format3_default;
	extern cfg_string cfg_format3;

	extern bool shuffle_mode;
	extern bool playback_mode;
}

class Preferences : public CDialogImpl<Preferences>, public preferences_page_instance {
	CCheckBox artist_checkbox;
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
class preferences_page_myimpl : public preferences_page_impl<Preferences> {
public:
	const char * get_name() override;
	GUID get_guid() override;
	GUID get_parent_guid() override;
};