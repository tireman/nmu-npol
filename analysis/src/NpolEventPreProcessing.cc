

#include "NpolEventPreProcessing.hh"

double NpolEventPreProcessing::NpolAng = 0.48869; /*radians; angle of NPOL relative to beam axis*/

NpolEventPreProcessing *NpolEventPreProcessing::PreProcess = NULL;

NpolEventPreProcessing *NpolEventPreProcessing::GetInstance() {
	if(PreProcess == NULL)
		PreProcess = new NpolEventPreProcessing();

	return PreProcess;
}

NpolEventPreProcessing::NpolEventPreProcessing(){}

NpolEventPreProcessing::~NpolEventPreProcessing(){}

int NpolEventPreProcessing::GetAVNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
	int underscorePos = volName.find_first_of("_",3);
	return atoi(volName.substr(3,underscorePos-3).c_str());
  } else{
	return -1;
  }
}

int NpolEventPreProcessing::GetImprNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscorePos = volName.find_first_of("_",1+
											  volName.find_first_of("_",3));
    return atoi(volName.substr(underscorePos+1,1).c_str());
  } else
    return -1;
}

int NpolEventPreProcessing::GetPlacementNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscorePos = volName.find_first_of("_",1+
      volName.find_first_of("_",1+
      volName.find_first_of("_",1+
      volName.find_first_of("_",1+
      volName.find_first_of("_",3)))));
    return atoi(volName.substr(underscorePos+1,std::string::npos).c_str());
  } else
    return -1;
}

void  NpolEventPreProcessing::AnalyzerTaggerHitPosition(double hPos[],double lPos[], int detNums[]){
  double AnalyzerOffset = 700.; // (cm)
  double DetSpacing = 65.; // (cm)
  double TaggerOffset = 693.; // (cm)
  
  do{
	hPos[0] = lPos[0] + rand->Gaus(0.0,2.0);
  } while (TMath::Abs(hPos[0]) > 50.0);
  hPos[1] = rand->Uniform(0,10)-5; // (cm)
  hPos[2] = rand->Uniform(0,10)-5; // (cm)
  RotateNpolToG4(hPos, NpolAng);
  
  if((detNums[0] == 9) || (detNums[0] == 11)) hPos[1] = -25 + 10.*float(detNums[2]) +  hPos[1];
  if((detNums[0] == 10) || (detNums[0] == 12)) hPos[1] = -35 + 10.*float(detNums[2]) +  hPos[1];
  
  if((detNums[0] == 9) && (detNums[1] == 1)) hPos[2] = hPos[2] + AnalyzerOffset + 0 * DetSpacing;
  if((detNums[0] == 9) && (detNums[1] == 2)) hPos[2] = hPos[2] + AnalyzerOffset + 1 * DetSpacing;
  if((detNums[0] == 10) && (detNums[1] == 1)) hPos[2] = hPos[2] + AnalyzerOffset + 2 * DetSpacing;
  if((detNums[0] == 10) && (detNums[1] == 2)) hPos[2] = hPos[2] + AnalyzerOffset + 3 * DetSpacing;
  if((detNums[0] == 11) && (detNums[1] == 1)) hPos[2] = hPos[2] + TaggerOffset + 0 * DetSpacing;
  if((detNums[0] == 11) && (detNums[1] == 2)) hPos[2] = hPos[2] + TaggerOffset + 0 * DetSpacing;
  if((detNums[0] == 12) && (detNums[1] == 1)) hPos[2] = hPos[2] + TaggerOffset + 0 * DetSpacing;
  if((detNums[0] == 12) && (detNums[1] == 2)) hPos[2] = hPos[2] + TaggerOffset + 0 * DetSpacing;
  RotateNpolToG4(hPos, NpolAng);
  return;
}

void  NpolEventPreProcessing::DeltaEarrayHitPosition(double hPos[],double lPos[], int detNums[]){
  
  double VertOffSet = 42.0;
  
  do {
	hPos[0] = lPos[0] + rand->Gaus(0.0, 2.0);
  } while (TMath::Abs(hPos[0]) > 80.0);
  hPos[1] = rand->Uniform(0,10)-5; // (cm)
  hPos[2] = rand->Uniform(0,10)-5; // (cm)
  RotateNpolToG4(hPos, NpolAng);

  if(detNums[0] == 3) hPos[1] = hPos[1] + VertOffSet;
  if(detNums[0] == 4) hPos[1] = hPos[1] + VertOffSet + 10.0;
  if(detNums[0] == 7) hPos[1] = hPos[1] + (-VertOffSet);
  if(detNums[0] == 8) hPos[1] = hPos[1] + (-(VertOffSet + 10.0));
  
  if((detNums[0] == 3) || (detNums[0] == 7)) hPos[2] = hPos[2] + 700. + (13. - (detNums[2] + 1)) * 10.;
  if((detNums[0] == 4) || (detNums[0] == 8)) hPos[2] = hPos[2] + 830. + (14. - (detNums[2] + 1)) * 10.;
  RotateNpolToG4(hPos, NpolAng);
  return;
}

void NpolEventPreProcessing::EarrayHitPosition(double hPos[],double lPos[], int detNums[]){
  double NDetStandardLength = 100.0;  // (cm)
  double NDetThickness = 10.0; // (cm)
  double EarrayRotAngle = 45.0 *TMath::Pi()/180.; // Erray rotation angle
  double VertOffSet = (NDetStandardLength + 60.0)/2 * sin(EarrayRotAngle) + 40.0; // 40*cm offset from geometry
  double VertOffSet2 = VertOffSet + 10.0;
  double HorOffSet = (NDetStandardLength + 60.0)/2 * cos(EarrayRotAngle) + NDetThickness/2 * sin(EarrayRotAngle);
  
  //double VertOffSet = 90.0;
  //double HorOffSet = 60.2;
  
  do {
	hPos[0] = lPos[0] + rand->Gaus(0.0, 2.0);
  } while (TMath::Abs(hPos[0]) > 80.0);
  hPos[1] = rand->Uniform(0,10)-5; // (cm)
  hPos[2] = rand->Uniform(0,10)-5; // (cm)
  RotateDetToNpol(hPos,detNums);
 
  if(((detNums[0] == 1) || (detNums[0] == 2)) && (detNums[1] == 1)) hPos[0] = hPos[0] + HorOffSet;
  if(((detNums[0] == 5) || (detNums[0] == 6)) && (detNums[1] == 1)) hPos[0] = hPos[0] + HorOffSet;
  if(((detNums[0] == 1) || (detNums[0] == 2)) && (detNums[1] == 2)) hPos[0] = hPos[0] - HorOffSet;
  if(((detNums[0] == 5) || (detNums[0] == 6)) && (detNums[1] == 2)) hPos[0] = hPos[0] - HorOffSet;
  
  RotateNpolToG4(hPos, NpolAng);
  
  if(detNums[0] == 1) hPos[1] = hPos[1] + VertOffSet;
  if(detNums[0] == 2) hPos[1] = hPos[1] + (VertOffSet2);
  if(detNums[0] == 5) hPos[1] = hPos[1] - VertOffSet;
  if(detNums[0] == 6) hPos[1] = hPos[1] - (VertOffSet2);

  if((detNums[0] == 1) || (detNums[0] == 5)) hPos[2] = hPos[2] + 700. + (13. - (detNums[2] + 1)) * 10.;
  if((detNums[0] == 2) || (detNums[0] == 6)) hPos[2] = hPos[2] + 830. + (14. - (detNums[2] + 1)) * 10.;
  
  RotateNpolToG4(hPos, NpolAng);
 
  return;
}

void  NpolEventPreProcessing::RotateNpolToG4(double hPos[], double RotAng){
  // Rotate the hit point by RotAng (generally NpolAng) so hits in NPOL volumes are in the sim's G4 coordinates
  double tempPos[3] = { hPos[0], hPos[1], hPos[2] };
  double RightAng = TMath::Pi()/2;
  
  hPos[0] = TMath::Cos(RotAng) * tempPos[0] + TMath::Cos(RightAng) * tempPos[1] + TMath::Cos(RightAng+RotAng) * tempPos[2];
  hPos[1] = TMath::Cos(RightAng) * tempPos[0] + TMath::Cos(0) * tempPos[1] + TMath::Cos(RightAng) * tempPos[2];
  hPos[2] = TMath::Cos(RightAng-RotAng) * tempPos[0] + TMath::Cos(RightAng) * tempPos[1] + TMath::Cos(RotAng) * tempPos[2];

  return;
}

void  NpolEventPreProcessing::RotateDetToNpol(double hPos[], int detNums[]){
  // A 45 degree rotation to move E-array hit positions into the NPOL coordinates
  double tempPos[3] = { hPos[0], hPos[1], hPos[2] };
  double DetAng = TMath::Pi()/4;
  double RightAng = TMath::Pi()/2;
 
  if(((detNums[0] == 1) || (detNums[0] == 2)) && (detNums[1] == 2)) DetAng = -1*DetAng;
  if(((detNums[0] == 5) || (detNums[0] == 6)) && (detNums[1] == 1)) DetAng = -1*DetAng;
   
  hPos[0] = TMath::Cos(DetAng) * tempPos[0] + TMath::Cos(RightAng - DetAng) * tempPos[1] + TMath::Cos(RightAng) * tempPos[2];
  hPos[1] = TMath::Cos(RightAng + DetAng) * tempPos[0] + TMath::Cos(DetAng) * tempPos[1] + TMath::Cos(RightAng) * tempPos[2];
  hPos[2] = TMath::Cos(RightAng) * tempPos[0] + TMath::Cos(RightAng) * tempPos[1] + TMath::Cos(0) * tempPos[2];
  
  return;
}

