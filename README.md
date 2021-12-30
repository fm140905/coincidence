<!--
 * @Description: 
 * @Author: Ming Fang
 * @Date: 2021-04-06 15:24:22
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-04-06 17:11:43
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
