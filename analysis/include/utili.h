#ifndef __XJJANA_UTILI__
#define __XJJANA_UTILI__

#include <cmath>

namespace xjjana
{
  float cal_dphi_0pi(float phi1, float phi2);
  float cal_dphi_01(float phi1, float phi2);
  float cal_dphi_2pi(float phi1, float phi2);
  float cal_dphi_2(float phi1, float phi2);
  float cal_dr(float phi1, float eta1, float phi2, float eta2);
}

float xjjana::cal_dphi_0pi(float phi1, float phi2)
{
  // phi1, phi2: -pi ~ pi
  float dphi = phi1 - phi2; // -2pi ~ 2pi
  dphi = fabs(dphi); // 0 ~ 2pi
  if(dphi > M_PI) dphi = 2*M_PI - dphi; // 0 ~ pi

  return dphi;
}

float xjjana::cal_dphi_2pi(float phi1, float phi2)
{
  // phi1, phi2: -pi ~ pi
  float dphi = phi1 - phi2; // -2pi ~ 2pi
  if(dphi < -M_PI/2.) dphi += 2*M_PI;
  if(dphi > M_PI*3/2.) dphi -= 2*M_PI; // -pi/2 ~ 3pi/2

  return dphi;
}

float xjjana::cal_dphi_01(float phi1, float phi2)
{
  float dphi = cal_dphi_0pi(phi1, phi2);
  dphi /= M_PI;

  return dphi;
}

float xjjana::cal_dphi_2(float phi1, float phi2)
{
  float dphi = cal_dphi_2pi(phi1, phi2);
  dphi /= M_PI;

  return dphi;
}

float xjjana::cal_dr(float phi1, float eta1, float phi2, float eta2)
{
  float dphi = cal_dphi_0pi(phi1, phi2);
  float deta = eta1 - eta2;
  float dr = std::sqrt(dphi*dphi + deta*deta);
  return dr;
}


#endif
