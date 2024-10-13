Integrating Chromium Embedded Framework (CEF) in a sample MFC application
=========================================================================

## Introduction

CEF allows a Chromium browser to be embedded into an application.
Please refer to [this page](https://github.com/chromiumembedded/cef) for more information on building CEF from source or getting its binary distributions.
This example shows how CEF can be integrated in a sample MFC application for Windows.

## Features

The following CEF features are demonstrated.

- Creating a browser window and loading a web page
- Common browser actions, such as back, forward, reload and stop
- Zooming in and out
- Finding a word or a phrase on the page (does not work in this feature branch)
- Passing a URL parameter to the command line: --url=<URL>

## Building the application

Prerequisites:
- [CMake](https://cmake.org/) (minimum version: 3.21)
- Visual Studio 2022 with C++ support ([Community Edition](https://visualstudio.microsoft.com/vs/community/) can be used too)

A Visual Studio 2022 solution is provided.
A CEF binary distribution is included in this feature branch.

The CEF header files need to be included in the project folder.
The import library libcef.lib and the corresponding DLL, libcef.dll are available in the CEF distribution.
The static library libcef_dll_wrapper.lib needs to be built from the CEF distribution.
The application must link to libcef.lib and libcef_dll_wrapper.lib.
Please refer to the readme text files in the repository for more information.
The CEF binary files (DLLs, locale files, etc.) must be placed in the same folder as the application.

A build script, build.bat is provided to automate the whole process of building solutions and copying the necessary files.
Please update build.bat to specify the correct paths to CMake and Visual Studio MSBuild before running the script.
Another script, clean.bat is provided to clean all build files.

## Issues

The following issues occur in this feature branch. There are no solutions yet.

- Sometimes, the content of a page is not properly displayed.
- The find feature does not work. Words or phrases that match keywords are not highlighted.
- The browser window covers all buttons and textboxes on launch. After the application window is resized, the buttons and the textboxes will be displayed.
