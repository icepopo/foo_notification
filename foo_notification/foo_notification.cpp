#include "stdafx.h"
#include "foo_notification.h"
#include "windows_notification.h"
#include "Contextmenu.h"
#include "Preferences.h"
/*
 * TODO: FIX ALL THE GUIDs
 */

foo_notification g_notification;

DECLARE_COMPONENT_VERSION(
COMPONENT_NAME,
"0.1",
"Foobar compontent showing a toast notification when asked nicely.\n"
);

class initquit_notification : public initquit {
	virtual void on_init() {
		g_notification.on_init();
	}
	virtual void on_quit() {
		g_notification.on_quit();
	}
};

initquit_factory_t<initquit_notification> g_foo;
contextmenu_item_factory_t<Contextmenu> contextmenu_factory;
preferences_page_factory_t<preferences_page_myimpl> g_preferences_page_myimpl_factory;

void foo_notification::on_init() {
	static_api_ptr_t<titleformat_compiler>()->compile_force(artist_format, "[%album artist%]");
	static_api_ptr_t<titleformat_compiler>()->compile_force(album_format, "[%album%]");
	static_api_ptr_t<titleformat_compiler>()->compile_force(title_format, "[%title%]");

	static_api_ptr_t<play_callback_manager>()->register_callback(this,
		flag_on_playback_new_track,
		false);
	
	wn = new windows_notification();
	wn->TryCreateShortcut();
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

	string8ToWide(artist, artistname);
	string8ToWide(album, albumname);
	string8ToWide(title, songname);
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
	string8ToWide(cover, coverpath);
}

void foo_notification::check_for_change(wchar_t* old_name, wchar_t* name, bool &status) {
	if (old_name != nullptr) {
		if (wcscmp(old_name, name) != 0) {
			status = true;
		}
	} else {
		status = true;
	}
}

void foo_notification::show_notification_by_hand() {
	metadb_handle_ptr p_track;
	if (static_api_ptr_t<playback_control>()->get_now_playing(p_track)) {
		show_notification(p_track, true);
	}
}

void foo_notification::show_notification(metadb_handle_ptr p_track, bool by_hand) {
	wchar_t* songname = nullptr;
	wchar_t* albumname = nullptr;
	wchar_t* artistname = nullptr;
	wchar_t* coverpath = nullptr;

	bool songChanged = false;
	bool albumChanged = false;
	bool artistChanged = false;

	get_track_info(p_track, songname, albumname, artistname);
	get_track_cover(p_track, coverpath);

	wchar_t* text[] = {
		songname,
		artistname,
		albumname
	};

	check_for_change(old_songname, songname, songChanged);
	check_for_change(old_albumname, albumname, albumChanged);
	check_for_change(old_artistname, artistname, artistChanged);

	bool showNotification = false;
	if (by_hand) {
		showNotification = true;
	} else if (songChanged && Config::cfg_song_checkbox) {
		showNotification = true;
	} else if (albumChanged && Config::cfg_album_checkbox) {
		showNotification = true;
	} else if (artistChanged && Config::cfg_artist_checkbox) {
		showNotification = true;
	} else if (Config::shuffle_mode) {
		showNotification = true;
	}

	if (showNotification) {
		wn->DisplayToast((wchar_t const*)coverpath, text);
	}

	//TODO: check if it work
	if (by_hand) {
		delete[] songname;
		delete[] albumname;
		delete[] artistname;
	} else {
		if (old_songname != nullptr) {
			delete[] old_songname;
		}

		if (old_albumname != nullptr) {
			delete[] old_albumname;
		}

		if (old_artistname != nullptr) {
			delete[] old_artistname;
		}

		old_songname = songname;
		old_albumname = albumname;
		old_artistname = artistname;
	}
	delete[] coverpath;
}

void foo_notification::on_playback_new_track(metadb_handle_ptr p_track) {
	show_notification(p_track);
	//TODO: make it so you can display the notification by hand multiple times, not limited by the settings;
}

void foo_notification::changedRandomMode() {
	wchar_t* title = L"Changed settings";
	wchar_t* shuffleOn = L"Shuffle mode was turned on";
	wchar_t* shuffleOff = L"Shuffle mode was turned off";
    wchar_t* blank = L"";
	wchar_t** shuffleOption = nullptr;

	if (Config::shuffle_mode) {
		shuffleOption = &shuffleOn;
	} else {
		shuffleOption = &shuffleOff;
	}

	wchar_t* text[] = {
		title,
		*shuffleOption,
		blank
	};

	wn->DisplayToast(blank, text);
}

void foo_notification::changePlaybackMode() {
	wchar_t* title = L"Changed settings";
	wchar_t* modeDefault = L"Changed playback mode to default";
	wchar_t* modeRandom = L"Changed playback mode to random";
	wchar_t* blank = L"";
	wchar_t** modeOption = nullptr;

	t_size active_playback = static_api_ptr_t<playlist_manager>()->playback_order_get_active();

	if (active_playback == 0) {
		modeOption = &modeRandom;
		static_api_ptr_t<playlist_manager>()->playback_order_set_active(3);
		Config::playback_mode = 1;
	} else {
		modeOption = &modeDefault;
		static_api_ptr_t<playlist_manager>()->playback_order_set_active(0);
		Config::playback_mode = 0;
	}

	wchar_t* text[] = {
		title,
		*modeOption,
		blank
	};

	wn->DisplayToast(blank, text);
}

size_t foo_notification::string8ToWide(pfc::string8 source, wchar_t *&dest) {
	dest = new wchar_t[source.length()+1];// TODO: should check for nullptr ...
	return pfc::stringcvt::convert_utf8_to_wide(dest, source.length() + 1, source, source.length() + 1);//+1 becouse the title and cover is getting cut one character
}