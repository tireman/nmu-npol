
#include "NpolEventProcessing.hh"
#include "NpolEventPreProcessing.hh"

//NpolEventPreProcessing PreData;

/*MeV This threshold is a per detector low value in SOI selection */
double NpolEventProcessing::LOW_THRESHOLD = 0.040;
/* number of analyzer layers; not general; only good for 4 and 6 layers */
int NpolEventProcessing::LAYER_NUM = 4; 


NpolEventProcessing *NpolEventProcessing::Process = NULL;

NpolEventProcessing *NpolEventProcessing::GetInstance() {
	if(Process == NULL)
		Process = new NpolEventProcessing();

	return Process;
}


NpolEventProcessing::NpolEventProcessing(){}

NpolEventProcessing::~NpolEventProcessing(){}

// The NPOL polarimeter is divided into 4 or 6 sections.  This function takes
// a volume name and returns the section number that the detector belongs to,
// or -1 if the volume does not belong in the polarimeter.
int NpolEventProcessing::sectionNumber(const std::string &volName) {
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  
  int avNum, imprNum, pvNum;
  if((avNum = PProcess->GetAVNumber(volName)) == -1) return -1;
  
  if(LAYER_NUM == 4){
    switch(avNum) {
    case 1: // Top E array 1 
    case 5: // Bottom E array 1
    case 3: // Top dE array 1 
    case 7: // Bottom dE array 1 
      pvNum =  PProcess->GetPlacementNumber(volName);
      if((pvNum <= 12) && (pvNum >= 6)) {return 0;}  // section 1
      else if((pvNum <= 5) && (pvNum >= 0)) {return 1;}  // section 2
      else {return -1;}
    case 2: // Top E array 2
	case 6: // Bottom E array 2
    case 4: // Top dE array 2
	case 8: // Bottom dE array 2
      pvNum =  PProcess->GetPlacementNumber(volName);
      if((pvNum <= 13) && (pvNum >= 7)) {return 2;}  // section 3
      else if((pvNum <= 6) && (pvNum >= 0)) {return 3;}  // section 4
      else {return -1;}
    case 9: // Front array 1
    case 11: // Front Veto array 1
      imprNum =  PProcess->GetImprNumber(volName);
      if(imprNum == 1) return 0;  // section 1
      else if(imprNum == 2) return 1;  // section 2
      else return -1;
    case 10: // Front array 2
    case 12: // Front Veto array 2
      imprNum =  PProcess->GetImprNumber(volName);
      if(imprNum == 1) return 2;  // section 3
      else if(imprNum == 2) return 3;  // section 4
      else return -1;
    default:
      return -1;
    }
  }else if(LAYER_NUM == 6){
    switch(avNum) {
    case 1: // Top E array 1 & 2
    case 5: // Bottom E array 1 & 2
      pvNum =  PProcess->GetPlacementNumber(volName);
      imprNum =  PProcess->GetImprNumber(volName);
      if((imprNum == 1) || (imprNum == 2)){
		if(pvNum <= 12 && pvNum >= 6) return 0;  // section 1
		else if(pvNum <= 5 && pvNum >= 0) return 1;  // section 2
      }else if((imprNum == 3) || (imprNum == 4)){
		if(pvNum <= 12 && pvNum >= 6) return 2;  // section 3
		else if(pvNum <= 5 && pvNum >= 0) return 3;  // section 4
      }else{
		return -1;
      }
    case 3: // Top dE array 1 & 2
    case 7: // Bottom dE array 1 & 2
      pvNum =  PProcess->GetPlacementNumber(volName);
      imprNum =  PProcess->GetImprNumber(volName);
      if(imprNum == 1){
		if(pvNum <= 12 && pvNum >= 6) return 0;  // section 1
		else if(pvNum <= 5 && pvNum >= 0) return 1;  // section 2
      }else if(imprNum == 2){
		if(pvNum <= 12 && pvNum >= 6) return 2;  // section 3
		else if(pvNum <= 5 && pvNum >= 0) return 3;  // section 4
      }else{
		return -1;
      }
    case 2: // Top E array 3
    case 4: // Top dE array 3
    case 6: // Bottom E array 3
    case 8: // Bottom dE array 3
      pvNum =  PProcess->GetPlacementNumber(volName);
      if(pvNum <= 13 && pvNum >= 6) return 4;  // section 5
      else if(pvNum <= 5 && pvNum >= 0) return 5;  // section 6
      else return -1;
    case 9: // Front array 1
    case 12: // Front tag array 1
      imprNum =  PProcess->GetImprNumber(volName);
      if(imprNum == 1) return 0;  // section 1
      else if(imprNum == 2) return 1;  // section 2
      else return -1;
    case 10: // Front array 2
    case 13: // Front tag array 2
      imprNum =  PProcess->GetImprNumber(volName);
      if(imprNum == 1) return 2;  // section 3
      else if(imprNum == 2) return 3;  // section 4
      else return -1;
    case 11:  // Front array 3
    case 14:  // Front tag array 4
      imprNum =  PProcess->GetImprNumber(volName);
      if(imprNum == 1) return 4;  // section 5
      else if(imprNum == 2) return 5;  // section 6
      else return -1;
    default:
      return -1;
    }
  } else { return -1;
  }
}

// This method returns the NPOL detector "type" which is defined as the following:
// analyzer = detector in 1 of the 4 vertically stacked arrays making up the analyzer sections
// tagger = detector in the arrays in front of each analyzer section
// topEArray = any detector in the top E-array detectors.  There are 2 imprints per section with 2 sections (4 total)
// botEArray = any detector in the bottom E-array detectors.  There are 2 imprints per section with 2 sections (4 total)
// topdEArray = any detector in the top dE-array detectors.  There are 2 imprints per section with 2 sections (4 total)
// botdEArray = any detector in the bottom dE-array detectors.  There are 2 imprints per section with 2 sections (4 total)
PolarimeterDetector NpolEventProcessing::detectorType(const std::string &volName) {
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  
  int avNum = PProcess->GetAVNumber(volName);
  if(LAYER_NUM == 4){
    switch(avNum) {
    case 1: case 2: return topEArray;
    case 5: case 6: return botEArray;
    case 3: case 4: return topdEArray;
    case 7: case 8: return botdEArray;
    case 9: case 10: return analyzer;
    case 11: case 12: return tagger;
    case 13: return backPlane;
    default: return unknown;
    }
  }else if(LAYER_NUM == 6){
    switch(avNum) {
    case 1: case 2: return topEArray;
    case 5: case 6: return botEArray;
    case 3: case 4: return topdEArray;
    case 7: case 8: return botdEArray;
    case 9: case 10: case 11: return analyzer;
    case 12: case 13: case 14: return tagger;
    case 15: return backPlane;
    default: return unknown;
    }
  }else{
	return unknown;
  }
}

// Return the frontmost polarimeter section that passes requirements 1 and 2.
// If -1 is returned, then no section passed requirements 1 and 2.
int NpolEventProcessing::getSectionOfInterest(const std::map<std::string,NpolDetectorEvent *> *detEvents) {
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  int sectionOfInterest = -1;
  int totalDetHit = 0; int AVNum;
  for(int section = (LAYER_NUM - 1); section >= 0; section--) {
    std::map<std::string,NpolDetectorEvent *>::const_iterator it;
	bool analyzerFlag = false;
	bool topdEArrayFlag = false;
	bool botdEArrayFlag = false;
	bool taggerFlag = false;
	for(it = detEvents->begin(); it != detEvents->end(); it++) {
	  if(sectionNumber(it->first) == section) {
		AVNum =  PProcess->GetAVNumber(it->first);
		if((AVNum >= 1) && (AVNum <=12)){
		  if(it->second->totEnergyDep >= LOW_THRESHOLD) totalDetHit++; 
		}
		switch(detectorType(it->first)) {
		case analyzer: analyzerFlag |= it->second->thresholdExceeded == true;
		  break;
		case tagger: taggerFlag |= it->second->thresholdExceeded == true; 
		  break;
		case topdEArray: topdEArrayFlag |= it->second->thresholdExceeded == true; 
		  break;
		case botdEArray: botdEArrayFlag |= it->second->thresholdExceeded == true; 
		  break;
		default:
		  break;
		}
	  }
	}
	if( ((analyzerFlag) /*&& (!taggerFlag)*/) && ((topdEArrayFlag) || (botdEArrayFlag)) ) sectionOfInterest = section;
  }

  // With the preliminaary selection of SOI, now check the E-arrays in the SOI and SOI+1 for at least 1 hit
  if(sectionOfInterest != -1){
	int firstSection = sectionOfInterest;
	int secondSection;
	bool topEArrayFlag = false;
	bool botEArrayFlag = false;
	if(sectionOfInterest == (LAYER_NUM - 1)){
	  secondSection = LAYER_NUM - 1;
	} else {
	  secondSection = sectionOfInterest + 1;
	}
	for(int section = firstSection; section <= secondSection; section++) {
	  std::map<std::string,NpolDetectorEvent *>::const_iterator E_it;
	  for(E_it = detEvents->begin(); E_it != detEvents->end(); E_it++) {
		if(sectionNumber(E_it->first) == section) {
		  switch(detectorType(E_it->first)) {
		  case topEArray: topEArrayFlag |= E_it->second->thresholdExceeded == true; 
			break;
		  case botEArray: botEArrayFlag |= E_it->second->thresholdExceeded == true; 
			break;
		  default:
			break;
		  }
		}
	  }
	}
	if(!((topEArrayFlag) || (botEArrayFlag))) sectionOfInterest = -1;
  }

  // Reject events with more than 40 detectors with hits above 0.040 MeV; Otherwise return the ID'd section of interest
  if((sectionOfInterest != -1) && (totalDetHit >= 40)) { 
	std::cout << "Event #  Rejected! Total number of detectors with 40 keV or greater: " << totalDetHit << std::endl;
	  sectionOfInterest = -1;
  }
  
  return sectionOfInterest;
}

PolarimeterDetector NpolEventProcessing::getEArrayOfInterest(std::map<std::string,NpolDetectorEvent *> *detEvents, std::map<PolarimeterDetector, double> *eDepArrayTotal, int SOI) {
  double topETotal = (*eDepArrayTotal)[topEArray];
  double botETotal = (*eDepArrayTotal)[botEArray];
  double topdETotal = (*eDepArrayTotal)[topdEArray];
  double botdETotal = (*eDepArrayTotal)[botdEArray];

  PolarimeterDetector EOI = unknown;
  double totalTop = topETotal + topdETotal;
  double totalBot = botETotal + botdETotal;
  double topR = totalTop/totalBot;
  double botR = totalBot/totalTop;
  bool topEFlag = false;
  bool botEFlag = false;
  bool topdEFlag = false;
  bool botdEFlag = false;
  
  if((topETotal >= 5) && (topETotal <= 150)) topEFlag = true;  // energy units of MeV
  if((botETotal >= 5) && (botETotal <= 150)) botEFlag = true;  // energy units of MeV
  if((topdETotal >= 1) && (topdETotal <= 25)) topdEFlag = true;  // energy units of MeV
  if((botdETotal >= 1) && (botdETotal <= 25)) botdEFlag = true;  // energy units of MeV
  
  if(topEFlag && /*(topR >= 20) &&*/ !(botEFlag)){
    if(topdEFlag && !(botdEFlag)) EOI = topEArray;
  } else if(!(topEFlag) && botEFlag /*&& (botR >= 20)*/){
    if(!(topdEFlag) && botdEFlag) EOI = botEArray;
  } else {
    EOI = unknown;
	//std::cout << "Failed to detect top or bottom E-array" << std::endl;
  }

  return EOI;
}

void NpolEventProcessing::getEDepArrayTotal(std::map<std::string,NpolDetectorEvent *> *detEvents, std::map<PolarimeterDetector, double> *eDepArrayTotal, int SOI){
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  
  std::map<std::string,NpolDetectorEvent *>::iterator e_it;
  for(e_it = detEvents->begin(); e_it != detEvents->end(); e_it++) {	
	int section = sectionNumber(e_it->first);
	PolarimeterDetector detector = detectorType(e_it->first);
	if(section == SOI) {
	  if((detector == analyzer) || (detector == tagger) || (detector == topdEArray)
		 || (detector == botdEArray) || (detector == topEArray) || (detector == botEArray)){
		(*eDepArrayTotal)[detector] += e_it->second->totEnergyDep;
	  }
	}
	
	if(section == (SOI + 1)){
	  int AVNum = PProcess->GetAVNumber(e_it->first);
	  int PVNum = PProcess->GetPlacementNumber(e_it->first);
	  if(section == 1){
		if((detector == topdEArray) || (detector == botdEArray)){
		  if((AVNum == 3) || (AVNum == 7)){
			if((PVNum == 5)){
			  (*eDepArrayTotal)[detector] += e_it->second->totEnergyDep;
			}
		  }
		} else if((detector == topEArray) || (detector == botEArray)){
		  if((AVNum == 1) || (AVNum == 5)){
			if((PVNum >= 3) && (PVNum <= 5)){
			  (*eDepArrayTotal)[detector] += e_it->second->totEnergyDep;
			} 
		  }
		}
	  } else if(section == 2){
		if((detector == topdEArray) || (detector == botdEArray)){
		  if((AVNum == 4) || (AVNum == 8)){
			if((PVNum == 13)){
			  (*eDepArrayTotal)[detector] += e_it->second->totEnergyDep;
			}
		  }
		} else if((detector == topEArray) || (detector == botEArray)){
		  if((AVNum == 2) || (AVNum == 6)){
			if((PVNum >= 11) && (PVNum <= 13)){
			  (*eDepArrayTotal)[detector] += e_it->second->totEnergyDep;
			} 
		  }
		}
	  } else if(section == 3){
		if((detector == topdEArray) || (detector == botdEArray)){
		  if((AVNum == 4) || (AVNum == 8)){
			if((PVNum == 6)){
			  (*eDepArrayTotal)[detector] += e_it->second->totEnergyDep;
			}
		  }
		} else if((detector == topEArray) || (detector == botEArray)){
		  if((AVNum == 2) || (AVNum == 6)){
			if((PVNum >= 4) && (PVNum <= 6)){
			  (*eDepArrayTotal)[detector] += e_it->second->totEnergyDep;
			} 
		  }
		}
	  }
	}
  }
  return;
}

bool NpolEventProcessing::checkEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents){
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  int countTop = 0;
  int countBottom = 0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	if((Process->detectorType(it->first) == topEArray) && (it->second->thresholdExceeded == true)) countTop++;
	if((Process->detectorType(it->first) == botEArray) && (it->second->thresholdExceeded == true)) countBottom++;
  }
  std::cout << "Number Top E-Array Detectors 'Hit' =  " << countTop << std::endl;
  std::cout << "Number Bottom E-Array Detectors 'Hit' =  " << countBottom << std::endl;
  return true;
}

bool NpolEventProcessing::checkdEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents){
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  int countTop = 0;
  int countBottom = 0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	if((Process->detectorType(it->first) == topdEArray) && (it->second->thresholdExceeded == true)) countTop++;
	if((Process->detectorType(it->first) == botdEArray) && (it->second->thresholdExceeded == true)) countBottom++;
  }
  std::cout << "Number Top dE-Array Detectors 'Hit' =  " << countTop << std::endl;
  std::cout << "Number Bottom dE-Array Detectors 'Hit' =  " << countBottom << std::endl;
  return true;
}
