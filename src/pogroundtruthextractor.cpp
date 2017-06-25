#include "pogroundtruthextractor.h"

void POGroundTruthExtractor::_process_patch(int patchRow, int patchCol) {
    Mat thePatch(_gtImage,Rect(patchCol*_POD_PATCH_COLS_,patchRow*_POD_PATCH_ROWS_,_POD_PATCH_COLS_,_POD_PATCH_ROWS_));
    assert(thePatch.type()==CV_8UC1);
    int nZeros=0,nOnes=0;
    for (int curRow=0;curRow<_POD_PATCH_ROWS_;curRow++){
        for (int curCol=0;curCol<_POD_PATCH_COLS_;curCol++){
            int curValue=(int)thePatch.at<unsigned char>(curRow,curCol);
            nOnes+=(curValue>128);
            nZeros+=(curValue<=128);
        }
    }
    #ifdef _POD_MAJORITY_DESC_
    _gtData[patchRow][patchCol]=(nOnes>nZeros?1:0);
    #else
    int theVal;
    if (nOnes==0) {
        theVal=0;
    } else if (nZeros==0) {
        theVal=1;
    } else theVal=2;
    _gtData[patchRow][patchCol]=theVal;
    #endif
}

void POGroundTruthExtractor::load_image(string fileName){
    _gtImage=imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
    // Assert image is loaded
    assert(_gtImage.data);
    // Resize to desired width and height
    resize(_gtImage,_gtImage,Size(_POD_IMAGE_COLS_,_POD_IMAGE_ROWS_));
}

void POGroundTruthExtractor::process(){
    for (int pRow=0;pRow<_POD_NPATCH_ROW_;pRow++){
        for (int pCol=0;pCol<_POD_NPATCH_COL_;pCol++){
            _process_patch(pRow,pCol);
        }
    }
}

int *POGroundTruthExtractor::get_ground_truth(){
    return (int *)_gtData;
}