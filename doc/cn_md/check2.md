# CS144: 计算机网络导论

2025 年冬季

## 实验检查点 2：TCP 接收器

截止日期：1月26日 周日 晚上 11:59

---

## 0. 概述

建议：实现前先完整阅读整个实验文档。

在检查点 0 中，你实现了流量控制的字节流抽象（ByteStream）。在检查点 1 中，你创建了一个 Reassembler 来重新组装子字符串。这些模块将在你的 TCP 实现中发挥作用，但它们并不特定于 TCP 的细节。检查点 2 开始不同——你将实现 **TCPReceiver**，负责处理传入的字节流。

TCPReceiver 从对等方的发送方接收消息（通过 `receive()` 方法），并将其转换为对 Reassembler 的调用，最终写入传入的 ByteStream。应用程序从这个 ByteStream 读取数据。

同时，TCPReceiver 也生成返回给对等方发送方的消息（通过 `send()` 方法）。这些"接收方消息"负责告知发送方：
1. "第一个未组装的"字节的索引，称为"确认号"（ackno）
2. 输出 ByteStream 的可用容量，称为"窗口大小"

ackno 和窗口大小共同描述了接收方的窗口：TCP 发送方被允许发送的索引范围。

> **为什么我要这样做？** 这些信号对 TCP 在不可靠的数据报网络上提供流量控制的可靠字节流服务至关重要。

---

## 1. 入门

1. 确保已提交检查点 1 的所有解决方案
2. 运行 `git fetch --all` 获取最新版本
3. 下载：`git merge origin/check2-startercode`
4. 构建：`cmake -S . -B build && cmake --build build`
   - 注意：arm64 Mac 用户配置 cmake 使用 clang++：`cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++`
5. 编辑 `writeups/check2.md`

---

## 2. 检查点 2：TCP 接收器

TCP 是一种通过不可靠数据报可靠传输一对流量控制字节流的协议。两个对等方参与 TCP 连接，每个对等方同时充当"发送方"和"接收方"。

### 2.1 64 位索引与 32 位序列号之间的转换

**问题**：Reassembler 使用 64 位流索引（从零开始，不会溢出）。但 TCP 头部空间有限，每个字节的索引用 32 位"序列号"表示。这引入了三个复杂性：

1. **32 位整数会回绕**：流可以任意长，但 2^32 字节只有 4 GiB，超出后会从零重新开始
2. **序列号从随机值开始**：TCP 使用随机初始序列号（ISN）来避免混淆
3. **流的开始和结束各占一个序列号**：SYN 和 FIN 各占用一个序列号

三种索引方式：
- **序列号（seqno）**：从 ISN 开始，包含 SYN/FIN，32 位会回绕
- **绝对序列号**：从 0 开始，包含 SYN/FIN，64 位不回绕
- **流索引**：从 0 开始，不包含 SYN/FIN，64 位不回绕

### 实现转换函数

在 `wrapping_integers.cc` 中实现：

```cpp
static Wrap32 wrap(uint64_t n, Wrap32 zero_point);  // 绝对 seqno → seqno
uint64_t unwrap(Wrap32 zero_point, uint64_t checkpoint) const;  // seqno → 绝对 seqno
```

提示：
- `wrap` 预计 1 行代码
- `unwrap` 预计少于 10 行代码
- 使用 `wrapping_integers.hh` 中提供的辅助函数

### 2.2 实现 TCP 接收器

```cpp
class TCPReceiver {
    explicit TCPReceiver(Reassembler&& reassembler);
    void receive(TCPSenderMessage message);
    TCPReceiverMessage send() const;
};
```

`receive()` 方法需要：
- 必要时设置初始序列号（第一个带 SYN 标志的段的序列号）
- 将任何数据推送到 Reassembler（FIN 标志表示有效载荷的最后一个字节是整个流的最后一个字节）
- 需要将序列号解包为流索引

预计代码量：约 15 行。

---

## 3. 开发与调试建议

1. 在 `tcp_receiver.cc` 中实现，在 `tcp_receiver.hh` 中添加私有成员
2. 测试：`cmake --build build --target check2`
3. 保持小提交、好消息的 Git 习惯
4. 保持代码可读性，使用防御性编程

---

## 4. 提交

1. 只修改 `libsponge/` 目录下的 `.hh` 和 `.cc` 文件
2. 提交前运行：
   - `cmake --build build --target format`
   - `cmake --build build --target check2`
   - `cmake --build build --target tidy`（可选）
3. 在 `writeups/check2.md` 中撰写报告

---

## 5. 额外奖励

为测试套件添加改进（捕获真实 Bug 的测试用例），作为 Pull Request 提交。

---

## 检查单

| 分数 | 任务 |
|------|------|
| ? 分 | 包装/解包逻辑正确 |
| ? 分 | TCPReceiver 实现正确 |
| ? 分 | 所有测试通过 |
| ? 分 | writeup 完整 |
