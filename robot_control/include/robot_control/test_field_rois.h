#ifndef TEST_FIELD_ROIS_H
#define TEST_FIELD_ROIS_H

/*ROI.eMap = 100.0; // precached (0)
ROI.sMap = 35.0;
ROI.sampleProb = 1.0;
ROI.sampleSig = 10.0;
ROI.radialAxis = 7.5;
ROI.tangentialAxis = 7.5;
ROI.allocatedTime = 210.0;
ROI.highRisk = 0;
ROI.hardLockout = false;
ROI.roiGroup = 0;
ROI.whiteProb = 1.0;
ROI.silverProb = 1.0;
ROI.blueOrPurpleProb = 0.0;
ROI.pinkProb = 0.0;
ROI.redProb = 0.0;
ROI.orangeProb = 0.0;
ROI.yellowProb = 0.0;
regionsOfInterest.push_back(ROI);*/
ROI.eMap = 120.0; // 1
ROI.sMap = 70.0;
ROI.sampleProb = 0.5;
ROI.sampleSig = 1.0;
ROI.radialAxis = 5.5;
ROI.tangentialAxis = 5.5;
ROI.allocatedTime = 210.0;
ROI.highRisk = 0;
ROI.hardLockout = false;
ROI.roiGroup = 1;
ROI.whiteProb = 0.2;
ROI.silverProb = 0.2;
ROI.blueOrPurpleProb = 0.2;
ROI.pinkProb = 0.07;
ROI.redProb = 0.07;
ROI.orangeProb = 0.07;
ROI.yellowProb = 0.0;
regionsOfInterest.push_back(ROI);

#endif // TEST_FIELD_ROIS_H
