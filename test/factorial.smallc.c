/* Copyright 2005-2025 Varghese Mathew (Matt)
 *
 * This file is part of Coconut (TM).
 * Coconut is a
 *     Multi-threaded simulation of the pipeline of a MIPS-like
 *     Microprocessor (integer instructions only) replete with 
 *     Memory Subsystem, Caches and their performance analysis,
 *     I/O device modules and an assembler.
 * 
 * Coconut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Coconut is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Coconut.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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
