---
title: Using the Cryptolens C++ client on Windows
author: Martin Svedin
description: How to use Cryptolens C++ client on Windows with Visual Studio
labelID: web_api
---

# Windows support with Visual Studio

It is fairly simple to get started with Cryptolens client for C++ on Windows. 

> Note: this version of the library is specifically built for Windows since it uses native Windows call.

## Getting the library

### Building the library yourself

Instructions for building the library and creating *CryptolensSKM.lib* using Visual Studio:

  1. Open the *CryptolensSKM.sln* solution file.
  1. Select the desired platform and configuration in the Visual Studio GUI
  1. Build the solution (Build -> Solution).
  1. *CryptolensSKM.lib* can now be found in the *Output\\\<Platform\>\\\<Configuration\>* directory.

### Using in your own project

To use the library in your own Visual Studio projects two things need to be
done. First, Visual Studio needs to be told to use include files from the
*cryptolens-cpp\include* directory, and secondly, the linker in Visual Studio
needs to be set up to link against CryptolensSKM.lib.

Example projects can be found in *cryptolens-cpp\examples\Visual Studio\* folder.
All of the projects are set up to work with the *CryptolensSKM.sln* file, i.e. it looks for
*CryptolensSKM.lib* in *cryptolens-cpp\Output\\\<Platform\>\\\<Configuration\>*.

### Setting up include directories

  1. Right-click on the project in in Visual Studio and select Properties.

     ![Right-click on project](images/step1.png)
  1. The path to *cryptolens-cpp\include* needs to be added under *Configuration Properties ->
     C\C++ -> General -> Additional Include Directories*.

     Thus if the cryptolens-cpp git repository was cloned at
     ```
     C:\Users\<user>\Documents\Visual Studio 2017\Projects\cryptolens-cpp
     ```
     this path would be added under *Additional Include Directories*.

     Alternatively, a relative path can be used as in the Example1 project:

<img src="images/step2.png" style="width:100%">
<!--![Additional Include Directories](/images/step2.png)-->

### Setting up the linker

  1. Right-click on the project in Visual Studio and select Properties.
  1. Under *Configuration Properties -> Linker -> Input -> Additional Dependencies*
     we need to add "winhttp.lib;CryptolensSKM.lib" to the existing list of
     dependencies.

     I.e. if this field already contains the value
     ```
     kernel32.lib;%(AdditionalDependencies)
     ```
     the new value would be:
     ```
     winhttp.lib;CryptolensSKM.lib;kernel32.lib;%(AdditionalDependencies)
     ```

     The settings should thus look something like the following:
<img src="images/step3.png" style="width:100%">
<!--![Additional Dependencies](/images/step3.png)-->

  1. Under *Configuration Properties -> Linker -> General -> Additional Library
     Directories* we need to add the directory containing *CryptolensSKM.lib*.

     If we cloned the github repository at
     ```
     C:\Users\<user>\Documents\Visual Studio 2017\Projects\cryptolens-cpp
     ```
     and we built the library as described above, then we would add
     ```
     C:\Users\<user>\Documents\Visual Studio 2017\Projects\cryptolens-cpp\vsprojects\Output\$(Platform)\$(Configuration)\
     ```
     to the *Additional Library Directories*, assuming your project uses the default
     Release/Debug configurations in Visual Studio.

     The Example1 project uses the following relative path, which also works:

<img src="images/step4.png" style="width:100%">
<!--![Additional Library Directories](/images/step4.png)-->
