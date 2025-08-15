#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCut.h>

#include <vector>
#include <algorithm>    // std::sort
#include <string>

#include "include/xjjrootuti.h"
#include "include/xjjcuti.h"
#include "include/cfout.h"

#define NCENTBINS 200
const float upper_limit = 2.e+4;

int make_header(std::vector<float> vhibins, std::string filename,
                std::string inputname="", std::string treevar="", std::string var="") {

  if (vhibins.size() != NCENTBINS+1) {
    std::cout<<"error: wrong number of bins: "<<vhibins.size()<<std::endl;
    return 1;
  }
  auto outputh = filename.c_str();
  std::ofstream ofs(outputh);
  xjjc::cfout out(ofs, std::cout);

  std::cout<<"\e[30;3m"<<std::endl;

  out<<"// "<<inputname<<std::endl;
  out<<"// "<<treevar<<std::endl;
  out<<std::endl;

  out<<"const int nBins="<<NCENTBINS<<";"<<std::endl;
  out<<"const double binTable[nBins+1] = { 0., ";
  for (int i=1; i<=NCENTBINS; i++) {
    out<<vhibins[i];
    if (i<NCENTBINS) out<<", ";
    if (i%10==9) out<<std::endl<<"  ";
  }
  out<<"};"<<std::endl;

  out<<std::endl;
  
  out<<"static const float hf[21] = {"<<std::endl;
  out<<"  0., ";
  for (int i=1; i<=20; i++) {
    out<<vhibins[i*10];
    if (i<20) out<<", ";
    if (i%5==4) out<<std::endl<<"  ";
  }
  out<<std::endl<<"};"<<std::endl;

  out<<std::endl;
  
  out<<"int getHiBinFromhiHF(const double "<<var<<") { \n\
  int binPos = -1; \n\
  for (int i = 0; i < nBins; ++i) { \n\
    if ("<<var<<" >= binTable[i] && "<<var<<" < binTable[i+1]) { \n\
      binPos = i; \n\
      break; \n\
    } \n\
  } \n\
 \n\
  binPos = nBins - 1 - binPos; \n\
 \n\
  return (int)(200*((double)binPos)/((double)nBins)); \n\
}"<<std::endl;

  std::cout<<"\e[0m"<<std::endl;
  std::cout<<outputh<<std::endl;

  return 0;
}

int macro(std::string inputname, std::string outputname, std::string treevar="pixel/PixelTree::hft", int division=1)
// int macro(std::string inputname, std::string treevar="TrackletTree12::hft")
{
  auto vars = xjjc::str_divide(treevar, "::");
  auto t = (TTree*)TFile::Open(inputname.c_str())->Get(vars[0].c_str());
  auto var = vars[1];
  auto ismc = t->GetEntries("npart!=0");
  std::cout<<"  [ "<<(ismc?"MC":"data")<<" ]"<<std::endl;

  t->SetBranchStatus("*", 0);
  float hft; t->SetBranchStatus(var.c_str(), 1); t->SetBranchAddress(var.c_str(), &hft);
  int nhfp; t->SetBranchStatus("nhfp", 1); t->SetBranchAddress("nhfp", &nhfp);
  int nhfn; t->SetBranchStatus("nhfn", 1); t->SetBranchAddress("nhfn", &nhfn);
  int process; t->SetBranchStatus("process", 1); t->SetBranchAddress("process", &process);

  TH1D* hhft = new TH1D("hhft", ";HF Energy;", 1000, 0, 6.e+3);
  
  std::vector<float> vhft, vhibins(1, 0);
  auto nentries = t->GetEntries();
  for (int i=0; i<nentries; i+=division) {
    xjjc::progressslide(i, nentries, 100*division);
    t->GetEntry(i);
    if (!ismc && (nhfp <= 2 || nhfn <= 2 )) continue; // data
    if (ismc && (process == 102 || process == 103 || process == 104)) continue; // MC
    vhft.push_back(hft);
    hhft->Fill(hft);
  }
  xjjc::progressbar_summary(nentries);

  int n = vhft.size();
  // std::sort(vhft.begin(), vhft.end(), std::greater<float>());
  std::sort(vhft.begin(), vhft.end());

  for (int i=1; i<NCENTBINS; i++)
    vhibins.push_back(vhft[std::ceil(i/(NCENTBINS*1.)*n)-1]);
  vhibins.push_back(upper_limit);

  make_header(vhibins, Form("cent/cent_%s.h", outputname.c_str()),
              inputname, treevar, var);
  
  auto outf = new TFile(Form("../data/cent-%s.root", outputname.c_str()), "recreate");
  outf->cd();
  xjjroot::writehist(hhft);
  outf->Close();

  return 0;
}

std::vector<float> vhibins_default = {0,0.55,1.1,1.65,2.2,2.75,3.3,3.85,4.4,4.95,5.5,6.05,6.6,7.15,7.7,8.25,8.8,9.35,9.9,10.45,11,11.55,12.1,12.65,13.2,13.75,14.3,14.85,15.4,15.95,16.5,17.05,17.6,18.15,18.7,19.25,19.8,20.35,20.9,21.45,22,22.5761,23.1654,23.7619,24.3606,24.9804,25.6087,26.2441,26.8777,27.5234,28.182,28.8467,29.5063,30.1785,30.8715,31.5705,32.2808,33.0025,33.7232,34.4671,35.2137,35.9678,36.7286,37.5151,38.2974,39.091,39.9013,40.7127,41.5246,42.3431,43.1888,44.0411,44.9136,45.7779,46.6476,47.5315,48.4342,49.3512,50.2849,51.2271,52.178,53.1469,54.1325,55.1142,56.1127,57.1195,58.1569,59.2001,60.2706,61.3419,62.43,63.5325,64.657,65.776,66.9236,68.0984,69.2688,70.4616,71.6792,72.902,74.166,75.4323,76.7112,78.0024,79.3182,80.6521,82.005,83.3776,84.7627,86.1778,87.6199,89.0901,90.5849,92.0729,93.6042,95.1469,96.6985,98.2723,99.8689,101.511,103.141,104.801,106.494,108.202,109.943,111.693,113.44,115.231,117.062,118.953,120.903,122.801,124.729,126.705,128.675,130.703,132.759,134.853,136.951,139.096,141.272,143.463,145.676,147.93,150.265,152.612,154.949,157.377,159.808,162.244,164.728,167.246,169.791,172.377,175.022,177.724,180.44,183.196,185.963,188.815,191.697,194.604,197.596,200.627,203.697,206.821,210,213.241,216.535,219.861,223.222,226.695,230.193,233.778,237.413,241.096,244.842,248.661,252.597,256.69,260.858,265.119,269.494,273.994,278.663,283.501,288.466,293.636,299.078,304.819,310.863,317.284,324.285,331.783,339.98,349.201,359.795,372.651,389.193,414.668,upper_limit};

std::vector<float> vhibins_up = {0,0.583333,1.16667,1.75,2.33333,2.91667,3.5,4.08333,4.66667,5.25,5.83333,6.41667,7,7.58333,8.16667,8.75,9.33333,9.91667,10.5,11.0833,11.6667,12.25,12.8333,13.4167,14,14.5833,15.1667,15.75,16.3333,16.9167,17.5,18.0833,18.6667,19.25,19.8333,20.4167,21,21.5833,22.1667,22.75,23.3333,23.919,24.515,25.1265,25.7505,26.3745,27.0023,27.6384,28.2895,28.948,29.5971,30.2621,30.9475,31.635,32.3372,33.0489,33.7612,34.4941,35.2302,35.9738,36.7238,37.4985,38.2704,39.0523,39.8503,40.6508,41.4512,42.2548,43.087,43.9273,44.7843,45.6347,46.4931,47.3619,48.2466,49.149,50.0704,50.9936,51.9262,52.8821,53.8438,54.8169,55.7936,56.7817,57.7925,58.8239,59.8663,60.9245,61.987,63.0702,64.1654,65.2738,66.383,67.5382,68.693,69.8568,71.0421,72.25,73.4746,74.7188,75.9727,77.2391,78.5217,79.8256,81.1453,82.4874,83.8443,85.2176,86.6238,88.0529,89.5143,90.9775,92.4584,93.9726,95.4938,97.0261,98.5827,100.16,101.787,103.393,105.035,106.708,108.395,110.112,111.837,113.562,115.329,117.135,119.001,120.924,122.795,124.696,126.643,128.585,130.58,132.605,134.667,136.736,138.841,140.986,143.141,145.321,147.524,149.823,152.139,154.439,156.817,159.212,161.612,164.044,166.518,169.016,171.545,174.137,176.791,179.464,182.166,184.889,187.65,190.504,193.331,196.261,199.236,202.245,205.296,208.397,211.551,214.781,218.047,221.34,224.706,228.139,231.608,235.167,238.764,242.421,246.141,249.95,253.877,257.928,262.052,266.279,270.629,275.1,279.757,284.549,289.481,294.628,300.044,305.75,311.749,318.138,325.111,332.565,340.73,349.894,360.476,373.28,389.745,415.128,upper_limit};

std::vector<float> vhibins_down = {0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,15.5,16,16.5,17,17.5,18,18.5,19,19.5,20,20.56,21.1348,21.7134,22.2971,22.8925,23.503,24.1116,24.7369,25.3723,26.026,26.6685,27.3292,27.9994,28.6734,29.3496,30.0327,30.7372,31.4545,32.1772,32.914,33.6482,34.4089,35.172,35.9421,36.7194,37.5236,38.3229,39.1346,39.9638,40.7943,41.6223,42.4634,43.33,44.2018,45.0964,45.9826,46.8709,47.778,48.7087,49.6561,50.6069,51.5726,52.5593,53.5491,54.5632,55.5694,56.5921,57.6366,58.7052,59.783,60.8806,61.982,63.1055,64.2431,65.3911,66.5488,67.7478,68.9434,70.1573,71.3955,72.6458,73.931,75.2234,76.5294,77.8479,79.1889,80.5515,81.9332,83.3359,84.7517,86.1983,87.6736,89.179,90.7042,92.2307,93.7983,95.3757,96.9639,98.5773,100.215,101.901,103.569,105.277,107.014,108.771,110.554,112.342,114.145,115.992,117.89,119.85,121.82,123.775,125.77,127.789,129.83,131.923,134.046,136.195,138.359,140.584,142.813,145.073,147.348,149.731,152.134,154.519,156.989,159.473,161.963,164.495,167.067,169.666,172.308,175.011,177.774,180.55,183.369,186.199,189.126,192.07,195.058,198.125,201.232,204.376,207.577,210.842,214.178,217.558,220.969,224.448,228.007,231.603,235.296,239.027,242.825,246.693,250.665,254.771,258.993,263.301,267.716,272.262,276.949,281.861,286.874,292.083,297.564,303.362,309.474,315.946,322.99,330.599,338.806,348.116,358.729,371.666,388.329,413.952,upper_limit};

int make_small() {
  make_header(vhibins_default, "cent/cent_default.h");
  make_header(vhibins_up, "cent/cent_up.h");
  make_header(vhibins_down, "cent/cent_down.h");

  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==4) return macro(argv[1], argv[2], argv[3]);
  if (argc==3) return macro(argv[1], argv[2]);
  if (argc==1) return make_small();
  return 1;
}

