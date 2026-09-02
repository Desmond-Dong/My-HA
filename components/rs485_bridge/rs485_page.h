#pragma once

#include <cstddef>

namespace esphome {
namespace rs485_bridge {

// Self-contained WebSocket console page served at GET /rs485/ and /rs485.
static const char kPageHtml[] = R"HTML(
<!DOCTYPE html>
<html lang="zh">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>RS485 Bridge</title>
<style>
  body { font-family: monospace; background: #111; color: #ddd; margin: 12px; }
  h1 { font-size: 18px; color: #fff; }
  .bar { margin: 8px 0; }
  button, input, select { background: #222; color: #ddd; border: 1px solid #444;
    padding: 6px 10px; margin-right: 6px; font-family: inherit; }
  button:hover { background: #333; }
  #log { width: 100%; height: 55vh; background: #000; color: #0f0; border: 1px solid #333;
    padding: 6px; overflow: auto; white-space: pre-wrap; box-sizing: border-box; font-size: 13px; }
  #status { color: #9f9; }
</style>
</head>
<body>
<h1>RS485 Bridge &mdash; Tab5</h1>
<div class="bar">
  <label>服务器: ws://<span id="host"></span><span id="wspath"></span></label>
  <button id="btnConnect">连接</button>
  <button id="btnDisconnect" disabled>断开</button>
  <button id="btnClear">清屏</button>
  <button id="btnRefresh">刷新状态</button>
</div>
<div id="status">未连接</div>
<div class="bar">
  <label>模式:
    <select id="mode"><option value="hex">HEX</option><option value="text">TEXT</option></select>
  </label>
  <input id="tx" type="text" size="60" placeholder="例如 01 03 00 00 00 02 c4 0b">
  <button id="btnSend">发送</button>
</div>
<div id="log"></div>
<script>
"use strict";
var ws = null, hostEl = document.getElementById("host"), pathEl = document.getElementById("wspath");
var logEl = document.getElementById("log"), statusEl = document.getElementById("status");
var btnConnect = document.getElementById("btnConnect"), btnDisconnect = document.getElementById("btnDisconnect");
var btnSend = document.getElementById("btnSend"), btnClear = document.getElementById("btnClear");
var btnRefresh = document.getElementById("btnRefresh");
hostEl.textContent = location.hostname; pathEl.textContent = "/rs485/ws";
function log(line, cls) {
  var pre = document.createElement("div");
  if (cls) pre.style.color = cls;
  pre.textContent = line;
  logEl.appendChild(pre);
  logEl.scrollTop = logEl.scrollHeight;
}
function setStatus(t, ok) { statusEl.textContent = t; statusEl.style.color = ok ? "#9f9" : "#f99"; }
function toHex(buf) {
  var s = "";
  for (var i = 0; i < buf.length; i++) s += ("0" + buf[i].toString(16)).slice(-2) + " ";
  return s.trim();
}
function fromHex(str) {
  var t = str.replace(/[\s,;]+/g, "");
  if (t.length % 2 !== 0) throw "长度必须是偶数";
  var out = new Uint8Array(t.length / 2);
  for (var i = 0; i < out.length; i++) out[i] = parseInt(t.substr(i * 2, 2), 16);
  if (isNaN(out[0] * 0)) throw "包含非 HEX 字符";
  return out;
}
function render(buf) {
  var hex = toHex(buf), txt = "";
  for (var i = 0; i < buf.length; i++) {
    var c = buf[i];
    txt += (c >= 32 && c < 127) ? String.fromCharCode(c) : ".";
  }
  log("[RX] " + (hex || "(空)") + (txt ? "  |  " + txt : ""));
}
function connect() {
  var proto = location.protocol === "https:" ? "wss://" : "ws://";
  var url = proto + location.host + "/rs485/ws";
  try { ws = new WebSocket(url); } catch (e) { setStatus("连接失败: " + e.message, false); return; }
  ws.binaryType = "arraybuffer";
  ws.onopen = function() {
    setStatus("已连接 " + url, true);
    btnConnect.disabled = true; btnDisconnect.disabled = false;
    log("已连接 " + url, "#6af");
  };
  ws.onmessage = function(ev) { render(new Uint8Array(ev.data)); };
  ws.onerror = function() { setStatus("错误: 连接断开或无任何数据", false); };
  ws.onclose = function() {
    setStatus("已断开", false);
    btnConnect.disabled = false; btnDisconnect.disabled = true;
  };
}
function disconnect() { if (ws) { try { ws.close(); } catch (e) {} ws = null; } }
btnConnect.onclick = connect;
btnDisconnect.onclick = disconnect;
btnClear.onclick = function() { logEl.innerHTML = ""; };
btnSend.onclick = function() {
  var mode = document.getElementById("mode").value;
  var raw = document.getElementById("tx").value;
  if (!raw) return;
  if (!ws || ws.readyState !== 1) { setStatus("未连接", false); return; }
  try {
    var bytes = (mode === "hex") ? fromHex(raw) : new TextEncoder().encode(raw);
  } catch (e) { setStatus("输入错误: " + e.message, false); return; }
  ws.send(bytes);
  log("[TX] " + toHex(bytes), "#fa0");
};
btnRefresh.onclick = function() {
  fetch("/rs485/status").then(function(r) { return r.json(); }).then(function(j) {
    setStatus("波特率 " + j.baud_rate + " | RX待读 " + j.rx_waiting + " | WS " + j.ws_clients +
              " | tx_total " + j.tx_total + " | rx_total " + j.rx_total, true);
  }).catch(function() { setStatus("无法读取状态", false); });
};
document.getElementById("tx").addEventListener("keydown", function(e) {
  if (e.key === "Enter") btnSend.onclick();
});
</script>
</body>
</html>
)HTML";

}  // namespace rs485_bridge
}  // namespace esphome