#!/bin/csh

foreach i (`seq $1 1 $2`)

jkill $i

end
