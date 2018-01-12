# Update: 12-January-2017

No significant changes.  There was some clean up of the constructor files for the magnets and a geometry error in the generation of Dipole 1 was found a fixed.  Openings and positions are better determined by a few parameters. Magnetic fields are now changed with a simple change at the top of the each constructor.

The system was modified to allow for the lead curtain to be placed in three locations: in from of D1, between D1 and D2, and between D2 and the front shield wall.  The size of the lead curtain is now constant since the lead sheets come in standard sizes.  

The collimator was better computed using information from Andrei. 

Particle generation was expanded by adding more macros under which to run the simulation.  This allows for looping through runs of particles and scaling the number of events generated. We can now use the Generalized Particle Source in a script loop to generate a set of superimposed plots which are constructed by added up the events from individual particle types that are ran in their own runs. 

-- Will Tireman

# Update: 27-July-2016

Several updates and changes.  Analysis now has a file manager to take care of file names and separate if rom the analysis manager.  The field clamps for the dipole magnets have been opened up a bit more to reduce interference with the particles from the target. 

The new setup splits the simulation and analysis sections up.  The cleanBuild script attempts to build both simulation (dumped in build/simulation) and the analysis (dumped in build/analysis) all at once.  The user can then remake either simulation or analysis by cd'ing to the build diectory for that build and typing "make".  Analysis was broken out from the is simulation and can be compiled and run in fully functional ROOT analysis program.

Major change: The particle gun was replaced with a generalized particle source.  This allows for the particle generator to be changed in a Macro file rather than in the primary particle generator class.  This has also led us to be able to bias particle distributions from analysis of electron-scattering data. See README.md file for more details.

-- Will Tireman

# Update: 3-Dec-2015

A philosophy error was made with the energy.  The scheme didn't work to find the total energy deposited in a volume.  A change was made to save any event to the ROOT file that has at least 1 step in the polarimeter.  Also, step information such as volume, energy deposit in the volume, and the global time are stored in a new branch vector.  We can now sum up the total energy in each volume and determine the time at which detector fired.  This vector is sorted in time from earliest to latest in timee before the vector is saved.  

# Update: 26-Oct-2015

Clean up of the NpolShieldHut.cc file was performed.  A better positioning of the shield hut so a proper 10 cm of lead curtain could be placed right at the entrance to the shield hut.  The collimator had to be changed to account for the change in position of the shield hut to fully illuminate the front array.  All done by visual inspection.  No hard numbers are available from C-GEN so best guess was employed. Polarimeter Tagger volume was moved to be as close to the exit from the collimator as possible and resized to be only as large as necessary.

Checked the Scattering chamber to make sure the windows are large enough.  The basic check seemed to be okay, however, when specifics on the size of the chamber are available the file should be updated.

-- Will Tireman

# Update: 20-Oct-2015

After several runs on the farm, some ROOT script work, and some after thought the following changes were made.

SHMS H-bender magnet was throwing warnings on some tracks.  Investigated and found that an illegal operation between two surfaces or lines in the boolean subtraction volumes (not the HBCoil volume) is occuring.  Unable to resolve in the limited time available so the HBCoil is not placed in the world.  The rest of the H-Bender is placed which is the majority of the mass.  Little background in the polarimeter is expected from the SHMS.

SHMS tagger has been added.  A thin air volume was placed right in front of the SHMS bore via the contstructor for th SHMS HBender magnet.  This allows for "tagging" particles entering the SHMS acceptance.  Could prove to be useful later.  This data is stored in a branch under the main tree.

A new tree was added to track "statistics"  Since events that don't deposit any energy in the polarimeter are NOT recorded, it is necessary to check just how many are saved and how many are not for total calculations.  This is done in a new Tree which is only saved at the end of the run.  Other values can be added later that are run specific. There is also a version number added that should be changed when the NpolAnalysisManager undergoes changes that effect simulation output files.

The ROOT files:  The ROOT files now contain Two trees.  The first Tree is saved at every "good" event with all the vertices and has three branches.  The second tree is only saved at the end of the run and contains run specific information or statistics.

-- Will Tireman

# Update: 10-3-2015

Several Changes to the analysis section and steppingacton along with file naming "standard" ;).  Run scripts have been created as well.
1. In analysis manager/tracking action the daughters along any track are added to that mother.  This is to create a "tree" so that we can scan through it in a root script. 

2. Added in analysis manager a check to see any daughters exist at all.  If none then the electron when through the target with no production of daughters and the npolTree->fill() command is NOT executed.  The event is dumped!

3. In that same routine, we currently dump all events that don't produce at least one track in the polarimeter.  This is to reduce file size and speed up ROOT scripts.  Unfortunately, this must be done after the event has been completed so it doesn't speed up the G4 simulation.

4. A naming schema has been "adopted" by me.  Output files should have a base name like "npol" which is set in the script NMUsetuprun.sh or JLABsetuprun.csh using the env variable "NPOLBASE".  See next items. The default file name is "npol_1000_0001.root"  The first part is the NPOLBASENAME and the second part, between the undercores, is the JOBNUMBER.  The third part is the fileNumber which is set within NpolAnalysisManager during the run.

5. To run the program you have some options.  Just typing ./Npolapp npolmac will run the program and a default location and name has been set.  If you want more control, read on.  Run scripts have been added to a folder called "scripts".

   5.a.  For NMU, using "NMUjobsubmit 1 2" from the scripts folder will first source the NMUsetuprun.sh file then it will start two jobs with JOBNUMBER 1 and 2 respectively and place them in the background.  This is nice for starting multiply jobs without having to manually change file names and have many terminals open.  Putting in "12 15" will start 4 jobs with JOBNUMBERs 12, 13, 14, 15.  It will pipe all output to a text file in the output directory.  NMU can also start the job manually as long as they set the JOBNUMBER by hand otherwise it uses the above mentioned default name.  You can also source NMUsetuprun.sh manually as well after making the sensible modifcations for in that script. 
   
   5.b. For JLAB there is the default option like NMU to just launch the program with ./Npolapp npol.mac from the build directory and it will output the default name in the default folder.  You can also run the JLABsetuprun.csh script manually as well after making appropriate changes.  You will have to set the JOBNUMBER env variable manually if you want something other than the default value.  If you wish to submit to the farm system, you will need to run JLABjobscript.csh.  The action is "./JLABjobscript.csh 1 4".  Here, like the NMU one, the 1 and 4 are the first and last job you want to run for the JOBNUMBER env variable.  This script generates the jobfile (e.g. jobfile_1) which is needed for the 'jsub' command on the ifarm.  This file calls the JLABsimRunCommands.csh script which sets up the env variables using the JLABsetuprun.csh script, calls in the other stuff and then runs the program on the farm node.

Whew!  That is a lot just to push jobs to a series of machines. -- Will Tireman
 
------------------------------------------------------------------------------
# Update: 9-2-2015

Several changes to the analysis section.
1. Added TTree output in the analysis manager.  This coincides with removal the ntuple methodolgy and all the associated parts and pieces.

2. Changed from sensitive detectors and hit class to saving the vertex information (track) with the use of a vector in the TTree branch.

3. Added an end cap to the end of the downstream beam line in NpolBeamlineDown.  This cap is thin and a vacuum.  Using steppingAction class, we kill all tracks that get to this point.  This cleans up the tracks from portions that were not going to effect the background in the neutron detectors.

4. Added a NpolAnalysisMessenger class in attempt to generate a macro command 'setFileName' which would allow for setting the file name of the TFile from the Macro.  This didn't work but it is close ... needs checked.

-- Will Tireman

-------------------------------------------------------------------------------

# Update: 7-12-2015

Major changes have been made to the code. Geometry parts can be added and taken away in the NpolDetectorFactory.cc file.  New geometry is created in its own detector class and added to the DetectorFactory.  A map of all the Physical Volumes is generated and stored in a file called detIDs.txt which is needed later for ROOT scripts to sort through the generated Ntuple data.  This is all done automatically by the DetectorFactory Class.  

The beam dump and SHMS horizontal bender magnet are included via GDML file thanks to code receieved from B.Waidyawansa (Hall C).  This gives our code the ability to easily take in both geometry classes and GDML files.

The scattering chamber has been updated to and extruded file.  This allows for a 1 degree vertex separation and the npol and shms windows in the chamber better line up with the size of the walls so the subtracted solid is more precision.  

40 cm LD2 target with known wall thicknesses has been simulated.

The downstream beamline has been simulated for the first several meters from the scattering chamber in detail.  Included is the vacuum gate valve, flanges, and pipe diameter changes.  The remainder of the downstream line is a simple extruded solid.

Dipole 1 (charybdis) and Dipole 2 (BNL 48D48) have been included with field clamps and copper coils.  A G4UniformMagnetic field is generated in the volume of air between the poles.

Simulated data is saved in a ROOT file and inlcudes:
  1. histograms of the total energy deposited in each polarimeter detector.
  2. ntuple file which has columns of various data for each step that passes cuts in the NpolSteppingAction.cc. This can be very large.  Currently, any tracks that get to the beam dump are killed.  Others maybe necessary for speed later on. 

ROOT scripts for data analysis have been generated.  A chain loader using TChain is employed for multi ntuples but there is a complication.  Since the volume ID is a string which can't be saved to the Ntuple, it was decided to store a volume ID number to the ntuple via a map.  This was done to increase the simpcity of the code for modification but the map is not ordered.  So that the ROOT scripts know which volume ID belongs to which physical volume, the map is written to a file named detIDs.txt (or similar) in the same directory as the ntuple.  The ROOT script has to read this as well so it can match of volumes.   Complicated, right?  If you have a better solution then email me some information and I would be very grateful.  

That's about it for now, folks.

Will Tireman

-----------------------------------------------------------------------
# Update: 12-5-2014

New branch created.  It is a new "day".

This code is being developed by the NMU collaborators for use by the Hall C GeN collaboration for background studies in Hall C.  The code used GEANT4 and is in the initial stages.  An older version exists in the branch called "retired".  This version, however, has reorganized the structure of the code so that it will be easier to modify in the future.  Included in the ease of use are individual classes for adding components to the simulation, new materials to the NpolMaterials list, and the use of a stepping action.

Will Tireman 

-----------------------------------------------------------------------
# Update: 8-24-2014

Current Status: As of 8/24/2014, the current status of the code is as follows:

1. The code projects neutrons (and other particles) from a rectangular surface the same size as the front array of detectors (60 cm high by 100 cm wide) parallel to the z-axis at a specified initial kinetic energy.

2. The code uses a detector construction via the helper class assembly volume to aid in building the repeatable structure. The simulation is setup to be the same as the current proposed geometry.

3. A first attempt at tracking code has been made beyond the standard G4Track class. This will need a lot of work.

4. Currently only the default physics list loads when the code runs. It will be necessary to investigate a user defined class is necessary or changes to the physics processes.

5. Data output is limited to a formated text file at the moment and the output location is hard coded into the NpolEventAction.cc file. The user will need to modify this as necessary.

Will Tireman

-----------------------------------------------------------------------
