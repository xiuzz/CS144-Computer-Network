#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), _unassembled_substrings(capacity, '\0'), _bitmap(capacity, false) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof) {
        _eof = true;
    }
    size_t data_size = data.size();
    if (data_size == 0 && unassembled_bytes() == 0 && eof) {
        _output.end_input();
        return;
    }
    if (index >= _first_unassembled_index + _capacity) {
        return;
    }
    if (index >= _first_unassembled_index) {
        size_t offset{index - _first_unassembled_index};
        size_t real_len = min(data_size, _output.remaining_capacity() - offset);
        if (real_len < data_size) {
            _eof = false;
        }
        for (size_t i = 0; i < real_len; i++) {
            if (_bitmap[i+offset]) continue;
            _unassembled_substrings[i+offset] = data[i];
            _bitmap[i+offset] = true;
            _unassembled_bytes++;
        }
    } else if  (index + data_size > _first_unassembled_index) {
        int offset = _first_unassembled_index - index;
        size_t real_len = min(data_size - offset, _output.remaining_capacity());
        if (real_len < data_size - offset) {
            _eof = false;
        }
        for (size_t i = 0; i < real_len; i++) {
            if (_bitmap[i]) continue;
            _unassembled_substrings[i] = data[i+offset];
            _bitmap[i] = true;
            _unassembled_bytes++;
        }
    }
    check_contiguous();
    if (_eof && unassembled_bytes() == 0) {
        _output.end_input();
    }
}

void StreamReassembler::check_contiguous() {
    string tmp{};
    while (_bitmap.front()) {
        tmp += _unassembled_substrings.front();
        _unassembled_substrings.pop_front();
        _bitmap.pop_front();
        _unassembled_substrings.push_back('\0');
        _bitmap.push_back(false);
    }
    if (tmp.size() > 0) {
        _output.write(tmp);
        _first_unassembled_index += tmp.size();
        _unassembled_bytes -=  tmp.size();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return {}; }
