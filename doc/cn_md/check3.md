# CS144: 计算机网络导论

2024 年冬季

## 实验检查点 3：TCP 发送器

截止日期：2月8日 周四 下午 3:00

---

## 0. 概述

建议：实现前先完整阅读整个实验文档。

检查点 0：实现了流量控制的字节流（ByteStream）。检查点 1 和 2：实现了将数据报中的段转换为传入字节流的工具（Reassembler 和 TCPReceiver）。

检查点 3：实现另一侧——**TCPSender**，将输出字节流转换为将成为不可靠数据报载荷的 TCP 段。检查点 4 将把两者结合成完整的 TCP 实现。

---

## 1. 入门

1. 确保已提交检查点 2 的所有解决方案
2. 运行 `git fetch --all`
3. 下载：`git merge origin/check3-startercode`
4. 构建：`cmake -S . -B build && cmake --build build`
5. 编辑 `writeups/check3.md`

---

## 2. 检查点 3：TCP 发送器

TCP 发送器负责：
- 跟踪接收方的窗口（接收带有 acknos 和窗口大小的 TCPReceiverMessage）
- 尽可能填充窗口：从 ByteStream 读取，创建新的 TCP 段（SYN 和 FIN 标志如需要），并发送
- 跟踪已发送但尚未被接收方确认的段（"未完成"段）
- 如果未确认的段等待时间过长，则重新发送

> **为什么我要这样做？** 基本原则：发送接收方允许发送的任何内容（填充窗口），并持续重传直到接收方确认每个段。这叫做"自动重复请求"（ARQ）。

### 2.1 TCPSender 如何知道段丢失了？

发送方维护一个"未完成"段的集合，直到它们被完全确认。`tick` 方法定期被调用，告知自上次调用以来过去了多少毫秒。

**重传超时（RTO）规则**：

1. 每次调用 `tick(ms)` 时累计时间
2. 构造时给定初始 RTO 值（毫秒）
3. 实现重传定时器
4. 每次发送包含数据的段时，如果定时器未运行，则启动它
5. 所有未完成数据被确认时，停止重传定时器
6. 如果 tick 且重传定时器已过期：
   - 重传最早（序列号最小）的未完成段
   - 如果窗口大小非零：增加连续重传计数，RTO 加倍（指数退避）
   - 重置并重启定时器
7. 当接收方确认新数据时：RTO 恢复为初始值，重启定时器，重置连续重传计数

### 2.2 实现 TCPSender

四个重要事件：

1. **`void push(const TransmitFunction& transmit)`**
   - 填充窗口：尽可能多地读取并发送
   - 确保每个段都完全在接收方窗口内
   - 窗口大小为零时？假装窗口大小为 1，发送一个字节以触发新的确认

2. **`void receive(const TCPReceiverMessage& msg)`**
   - 接收来自接收方的消息，更新窗口边界
   - 移除已完全确认的未完成段

3. **`void tick(uint64_t ms_since_last_tick, const TransmitFunction& transmit)`**
   - 时间流逝，可能需要重传

4. **`TCPSenderMessage make_empty_message() const`**
   - 生成并发送一个序列号正确但长度为空的段

### 2.3 FAQ

- 初始窗口大小假设为多少？**1**
- 部分确认如何处理？等待完全确认
- 三个单独的段"a"、"b"、"c"未确认，能合并为一个"abc"重传吗？不需要，保持每个段独立追踪
- 需要存储空段吗？不，只有占用序列空间的段才需要追踪和重传

---

## 3. 开发与调试建议

1. 在 `tcp_sender.cc` 中实现
2. 测试：`cmake --build build --target check3`
3. 保持小提交、好消息的 Git 习惯
4. 保持代码可读性

---

## 4. 提交

1. 只修改 `libsponge/` 下的 `.hh` 和 `.cc` 文件
2. 提交前运行：
   - `cmake --build build --target format`
   - `cmake --build build --target check3`
3. 在 `writeups/check3.md` 中撰写报告

---

## 5. 额外奖励

为测试套件添加改进测试用例。

---

## 检查单

| 分数 | 任务 |
|------|------|
| ? 分 | TCPSender 实现正确 |
| ? 分 | 重传定时器逻辑正确 |
| ? 分 | 所有测试通过 |
| ? 分 | writeup 完整 |
