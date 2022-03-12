pub fn swap<T: Copy + std::ops::BitXorAssign>(a: &mut [T], x: usize, y: usize) {
    a[x] ^= a[y];
    a[y] ^= a[x];
    a[x] ^= a[y];
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn i64_array_swap_elem() {
        let mut a: [i64; 3] = [1, 2, 3];
        swap(&mut a, 0, 2);
        assert_eq!(a, [3, 2, 1]);
    }

    #[test]
    fn usize_array_swap_elem() {
        let mut a: [usize; 3] = [1, 2, 3];
        swap(&mut a, 0, 2);
        assert_eq!(a, [3, 2, 1]);
    }
}
