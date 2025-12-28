int print_int(int x) {
  if (x == 0) { putc('0'); return 0; }

  int buf[12];
  int i = 0;

  while (x > 0) {
    int d = x % 10;
    buf[i] = d;
    i = i + 1;
    x = x / 10;
  }

  while (i > 0) {
    i = i - 1;
    putc(buf[i] + '0');
  }
  return 0;
}

int main() {
  puts("Enter n (0-9): ");
  int ch = getc();
  putc(ch);
  putc('\n');

  int n = ch - '0';
  int i = 1;
  int f = 1;

  while (i <= n) {
    f = f * i;
    i = i + 1;
  }

  puts("fact = ");
  print_int(f);
  putc('\n');
  return 0;
}
