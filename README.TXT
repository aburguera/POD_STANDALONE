==============
=== CREDIT ===
==============

Science    : A. Burguera, F. Bonin and J.L. Lisani
Code       : A. Burguera.
Contact    : antoni.burguera@uib.es
References :

A. Burguera, F. Bonin-Font, J.L. Lisani and A.B. Petro.
Towards Automatic Visual Sea Grass Detection in Underwater Areas of Ecological Interest.
In IEEE International Conference on Emerging Technologies and Factory Automation, Berlin, 2016.

F. Bonin-Font, A. Burguera and J. L. Lisani
Visual Discrimination and Large Area Mapping of Posidonia Oceanica Using a Lightweight AUV
Inn IEEE Access, vol. 5, pp. 24479-24494, 2017.
doi: 10.1109/ACCESS.2017.2764998

===========================
=== BUILD TEST PROGRAMS ===
===========================

There are two test programs: detector and trainer. To build them, type the
following lines in the project root directory:

cmake .
make


=============================
=== EXECUTE TEST PROGRAMS ===
=============================

From the project root directory type bin/trainer or bin/detector for
explanation on usage.


============================
=== EVALUATE THE RESULTS ===
============================

The python script "script/evaluate_results.py" performs a basic evaluation.
Execute it for explanation on usage.


=======================
=== IMPORTANT NOTES ===
=======================

The file config/GABOR.CSV should NOT be modified. To place it in another
directory, be sure to make _POD_GABOR_FNAME_ in podefines.h to point to the right
place.

When using the test programs, make sure that:

1) All directory specs end with "/" (slash). Otherwise, files will not be saved
where expected.

2) Every directory that has to be fully analyzed contains a FILES.TXT file.
This file specifies the files in the directory to be used and the exact order
in which they have to be processed. The file must contain a single file name per
line. For example, if all PNG files in one dir must be used, FILES.TXT can be
created by entering the directory and typing:

ld *.png -1 > FILES.TXT

Note that it is "minus one", not "minus L".

==========================
=== FULL USAGE EXAMPLE ===
==========================

Let PATH denote the root directory (i.e. the one with the CMakeLists.txt).
From this path, proceed as follows:

1) Build the project:

cmake .
make

2) Create the following folders

mkdir data
mkdir data/GTCLASS
mkdir data/GTTRAIN
mkdir data/IMGTRAIN
mkdir data/IMGCLASS
mkdir data/OUT

3) Within each of the previous folders put:

  GTCLASS  : The ground truth files used to assess the classification. Optional.
  GTTRAIN  : The ground truth files used for training. Mandatory.
  IMGTRAIN : The images used for training. Mandatory.
  IMGCLASS : The images to clasify. Mandatory.
  OUT      : Leave it empty

Be sure that every file in GTXX folder has a corresponding file in IMGXX with the
same exact name.

5) Generate the FILES.TXT. If images are PNG, do the followint from PATH:

cd data/GTCLASS/
ls *.png -1 > FILES.TXT
cd ../GTTRAIN/
ls *.png -1 > FILES.TXT
cd ../IMGTRAIN/
ls *.png -1 > FILES.TXT
cd ../IMGCLASS/
ls *.png -1 > FILES.TXT
cd ../..

6) Do the training (it might take some time)

bin/trainer data/IMGTRAIN/ data/GTTRAIN/ data/TRAINEDSVM.XML

7) Do the classification

bin/detector data/TRAINEDSVM.XML data/IMGCLASS/ data/OUT/ s

The "s" is optional. It simply shows the output during the execution.

8) Evaluate the results

If GTCLASS is available, the results can be evaluated:

python script/evaluate_results.py data/OUT/ data/GTCLASS

The evaluation is simply printed to screen.

9) Explore. The classifier output is available in data/OUT.


========================================
=== USING THE CODE IN OTHER PROJECTS ===
========================================

The trainer example should suffice for all purposes, as it generates a
ready-to-use trained SVM.

As for the classifier, the example (src/detect_main.cpp) is aimed at classifying
files from disk, but it can be used as reference to different detection schemas.
For example, the detector class has a method to load images from disk and another
to use pre-loaded images.

Additionally, every main file in include has a short usage example.


=====================
=== FINAL REMARKS ===
=====================

Most of the trainer/detector parameters can be modified in podefines.h
Note that every change requires re-compiling and that, in most cases,
a trained SVM is not valid to classify if parameters in podefines.h have
changed.


=======================
=== TROUBLESHOOTING ===
=======================

The thread library is used to speed-up the description process. This library
requires a c++11 compliant compiler. If not available or threads are unwanted
for some reason, just comment the following line in include/podefines.h:

#define _POD_THREADED_

Also, removing the c++11 flag from CMakeLists.txt might be necessary.
