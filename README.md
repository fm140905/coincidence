<!--
 * @Description: 
 * @Author: Ming Fang
 * @Date: 2021-04-06 15:24:22
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-04-06 17:11:43
-->
## Clone
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
-  Terminal shows `Test passed`. 
-  Output files are written to the `test/output` directory, where
    - Examples of good/rejected pulses are saved for each channel.
    - Pulse height (PH) and Pulse height distribution (PHD) are saved for each channel.
    - Pulse integral (PI) and Pulse integral distribution (PID) are saved for each channel.
    - Time stamps are saved for each channel.
    - Differences of arrival times (TOF) are saved for each channel.

## Run
- Copy the executable `main` and input file `input.json` in `bin` folder to your working directory.
- Edit the settings in `input.json` if necessary. Explanation of the parameters can be found in `Inuput file explained.md`.
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
- Outfiles are saved in `output` directory.
