#include "stdafx.h"
#include "foo_notification.h"
#include "windows_notification.h"

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
	wchar_t* wc = new wchar_t[cSize];//check for nullptr
	mbstowcs(wc, string, cSize);

	return wc;
}

/*
 * Code taken from
 * http://foosion.foobar2000.org/
 */

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