# Grove Vision AI V2 ESPHome Component

ESPHome自定义组件，用于集成Seeed Grove Vision AI V2 (Seeed_Arduino_SSCMA库)到Home Assistant。

## 功能

- 通过UART/I2C/SPI与Grove Vision AI V2通信
- 触发AI推理获取检测结果
- 支持目标检测（边界框、类别、置信度）
- 支持姿态估计（关键点）
- 提供传感器显示检测数量
- 提供二进制传感器用于触发自动化
- 提供文本传感器显示检测结果JSON
- 支持基于检测结果的自动化触发
- **视频流（ESP32-S3专用）** - MJPEG实时视频流，带AI检测结果叠加

## 硬件要求

### 基础功能
- ESP32或ESP32-S3开发板
- Grove Vision AI V2模块
- USB转TTL或直接连接（取决于使用UART/I2C/SPI）

### 视频流功能（ESP32-S3专用）
- ESP32-S3开发板（建议使用WROOM-1或WROOM-2）
- 至少8MB PSRAM（建议16MB或更大）
- 稳定的WiFi连接（推荐5GHz）
- Grove Vision AI V2模块

## 安装

1. 将`grove_vision_ai`文件夹复制到你的ESPHome配置目录
2. 在你的YAML配置文件中引用这个组件

## 接线方式

### UART连接（推荐）

| Grove Vision AI V2 | ESP32 |
|-------------------|-------|
| TX                | RX    |
| RX                | TX    |
| GND               | GND   |
| 5V/VCC            | 5V    |

示例GPIO配置：
- TX: GPIO17
- RX: GPIO18
- Baud rate: 921600

## 配置示例

```yaml
esphome:
  name: vision_ai_device
  friendly_name: Grove Vision AI V2

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

# Grove Vision AI V2 Component
external_components:
  - source:
      type: local
      path: ./grove_vision_ai

uart:
  - id: uart_bus
    tx_pin: GPIO17
    rx_pin: GPIO18
    baud_rate: 921600
    rx_buffer_size: 32768

grove_vision_ai:
  id: vision_ai
  update_interval: 500ms

# 二进制传感器
binary_sensor:
  - platform: grove_vision_ai
    name: "Motion Detected"
    min_confidence: 0.6

  - platform: grove_vision_ai
    name: "Person Detected"
    target_id: 0  # person class ID
    min_confidence: 0.7

# 自动化
automation:
  - alias: "Notify on person detection"
    trigger:
      - platform: grove_vision_ai
        on_detection:
          target_id: 0
          min_confidence: 0.7
    action:
      - service: notify.mobile_app
        data:
          title: "Person Detected!"
          message: "Person detected with high confidence"
```

## 配置参数

### 主组件

| 参数 | 类型 | 默认值 | 说明 |
|-----|------|--------|------|
| `id` | string | - | 组件ID |
| `update_interval` | time | 1000ms | 推理更新间隔 |

### 二进制传感器

| 参数 | 类型 | 默认值 | 说明 |
|-----|------|--------|------|
| `target_id` | int | -1 | 目标类别ID（-1表示任意目标） |
| `min_confidence` | float | 0.5 | 最小置信度阈值 |

### 文本传感器

| 参数 | 类型 | 默认值 | 说明 |
|-----|------|--------|------|
| `name` | string | - | 传感器名称 |

### 视频流组件（ESP32-S3专用）

| 参数 | 类型 | 默认值 | 说明 |
|-----|------|--------|------|
| `port` | int | 8080 | 视频流端口 |

**注意：** 视频流功能需要在ESP32-S3上配置PSRAM：

```yaml
esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

psram:
  mode: octal      # 或 opi
  speed: 80MHz     # 推荐80MHz
```

### 完整视频流配置示例

```yaml
esphome:
  name: vision_ai_stream
  friendly_name: Grove Vision AI Stream

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

psram:
  mode: octal
  speed: 80MHz

external_components:
  - source:
      type: local
      path: ./grove_vision_ai

uart:
  - id: uart_bus
    tx_pin: GPIO17
    rx_pin: GPIO18
    baud_rate: 921600
    rx_buffer_size: 32768

grove_vision_ai:
  id: vision_ai
  update_interval: 500ms

web_server:
  port: 80
  auth:
    username: admin
    password: !secret web_server_password

video_stream:
  port: 8080
```

## 访问视频流

### 在浏览器中

```
http://<设备IP>:8080/stream
```

### 获取检测结果

```
http://<设备IP>:8080/stream/result
```

### 在Home Assistant中

```yaml
type: picture-glance
title: Grove Vision AI
camera_image: camera.grove_vision_ai
entities:
  - binary_sensor.motion_detected
  - binary_sensor.person_detected
```

## 自动化触发器

```yaml
automation:
  - alias: "On Detection"
    trigger:
      - platform: grove_vision_ai
        on_detection:
          target_id: 0
          min_confidence: 0.7
```

自动化回调参数：
- `target_id`: 检测到的目标类别ID
- `x`, `y`: 边界框左上角坐标
- `w`, `h`: 边界框宽度和高度
- `score`: 检测置信度（0-1）

## 目标类别ID

根据Grove Vision AI V2上加载的模型不同，类别ID可能不同。常见模型：

- Person Detection: 0
- Face Detection: 0
- ...

请在你的设备上运行推理并查看日志以确认类别ID。

## 故障排除

### 无法连接

1. 检查接线是否正确
2. 确认波特率设置正确（默认921600）
3. 查看ESP日志获取详细错误信息

### 没有检测结果

1. 确认Grove Vision AI V2已正确加载模型
2. 调整`min_confidence`阈值
3. 检查摄像头是否正常工作
4. 查看日志中的原始响应数据

### 内存不足

- 减小`rx_buffer_size`
- 降低`update_interval`
- 使用ESP32-S3获得更多PSRAM

## 限制

- 视频流功能仅支持ESP32-S3（需要PSRAM）
- 追踪功能（BYTETracker）尚未实现
- 不支持自定义模型上传
- 视频流暂不支持检测结果叠加

## 后续开发

- [ ] 视频流检测结果叠加显示
- [ ] 支持I2C和SPI通信
- [ ] 实现对象跟踪（BYTETracker）
- [ ] 支持多区域检测
- [ ] 支持自定义模型上传
- [ ] 优化视频流性能

## 许可证

MIT License

## 参考

- [Seeed_Arduino_SSCMA](https://github.com/Seeed-Studio/Seeed_Arduino_SSCMA)
- [Grove Vision AI V2](https://www.seeedstudio.com/Grove-Vision-AI-V2-Kit-p-5852.html)
- [ESPHome Documentation](https://esphome.io/)