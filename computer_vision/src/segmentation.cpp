#include <computer_vision/segmentation.hpp>

Segmentation::Segmentation()
{
	segmentationServ = nh.advertiseService("/vision/segmentation/segmentimage", &Segmentation::segmentImage, this);
}

//load segmentation lookup table from file
int Segmentation::loadLookupTable(std::string filename)
{
	//allocate memory for lookup table
	G_lookup_table = new unsigned char**[256];
	for (int i = 0; i < 256; ++i) 
	{
		G_lookup_table[i] = new unsigned char*[256];
		for (int j = 0; j < 256; ++j)
		{
			G_lookup_table[i][j] = new unsigned char[256];
		}
	}	

	//open file containing lookup table
	std::ifstream inputFile;
	inputFile.open(filename.c_str());
	if(!inputFile)
	{
		ROS_INFO("Error! Failed to open file %s!", filename.c_str());
		return -1;
	}

	//load data in the lookup table
	int element = 0;
	for (int i=0; i<256; i++) //R
	{
		for (int j=0; j<256; j++) //G
		{
			for (int k=0; k<256; k++) //B
			{
				if(inputFile.eof())
				{
					ROS_INFO("Error! Unexpected end of file while accessing element (%i, %i) in file %s", i, j, filename.c_str());
					return -2;
				}
				else
				{
					inputFile >> element;
					G_lookup_table[i][j][k] = element;	
				}
			}
		}
	}

	//close file
	inputFile.close();

	return 1;
}

//assign colors from lookup table (recommended method by opencv for scanning image)
void Segmentation::assign_colors(cv::Mat& I, uchar*** &lut) 
{
    CV_Assert(I.depth() == CV_8U);

    int channels = I.channels();

    int nRows = I.rows;
    int nCols = I.cols * channels;

    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i,j;
    int r, g, b;
    uchar* p;

    for( i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for ( j = 0; j < nCols-3; j=j+3)
        {
        	p[j] = lut[p[j+2]][p[j+1]][p[j]];
        }
    }
}

//find rectangles for blobs in segmented image
std::vector<cv::Rect> Segmentation::getIndividualBlobs(const cv::Mat& segmented)
{
    // takes in a masked image (black and white) and returns a list of possible objects (blobs) in the scene
    cv::Mat copy = segmented.clone();
    
    assert(segmented.empty() != true);
    segmented.convertTo(segmented, CV_8U);

    const int min_width = 30;
    const int max_width = 325;
    const int min_height = 30;
    const int max_height = 313;

    const int min_area = (min_width-10) * (min_height-10);
    const int max_area = (max_width+10) * (max_height+10);

    cv::SimpleBlobDetector::Params params;
    
    params.minDistBetweenBlobs = 100.0f;
    params.maxThreshold = 255;
    params.minThreshold = 200;
    
    params.filterByConvexity = false;
    params.filterByColor = false;
    params.filterByInertia = false;
    params.filterByCircularity = false;
    params.filterByArea = true;
    
    params.minArea = (float) min_area;
    params.maxArea = (float) max_area;

    cv::SimpleBlobDetector blob_detector(params);

    std::vector<cv::KeyPoint> keyPoints;
    // finding contours requires
    blob_detector.detect(segmented, keyPoints);

    // get contours and draw contours
    std::vector<std::vector<cv::Point> > contourPoints;
    findContours(segmented, contourPoints, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::Mat contouredImage(segmented.size(),CV_8U,cv::Scalar(255));
    
    // maximum contour length
    const int maxContourLength = 2250; 
    // minimum contour length
    const int minContourLength = 15;

    std::vector<std::vector<cv::Point> >:: iterator itContours = contourPoints.begin();
    
    while (itContours!=contourPoints.end())
    {
        if (itContours->size() < minContourLength || itContours->size() > maxContourLength)
        {
            itContours= contourPoints.erase(itContours);
        }
        else
        {
            ++itContours;
        }
    }
    drawContours(contouredImage, contourPoints, -1, cv::Scalar(0), 8);
    cv::Rect boundingBoxonBlob;
    std::vector<cv::Rect> blob_list;
    cv::Mat r1;
    for(int i = 0;i<contourPoints.size();i++)
    {
        boundingBoxonBlob = boundingRect(cv::Mat(contourPoints[i]));

        //ROS_INFO("segmented.cols, segmented.rows = %i, %i", segmented.cols, segmented.rows);

        //shift coordinates to be relative to center of image
        float centerx = (boundingBoxonBlob.br().x - boundingBoxonBlob.tl().x)/2 - segmented.rows/2;
        float centery = (boundingBoxonBlob.br().y - boundingBoxonBlob.tl().y)/2 - segmented.cols/2;

        //remove blobs that might be inside the robot
        if(centerx > 2100/5792 && centerx < 3700/5792 && centerx>2150)
        {
        	//ignore blob
        	ROS_INFO("ignoring blob %i",i);
        	continue;
        }

        //remove blobs that are too far from center
        if(centerx*centerx+centery*centery < (2.0/3.0*segmented.rows/2)*(2.0/3.0*segmented.cols/2) )
        {
        	//ignore blob
        	ROS_INFO("ignoring blob %i",i);
        	continue;
        }

        blob_list.push_back(boundingBoxonBlob);
    }
    
    keyPoints.clear();

    return blob_list;
}

//write the segmented blobs to file
int Segmentation::writeSegmentsToFile(std::vector<cv::Rect> rectangles, const cv::Mat& origional)
{
    for(int i = 0; i<rectangles.size(); i++)
    {
        //enlarge bounding box for each blob
        int tl_x, tl_y, br_x, br_y;
        tl_x = rectangles[i].tl().x - (float)rectangles[i].width*(50.0/100.0);
        tl_y = rectangles[i].tl().y - (float)rectangles[i].height*(50.0/100.0);
        br_x = rectangles[i].br().x + (float)rectangles[i].width*(50.0/100.0);
        br_y = rectangles[i].br().y + (float)rectangles[i].height*(50.0/100.0);

        //trim parts of bounding box outside of the image
        if(tl_x < 0) tl_x = 0;
        if(tl_y < 0) tl_y = 0;
        if(br_x > origional.cols) br_x = origional.cols;
        if(br_y > origional.rows) br_y = origional.rows; 
        cv::Rect extended_rect = cv::Rect( cv::Point(tl_x,tl_y), cv::Point(br_x,br_y) );

        //resize image (150x150)

        //write image of blob to file
		//boost::filesystem::path P( ros::package::getPath("computer_vision") );
        //imwrite(P.string() + "/segments/segment" + patch::to_string(i+1) + ".JPG", origional( extended_rect ) );
    }

    return 1;
}

//callback function for segmentating image into blobs
bool Segmentation::segmentImage(computer_vision::SegmentImage::Request &req, computer_vision::SegmentImage::Response &res)
{

	if(capture.capture_image()!=1)
	{
		return false;
	}

	cv::Mat image_file = capture.image_Mat; //cv::imread("/home/atlas/cataglyphis_ws/src/computer_vision/img1.JPG");
	cv::Mat image_file_copy = image_file.clone();

   /*
        COLOR CLASSIFICATION
    */

	//assign color to each pixel
    //double t;
    //t = get_wall_time();

    assign_colors(image_file, G_lookup_table); //replace blue channel with integer value representing the color ()

    //t = (get_wall_time() - t)*1000;
    //cout << "Time for assigning pixel color: " << t << " milliseconds for " << (double)(image.rows*image.cols)/1000000 << " megapixels."<< endl;

	/*
		SEGMENTATION
	*/

    //extract blue channel from image (blue channel contains pixel colors)
	//t = get_wall_time();

	std::vector<cv::Mat> channels(3);
	split(image_file, channels);
	cv::multiply(channels[0],cv::Scalar(255),channels[0]);

    //t = (get_wall_time() - t)*1000;
    //cout << "Time to extract blue channel: " << t << " milliseconds." << endl;

	//remove noise using morphological operations
	//t = get_wall_time();

	cv::Mat erodeElement = getStructuringElement(cv::MORPH_CROSS, cv::Size(8, 8)); //TODO: the size of the erode and dilate elements should be a function of the camera parameters and resolution
	cv::Mat dilateElement = getStructuringElement(cv::MORPH_RECT, cv::Size(8, 8));

    erode(channels[0], channels[0], erodeElement);
    erode(channels[0], channels[0], erodeElement);

    dilate(channels[0], channels[0], dilateElement);
    dilate(channels[0], channels[0], dilateElement);

    //t = (get_wall_time() - t)*1000;
    //cout << "Time perform morphological operations: " << t << " milliseconds." << endl;

    //extract blobs from mask
 	//t = get_wall_time();

    std::vector<cv::Rect> rectangles = getIndividualBlobs(channels[0]);

    //t = (get_wall_time() - t)*1000;
    //cout << "Time extract blobs from mask: " << t << " milliseconds." << endl;

    writeSegmentsToFile(rectangles, image_file_copy);

	res.number_of_segments = rectangles.size();
	return true;
}