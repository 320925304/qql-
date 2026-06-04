#pragma once

#include <string>

// 跨 TU 复用的字符串处理工具。
// 历史上这 4 个函数在 pipeline_runner / ffmpeg_packet_source / mpp_encoder /
// nvenc_encoder 各自匿名命名空间内逐行重复,现统一在此处,所有 TU 仅 include 即可。
namespace vip {

// ASCII 范围内 A-Z → a-z。其它字符原样保留。
inline std::string toLowerAscii(std::string value) {
  for (char& ch : value) {
    if (ch >= 'A' && ch <= 'Z') {
      ch = static_cast<char>(ch - 'A' + 'a');
    }
  }
  return value;
}

inline bool startsWithIgnoreCase(const std::string& value, const std::string& prefix) {
  const std::string lowerValue = toLowerAscii(value);
  const std::string lowerPrefix = toLowerAscii(prefix);
  return lowerValue.size() >= lowerPrefix.size() &&
         lowerValue.compare(0, lowerPrefix.size(), lowerPrefix) == 0;
}

inline bool hasSuffixIgnoreCase(const std::string& value, const std::string& suffix) {
  const std::string lowerValue = toLowerAscii(value);
  const std::string lowerSuffix = toLowerAscii(suffix);
  return lowerValue.size() >= lowerSuffix.size() &&
         lowerValue.compare(lowerValue.size() - lowerSuffix.size(), lowerSuffix.size(), lowerSuffix) == 0;
}

inline bool isRtspUrl(const std::string& value) {
  return startsWithIgnoreCase(value, "rtsp://");
}

}  // namespace vip
