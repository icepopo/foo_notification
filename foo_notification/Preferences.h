#pragma once
#include "stdafx.h"
#include "resource.h"
#include "foo_notification.h"
static const GUID guid_artist_checkbox = { 0x9cd130f7, 0x5d80, 0x4804,{ 0x9b, 0x79, 0x11, 0xee, 0xcb, 0xd6 } };
static const bool cfg_artist_checkbox_default = false;
static cfg_bool cfg_artist_checkbox(guid_artist_checkbox, cfg_artist_checkbox_default);

static const GUID guid_album_checkbox = { 0x67ef4917, 0x9a23, 0x480a,{ 0x80, 0x59, 0x5d, 0xb9, 0xa4, 0x6b } };
static const bool cfg_album_checkbox_default = false;
static cfg_bool cfg_album_checkbox(guid_album_checkbox, cfg_album_checkbox_default);

static const GUID guid_song_checkbox = { 0x1b547b01, 0x17dd, 0x4cb6,{ 0xb9, 0x67, 0x84, 0x24, 0x39, 0x55 } };
static const bool cfg_song_checkbox_default = true;
static cfg_bool cfg_song_checkbox(guid_song_checkbox, cfg_album_checkbox_default);

static const GUID guid_cfg_format1 = { 0xa1b8a78d, 0x460f, 0x471a,{ 0xbf, 0x02, 0xfd, 0x89, 0x82, 0x8d } };
static const char *cfg_format1_default = "%artist%";
static cfg_string cfg_format1(guid_cfg_format1, cfg_format1_default);

static const GUID guid_cfg_format2 = { 0x723c6622, 0xdf03, 0x4cfa,{ 0xb4, 0x66, 0x2d, 0x88, 0x7d, 0xdd } };
static const char *cfg_format2_default = "%album%";
static cfg_string cfg_format2(guid_cfg_format2, cfg_format2_default);

static const GUID guid_cfg_format3 = { 0x583fef1c, 0xd048, 0x48ca,{ 0x8e, 0xe3, 0x2b, 0x1b, 0x03, 0xd2 } };
static const char *cfg_format3_default = "%title%";
static cfg_string cfg_format3(guid_cfg_format3, cfg_format3_default);

 const GUID guid_preference_page = { 0x290cd872 , 0xcab3, 0x4756,{ 0x95, 0x40, 0xd2, 0x9b, 0xc3, 0xeb } };

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