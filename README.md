This repository contains the source files for the "Let's Build a Linux Shell" tutorial.

# How to use the code

You need to download this repository to your computer, then compile and run the code.

## Downloading the repository

Click on the green "Clone or Download" button on the top-right, then click on "Download Zip" to save the file on your computer.

## Extracting the archive

Open a terminal emulator (GNOME Terminal, Konsole, XTerm, or what-have-you), and navigate to the directory where you downloaded the ZIP file:
```
cd ~/path/to/zip
```
Extract the archive and navigate to the extracted directory:
```
unzip lets-build-a-linux-shell-master.zip
```
## Compiling the shell
First, navigate to the directory in which you've extracted the downloaded archive:
```
cd lets-build-a-linux-shell-master/
```
If you want to compile and run the shell from Part II/III of the tutorial, you can do so by navigating to the subdirectory and then running `gcc`:
```
cd part2-3/
gcc -o shell main.c source.c scanner.c parser.c node.c executor.c prompt.c
```
If you want to compile Parts IV and above, navigate to the subdirectory and run `make`:
```
cd part4/
make
```
## Running the shell
Now you can run the shell by invoking:
```
./shell
```
