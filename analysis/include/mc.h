#ifndef GENERATE
#include "measurements.h"
#endif
#include "tool.h"

std::vector<float> epos_oo_5p36_raw = {
                                       10.0362, 12.5303, 15.2127, 17.9155, 21.3596, 25.4345, 29.9818, 35.5019, 41.5055, 48.5471, 56.8699, 65.7771, 78.0588, 97.9225};
std::vector<float> epos_oo_5p36_rawerr = {
                                          0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0};
std::vector<float> epos_oo_5p36_cent = {
                                        32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5,
                                        67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> epos_oo_5p36_npart = {
                                         4.56843, 5.21041, 5.92908, 6.80908, 7.82537, 9.01757, 10.2711, 11.8192, 13.4388, 15.3111, 17.345, 19.2236, 21.3056, 23.8995};
std::vector<float> epos_oo_5p36_nparterr = {
                                            0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0};
std::vector<float> epos_oo_5p36_ncoll = epos_oo_5p36_npart;
std::vector<float> epos_oo_5p36_ncollerr = epos_oo_5p36_nparterr;
auto epos_oo_5p36_2a = 32.;
auto epos_oo_5p36_size = epos_oo_5p36_raw.size();

std::vector<float> amptnm_oo_5p36_raw = {
                                         11.1289, 13.9996, 17.6464, 22.0299, 27.1523, 33.4231, 40.5998, 49.4082, 59.6049, 71.7224, 85.7052, 103.352, 124.568, 159.705};
std::vector<float> amptnm_oo_5p36_rawerr = {
                                            0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0};
std::vector<float> amptnm_oo_5p36_cent = {
                                          32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5,
                                          67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> amptnm_oo_5p36_npart = {
                                           4.5136, 5.3542, 6.30354, 7.3218, 8.59888, 10.0214, 11.5102, 13.4401, 15.3443, 17.4834, 19.8048, 22.1114, 24.5711, 27.1406};
std::vector<float> amptnm_oo_5p36_nparterr = {
                                              0, 0, 0, 0, 0, 0, 0,
                                              0, 0, 0, 0, 0, 0, 0};
std::vector<float> amptnm_oo_5p36_ncoll = amptnm_oo_5p36_npart;
std::vector<float> amptnm_oo_5p36_ncollerr = amptnm_oo_5p36_nparterr;
auto amptnm_oo_5p36_2a = 32.;
auto amptnm_oo_5p36_size = amptnm_oo_5p36_raw.size();

std::vector<float> amptsm_oo_5p36_raw = {
                                         11.21, 14.1163, 17.7665, 22.1338, 27.2149, 33.4223, 40.859, 49.5736, 59.9497, 72.3531, 87.1113, 104.19, 125.368, 161.051};
std::vector<float> amptsm_oo_5p36_rawerr = {
                                            0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0};
std::vector<float> amptsm_oo_5p36_cent = {
                                          32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5,
                                          67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> amptsm_oo_5p36_npart = {
                                           4.51139, 5.34633, 6.29246, 7.36586, 8.55026, 9.96877, 11.504, 13.317, 15.235, 17.3791, 19.7095, 22.0825, 24.5159, 27.0753};
std::vector<float> amptsm_oo_5p36_nparterr = {
                                              0, 0, 0, 0, 0, 0, 0,
                                              0, 0, 0, 0, 0, 0, 0};
std::vector<float> amptsm_oo_5p36_ncoll = amptsm_oo_5p36_npart;
std::vector<float> amptsm_oo_5p36_ncollerr = amptsm_oo_5p36_nparterr;
auto amptsm_oo_5p36_2a = 32.;
auto amptsm_oo_5p36_size = amptsm_oo_5p36_raw.size();

std::vector<float> hijing_oo_5p36_raw = {
                                         10.3158, 13.3047, 17.0661, 21.7507, 27.567, 34.7142, 43.8391, 55.0784, 68.9885, 86.1451, 107.2, 132.9, 166.264, 223.046};
std::vector<float> hijing_oo_5p36_rawerr = {
                                            0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0};
std::vector<float> hijing_oo_5p36_cent = {
                                          32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5,
                                          67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> hijing_oo_5p36_npart = {
                                           4.4888, 5.33419, 6.24125, 7.27632, 8.43922, 9.82772, 11.3853, 13.1801, 15.1836, 17.3292, 19.6914, 22.132, 24.5213, 27.1376};
std::vector<float> hijing_oo_5p36_nparterr = {
                                              0, 0, 0, 0, 0, 0, 0,
                                              0, 0, 0, 0, 0, 0, 0};
std::vector<float> hijing_oo_5p36_ncoll = hijing_oo_5p36_npart;
std::vector<float> hijing_oo_5p36_ncollerr = hijing_oo_5p36_nparterr;
auto hijing_oo_5p36_2a = 32.;
auto hijing_oo_5p36_size = hijing_oo_5p36_raw.size();

std::vector<float> hydjet_oo_5p36_raw = {
                                         11.9149, 13.9732, 16.6711, 21.5904, 24.1539, 30.6839, 36.2954, 39.6758, 47.3319, 54.6655, 63.2525, 84.8891, 91.9846, 120.949};
std::vector<float> hydjet_oo_5p36_rawerr = {
                                            0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0};
std::vector<float> hydjet_oo_5p36_cent = {
                                          32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5,
                                          67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> hydjet_oo_5p36_npart = {
                                           2.90269, 3.48732, 4.58222, 5.28764, 6.28701, 7.44619, 8.24798, 8.99663, 10.9802, 13.8042, 15.9331, 18.6772, 21.3591, 22.9117};
std::vector<float> hydjet_oo_5p36_nparterr = {
                                              0, 0, 0, 0, 0, 0, 0,
                                              0, 0, 0, 0, 0, 0, 0};
std::vector<float> hydjet_oo_5p36_ncoll = hydjet_oo_5p36_npart;
std::vector<float> hydjet_oo_5p36_ncollerr = hydjet_oo_5p36_nparterr;
auto hydjet_oo_5p36_2a = 32.;
auto hydjet_oo_5p36_size = hydjet_oo_5p36_raw.size();

std::vector<float> angantyr_oo_5p36_raw = {
                                           25.7382, 30.9674, 37.026, 43.4097, 50.5162, 58.1661, 66.3364, 75.3796, 84.8683, 95.199, 106.236, 118.681, 132.517, 154.649};
std::vector<float> angantyr_oo_5p36_rawerr = {
                                              0, 0, 0, 0, 0, 0, 0,
                                              0, 0, 0, 0, 0, 0, 0};
std::vector<float> angantyr_oo_5p36_cent = {
                                            32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5,
                                            67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> angantyr_oo_5p36_npart = {
                                             9.40831, 10.9145, 12.4494, 13.987, 15.6195, 17.1962, 18.8255, 20.4012, 21.9408, 23.451, 24.915, 26.2775, 27.4813, 28.7514};
std::vector<float> angantyr_oo_5p36_nparterr = {
                                                0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0};
std::vector<float> angantyr_oo_5p36_ncoll = angantyr_oo_5p36_npart;
std::vector<float> angantyr_oo_5p36_ncollerr = angantyr_oo_5p36_nparterr;
auto angantyr_oo_5p36_2a = 32.;
auto angantyr_oo_5p36_size = angantyr_oo_5p36_raw.size();

std::vector<float> nleft_oo_6p8_raw = {
   170.402, 154.196, 145.891, 139.613, 134.288, 129.583, 125.259, 121.209, 117.384, 113.749, 103.754, 88.9027, 75.9675, 64.6397, 54.7359, 46.0614, 38.4848, 31.9369, 26.2742};
std::vector<float> nleft_oo_6p8_rawerr = {
   13.9617, 11.8379, 10.9571, 10.3878, 9.91606, 9.5301, 9.20657, 8.89295, 8.61532, 8.35227, 7.65961, 6.6449, 5.75939, 4.99502, 4.33577, 3.74253, 3.21058, 2.76594, 2.36722};
std::vector<float> nleft_oo_6p8_cent = {
   99.5, 98.5, 97.5, 96.5, 95.5, 94.5, 93.5, 92.5, 91.5, 90.5, 87.5, 82.5, 77.5, 72.5, 67.5, 62.5, 57.5, 52.5, 47.5};
std::vector<float> nleft_oo_6p8_npart = {
   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
std::vector<float> nleft_oo_6p8_nparterr = {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
std::vector<float> nleft_oo_6p8_ncoll = nleft_oo_6p8_npart;
std::vector<float> nleft_oo_6p8_ncollerr = nleft_oo_6p8_nparterr;
auto nleft_oo_6p8_2a = 32.;
auto nleft_oo_6p8_size = nleft_oo_6p8_raw.size();

std::vector<float> pgcm_oo_6p8_raw = {
   169.103, 153.372, 145.267, 139.077, 133.807, 129.092, 124.728, 120.631, 116.751, 113.019, 102.761, 87.5014, 74.3336, 62.8953, 52.9574, 44.3523, 36.9097, 30.5249, 25.069, };
std::vector<float> pgcm_oo_6p8_rawerr = {
   14.3372, 12.1747, 11.2659, 10.676, 10.1961, 9.80706, 9.44681, 9.1293, 8.84523, 8.57576, 7.82087, 6.73974, 5.80674, 5.01503, 4.31134, 3.70975, 3.18325, 2.72384, 2.31959, };
std::vector<float> pgcm_oo_6p8_cent = {
   99.5, 98.5, 97.5, 96.5, 95.5, 94.5, 93.5, 92.5, 91.5, 90.5, 87.5, 82.5, 77.5, 72.5, 67.5, 62.5, 57.5, 52.5, 47.5, };
std::vector<float> pgcm_oo_6p8_npart = {
   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, };
std::vector<float> pgcm_oo_6p8_nparterr = {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
std::vector<float> pgcm_oo_6p8_ncoll = pgcm_oo_6p8_npart;
std::vector<float> pgcm_oo_6p8_ncollerr = pgcm_oo_6p8_nparterr;
auto pgcm_oo_6p8_2a = 32.;
auto pgcm_oo_6p8_size = pgcm_oo_6p8_raw.size();

GENERATE(hijing_oo_5p36)
GENERATE(amptnm_oo_5p36)
GENERATE(amptsm_oo_5p36)
GENERATE(hydjet_oo_5p36)
GENERATE(angantyr_oo_5p36)
GENERATE(epos_oo_5p36)
GENERATE(nleft_oo_6p8)
GENERATE(pgcm_oo_6p8)

struct mccent {
  TGraphErrors* g;
  std::string tag;
};
std::vector<std::string> _tags_mc = { "hijing", "amptnm", "amptsm", "hydjet", "angantyr", "epos" };
std::vector<mccent> _m_mc;
void init_mc_cent() {
  constexpr int ntotal = NCENT - OFFSET;
  for (auto& tag : _tags_mc) {    
    auto gr = new TGraphErrors(ntotal); gr->SetName(Form("g%s", tag.c_str()));
    xjjana::grzero(gr);
    mccent m = { .g = gr, .tag = tag };
    _m_mc.push_back(m);
  }
}
void fill_mc_cent(mccent& m, TFile* f, int cindex) {
  auto gmc = (TGraphErrors*)f->Get(Form("gh1WGhadron-%s", m.tag.c_str()));
  float midy = gmc->Eval(0.);// gmc->GetPointY(ipoint);
  m.g->SetPoint(cindex, 0, midy);
  m.g->SetPointError(cindex, 0, 0);
}

void print_mc_cent(const mccent &m) {
  std::cout<<"std::vector<float> "<<m.tag<<"_oo_5p36_raw = {"<<std::endl;
  for (int i=0; i<m.g->GetN(); i++) {
    std::cout<<(i?", ":"   ")
             <<m.g->GetPointY(i);
  } std::cout<<"};"<<std::endl;;
}



#define MAKE_SET_MC(OBS)                                                \
  auto* ghijing_oo_5p36##OBS = hijing_oo_5p36##OBS();                   \
  auto* gamptnm_oo_5p36##OBS = amptnm_oo_5p36##OBS();                   \
  auto* gamptsm_oo_5p36##OBS = amptsm_oo_5p36##OBS();                   \
  auto* ghydjet_oo_5p36##OBS = hydjet_oo_5p36##OBS();                   \
  auto* gepos_oo_5p36##OBS = epos_oo_5p36##OBS();                       \
  auto* gangantyr_oo_5p36##OBS = angantyr_oo_5p36##OBS();               \
  auto* gpgcm_oo_6p8##OBS = pgcm_oo_6p8##OBS();                         \
  auto* gnleft_oo_6p8##OBS = nleft_oo_6p8##OBS();                       \
  xjjroot::setlinestyle(ghijing_oo_5p36##OBS, COLOUR10, get_mc_line_style("hijing"), 3); \
  xjjroot::setlinestyle(gangantyr_oo_5p36##OBS, COLOUR2, get_mc_line_style("angantyr"), 3); \
  xjjroot::setlinestyle(gamptnm_oo_5p36##OBS, COLOUR5, get_mc_line_style("amptnm"), 3); \
  xjjroot::setlinestyle(gamptsm_oo_5p36##OBS, COLOUR6, get_mc_line_style("amptsm"), 3); \
  xjjroot::setlinestyle(ghydjet_oo_5p36##OBS, COLOUR3, get_mc_line_style("hydjet"), 3); \
  xjjroot::setlinestyle(gepos_oo_5p36##OBS, COLOUR1, get_mc_line_style("epos"), 3); \
  xjjroot::setthgrstyle(gnleft_oo_6p8##OBS, -1, -1, -1, COLOUR9, 1, 3, COLOUR9, 0.5, 3345); \
  xjjroot::setthgrstyle(gpgcm_oo_6p8##OBS, -1, -1, -1, COLOUR7, 9, 3, COLOUR7, 0.5, 3354); \

#define DRAW_MC(OBS)                            \
  ghijing_oo_5p36##OBS->Draw("c same");         \
  gangantyr_oo_5p36##OBS->Draw("c same");       \
  gamptnm_oo_5p36##OBS->Draw("c same");         \
  gamptsm_oo_5p36##OBS->Draw("c same");         \
  gepos_oo_5p36##OBS->Draw("c same");
  // ghydjet_oo_5p36##OBS->Draw("c same");

