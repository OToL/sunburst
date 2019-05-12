import sys
import os
import io

from optparse import OptionParser
from subprocess import Popen, PIPE

sys.path.append(os.path.join(os.path.dirname(__file__), "../common"))
from sb import *

def GetToolchainIncludePaths(targetName):
    
    includePaths = []

    if (targetName == "macos") or (targetName == "linux"):

        process = Popen(["clang", "-E" , "-x", "c++" , "/dev/null", "-v"], stdout=PIPE, stderr=PIPE)
        (output, err) = process.communicate()
        exitCode = process.wait()

        if exitCode == 0:
            
            # clang include paths are in error stream ...
            buf = io.StringIO(err.decode('unicode_escape'))
            
            lineStr = buf.readline().strip()

            # parse the whole output
            while lineStr :
                                
                # start include section
                if lineStr and (lineStr[0] == '#'):
                    while True:
                        lineStr = buf.readline().strip()
                        
                        if (not lineStr) or (lineStr[0] == '#') or ("end of search list" in lineStr.lower()):
                            break
                    
                        includePaths.append(lineStr.split()[0])
                else:
                    lineStr = buf.readline()
        else:
            log.LOG(log.Level.ERROR, "Clang failed with code {0}".format(exitCode))
    else:
        log.LOG(log.Level.ERROR, "Target platfom '{0}' not supported", targetName)

    return includePaths

class MyOption():
    pass;

if __name__ == "__main__" :

    optParser = OptionParser()
    
    optParser.add_option("-t", "--target", action="store", type="string", help="target name", dest="targetName")
    optParser.add_option("-i", "--inclusions", action="store_true", default=False, help="output toolchain include paths", dest="genIncludePaths")

    (options, args) = optParser.parse_args()

    if (options.targetName is None) :
        log.LOG(log.Level.FATAL_ERROR, "options -t is required")

    targetName = options.targetName.strip()

    if options.genIncludePaths:
        includePaths = GetToolchainIncludePaths(targetName)
        if len(includePaths)>=1:
            log.LOG(log.Level.LOG, ";".join(includePaths))    
    else:
        log.LOG(log.Level.FATAL_ERROR, "nothing to be done")

    log.Exit()
