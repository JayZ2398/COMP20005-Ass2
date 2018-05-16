This is a little script that compares the outputs of your program to Alistair's sample outputs, and prints any differences side by side.

Instructions:
1. Place the 'Compare Outputs' folder in the same directory as your assignment .c file.
	- Make sure you copy the whole folder, and not just the files inside it.
	- You don't need to compile your program - this script will do it for you.
2. Rename your assignment to ass2.c.
3. Run the comparison as follows depending on your computer.
4. Any differences between your file will appear side by side.
5. If you get "No differences found :)" for all files, your output should exactly match the sample.
	* This may not include trailing whitespace.

Windows:
1. Open the MinGW shell, and cd to the 'Compare Outputs' folder.
2. Type 
	compare_ass2_win.txt

Mac/Dimefox/UNIX/etc:
1. Open Terminal/MinGW/any shell you like.
2. Type
	./compare_ass2_unix.txt
3. If this doesn't work, type
	chmod +x compare_ass2_unix.txt
then type
	./compare_ass2_unix.txt