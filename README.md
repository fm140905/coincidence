<!--
 * @Description: 
 * @Author: Ming Fang
 * @Date: 2021-04-06 15:24:22
 * @LastEditors: Ming Fang
 * @LastEditTime: 2022-02-28 20:56:40
-->
## Compile
- Install `root`(v6.20 or above), `cmake` and `git`.
- Open a new terminal and run 
    ```bash
    git clone https://github.com/fm140905/coincidence.git
    ```
- `cd` into the repo that you have just cloned.
- Run 
  ```bash
  mkdir -p build
  cd build
  cmake ..
  cmake --build .
  ```
  to compile the project.

## Test
- Open a new terminal under the root of the repo that you have just cloned, and run 
    ```bash
    cd test
    ./runTest.sh
    ```
-  Terminal should show `Test passed`. 
-  Output files are written to the `test/output` directory, where
    - Examples of good/rejected pulses are saved for each channel.
    - Pulse height (PH) and Pulse height distribution (PHD) are saved for each channel.
    - Pulse integral (PI) and Pulse integral distribution (PID) are saved for each channel.
    - Time stamps are saved for each channel.
    - Differences of arrival times (TOF) are saved for each channel.

## Run
 It's important that you set the correct CoMPASS version in the json input file because the binary data saved by CoMPASS V1 and V2 have different formats.
### Calibration
- Copy the executable `CsCalibration` and input file `CsCalibration.json` in `bin` folder to your working directory.
- Edit the settings in `CsCalibration.json`. Explanation of the parameters can be found in `Inuput file explained.md`.
- Under your working directory, run
    ```bash
    ./CsCalibration ${path-to-CsCalibration.json}
    ```
- A root window displays the uncalibrated/calibrated PHD and PID. Adjust the integration gates and PH/PI bins in `CsCalibration.json` until the calculated Comtpon edges are at expected positions, and the calibrated PHD and PID are roughly the same.
- Calibration coefficients are in the terminal output. 
### Processing full data
- Copy the executable `main` and input file `input.json` in `bin` folder to your working directory.
- Edit the settings in `input.json`. Explanation of the parameters can be found in `Inuput file explained.md`. Make sure you use the same integration gates and calibration coefficients in the `Calibration` step.
- Under your working directory, run
    ```bash
    ./main -i ${path-to-input.json}
    ```
    Possible command line options are:
    ```
    -h
        Output this text; Script is not executed.
    -i FILE
        Load parameters from FILE and prcoess data.
    -d
        Display results, including pulses, spectra, and PSD.
    ```
- Output are saved in text files in `output` directory.

## Extra
Please cite our paper if you use the code in this repository:

```
@article{FANG2019162507,
title = {Positron annihilation lifetime spectroscopy using fast scintillators and digital electronics},
journal = {Nuclear Instruments and Methods in Physics Research Section A: Accelerators, Spectrometers, Detectors and Associated Equipment},
volume = {943},
pages = {162507},
year = {2019},
issn = {0168-9002},
doi = {https://doi.org/10.1016/j.nima.2019.162507},
url = {https://www.sciencedirect.com/science/article/pii/S0168900219310460},
author = {M. Fang and N. Bartholomew and A. {Di Fulvio}}}
```
