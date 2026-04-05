# CS144: 计算机网络导论

2025 年秋季

## 实验检查点 0：网络热身

截止日期：9月28日 周日 晚上 11:59

---

欢迎来到 CS144：计算机网络导论。在这个热身实验中，你将：

1. 在电脑上安装 GNU/Linux 系统
2. 学会手动完成一些互联网任务
3. 用 C++ 编写一个从互联网获取网页的小程序
4. 实现网络中的一个关键抽象：在写入器和读取器之间建立可靠的字节流

预计完成时间：**2-6 小时**（后续检查点需要更多时间）。

### 三个快速提示

- 建议先完整阅读整个文档再动手！
- 在这 8 个实验的过程中，你将逐步构建出自己的互联网实现——包括路由器、网络接口和 TCP 协议（将不可靠的数据报转换为可靠的字节流）。大多数实验会基于之前的工作，因此很难"跳过"某个检查点。
- 如果你不满足 CS144 的先修条件，请先不要选这门课。前两个检查点可以作为测试：如果感到不适应，请考虑明年再选。

### 文档性质说明

这些实验文档不是"规格说明书"——它们不是单向传递的。它们更接近于软件工程师从老板或客户那里获得的详细程度。我们希望你能从实验课程中受益，如果在文档中发现任何模糊之处，请随时提问。

---

## 0. 协作策略

编程作业必须是**你个人的工作**：你必须自己编写所有提交的代码（除了我们提供的代码）。请勿从 Stack Overflow、GitHub、ChatGPT 或其他来源复制粘贴代码。如果参考了网络上的示例，请在提交的代码中注明 URL。

**与他人合作**：你不得向任何人展示你的代码，不得查看他人的代码（无论是否由人类或 AI 编写），不得查看历年的解决方案。你可以与其他学生讨论作业，但不得复制任何人的代码。如果你与他人讨论了作业，请在提交代码中注明他们的名字。

> 注意：GitHub Copilot 或 ChatGPT 等服务被视为"上过 CS144 的往年学生"。

**EdStem**：欢迎在 EdStem 上提问，但请勿发布任何源代码。

---

## 1. 在电脑上安装 GNU/Linux

CS144 的作业需要 GNU/Linux 操作系统和支持 C++ 2023 标准的编译器。请选择以下方式之一：

1. **推荐**：安装 CS144 VirtualBox 虚拟机镜像
2. 使用 Google Cloud 虚拟机
3. 运行 Ubuntu 25.04
4. 使用其他 GNU/Linux 发行版（风险自担，你的代码将在 Ubuntu 25.04 LTS 上测试）
5. 如果你有 2020-2024 年的 MacBook（M 系列芯片），请安装 UTM 虚拟机软件

## 2. 安装所需软件包

```bash
sudo apt update && sudo apt install git cmake gdb build-essential clang \
clang-tidy clang-format gcc-doc pkg-config glibc-doc tcpdump tshark
```

## 3. 手动使用网络

接下来你将手动完成两个任务：获取网页和发送电子邮件。这两个任务都依赖于一个叫做**可靠双向字节流**的网络抽象。

### 3.1 获取网页

1. 在浏览器中访问 http://cs144.keithw.org/hello
2. 用 telnet 手动完成同样的操作：
   ```bash
   telnet cs144.keithw.org http
   ```
   然后依次输入：
   ```
   GET /hello HTTP/1.1
   Host: cs144.keithw.org
   Connection: close

   ```
3. **作业**：用同样方法获取 http://cs144.keithw.org/lab0/sunetid（将 sunetid 替换为你的 SUNet ID），你会收到一个密钥，保存好用于 writeup。

### 3.2 发送电子邮件

1. SSH 到 cardinal.stanford.edu，然后：
   ```bash
   telnet 67.231.149.169 smtp
   ```
2. 依次输入：
   - `HELO mycomputer.stanford.edu`
   - `MAIL FROM: sunetid@stanford.edu`
   - `RCPT TO: sunetid@stanford.edu`
   - `DATA` 然后编写邮件内容，以`.`单独一行结束
   - `QUIT` 退出

### 3.3 监听和连接

1. 终端1运行：`netcat -v -l -p 9090`
2. 终端2运行：`telnet localhost 9090`
3. 可以在任意窗口打字，信息会显示在另一个窗口

---

## 4. 使用操作系统流套接字编写网络程序

### 4.1 创建代码仓库

```bash
git clone https://github.com/cs144/minnow
cd minnow
# 在 GitHub 创建同名私有仓库
git remote add github https://github.com/username/minnow
git push github
```

### 4.2 编译起始代码

```bash
cmake -S . -B build
cmake --build build
```

### 4.3 现代 C++ 风格

CS144 使用 contemporary C++ 风格，主要原则：

- 使用 **RAII**（资源获取即初始化）管理模式
- **不要**使用 malloc/free 或 new/delete
- **不要**使用原始指针，使用智能指针（unique_ptr/shared_ptr）
- **不要**使用 C 风格字符串（char *str）或字符串函数（strlen/strcpy）
- 优先使用 **const 引用**传递参数
- 每个变量尽量为 const
- 每个方法尽量为 const
- 避免全局变量

### 4.4 阅读 Minnow 支持代码

请阅读 `util/socket.hh` 和 `util/file_descriptor.hh` 的公共接口。

### 4.5 编写 webget

编辑 `apps/webget.cc`，实现 `get_URL` 函数：

**提示**：
- HTTP 每行必须以 `\r\n` 结尾
- 不要忘记包含 `Connection: close`
- 确保读取服务器的所有输出直到 EOF
- 预计需要约 8 行代码

**测试**：
```bash
./apps/webget cs144.keithw.org /hello
cmake --build build --target check_webget
```

---

## 5. 内存中的可靠字节流

实现一个内存中的字节流抽象，具有以下接口：

### 写入端接口

```cpp
void push(std::string data);      // 尽可能多地推送数据到流中
void close();                      // 关闭流
bool is_closed() const;            // 流是否已关闭
uint64_t available_capacity() const;  // 当前可推送的字节数
uint64_t bytes_pushed() const;     // 累计推送的字节数
```

### 读取端接口

```cpp
std::string_view peek() const;     // 查看下一个字节
void pop(uint64_t len);           // 从缓冲区移除指定字节
bool is_finished() const;         // 流是否已结束
bool has_error() const;           // 是否出错
uint64_t bytes_buffered() const;  // 当前缓冲的字节数
uint64_t bytes_popped() const;    // 累计读取的字节数
```

**关键特性**：
- 容量限制：流最多在内存中存储指定数量的字节
- 即使容量很小（比如 1 字节），流本身可以非常长（最多 2^64 字节）
- 容量限制的是"已写入但未读取"的字节数，而非流的总长度

**测试**：
```bash
cmake --build build --target check0
```

性能要求：速度超过 0.1 Gbit/s 即可（实际可能超过 10 Gbit/s）。

---

## 6. 提交

1. **只修改** `apps/webget.cc` 和 `libsponge/` 目录下的源文件
2. 经常提交代码并推送到 GitHub
3. 提交前依次运行：
   - `git status` 确认无未提交更改
   - `cmake --build build --target format` 格式化代码
   - `cmake --build build --target check0` 运行自动化测试
   - `cmake --build build --target tidy`（可选）检查代码建议
4. 完成 `writeups/check0.md`，填写花费时间和评论
5. 确保代码已提交并推送到 GitHub
6. 在 Gradescope 提交你的 commit ID

---

## 检查单（供 TA 评分）

| 分数 | 任务 | 完成 |
|------|------|------|
| 3 分 | webget.cpp 正确编译运行，获取 `http://cs144.keithw.org/hello` | ☐ |
| 3 分 | ares.hh / ares.cc 正确解析任意域名 | ☐ |
| 2 分 | writeup 包含代码工作原理的书面说明 | ☐ |

**展示准备**：
1. 代码编译 + 测试输出 + webget 输出
2. 配置检查：确认目录包含 `.clang-format` 等配置文件
