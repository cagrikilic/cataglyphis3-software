#ifndef EVANSDALE_FULL_ROIS_H
#define EVANSDALE_FULL_ROIS_H

ROI.eMap = 170.5; // 0
ROI.sMap = 138.4;
ROI.sampleProb = 1.0 - pow(9.0/10.0,10.0);
ROI.sampleSig = 1.0;
ROI.radialAxis = 7.5;
ROI.tangentialAxis = 7.5;
ROI.allocatedTime = 180.0;
ROI.highRisk = 0;
ROI.hardLockout = false;
ROI.roiGroup = 0;
ROI.whiteProb = 0.2;
ROI.silverProb = 0.2;
ROI.blueOrPurpleProb = 0.2;
ROI.pinkProb = 0.07;
ROI.redProb = 0.07;
ROI.orangeProb = 0.07;
ROI.yellowProb = 0.07;
regionsOfInterest.push_back(ROI);

ROI.eMap = 136.7; // 1
ROI.sMap = 135.3;
ROI.sampleProb = 1.0 - pow(9.0/10.0,10.0);
ROI.sampleSig = 1.0;
ROI.radialAxis = 7.5;
ROI.tangentialAxis = 7.5;
ROI.allocatedTime = 180.0;
ROI.highRisk = 0;
ROI.hardLockout = false;
ROI.roiGroup = 0;
ROI.whiteProb = 0.2;
ROI.silverProb = 0.2;
ROI.blueOrPurpleProb = 0.2;
ROI.pinkProb = 0.07;
ROI.redProb = 0.07;
ROI.orangeProb = 0.07;
ROI.yellowProb = 0.07;
regionsOfInterest.push_back(ROI);

northTransformROIs();

#endif // EVANSDALE_FULL_ROIS_H