#include <aoc.h>

struct marker {
    size_t size{0};
    size_t count{0};
};
static inline marker parse_marker(std::string_view s_marker) {
    auto ret = marker{};
    if (!s_marker.empty()) {
        auto p = s_marker.data();
        while (p < s_marker.end() && *p >= '0' && *p <= '9')
            ret.size = ret.size * 10 + (*p++ - '0');
        p++;
        while (p < s_marker.end() && *p >= '0' && *p <= '9')
            ret.count = ret.count * 10 + (*p++ - '0');
    }
    return ret;
}

struct piece {
    marker m{};
    std::string_view prefix{};
    std::string_view data{};
    std::string_view postfix{};
};
static inline piece parse_piece(std::string_view s_piece) {
    auto ret = piece{};
    auto p = s_piece.find('(');
    if (p == s_piece.npos) {
        ret.prefix = s_piece;
        return ret;
    }
    ret.prefix = aoc::substr(s_piece, 0, p);
    p = s_piece.find(')', p + 1);
    if (p != s_piece.npos) {
        auto s_marker = aoc::substr(s_piece, ret.prefix.size() + 1, p - ret.prefix.size() - 1);
        ret.m = parse_marker(s_marker);
        ret.data = aoc::substr(s_piece, p + 1, ret.m.size);
        ret.postfix = aoc::substr(s_piece, p + ret.m.size + 1);
    }
    return ret;
}

template <bool recursive>
static inline size_t piece_length(piece p) {
    size_t ret{p.prefix.size()};
    if constexpr (recursive) {
        auto data = p.data;
        auto ds = size_t{0};
        while (!data.empty()) {
            auto sp = parse_piece(data);
            ds += piece_length<true>(sp);
            data = sp.postfix;
        }
        ret += p.m.count * ds;
    } else {
        ret += p.m.count * p.m.size;
    }
    return ret;
}

static inline size_t part1(std::string_view data) {
    size_t ret{0};
    while (!data.empty()) {
        auto p = parse_piece(data);
        ret += piece_length<false>(p);
        data = p.postfix;
    }
    return ret;
}

static inline size_t part2(std::string_view data) {
    size_t ret{0};
    while (!data.empty()) {
        auto p = parse_piece(data);
        ret += piece_length<true>(p);
        data = p.postfix;
    }
    return ret;
}

int main() {
    auto line = std::string{};
    line.reserve(16 * 1024);
    if (!std::getline(std::cin, line)) return 1;

    fmt::print("{}\n", part1(aoc::trim(line)));
    fmt::print("{}\n", part2(aoc::trim(line)));

    return 0;
}
