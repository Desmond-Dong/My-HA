# Grove Vision AI V2 - 视频流快速参考

## 硬件要求

### 必需
- ✅ ESP32-S3开发板
- ✅ 至少8MB PSRAM（推荐16MB）
- ✅ Grove Vision AI V2模块
- ✅ 稳定WiFi连接

### 推荐
- 5GHz WiFi网络
- ESP32-S3-WROOM-1/N8R8或更高规格

## 基本配置

```yaml
esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

psram:
  mode: octal
  speed: 80MHz

grove_vision_ai:
  id: vision_ai
  update_interval: 500ms

web_server:
  port: 80

video_stream:
  port: 8080
```

## 访问视频流

### 浏览器直接访问
```
http://<设备IP>:8080/stream
```

### 获取检测结果（JSON）
```
http://<设备IP>:8080/stream/result
```

### VLC播放器
1. 打开VLC
2. 媒体 -> 打开网络串流
3. 输入：`http://<设备IP>:8080/stream`
4. 播放

### Home Assistant图片卡片
```yaml
type: picture-glance
title: Grove Vision AI
camera_image: camera.grove_vision_ai
entities:
  - binary_sensor.motion_detected
  - binary_sensor.person_detected
```

### 移动应用
- **iOS**: VLC, IP Cam Viewer, iCamViewer
- **Android**: VLC, IP Webcam, TinyCam Monitor

## 性能调优

### 帧率调整
修改 `video_stream/video_stream.h`:
```cpp
static const int FRAME_INTERVAL_MS = 100;  // 10fps
```

| 帧率 | 间隔 | 用途 |
|-----|------|------|
| 20fps | 50ms | 高帧率，高CPU/带宽 |
| 10fps | 100ms | 默认，平衡 |
| 5fps | 200ms | 低帧率，低CPU/带宽 |

### JPEG质量调整
修改 `video_stream/video_stream.h`:
```cpp
static const int JPEG_QUALITY = 12;  // 0-63，越低质量越高
```

| 质量 | 值 | 用途 |
|-----|-----|------|
| 高 | 10-15 | 推荐，清晰 |
| 中 | 16-30 | 可接受 |
| 低 | 31-63 | 节省带宽 |

### 推理间隔调整
```yaml
grove_vision_ai:
  id: vision_ai
  update_interval: 1000ms  # 降低推理频率
```

## 常见问题

### Q: 视频流卡顿怎么办？
A:
1. 降低帧率（增加FRAME_INTERVAL_MS）
2. 降低JPEG质量（增加JPEG_QUALITY）
3. 使用5GHz WiFi
4. 减少同时连接的客户端

### Q: 无法访问视频流？
A:
1. 检查web_server是否启用
2. 确认端口8080未被占用
3. 检查防火墙设置
4. 确认PSRAM配置正确

### Q: 视频流黑屏？
A:
1. 确认Grove Vision AI V2已正确连接
2. 检查日志中的图像数据
3. 确认SSCMA模块支持图像传输
4. 查看UART通信是否正常

### Q: CPU使用率过高？
A:
1. 降低帧率
2. 降低JPEG质量
3. 增加推理间隔
4. 关闭不必要的传感器

### Q: 如何在视频流上显示检测结果？
A:
当前版本不支持直接叠加。未来版本将支持：
- 边界框绘制
- 类别标签
- 置信度显示

## 网络要求

### 带宽估算

| 质量 | 分辨率 | 帧率 | 带宽 |
|-----|--------|------|------|
| 高 | 320x240 | 10fps | ~800KB/s |
| 中 | 320x240 | 10fps | ~500KB/s |
| 低 | 320x240 | 5fps | ~250KB/s |

### 推荐网络配置
- WiFi: 5GHz，信号强度 > -60dBm
- 路由器: 支持802.11ac或更高
- 距离: <10米（视障碍物而定）

## Home Assistant集成示例

### 1. 创建相机卡片
```yaml
type: picture-elements
title: Grove Vision AI
image: "http://<设备IP>:8080/stream"
elements:
  - type: state-badge
    entity: binary_sensor.person_detected
    style:
      top: 5%
      left: 5%
  - type: state-badge
    entity: binary_sensor.motion_detected
    style:
      top: 5%
      left: 15%
```

### 2. 创建仪表板
```yaml
type: vertical-stack
cards:
  - type: picture-glance
    title: Live Stream
    camera_image: camera.grove_vision_ai
    entities:
      - binary_sensor.person_detected
      - binary_sensor.motion_detected
      - sensor.detection_count
  - type: entities
    title: Status
    entities:
      - sensor.wifi_signal
      - sensor.uptime
      - switch.enable_streaming
```

### 3. 自动化示例
```yaml
automation:
  - alias: "保存检测快照"
    trigger:
      - platform: grove_vision_ai
        on_detection:
          min_confidence: 0.9
    action:
      - service: camera.snapshot
        target:
          entity_id: camera.grove_vision_ai
        data:
          filename: "/config/www/detection_{{ now().strftime('%Y%m%d_%H%M%S') }}.jpg"
```

## 技术规格

### 视频流参数
- 编码: JPEG
- 格式: MJPEG流
- 最大分辨率: 取决于Grove Vision AI V2
- 最大帧率: ~20fps（取决于网络和CPU）
- 最大客户端: 1-2（推荐）

### 系统资源使用
- PSRAM: ~8-16MB
- CPU: 30-50%（10fps）
- 网络: ~500KB/s（10fps，高质量）
- 堆内存: ~100KB

## 故障排除命令

### 查看日志
```bash
esphome logs grove_vision_ai_stream.yaml
```

### 检查网络连接
```bash
ping <设备IP>
curl http://<设备IP>:8080/stream
```

### 监控CPU使用
```bash
esphome logs grove_vision_ai_stream.yaml | grep "Free Heap"
```

## 更新日志

### v1.0.0 (当前版本)
- ✅ 初始视频流支持
- ✅ MJPEG编码
- ✅ 实时推理结果
- ✅ HTTP流服务器
- ✅ 多客户端支持（有限）

### 计划功能
- [ ] 检测结果叠加
- [ ] H.264编码支持
- [ ] 多分辨率支持
- [ ] 录像功能
- [ ] WebRTC支持

## 支持

- 文档: README.md, QUICKSTART.md
- 问题反馈: GitHub Issues
- 讨论: GitHub Discussions