## Example usage (2025 Low pile-up pp)
- Set up
```
cmsrel CMSSW_15_0_15_patch4
cd CMSSW_15_0_15_patch4/src
cmsenv
git clone -b pixelmin https://github.com/boundino/tracklet.git
ln -s tracklet/cmssw/Arboretum .
scram b -j4
```
- Generate pixel hit trees
The example input is a temporary streamer file and may not be available permanently.
You can use your own input **RAW** data instead.
Make sure the CMSSW release, era and GT configured in `pixeldata_pre_minimal_cfg.py` correspond to the input data.
```
cp Arboretum/PixelPlant/test/pixeldata_pre_minimal_cfg.py .
cmsRun pixeldata_pre_minimal_cfg.py
```
- Draw rechit maps
```
cd tracklet/analysis/
./hh.sh
```
The output would be in `figspdf/` for PDFs and `figs/` for PNGs.