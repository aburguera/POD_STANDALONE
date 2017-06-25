from PIL import Image
import numpy as np
import os
import sys

def image_to_single_channel(theImage):
    """Converts images to 1 format"""
    if theImage.mode=='RGBA':
        x=np.array(theImage)
        r,g,b,a=np.rollaxis(x, axis=-1)
        meanColor=(r+g+b)/3
        meanColor[a==0]=255
        return Image.fromarray(meanColor,'L').convert('1')
    else:
        return theImage.convert('1')

def compare_labels(gtItem,testItem,TP=0,FP=0,TN=0,FN=0,labelPO=0):
    """Updates True and False Positive and Negatives according
    to the provided items"""
    if testItem==labelPO and gtItem==labelPO:
        TP+=1
    elif testItem==labelPO and gtItem!=labelPO:
        FP+=1
    elif testItem!=labelPO and gtItem!=labelPO:
        TN+=1
    elif testItem!=labelPO and gtItem==labelPO:
        FN+=1
    return [TP,FP,TN,FN]

def get_patch_label(theImage,theMode,pRow,pCol,nPatchRow=20,nPatchCol=20,labelPO=0):
    """Given a patch coordinates, outputs the corresponding label"""
    w,h=theImage.size
    pw=w/nPatchCol
    ph=h/nPatchRow
    nPO=0
    nNoPO=0
    subImage=theImage.crop((pCol*pw,pRow*ph,pCol*pw+pw-1,pRow*ph+ph-1)).getdata()
    for curPixel in subImage:
        if curPixel==labelPO:
            nPO+=1
        else:
            labelNoPO=curPixel
            nNoPO+=1
    if theMode==0:
        if nPO>nNoPO:
            return labelPO
        else:
            return labelNoPO
    else:
        if nPO==0:
            return labelNoPO
        elif nNoPO==0:
            return labelPO
        else:
            return -1

def eval_refined(gtImage,testImage,TP=0,FP=0,TN=0,FN=0,labelPO=0):
    """Computes true and false positives and negatives of an
    image of the same characteristics that the ground truth"""
    gtData=gtImage.getdata()
    testData=testImage.getdata()
    for i in range(len(gtData)):
        [TP,FP,TN,FN]=compare_labels(gtData[i],testData[i],TP,FP,TN,FN,labelPO)
    return [TP,FP,TN,FN]

def eval_patches(gtImage,testImage,theMode,TP=0,FP=0,TN=0,FN=0,nPatchRow=20,nPatchCol=20,labelPO=0):
    """Computes true and false positives and negatives for a patch-level
    classification"""
    w,h=testImage.size
    if w!=nPatchCol or h!=nPatchRow:
        print("[ERROR] PATCH-LEVEL CLASSIFICATION HAS WRONG SIZE")
        sys.exit(-1)
    for r in range(nPatchRow):
        for c in range(nPatchCol):
            gtLabel=get_patch_label(gtImage,theMode,r,c,nPatchRow,nPatchCol,labelPO)
            testLabel=testImage.getpixel((c,r))
            [TP,FP,TN,FN]=compare_labels(gtLabel,testLabel,TP,FP,TN,FN,labelPO)
    return [TP,FP,TN,FN]

def eval_dir(dataDir,gtDir,theExtension='.PNG',refPrefix="REF_",patchPrefix="PTC_"):
    """Evaluates a dir. The base for file names is gtDir. So, all files
    in gtDir must have correspondences in dataDir"""
    rTP,rFP,rTN,rFN=0,0,0,0
    pTP,pFP,pTN,pFN=0,0,0,0
    aTP,aFP,aTN,aFN=0,0,0,0
    allFiles=next(os.walk(gtDir))[2]
    for curFile in allFiles:
        fName,fExtension=os.path.splitext(curFile)
        if fExtension.upper()==theExtension:
            print('[PROCESSING] '+curFile)
            nameGT=os.path.join(gtDir,curFile)
            nameRefined=os.path.join(dataDir,refPrefix+curFile)
            namePatched=os.path.join(dataDir,patchPrefix+curFile)
            theGT=image_to_single_channel(Image.open(nameGT))
            theRefined=image_to_single_channel(Image.open(nameRefined))
            thePatched=image_to_single_channel(Image.open(namePatched))
            [rTP,rFP,rTN,rFN]=eval_refined(theGT,theRefined,rTP,rFP,rTN,rFN)
            [pTP,pFP,pTN,pFN]=eval_patches(theGT,thePatched,0,pTP,pFP,pTN,pFN)
            [aTP,aFP,aTN,aFN]=eval_patches(theGT,thePatched,1,aTP,aFP,aTN,aFN)
    return [rTP,rFP,rTN,rFN,pTP,pFP,pTN,pFN,aTP,aFP,aTN,aFN]

def print_results(theTitle,TP,FP,TN,FN):
    """Prints the results in human-readable format"""
    TP,FP,TN,FN=float(TP),float(FN),float(TN),float(FN)
    theTotal=TP+FP+TN+FN
    TPpct=TP*100/theTotal
    FPpct=FP*100/theTotal
    TNpct=TN*100/theTotal
    FNpct=FN*100/theTotal
    Tpct=(TP+TN)*100/theTotal
    Fpct=(FP+FN)*100/theTotal
    Ppct=(TP+FP)*100/theTotal
    Npct=(TN+FN)*100/theTotal

    print ("       TITLE: "+theTitle)
    print ("      +----------------+--------+")
    print ("      | POS     NEG    | SUM    |")
    print ("+-----+----------------+--------+")
    print ("| T   | {:>5.2f}%  {:>5.2f}% | {:>5.2f}% |".format(TPpct,TNpct,Tpct))
    print ("| F   | {:>5.2f}%  {:>5.2f}% | {:>5.2f}% |".format(FPpct,FNpct,Fpct))
    print ("+-----+----------------+--------+")
    print ("| SUM | {:>5.2f}%  {:>5.2f}% | {:>5.1f}% |".format(Ppct,Npct,Tpct+Fpct))
    print ("+-----+----------------+--------+")


def main(argv):
    if len(argv)!=3:
        print("USAGE: "+argv[0]+ " DATADIR GTDIR")
        sys.exit(-1)
    [rTP,rFP,rTN,rFN,pTP,pFP,pTN,pFN,aTP,aFP,aTN,aFN]=eval_dir(argv[1],argv[2])
    print_results("PIXEL",rTP,rFP,rTN,rFN)
    print_results("PATCH (MAJORITY)",pTP,pFP,pTN,pFN)
    print_results("PATCH (ALL-NONE)",aTP,aFP,aTN,aFN)

main(sys.argv)