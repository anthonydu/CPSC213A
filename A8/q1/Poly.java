package q1;

import static java.lang.System.out;

public class Poly {

  public class A {
    void ping() {
      out.printf("A.ping %d\n", this.i);
    }

    void pong() {
      out.printf("A.pong %d\n", this.i);
    }

    int i;

    public A(int i) {
      this.i = i;
    }
  }

  public class B extends A {
    void ping() {
      out.printf("B.ping %d\n", this.i);
    }

    void wiff() {
      out.printf("B.wiff %d\n", this.i);
    }

    public B(int i) {
      super(i);
    }
  }

  public class C extends B {
    int id;

    C(int i, int id) {
      super(i);
      this.id = id;
    }

    void ping() {
      out.printf("C.ping(%d,%d)\n", i, id);
    }
  }

  static void foo(A a) {
    a.ping();
    a.pong();
  }

  void bar() {
    foo(new A(10));
    foo(new B(20));
    foo(new C(30, 100));
  }

  public static void main(String args[]) {
    (new Poly()).bar();
  }
}
