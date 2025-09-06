#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

int trajectum(std::string input, std::string tag) {
  std::ifstream getdata(input.c_str());

  std::string line = "";

  std::vector<float> raw, rawerr, cent;
  while (true) {
    std::getline(getdata, line);
    if(line == "" || getdata.eof()) break;
    if(line[0] == '#') continue;

    std::istringstream iss(line);
    float x, y, stat, syst;
    iss >> x >> y >> stat >> syst;
    syst = std::sqrt(stat*stat + syst*syst);
    x = 100 - x;
    cent.push_back(x);
    raw.push_back(y);
    rawerr.push_back(syst);
  }

  
#define PRINT(t)                                        \
  std::cout<<"std::vector<float> "<<tag<<"_" #t " = { "<<std::endl<<"   ";   \
  for (auto i : t) {                                    \
    std::cout<<i<<", ";                                 \
  }                                                     \
  std::cout<<"};"<<std::endl;                           \

  
  PRINT(raw);
  PRINT(rawerr);
  PRINT(cent);

  std::vector<float> npart(raw.size(), 32),
    nparterr(raw.size(), 0);

  PRINT(npart);
  PRINT(nparterr);

  std::cout<<"std::vector<float> "<<tag<<"_ncoll = "<<tag<<"_npart;"<<std::endl
           <<"std::vector<float> "<<tag<<"_ncollerr = "<<tag<<"_nparterr;"<<std::endl
           <<"auto "<<tag<<"_2a = 32.;"<<std::endl
           <<"auto "<<tag<<"_size = "<<tag<<"_raw.size();"<<std::endl;

  std::cout<<std::endl;
  
  return 0;
}

int main() {
  trajectum("data/multOONLEFT.dat", "nleft_oo_6p8");
  trajectum("data/multOOPGCM.dat", "pgcm_oo_6p8");

  return 0;
}
