import sys
from optparse import OptionParser
import os
import json

def WriteInclusions(outFile, jsonObj, builtinIncludeList):

    inclusionList = builtinIncludeList

    if "SimpleDataTypes" in jsonObj:
        for SDTIter in jsonObj["SimpleDataTypes"]:
            if (not (SDTIter["File"] in inclusionList)):
                inclusionList.append(SDTIter["File"])

    if ("EnumTypes" in jsonObj) :
        for EnumTIter in jsonObj["EnumTypes"]:
            if (not (EnumTIter["File"] in inclusionList)):
                inclusionList.append(EnumTIter["File"])

    if ("ClassTypes" in jsonObj) :
        for ClassTIter in jsonObj["ClassTypes"]:
            if (not (ClassTIter["File"] in inclusionList)):
                inclusionList.append(ClassTIter["File"])

    for incIter in inclusionList:
        outFile.write("#include<{0}>\n".format(incIter))

    outFile.write("\n")

def WriteEnumTypes(outFile, enumtList):

    for enumtIter in enumtList:

        enumName = enumtIter["FullName"]
        outFile.write("\nSB_DEFINE_ENUMERATION_TYPE_BEGIN({0}, {1})\n".format(enumtIter["Lib"], enumName))

        if "Values" in enumtIter:

            valuesObj = enumtIter["Values"]
            if valuesObj:

                for valueIter in valuesObj:

                    outFile.write("\tSB_ENUMERATION_VALUE({0})\n".format(valueIter["Name"]))

        outFile.write("SB_DEFINE_ENUMERATION_TYPE_END()\n")

def WriteClassTypes(outFile, classtList) :

    for classtIter in classtList:

        className = classtIter["Name"]
        outFile.write("\nSB_DEFINE_CLASS_TYPE_BEGIN({0}, {1})\n".format(classtIter["Lib"], className))

        if "Fields" in classtIter:

            outFile.write("\tSB_CLASS_ATTRIBUTES_BEGIN()\n")

            fields = classtIter["Fields"]
            for fieldIter in fields:

                outFile.write("\t\tSB_CLASS_ATTRIBUTE({0})\n".format(fieldIter["Name"]))

            outFile.write("\tSB_CLASS_ATTRIBUTES_END()\n")

        outFile.write("SB_DEFINE_CLASS_TYPE_END({0})\n".format(className))

def WriteSimpleDataTypes(outFile, sdtList):

    for sdtIter in sdtList:
        outFile.write("SB_DEFINE_SIMPLEDATA_TYPE({0}, {1})\n".format(sdtIter["Lib"], sdtIter["FullName"]))

def ReadJsonData(jsonFilePath):

    jsonFile = open(jsonFilePath, "r")
    jsonData = jsonFile.read()
    jsonFile.close()
    jsonObj = json.loads(jsonData)

    return jsonObj

def arg2ListCB(option, opt, value, parser):

    setattr(parser.values, option.dest, value.split(','))

if __name__ == "__main__" :

    optParser = OptionParser()

    optParser.add_option("-j", "--jsondb", action="store", type="string", help="path to json db", dest="jsonDB")
    optParser.add_option("-o", "--output", action="store", type="string", help="output cpp file", dest="outputFile")
    optParser.add_option("-i", "--includes", type="string", action='callback', callback=arg2ListCB, help="additional includes", dest="includeList")

    (options, args) = optParser.parse_args()

    if ((options.jsonDB is None) or (options.outputFile is None) ) :
        optParser.error("options -j and -o are required")

    outFile = options.outputFile.strip()
    jsonDB = options.jsonDB.strip()

    outDir = os.path.dirname(outFile)

    if (not os.path.exists(outDir)):
         os.makedirs(outDir)

    jsonObj = ReadJsonData(jsonDB)

    outFile = open(outFile, "w")

    includeList = []

    if options.includeList:
        includeList =  [x.strip() for x in options.includeList]

    WriteInclusions(outFile, jsonObj, includeList)

    if "SimpleDataTypes" in jsonObj:
        WriteSimpleDataTypes(outFile, jsonObj["SimpleDataTypes"])

    if "EnumTypes" in jsonObj:
        WriteEnumTypes(outFile, jsonObj["EnumTypes"])

    if "ClassTypes" in jsonObj:
        WriteClassTypes(outFile, jsonObj["ClassTypes"])

    outFile.close()