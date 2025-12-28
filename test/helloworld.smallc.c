int main() {
  puts("Hello from SmallC!\n");
  putc('>');
  int c = getc();
  putc(c);
  putc('\n');
  return 0;
}
