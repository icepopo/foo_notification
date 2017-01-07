#include "stdafx.h"
#include "foo_notification.h"
#include "windows_notification.h"
/*
 * TODO: FIX ALL THE GUIDs
 */

DECLARE_COMPONENT_VERSION(
COMPONENT_NAME,
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