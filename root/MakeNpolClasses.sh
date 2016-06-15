#! /bin/bash

cp ../include/NpolVertex.hh .
cp ../include/NpolTagger.hh .
cp ../include/NpolStep.hh .
cp ../include/NpolStatistics.hh .
cp ../include/NpolDetectorEvent.hh .
cp ../include/linkdef.hh .
rootcling -f dict.cxx -c NpolVertex.hh NpolTagger.hh NpolStatistics.hh NpolStep.hh NpolDetectorEvent.hh linkdef.hh
g++ -std=c++11 -Wall -shared -fPIC -I$ROOTSYS/include/root -o NpolClass.so dict.cxx

