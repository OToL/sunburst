import os.path
import sys
import io
import re
from optparse import OptionParser
from subprocess import Popen, PIPE
from sbpy import host
from sbpy import log
from sbpy import code
from sbpy import file_system

# @todo: put somewhere in common
def logProcessError(error):
    buf = io.StringIO(error.decode('unicode_escape'))
    lineStr = buf.readline()
    while lineStr:
        log.LOG(log.Level.ERROR, lineStr + "\n")
        lineStr = buf.readline()

def executeClangCheckOnFile(clang_tidy_path, database_path, file_path, clang_tidy_checks):
    log.LOG(log.Level.LOG, "Analyzing '{}' ...\n".format(file_path))
    process = Popen([clang_tidy_path, "-p", database_path, "--quiet", "--checks=" + clang_tidy_checks, "--header-filter=/sunburst/",  file_path], stdout=PIPE, stderr=PIPE)
    (output, err) = process.communicate()
    res = process.wait()

    buf = io.StringIO(output.decode('ascii'))
    needs_output = res != 0
    output_lines = []

    line_str = buf.readline()
    while line_str:
        if line_str.isspace() or ("warnings generated" in line_str) or ("-header-filter=" in line_str):
            continue
        if ("warning:" in line_str) or ("error:" in line_str):
            needs_output = True
        output_lines.append(line_str) 
        line_str = buf.readline()

    if needs_output:
        log.LOG(log.Level.LOG, '\n'.join(output_lines))

    if res != 0:
        logProcessError(err)

    return res

def executeClangCheckOnDir(clang_tidy_path, database_path, dir_path, excludes, clang_tidy_checks):
    if os.path.isdir(dir_path):
        files = []
        file_system.FindFiles( files, dirs=[dir_path], extensions=['.cpp'] )

        for file_iter in files:
            matches = [exclude for exclude in excludes if re.match(exclude, file_iter, re.I) != None]
            if len(matches) == 0:
                err = executeClangCheckOnFile(clang_tidy_path, database_path, file_iter, clang_tidy_checks)
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

    opt_parser.add_option("-c", "--checks", action="store",
                         help="Clang-tidy checks", dest="clang_tidy_checks")   

    opt_parser.add_option("-x", "--exclude", type="string", action='callback', callback=arg2ListCB, 
                        help="Suffixes to exclude", dest="excludes")

    (options, args) = opt_parser.parse_args()

    #clang_tidy_path = os.path.join(host.GetUtlBinDirPath(), host.GetExeFileName("clang-tidy"))
    clang_tidy_path = "c:\\Program Files\\LLVM\\bin\\clang-tidy.exe"
    excludes = []
    checks = "bugprone-*,clang-diagnostic-*"

    if not options.clang_tidy_checks is None:
        checks = options.clang_tidy_checks
    if not options.excludes is None:
        excludes = options.excludes

    if os.path.isfile(clang_tidy_path):
        if options.analyze_engine :
            executeClangCheckOnDir(clang_tidy_path, options.database_path, host.GetEngineSrcDirPath(), excludes, checks)
        if options.analyze_sample :
            executeClangCheckOnDir(clang_tidy_path, options.database_path, host.GetSampleSrcDirPath(), excludes, checks)
        if options.analyze_test :
            executeClangCheckOnDir(clang_tidy_path, options.database_path, host.GetTestSrcDirPath(), excludes, checks)
    else:
        log.LOG(log.Level.ERROR, "Cannot find clang-check executable: {}".format(clang_tidy_path))

    if log.GetLastError() == 0:
        log.LOG(log.Level.LOG, "Clang static analyzis has finished successfully\n")
    else:
        log.LOG(log.Level.LOG, "Clang static analyzis has finished with error(s)\n")

    log.Exit()
