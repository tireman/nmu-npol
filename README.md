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
### First time  ---- See item 3 if compiling after changes are made!
1.  Run the cleanBuild script.  It will attempt to build the code from scratch.
	This new version builds an "analysis" binary and a "simulation" binary which 
    can be found in the ``build/analysis" or "build/simulation`` directories. 
  ```
    % ./cleanBuild
    % cd build/analysis
	  OR
	% cd build/simulation
  ```
  * If all goes well, then goto **Run the Simulation** below. 
  * Beware of differences between personal systems and JLAB.

### Details of the build (ie. what cleanBuild does)
1.  Create a two directories inside nmu-npol/ called build/analysis and build/simulation
	 and cd into each in turn:
  ```
    % mkdir build/analysis
    % cd build/analysis
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

	This process is repeated for the simulation just replacing "analysis" with "simulation".

3.  The resulting binary is dumped into the build directory along with a copy of
    any ``*.mac`` files specified in the CMakeLists.txt file.  The GDML files are
    also copied over.  ###WARNING#### This copying of macro files over doesn't seem to work
	when just running "make" in the build directory!  Don't know why and should be investigated.
	#####
	If you make changes to these files it is suggested you
    do it in their original locations and run 'make' in the build/analysis or
	build/simulation directory again which will copy them over even if no 
	source files had been changed. #####
    ```
    ***Note***  There generally no need to re-run the cmake command, even if
    you change CMakeLists.txt or other source files.  Just run ``make`` in the
    ``build/analysis" or "build/simulation`` directory and all your changes will 
	be picked up. If you added new classes then cmake must be rerun as cmake needs
	to find the new classes and add them to the list. 
    ```

### Environmental variable and directory setup scripts for simulation and analysis

### Macro files
	The macro files have been updated after the addition of the General Particle
	Source.  There are three main macro files but more can be added if the user
	wishes.  All macros for the simulation are found in "build/simulation/macros" 
	directory.	Data for the ParticleFlux.mac macro are in 
	"build/simulation/particleFluxData" directory.
	
	1) Electron beam on target
	File name: ElectronBeam.mac

	This macro projects a beam of electrons with circular distributions of 2 mm in the
	+z-direction starting from a position of -3 meters from the target. The user can change 
	the electron beam energy, profile, beam size, and initial position and direction.
	If so desired, the particle type can be changed but the user may wish to create a 
	new file for this purpose.  The primary objective for this macro is for electrons 
	on the 40-cm liquid target.

	2) Point Source on Polarimeter
	File name: PointSource.mac
	
	This macro was used for projecting a cone of particles, typically neutrons, onto the polarimeter
	only as tests for the polarimeter analysis code.  It uses limits on theta and phi to generate the 
	cone and rotates the momentum direction to 28 degrees to beam line right to point at the 
	polarimeter.  This is usually run only with all elements turned off except for the polarimeter.

	3) Biased particle flux generated at a "tagger" volume
	File name: ParticleFlux.mac
	Special requirement: Changes to simulation setup are needed.  First, in NpolDetectorConstruction 
	you must comment out all unnecessary elements such as beam line, Hall shell, scattering chamber.
	If generating at NPOL tagger than comment out shield hut and dipole magnets as well.  Then you 
	will need to go into NpolPolarimeter.cc, NpolParticleFluxTagger.cc, and NpolShieldHut.cc and 
	change the variable at the top called "NpolAng" from 28.0 degrees to 0.0 degrees.  This is 
	necessary in order to get the theta biasing correct.

	This macro was developed to bias the event generator with distributions of particles at a 
	"tagger" volume. The tagger volumes are placed in the full simulation with electron beam on target
	to track particles initially crossing those volumes as a way to determine the flux of various 
	particles through those surfaces. The tagger volumes are as follows.  
	
	"Target tagger" is a thin volume placed very close to the entrance of Dipole 1 (Charybdis). 
	The idea is to capture flux data from electron scattering in the target, scattering chamber, 
	beam line elements, etc. for future analysis.

	"Npol tagger" is a thin volume placed very close to the front wall of the polarimeter shield hut
	on the inside of the shield hut.  This tracks particles that manage to get through both magnets, 
	the collimator, and the lead curtain.

	"SHMS tagger" is a thin volume placed very close to the horizontal bender magnets entrance.

	To run with the biased particle flux, the user has to comment/uncomment the appropriate lines 
	in the macro file.  They also have to choose the correct particle flux data files on the lines
	loading in the histogram points.  For example, if you wish to generate particles at the NPOL
	tagger, then uncomment all lines for NPOL tagger source setup and the /control/execute "filename"
	lines.  Then make sure the files being loaded are for the particle you are studying.  For example, 
	if you want to look at neutrons on the polarimeter you would make sure all files loading have the 
	word "neutron" in the name. Also make sure you have set the primary particle to neutron as well
	at the top of the macro. 

### Run the simulation
1.  You can run in batch mode in one of two ways both on the Jlab Farm or on a
    local machine.

    1.A. You can run in batch mode on a local machine by modifying the appropriate macro file. 
	You could also modify the NMUsetuprun.sh script to match your plans, however, the simulation
	will default to a local output directory and file name if this is not done.
	Then run by: 
  ```
    % cd build/simulation				# if not already in this directory
    % source script/NMUsetuprun.sh    	# sets up output dir and name (optional); 
      	     			     		  	# change as needed
    % ./Npolapp macros/ElectronBeam.mac	# choose your macro file from above or create your own
  ```
	If you want to run a set of runs on a muticore machine, use the script 
	NMUjobsubmit.sh.  From the build directory:
  ```
    % cd build/simulation				# if not already in this directory
    % ./scripts/NMUjobsubmit.sh i j 	# 'i' is the first job number and 
      				  	 		  		# 'j' is the number of jobs you want to run
										# jobs will be put in the background
  ```

	#### NOTES on NMUjobsubmit.sh script #### 
	Jobs will be placed in the background and run to completion if you use the job submission
	script.  If you use i=1 and j=1 only 1 job will start and it will have jobnumber = 1.  
	If you use i=11 and j = 6 then 6 jobs will start (j=6) with the first JOBNUMBER set to 11 
	and the last will be 16.

    1.B. If on the JLAB farm, one can run as above in interactive mode just 
    	 remember to source the production.csh script on the Jlab farm system
	 before attempting to compile and run.  Remember to run only tests on the JLAB interactive
	 farm.  Play nice in the sandbox.

	 If you wish to run on the batch farm nodes, then a job submission 
	 script needs to create a jobsub file. You may need to modify the job
	 submission script (JLABjobscript.csh) to generate jobsub files with 
	 the correct information (name, job, etc.). Then modify the 
	 JLABsetuprun.csh script which sets directories and env. variables
	 as necessary. Then, to submit a number of jobs to the farm nodes
	 do the following:
  ```
    % cd build/simulation/scripts	# do this after you have completed a build
    % ./JLABjobscript.csh i j
  ```

	This script will generate the jobsub file, submit the job, pause
	for 1 seconds (take a breath), and then generate the next one and 
	submit until all have been submitted between i and j where i is the 
	first job number and j is the last job number with i < j.  Note that
	these job numbers are used in the ROOT file identification so you 
	MUST watch your file names and job numbers. You could overwrite previous runs.
	User beware!
	 

2.  For visualization, you can modify and run another ``*.mac`` file or run
    ``./Npolapp`` by itself on the command line.  You can rotate, move, and
    change various elements of the visualized geometry with the mouse.
	By running ./Npolapp from the build/simulation directory it automatically 
	runs the init_vis.mac file which calls the vis.mac file in build/simulation/macros.
	You can make changes in vis.mac such as background color, rotation, etc.

3.  The "data" are outputted in a root file as per NpolAnalysisManager.  You will
    have to change the directory to which the data file(s) are written via the 
	ENV variables in the NMUsetuprun.sh or JLABsetuprun.csh script.

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
