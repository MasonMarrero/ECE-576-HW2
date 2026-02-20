#include <systemc.h>


SC_MODULE (hello_world) {
  SC_CTOR (hello_world) {
    SC_THREAD (main_thread);
  }

  void main_thread() {
    cout << "Hello World.\n";
  }
};

int sc_main (int argc, char* argv[]) {
  hello_world hello("HELLO");
  sc_start();
  return 0;
}