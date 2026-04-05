# CS144: 计算机网络导论

2024 年冬季

## 实验检查点 6：构建 IP 路由器

截止日期：3月7日 周四 晚上 11:00

---

## 0. 协作策略

与检查点 0 相同。请勿查看他人的代码或历年解决方案。

---

## 1. 概述

本周的检查点是基于你现有的 NetworkInterface 实现一个 IP 路由器。路由器有多个网络接口，可以从任何一个接收互联网数据报。路由器的任务是根据路由表转发收到的数据报。

你的任务是实现一个路由器，能够为任何给定的数据报确定：
- 使用哪个接口发送
- 下一跳的 IP 地址

你不需要实现生成路由表的算法（如 RIP、OSPF、BGP），只需要实现根据路由表转发的算法。

预计代码量：**30-60 行**

---

## 2. 入门

1. 确保已提交检查点 5 的所有解决方案
2. 运行 `git fetch --all`
3. 下载：`git merge origin/check6-startercode`
4. 构建：`cmake -S . -B build && cmake --build build`
5. 编辑 `writeups/check6.md`

---

## 3. 实现路由器

在 `router.hh` 和 `router.cc` 中实现 Router 类。

### 3.1 `void add_route(uint32_t route_prefix, uint8_t prefix_length, optional<Address> next_hop, size_t interface_num)`

添加路由到路由表。路由由"匹配-动作"规则组成：
- **匹配**：路由前缀和前缀长度共同指定一个 IP 地址范围
- **动作**：如果路由器直接连接到目标网络，next_hop 为空（使用数据报的目标地址）；否则为下一跳路由器的 IP

### 3.2 `void route()`

路由每一个到来的数据报：
- 在路由表中搜索匹配数据报目标地址的路由
- 在匹配的路由中，选择前缀长度最大的（最长前缀匹配）
- 如果没有匹配，则丢弃数据报
- 减少数据报的 TTL；如果 TTL 为零则丢弃
- 否则，通过适当的接口发送到适当的下一跳

### 关键点

- 使用 `interface(interface_num)->send_datagram()` 发送
- 最长前缀匹配是最核心的逻辑
- 路由器不需要知道 TCP、ARP 或以太网帧

---

## 4. 测试

运行：`cmake --build build --target check5`
（检查点 6 使用相同的测试网络）

---

## 5. Q&A

- 路由表的数据结构？O(N) 工作即可，不需要过度优化
- IP 地址转换？`Address::ipv4_numeric()` 和 `Address::from_ipv4_numeric()`
- 比较两个 32 位 IP 地址的前 N 位？最复杂的部分，需要正确处理
- 没有路由或 TTL 为零时发送 ICMP 错误？不需要，丢弃即可

---

## 6. 提交

1. 只修改 `libsponge/` 下的 `.hh` 和 `.cc` 文件
2. 提交前运行：
   - `cmake --build build --target format`
   - `cmake --build build --target check6`
3. 在 `writeups/check6.md` 中撰写报告

---

## 检查单

| 分数 | 任务 |
|------|------|
| ? 分 | 路由表数据结构正确 |
| ? 分 | 最长前缀匹配逻辑正确 |
| ? 分 | TTL 处理正确 |
| ? 分 | 所有测试通过 |
| ? 分 | writeup 完整 |
