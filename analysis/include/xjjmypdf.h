#include <TCanvas.h>
#include <string>

#include "xjjrootuti.h"

/*
  xjjroot::mypdf pdf("output.pdf");
  // --> loop
  pdf.prepare();
  // draw
  pdf.write();
  // <--
  pdf.close();
*/
namespace xjjroot
{
  class mypdf
  {
  public:
    mypdf(std::string filename, std::string canvasname="c", int ww=1000, int hh=1000);
    mypdf(TCanvas* cc, std::string filename);
    void prepare() { fc->Clear(); fc->cd(); }
    void write(std::string pngname = "", std::string opt = "");
    void close() { fc->Print(Form("%s]", ffname.c_str())); delete fc; }
    TCanvas* getc() { return fc; }
    void draw_cover(std::vector<std::string> title, Size_t tsize=0.045);
    std::string getfilename() { return ffname; }
  private:
    TCanvas* fc;
    std::string ffname;
  };
}

xjjroot::mypdf::mypdf(std::string filename, std::string canvasname,
                      int ww, int hh) : ffname(filename) {
  mkdir(ffname);
  fc = new TCanvas(canvasname.c_str(), "", ww, hh);
  fc->Print(Form("%s[", ffname.c_str()));
}

xjjroot::mypdf::mypdf(TCanvas* cc, std::string filename)
  : ffname(filename), fc(cc) {
  fc->Print(Form("%s[", ffname.c_str()));
}

void xjjroot::mypdf::write(std::string pngname, std::string opt) {
  if (opt.find("Q") == std::string::npos) {
    std::time_t t = std::time(0);
    std::string datetime(100,0);
    datetime.resize(std::strftime(&datetime[0], datetime.size(), 
                                  "%b %d %a %-H:%M:%S %Z %Y", std::localtime(&t)));
    drawcomment(datetime, "rb");
    drawcomment(pngname);
  }

  fc->Print(Form("%s", ffname.c_str()));

  if (pngname != "" && opt.find("X") == std::string::npos) {
    mkdir(pngname);
    fc->SaveAs(pngname.c_str());
  }
}

void xjjroot::mypdf::draw_cover(std::vector<std::string> title, Size_t tsize) {
  if (title.empty()) return;
  prepare();
  xjjroot::drawtexgroup_wrap(0.5, 0.5, title, tsize, 22, 42, 1.15, 2./tsize);
  write();
}
