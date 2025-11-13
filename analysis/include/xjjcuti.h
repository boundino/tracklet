#ifndef _XJJC_UTI_H_
#define _XJJC_UTI_H_

/* xjjcuti.h */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <string>
#include <cmath>
#include <vector>
#include <cxxabi.h>
#include <algorithm>
#include <regex>
#include <ctime>
#include <cctype>
#include <sstream>
#include <chrono>
#include <random>

namespace xjjc
{
  std::vector<std::string> speciallist = {" ", "/", "(", ")", "^", "#", "%", "$", ",", ".", "*", "&", ":", "{", "}", ";", "|"};

  template<size_t N, typename T> void initarray(T (*array_)[N], T initval_=0);
  template<size_t N, typename T> int findibin(const T (*array_)[N], T element_); // overflow: -1
  template<typename T> int findibin(const std::vector<T> array_, T element_); // overflow: -1
  template<size_t N, typename T> int findiedge(const T (*array_)[N], T element_);
  template<typename T> int findiedge(const std::vector<T> array_, T element_);

  template<typename T> std::string number_to_string(T param_);
  float string_to_number(std::string param_);
  template<typename T> std::string number_remove_zero(T param_);
  template<typename T> std::string number_range_string(T val1_, T val2_, std::string var, std::string opt);
  template<typename T> std::string number_range_string(T val1_, T val2_, std::string var, T over1_=0, T over2_=1.e+3, std::string unit="");
  int number_digit(int i, int n);
  template<typename T> T str_convert(const std::string& s);

  void progressbar(int index_, int total_, int step=10000, int morespace_=0);
  void progressslide(int index_, int total_, int step=10000, char done='=', char yet='.', char arrow='>');
  void progressbar_summary(int total_);

  template<typename T> char* gettype(T exp);

  template<class T> bool sortbydes(const T &a, const T &b) { return a > b; }
  template<class T1, class T2> bool sortbyfirst_des(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.first > b.first; }
  template<class T1, class T2> bool sortbyfirst_as(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.first < b.first; }
  template<class T1, class T2> bool sortbysecond_des(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.second > b.second; }
  template<class T1, class T2> bool sortbysecond_as(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.second < b.second; }
  // template<class T1, class T2> std::map<T1, T2>::iterator findbysecond(std::map<T1, T2>& map_, T2 value_);

  std::string str_replaceall(std::string strs, std::string sub, std::string newsub);
  std::string str_replaceallspecial(std::string strs, std::string newsub = "_");
  std::string str_eraseall(std::string strs, std::string sub) { return str_replaceall(strs, sub, ""); }
  std::string str_erasestar(std::string strs, std::string sub); // e.g. sub = */ or .*
  std::string str_trim(std::string strs);
  bool str_contains(std::string str1, std::string str2) { return str1.find(str2) != std::string::npos; }
  bool str_startsby(std::string str1, std::string str2) { return str1.find(str2) == 0; }
  bool str_endsby(std::string str1, std::string str2) { return str1.ends_with(str2); }
  bool str_isnumber(std::string strs) { return (std::regex_match(strs, std::regex("-?[0-9]+([.][0-9]*)?")) || std::regex_match(strs, std::regex("-?[0-9]*[.][0-9]+"))); }
  bool str_isinteger(std::string strs) { return std::regex_match(strs, std::regex("-?[0-9]+")); }
  std::vector<std::string> str_divide(std::string str, std::string div);
  std::vector<std::string> str_divide_trim(std::string str, std::string div);
  std::string str_divide_lastel(std::string str, std::string div);
  std::string str_getdir(std::string filename);
  std::string str_tolower(std::string str);
  std::string str_toupper(std::string str);

  template <class T> using array2D = std::vector<std::vector<T>>;
  template <class T> using array3D = std::vector<std::vector<std::vector<T>>>;
  template<class T> std::vector<std::vector<T>> array2d(int n1, int n2);
  template<class T> std::vector<std::vector<std::vector<T>>> array3d(int n1, int n2, int n3);

  std::string currenttime();
  std::string unique_str();
  void prt_divider(std::string color="\e[0m", int len=35) { std::cout<<color<<std::string(len, '-')<<"\e[0m"<<std::endl; }
}

/* ---------- */

template<size_t N, typename T>
void xjjc::initarray(T (*array_)[N], T initval_/*=0*/)
{
  for(std::size_t i=0;i<(sizeof(*array_)/sizeof((*array_)[0]));i++)
    {
      (*array_)[i]  = initval_;
    }
}

template<size_t N, typename T>
int xjjc::findibin(const T (*array_)[N], T element_)
{
  int ielement = -1;
  for(std::size_t i=0;i<((sizeof(*array_)/sizeof((*array_)[0]))-1) && ielement<0;i++)
    {
      if(element_>=(*array_)[i] && element_<(*array_)[i+1]) ielement = i;
    }
  return ielement;
}

template<typename T>
int xjjc::findibin(const std::vector<T> array_, T element_)
{
  int ielement = -1;
  for(std::size_t i=0;i<(array_.size()-1) && ielement<0;i++)
    {
      if(element_>=array_[i] && element_<array_[i+1]) ielement = i;
    }
  return ielement;
}

template<size_t N, typename T>
int xjjc::findiedge(const T (*array_)[N], T element_)
{
  int ielement = -1;
  for(std::size_t i=0;i<(sizeof(*array_)/sizeof((*array_)[0])) && ielement<0;i++)
    {
      if(element_==(*array_)[i]) ielement = i;
    }
  return ielement;
}

template<typename T>
int xjjc::findiedge(const std::vector<T> array_, T element_)
{
  int ielement = -1;
  for(std::size_t i=0;i<array_.size() && ielement<0;i++)
    {
      if(element_==array_[i]) ielement = i;
    }
  return ielement;
}

template<typename T>
std::string xjjc::number_to_string(T param_)
{
  std::string str;
  if(param_<0) { str += "M"; param_ = std::abs(param_); }
  str += std::to_string(param_);
  std::size_t found = str.find('.');
  if(found==std::string::npos)
    return str;
  str.replace(found, 1, "p");
  while(*(str.end()-1)=='0' && *(str.end()-2)!='p' && !str.empty()) str.erase(str.end()-1);
  if(*(str.end()-1)=='0' && *(str.end()-2)=='p') str.erase(str.size()-2, 2);
  return str;
}

float xjjc::string_to_number(std::string param_)
{
  bool minus = false;
  if(param_.front() == 'M')
    { minus = true; param_.erase(0, 1); }
  float result = atof(str_replaceall(param_, "p", ".").c_str());
  if(minus) result = 0-result;
  return result;
}

template<typename T>
std::string xjjc::number_remove_zero(T param_)
{
  bool negative = param_<0;
  if(negative) param_ = 0-param_;
  std::string str = std::to_string(param_);
  std::size_t found = str.find('.');
  if(found==std::string::npos) return str;
  while(*(str.end()-1)=='0') str.erase(str.end()-1);
  if(*(str.end()-1)=='.') str.erase(str.end()-1);
  if(negative) str = "-"+str;
  return str;
}

template<typename T>
std::string xjjc::number_range_string(T val1_, T val2_, std::string var, std::string opt)
{
  std::string str = "";
  if(val1_ == val2_) { return str; }
  if(val1_<val2_ && opt=="fmax") { T valtmp_ = val1_; val1_ = val2_; val2_ = valtmp_; }
  if(val1_>val2_ && opt=="fmin") { T valtmp_ = val1_; val1_ = val2_; val2_ = valtmp_; }
  std::string sign = val1_<val2_?"<":">";
  if(opt!="fmax" && opt!="fmin" ) { str += (number_remove_zero(val1_)+" "+sign+" "); }
  str += var;
  str += (" "+sign+" "+number_remove_zero(val2_)); 
  return str;
}

template<typename T>
std::string xjjc::number_range_string(T val1_, T val2_, std::string var, T over1_/*=0*/, T over2_/*=1.e+3*/, std::string unit/*=""*/)
{
  if (val2_ >= over2_) return number_range_string(val1_, val2_, var, "fmax")+" "+unit;
  else if (val1_ <= over1_) return number_range_string(val1_, val2_, var, "fmin")+" "+unit;
  return number_range_string(val1_, val2_, var, "")+" "+unit;
}


int xjjc::number_digit(int i, int n)
{
  if(n < 0) return 0;
  int i1 = i%((int)std::pow(10, n+1));
  if(n==0) return i1;
  int i2 = std::floor(float(i1/std::pow(10, n-1)));
  return i2;
}

template<typename T>
T xjjc::str_convert(const std::string& s) {
  if constexpr (std::is_same_v<T, std::string>)
                 return s;
  else if constexpr (std::is_same_v<T, int>)
                      return s.empty() ? 0 : std::stoi(s);
  else if constexpr (std::is_same_v<T, float>)
                      return s.empty() ? 0.f : std::stof(s);
  else
    static_assert(!sizeof(T*), "Unsupported type for safeConvert()");
}

void xjjc::progressbar(int index_, int total_, int step, int morespace_/*=0*/)
{
  if(index_%step==0 || index_ == total_-1)
    {
      if(total_ > 0)
        std::cout << std::setiosflags(std::ios::left)
                  << "  [ \e[36m" << std::setw(10+morespace_) << index_ << "\e[0m"
                  << " / " << std::setw(10+morespace_) << total_ << " ] "
                  << "\e[36m" << round(100.*index_/total_) << "%\e[0m" << "\r" << std::flush;
      else
        std::cout << std::setiosflags(std::ios::left)
                  << "  [ \e[36m" << std::setw(10+morespace_) << index_ << "\e[0m ]" << "\r" << std::flush;
    }
}

void xjjc::progressslide(int index_, int total_, int step, char done, char yet, char arrow)
{
  if(index_%step==0 || index_ == total_-1)
    {
      if(index_ == total_-1) index_++;
      std::cout << std::setiosflags(std::ios::left) << "  [ \e[36m"
                << std::setw(21) << std::string(round(20.*index_/total_), done) + std::string(1, arrow)+std::string(20-round(20.*index_/total_), yet)
                << "\e[0m ] " << "\e[36m" << index_ << "\e[0m" << " / " << total_ << "\r" << std::flush;
    }
}

void xjjc::progressbar_summary(int total_)
{
  std::cout << std::endl << "  Processed " << "\e[1;31m" << total_ << "\e[0m event(s)." << std::endl;
}

template<typename T>
char* xjjc::gettype(T exp)
{
  const std::type_info &ti = typeid(exp);
  int status = 0;
  char* humantypename = abi::__cxa_demangle(ti.name(), 0, 0, &status);
  return humantypename;
}

std::string xjjc::str_erasestar(std::string strs, std::string sub)
{
  std::string realsub = str_replaceall(sub, "*", "");
  std::string result(strs), str(strs);
  if(sub.front() == '*')
    {
      size_t pos = str.find(realsub);
      while (str.find(realsub, pos+1) != std::string::npos) {
        pos = str.find(realsub, pos+1);
      }
      if(pos != std::string::npos)
        result.erase(0, pos+realsub.size());
    }
  else if(sub.back() == '*')
    { 
      size_t pos = str.find(realsub); 
      if(pos != std::string::npos)
        result.erase(pos, str.size() - pos);
    }
  return result;
}

std::string xjjc::str_trim(std::string strs)
{
  std::string result(strs), str(strs);
  size_t pos_front = str.find_first_not_of(" ");
  if(pos_front != std::string::npos && pos_front != 0)
    result.erase(0, pos_front);
  size_t pos_back = result.find_last_not_of(" ");
  if(pos_back != std::string::npos && pos_back != result.size()-1)
    result.erase(pos_back+1, str.size() - (pos_back+1));
  if(pos_front == std::string::npos && pos_back == std::string::npos)
    result = "";
  return result;
}

std::string xjjc::str_replaceall(std::string strs, std::string sub, std::string newsub)
{
  std::string result(strs);
  std::string str(strs);
  size_t pos = str.find(sub, 0);
  size_t thispos = 0;
  while(pos != std::string::npos)
    {
      result.replace(pos + thispos, sub.size(), newsub);
      thispos += (newsub.size() - sub.size());
      pos = str.find(sub, pos+1);
    }
  return result;
}

std::string xjjc::str_replaceallspecial(std::string strs, std::string newsub)
{
  std::string result(strs);
  for(auto& isp : speciallist) { result = xjjc::str_replaceall(result, isp, newsub); }
  return result;
}

std::vector<std::string> xjjc::str_divide(std::string str, std::string div)
{
  std::vector<std::string> token;
  size_t lastpos = 0;
  size_t pos = str.find(div, 0);
  while(pos != std::string::npos) 
    {
      std::string thistoken = str.substr(lastpos, pos-lastpos);
      token.push_back(thistoken);
      lastpos = pos + div.size();
      pos = str.find(div, pos + div.size());
    }
  if(lastpos != 0) { token.push_back(str.substr(lastpos, str.size()-lastpos)); }
  if(token.empty()) token.push_back(str);
  return token;
}

std::vector<std::string> xjjc::str_divide_trim(std::string str, std::string div) {
  auto token = str_divide(str, div);
  for (auto& tt : token) {
    tt = str_trim(tt);
  }
  return token;
}

std::string xjjc::str_divide_lastel(std::string str, std::string div)
{
  std::vector<std::string> token = str_divide(str, div);
  if(token.empty()) return "";
  return token[token.size()-1];
}

std::string xjjc::str_tolower(std::string str)
{
  std::string newstr(str);
  std::transform(newstr.begin(), newstr.end(), newstr.begin(),
                 [](unsigned char c){ return std::tolower(c); });
  return newstr;
}

std::string xjjc::str_toupper(std::string str)
{
  std::string newstr(str);
  std::transform(newstr.begin(), newstr.end(), newstr.begin(),
                 [](unsigned char c){ return std::toupper(c); });
  return newstr;
}

std::string xjjc::currenttime()
{
  std::time_t t = std::time(0);   // get time now
  std::tm* now = std::localtime(&t);
  char chartime[29]; // yyyymmdd-hhmmss
  snprintf(chartime, 29, "%d%s%d%d-%d%d%d", now->tm_year+1900, (now->tm_mon>=9?"":"0"), now->tm_mon+1, now->tm_mday,
           now->tm_hour, now->tm_min, now->tm_sec);
  return std::string(chartime);
}

std::string xjjc::unique_str()
{
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  
  // Generate a random number
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distribution(1000, 9999);
  int randomNumber = distribution(generator);

  // Combine time and random number into a unique string
  std::ostringstream uniqueString;
  uniqueString << millis << "_" << randomNumber;

  return uniqueString.str();
}

std::string xjjc::str_getdir(std::string filename)
{
  std::string dir = str_replaceall(filename, str_divide(filename, "/").back(), "");
  return dir;
}

template<class T> std::vector<std::vector<T>> xjjc::array2d(int n1, int n2)
{
  std::vector<std::vector<T>> v(n1);
  for(auto& vi : v)
    vi.resize(n2);
  return v;
}

template<class T> std::vector<std::vector<std::vector<T>>> xjjc::array3d(int n1, int n2, int n3)
{
  std::vector<std::vector<std::vector<T>>> v(n1);
  for(auto& vi : v)
    {
      vi.resize(n2);
      for(auto& vj : vi)
        vj.resize(n3);
    }
  return v;
}

#ifndef __PRMYERR
#define __PRMYERR(info) { std::cout<<"\e[42m("<<__FUNCTION__<<")\e[0m \e[31;1merror: \e[0m"<<#info<<std::endl; }
#endif

#endif
