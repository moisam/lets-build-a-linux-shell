# lets-build-a-linux-shell
Repository code for the "Let's Build a Linux Shell" tutorial

# How to use the code
First, you need to download a copy of this repository to your local machine. Click on the green "Clone or Download" button on the top-right, then click on "Download Zip" to save the file on your computer.
Now open a terminal emulator (GNOME Terminal, Konsole, XTerm, or what-have-you), and navigate to the directory where you downloaded the ZIP file:
`
cd ~/path/to/zip
`
Extract the archive and navigate to the extracted directory:
`
unzip lets-build-a-linux-shell-master.zip
cd lets-build-a-linux-shell-master/
`
If you want to compile and run the shell from Part II/III of the tutorial, you can do so by navigating to the subdirectory and then running `gcc`:
`
cd part2-3/
gcc -o shell main.c source.c scanner.c parser.c node.c executor.c prompt.c
`
Now you can run the shell by invoking:
`
./shell
`
