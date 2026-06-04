#pragma once

// FFmpeg 错误抛出助手。
// 历史上 4 个 TU 各自写了一份"调用 av_strerror 把描述拼到 message 末尾再 throw"
// 的模板 (ffmpeg_packet_source / nvdec_decoder / nvenc_encoder / mpp_encoder),
// 集中到此 header 之后所有 TU 仅 include 并使用 vip::throwFfmpegError(...) 即可。

extern "C" {
#include <libavutil/error.h>
}

#include <stdexcept>
#include <string>

namespace vip {

[[noreturn]] inline void throwFfmpegError(const std::string& message, int errorCode) {
  char errorBuffer[AV_ERROR_MAX_STRING_SIZE] = {};
  if (av_strerror(errorCode, errorBuffer, sizeof(errorBuffer)) < 0) {
    errorBuffer[0] = '\0';
  }
  throw std::runtime_error(message + ": " + errorBuffer);
}

}  // namespace vip
