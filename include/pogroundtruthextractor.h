// Extracts ground truth from ground truth image.
// The image must be black and white (two classes). In order to extracy the
// labels, the image is gray-scaled (0..255) and thresholded at 128.
// Classes are labelled 1 if values>128 are majority within the patch
// and -1 otherwise.
// Usage example:
// PGroundTruthExtractor theExtractor;
// theExtractor.load_image("GTDATA.png");
// theExtractor.process();
// int *gt=theExtractor.get_ground_truth()
// Author   : A. Burguera.
// Creation : 21-Jan-2017

#ifndef _POGROUNDTRUTHEXTRACTOR_H_
#define _POGROUNDTRUTHEXTRACTOR_H_

#include "podefines.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class POGroundTruthExtractor {
private:
    // The provided ground truth image
    Mat _gtImage;
    // The extracted ground truth vector.
    int _gtData[_POD_NPATCH_ROW_][_POD_NPATCH_COL_];
    // Determine the ground truth of a patch.
    void _process_patch(int patchRow, int patchCol);

public:
    // Load the ground truth image and scales to the desired size.
    void load_image(string fileName);
    // Computes the ground truth
    void process();
    // Returns a pointer to the whole ground truth
    int *get_ground_truth();
};

#endif