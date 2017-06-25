// Author   : A. Burguera.
// Creation : 24-Jan-2017

#include "dirreader.h"
#include "helper.h"
#include "podefines.h"
#include "podescriber.h"
#include "pogroundtruthextractor.h"
#include "potrainer.h"

// Prints usage
void print_usage(char *progName) {
    printf("=======\n");
    printf("Usage:\n");
    printf("%s DATA_DIR GT_DIR SAVE_FILE\n",progName);
    printf("  - DATA_DIR  : Contains the images used to train.\n");
    printf("  - GT_DIR    : Contains the ground truth images.\n");
    printf("  - SAVE_FILE : Name to save the trained SVM\n");
    printf("IMPORTANT: \n");
    printf("  - DATA_DIR and GT_DIR must end with '/'\n");
    printf("  - DATA_DIR must contain FILES.TXT with the list of images\n");
    printf("    to use in the desired order. Build it from command line\n");
    printf("    with 'ls [...] -1 > FILES.TXT\n");
    printf("  - For each file in DATA_DIR, the corresponding ground truth in GT_DIR\n");
    printf("    must have the same file name.\n");
    printf("=======\n");
}

// Main program to perform training
int main(int argc, char** argv )
{
    // Check input parameters
    if (argc!=4) {
        printf("[ERROR] WRONG NUMBER OF PARAMETERS!\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    // Check if DIRs exist and contain FILES.TXT
    if (!Helper::file_exists(string(argv[1])+_DRD_DIRLIST_) || !Helper::file_exists(string(argv[2])+_DRD_DIRLIST_)) {
        printf("[ERROR] NO FILES.TXT DETECTED!\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Do the job
    POTrainer theTrainer(argv[1],argv[2]);
    theTrainer.do_training(argv[3]);

    exit(EXIT_SUCCESS);
}

