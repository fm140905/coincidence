## Clone
- Install `root`, `make` and `git`.
- Open a new terminal and run `git clone xx`.
- `cd` into the repo that you have just cloned.
- Run `make clean && make` to compile the program.

## Test
- Open a new terminal under the root of the repo that you have just cloned, and run 
    ```bash
    cd test
    ./runTest.sh
    ./compareOutput.sh
    ```
-  Terminal shows `Test passed`. Output files are written to the `test/output` directory.


## Run
- Copy the `bin/` diretory and `input.txt` to anywhere you like.
- Edit the settings in `input.txt` if necessary.
- Run `bin/main $(path-to-input.txt)`.