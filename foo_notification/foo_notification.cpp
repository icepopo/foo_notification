#include "stdafx.h"
#include "foo_notification.h"
#include "windows_notification.h"
#include "resource.h"

DECLARE_COMPONENT_VERSION(
"foo_notification",
"0.0.1",
"Foobar compontent showing a toast notification when asked nicely.\n"
);

initquit_factory_t<foo_notification> g_foo;

void foo_notification::on_init() {
	static_api_ptr_t<titleformat_compiler>()->compile_force(artist_format, "[%album artist%]");
	static_api_ptr_t<titleformat_compiler>()->compile_force(album_format, "[%album%]");
	static_api_ptr_t<titleformat_compiler>()->compile_force(title_format, "[%title%]");

	static_api_ptr_t<play_callback_manager>()->register_callback(this,
		flag_on_playback_new_track,
		false);

	windows_notification::TryCreateShortcut();
}

void foo_notification::on_quit() {

}

bool foo_notification::pretranslate_message(MSG* p_msg) {
	return true;
}

void foo_notification::get_track_info(metadb_handle_ptr p_track, wchar_t *&songname, wchar_t *&albumname, wchar_t *&artistname) {
	pfc::string8 artist;
	pfc::string8 album;
	pfc::string8 title;

	p_track->format_title(NULL, artist, artist_format, NULL);
	p_track->format_title(NULL, album, album_format, NULL);
	p_track->format_title(NULL, title, title_format, NULL);

	songname = cstrToWchar(title.c_str());
	albumname = cstrToWchar(album.c_str());
	artistname = cstrToWchar(artist.c_str());
}

void foo_notification::get_track_cover(metadb_handle_ptr p_track, wchar_t *&coverpath) {
	pfc::string8 cover;
	metadb_handle_list track_list;
	track_list.add_item(p_track);

	pfc::list_t<GUID> guids;
	guids.add_item(album_art_ids::cover_front);

	album_art_data::ptr data = nullptr;
	album_art_extractor_instance_v2::ptr extractor;
	try {
		extractor = static_api_ptr_t<album_art_manager_v2>()->open(track_list, guids, abort_dummy);
		album_art_path_list::ptr list = extractor->query_paths(album_art_ids::cover_front, abort_dummy);

		cover = (*list).get_path(0);
	}
	catch (exception_album_art_not_found) {
		cover = "not found cover.jpg";
	}
	console::formatter() << "Image hopefully: " << cover;

	coverpath = cstrToWchar(cover.c_str());
}
void foo_notification::on_playback_new_track(metadb_handle_ptr p_track) {
	wchar_t* songname = nullptr;
	wchar_t* albumname = nullptr;
	wchar_t* artistname = nullptr;
	get_track_info(p_track, songname, albumname, artistname);

	wchar_t* coverpath = nullptr;
	get_track_cover(p_track, coverpath);


	wchar_t* text[] = {
		songname,
		artistname,
		albumname
	};
	windows_notification::DisplayToast((wchar_t const*)coverpath, text);
	
	delete[] songname;
	delete[] albumname;
	delete[] artistname;
	delete[] coverpath;
}

wchar_t* foo_notification::cstrToWchar(const char *string) {
	const size_t cSize = strlen(string) + 1;
	wchar_t* wc = new wchar_t[cSize];//TODO(Artur): Check for nullptr
	mbstowcs(wc, string, cSize);

	return wc;
}

// Identifier of our context menu group. Substitute with your own when reusing code.
static const GUID guid_mygroup = { 0x572de7f4, 0xcbdf, 0x479a,{ 0x97, 0x26, 0xa, 0xb0, 0x97, 0x47, 0x69, 0xe3 } };


// Switch to contextmenu_group_factory to embed your commands in the root menu but separated from other commands.

//static contextmenu_group_factory g_mygroup(guid_mygroup, contextmenu_groups::root, 0);
static contextmenu_group_popup_factory g_mygroup(guid_mygroup, contextmenu_groups::root, "Sample component", 0);

static void RunTestCommand(metadb_handle_list_cref data);
//void RunCalculatePeak(metadb_handle_list_cref data); //decode.cpp

// Simple context menu item class.
class myitem : public contextmenu_item_simple {
public:
	enum {
		cmd_test1 = 0,
		cmd_test2,
		cmd_total
	};
	GUID get_parent() { return guid_mygroup; }
	unsigned get_num_items() { return cmd_total; }
	void get_item_name(unsigned p_index, pfc::string_base & p_out) {
		switch (p_index) {
		case cmd_test1: p_out = "Test command"; break;
		case cmd_test2: p_out = "Calculate peak"; break;
		default: uBugCheck(); // should never happen unless somebody called us with invalid parameters - bail
		}
	}
	void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) {
		switch (p_index) {
		case cmd_test1:
			RunTestCommand(p_data);
			break;
		case cmd_test2:
			//RunCalculatePeak(p_data);
			break;
		default:
			uBugCheck();
		}
	}
	// Overriding this is not mandatory. We're overriding it just to demonstrate stuff that you can do such as context-sensitive menu item labels.
	bool context_get_display(unsigned p_index, metadb_handle_list_cref p_data, pfc::string_base & p_out, unsigned & p_displayflags, const GUID & p_caller) {
		switch (p_index) {
		case cmd_test1:
			if (!__super::context_get_display(p_index, p_data, p_out, p_displayflags, p_caller)) return false;
			// Example context sensitive label: append the count of selected items to the label.
			p_out << " : " << p_data.get_count() << " item";
			if (p_data.get_count() != 1) p_out << "s";
			p_out << " selected";
			return true;
		case cmd_test2:
			return __super::context_get_display(p_index, p_data, p_out, p_displayflags, p_caller);
		default: uBugCheck();
		}
	}
	GUID get_item_guid(unsigned p_index) {
		// These GUIDs identify our context menu items. Substitute with your own GUIDs when reusing code.
		static const GUID guid_test1 = { 0x4021c80d, 0x9340, 0x423b,{ 0xa3, 0xe2, 0x8e, 0x1e, 0xda, 0x87, 0x13, 0x7f } };
		static const GUID guid_test2 = { 0xe629b5c3, 0x5af3, 0x4a1e,{ 0xa0, 0xcd, 0x2d, 0x5b, 0xff, 0xa6, 0x4, 0x58 } };
		switch (p_index) {
		case cmd_test1: return guid_test1;
		case cmd_test2: return guid_test2;
		default: uBugCheck(); // should never happen unless somebody called us with invalid parameters - bail
		}

	}
	bool get_item_description(unsigned p_index, pfc::string_base & p_out) {
		switch (p_index) {
		case cmd_test1:
			p_out = "This is a sample command.";
			return true;
		case cmd_test2:
			p_out = "This is a sample command that decodes the selected tracks and reports the peak sample value.";
			return true;
		default:
			uBugCheck(); // should never happen unless somebody called us with invalid parameters - bail
		}
	}
	virtual t_enabled_state get_enabled_state(unsigned p_index) { return contextmenu_item::DEFAULT_ON; }
};

static contextmenu_item_factory_t<myitem> g_myitem_factory;


static void RunTestCommand(metadb_handle_list_cref data) {
	pfc::string_formatter message;
	message << "This is a test command.\n";
	if (data.get_count() > 0) {
		message << "Parameters:\n";
		for (t_size walk = 0; walk < data.get_count(); ++walk) {
			message << data[walk] << "\n";
		}
	}
	popup_message::g_show(message, "Blah");
}

///////////



// Sample preferences interface: two meaningless configuration settings accessible through a preferences page and one accessible through advanced preferences.


// These GUIDs identify the variables within our component's configuration file.
static const GUID guid_cfg_bogoSetting1 = { 0xbd5c777, 0x735c, 0x440d,{ 0x8c, 0x71, 0x49, 0xb6, 0xac, 0xff, 0xce, 0xb8 } };
static const GUID guid_cfg_bogoSetting2 = { 0x752f1186, 0x9f61, 0x4f91,{ 0xb3, 0xee, 0x2f, 0x25, 0xb1, 0x24, 0x83, 0x5d } };

// This GUID identifies our Advanced Preferences branch (replace with your own when reusing code).
static const GUID guid_advconfig_branch = { 0x28564ced, 0x4abf, 0x4f0c,{ 0xa4, 0x43, 0x98, 0xda, 0x88, 0xe2, 0xcd, 0x39 } };
// This GUID identifies our Advanced Preferences setting (replace with your own when reusing code) as well as this setting's storage within our component's configuration file.
static const GUID guid_cfg_bogoSetting3 = { 0xf7008963, 0xed60, 0x4084,{ 0xa8, 0x5d, 0xd1, 0xcd, 0xc5, 0x51, 0x22, 0xca } };


enum {
	default_cfg_bogoSetting1 = 1337,
	default_cfg_bogoSetting2 = 666,
	default_cfg_bogoSetting3 = 42,
};

static cfg_uint cfg_bogoSetting1(guid_cfg_bogoSetting1, default_cfg_bogoSetting1), cfg_bogoSetting2(guid_cfg_bogoSetting2, default_cfg_bogoSetting2);

static advconfig_branch_factory g_advconfigBranch("Sample Component", guid_advconfig_branch, advconfig_branch::guid_branch_tools, 0);
static advconfig_integer_factory cfg_bogoSetting3("Bogo setting 3", guid_cfg_bogoSetting3, guid_advconfig_branch, 0, default_cfg_bogoSetting3, 0 /*minimum value*/, 9999 /*maximum value*/);

class CMyPreferences : public CDialogImpl<CMyPreferences>, public preferences_page_instance {
public:
	//Constructor - invoked by preferences_page_impl helpers - don't do Create() in here, preferences_page_impl does this for us
	CMyPreferences(preferences_page_callback::ptr callback) : m_callback(callback) {}

	//Note that we don't bother doing anything regarding destruction of our class.
	//The host ensures that our dialog is destroyed first, then the last reference to our preferences_page_instance object is released, causing our object to be deleted.


	//dialog resource ID
	enum { IDD = IDD_MYPREFERENCES };
	// preferences_page_instance methods (not all of them - get_wnd() is supplied by preferences_page_impl helpers)
	t_uint32 get_state();
	void apply();
	void reset();

	//WTL message map
	BEGIN_MSG_MAP(CMyPreferences)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_BOGO1, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_BOGO2, EN_CHANGE, OnEditChange)
	END_MSG_MAP()
private:
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnEditChange(UINT, int, CWindow);
	bool HasChanged();
	void OnChanged();

	const preferences_page_callback::ptr m_callback;

};

BOOL CMyPreferences::OnInitDialog(CWindow, LPARAM) {
	SetDlgItemInt(IDC_BOGO1, cfg_bogoSetting1, FALSE);
	SetDlgItemInt(IDC_BOGO2, cfg_bogoSetting2, FALSE);
	return FALSE;
}

void CMyPreferences::OnEditChange(UINT, int, CWindow) {
	// not much to do here
	OnChanged();
}

t_uint32 CMyPreferences::get_state() {
	t_uint32 state = preferences_state::resettable;
	if (HasChanged()) state |= preferences_state::changed;
	return state;
}

void CMyPreferences::reset() {
	SetDlgItemInt(IDC_BOGO1, default_cfg_bogoSetting1, FALSE);
	SetDlgItemInt(IDC_BOGO2, default_cfg_bogoSetting2, FALSE);
	OnChanged();
}

void CMyPreferences::apply() {
	cfg_bogoSetting1 = GetDlgItemInt(IDC_BOGO1, NULL, FALSE);
	cfg_bogoSetting2 = GetDlgItemInt(IDC_BOGO2, NULL, FALSE);

	OnChanged(); //our dialog content has not changed but the flags have - our currently shown values now match the settings so the apply button can be disabled
}

bool CMyPreferences::HasChanged() {
	//returns whether our dialog content is different from the current configuration (whether the apply button should be enabled or not)
	return GetDlgItemInt(IDC_BOGO1, NULL, FALSE) != cfg_bogoSetting1 || GetDlgItemInt(IDC_BOGO2, NULL, FALSE) != cfg_bogoSetting2;
}
void CMyPreferences::OnChanged() {
	//tell the host that our state has changed to enable/disable the apply button appropriately.
	m_callback->on_state_changed();
}

class preferences_page_myimpl : public preferences_page_impl<CMyPreferences> {
	// preferences_page_impl<> helper deals with instantiation of our dialog; inherits from preferences_page_v3.
public:
	const char * get_name() { return "Sample Component"; }
	GUID get_guid() {
		// This is our GUID. Replace with your own when reusing the code.
		static const GUID guid = { 0x7702c93e, 0x24dc, 0x48ed,{ 0x8d, 0xb1, 0x3f, 0x27, 0xb3, 0x8c, 0x7c, 0xc9 } };
		return guid;
	}
	GUID get_parent_guid() { return guid_tools; }
};

static preferences_page_factory_t<preferences_page_myimpl> g_preferences_page_myimpl_factory;