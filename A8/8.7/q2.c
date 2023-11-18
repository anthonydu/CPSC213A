int main() {}

int q2(int a, int b, int c) {
  switch (a) {
    case 10:
      return b + c;
    case 12:
      return b - c;
    case 14:
      return b > c;
    case 16:
      return c > b;
    case 18:
      return b == c;
    default:
      return 0;
  }
}