/* Minimal driver: load the three tables from <dir>, run the cooling
   calculation, and write the cooling curve to <out>. */
#include <iostream>
#include <string>
#include <cstdlib>
#include "nscool_wrap.h"

int main(int argc, char *argv[]) {
  std::string dir = (argc > 1) ? argv[1] : ".";
  std::string out = (argc > 2) ? argv[2] : "cool_curve.o2";
  double eta_in = (argc > 3) ? atof(argv[3]) : 0.0;

  nscool_wrap nw(dir);
  nscool_wrap_ptrs.resize(1);
  nscool_wrap_ptrs[0] = &nw;
  nw.main_out_it = 1000000;          // suppress per-step printing
  nw.eta = eta_in;                   // 0 = iron envelope, ~1 = max light-element

  int ret = nw.run(0);
  size_t n = nw.v_time.size();
  if (n == 0) {
    std::cout << "FAIL ret=" << ret << " no points" << std::endl;
    return 1;
  }
  nw.write_cool_curve(out);
  std::cout << "OK n=" << n
            << " t_last=" << nw.v_time[n-1]
            << " T_last=" << nw.v_tptr[n-1] << std::endl;
  return 0;
}
