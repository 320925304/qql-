#pragma once

// 通用对齐工具。原本散落在 mpp_encoder / rga_preprocessor / rknn_infer 三处,
// 其中 mpp_encoder 用了算术形式 ((v+a-1)/a)*a,另两处用了位运算形式
// (v+a-1) & ~(a-1) (隐含 a 必须是 2 的幂)。统一用算术形式,对任何正整数
// 对齐值都正确,行为兼容所有调用方 (它们实际都传 16 或 64)。
namespace vip {

inline int alignUp(int value, int alignment) {
  return ((value + alignment - 1) / alignment) * alignment;
}

}  // namespace vip
