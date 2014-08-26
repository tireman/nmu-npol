Background: This software was orginally developed by Wei-Ming at Kent State University as a second simulation for the Fluka code.  The code was a mixed of code from examples novice N02 and extended A01 from the GEANT 4.9.3 release.  At the time Wei-Ming retired at Kent State, the code was given to Will Tireman at Northern Michigan University where the code was modified cleaned up to be easier to work with since the modified code contained many unused classes.  

Introduction: The nmuNpol code was developed with the express purpose of simulating the effects of neutrons projected at the neutron recoil polarimeter proposed in E11-009 and later modified by the collaboration to be a neutron-proton recoil polarimeter.  To this point the code has been modified and cleanup to run faster and provide a basis from which the collaboration can continue build upon.    

Current Status:  As of 8/24/2014, the current status of the code is as follows:

1) The code projects neutrons (and other particles) from a rectangular surface the same size as the front array of detectors parrallel to the z-axis at a specified initial kinetic energy.

2) The code uses a detector construction via the helper class assembly volume to aid in building the repeatable structure.  The simulation is setup to be the same as the current proposed geometry.

3) A first attempt at tracking code has been made beyond the standard G4Track class.  This will need a lot of work.

4) Currently only the default physics list loads when the code runs.  It will be necessary to investigate a user defined class is necessary or changes to the physics processes.  

5) Data output is limited to a formated text file at the moment.

How to run the code:  The code runs in batch and visualization mode.  The current developers have adopted using the "newer" cmake method for compiling the code.  This can be accomplished with the following steps after confirming that cmake is installed.

1) Unpack the code in the folder of your choice which I will call $PWD/nmunpol
2) Create a directory next to this one called $PWD/nmunpol-build
3) In nmunpol-build directory place the CMakeLists.txt file.  Edit as necessary.
4) In the nmunpol-build directory run the command cmake -DGeant_DIR=$G4INSTALL $PWD/nmunpol.  This will generate all the make files, etc.  Note that you must set your env variable G4INSTALL to make this work.  
5) Type 'make' in the nmunpol-build directory
6) The resulting binary is dumped into the build directory along with a copy of an *.mac files specified in the CMakeLists.txt file.  
7) You can run in batch mode by setting particle type, energy, and # of events in the npol.mac file and type './Npolapp npol.mac'.
8) For visuaulization you can modify and run another *.mac file or type './Npolapp" by itself in the command line.



