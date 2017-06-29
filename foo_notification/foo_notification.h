#pragma once
#include "stdafx.h"
#include "windows_notification.h"

#define COMPONENT_NAME "Foo Notification"
#define COMPONENT_DLL_NAME "foo_notification"
#define NOTIFICATION_APP_NAME L"Foobar 2000"

class foo_notification :
	public message_filter, 
	private play_callback {

public:
	void on_init();
	void on_quit();

	void show_notification_by_hand();
	void changedRandomMode();
	void changePlaybackMode();
private:

	windows_notification *wn;

	wchar_t* old_songname = nullptr;
	wchar_t* old_albumname = nullptr;
	wchar_t* old_artistname = nullptr;

	void show_notification(metadb_handle_ptr p_track, bool by_hand = false);
	size_t string8ToWide(pfc::string8 source, wchar_t *&dest);
	void get_track_info(metadb_handle_ptr p_track, wchar_t *&songname, wchar_t *&albumname, wchar_t *&artistname);
	void get_track_cover(metadb_handle_ptr p_track, wchar_t*& coverpath);
	void check_for_change(wchar_t* old_name, wchar_t* name, bool &status);

	titleformat_object::ptr artist_format;
	titleformat_object::ptr album_format;
	titleformat_object::ptr title_format;

	abort_callback_impl abort_dummy;
	// message_filter methods
	bool pretranslate_message(MSG * p_msg) override;

	// play_callback methods (the ones we're interested in)
	void on_playback_new_track(metadb_handle_ptr p_track) override;

	// play_callback methods (the rest)
	virtual void on_playback_stop(play_control::t_stop_reason reason) {}
	virtual void on_playback_dynamic_info_track(const file_info & p_info) {}
	virtual void on_playback_starting(play_control::t_track_command p_command, bool p_paused) {}
	virtual void on_playback_seek(double p_time) {}
	virtual void on_playback_pause(bool p_state) {}
	virtual void on_playback_edited(metadb_handle_ptr p_track) {}
	virtual void on_playback_dynamic_info(const file_info & p_info) {}
	virtual void on_playback_time(double p_time) {}
	virtual void on_volume_change(float p_new_val) {}
};

extern foo_notification g_notification;