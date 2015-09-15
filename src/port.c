typedef unsigned char byte;

int main() {
  int fn, arg, res;
  byte buf[100];

  // We just read commands from standard input as long as we can...
  while (read_cmd(buf) > 0) {
    // Once we've gotten a command, we know the function is determined by the
    // first byte and its argument is the second byte
    fn = buf[0];
    arg = buf[1];

    if (fn == 1) {
      // We said that 1 was foo
      res = foo(arg);
    } else if (fn == 2) {
      // and 2 was bar
      res = bar(arg);
    }

    // We set the response value in the first byte of our buffer
    buf[0] = res;
    // and we write that byte back to the elixir program over standard out
    write_cmd(buf, 1);
  }
}
