import os.path
import sys
import io
from optparse import OptionParser
from subprocess import Popen, PIPE
from sbpy import host
from sbpy import log
from sbpy import code
from sbpy import file_system

# @todo: put somewhere in common
def logProcessOutput(output):
    buf = io.StringIO(output.decode('unicode_escape'))
    lineStr = buf.readline()
    while lineStr:
        log.LOG(log.Level.LOG, lineStr + "\n")
        lineStr = buf.readline()

# @todo: put somewhere in common
def logProcessError(error):
    buf = io.StringIO(error.decode('unicode_escape'))
    lineStr = buf.readline()
    while lineStr:
        log.LOG(log.Level.ERROR, lineStr + "\n")
        lineStr = buf.readline()

def executeClangCheckOnFile(clang_check_path, database_path, file_path):
    log.LOG(log.Level.LOG, "Analyzing '{}' ...\n".format(file_path))
    process = Popen([clang_check_path, "-p", database_path, file_path], stdout=PIPE, stderr=PIPE)
    (output, err) = process.communicate()
    res = process.wait()

    # @todo: put somewhere in common
    logProcessOutput(output)
    if res != 0:
        logProcessError(err)

    return res

def executeClangCheckOnDir(clang_check_path, database_path, dir_path, excludes):
    if os.path.isdir(dir_path):
        files = []
        file_system.FindFiles( files, dirs=[dir_path], extensions=['.cpp'] )

        for file_iter in files:
            matches = [exclude for exclude in excludes if re.match(exclude, file_iter, re.I) != None]
            if len(matches) == 0:
                err = executeClangCheckOnFile(clang_check_path, database_path, file_iter)
    else:
        log.LOG(log.Level.ERROR, "Cannot find directory '{}'".format(dir_path))


def arg2ListCB(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

if __name__ == "__main__":
    
    opt_parser = OptionParser()

    opt_parser.add_option("-d", "--database", action="store",
                         type="string", help="Path to the compilation database", dest="database_path")

    opt_parser.add_option("-e", "--engine", action="store_true",
                         help="Analyze all engine sources", dest="analyze_engine")

    opt_parser.add_option("-s", "--sample", action="store_true",
                         help="Analyze all sample sources", dest="analyze_sample")
    
    opt_parser.add_option("-t", "--test", action="store_true",
                         help="Analyze all test sources", dest="analyze_test")

    opt_parser.add_option("-x", "--exclude", type="string", action='callback', callback=arg2ListCB, help="Suffixes to exclude", dest="excludes")

    (options, args) = opt_parser.parse_args()

    clang_check_path = os.path.join(host.GetUtlBinDirPath(), host.GetExeFileName("clang-check"))
    excludes = []

    if not options.excludes is None:
        excludes = options.excludes

    if os.path.isfile(clang_check_path):
        if options.analyze_engine :
            executeClangCheckOnDir(clang_check_path, options.database_path, host.GetEngineSrcDirPath(), excludes)
        if options.analyze_sample :
            executeClangCheckOnDir(clang_check_path, options.database_path, host.GetSampleSrcDirPath(), excludes)
        if options.analyze_test :
            executeClangCheckOnDir(clang_check_path, options.database_path, host.GetTestSrcDirPath(), excludes)
    else:
        log.LOG(log.Level.ERROR, "Cannot find clang-check executable: {}".format(clang_check_path))

    if log.GetLastError() == 0:
        log.LOG(log.Level.LOG, "Clang static analyzis has finished successfully\n")
    else:
        log.LOG(log.Level.LOG, "Clang static analyzis has finished with error(s)\n")

    log.Exit()
