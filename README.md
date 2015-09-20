# Introduction
The nmuNpol code was developed with the express purpose of simulating the
effects of background radiation on the Neutron-Proton Recoil polarimeter for
the [E12-11-009 (CGEN)](https://wiki.jlab.org/E12-11-009/) collaboration.


This file contains instructions on how to install and run the code.
Status Updates can be found in STATUS.md.

**WARNING:  The instructions may be incomplete.**


# How to run the code
The code runs in batch and visualization mode. The current developers have
adopted using the "newer" cmake method for compiling the code.  This can be
accomplished with the following steps after confirming that cmake is
installed.

1.  Unpack the code in the folder of your choice which I will call
    nmu-npol/.  If you are downloading it from GitHub then that directory
    will be created for you.

## Building
### First time
1.  Run the cleanBuild script.  It will attempt to build the code from scratch.
    Binaries will be found in the ``build/`` directory.
  ```
    % ./cleanBuild
    % cd build
  ```
  * If all goes well, then goto **Run the Simulation** below.

### Details of the build (ie. what cleanBuild does)
1.  Create a directory inside nmu-npol/ called build/ and cd into it:
  ```
    % mkdir build
    % cd build
  ```

2.  Run cmake in the build/ directory:
  ```
    % cmake CMakeLists.txt ../
  ```

    This will generate all the make files, etc in the build directory.
    Assuming it succeeds, you may build the executable:
  ```
    % make -j4
  ```
    _Notes_
    - The ``-j4`` tells make to run 4 parallel build threads
    - See FAQ at the bottom if ROOT/GEANT4 can't be found.

3.  The resulting binary is dumped into the build directory along with a copy of
    any ``*.mac`` files specified in the CMakeLists.txt file.  The GDML files are
    also copied over.  If you make changes to these files it is suggested you
    do it in their original locations and run 'make' in the build/ directory
    again which will copy them over even if no source files had been changed.
    ```
    ***Note***  There generally no need to re-run the cmake command, even if
    you change CMakeLists.txt or other source files.  Just run ``make`` in the
    ``build/`` directory and all your changes will be picked up.
    ```

### Run the simulation
1.  You can run in batch mode by setting particle type, energy, and number of
    events in the npol.mac file and run:
  ```
    % cd build             # if not already in this directory
    % source setuprun.sh   # sets up output dir and name; change as needed
    % ./Npolapp npol.mac
  ```

2.  For visualization, you can modify and run another ``*.mac`` file or run
    ``./Npolapp`` by itself on the command line.  You can rotate, move, and
    change various elements of the visualized geometry with the mouse.

3.  The "data" are outputted in an ntuple in a step-by-step format.  You will
    have to change the directory to which the ntuple is written as this is
    hard-coded in the npol.mac file.

4.  In order to know which volume was struck by a particular step, the volume
    name was given a number and recorded in the ntuple but this is down in the
    code via an unordered map.  This map is written to the directory of choice
    (currently the same as the ntuple) in the NpolAnalysisManager.cc file.
    This location and file name should be updated by the user.  

5.  The ntuple contains both histograms (energy deposited in each detector in
    the polarimeter for each event) and ntuple columns for each step that
    passes cuts in NpolSteppingAction.cc for each event.


# Miscellaneous Notes / FAQ:
- CMake can not find GEANT4 or ROOT:

  If you have a non-standard geant4 installation, you will need to have your
  environment properly configured so G4 can be located by the system.  This is
  typically done by sourcing the geant4.sh setup script included with the G4
  build.  For example, if your G4 installation is in /usr/local/geant4 then
  you can add the following two lines to your .bashrc file:
  ```
        G4SETUP="/usr/local/geant4/bin/geant4.sh"
        [ -r "$G4SETUP" ] && source "$G4SETUP"
  ```

  Similarly, for ROOT:
  ```
        ROOTSETUP="/usr/local/root/bin/thisroot.sh"
        [ -r "$ROOTSETUP" ] && source "$ROOTSETUP"
  ```

- This and other ``*.md`` files use markdown format.  See
  [markdown basics](https://help.github.com/articles/markdown-basics/) for details.
