// Trains the SVM to detect PO using a train set and the corresponding ground truth.
// Usage example:
// POTrainer theTrainer ("IMG/","GT/");
// theTrainer.do_training("SAVEDSVM.XML");
// Author   : A. Burguera.
// Creation : 21-Jan-2017

#ifndef _POTRAINER_H_
#define _POTRAINER_H_

#include <iostream>
#include "podefines.h"
#include "podescriber.h"
#include "pogroundtruthextractor.h"
#include "dirreader.h"
#include "helper.h"

using namespace std;

class POTrainer {
private:
    string _trainingDir;                        // The directory with the training images.
    string _gtDir;                              // The directory with the ground truth images.
    PODescriber _theDescriber;                  // The described used to compute image descriptors.
    POGroundTruthExtractor _theGTExtractor;     // The GTExtractor used to extract ground truth from GT images.
    Ptr<ml::SVM> _theSVM;                       // The SVM
    // Removes all entries with class 2 in labels and training data
    int _remove_class(Mat &labelsMat, Mat &trainingDataMat);

public:
    // Constructor. Store dir names and create SVM.
    POTrainer(string trainingDir, string gtDir);
    // Do the training and store SVM in specified filename.
    void do_training(string saveFileName=_POD_SVM_FNAME_);
};

#endif
