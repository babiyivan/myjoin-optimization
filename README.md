# EP-Group-12

## Original Implementation

This implementation serves as the baseline reference, without any efficiency optimizations. It is intended as a starting point for further improvements.

### Building the Program
1. Navigate to the `./original_implementation` directory.
2. Run the following command to build the program:
   ```bash
   make
   ```

### Cleaning the Build
To clean up all generated files (object files and executables) and reset repository to its original state, run:
```bash
make clean
```

### Testing Environment
This implementation has been tested on the `g0` server. To replicate the tests:
1. Clone the repository to the `g0` server.
2. Build the executable as described above.
3. Navigate to the directory `/localtmp/efficient24`:
   ```bash
   cd /localtmp/efficient24
   ```

### Comparing Output
You can compare the program's output with the expected results using the following commands:

#### Small Data Set:
```bash
~/EP-Group-12/original_implementation/myjoin a.csv b.csv c.csv d.csv | sort | diff - abcd.csv
```

#### Large Data Set:
```bash
~/EP-Group-12/original_implementation/myjoin f1.csv f2.csv f3.csv f4.csv | sort | diff - output.csv
```

### Local test

Navigate to the `./original_implementation` and to execute the program with predefined test files form `test` subdirectory, run:

```
./myjoin test/file1.csv test/file2.csv test/file3.csv test/file4.csv
```