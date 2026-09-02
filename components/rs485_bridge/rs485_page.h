#pragma once

#include <cstddef>

namespace esphome {
namespace rs485_bridge {

// Self-contained polling console page served at GET /rs485/ by the shared
// ESPHome web server. Polls GET /rs485/rx?raw=1 (drains the bridge's ring
// buffer server-side, so nothing is lost between polls) and POSTs to
// /rs485/tx. No WebSocket involved.
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
  <label>地址: http://<span id="host"></span>/rs485/</label>
  <button id="btnPause">暂停接收</button>
  <button id="btnClear">清屏</button>
  <button id="btnRefresh">刷新状态</button>
</div>
<div id="status">轮询中</div>
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
var paused = false, busy = false;
var hostEl = document.getElementById("host");
var logEl = document.getElementById("log"), statusEl = document.getElementById("status");
var btnPause = document.getElementById("btnPause"), btnClear = document.getElementById("btnClear");
var btnSend = document.getElementById("btnSend"), btnRefresh = document.getElementById("btnRefresh");
hostEl.textContent = location.hostname;
function log(line, cls) {
  var pre = document.createElement("div");
  if (cls) pre.style.color = cls;
  pre.textContent = line;
  logEl.appendChild(pre);
  while (logEl.childNodes.length > 500) logEl.removeChild(logEl.firstChild);
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
  for (var i = 0; i < out.length; i++) {
    out[i] = parseInt(t.substr(i * 2, 2), 16);
    if (isNaN(out[i])) throw "包含非 HEX 字符";
  }
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
async function pollRx() {
  if (paused || busy || document.hidden) return;
  busy = true;
  try {
    var r = await fetch("/rs485/rx?raw=1", {cache: "no-store"});
    var buf = new Uint8Array(await r.arrayBuffer());
    if (buf.length) render(buf);
    setStatus("轮询中 (" + (paused ? "已暂停" : "活动") + ")", true);
  } catch (e) {
    setStatus("读取失败: " + e.message, false);
  } finally {
    busy = false;
  }
}
btnPause.onclick = function() {
  paused = !paused;
  btnPause.textContent = paused ? "恢复接收" : "暂停接收";
};
btnClear.onclick = function() { logEl.innerHTML = ""; };
btnSend.onclick = async function() {
  var mode = document.getElementById("mode").value;
  var raw = document.getElementById("tx").value;
  if (!raw) return;
  var bytes;
  try {
    bytes = (mode === "hex") ? fromHex(raw) : new TextEncoder().encode(raw);
  } catch (e) { setStatus("输入错误: " + e.message, false); return; }
  try {
    var r = await fetch("/rs485/tx", {method: "POST", body: bytes});
    var j = await r.json();
    log("[TX] " + toHex(bytes) + "  (" + j.written + " 字节)", "#fa0");
  } catch (e) {
    setStatus("发送失败: " + e.message, false);
  }
};
btnRefresh.onclick = function() {
  fetch("/rs485/status").then(function(r) { return r.json(); }).then(function(j) {
    setStatus("波特率 " + j.baud_rate + " | RX待读 " + j.rx_waiting +
              " | tx_total " + j.tx_total + " | rx_total " + j.rx_total, true);
  }).catch(function() { setStatus("无法读取状态", false); });
};
document.getElementById("tx").addEventListener("keydown", function(e) {
  if (e.key === "Enter") btnSend.onclick();
});
setInterval(pollRx, 300);
pollRx();
</script>
</body>
</html>
)HTML";

}  // namespace rs485_bridge
}  // namespace esphome
