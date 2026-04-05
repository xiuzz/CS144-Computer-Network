# CS144: 计算机网络导论

2024 年冬季

## 实验检查点 4：在世界中互操作

截止日期：2月20日 周二 下午 3:00

---

## 0. 协作策略

与检查点 0 相同。请勿查看他人的代码或历年解决方案。

---

## 1. 概述

到目前为止，你已经以几乎完全符合标准的方式实现了 TCP。TCP 是世界上部署最广泛的计算机程序，在数十亿设备中运行。大多数实现使用与你不同的策略，但因为所有 TCP 实现共享一种通用语言，它们可以互操作。

检查点 4 是关于在真实世界中测试你的 TCP 实现。

如果你的 TCP 实现正确编写，你可能不需要为这个检查点编写任何代码。但尽管我们努力，Bug 可能会逃脱单元测试。如果发现问题，你可能需要用 Wireshark 调试并修复。

---

## 2. 入门

1. 确保已提交检查点 3 的所有解决方案
2. 运行 `git fetch --all`
3. 下载：`git merge origin/check4-startercode`
4. 构建：`cmake -S . -B build && cmake --build build`

---

## 3. 在真实世界中使用你的 TCP 实现

### 3.1 测试 webget

1. 运行 `cmake --build build --target check_webget` 确认 webget 仍然工作
2. 确保 TCP 实现从不向 cout 发送调试信息（调试信息用 cerr）
3. 合并检查点 4 的起始代码后，在 `apps/webget.cc` 顶部添加：
   ```cpp
   #include "tcp_minnow_socket.hh"
   ```
   并将 `TCPSocket` 改为 `CS144TCPSocket`
4. 运行 `./scripts/tun.sh start 144` 给予原始数据报发送权限
5. 重新运行 `cmake --build build --target check_webget`

### 3.2 手动调试

```bash
./build/apps/webget cs144.keithw.org /nph-hasher/xyzzy
```

成功输出示例：
```
HTTP/1.1 200 OK
Content-type: text/plain
7SmXqWkrLKzVBCEalbSPqBcvs11Pw263K7x4Wv3JckI
DEBUG: minnow TCP connection finished cleanly.
```

### 3.3 使用 tcp_ipv4 程序

在一个终端运行（服务器，使用 Linux TCP）：
```bash
./build/apps/tcp_native -l 0 9090
```

在另一个终端运行（客户端，使用你的 TCP）：
```bash
./build/apps/tcp_ipv4 169.254.144.1 9090
```

---

## 4. 收集数据

1. **选择一个远程主机**：RTT 至少 100 毫秒
   - www.cs.ox.ac.uk（牛津大学）
   - 162.105.253.58（北京大学）
   - www.canterbury.ac.nz（坎特伯雷大学）
   - 或其他 RTT ≥ 100ms 的选择

2. 使用 `mtr` 或 `traceroute` 追踪路由

3. 运行至少两小时的 ping：
   ```bash
   ping -D -n -i 0.2 hostname | tee data.txt
   ```
   （每 0.2 秒发送一次请求，约 36,000 个请求）

4. 注意：请不要超过每 5 秒 1 个包的速率

---

## 5. 分析数据

使用你选择的编程语言和图形工具准备报告（PDF 格式），包含：

1. 整体交付率（收到的回复数 / 发送的请求数）
2. 最长连续成功 ping 数
3. 最长连续丢失 burst 数
4. 丢包的时间独立性和相关性：
   - 给定第 N 个请求收到回复，第 N+1 个也成功的概率？
   - 给定第 N 个请求未收到回复，第 N+1 个成功的概率？
5. 最小 RTT
6. 最大 RTT
7. RTT 随时间变化的图表
8. RTT 分布的直方图或累积分布函数
9. 相邻 ping 的 RTT 相关性图表
10. 你的结论

---

## 提交

将报告作为 PDF 通过 Gradescope 提交。

---

## 检查单

| 分数 | 任务 |
|------|------|
| ? 分 | webget 使用你的 TCP 实现正常工作 |
| ? 分 | 数据分析报告完整 |
| ? 分 | RTT 统计图表齐全 |
