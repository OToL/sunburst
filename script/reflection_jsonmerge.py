from optparse import OptionParser;
import os;
import json;

def MergeSimpleDataTypes(tmpSDT, outputSDT):
    
    tempObjects = [];
    
    for SDTIter in tmpSDT:
        found = False;
        for SDTOutIter in outputSDT:
            if SDTIter["Id"] == SDTOutIter["Id"]:
                found = True;
                break;
        if (not found):
            tempObjects.append(SDTIter);

    if (len(tempObjects)):
        outputSDT.extend(tempObjects);

def MergeDB(tmpJSONObj, outputJSONObj):
    
    if ("SimpleDataTypes" in tmpJSONObj):
        MergeSimpleDataTypes(tmpJSONObj["SimpleDataTypes"], outputJSONObj["SimpleDataTypes"]);

def DumpMergedDB(jsonObj, filePath):
    
    outDBStr = json.dumps(outputJSONObj, sort_keys=True, indent=2);
    
    outFile = open(filePath, "w");
    
    outFile.write(outDBStr);
    
    outFile.close();

def DriectoryWalker(arg, dirname, names):
    
    for contentIter in names:     
        if (".json" in contentIter):
            arg.append(os.path.join(dirname, contentIter));

if __name__ == "__main__" :

    optParser = OptionParser();
    
    optParser.add_option("-d", "--directory", action="store", type="string", help="source directory", dest="srcDir");
    optParser.add_option("-o", "--output", action="store", type="string", help="path to merged json file", dest="outputFile");
    
    (options, args) = optParser.parse_args()
    
    if (not (options.srcDir and options.outputFile) ) :
        optParser.error("options -d and -o are required");
    
    outFile = options.outputFile.strip();
    srcDir = options.srcDir.strip();
    
    outDir = os.path.dirname(outFile);
    
    if (not os.path.exists(outDir)):
         os.makedirs(outDir);

    fileList = []
    os.path.walk(srcDir, DriectoryWalker, fileList);
    
    outputJSONObj = {
        "SimpleDataTypes" : [],
     };

    for dbIter in fileList:
        
        dbFile = open(dbIter, "r");
        dbdata = dbFile.read();
        dbFile.close();
        
        tmpJSONObj = json.loads(dbdata);
        MergeDB(tmpJSONObj, outputJSONObj);
      
    DumpMergedDB(outputJSONObj, outFile);  
    
    