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

std::vector<float> nleft_oo_5p36_raw =
  {//0.5,   1.5,     2.5,   3.5,     4.5,     7.5,     12.5,    17.5,    22.5,    27.5,    32.5,    37.5,    42.5,    47.5,    52.5,    57.5,    62.5,    67.5,    72.5,    77.5,    82.5,    87.5
   // 153.834, 138.624, 130.9, 125.012, 120.055, 108.179, 92.0628, 78.6803, 67.0746, 56.9938, 48.2659, 40.6549, 33.9441, 28.1407, 23.0904, 18.7436, 15.1051, 11.9863, 9.31269, 7.02533, 4.99804, 3.26491 };
   153.834, 138.624, 130.9, 125.012, 120.055, 108.179, 92.0628, 78.6803, 67.0746, 56.9938, 48.2659, 40.6549, 33.9441, 28.1407, 23.0904, 18.7436, 0,    0,    0,    0,    0,    0 };
std::vector<float> nleft_oo_5p36_rawerr =
  {
   9.26886, 8.45095, 8.06591, 7.73995, 7.47824, 6.81874, 5.90056, 5.15352, 4.51879, 3.96379, 3.4676, 3.01573, 2.61673, 2.26394, 1.96877, 1.70386, 1.47981, 1.28129, 1.0976, 0.92452, 0.751261, 0.581132 };
std::vector<float> nleft_oo_5p36_cent =
  {//0.5, 1.5, 2.5,  3.5,  4.5,  7.5,  12.5, 17.5, 22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5
   99.5, 98.5, 97.5, 96.5, 95.5, 92.5, 87.5, 82.5, 77.5, 72.5, 67.5, 62.5, 57.5, 52.5, 47.5, 42.5, 37.5, 32.5, 27.5, 22.5, 17.5, 12.5 };
std::vector<float> nleft_oo_5p36_npart =
  {//0.5,   1.5,     2.5,     3.5,     4.5,     7.5,     12.5,    17.5,    22.5,    27.5,    32.5,    37.5,    42.5,    47.5,   52.5,   57.5,   62.5, 67.5, 72.5, 77.5, 82.5, 87.5
   29.3181, 28.3132, 27.6359, 27.0542, 26.5150, 24.9947, 22.5608, 20.1950, 17.9327, 15.8293, 13.9037, 12.1538, 10.5752, 9.1793, 7.9305, 6.8005, 0,    0,    0,    0,    0,    0 };
std::vector<float> nleft_oo_5p36_nparterr =
  {//0.5,   1.5,     2.5,     3.5,     4.5,     7.5,     12.5,    17.5,    22.5,    27.5,    32.5,    37.5,    42.5,    47.5,   52.5,   57.5,   62.5, 67.5, 72.5, 77.5, 82.5, 87.5
   0.3407,  0.3064,  0.2802,  0.2566,  0.2366,  0.1905,  0.1476,  0.1271,  0.1154,  0.1172,  0.1074,  0.1011,  0.0970,  0.0985, 0.0963, 0.0886, 0,    0,    0,    0,    0,    0 };
std::vector<float> nleft_oo_5p36_ncoll = nleft_oo_5p36_npart;
std::vector<float> nleft_oo_5p36_ncollerr = nleft_oo_5p36_nparterr;
auto nleft_oo_5p36_2a = 32.;
auto nleft_oo_5p36_size = nleft_oo_5p36_raw.size();

std::vector<float> pgcm_oo_5p36_raw =
  {//0.5,   1.5,     2.5,     3.5,     4.5,     7.5,     12.5,    17.5,    22.5,    27.5,   32.5,    37.5,    42.5,    47.5,    52.5,    57.5,    62.5,    67.5,    72.5,    77.5,   82.5,    87.5
   // 152.736, 137.878, 130.291, 124.625, 119.721, 107.896, 91.4613, 77.7573, 65.9914, 55.816, 46.9526, 39.3082, 32.7359, 27.0592, 22.2047, 18.1067, 14.6379, 11.6953, 9.17214, 6.9983, 5.06722, 3.34409 };
   152.736, 137.878, 130.291, 124.625, 119.721, 107.896, 91.4613, 77.7573, 65.9914, 55.816, 46.9526, 39.3082, 32.7359, 27.0592, 22.2047, 18.1067, 0,    0,    0,    0,    0,    0 };
std::vector<float> pgcm_oo_5p36_rawerr =
  {
   9.65182, 8.76726, 8.33419, 8.03427, 7.74305, 7.06907, 6.10891, 5.28382, 4.56653, 3.95519, 3.43849, 2.98583, 2.59146, 2.23803, 1.93033, 1.67354, 1.45693, 1.26889, 1.09027, 0.92399, 0.770138, 0.594804 };
std::vector<float> pgcm_oo_5p36_cent =
  {//0.5, 1.5, 2.5,  3.5,  4.5,  7.5,  12.5, 17.5, 22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5
   99.5, 98.5, 97.5, 96.5, 95.5, 92.5, 87.5, 82.5, 77.5, 72.5, 67.5, 62.5, 57.5, 52.5, 47.5, 42.5, 37.5, 32.5, 27.5, 22.5, 17.5, 12.5 };
std::vector<float> pgcm_oo_5p36_npart =
  {//0.5,   1.5,     2.5,     3.5,     4.5,     7.5,     12.5,    17.5,    22.5,    27.5,    32.5,    37.5,    42.5,    47.5,   52.5,   57.5,   62.5, 67.5, 72.5, 77.5, 82.5, 87.5
   29.0364, 28.0797, 27.4362, 26.8793, 26.3508, 24.8209, 22.2889, 19.8464, 17.5675, 15.4445, 13.5005, 11.7487, 10.1807, 8.7911, 7.5546, 6.4615, 0,    0,    0,    0,    0,    0 };
std::vector<float> pgcm_oo_5p36_nparterr =
  {//0.5,   1.5,     2.5,     3.5,     4.5,     7.5,     12.5,    17.5,    22.5,    27.5,    32.5,    37.5,    42.5,    47.5,   52.5,   57.5,   62.5, 67.5, 72.5, 77.5, 82.5, 87.5
   0.3953,  0.3648,  0.3475,  0.3411,  0.3309,  0.3064,  0.2762,  0.2616,  0.2536,  0.2351,  0.2134,  0.1935,  0.1717,  0.1488, 0.1242, 0.1089, 0,    0,    0,    0,    0,    0 };
std::vector<float> pgcm_oo_5p36_ncoll = pgcm_oo_5p36_npart;
std::vector<float> pgcm_oo_5p36_ncollerr = pgcm_oo_5p36_nparterr;
auto pgcm_oo_5p36_2a = 32.;
auto pgcm_oo_5p36_size = pgcm_oo_5p36_raw.size();

GENERATE(hijing_oo_5p36)
GENERATE(amptnm_oo_5p36)
GENERATE(amptsm_oo_5p36)
GENERATE(hydjet_oo_5p36)
GENERATE(angantyr_oo_5p36)
GENERATE(epos_oo_5p36)
GENERATE(nleft_oo_5p36)
GENERATE(pgcm_oo_5p36)

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
  auto* gpgcm_oo_5p36##OBS = pgcm_oo_5p36##OBS();                         \
  auto* gnleft_oo_5p36##OBS = nleft_oo_5p36##OBS();                       \
  cleangrzero(gnleft_oo_5p36##OBS); \
  cleangrzero(gpgcm_oo_5p36##OBS); \
  xjjroot::setlinestyle(ghijing_oo_5p36##OBS, get_mc_color("hijing"), get_mc_line_style("hijing"), 5); \
  xjjroot::setlinestyle(gangantyr_oo_5p36##OBS, get_mc_color("angantyr"), get_mc_line_style("angantyr"), 5); \
  xjjroot::setlinestyle(gamptnm_oo_5p36##OBS, get_mc_color("amptnm"), get_mc_line_style("amptnm"), 5); \
  xjjroot::setlinestyle(gamptsm_oo_5p36##OBS, get_mc_color("amptsm"), get_mc_line_style("amptsm"), 5); \
  xjjroot::setlinestyle(ghydjet_oo_5p36##OBS, get_mc_color("hydjet"), get_mc_line_style("hydjet"), 5); \
  xjjroot::setlinestyle(gepos_oo_5p36##OBS, get_mc_color("epos"), get_mc_line_style("epos"), 4); \
  xjjroot::setthgrstyle(gnleft_oo_5p36##OBS, -1, -1, -1, get_mc_color("nleft"), 1, 3, get_mc_color("nleft"), 0.7, 3354); \
  xjjroot::setthgrstyle(gpgcm_oo_5p36##OBS, -1, -1, -1, get_mc_color("pgcm"), 9, 3, get_mc_color("pgcm"), 0.7, 3345); \

#define DRAW_MC(OBS)                            \
  ghijing_oo_5p36##OBS->Draw("c same");         \
  gangantyr_oo_5p36##OBS->Draw("c same");       \
  gamptnm_oo_5p36##OBS->Draw("c same");         \
  gamptsm_oo_5p36##OBS->Draw("c same");         \
  gepos_oo_5p36##OBS->Draw("c same");
  // ghydjet_oo_5p36##OBS->Draw("c same");

