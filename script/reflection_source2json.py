import sys

import clang.cindex
import clang.utils
import clang.asciitree
import sb.host
import os
import string

from optparse import OptionParser

def NormalizePath(path):

     return path.lower().replace('\\', '/')

class PType:

    def __init__(self, name, fullName, namespace, lib, filePath, id):

        self.m_Name = name
        self.m_FullName = fullName
        self.m_Namespace = namespace
        self.m_Lib = lib
        self.m_FilePath = filePath
        self.m_Id = id

    m_Name = ""
    m_FullName = ""
    m_Namespace = ""
    m_Lib = ""
    m_FilePath = ""
    m_Id = 0

class PSimpleType(PType):

    def __init__(self, name, fullName, namespace, lib, filePath, id):

        super().__init__(name, fullName, namespace, lib, filePath, id)

class PEnumType(PType):

    def __init__(self, name, fullName, namespace, lib, filePath, id, values):

        super().__init__(name, fullName, namespace, lib, filePath, id)
        self.m_Values = values

    m_Values = []

class PSourceParser:

    def __init__(self):

        pass

    def Parse(self, filePath, options, filters):

        self.m_TranslationUnit = clangIndex.parse(filePath, options)
        self.m_Filters = filters;

        if self.m_TranslationUnit :

            self.__ParseInternal(self.m_TranslationUnit.cursor)

    def __ParseEnumType(self, cursor, attrs):

        for child_node in cursor.get_children():

            if child_node.kind == clang.cindex.CursorKind.TYPE_REF :

                srcTypeCursor = self.__FindType(child_node)

                #clang.utils.dumpAST(srcTypeCursor)

                if srcTypeCursor :

                    values = []

                    for enum_iter in srcTypeCursor.get_children():

                        values.append((clang.utils.getCursorName(enum_iter), enum_iter.enum_value))

                    cursorPath = ([],[])
                    clang.utils.getPathToCursor(srcTypeCursor, cursorPath)

                    typeName = clang.utils.getCursorName(srcTypeCursor)

                    fullPath = self.__ComposeCursorPath(cursorPath[0])

                    if len(fullPath):

                        fullPath += "::"

                    fullPath += typeName

                    self.m_EnumTypes.append(PEnumType(typeName, fullPath, self.__ComposeCursorPath(cursorPath[1]), attrs["lib"], clang.utils.getCursorFilePath(srcTypeCursor), self.GetSimpleDataTypeId(cursor), values))

                    break

    def __ParseSimpleDataType(self, cursor, attrs):

        for child_node in cursor.get_children():

            if child_node.kind == clang.cindex.CursorKind.TYPE_REF :

                srcTypeCursor = self.__FindType(child_node)

                if srcTypeCursor :

                    cursorPath = ([],[])
                    clang.utils.getPathToCursor(srcTypeCursor, cursorPath)

                    typeName = clang.utils.getCursorName(srcTypeCursor)

                    fullPath = self.__ComposeCursorPath(cursorPath[0])

                    if len(fullPath):

                        fullPath += "::"

                    fullPath += typeName

                    self.m_SimpleDataTypes.append(PSimpleType(typeName, fullPath, self.__ComposeCursorPath(cursorPath[1]), attrs["lib"], clang.utils.getCursorFilePath(srcTypeCursor), self.GetSimpleDataTypeId(cursor)))

                    break

    def ParseAnnotations(self, annotations):

        attrs = {}

        annotationList = annotations.split(";")

        for attrIter in annotationList:

            attr = attrIter.split("=")

            if (len(attr) == 2):

                attrs[attr[0]] = attr[1]

        return attrs

    def GetAnnotations(self, cursor):

        for child_node in cursor.get_children():

            if child_node.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:

                return clang.utils.getCursorName(child_node)

        return ""

    def GetSimpleDataTypeId(self, cursor):

        typeId = None

        for child_node in cursor.get_children():

            if (child_node.kind == clang.cindex.CursorKind.INTEGER_LITERAL):

                nodeExtent = child_node.extent

                tokens = clang.cindex.TokenGroup.get_tokens(self.m_TranslationUnit, nodeExtent)

                for tokenIter in tokens:

                    if clang.cindex.TokenKind.LITERAL == tokenIter.kind:

                        id = 0;

                        idStr = clang.utils.decodeClangStr(tokenIter.spelling)
                        idLen = len(idStr);

                        for iter in idStr[::-1]:
                            if (not iter.isdigit()):
                                lowerChar = iter.lower()
                                if (lowerChar > 'f'):
                                    idLen = idLen - 1
                                else:
                                    break
                            else:
                                break

                        idStr = idStr[:idLen]
                        typeId = int(idStr, 0)

                        break

                if not typeId is None:
                    break

            else:

                typeId = self.GetSimpleDataTypeId(child_node)

                if typeId:

                    break;

        return typeId

    def __ParseInternal(self, cursor):

        isFilteredIn = True
        fileName = ""

        if cursor.location.file and (self.m_CurrCppFile != cursor.location.file) and len(self.m_Filters):
            normFilePath = NormalizePath(clang.utils.getCursorFilePath(cursor))

            isFilteredIn = False

            for filterIter in self.m_Filters:

                if -1 != normFilePath.find(filterIter):
                    isFilteredIn = True
                    fileName = normFilePath;
                    break

            if isFilteredIn:
                self.m_CurrCppFile = cursor.location.file

        if isFilteredIn:

            for child_node in cursor.get_children():

                    if (child_node.kind == clang.cindex.CursorKind.CLASS_DECL) and (child_node.spelling == b'C_TypeLookup'):

                        attrs = self.ParseAnnotations(self.GetAnnotations(child_node))

                        if ("type" in attrs):

                            typeStr = attrs["type"]

                            if "simpledata" == typeStr:

                                self.__ParseSimpleDataType(child_node, attrs)

                            elif "enum" == typeStr:

                                self.__ParseEnumType(child_node, attrs)

                            else:

                                pass;



                    self.__ParseInternal(child_node)


    def __ComposeCursorPath(self, inputTypePath):

        composedPath = ""

        typePath = inputTypePath
        typePath.reverse()

        if len(typePath):

            for pathIter in typePath:

                if 0 < len(composedPath):

                   composedPath += "::"

                composedPath += pathIter

        return composedPath

    def __FindType(self, cursor, source = False):

        if source:

            if cursor.kind == clang.cindex.CursorKind.TYPE_REF :

                return self.__FindType(cursor.type.get_declaration())

            elif cursor.kind == clang.cindex.CursorKind.TYPEDEF_DECL:

                originalType = clang.utils.getFirstChild(cursor)

                if originalType:

                    return self.__FindType(originalType)

                else:

                    return cursor

            else:

                return cursor;

        else:

            if cursor.kind == clang.cindex.CursorKind.TYPE_REF :

                return cursor.type.get_declaration()

        return None


    m_TranslationUnit = None
    m_SimpleDataTypes = []
    m_EnumTypes = []
    m_Filters = []
    m_CurrCppFile = ""

def WriteSimpleDataTypes(outFile, simpleDataTypes):

    outFile.write("\t\"SimpleDataTypes\" : [\n")

    typeIndex = 1

    for simpleDataIter in simpleDataTypes:

        outFile.write("\t\t{\n")

        outFile.write("\t\t\t\"Id\" : " + str(simpleDataIter.m_Id) + ",\n")
        outFile.write("\t\t\t\"Name\" : \"" + simpleDataIter.m_Name + "\",\n")
        outFile.write("\t\t\t\"FullName\" : \"" + simpleDataIter.m_FullName + "\",\n")
        outFile.write("\t\t\t\"Namespace\" : \"" + simpleDataIter.m_Namespace + "\",\n")
        outFile.write("\t\t\t\"Lib\" : \"" + simpleDataIter.m_Lib + "\",\n")
        outFile.write("\t\t\t\"File\" : \"" + simpleDataIter.m_FilePath + "\"\n")

        endBlock = "\t\t}"

        if typeIndex < len(simpleDataTypes):
            endBlock+=","

        endBlock += "\n"

        outFile.write(endBlock)
        typeIndex += 1


    outFile.write("\t],\n")

def WriteEnumTypes(outFile, enumTypes):

    outFile.write("\t\"EnumTypes\" : [\n")

    typeIndex = 1

    for enumter in enumTypes:

        outFile.write("\t\t{\n")

        outFile.write("\t\t\t\"Id\" : " + str(enumter.m_Id) + ",\n")
        outFile.write("\t\t\t\"Name\" : \"" + enumter.m_Name + "\",\n")
        outFile.write("\t\t\t\"FullName\" : \"" + enumter.m_FullName + "\",\n")
        outFile.write("\t\t\t\"Namespace\" : \"" + enumter.m_Namespace + "\",\n")
        outFile.write("\t\t\t\"Lib\" : \"" + enumter.m_Lib + "\",\n")
        outFile.write("\t\t\t\"File\" : \"" + enumter.m_FilePath + "\",\n")

        outFile.write("\t\t\t\"Values\" : [\n")

        valueIndex = 1

        for valueIter in enumter.m_Values:

            outFile.write("\t\t\t\t{\n")

            outFile.write("\t\t\t\t\t\"Name\" : \"" + valueIter[0] + "\",\n")
            outFile.write("\t\t\t\t\t\"FullName\" : \"" + enumter.m_FullName + "::" + valueIter[0] + "\",\n")
            outFile.write("\t\t\t\t\t\"Value\" : " + str(valueIter[1]) + "\n")

            if valueIndex < len(enumter.m_Values):

                outFile.write("\t\t\t\t},\n")

            else:

                outFile.write("\t\t\t\t}\n")

            valueIndex += 1

        outFile.write("\t\t\t]\n")

        endBlock = "\t\t}"

        if typeIndex < len(enumTypes):
            endBlock+=","

        endBlock += "\n"

        outFile.write(endBlock)
        typeIndex += 1

    outFile.write("\t],\n")

def WriteClassTypes(outFile, classTypes):

    outFile.write("\t\"ClassTypes\" : [\n")

    outFile.write("\t]\n")

def DumpTypes2Json(filePath, simpleDataTypes, enumTypes):

    outFile = open(filePath, "w")

    outFile.write("{\n")

    WriteSimpleDataTypes(outFile, simpleDataTypes)
    WriteEnumTypes(outFile, enumTypes)
    WriteClassTypes(outFile, [])

    outFile.write("}\n")

    outFile.close()

def arg2ListCB(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

if __name__ == "__main__" :

    optParser = OptionParser()

    optParser.add_option("-s", "--source", action="store", type="string", help="input cpp file", dest="srcFilePath")
    optParser.add_option("-o", "--output", action="store", type="string", help="output json file", dest="outputFilePath")
    optParser.add_option("-a", "--arch", action="store", type="string", help="current architecture", dest="currArch")
    optParser.add_option("-i", "--includes", type="string", action='callback', callback=arg2ListCB, help="include list", dest="includeList")
    optParser.add_option("-f", "--filters", type="string", action='callback', callback=arg2ListCB, help="filter list", dest="filterList")

    (options, args) = optParser.parse_args()

    if ((options.srcFilePath is None) or (options.outputFilePath is None) or (options.includeList is None)  or (options.currArch is None)) :
        optParser.error("options -i, -s, -a and -o are required")

    filters = []

    if options.filterList:
        filters = [NormalizePath(x.strip()) for x in options.filterList]

    pwd = os.path.dirname(__file__)
    
    libClangPath = "../../3rd/lib/{0}/release/libclang.{1}".format(options.currArch.strip(), sb.host.GetDynLibExt())
    clang.cindex.Config.set_library_file(os.path.join(pwd, libClangPath))
    clangIndex = clang.cindex.Index.create()

    parser = PSourceParser()

    includes = [x.strip() for x in options.includeList]

    parser.Parse(options.srcFilePath.strip(), ['-x',
                                            'c++',
                                            '-std=c++11',
                                            '-stdlib=wstd',
                                            '-D__CODE_GENERATOR__'] + includes, filters)


    DumpTypes2Json(options.outputFilePath.strip(), parser.m_SimpleDataTypes, parser.m_EnumTypes)
    