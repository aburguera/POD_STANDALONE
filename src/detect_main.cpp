// Author   : A. Burguera.
// Creation : 24-Jan-2017

#include "dirreader.h"
#include "helper.h"
#include "podefines.h"
#include "podescriber.h"
#include "podetector.h"
#ifdef _POD_REFINED_
#include "porefiner.h"
#endif

using namespace std;

// Prints usage
void print_usage(char *progName) {
    printf("=======\n");
    printf("Usage:\n");
    printf("%s SVM_FILE IN_DIR OUT_DIR [S]\n",progName);
    printf("  - SVM_FILE : Valid trained SVM in XML format.\n");
    printf("  - IN_DIR   : Contains the images to classify.\n");
    printf("  - OUT_DIR  : The directory to save results.\n");
    printf("  - S (opt.) : If specified, show results on screen.\n");
    printf("IMPORTANT: \n");
    printf("  - IN_DIR and OUT_DIR must end with '/'\n");
    printf("  - IN_DIR must contain FILES.TXT with the list of images\n");
    printf("    to use in the desired order. Build it from command line\n");
    printf("    with 'ls [...] -1 > FILES.TXT\n");
    printf("=======\n");
}

// Main program to perform training
int main(int argc, char** argv )
{
    unsigned char outToScreen;
    // Check the s first
    if (argc!=4 && !(outToScreen=(argc==5 && (argv[4][0]=='s' || argv[4][0]=='S')))) {
        printf("[ERROR] WRONG NUMBER OF PARAMETERS!\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    // Check the SVM file
    if (!Helper::file_exists(argv[1])) {
        printf("[ERROR] CANNOT OPEN '%s'!\n",argv[1]);
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Check input dir
    if (!Helper::file_exists(string(argv[2])+_DRD_DIRLIST_)) {
        printf("[ERROR] NO FILES.TXT DETECTED IN '%s'!\n",argv[2]);
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Check output dir
    if (!Helper::dir_exists(argv[3])) {
        printf("[ERROR] OUTPUT DIR '%s' DOES NOT EXIST!\n",argv[3]);
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Do the job!
    DirReader theDirReader(argv[2]);
    PODetector theDetector(argv[1]);
    #ifdef _POD_REFINED_
    PORefiner theRefiner;
    #endif
    while (theDirReader.next()) {
        Helper::show_progress("CLASSIFYING "+theDirReader.get()+" ("+to_string(theDirReader.get_cur_file_num())+" OF "+to_string(theDirReader.get_num_files())+")");
        // Load the image to classify
        theDetector.load_image(theDirReader.get());
        // To the classification
        theDetector.classify_image();
        #ifdef _POD_REFINED_
        theRefiner.set_data(theDetector.get_image(),theDetector.get_classification());
        theRefiner.process();
        #endif
        // Show the "nice" image to screen if S is set
        if (outToScreen) {
            namedWindow("CLASSIFIED");
            #ifdef _POD_REFINED_
            imshow("CLASSIFIED",theRefiner.get_refined_overlayed());
            #else
            imshow("CLASSIFIED",theDetector.get_graphic_classification());
            #endif
            waitKey(1);
        }
        // Save classification (patch level)
        imwrite(string(argv[3])+"PTC_"+theDirReader.get_filename(),theDetector.get_mask());
        #ifdef _POD_REFINED_
        // Save the refined classification scaled to the original image size
        imwrite(string(argv[3])+"REF_"+theDirReader.get_filename(),theRefiner.get_refined_scaled());
        // Save the overlayed version
        imwrite(string(argv[3])+"OVL_"+theDirReader.get_filename(),theRefiner.get_refined_overlayed());
        #endif
    }
    exit(EXIT_SUCCESS);
}