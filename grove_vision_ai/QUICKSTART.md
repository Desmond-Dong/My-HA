# Grove Vision AI V2 ESPHome Component - 快速开始

这个组件已经将Grove Vision AI V2的camera_web_server示例转换为ESPHome兼容的自定义组件。

## 已实现的功能

✅ SSCMA库通信（UART）
✅ AI推理触发
✅ 目标检测（边界框、类别、置信度）
✅ 分类结果解析
✅ 姿态估计（关键点）
✅ 地标点（landmarks）
✅ 性能指标解析
✅ 二进制传感器（触发器）
✅ 文本传感器（结果JSON）
✅ 自动化触发器
✅ **视频流（ESP32-S3专用）** - MJPEG流，支持实时查看

## 快速开始

### 1. 复制组件

将整个`grove_vision_ai`文件夹复制到你的ESPHome配置目录中。

### 2. 硬件连接

#### 视频流硬件要求（ESP32-S3）

视频流功能需要ESP32-S3和足够的PSRAM：

```yaml
esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

psram:
  mode: octal      # 或 opi
  speed: 80MHz     # 推荐80MHz
```

**注意：** 视频流功能需要至少8MB的PSRAM，建议使用ESP32-S3-WROOM-1或ESP32-S3-WROOM-2模块。

使用UART连接Grove Vision AI V2到ESP32：

```
Grove Vision AI V2    ESP32
TX      ->    RX (GPIO18)
RX      ->    TX (GPIO17)
GND     ->    GND
5V      ->    5V
```

### 3. 创建配置文件

复制`grove_vision_ai_example.yaml`并根据你的需求修改：

```yaml
esphome:
  name: your_device_name
  friendly_name: Grove Vision AI V2

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

# 导入本地组件
external_components:
  - source:
      type: local
      path: ./grove_vision_ai

# UART配置
uart:
  - id: uart_bus
    tx_pin: GPIO17
    rx_pin: GPIO18
    baud_rate: 921600
    rx_buffer_size: 32768

# Grove Vision AI组件
grove_vision_ai:
  id: vision_ai
  update_interval: 500ms

# Web服务器（用于视频流）
web_server:
  port: 80
  auth:
    username: admin
    password: !secret web_server_password

# 视频流组件（ESP32-S3专用）
video_stream:
  port: 8080

# 二进制传感器
binary_sensor:
  - platform: grove_vision_ai
    name: "Motion Detected"
    min_confidence: 0.6

# 自动化示例
automation:
  - alias: "Send notification on detection"
    trigger:
      - platform: grove_vision_ai
        on_detection:
          min_confidence: 0.7
    action:
      - service: notify.mobile_app
        data:
          title: "Detection!"
          message: "Object detected"
```

### 4. 编译和上传

```bash
esphome run grove_vision_ai_example.yaml
```

### 5. 在Home Assistant中查看

1. 打开Home Assistant
2. 转到Settings -> Devices & Services
3. 查找你的Grove Vision AI设备

### 6. 访问视频流（ESP32-S3）

启用视频流后，你可以通过以下方式访问：

#### 在浏览器中直接访问

```
http://<设备IP>:8080/stream
```

或获取检测结果：

```
http://<设备IP>:8080/stream/result
```

#### 在Home Assistant中使用

创建一个图片卡片来显示视频流：

```yaml
type: picture-glance
title: Grove Vision AI Stream
camera_image: camera.grove_vision_ai
entities:
  - binary_sensor.motion_detected
  - binary_sensor.person_detected
```

#### 使用VLC或其他播放器

```
http://<设备IP>:8080/stream
```

#### 在移动设备上

使用支持MJPEG流的应用，如：
- iOS: VLC, IP Cam Viewer
- Android: VLC, IP Webcam

## 视频流性能优化

### 调整帧率

在`video_stream/video_stream.h`中修改帧间隔：

```cpp
static const int FRAME_INTERVAL_MS = 100;  // 默认100ms = 10fps
```

降低帧率可以减少CPU使用和网络带宽：

- 50ms = 20fps（高帧率，高带宽）
- 100ms = 10fps（默认，平衡）
- 200ms = 5fps（低帧率，低带宽）

### 调整JPEG质量

在`video_stream/video_stream.h`中修改JPEG质量：

```cpp
static const int JPEG_QUALITY = 12;  // 默认12 (0-63，越低质量越高)
```

- 10-15: 高质量（推荐）
- 16-30: 中等质量
- 31-63: 低质量（节省带宽）

### 调整推理间隔

如果视频流卡顿，可以降低推理频率：

```yaml
grove_vision_ai:
  id: vision_ai
  update_interval: 1000ms  # 从500ms增加到1000ms
```

### 网络优化

- 使用5GHz WiFi以获得更好的性能
- 将设备靠近路由器
- 避免同时进行其他高带宽操作

## 使用示例
4. 查看检测数据和传感器

## 使用示例

### 检测特定对象（如人员）

```yaml
binary_sensor:
  - platform: grove_vision_ai
    name: "Person Detected"
    target_id: 0  # 假设人员是类别0
    min_confidence: 0.7
```

### 创建基于检测的自动化

```yaml
automation:
  - alias: "Turn on light when person detected"
    trigger:
      - platform: binary_sensor
        entity_id: binary_sensor.person_detected
        to: "on"
    action:
      - service: switch.turn_on
        target:
          entity_id: switch.living_room_light

  - alias: "Send detailed detection info"
    trigger:
      - platform: grove_vision_ai
        on_detection:
          min_confidence: 0.6
    action:
      - service: notify.mobile_app
        data:
          title: "Detection Details"
          message: |
            Target: {{ trigger.target_id }}
            Score: {{ trigger.score | round(2) }}
            Position: ({{ trigger.x }}, {{ trigger.y }})
            Size: {{ trigger.w }}x{{ trigger.h }}
```

### 多目标检测

```yaml
binary_sensor:
  - platform: grove_vision_ai
    name: "Person"
    target_id: 0
    min_confidence: 0.7

  - platform: grove_vision_ai
    name: "Car"
    target_id: 1
    min_confidence: 0.7

  - platform: grove_vision_ai
    name: "Dog"
    target_id: 2
    min_confidence: 0.6
```

## 故障排除

### 看不到检测结果

1. 检查日志：`esphome logs your_device.yaml`
2. 确认硬件连接正确
3. 降低`min_confidence`阈值
4. 确认Grove Vision AI V2已加载模型

### 连接失败

```
[17:32:45][D][grove_vision_ai:123]: SSCMA initialization command sent
[17:32:46][E][grove_vision_ai:098]: Failed to initialize SSCMA library
```

检查：
- UART引脚配置
- 波特率（默认921600）
- 硬件连接

### JSON解析错误

```
[17:32:45][W][grove_vision_ai:178]: JSON parse error: NoMemory
```

解决方案：
- 增加ESP32 PSRAM
- 减小JSON文档大小（修改StaticJsonDocument大小）

## 与原Arduino示例的区别

| 功能 | Arduino示例 | ESPHome组件 |
|-----|-----------|------------|
| 视频流 | ✅ Web服务器 | ✅ ESP32-S3专用 |
| 目标检测 | ✅ | ✅ |
| 追踪 | ✅ BYTETracker | ❌ 计划中 |
| 网页UI | ✅ | ✅ 通过HA UI |
| MQTT | ✅ | ✅ 通过ESPHome MQTT |
| 自定义动作 | ✅ | ❌ 计划中 |
| 自动化 | ❌ 需手动编写 | ✅ HA原生支持 |
| 传感器集成 | ❌ | ✅ HA原生支持 |

## 后续改进

- [ ] 添加对象跟踪（BYTETracker集成）
- [ ] 视频流检测结果叠加
- [ ] 支持I2C和SPI通信
- [ ] 图像捕获和上传
- [ ] 多区域检测
- [ ] 自定义动作支持
- [ ] 模型管理API

## 技术细节

### 响应格式

Grove Vision AI V2返回的JSON格式：

```json
{
  "type": 1,
  "name": "INVOKE",
  "data": {
    "boxes": [
      [x, y, w, h, score, target],
      ...
    ],
    "perf": [preprocess, inference, postprocess],
    "classes": [
      [score, target],
      ...
    ],
    "keypoints": [
      [[x, y, w, h, score, target], [[x, y, score, target], ...]],
      ...
    ],
    "points": [
      [x, y, score, target],
      ...
    ]
  }
}
```

### 性能考虑

- 默认更新间隔：500ms
- RX缓冲区大小：32KB
- 建议使用ESP32-S3以获得更好的性能

#### 视频流性能（ESP32-S3）

- 默认帧率：10fps（100ms间隔）
- JPEG质量：12（高质量）
- 推荐PSRAM：16MB或更大
- 网络带宽：约500KB/s（取决于质量）
- CPU使用率：约30-50%（取决于帧率和质量）

#### 优化建议

1. **降低帧率**以减少CPU使用
2. **降低JPEG质量**以节省带宽
3. **使用5GHz WiFi**以获得更稳定的连接
4. **增加PSRAM**以处理更大的图像缓冲

## 许可证

MIT License

## 支持

如有问题，请查看：
1. README.md - 详细文档
2. 日志输出 - `esphome logs your_device.yaml`
3. Grove Vision AI V2官方文档