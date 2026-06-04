#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

// 通用 RGB 颜色。原本在 pipeline_runner.cpp 与 opencv_visualizer.cpp 两处
// 独立定义,集中到此处供 CPU 标注叠加与 OpenCV 可视化复用。
namespace vip {
struct RgbColor {
  std::uint8_t r = 0;
  std::uint8_t g = 0;
  std::uint8_t b = 0;
};

// Ultralytics YOLO 默认 20 色调色板 (按 classId 循环取色)。
// 同时被 CPU 端 NV12 叠加 (pipeline_runner.cpp) 与 OpenCV 可视化 (opencv_visualizer.cpp)
// 使用,保证两路输出一致。
inline constexpr RgbColor kUltralyticsPalette[] = {
    {4, 42, 255},   {11, 219, 235}, {243, 243, 243}, {0, 223, 183},  {17, 31, 104},
    {255, 111, 221}, {255, 68, 79},  {204, 237, 0},   {0, 243, 68},   {189, 0, 255},
    {0, 180, 255},  {221, 0, 186},  {0, 255, 255},   {38, 192, 0},   {1, 255, 179},
    {125, 36, 255}, {123, 0, 104},  {255, 27, 108},  {252, 109, 47}, {162, 255, 11},
};
inline constexpr std::size_t kUltralyticsPaletteSize =
    sizeof(kUltralyticsPalette) / sizeof(kUltralyticsPalette[0]);
}  // namespace vip

/**
 * 检测框结构
 */
struct BoundingBox {
  float x1 = 0.0f;    // 左上角 x
  float y1 = 0.0f;    // 左上角 y
  float x2 = 0.0f;    // 右下角 x
  float y2 = 0.0f;    // 右下角 y
  float score = 0.0f; // 置信度 (class_conf)
  int classId = 0;    // 类别 ID
  std::string label;  // 类别名称

  float width() const { return x2 - x1; }
  float height() const { return y2 - y1; }
  float area() const { return width() * height(); }
};

/**
 * 单帧检测结果
 */
struct DetectionResult {
  int64_t pts = 0;                       // 时间戳
  std::vector<BoundingBox> boxes;         // 检测框列表
  int imageWidth = 0;                     // 原图宽度
  int imageHeight = 0;                   // 原图高度
};

/**
 * YOLO 模型类型
 */
enum class YoloVersion {
  kYolov8,   // 84 = 4 bbox + 80 classes, 需要 NMS (一对多头部)
  kYolo26,   // 端到端无 NMS (一对一头部): (N, 300, 6)
             // 传统模式 (一对多头部): (N, 84, 8400), 需要 NMS
};

/**
 * COCO 80 类标签
 */
static const std::vector<std::string> kCocoLabels = {
    "person",        "bicycle",       "car",           "motorcycle",
    "airplane",      "bus",           "train",         "truck",
    "boat",          "traffic light", "fire hydrant",  "stop sign",
    "parking meter", "bench",         "bird",          "cat",
    "dog",           "horse",         "sheep",         "cow",
    "elephant",      "bear",          "zebra",         "giraffe",
    "backpack",      "umbrella",      "handbag",       "tie",
    "suitcase",      "frisbee",       "skis",          "snowboard",
    "sports ball",   "kite",          "baseball bat",  "baseball glove",
    "skateboard",    "surfboard",     "tennis racket", "bottle",
    "wine glass",    "cup",           "fork",          "knife",
    "spoon",         "bowl",          "banana",        "apple",
    "sandwich",      "orange",        "broccoli",      "carrot",
    "hot dog",       "pizza",         "donut",         "cake",
    "chair",         "couch",         "potted plant",  "bed",
    "dining table",  "toilet",        "tv",            "laptop",
    "mouse",         "remote",        "keyboard",      "cell phone",
    "microwave",     "oven",          "toaster",       "sink",
    "refrigerator",  "book",          "clock",         "vase",
    "scissors",      "teddy bear",    "hair drier",    "toothbrush"};
