## Clone
- Install `root`, `make` and `git`.
- Open a new terminal and run 
    ```bash
    git clone https://github.com/fm140905/coincidence.git
    ```
- `cd` into the repo that you have just cloned.
- Run 
  ```bash
  make clean && make
  ```
  to compile.

## Test
- Open a new terminal under the root of the repo that you have just cloned, and run 
    ```bash
    cd test
    ./runTest.sh
    ./compareOutput.sh
    ```
-  Terminal shows `Test passed`. Output files are written to the `test/output` directory, where
    - `timestamp_DIACFD_CH*` and `timestamp_Header_CH*` stores the pulse arrival times calculated using our `DIACFD` algorithm and by the digitizer (leading edge or CFD, depending on the acquisition settings).
    - `timediff_DIACFD_CH*` and `timediff_Header_Ch*` stores the differences of the arrival times of two coincident events, calculated using two methods.
    - `TOF_$(fraction)_$(delta)_$(number-of-interpolation-points).txt` saves the histogram of the differences of pulse arrival times calculated using `DIACFD` method.


## Run
- Copy the `bin/` diretory and `input.txt` to anywhere you like.
- Edit the settings in `input.txt` if necessary. Explanation of the parameters can be found [here!](https://gitlab.engr.illinois.edu/nml/pals/-/blob/master/README.md)
- Run 
    ```bash
    bin/main $(path-to-input.txt)
    ```
- Outfiles are saved in `output` directory.
