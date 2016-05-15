# bin2text
This software aims at providing a solution to the following problem:

## THE GATES FOUNDATION - BINARY TO TEXT SOFTWARE PERFORMANCE CHALLENGE IN C

You can read the full challenge overview at [this](https://www.topcoder.com/challenge-details/30053925/?type=develop) website or as [plain text](CHALLENGE_OVERVIEW.txt)


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

The usage of the program is as follows:

```
#!bash
Usage:   run [options] <input1> <input2>
Options:
         -n INT    number of sorted tuples for out1.txt and out2.txt
         -k INT    number of sorted tuples for out3.txt
         -b FLOAT  bin width for out4.txt
         -s FLOAT  number of standard deviations for out5.txt
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