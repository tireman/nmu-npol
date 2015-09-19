# Update: 9-2-2015

Several changes to the analysis section.
1. Added TTree output in the analysis manager.  This coincides with removal the ntuple methodolgy and all the associated parts and pieces.

2. Changed from sensitive detectors and hit class to saving the vertex information (track) with the use of a vector in the TTree branch.

3. Added an end cap to the end of the downstream beam line in NpolBeamlineDown.  This cap is thin and a vacuum.  Using steppingAction class, we kill all tracks that get to this point.  This cleans up the tracks from portions that were not going to effect the background in the neutron detectors.

4. Added a NpolAnalysisMessenger class in attempt to generate a macro command 'setFileName' which would allow for setting the file name of the TFile from the Macro.  This didn't work but it is close ... needs checked.

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
