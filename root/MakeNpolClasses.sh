#! /bin/bash

cp ../include/NpolVertex.hh .
cp ../include/NpolTagger.hh .
cp ../include/NpolStep.hh .
cp ../include/NpolStatistics.hh .
cp ../include/linkdef.hh .
rootcint -f dict.cxx -c NpolVertex.hh NpolTagger.hh NpolStatistics.hh NpolStep.hh linkdef.hh
g++ -Wall -shared -fPIC -I/usr/local/include/root -o NpolClass.so dict.cxx

