# Major Issues and/or needs

1. Reworking or modification of the NPOL efficency ROOT script progressing slowly.  WT found the "factor of 7" which was in a mistake made in the Section of Interest (SOI) section.  With this bug corrected, work has progressed on the proton track ID which still needs a lot of work/testing.

2. Implementation of QE neutron generator.  Tongtong (Post-Doc with Dr. Michael Kohl) has written up and tested a C++ (compiled against ROOT) program to generate QE neutron events.  This has yet to be implemented into our code.  In the future, this will prove useful and more compact than the current method. 

# Minor Issues and/or needs

1. Multithreading option was partially coded in and tested in 2015/16 however a race condition was found to cause the start up to crash periodically. There was also an issue with the shutting down portion of the code which appeared to occur after the end of all worker threads.  Since each worker thread used its own output file and the master thread did not try to combine them together at the end, it did not effect the output files.  It would generate huge error output. Running locally can be dealt with using scripts to launch and monitor several instances of the simulation under similar (different run numbers) conditions or different conditions such as particle type.  If multithreading was made to work, we could see improvements on the JLAB batch farm both in memory usage and resources allocation.  Memory usage is lowered in multithreading applications and given the puny amounts of RAM per thread on the batch farm it would give some advantage and efficency.  

2. Implement TOSCA maps for the two dipole magnets.  Currently, we use the G4UniformMagField class to generate the magnetic fields.  A TOSCA map would be more realistic, however, it will result in significant time penalties for the simulation.  When it is necessary this will have to be implemented. 
