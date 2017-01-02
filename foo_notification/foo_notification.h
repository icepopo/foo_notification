#include "stdafx.h"

#define COMPONENT_NAME "Foo Notification"
#define COMPONENT_DLL_NAME "foo_notification"
#define NOTIFICATION_APP_NAME L"Foobar 2000"

class foo_notification :
	public initquit,
	public message_filter, 
	private play_callback {

public:
	void on_init() override;
	void on_quit() override;

private:
	wchar_t* cstrToWchar(const char *string);
	void get_track_info(metadb_handle_ptr p_track, wchar_t *&songname, wchar_t *&albumname, wchar_t *&artistname);
	void get_track_cover(metadb_handle_ptr p_track, wchar_t*& coverpath);

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