/* See https://root.cern.ch/selecting-dictionary-entries-linkdefh */
#ifdef __CINT__
//#ifdef __CLING__
/* Appending the '+' requests the 'new' I/O */
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class NpolTagger+ ;
#pragma link C++ class NpolVertex+ ;
#pragma link C++ class NpolStep+;
#pragma link C++ class NpolStatistics+;
#pragma link C++ class NpolDetectorEvent+;

#pragma link C++ class std::vector<NpolVertex*>+;
#pragma link C++ class std::vector<NpolTagger*>+;
#pragma link C++ class std::vector<NpolStep*>+;
#pragma link C++ class std::vector<NpolStatistics*>+;
#pragma link C++ class std::vector<NpolDetectorEvent*>+;

#endif

