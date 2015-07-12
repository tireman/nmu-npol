Status Updates can be found in STATUS.md.  This file contains instructions on how to install and run the code.  WARNING:  The instructions may be complete.

Introduction: The nmuNpol code was developed with the express purpose of simulating the effects of background radiation on the Neutron-Proton Recoil polarimeter for the E12-11-009 (CGEN) collaboration.

How to run the code: The code runs in batch and visualization mode. The current developers have adopted using the "newer" cmake method for compiling the code. This can be accomplished with the following steps after confirming that cmake is installed.

1) Unpack the code in the folder of your choice which I will call $PWD/nmu-npol. If you are downloading it from GitHub then this will all be done for you. 

2) Create a directory next $PWD/nmu-npol called $PWD/nmu-npol-build

3) In nmu-npol-build directory place the CMakeLists.txt file. Edit as necessary.

4) In the nmu-npol-build directory run the command 'cmake -DGeant_DIR=$G4INSTALL $PWD/nmu-npol'. This will generate all the make files, etc. Note that you must set your env variable G4INSTALL to make this work or type in the directory in place of G4INSTALL.

5) Type 'make' in the nmu-npol-build directory

6) The resulting binary is dumped into the build directory along with a copy of any *.mac files specified in the CMakeLists.txt file.  The GDML files are also copied over.  If you make changes to these files it is suggested you do it in their original locations and run 'make' again which will copy them over even if no source files had been changed.

7) You can run in batch mode by setting particle type, energy, and # of events in the npol.mac file and type './Npolapp npol.mac'.

8) For visuaulization you can modify and run another *.mac file or type './Npolapp" by itself in the command line.  You can rotate, move, and chane various elements of the visualized geometry with the mouse.

9) The "data" is outputted in an ntuple in a step-by-step format.  You will have to change the directory to which the ntuple is writen as this is hardcoded in the npol.mac file.

10) In order to know which volume was struck by a particular step, the volume name was given a number and recorded in the ntuple but this is down in the code via an unordered map.  This map is written to the directory of choice (currently the same as the ntuple) in the NpolAnalysisManager.cc file.  This location and file name should be updated by the user.  

11) The ntuple contians both histograms (energy deposited in each detector in the polarimeter for each event) and ntuple columns for each step that passes cuts in NpolSteppingAction.cc for each event.