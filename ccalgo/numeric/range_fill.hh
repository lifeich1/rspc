#pragma once

namespace A {
/// Fill integers [v_first, v_first+(d_last-d_first)) into range [d_first, d_last)
template <class T, class OutputIt>
void range_fill(OutputIt d_first, OutputIt d_last, T v_first) {
    for (; d_first != d_last; *d_first++ = v_first++);
}

/// Fill integers [v_first, v_first+n) into range [d_first, d_first+n)
template <class T, class OutputIt, class Size>
void range_fill_n(OutputIt d_first, Size n, T v_first) {
    for (Size i = 0; i++ < n; *d_first++ = v_first++);
}
}
