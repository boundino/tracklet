#include <TF1.h>

#ifndef VZ_FUNC
#define VZ_FUNC

TF1* truncated_gaussian(const char* fname,
                        double xmin = -15, double xmax = 15) {
  // par[0] = amplitude
  // par[1] = mean
  // par[2] = sigma
  // par[3] = truncation parameter (0-1)
  auto truncatedGaus = [](double* x, double* par) {
                         auto dx = (x[0] - par[1]) / par[2];
                         // auto mygaus = gaus();
                         auto gaus = par[0] * TMath::Gaus(x[0], par[1], par[2]) / (TMath::Sqrt(2*TMath::Pi()) * par[2]);
                         // Apply truncation - reduces tails
                         auto truncation = TMath::Exp(-par[3] * dx * dx * dx * dx); // quartic term
                         return gaus * truncation;
                       };

  auto* f = new TF1(fname, truncatedGaus, xmin, xmax, 4);
  f->SetParameters(1, 0, 5, 0.1);
  f->SetParLimits(3, 0, 1); // Truncation between 0 and 1

  return f;
}

#endif
