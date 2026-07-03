/**
 * @file globals.h
 * @brief Declarations for globally accessible variables and functions.
 */
#pragma once

// local includes
#include "entry_handler.h"
#include "thread_pool.h"

/**
 * @brief A thread pool for processing tasks.
 */
extern thread_pool_util::ThreadPool task_pool;

/**
 * @brief A boolean flag to indicate whether the cursor should be displayed.
 */
extern bool display_cursor;

/**
 * @brief Beam: the cursor flag the capture loops actually consume.
 *
 * Mirrors display_cursor, EXCEPT while every consumer of the (shared) capture has opted
 * out of the composited cursor — i.e. at least one Beam WebRTC session requested
 * client-side cursor rendering ({type:"client_cursor",enabled:true}) and there is no
 * Moonlight session and no stock WebRTC session left that still needs the cursor baked
 * into the video. Then it is forced false so DXGI desktop-duplication frames ship
 * cursor-free (DXGI delivers the pointer separately; skipping our blend is lossless and
 * reversible).
 *
 * Maintained by webrtc_stream's cursor watcher thread (Windows). Any code that toggles
 * display_cursor must mirror the new value into this flag (input.cpp does) — the watcher
 * re-applies suppression on its next tick when it is running.
 */
extern bool beam_effective_display_cursor;

#ifdef _WIN32
  // Declare global singleton used for NVIDIA control panel modifications
  #include "platform/windows/nvprefs/nvprefs_interface.h"

/**
 * @brief A global singleton used for NVIDIA control panel modifications.
 */
extern nvprefs::nvprefs_interface nvprefs_instance;
#endif

/**
 * @brief Handles process-wide communication.
 */
namespace mail {
#define MAIL(x) \
  constexpr auto x = std::string_view { \
    #x \
  }

  /**
   * @brief A process-wide communication mechanism.
   */
  extern safe::mail_t man;

  // Global mail
  MAIL(shutdown);
  MAIL(broadcast_shutdown);
  MAIL(video_packets);
  MAIL(audio_packets);
  MAIL(switch_display);

  // Local mail
  MAIL(touch_port);
  MAIL(idr);
  MAIL(invalidate_ref_frames);
  MAIL(gamepad_feedback);
  MAIL(hdr);
#undef MAIL

}  // namespace mail
