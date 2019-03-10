# Gambit Chess Engine

[![Build Status](https://travis-ci.org/looak/Gambit.svg?branch=master)](https://travis-ci.org/looak/Gambit)

I got frustrated about how bad I was playing chess, so I decided to write myself a chess engine and let the computer do it for me! Not intending to use it against unaware non engine players.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

This is a C++ project and I'm not distributing any binaries, so to run the engine you need to compile it yourself. 

### Installing and Running
Through gitbash or ui clone repository

```bash
git clone --recursive https://github.com/looak/Gambit.git
```

#### Windows
Easiest way to get running in Windows is installing Visual Studio Code and extensions for cmake projects, or Visual Studio Community and opening the initial CMakeLists.txt.

#### Linux

* Create a build directory:
```bash
mkdir build
cd build
```
* Call Cmake:
```bash
cmake ..
```
Build:
```
make -j8
```

## Running Gambit

Interfacing with GambitEngine is done through GambitCLI.

## Running the tests

`GambitTest` should have been output after build step on your platform, Visual Studio will should automatically discover the tests and they can be run through the IDE. Code can be configured to run the tests in the task.json or other ways.


## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Alexander Loodin Ek** - *Initial work* - [looak](https://github.com/looak)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the GNU GPL v3.0 - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* **Melanie Pieber** - *My Lovely Partner*
* **Aaton & Preston** - *Our Annoying Cats*