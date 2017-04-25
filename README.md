Integrating Chromium Embedded Framework (CEF) in a sample MFC application
=========================================================================

## Introduction

CEF allows a Chromium browser to be embedded into an application.
The CEF distributions are available [here](http://opensource.spotify.com/cefbuilds/index.html).
The examples in the CEF distributions do not demonstrate the integration of CEF in MFC Windows applications.
This example shows how CEF can be integrated in a sample MFC application.

## Features

The following CEF features are demonstrated.

- Creating a browser window and loading a web page
- Common browser actions, such as back, forward, reload and stop
- Zooming in and out
- Finding a word or a phrase on the page
- Passing a URL parameter to the command line: --url=<URL>

## Building the application

A Visual Studio 2015 solution is provided.
The CEF distribution is not included because it is being updated frequently and it has a large size.

The CEF header files need to be included in the project folder.
The import library libcef.lib is available in the CEF distribution.
The static library libcef_dll_wrapper.lib needs to be built from the CEF distribution.
The application must link to libcef.lib and libcef_dll_wrapper.lib.
Please refer to the readme text files in the repository for more information.

The CEF binary files (DLLs, locale files, etc.) must be placed in the same folder as the application.

## Issues

The following issues occur only in the Debug version.

- The Debug version of the application does not work.
- Calling CefShutdown() triggers an exception in the Debug version.
