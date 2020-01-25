# Hybrid genetic algorithm for job shop scheduling problem

This project implements a hybrid genetic algorithm for the job scheduling problem
[link to paper](https://github.com/kwh44/hybrid_genetic_algorithm_JSP/blob/master/A_hybrid_genetic_algorithm_for_the_job_shop_scheduling_problem.pdf)  
The project was developed under GNU/Linux 64-bit OS.

#### Requirements
- CMake >= 3.0</li>
- make - a GNU tool for generating executables</li>
- C++ compiler supporting C++11 standard</li>
- JSON for Modern C++  3.2 or newer
    - Short guide how to install this library on linux machine:
    - Clone repository:
    ```bash
    $ git clone https://github.com/nlohmann/json
    ```
    - Change your working directory:
    ```bash
    $ cd json
    ```
    - Build the library
    ```bash 
       $ mkdir build
       $ cd build
       $ cmake ../
       $ make install
     ```
#### How to build the project
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
```
#### How to run the project
```bash
$ ./hybrid_algorithm 
``` 
The above command starts the algorithm working on default dataset. To change the working dataset from dataset.json - choose the desired dataset index passed as a parameter in the run_test_case function, called from main.
