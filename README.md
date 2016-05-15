# bin2text
This software aims at providing a solution to the following problem:

## THE GATES FOUNDATION - BINARY TO TEXT SOFTWARE PERFORMANCE CHALLENGE IN C

You can read the full challenge overview [here](https://www.topcoder.com/challenge-details/30053925/?type=develop) website or as [plain text](CHALLENGE_OVERVIEW.txt).


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisities

This project was developed and tested on x86\_64 GNU/Linux (Linux 4.5.0) with the following configuration

```
gcc-5.3.0 
glibc-2.32
gmake-4.1
```

This project has no dependencies on 3rd party libraries other than those shipped with gcc-5.3.0.


### Installing

Clone the repository to your preferred directory:

```
#!bash
[user@hostname]~$ git clone <URL> && cd bin2text
```

There are three build target configurations available: all, debug, clean [default: all]. Invoke the build system with gmake:

```
#!bash
[user@hostname]~bin2text$ make [target]
```

## Usage

The program is intended as a command line application, whose usage is as follows:

```
#!bash
Usage:   run [options] <input1> <input2>
Options:
         -n INT    number of sorted tuples for out1.txt and out2.txt [default: 0]
         -k INT    number of sorted tuples for out3.txt              [default: 0]
         -b FLOAT  bin width for out4.txt                            [default: 0]
         -s FLOAT  number of standard deviations for out5.txt
```

Input:

- 2 binary files, **<input1>** and **<input2>** format described below
- 1 unsigned int **n** (4 bytes, 0 <= n <= 1,000,000)
- 1 unsigned int **k** (4 bytes, 0 <= k <= 100,000)
- 1 double **b** (8 bytes, 0 <= b <= 1)

Optional:

- 1 double **s** (8 bytes, -100,000 <= s <= 100,000)

**<input1>** in a binary file with the following representation:

```
#!text
type          | bytes       | field     | description
-----------------------------------------------------
signed int    | 4           | 0         | zero
signed int    | 4           | d         | dimension (# ints in each tuple) 
signed int    | 4           | n_vars    | number of variables total
unsigned long | 8           | n_tups    | number of tuples
double        | 8           | avg       | average of all scores
----------------------------------------------------------------
              | 28 bytes total
```

The program asserts that **<input1>** is 28 bytes as one of the safety checks before main execution.

**<input2>** is a binary file containing the data needed to run. The size of the data is determined by **<input1>**.
It has the following representation:

```
#!text
type          | bytes       | field     | description
-----------------------------------------------------
int[]         | d*4         | t1        | tuple 1
double        | 8           | t1_s      | tuple 1 score
int[]         | d*4         | t2        | tuple 2
double        | 8           | t2_s      | tuple 2 score
...            ...            ...        ...
int[]         | d*4         | tlast     | last tuple
double        | 8           | tlast_s   | last tuple score
-----------------------------------------------------------
              | n_tups*((4*d)+8) bytes total

```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* Dropwizard - Bla bla bla
* Maven - Maybe
* Atom - ergaerga

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [git](https://git-scm.com/) for versioning. 

## Authors

* **Thomas Legris** - [noboruma](https://github.com/noboruma)
* **Edmund Higham** - [ehigham](https://github.com/ehigham)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc