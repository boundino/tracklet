#include "TH1.h"
#include "TGraphErrors.h"

#define GENERATE(MSRMNT)                                                      \
TGraphErrors* MSRMNT() {                                                      \
   TGraphErrors* g##MSRMNT = new TGraphErrors(MSRMNT##_size);                 \
   g##MSRMNT->SetName("g" #MSRMNT);                                           \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                              \
      g##MSRMNT->SetPoint(i, MSRMNT##_cent[i], MSRMNT##_raw[i]);              \
      g##MSRMNT->SetPointError(i, 0, MSRMNT##_rawerr[i]); }                   \
   return g##MSRMNT; }                                                        \
TGraphErrors* MSRMNT##_n2a() {                                                \
   TGraphErrors* g##MSRMNT##_n2a = new TGraphErrors(MSRMNT##_size);           \
   g##MSRMNT##_n2a->SetName("g" #MSRMNT "_n2a");                              \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                              \
      g##MSRMNT##_n2a->SetPoint(i, MSRMNT##_cent[i],                          \
         MSRMNT##_raw[i]/MSRMNT##_2a);                                        \
      g##MSRMNT##_n2a->SetPointError(i, 0,                                    \
         MSRMNT##_rawerr[i]/MSRMNT##_2a); }                                   \
   return g##MSRMNT##_n2a; }                                                  \
TGraphErrors* MSRMNT##_nnpart() {                                             \
   TGraphErrors* g##MSRMNT##_nnpart = new TGraphErrors(MSRMNT##_size);        \
   g##MSRMNT##_nnpart->SetName("g" #MSRMNT "_nnpart");                        \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                              \
      g##MSRMNT##_nnpart->SetPoint(i, MSRMNT##_cent[i],                       \
         MSRMNT##_npart[i]?MSRMNT##_raw[i]/MSRMNT##_npart[i]: 0);             \
      g##MSRMNT##_nnpart->SetPointError(i, 0,                                 \
         MSRMNT##_npart[i]?MSRMNT##_rawerr[i]/MSRMNT##_npart[i]: 0); }        \
   return g##MSRMNT##_nnpart; }                                               \
TGraphErrors* MSRMNT##_n2a_x_npart() {                                        \
   TGraphErrors* g##MSRMNT##_n2a_x_npart = new TGraphErrors(MSRMNT##_size);   \
   g##MSRMNT##_n2a_x_npart->SetName("g" #MSRMNT "_n2a_x_npart");              \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                              \
      g##MSRMNT##_n2a_x_npart->SetPoint(i, MSRMNT##_npart[i],                 \
         MSRMNT##_raw[i]/MSRMNT##_2a);                                        \
      g##MSRMNT##_n2a_x_npart->SetPointError(i, MSRMNT##_nparterr[i],         \
         MSRMNT##_rawerr[i]/MSRMNT##_2a); }                                   \
   return g##MSRMNT##_n2a_x_npart; }                                          \
TGraphErrors* MSRMNT##_nnpart_x_npart() {                                     \
   TGraphErrors* g##MSRMNT##_nnpart_x_npart = new TGraphErrors(MSRMNT##_size);\
   g##MSRMNT##_nnpart_x_npart->SetName("g" #MSRMNT "_nnpart_x_npart");        \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                              \
      g##MSRMNT##_nnpart_x_npart->SetPoint(i, MSRMNT##_npart[i],              \
         MSRMNT##_npart[i]?MSRMNT##_raw[i]/MSRMNT##_npart[i]: 0);             \
      g##MSRMNT##_nnpart_x_npart->SetPointError(i, MSRMNT##_nparterr[i],      \
         MSRMNT##_npart[i]?MSRMNT##_rawerr[i]/MSRMNT##_npart[i]: 0); }        \
   return g##MSRMNT##_nnpart_x_npart; }                                       \
TGraphErrors* MSRMNT##_n2a_x_npart2a() {                                      \
   TGraphErrors* g##MSRMNT##_n2a_x_npart2a =                                  \
      new TGraphErrors(MSRMNT##_size);                                        \
   g##MSRMNT##_n2a_x_npart2a->SetName("g" #MSRMNT "_n2a_x_npart2a");          \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                              \
      g##MSRMNT##_n2a_x_npart2a->SetPoint(i, MSRMNT##_npart[i]/MSRMNT##_2a,   \
         MSRMNT##_raw[i]/MSRMNT##_2a);                                        \
      g##MSRMNT##_n2a_x_npart2a->SetPointError(i,                             \
         MSRMNT##_nparterr[i]/MSRMNT##_2a,                                    \
         MSRMNT##_rawerr[i]/MSRMNT##_2a); }                                   \
   return g##MSRMNT##_n2a_x_npart2a; }                                        \
TGraphErrors* MSRMNT##_nnpart_x_npart2a() {                                   \
   TGraphErrors* g##MSRMNT##_nnpart_x_npart2a =                               \
      new TGraphErrors(MSRMNT##_size);                                        \
   g##MSRMNT##_nnpart_x_npart2a->SetName("g" #MSRMNT "_nnpart_x_npart2a");    \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                              \
      g##MSRMNT##_nnpart_x_npart2a->SetPoint(i,                               \
         MSRMNT##_npart[i]/MSRMNT##_2a,                                       \
         MSRMNT##_npart[i]?MSRMNT##_raw[i]/MSRMNT##_npart[i]: 0);             \
      g##MSRMNT##_nnpart_x_npart2a->SetPointError(i,                          \
         MSRMNT##_nparterr[i]/MSRMNT##_2a,                                    \
         MSRMNT##_npart[i]?MSRMNT##_rawerr[i]/MSRMNT##_npart[i]: 0); }        \
   return g##MSRMNT##_nnpart_x_npart2a; }                                     \
 TGraphErrors* MSRMNT##_nnpart_x_ncollnpart() {                         \
   TGraphErrors* g##MSRMNT##_nnpart_x_ncollnpart =                      \
     new TGraphErrors(MSRMNT##_size);                                   \
   g##MSRMNT##_nnpart_x_ncollnpart->SetName("g" #MSRMNT "_nnpart_x_ncollnpart"); \
   for (std::size_t i=0; i<MSRMNT##_size; ++i) {                        \
     g##MSRMNT##_nnpart_x_ncollnpart->SetPoint(i,                       \
         MSRMNT##_npart[i]?MSRMNT##_ncoll[i]/MSRMNT##_npart[i]: 0, \
         MSRMNT##_npart[i]?MSRMNT##_raw[i]/MSRMNT##_npart[i]: 0);             \
     auto ncollnparterr = ((MSRMNT##_ncoll[i] + MSRMNT##_ncollerr[i]) / (MSRMNT##_npart[i] - MSRMNT##_nparterr[i]) \
                           - (MSRMNT##_ncoll[i] - MSRMNT##_ncollerr[i]) / (MSRMNT##_npart[i] + MSRMNT##_nparterr[i])) / 2.; \
     g##MSRMNT##_nnpart_x_ncollnpart->SetPointError(i,                  \
         ncollnparterr,      \
         MSRMNT##_npart[i]?MSRMNT##_rawerr[i]/MSRMNT##_npart[i]: 0); }        \
   return g##MSRMNT##_nnpart_x_ncollnpart; }                            \
 TGraphErrors* MSRMNT##_nnpart_x_ncollnpartnpart() {                    \
 TGraphErrors* g##MSRMNT##_nnpart_x_ncollnpartnpart =                   \
   new TGraphErrors(MSRMNT##_size);                                     \
 g##MSRMNT##_nnpart_x_ncollnpartnpart->SetName("g" #MSRMNT "_nnpart_x_ncollnpartnpart"); \
 for (std::size_t i=0; i<MSRMNT##_size; ++i) {                          \
   g##MSRMNT##_nnpart_x_ncollnpartnpart->SetPoint(i,                    \
         MSRMNT##_ncoll[i]/MSRMNT##_npart[i]/MSRMNT##_npart[i], \
         MSRMNT##_npart[i]?MSRMNT##_raw[i]/MSRMNT##_npart[i]: 0);             \
   auto ncollnpartnparterr = ((MSRMNT##_ncoll[i] + MSRMNT##_ncollerr[i]) / (MSRMNT##_npart[i] - MSRMNT##_nparterr[i]) / (MSRMNT##_npart[i] - MSRMNT##_nparterr[i]) \
                              - (MSRMNT##_ncoll[i] - MSRMNT##_ncollerr[i]) / (MSRMNT##_npart[i] + MSRMNT##_nparterr[i]) / (MSRMNT##_npart[i] + MSRMNT##_nparterr[i])) / 2.; \
   g##MSRMNT##_nnpart_x_ncollnpartnpart->SetPointError(i,               \
         ncollnpartnparterr, \
         MSRMNT##_npart[i]?MSRMNT##_rawerr[i]/MSRMNT##_npart[i]: 0); }        \
 return g##MSRMNT##_nnpart_x_ncollnpartnpart; }                         \
 
std::vector<float> cms_pbpb_2p76_raw = {
   11.05, 18.49, 28.12, 44.46, 65.42, 93.55, 130.3, 176.1, 233.0,
   299.9, 383.9, 482.3, 596.8, 731.6, 893.9, 1082, 1313, 1612};
std::vector<float> cms_pbpb_2p76_rawerr = {
   1.77, 2.96, 3.26, 4.71, 5.41, 7.73, 8.6, 10.7, 13,
   15.3, 16.2, 18.7, 23.1, 26.3, 31.4, 38, 45, 55};
std::vector<float> cms_pbpb_2p76_cent = {
   12.5, 17.5, 22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5,
   57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> cms_pbpb_2p76_npart = {
   5.71, 8.64, 12.8, 18.5, 25.8, 35.3, 46.7, 60.4, 76.8,
   95.8, 117, 142, 171, 203, 240, 283, 329, 381};
std::vector<float> cms_pbpb_2p76_nparterr = {
   0.24, 0.56, 0.9, 1.2, 1.6, 2.0, 2.3, 2.7, 2.7,
   3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.0};
std::vector<float> cms_pbpb_2p76_ncoll = { // ! placeholder 
   5.71, 8.64, 12.8, 18.5, 25.8, 35.3, 46.7, 60.4, 76.8,
   95.8, 117, 142, 171, 203, 240, 283, 329, 381};
std::vector<float> cms_pbpb_2p76_ncollerr = {
   0.24, 0.56, 0.9, 1.2, 1.6, 2.0, 2.3, 2.7, 2.7,
   3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.0};
auto cms_pbpb_2p76_2a = 416.;
auto cms_pbpb_2p76_size = cms_pbpb_2p76_raw.size();

std::vector<float> alice_pbpb_2p76_raw = {
   35, 76, 149, 261, 426, 649, 966, 1294, 1601};
std::vector<float> alice_pbpb_2p76_rawerr = {
   2, 4, 6, 9, 15, 23, 37, 49, 60};
std::vector<float> alice_pbpb_2p76_cent = {
   25, 35, 45, 55, 65, 75, 85, 92.5, 97.5};
std::vector<float> alice_pbpb_2p76_npart = {
   15.8, 30.0, 52.8, 85.0, 128.9, 186.4, 260.5, 329.7, 382.8};
std::vector<float> alice_pbpb_2p76_nparterr = {
   0.6, 1.3, 2.0, 2.6, 3.3, 3.9, 4.4, 4.6, 3.1};
std::vector<float> alice_pbpb_2p76_ncoll = { // ! placeholder
   15.8, 30.0, 52.8, 85.0, 128.9, 186.4, 260.5, 329.7, 382.8};
std::vector<float> alice_pbpb_2p76_ncollerr = {
   0.6, 1.3, 2.0, 2.6, 3.3, 3.9, 4.4, 4.6, 3.1};
auto alice_pbpb_2p76_2a = 416.;
auto alice_pbpb_2p76_size = alice_pbpb_2p76_raw.size();

std::vector<float> alice_pbpb_5p02_raw = {
   44.9, 96.3, 183, 318, 512, 786, 1180, 1505, 1666, 1850, 2035};
std::vector<float> alice_pbpb_5p02_rawerr = {
   3.4, 5.8, 8, 12, 15, 20, 31, 44, 48, 55, 52};
std::vector<float> alice_pbpb_5p02_cent = {
   25, 35, 45, 55, 65, 75, 85, 91.25, 93.75, 96.25, 98.75};
std::vector<float> alice_pbpb_5p02_npart = {
   15.6, 30.4, 53.6, 86.3, 131, 188, 263, 320, 346, 372, 398};
std::vector<float> alice_pbpb_5p02_nparterr = {
   0.5, 0.8, 1.2, 1.7, 2, 3, 4, 4, 4, 3, 2};
std::vector<float> alice_pbpb_5p02_ncoll = { // ! placeholder
   15.6, 30.4, 53.6, 86.3, 131, 188, 263, 320, 346, 372, 398};
std::vector<float> alice_pbpb_5p02_ncollerr = {
   0.5, 0.8, 1.2, 1.7, 2, 3, 4, 4, 4, 3, 2};
auto alice_pbpb_5p02_2a = 416.;
auto alice_pbpb_5p02_size = alice_pbpb_5p02_raw.size();

std::vector<float> phobos_auau_0p2_raw = {
   27, 38, 54, 73, 100, 133, 165, 208,
   257, 313, 384, 465, 540, 619, 691};
std::vector<float> phobos_auau_0p2_rawerr = {
   2, 3, 4, 5, 8, 10, 12, 16,
   19, 24, 29, 35, 41, 46, 52};
std::vector<float> phobos_auau_0p2_cent = {
   32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 62.5, 67.5,
   72.5, 77.5, 82.5, 87.5, 92,  95.5, 98.5};
std::vector<float> phobos_auau_0p2_npart = {
   20, 28, 37, 49, 65, 82, 101, 124,
   150, 180, 215, 255, 297, 331, 361};
std::vector<float> phobos_auau_0p2_nparterr = {
   3, 3, 4, 5, 6, 6, 6, 6,
   6, 7, 7, 8, 9, 10, 11};
std::vector<float> phobos_auau_0p2_ncoll = { // ! placeholder
   20, 28, 37, 49, 65, 82, 101, 124,
   150, 180, 215, 255, 297, 331, 361};
std::vector<float> phobos_auau_0p2_ncollerr = {
   3, 3, 4, 5, 6, 6, 6, 6,
   6, 7, 7, 8, 9, 10, 11};
auto phobos_auau_0p2_2a = 394.;
auto phobos_auau_0p2_size = phobos_auau_0p2_raw.size();

std::vector<float> phobos_auau_0p13_raw = {
   83, 110, 140, 180, 220, 274, 327, 393, 472, 545, 613};
std::vector<float> phobos_auau_0p13_rawerr = {
   3, 4, 5, 7, 8, 11, 13, 15, 18, 21, 24};
std::vector<float> phobos_auau_0p13_cent = {
   52.5, 57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92, 95.5, 98.5};
std::vector<float> phobos_auau_0p13_npart = {
   63, 80, 100, 122, 148, 179, 214, 254, 295, 330, 355};
std::vector<float> phobos_auau_0p13_nparterr = {
   4, 5, 5, 6, 6, 7, 8, 8, 9, 10, 12};
std::vector<float> phobos_auau_0p13_ncoll = { // ! placeholder
   63, 80, 100, 122, 148, 179, 214, 254, 295, 330, 355};
std::vector<float> phobos_auau_0p13_ncollerr = {
   4, 5, 5, 6, 6, 7, 8, 8, 9, 10, 12};
auto phobos_auau_0p13_2a = 394.;
auto phobos_auau_0p13_size = phobos_auau_0p13_raw.size();

std::vector<float> phobos_auau_0p0624_raw = {
   17, 25, 36, 50, 67, 87, 111, 140,
   174, 212, 260, 316, 377, 433, 492};
std::vector<float> phobos_auau_0p0624_rawerr = {
   1, 2, 3, 4, 5, 6, 8, 10,
   13, 15, 19, 23, 28, 32, 36};
std::vector<float> phobos_auau_0p0624_cent = {
   32.5, 37.5, 45, 47.5, 52.5, 57.5, 62.5, 67.5,
   72.5, 77.5, 82.5, 87.5, 92, 95.5, 98.5};
std::vector<float> phobos_auau_0p0624_npart = {
   19, 27, 36, 48, 62, 78, 98, 119,
   145, 174, 209, 248, 288, 325, 356};
std::vector<float> phobos_auau_0p0624_nparterr = {
   3, 3, 4, 5, 6, 6, 7, 7,
   7, 7, 7, 8, 9, 10, 11};
std::vector<float> phobos_auau_0p0624_ncoll = { // ! placeholder
   19, 27, 36, 48, 62, 78, 98, 119,
   145, 174, 209, 248, 288, 325, 356};
std::vector<float> phobos_auau_0p0624_ncollerr = {
   3, 3, 4, 5, 6, 6, 7, 7,
   7, 7, 7, 8, 9, 10, 11};
auto phobos_auau_0p0624_2a = 394.;
auto phobos_auau_0p0624_size = phobos_auau_0p0624_raw.size();

std::vector<float> phobos_auau_0p0196_raw = {
   78, 97, 121, 148, 181, 216, 260, 297, 331};
std::vector<float> phobos_auau_0p0196_rawerr = {
   6, 7, 9, 11, 14, 16, 20, 22, 24};
std::vector<float> phobos_auau_0p0196_cent = {
   62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92, 95.5, 98.5};
std::vector<float> phobos_auau_0p0196_npart = {
   95, 117, 142, 171, 206, 247, 286, 322, 351};
std::vector<float> phobos_auau_0p0196_nparterr = {
   7, 7, 7, 7, 8, 8, 9, 10, 11};
std::vector<float> phobos_auau_0p0196_ncoll = { // ! placeholder
   95, 117, 142, 171, 206, 247, 286, 322, 351};
std::vector<float> phobos_auau_0p0196_ncollerr = {
   7, 7, 7, 7, 8, 8, 9, 10, 11};
auto phobos_auau_0p0196_2a = 394.;
auto phobos_auau_0p0196_size = phobos_auau_0p0196_raw.size();

std::vector<float> phobos_cucu_0p2_raw = {
   32, 40, 50, 62, 75, 91, 109, 132, 155, 175, 198};
std::vector<float> phobos_cucu_0p2_rawerr = {
   2, 3, 4, 5, 6, 7, 8, 10, 12, 13, 15};
std::vector<float> phobos_cucu_0p2_cent = {
   52.5, 57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92, 95.5, 98.5};
std::vector<float> phobos_cucu_0p2_npart = {
   22, 27, 33, 40, 48, 57, 67, 79, 91, 101, 108};
std::vector<float> phobos_cucu_0p2_nparterr = {
   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4};
std::vector<float> phobos_cucu_0p2_ncoll = { // ! placeholder
   22, 27, 33, 40, 48, 57, 67, 79, 91, 101, 108};
std::vector<float> phobos_cucu_0p2_ncollerr = {
   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4};
auto phobos_cucu_0p2_2a = 128.;
auto phobos_cucu_0p2_size = phobos_cucu_0p2_raw.size();

std::vector<float> phobos_cucu_0p0624_raw = {
   23, 28, 35, 43, 52, 64, 77, 92, 108, 123, 138};
std::vector<float> phobos_cucu_0p0624_rawerr = {
   2, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<float> phobos_cucu_0p0624_cent = {
   52.5, 57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92, 95.5, 98.5};
std::vector<float> phobos_cucu_0p0624_npart = {
   21, 26, 32, 38, 47, 55, 65, 76, 88, 97, 106};
std::vector<float> phobos_cucu_0p0624_nparterr = {
   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
std::vector<float> phobos_cucu_0p0624_ncoll = { // ! placeholder
   21, 26, 32, 38, 47, 55, 65, 76, 88, 97, 106};
std::vector<float> phobos_cucu_0p0624_ncollerr = {
   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
auto phobos_cucu_0p0624_2a = 128.;
auto phobos_cucu_0p0624_size = phobos_cucu_0p0624_raw.size();

std::vector<float> phobos_cucu_0p0224_raw = {
   18, 22, 27, 33, 40, 48, 57, 68, 80, 90, 101};
std::vector<float> phobos_cucu_0p0224_rawerr = {
   1, 2, 2, 2, 3, 4, 4, 5, 6, 7, 8};
std::vector<float> phobos_cucu_0p0224_cent = {
   52.5, 57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92, 95.5, 98.5};
std::vector<float> phobos_cucu_0p0224_npart = {
   20, 24, 30, 37, 44, 53, 63, 74, 86, 95, 103};
std::vector<float> phobos_cucu_0p0224_nparterr = {
   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
std::vector<float> phobos_cucu_0p0224_ncoll = { // ! placeholder
   20, 24, 30, 37, 44, 53, 63, 74, 86, 95, 103};
std::vector<float> phobos_cucu_0p0224_ncollerr = {
   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
auto phobos_cucu_0p0224_2a = 128.;
auto phobos_cucu_0p0224_size = phobos_cucu_0p0224_raw.size();

std::vector<float> brahms_auau_0p2_raw = {
   110., 174., 257., 377., 501., 625.};
std::vector<float> brahms_auau_0p2_rawerr = {
   10., 16., 23., 33., 44., 55.};
std::vector<float> brahms_auau_0p2_cent = {
   55, 65, 75, 85, 92.5, 97.5};
std::vector<float> brahms_auau_0p2_npart = {
   73, 114, 168, 239, 306, 357};
std::vector<float> brahms_auau_0p2_nparterr = {
   8, 9, 9, 10, 11, 8};
std::vector<float> brahms_auau_0p2_ncoll = { // ! placeholder
   73, 114, 168, 239, 306, 357};
std::vector<float> brahms_auau_0p2_ncollerr = {
   8, 9, 9, 10, 11, 8};
auto brahms_auau_0p2_2a = 394.;
auto brahms_auau_0p2_size = brahms_auau_0p2_raw.size();

std::vector<float> alice_xexe_5p44_raw = {
   13.3, 32.0, 64.7, 118, 198, 315, 478, 706, 891, 986, 1069, 1113, 1166, 1223, 1302};
std::vector<float> alice_xexe_5p44_rawerr = {
   0.9, 1.3, 2.0, 3, 5, 8, 11, 17, 24, 25, 20, 20, 23, 25, 17};
std::vector<float> alice_xexe_5p44_cent = {
   15, 25, 35, 45, 55, 65, 75, 85, 91.25, 93.75, 95.5, 96.5, 97.5, 98.5, 99.5};
std::vector<float> alice_xexe_5p44_npart = {
   5.13, 10.4, 19.7, 34.1, 54.6, 82.2, 118, 165, 199, 214, 225, 231, 236, 241, 246};
std::vector<float> alice_xexe_5p44_nparterr = {
   0.46, 1.1, 2.1, 3.0, 3.6, 3.9, 4, 3, 2, 3, 3, 2, 3, 2, 2};
std::vector<float> alice_xexe_5p44_ncoll = { // ! placeholder
   5.13, 10.4, 19.7, 34.1, 54.6, 82.2, 118, 165, 199, 214, 225, 231, 236, 241, 246};
std::vector<float> alice_xexe_5p44_ncollerr = {
   0.46, 1.1, 2.1, 3.0, 3.6, 3.9, 4, 3, 2, 3, 3, 2, 3, 2, 2};
auto alice_xexe_5p44_2a = 258.;
auto alice_xexe_5p44_size = alice_xexe_5p44_raw.size();

// https://twiki.cern.ch/twiki/bin/viewauth/CMS/XeXeGlauber
std::vector<float> cms_xexe_5p44_raw = {
  18.8, 29.2, 43.7, 63.2, 89, 121, 162, 212,
  273, 344, 430, 532, 653, 798, 977, 1187};
std::vector<float> cms_xexe_5p44_rawerr = {
  4.9, 6.4, 8.2, 9.9, 12, 13, 15, 17,
  18, 20, 22, 24, 25, 28, 32, 36};
std::vector<float> cms_xexe_5p44_cent = {
   22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5,
   57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> cms_xexe_5p44_npart = {
  8.62, 12.2, 16.8, 22.7, 29.7, 38.3, 48.6, 60.3,
  73.8, 89.5, 107.3, 127.5, 151.1, 177.1, 206.6, 236.5};
std::vector<float> cms_xexe_5p44_nparterr = {
   0.66, 1.0, 1.5, 1.9, 2.4, 2.6, 3.0,
   3.1, 3.1, 3.0, 2.9, 2.6, 2.3, 1.9, 1.5, 1.7};
std::vector<float> cms_xexe_5p44_ncoll = { 
  7.522, 11.61, 17.63, 26.38, 38.38, 55.2, 77.91, 107.6,
  145.9, 195.2, 257.9, 337.3, 438.7, 563.8, 724.1, 922};
std::vector<float> cms_xexe_5p44_ncollerr = {
   0.998, 1.72, 2.92, 4.46, 6.6, 9.03, 12.7,
   16.1, 20.1, 24.6, 29.1, 34.2, 38.5, 42.4, 48.3, 57.1};
auto cms_xexe_5p44_2a = 258.;
auto cms_xexe_5p44_size = cms_xexe_5p44_raw.size();

// https://twiki.cern.ch/twiki/bin/viewauth/CMS/Glauber5360GeVPbPb
std::vector<float> cms_pbpb_5p36_raw = {
   31.6, 48.4, 73.1, 106.2, 150.9, 206.9, 277.2, 358.9,
   462.6, 584.4, 724.5, 886.8, 1093.2, 1337.5, 1636.4, 2031.8};
std::vector<float> cms_pbpb_5p36_rawerr = {
   1.6, 2.2, 3.2, 3.9, 5.8, 7.4, 9.6, 13.3,
   18.4, 22.9, 25.7, 32.0, 42.6, 48.5, 64.6, 91.1};
std::vector<float> cms_pbpb_5p36_cent = {
   22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5, 57.5,
   62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5};
std::vector<float> cms_pbpb_5p36_npart = {
   13.1, 18.8, 26.3, 35.7, 47.2, 61.1, 77.4, 96.3,
   118.2, 143.2, 171.7, 204.1, 241.0, 283.3, 331.3, 382.3};
std::vector<float> cms_pbpb_5p36_nparterr = {
   0.4, 0.6, 0.8, 0.9, 1.2, 1.2, 1.3, 1.3,
   1.4, 1.5, 1.5, 1.5, 1.4, 1.4, 1.3, 1.6};
std::vector<float> cms_pbpb_5p36_ncoll = { 
   12.89, 20.59, 32.51, 49.82, 74.60, 109.0, 155.4, 216.5,
   295.0, 394.4, 518.3, 671.1, 859.4, 1094, 1388, 1770};
std::vector<float> cms_pbpb_5p36_ncollerr = {
   0.59, 1.14, 1.83, 2.69, 4.08, 5.1, 6.6, 8.3,
   10.4, 12.6, 15.3, 18.3, 22.3, 27., 34., 42.};
auto cms_pbpb_5p36_2a = 416.;
auto cms_pbpb_5p36_size = cms_pbpb_5p36_raw.size();

// https://arxiv.org/abs/2504.02505
std::vector<float> alice_pbpb_5p36_raw = {
   23.2, 53, 110, 205, 351, 559, 848, 1253, 1668, 2047};
std::vector<float> alice_pbpb_5p36_rawerr = {
   2.8, 5, 8, 11, 14, 19, 25, 33, 42, 54};
std::vector<float> alice_pbpb_5p36_cent = {
   15, 25, 35, 45, 55, 65, 75, 85, 92.5, 97.5};
std::vector<float> alice_pbpb_5p36_npart = {
   6.8, 15.4, 30.5, 53.7, 86.5, 130.6, 188.4, 263.1, 332.4, 383.6};
std::vector<float> alice_pbpb_5p36_nparterr = {
   0.2, 0.4, 0.8, 1.1, 1.5, 1.4, 1.3, 1.3, 1.7, 0.8};
std::vector<float> alice_pbpb_5p36_ncoll = { // ! placeholder
   6.8, 15.4, 30.5, 53.7, 86.5, 130.6, 188.4, 263.1, 332.4, 383.6};
std::vector<float> alice_pbpb_5p36_ncollerr = {
   0.2, 0.4, 0.8, 1.1, 1.5, 1.4, 1.3, 1.3, 1.7, 0.8};
auto alice_pbpb_5p36_2a = 416.;
auto alice_pbpb_5p36_size = alice_pbpb_5p36_raw.size();

// https://indico.cern.ch/event/1479384/contributions/6663047/attachments/3131557/5555439/IS_mult_v3.pdf
std::vector<float> alice_oo_5p36_raw =
  // 60-90, 50-60, 40-50, 30-40,  20-30,  10-20,  5-10,   0-5
  { 10.231, 26.851,   37.822, 51.075, 67.082, 87.105, 106.833, 129.450 };
std::vector<float> alice_oo_5p36_rawerr =
  // { 0.23, 0.39, 0.57, 0.81, 1.13, 1.55, 2.08, 2.76, 3.44, 4.23 } ;
  {1.451, 2.303, 2.909, 3.498, 4.070, 4.318, 4.299, 4.897 };
std::vector<float> alice_oo_5p36_cent =
  { 25, 45, 55, 65, 75, 85, 92.5, 97.5 };
std::vector<float> alice_oo_5p36_npart =
  // 80-90, 70-80, 60-70, 50-60, 40-50, 30-40,  20-30,  10-20,  5-10,   0-5
  {  4.276, 7.371, 9.874, 13.060, 16.884, 21.025, 24.075, 26.328 }; // alice
  // {  0,     0,     6.271, 8.24,  10.82, 14.02,  17.70,  21.51,  24.251, 26.419 }; // cms
std::vector<float> alice_oo_5p36_nparterr = std::vector<float>(alice_oo_5p36_raw.size(), 0);
std::vector<float> alice_oo_5p36_ncoll = alice_oo_5p36_npart;
std::vector<float> alice_oo_5p36_ncollerr = std::vector<float>(alice_oo_5p36_raw.size(), 0);
auto alice_oo_5p36_2a = 32.;
auto alice_oo_5p36_size = alice_oo_5p36_raw.size();

GENERATE(cms_pbpb_2p76)
GENERATE(alice_pbpb_2p76)
GENERATE(alice_pbpb_5p02)
GENERATE(phobos_auau_0p2)
GENERATE(phobos_auau_0p13)
GENERATE(phobos_auau_0p0624)
GENERATE(phobos_auau_0p0196)
GENERATE(phobos_cucu_0p2)
GENERATE(phobos_cucu_0p0624)
GENERATE(phobos_cucu_0p0224)
GENERATE(brahms_auau_0p2)
GENERATE(alice_xexe_5p44)
GENERATE(cms_xexe_5p44)
GENERATE(cms_pbpb_5p36)
GENERATE(alice_pbpb_5p36)
GENERATE(alice_oo_5p36)

std::vector<float> alice_oo_5p36_eta_raw =
  { 39.9, 39.7, 39.7, 39.4, 39.1, 38.9, 38.7, 38.6, 38.4, 38.3, 38.3, 38.4, 38.5, 38.7, 38.9, 39.1, 39.3, 39.6, 39.9, 40.1 };
std::vector<float> alice_oo_5p36_eta_rawerr =
  { 4.0, 4.1, 4.0, 4.0, 4.0, 4.0, 3.9, 3.9, 3.9, 3.9, 3.9, 3.9, 3.9, 3.9, 3.9, 3.9, 4.0, 4.0, 4.1, 4.1 };
std::vector<float> alice_oo_5p36_eta_cent =
  { -0.95, -0.85, -0.75, -0.65, -0.55, -0.45, -0.35, -0.25, -0.15, -0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95 };
std::vector<float> alice_oo_5p36_eta_npart = std::vector<float>(alice_oo_5p36_eta_cent.size(), 0);
std::vector<float> alice_oo_5p36_eta_nparterr = std::vector<float>(alice_oo_5p36_eta_raw.size(), 0);
std::vector<float> alice_oo_5p36_eta_ncoll = alice_oo_5p36_eta_npart;
std::vector<float> alice_oo_5p36_eta_ncollerr = std::vector<float>(alice_oo_5p36_eta_raw.size(), 0);
auto alice_oo_5p36_eta_2a = 32.;
auto alice_oo_5p36_eta_size = alice_oo_5p36_eta_raw.size();

GENERATE(alice_oo_5p36_eta)
