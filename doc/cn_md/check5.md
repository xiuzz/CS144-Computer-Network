# CS144: 计算机网络导论

2024 年冬季

## 实验检查点 5：深入栈（网络接口）

截止日期：2月29日 周四 晚上 11:00

---

## 0. 概述

本周的检查点，你将深入网络栈，实现一个 **网络接口**：互联网数据报与链路层以太网帧之间的桥梁。

网络接口可以"放在"你早期实验中的 TCP/IP 实现之下，但在不同场景下也会使用：在实验 6 中构建路由器时，它将在网络接口之间路由数据报。

### TCP 段如何传输？

三种方式：
- **TCP-in-UDP-in-IP**：使用 UDP 套接字，最容易实现
- **TCP-in-IP**：使用 TUN 设备，应用提供完整 IP 数据报
- **TCP-in-IP-in-Ethernet**：依赖 Linux 内核处理以太网帧

### ARP 协议

网络接口的主要任务是查找（和缓存）每个下一跳 IP 地址对应的以太网地址。协议叫做 **地址解析协议（ARP）**。

---

## 1. 入门

1. 确保已提交检查点 4 的所有解决方案
2. 运行 `git fetch --all`
3. 下载：`git merge origin/check5-startercode`
4. 构建：`cmake -S . -B build && cmake --build build`
5. 编辑 `writeups/check5.md`

---

## 2. 检查点 5：地址解析协议

你的主要任务是实现 NetworkInterface 的三个方法：

### 2.1 `void send_datagram(const InternetDatagram& dgram, const Address& next_hop)`

当调用者想发送出站 IP 数据报时调用：
- 如果目标以太网地址已知：立即发送
- 如果未知：广播 ARP 请求，并将 IP 数据报排队等待
- 注意：不要在 5 秒内重复发送相同 IP 的 ARP 请求

### 2.2 `void recv_frame(const EthernetFrame& frame)`

当以太网帧从网络到达时调用：
- 忽略不以广播地址或接口自身以太网地址为目标的帧
- 如果是 IPv4：解析为 InternetDatagram 并推送到接收队列
- 如果是 ARP：记住发送方 IP 到以太网地址的映射（30 秒有效），如果是 ARP 请求并针对我们的 IP，则发送 ARP 回复

### 2.3 `void tick(const size_t ms_since_last_tick)`

时间流逝，使过期的 IP 到以太网映射失效。

---

## 3. Q&A

- 预计代码量？`network_interface.cc` 约 100-150 行
- 如何"发送"以太网帧？调用 `transmit()`
- 缓存的数据结构？任意选择
- IP 地址转 32 位整数？使用 `Address::ipv4_numeric()`
- 如果 ARP 请求没有回复？不处理
- 如果排队的 IP 数据报永远等不到 ARP 回复？不处理

---

## 4. 开发与调试建议

1. 在 `network_interface.cc` 中实现
2. 测试：`cmake --build build --target check5`
3. 保持小提交、好消息的 Git 习惯
4. 保持代码可读性

---

## 5. 提交

1. 只修改 `libsponge/` 下的 `.hh` 和 `.cc` 文件
2. 提交前运行：
   - `cmake --build build --target format`
   - `cmake --build build --target check5`
3. 在 `writeups/check5.md` 中撰写报告：
   - 程序结构和设计
   - 实现挑战
   - 遗留 Bug
   - 花费时间

---

## 检查单

| 分数 | 任务 |
|------|------|
| ? 分 | NetworkInterface 实现正确 |
| ? 分 | ARP 缓存逻辑正确 |
| ? 分 | 所有测试通过 |
| ? 分 | writeup 完整 |
