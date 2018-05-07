
#include "NpolEventPreProcessing.hh"
#include "NpolEventProcessing.hh"
#include "NpolPhysicsVariables.hh"


NpolPhysicsVariables *NpolPhysicsVariables::PhysVars = NULL;

NpolPhysicsVariables *NpolPhysicsVariables::GetInstance() {
	if(PhysVars == NULL) PhysVars = new NpolPhysicsVariables();
	return PhysVars;
}

NpolPhysicsVariables::NpolPhysicsVariables(){
}

NpolPhysicsVariables::~NpolPhysicsVariables(){}

// Return the azimuthal part of the scattering angle (in degrees) in the polarimeter given
// three points in global coordinates: (p1x,p1y,p1z) - a point along the line of the
// incoming neutron's trajectory (the vertex in the target will do)
// (p2x,p2y,p2z) - the point where the n,p elastic collision occurs in the analyzer detector
// (p3x,p3y,p3z) - a point along the trajectory of the scattered proton (in the E array)
// The calculation is done by forming a triangle with these points and using the law of cosines
double NpolPhysicsVariables::getAzimuthAngle(const double p1x, const double p1y, const double p1z, const double p2x, const double p2y, const double p2z, const double p3x, const double p3y, const double p3z) {
  
  double s1, s2, s3; // the lengths of the triangle's three sides
  s1 = TMath::Sqrt(TMath::Power(p2x-p1x,2) + TMath::Power(p2y-p1y,2) + TMath::Power(p2z-p1z,2));
  s2 = TMath::Sqrt(TMath::Power(p3x-p2x,2) + TMath::Power(p3y-p2y,2) + TMath::Power(p3z-p2z,2));
  s3 = TMath::Sqrt(TMath::Power(p3x-p1x,2) + TMath::Power(p3y-p1y,2) + TMath::Power(p3z-p1z,2));

  double theta = TMath::ACos((s1*s1 + s2*s2 - s3*s3)/(2.0*s1*s2));
  return 180.0-TMath::RadToDeg()*theta;
}

// Pass an array of 3 doubles to hold the x,y,z positions, the section number and detector type identifying the array, and the detEvents map with global position set when the detector first crosses the desired energy threshold
void NpolPhysicsVariables::GetPoI(double *ret, double *time, const int section, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents) {

  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;

  ret[0] = 0.0;
  ret[1] = 0.0;
  ret[2] = 0.0;
  double totEdepSoFar = 0.0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if(Process->sectionNumber(it->first) == section && Process->detectorType(it->first) == type) {
      if(it->second->thresholdExceeded) {
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
      }
    }
  }
  
  // Compute the weighted average
  ret[0] /= totEdepSoFar;
  ret[1] /= totEdepSoFar;
  ret[2] /= totEdepSoFar;
  *time /= totEdepSoFar;
}
/*		if((it->second->totEnergyDep) > totEdepSoFar){
		  totEdepSoFar = it->second->totEnergyDep;
		  ret[0] = (it->second->hPosX);
		  ret[1] = (it->second->hPosY);
		  ret[2] = (it->second->hPosZ);
		  *time  = (it->second->time);
		}
      }
    }
  }
  }*/

void NpolPhysicsVariables::GetPoI2(double *ret, double *time, const int SOI, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents) {
  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  ret[0] = 0.0;
  ret[1] = 0.0;
  ret[2] = 0.0;
  double totEdepSoFar = 0.0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	int section = Process->sectionNumber(it->first);
    if(section == SOI && Process->detectorType(it->first) == type) {
      if(it->second->thresholdExceeded) {
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
      }
    } else if((section == (SOI + 1)) && Process->detectorType(it->first) == type){
	  int AVNum = PProcess->GetAVNumber(it->first);
	  int PVNum = PProcess->GetPlacementNumber(it->first);
	  if((SOI == 0) && ((AVNum == 1) || (AVNum == 5)) && ((PVNum >= 3) && (PVNum <= 5))){
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
	  } else if((SOI == 1) && ((AVNum == 2) || (AVNum == 6)) && ((PVNum >= 11) && (PVNum <= 13))){
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
	  } else if((SOI == 2) && ((AVNum == 2) || (AVNum == 6)) && ((PVNum >= 4) && (PVNum <= 6))){
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
	  }
	}
  }
  
  // Compute the weighted average
  ret[0] /= totEdepSoFar;
  ret[1] /= totEdepSoFar;
  ret[2] /= totEdepSoFar;
  *time /= totEdepSoFar;
}

/*  //This code looks for the highest energy detector and uses it.
		if((it->second->totEnergyDep) > totEdepSoFar){
		  totEdepSoFar = it->second->totEnergyDep;
		  ret[0] = (it->second->hPosX);
		  ret[1] = (it->second->hPosY);
		  ret[2] = (it->second->hPosZ);
		  *time  = (it->second->time);
		}
      }
    }
  }
  }*/


// Return the Azimuth angle only.  Check is in main code.
// Optionally return delta time-of-flight between the analyzer and E-array
double NpolPhysicsVariables::ReturnAngle(NpolVertex *incNeutronVert, std::map<std::string,NpolDetectorEvent *> *detEvents, const int SOI, const PolarimeterDetector EArrayOfInterest, double *dTOF = NULL) {
  double targetPt[3];
  double analyzerPt[3];
  double EarrayPt[3];
  double analyzerTime;
  double EarrayTime;

  targetPt[0] = incNeutronVert->posX;
  targetPt[1] = incNeutronVert->posY;
  targetPt[2] = incNeutronVert->posZ;

  GetPoI(analyzerPt,&analyzerTime,SOI,analyzer,detEvents);
  GetPoI2(EarrayPt,&EarrayTime,SOI,EArrayOfInterest,detEvents);

  double azAngle = getAzimuthAngle(targetPt[0],targetPt[1], targetPt[2],
								   analyzerPt[0],analyzerPt[1],analyzerPt[2],
								   EarrayPt[0],EarrayPt[1],EarrayPt[2]);

  if(dTOF != NULL)
    *dTOF = EarrayTime - analyzerTime;
  return azAngle;
}

double NpolPhysicsVariables::getEDepCluster(const std::map<std::string,NpolDetectorEvent *> *detEvents, int SOI, PolarimeterDetector detectorOfInterest) {
  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  
  double totEnergyDeposit = 0.0;
  double highEnergyDeposit = 0.0;
  std::string highestDetector = "";
  
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	if((SOI ==  Process->sectionNumber(it->first)) && (detectorOfInterest == Process->detectorType(it->first))) {
	  if((it->second->totEnergyDep) > highEnergyDeposit) {
		highestDetector = it->first;
		highEnergyDeposit = it->second->totEnergyDep;
	  }
	}
  }
  
  std::map<std::string,NpolDetectorEvent *>::const_iterator itt;
  int highAVNum = PProcess->GetAVNumber(highestDetector);
  int highImprNum = PProcess->GetImprNumber(highestDetector);
  int highPVNum = PProcess->GetPlacementNumber(highestDetector);
  for(itt = detEvents->begin(); itt != detEvents->end(); itt++) {
	if(detectorOfInterest == analyzer || detectorOfInterest == topdEArray || detectorOfInterest == botdEArray){
	  if((SOI ==  Process->sectionNumber(itt->first)) && (detectorOfInterest == Process->detectorType(itt->first))) {
		if(PProcess->GetAVNumber(itt->first) == highAVNum){
		  if(PProcess->GetImprNumber(itt->first) == highImprNum){
			int PVNum = PProcess->GetPlacementNumber(itt->first);
			if((PVNum == highPVNum) || (PVNum == (highPVNum + 1)) || (PVNum == (highPVNum - 1))){
			  totEnergyDeposit += itt->second->totEnergyDep;
			}
		  }
		}
	  }
	} else if(detectorOfInterest == topEArray || detectorOfInterest == botEArray){
	  if(((SOI ==  Process->sectionNumber(itt->first)) || ((SOI + 1) ==  Process->sectionNumber(itt->first))) && (detectorOfInterest == Process->detectorType(itt->first))) {
		if(PProcess->GetAVNumber(itt->first) == highAVNum){
		  if(PProcess->GetImprNumber(itt->first) == highImprNum){
			int PVNum = PProcess->GetPlacementNumber(itt->first);
			if((PVNum == highPVNum) || (PVNum == (highPVNum + 2)) || (PVNum == (highPVNum - 2))){
			  totEnergyDeposit += itt->second->totEnergyDep;
			}
		  }
		}
	  } 
	}
  }
  
  return totEnergyDeposit;
}
  
double NpolPhysicsVariables::getTotalEnergyDep(const std::map<std::string,NpolDetectorEvent *> *detEvents) {

  double totEnergyDeposit = -1.0;
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((it->second->totEnergyDep) > 0.0)  {
      totEnergyDeposit += it->second->totEnergyDep;
    }
  }

  return totEnergyDeposit;
}

double NpolPhysicsVariables::highestEDepPV(const std::map<std::string,NpolDetectorEvent *> *detEvents, int sectionOfInterest, PolarimeterDetector detectorOfInterest) {
  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  
  double highestE_Dep = -1.0;

  std::map<std::string,NpolDetectorEvent *>::const_iterator it;

  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((sectionOfInterest == Process->sectionNumber(it->first)) && (detectorOfInterest == Process->detectorType(it->first))) { 
      if((it->second->totEnergyDep) > highestE_Dep) 
		highestE_Dep = it->second->totEnergyDep;

    }
  }
  return highestE_Dep;
}
