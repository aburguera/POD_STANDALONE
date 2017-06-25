#include "potrainer.h"

int POTrainer::_remove_class(Mat &labelsMat, Mat &trainingDataMat) {
    Mat newLabels;
    Mat newTraining;
    int firstTime=1;
    for (int curRow=0;curRow<labelsMat.rows;curRow++) {
        if (labelsMat.at<int>(curRow,0)!=2) {
            if (firstTime) {
                newLabels=labelsMat.row(curRow);
                newTraining=trainingDataMat.row(curRow);
                firstTime=0;
            } else {
                newLabels.push_back(labelsMat.row(curRow));
                newTraining.push_back(trainingDataMat.row(curRow));
            }
        }
    }
    labelsMat=newLabels;
    trainingDataMat=newTraining;
    return (!firstTime);
}

POTrainer::POTrainer(string trainingDir, string gtDir){
    _trainingDir=trainingDir;
    _gtDir=gtDir;
    _theSVM=ml::SVM::create();
}

void POTrainer::do_training(string saveFileName){
    Mat allDescriptors,allLabels;
    int firstTime=1;
    DirReader theDirReader(_trainingDir);
    while (theDirReader.next()) {
        // Provide some feedback
        Helper::show_progress("PROCESSING "+theDirReader.get()+" ("+to_string(theDirReader.get_cur_file_num())+" OF "+to_string(theDirReader.get_num_files())+")");
        // Compute descriptors
        _theDescriber.load_image(theDirReader.get());
        _theDescriber.describe_image();
        // Obtain ground truth
        _theGTExtractor.load_image(_gtDir+theDirReader.get_filename());
        _theGTExtractor.process();
        // Prepare data for OpenCV SVM
        Mat labelsMat(_POD_NPATCH_ROW_*_POD_NPATCH_COL_, 1, CV_32SC1, _theGTExtractor.get_ground_truth());
        Mat trainingDataMat(_POD_NPATCH_ROW_*_POD_NPATCH_COL_, _POD_DESCR_TYPES_*_POD_IMAGE_NCHAN_*_POD_GABOR_SCALES_*_POD_GABOR_ORIENT_, CV_32FC1, _theDescriber.get_descriptor());
        #ifndef _POD_MAJORITY_DESC_
        if (_remove_class(labelsMat,trainingDataMat)) {
        #endif
            if (firstTime) {
                firstTime=0;
                allLabels=labelsMat;
                allDescriptors=trainingDataMat;
            } else {
                vconcat(allLabels,labelsMat,allLabels);
                vconcat(allDescriptors,trainingDataMat,allDescriptors);
            }
        #ifndef _POD_MAJORITY_DESC_
        }
        #endif
    }
    // Do the training
    Helper::show_progress("TRAINING SVM");
    _theSVM->trainAuto(ml::TrainData::create(allDescriptors,ml::ROW_SAMPLE,allLabels));

    // Save the trained SVM
    _theSVM->save(saveFileName);
    Helper::show_progress("TRAINED SVM SAVED TO "+saveFileName);
}