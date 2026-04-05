# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

This is a CMake-based C++ project. All builds happen in a `build/` subdirectory.

```bash
# Initial setup (once)
mkdir -p build && cd build && cmake ..

# Build (from build/)
make -j$(nproc)

# Run a specific lab's tests
make check_lab0
make check_lab1
# ... through lab7

# Other targets
make help        # see all available targets
make tidy        # clang-tidy linting
make format      # clang-format code formatting
make cppcheck    # static analysis
make doc         # generate documentation (doxygen)
```

Build types: Release (default), Debug (`cmake .. -DCMAKE_BUILD_TYPE=Debug`), RelASan, RelTSan, DebugASan, DebugTSan.

## Project Structure

This is the **Stanford CS144** network protocol stack implementation ("sponge"). The project builds a TCP/IP stack from scratch.

```
libsponge/              # Core implementation (what you write)
  byte_stream.hh/cc     # Flow-controlled in-memory byte stream (Lab 1)
  stream_reassembler.hh/cc  # Reassembles out-of-order segments (Lab 1)
  tcp_receiver.hh/cc    # TCP receiver state machine (Lab 2)
  tcp_sender.hh/cc      # TCP sender with retransmission (Lab 3)
  tcp_connection.hh/cc  # Full TCP connection combining sender+receiver (Lab 4)
  network_interface.hh/cc  # ARP and Ethernet (Lab 5)
  router.hh/cc          # IP routing table and forwarding (Lab 6)
  wrapping_integers.hh/cc  # 32-bit sequence number arithmetic
  tcp_helpers/          # Low-level TCP/IP primitives (provided)
    tcp_segment.hh/cc  # TCP segment parsing/serialization
    ipv4_datagram.hh/cc # IP datagram handling
    ethernet_frame.hh/cc # Ethernet frame handling
  util/                 # OS abstractions: sockets, TUN, event loop

tests/                  # Test suites per lab (fsm_* tests for TCP state machines)

apps/                   # Applications using the stack (webget, etc.)
```

## Lab Architecture

Labs 1-4 implement the TCP protocol stack in layers:
1. **ByteStream** - fixed-capacity queue, tracks read/write position
2. **StreamReassembler** - assembles unordered substrings using a sliding window with bitmap
3. **TCPReceiver** - tracks ACK seqno, generates gap-ack feedback
4. **TCPSender** - sends segments, tracks RTO retransmission timer, window flow control
5. **TCPConnection** - orchestrates sender/receiver, handles connection lifecycle

Labs 5-6 implement the network layer:
5. **NetworkInterface** - ARP cache, Ethernet framing, maps IP→MAC
6. **Router** - longest-prefix-match routing table, IP forwarding

Lab 7 integrates everything into a full stack that can communicate over TUN devices.

## Key Interfaces

- `ByteStream` has `write()`, `read()`, `peek()`, `pop()`; tracks `bytes_written`, `bytes_read`, `buffer_empty()`
- `StreamReassembler` has `push_substring()`, `ack_index()`, `bytes_pending()`; capacity limits unassembled bytes
- `TCPSender` uses `_next_seqno`, tracks `receiver_windowsize` (advertised) and `right_edge` independently
- `TCPConnection::end_input_stream()` triggers FIN exchange, then calls `drain()` to cleanup
