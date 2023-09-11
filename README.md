# LNK File Builder
Programmatically build a LNK file (shortcut file) that executes certain powershell script. Built mainly for spearphishing red-team engagement, with cpp and meson.

> Disclaimer:
> I, the creator of this program, has no responsibility for whatever you do with this tool. This tool was made solely for training and demonstration. Do anything at your own risk.

## Building / Installation

You can either build this tool from the source code with meson, or just simply downloading the precompiled binary. This tool currently only support 64 bit windows environment and I have no intention to support any other environment.

### Building From Source
Make sure you have meson installed, its easier to use meson rather than gcc or visual studio but you can choose anything you desire.

```
git clone https://github.com/idfp/lnk-builder
cd lnk-builder/
```
Build the project first before compiling it again with meson.
```
meson build
cd build
meson compile
```
If everything went smooth, you'll get an executable named `lnkbuilder.exe`.

### Precompiled Binary
Download the precompiled binary from the [Release Page](https://github.com/idfp/lnk-builder/releases/). And use it normally.

## Usage
```ruby
Usage: lnkbuilder.exe [Options]                                             
Options:
  --description / -d set description for lnk file.                                                 
  --payload / -p payload to be executed, must be powershell script.                                               
  --file / -f get payload from ps1 file.                                                 
  --icon / -i icon to be used.                                                 
  --output / -o name of output files, should ends with .lnk                                                   
    
Example: lnkbuilder.exe --payload "Start-Process notepad.exe" -o notepad.lnk                                                   
```