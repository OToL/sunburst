import io
import os.path
import re
import sys
from optparse import OptionParser
from subprocess import PIPE, Popen

from sbpy import code, file_system, host, log


# @todo: put somewhere in common
def logProcessError(error):
    buf = io.StringIO(error.decode('unicode_escape'))
    lineStr = buf.readline()
    while lineStr:
        log.LOG(log.Level.ERROR, lineStr + "\n")
        lineStr = buf.readline()

def executeClangCheckOnFile(clang_tidy_path, database_path, file_path):
    error_count = 0
    log.LOG(log.Level.LOG, "Analyzing '{}' ...\n".format(file_path))
    process = Popen([clang_tidy_path, "-p", database_path, "--quiet", file_path], stdout=PIPE, stderr=PIPE)
    (output, err) = process.communicate()
    res = process.wait()

    buf = io.StringIO(output.decode('ascii'))
    output_lines = []

    line_str = buf.readline()
    while line_str:
        if line_str.isspace() or ("warnings generated" in line_str) or ("-header-filter=" in line_str):
            continue
        if ("warning:" in line_str) or ("error:" in line_str):
            error_count += 1
        output_lines.append(line_str) 
        line_str = buf.readline()

    if error_count != 0:
        log.LOG(log.Level.LOG, '\n'.join(output_lines))

    if res != 0:
        logProcessError(err)

    return error_count

def executeClangCheckOnDir(clang_tidy_path, database_path, dir_path, excludes):
    error_count = 0
    if os.path.isdir(dir_path):
        files = []
        file_system.FindFiles( files, dirs=[dir_path], extensions=['.cpp'] )

        for file_iter in files:
            matches = [exclude for exclude in excludes if re.match(exclude, file_iter, re.I) != None]
            if len(matches) == 0:
                error_count += executeClangCheckOnFile(clang_tidy_path, database_path, file_iter)
    else:
        log.LOG(log.Level.ERROR, "Cannot find directory '{}'".format(dir_path))

    return error_count


def arg2ListCB(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

if __name__ == "__main__":
    
    opt_parser = OptionParser()

    opt_parser.add_option("-d", "--database", action="store", type="string",
                        help="Path to the compilation database", dest="database_path")

    opt_parser.add_option("-e", "--engine", action="store_true",
                        help="Analyze all engine sources", dest="analyze_engine")

    opt_parser.add_option("-s", "--sample", action="store_true",
                        help="Analyze all sample sources", dest="analyze_sample")
    
    opt_parser.add_option("-t", "--test", action="store_true",
                        help="Analyze all test sources", dest="analyze_test")

    opt_parser.add_option("-x", "--exclude", type="string", action='callback', callback=arg2ListCB, 
                        help="Suffixes to exclude", dest="excludes")

    opt_parser.add_option("-i", "--input", action="store", type="string", 
                        help="Input path to format", dest="input_path")

    (options, args) = opt_parser.parse_args()

    #clang_tidy_path = os.path.join(host.GetUtlBinDirPath(), host.GetExeFileName("clang-tidy"))
    clang_tidy_path = "c:\\Program Files\\LLVM\\bin\\clang-tidy.exe"
    excludes = []

    if not options.excludes is None:
        excludes = options.excludes

    error_count = 0
    if os.path.isfile(clang_tidy_path):
        if options.analyze_engine :
            error_count += executeClangCheckOnDir(clang_tidy_path, options.database_path, host.GetEngineSrcDirPath(), excludes)
        if options.analyze_sample :
            error_count += executeClangCheckOnDir(clang_tidy_path, options.database_path, host.GetSampleSrcDirPath(), excludes)
        if options.analyze_test :
            error_count += executeClangCheckOnDir(clang_tidy_path, options.database_path, host.GetTestSrcDirPath(), excludes)
        if (not options.input_path is None):
            if os.path.exists(options.input_path):
                if os.path.isfile(options.input_path):
                    error_count += executeClangCheckOnFile(clang_tidy_path, options.database_path, options.input_path)
                elif os.path.isdir(options.input_path):
                    ExecuteClangFormatOnDir(options.input_path)
                else:
                    log.LOG(log.Level.ERROR, "{} is not a supported input path".format(options.input_path))
            else:
                log.LOG(log.Level.ERROR, "{} does not exist".format(options.input_path))
    else:
        log.LOG(log.Level.ERROR, "Cannot find clang-check executable: {}".format(clang_tidy_path))

    if error_count == 0:
        log.LOG(log.Level.LOG, "Clang static analyzis has finished successfully\n")
    else:
        log.LOG(log.Level.LOG, "Clang static analyzis has finished with {} error(s)\n", error_count)

    log.Exit()
