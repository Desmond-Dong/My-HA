# SendSpin 功能集成说明

## 🎯 什么是 SendSpin？

SendSpin 是 ESPHome 的一个功能，允许将音频流从 ESPHome 设备发送到 Home Assistant 的其他媒体播放器。这使得：

1. **多房间音频同步** - 在多个设备上同步播放音频
2. **TTS 广播** - 语音助手的回复可以在多个扬声器播放
3. **音乐群组控制** - 从 reSpeaker 控制其他 HA 媒体播放器

## ✅ 已添加的组件

### 1. **SendSpin Hub**
```yaml
sendspin:
  id: sendspin_hub
  task_stack_in_psram: true
  kalman_process_error: 0.01
```

### 2. **媒体源**
```yaml
http_request:
  media_source:
    - platform: sendspin
      id: sendspin_source      # SendSpin 媒体源
    - platform: http_request
      id: http_source          # HTTP 音频流
      buffer_size: 500000
    - platform: file
      id: file_source          # 本地文件
```

### 3. **群组媒体播放器**
```yaml
media_player:
  - platform: sendspin
    id: sendspin_group_media_player
    name: "Group Media Player"
```

### 4. **主播放器更新**
添加了媒体源支持：
```yaml
sources:
  - file_source
  - http_source
  - sendspin_source
```

## 📋 **新增实体**

编译后在 Home Assistant 中会出现：

| 实体ID | 名称 | 类型 | 用途 |
|--------|------|------|------|
| `sendspin_group_media_player` | Group Media Player | media_player | 控制群组播放 |
| `external_media_player` | Media Player | media_player | 主播放器（已有，已更新） |

## 🎮 **使用场景**

### 场景 1: TTS 多房间广播
当语音助手回复时，音频会在所有配置的扬声器播放：

```yaml
# Home Assistant 自动化示例
automation:
  - alias: "VA回复多房间播放"
    trigger:
      - platform: state
        entity_id: media_player.respeaker_xvf3800_assistant
        to: 'playing'
    action:
      - service: media_player.join
        target:
          entity_id: media_player.living_room_speaker
        data:
          group_members:
            - media_player.bedroom_speaker
            - media_player.kitchen_speaker
```

### 场景 2: 从 reSpeaker 控制其他播放器
使用 Home Assistant 的媒体控制功能：

```yaml
service: media_player.play_media
target:
  entity_id: media_player.respeaker_xvf3800_assistant_group_media_player
data:
  media_content_id: "http://example.com/music.mp3"
  media_content_type: music
```

### 场景 3: 音量同步
```yaml
automation:
  - alias: "同步音量到其他播放器"
    trigger:
      - platform: state
        entity_id: media_player.respeaker_xvf3800_assistant
        attribute: volume_level
    action:
      - service: media_player.volume_set
        target:
          entity_id: media_player.other_speaker
        data:
          volume_level: "{{ trigger.to_state.state }}"
```

## 🔧 **配置要求**

### ESPHome 版本
- **最低要求**: ESPHome 2025.1.0 或更高版本
- **推荐**: ESPHome 2025.12.0（官方版本使用）

### Home Assistant 版本
- **最低要求**: Home Assistant 2024.11 或更高版本

### 必需的外部组件
以下组件已包含在 ESPHome 主分支（2025.1+）：
- `sendspin`
- `speaker_source`
- `http_request` (media_source)
- `file` (media_source)

## ⚙️ **高级配置**

### 调整缓冲区大小
如果遇到音频断续问题，可以增加缓冲区：

```yaml
http_request:
  media_source:
    - platform: http_request
      id: http_source
      buffer_size: 1000000  # 增加到 1MB
```

### Kalman 滤波器参数
调整音频同步的平滑度：

```yaml
sendspin:
  id: sendspin_hub
  task_stack_in_psram: true
  kalman_process_error: 0.001  # 更平滑但响应更慢（默认 0.01）
```

## 🐛 **故障排除**

### 问题 1: SendSpin 实体不显示
**原因**: ESPHome 版本过低
**解决**: 升级到 ESPHome 2025.1+

### 问题 2: 音频断续
**原因**: 缓冲区太小或网络延迟
**解决**:
1. 增加 `buffer_size`
2. 检查 WiFi 信号强度
3. 使用有线网络（如果可能）

### 问题 3: 无法控制其他播放器
**原因**: 需要在 HA 中配置媒体播放器组
**解决**: 使用 HA 的 `media_player.join` 服务创建组

## 📚 **参考资源**

- [ESPHome SendSpin PR](https://github.com/esphome/esphome/pull/12284)
- [官方配置文件](https://github.com/esphome/home-assistant-voice-pe/blob/dev/home-assistant-voice.yaml)
- [Home Assistant 媒体播放器文档](https://www.home-assistant.io/integrations/media_player/)

## 🎉 **享受多房间音频！**

现在您的 reSpeaker XVF3800 支持：
- ✅ 语音助手 TTS 广播到多个房间
- ✅ 从设备控制其他 HA 媒体播放器
- ✅ 同步音乐播放
- ✅ HTTP 音频流播放
