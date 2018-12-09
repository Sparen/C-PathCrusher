# C Path Crusher

This is a utility that crushes paths down while maintaining their accuracy (Unix style paths only). It was originally developed on the side during a PintOS assignment in my Operating Systems course. Note that due to standard library differences and other requirements, the version that ended up in the actual project has notable differences from this implementation. This code is meant for usage on a Unix style machine and was tested exclusively on macOS High Sierra.

This utility takes a relative or absolute path, and then does simplification on the path.

As quoted from the assignment:

> How should a file name like a//b be interpreted?
Multiple consecutive slashes are equivalent to a single slash, so this file name is the same as a/b.

> How about a file name like /../x?
The root directory is its own parent, so it is equivalent to /x.

This utility supports the following (where [] is a stand-in for the rest of the path):
* "/..[]" -> "/[]"
* "./[]" -> "[]"
* "[]/./[]" -> "[]/[]"
* "[]//[]" -> "[]/[]"
* "[]/." -> "[]/"
* "[]/a/../[]" -> "[]/[]"

Please note that this code is garbage and was thrown together haphazardly. Not all edge cases were tested. It is not meant for production systems unless you want to risk something going horribly wrong. The '..' simplifier naturally does not work on relative paths - only absolute paths.
