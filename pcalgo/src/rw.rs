use std::{io, str};

pub struct Scanner<R> {
    reader: R,
    buffer: Vec<String>,
}

impl<R: io::BufRead> Scanner<R> {
    pub fn new(reader: R) -> Self {
        Self {
            reader,
            buffer: Default::default(),
        }
    }

    pub fn token<T: str::FromStr>(&mut self) -> T {
        loop {
            if let Some(t) = self.buffer.pop() {
                return t.parse().ok().expect("Failed parse");
            }
            let mut line = String::new();
            self.reader.read_line(&mut line).expect("Failed read_line");
            self.buffer = line.split_whitespace().rev().map(String::from).collect();
        }
    }
}

pub fn stdio_scanner(stdin: &io::Stdin) -> Scanner<io::StdinLock> {
    Scanner::new(stdin.lock())
}

pub fn stdio_bufwriter(stdout: &io::Stdout) -> io::BufWriter<io::StdoutLock> {
    io::BufWriter::new(stdout.lock())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn do_add<R: io::BufRead, W: io::Write>(scan: &mut Scanner<R>, out: &mut W) {
        let x = scan.token::<i64>();
        let y = scan.token::<i64>();
        writeln!(out, "{} + {} = {}", x, y, x + y).unwrap();
    }

    #[test]
    fn test_in_memory() {
        let input: &[u8] = b"1 2";
        let mut scan = Scanner::new(input);
        let mut out = vec![];

        do_add(&mut scan, &mut out);
        assert_eq!(out, b"1 + 2 = 3\n");
    }
}
