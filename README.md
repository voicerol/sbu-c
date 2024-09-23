# sbu-c
Development of Bash text utilities: cat, grep.

### Dependencies
- Work only in based UNIX system.
- The make utility must be installed
- To format the code style you will need the clang-format option

### History of cat 

> cat was part of the early versions of Unix, e.g., Version 1, and replaced pr, a PDP-7 and Multics utility for copying a single file to the screen.

### Usage of cat

Cat is one of the most frequently used commands on Unix-like operating systems. It has three related functions with regard to text files: displaying them, combining copies of them and creating new ones.

`cat [OPTION] [FILE]...`

### cat options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display end-of-line characters as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display tabs as ^I  |

### History of grep 

> Thompson wrote the first version in PDP-11 assembly language to help Lee E. McMahon analyse the text of the Federalist Papers to determine the authorship of each paper. The text editor ed (also written by Thompson) had support for regular expressions, but could not be used on such a large amount of text, so Thompson extracted this code into a standalone tool. He chose the name because in ed, the command g/re/p would print all lines matching a given pattern. 
grep was first included in version 4 of Unix. Noting that it is "generally cited as the prototypical software tool", McIlroy credited grep with "irrevocably entrenching" Thompson's tool philosophy in Unix.

### Usage of grep 

`grep [options] template [file_name]`

### grep options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | pattern |
| 2 | -i | Ignore uppercase vs. lowercase.  |
| 3 | -v | Invert match. |
| 4 | -c | Output count of matching lines only. |
| 5 | -l | Output matching files only.  |
| 6 | -n | Precede each matching line with a line number. |
| 7 | -h | Output matching lines without preceding them by file names. |
| 8 | -s | Suppress error messages about nonexistent or unreadable files. |
| 9 | -f file | Take regexes from a file. |
| 10 | -o | Output the matched parts of a matching line. |
