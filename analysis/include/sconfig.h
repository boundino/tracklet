#ifndef __XJJC_SCONFIG_H_
#define __XJJC_SCONFIG_H_

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "xjjcuti.h"

namespace xjjc
{
  class sconfig
  {
  public:
    sconfig(std::string input, std::string tok1=",", std::string tok2="#", std::string opt="v") :
      input_(input), t1_(tok1), t2_(tok2), opt_(opt) { parse(); }
    int n() { return value.size(); }
    std::vector<std::vector<std::string>> value;
    void print();
  private:
    std::string input_, t1_, t2_, opt_;
    std::vector<size_t> size_;
    void parse();
  };
}

void xjjc::sconfig::parse()
{
  auto inputs = xjjc::str_divide_trim(input_, t1_);
  for(int i=0; i<inputs.size(); i++)
    {
      auto params = xjjc::str_divide_trim(inputs[i], t2_);
      value.push_back(params);
      if(!i) size_.resize(params.size(), 0);
      for(int j=0; j<size_.size(); j++)
        size_[j] = std::max(params[j].length(), size_[j]);
    }
  if(opt_.find('v') != std::string::npos)
    print();
}

void xjjc::sconfig::print()
{
  std::cout<<std::endl;
  for(auto& v : value)
    {      
      for(int j=0; j<v.size(); j++)
        std::cout<<std::left<<" \e[38:5:255m\e[48:5:238m "<<std::setw(size_[j]+1)<<v[j]<<"\e[0m";
      std::cout<<std::endl;
    }
  std::cout<<std::endl;
}

#endif
