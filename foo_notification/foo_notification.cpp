#include "stdafx.h"
#include "foo_notification.h"

DECLARE_COMPONENT_VERSION(
"foo_notification",
"0.0.1",
"Foobar compontent showing a toast notification when asked nicely.\n"
);

initquit_factory_t<foo_notification> g_foo;

void foo_notification::on_init() {
	console::formatter() << "'Hello world!', says " << core_version_info::g_get_version_string() << ".";

	static_api_ptr_t<titleformat_compiler>()->compile_force(artist_format, "[%album artist%]");
	static_api_ptr_t<titleformat_compiler>()->compile_force(album_format, "[%album%]");
	static_api_ptr_t<titleformat_compiler>()->compile_force(title_format, "[%title%]");

	static_api_ptr_t<play_callback_manager>()->register_callback(this,
		flag_on_playback_new_track |
		flag_on_playback_dynamic_info_track |
		flag_on_playback_stop,
		false);

}

void foo_notification::on_quit() {

}

bool foo_notification::pretranslate_message(MSG* p_msg) {
	return true;
}

void foo_notification::on_playback_new_track(metadb_handle_ptr p_track) {
	pfc::string8 artist;
	pfc::string8 album;
	pfc::string8 title;
	pfc::string8 cover;

	p_track->format_title(NULL, artist, artist_format, NULL);
	p_track->format_title(NULL, album,  album_format, NULL);
	p_track->format_title(NULL, title,  title_format, NULL);

	console::formatter() << "New track: " << artist <<", "<<album<<" - "<<title;


	metadb_handle_list list;
	list.add_item(p_track);

	pfc::list_t<GUID> guids;
	guids.add_item(album_art_ids::cover_front);

	album_art_data::ptr data = nullptr;

	album_art_extractor_instance_v2::ptr extractor;
	try {

		extractor = static_api_ptr_t<album_art_manager_v2>()->open(list, guids, abort_dummy);
		album_art_path_list::ptr list = extractor->query_paths(album_art_ids::cover_front, abort_dummy);
		cover = (*list).get_path(0);
	}
	catch (exception_album_art_not_found) {
		cover = "not found cover.jpg";
	}
	console::formatter() << "Image hopefully: " << cover;
}

void foo_notification::on_playback_stop(play_control::t_stop_reason reason) {
}

void foo_notification::on_playback_dynamic_info_track(const file_info& p_info) {
}

