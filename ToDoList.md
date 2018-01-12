# Major Issues and/or needs

1. Reworking or modification of the NPOL efficency ROOT script.  The current one is off from the proposal efficiencies by a factor of about 7.  My suspicion is that some of the difference comes from how we identify the (n,p) scattering events.  We are currenlty used the same energy cuts, however, the choice of "section" in which the hit occured and then susequent choice of detectors in the top/bottom arrays affects the calcuated angle and the energy. 

2. Implementation of QE neutron generator.  Currently, through a set of hoops we can use the output from the SimC code to feed the G4 Generalized Particle Source (GPS) with the 5 necessary histograms to bias the input for a surface right in from of the first Dipole magnet. This isn't bad but perhaps implementing our own internal QE neutron generator would be useful. 

# Minor Issues and/or needs

1. Multithreading option was partially coded in and tested in 2015/16 however a race condition was found to cause the start up to crash periodically. There was also an issue with the shutting down portion of the code which appeared to occur after the end of all worker threads.  Since each worker thread used its own output file and the master thread did not try to combine them together at the end, it did not effect the output files.  It would generate huge error output. Running locally can be dealt with using scripts to launch and monitor several instances of the simulation under similar (different run numbers) conditions or different conditions such as particle type.  If multithreading was made to work, we could see improvements on the JLAB batch farm both in memory usage and resources allocation.  Memory usage is lowered in multithreading applications and given the puny amounts of RAM per thread on the batch farm it would give some advantage and efficency.  

2. Implement TOSCA maps for the two dipole magnets.  Currently, we use the G4UniformMagField class to generate the magnetic fields.  A TOSCA map would be more realistic, however, it will result in significant time penalties for the simulation.  When it is necessary this will have to be implemented. 
