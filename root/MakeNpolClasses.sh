#! /bin/bash

cp ../include/NpolVertex.hh .
cp ../include/NpolTagger.hh .
cp ../include/NpolStep.hh .
cp ../include/NpolStatistics.hh .
cp ../include/linkdef.hh .
rootcint -f dict.cxx -c NpolVertex.hh NpolTagger.hh NpolStatistics.hh NpolStep.hh linkdef.hh
g++ -Wall -shared -fPIC -I/u/site/12gev_phys/1.2/Linux_CentOS6.5-x86_64-gcc4.4.7/root/5.34.21/root/include -o NpolClass.so dict.cxx

