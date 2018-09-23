import clang.asciitree
import clang.cindex
from clang.cindex import Index
from pprint import pprint

def nodeChildren(node):
    return node.get_children()

def printNode(node):
    text = node.spelling or node.displayname
    kind = str(node.kind)[str(node.kind).index('.')+1:]
    return '{} {}'.format(kind, text)

def get_diag_info(diag):
    return { 'severity' : diag.severity,
             'location' : diag.location,
             'spelling' : diag.spelling,
             'ranges' : diag.ranges,
             'fixits' : diag.fixits }

def get_cursor_id(cursor, cursor_list = []):
    #if not opts.showIDs:
     #   return None

    if cursor is None:
        return None

    # FIXME: This is really slow. It would be nice if the index API exposed
    # something that let us hash cursors.
    for i,c in enumerate(cursor_list):
        if cursor == c:
            return i
    cursor_list.append(cursor)
    return len(cursor_list) - 1

def get_info(node, depth=0):

    children = [get_info(c, depth+1)
                    for c in node.get_children()]
    return { 'id' : get_cursor_id(node),
             'kind' : node.kind,
             'usr' : node.get_usr(),
             'spelling' : node.spelling,
             'location' : node.location,
             'extent.start' : node.extent.start,
             'extent.end' : node.extent.end,
             'is_definition' : node.is_definition(),
             'definition id' : get_cursor_id(node.get_definition()),
             'children' : children }

def dumpAST(cursor, verbose = False):
    if verbose:
        pprint(('nodes', get_info(cursor)))
    else:
        print(clang.asciitree.draw_tree(cursor, nodeChildren, printNode))

def decodeClangStr(byteName):

    return byteName.decode("utf-8")

def getCursorName(cursor):

    return decodeClangStr(cursor.displayname or cursor.spelling)

def getFirstChild(cursor):

    for child_node in cursor.get_children():

        return child_node;

def getPathToCursor(cursor, cursorPath):

    parentCursor = cursor.semantic_parent

    if parentCursor:

        if parentCursor.kind != clang.cindex.CursorKind.TRANSLATION_UNIT:

            cursorPath[0].append(getCursorName(parentCursor))

            if parentCursor.kind == clang.cindex.CursorKind.NAMESPACE:

                cursorPath[1].append(getCursorName(parentCursor))

            getPathToCursor(parentCursor, cursorPath)

def getCursorFilePath(cursor):

    return decodeClangStr(cursor.location.file.name)