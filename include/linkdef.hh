/* See https://root.cern.ch/selecting-dictionary-entries-linkdefh */
#ifdef __CINT__
/* Appending the '+' requests the 'new' I/O */
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class NpolTagger+ ;
#pragma link C++ class NpolVertex+ ;
#pragma link C++ class NpolStatistics+ ;

#pragma link C++ class vector<NpolVertex*>+;
#pragma link C++ class vector<NpolTagger*>+;

#endif

