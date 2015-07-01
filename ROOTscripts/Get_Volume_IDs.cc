#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <errno.h>

#define BUFSZ 1024

// Read the file output by the Geant4 simulation that maps a unique ID
// to each volume in the simulation and fill a STL map with this information.
void Fill_Map(std::map<int, const char *>& aMap) {

	const char *filename = "/data/dwilbern/detIDs.txt";
	FILE *volIDsFile = fopen(filename, "r");
	if(volIDsFile == NULL) {
		perror("Error opening file containing volume IDs");
		exit(1);
	}

	char buf[BUFSZ];
	while(true) {
		if(!fgets(buf,BUFSZ,volIDsFile))
			break;

		int id = atoi(strtok(buf,","));
		char *volName = strtok(NULL,"\n");
		aMap[id] = strdup(volName);
	}

	fclose(volIDsFile);
}

int main() {

	std::map<int, const char *> volIDsMap;
	std::map<int, const char *>::iterator it;

	Fill_Map(volIDsMap);

	// example of iterating through the map
	for(it = volIDsMap.begin(); it != volIDsMap.end(); it++) {
		printf("Volume %s has ID %d.\n", it->second, it->first);
	}

	// example of looking up and ID in the map
	int someID = 9;
	it = volIDsMap.find(someID);
	if(it == volIDsMap.end())
		printf("No detector with ID %d is in the map.\n", someID);
	else
		printf("ID %d is mapped to %s.\n", someID, it->second);

	return 0;
}
