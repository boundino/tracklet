#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <vector>
#include <string>

#include "../analysis/include/xjjcuti.h"

xjjc::array2D<std::string> tree_list = {
                                        {
                                         "TrackletTree12",
                                         "TrackletTree13",
                                         "TrackletTree14",
                                         "TrackletTree23",
                                         "TrackletTree24",
                                         "TrackletTree34",
                                         "TrackletTree56",
                                         "TrackletTree57",
                                         "TrackletTree67",
                                         "truth" },
                                        {
                                         "TrackletTree11",
                                         "TrackletTree22",
                                         "TrackletTree33",
                                         "TrackletTree44",
                                         "TrackletTree55",
                                         "TrackletTree66",
                                         "TrackletTree77",
                                         "truth" },
                                        {
                                         "TrackletTree12",
                                         "TrackletTree13",
                                         "TrackletTree14",
                                         "TrackletTree23",
                                         "TrackletTree24",
                                         "TrackletTree34",
                                         "TrackletTree56",
                                         "TrackletTree57",
                                         "TrackletTree67",
                                         "TrackletTree11",
                                         "TrackletTree22",
                                         "TrackletTree33",
                                         "TrackletTree44",
                                         "TrackletTree55",
                                         "TrackletTree66",
                                         "TrackletTree77"
                                         "truth" },
                                        {
                                         "TrackletTree12",
                                         "TrackletTree13",
                                         "TrackletTree14",
                                         "TrackletTree23",
                                         "TrackletTree24",
                                         "TrackletTree34",
                                         "TrackletTree56",
                                         "TrackletTree57",
                                         "TrackletTree67",
                                        },
};

int macro(std::string outputname, std::string filelist, int itree = 0, int fillevery = 1) {

  TTree::SetMaxTreeSize(1LL * 1024 * 1024 * 1024 * 1024);

  std::vector<std::string> files;
  std::ifstream getfile(filelist.c_str());
  int count = 0;
  for(int i=0; ; i++) {
    std::string filename;
    getfile >> filename;
    if(getfile.eof()) { break; }
    files.push_back(filename);
    count++;
  }

  auto ttrees = tree_list[itree];
  std::vector<TChain*> nt;
  for (auto& t : ttrees) {
    auto chain = new TChain(t.c_str());
    nt.push_back(chain);
  }

  for(auto& f : files) {
    for (auto& t : nt) {
      t->Add(f.c_str());
    }
    std::cout<<f<<std::endl;
  }
  std::cout<<"Merged \e[31m"<<count<<"\e[0m files"<<std::endl;

  auto outf = TFile::Open(outputname.c_str(), "recreate", "", 101);
  std::vector<TTree*> new_nt;
  for (auto& t : nt) {
    auto new_tree = t->CloneTree(0);
    new_nt.push_back(new_tree);
  }

  auto nentries = nt.front()->GetEntries();
  std::cout<<" -- Event reading"<<std::endl;
  for(Long64_t i=0; i<nentries; i+=fillevery) {
    xjjc::progressslide(i, nentries);

    for (auto& t : nt) {
      t->GetEntry(i);
    }

    for (auto& t : new_nt) {
      t->Fill();
    }

  }
  xjjc::progressbar_summary(nentries);

  outf->Write();
  std::cout<<" -- Writing new trees done"<<std::endl;
  outf->Close();

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 5) {
    return macro(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
  }
  if (argc == 4) {
    return macro(argv[1], argv[2], atoi(argv[3]));
  }
  if (argc == 3) {
    return macro(argv[1], argv[2]);
  }

  return 1;
}
