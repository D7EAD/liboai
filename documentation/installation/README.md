<h1>Making Use of <code>liboai</code></h1>
<p>In order to integrate the power of artificial intelligence and <code>liboai</code> into your codebase, you have a couple of options.</p>

<h3>Integrate via source code</h3>
<p>As <code>liboai</code> implements a cURL wrapper internally and uses a pure C++ JSON solution, <code>liboai</code>'s header and implementation files can be added to an existing C++17 project and compiled alongside it. However, in order to do so, the project must have the following elements:</p>

* cURL available and <b>linked</b> to the project.
* nlohmann-json available.
* Compiled as C++17.

<p>Assuming your existing codebase has the above in mind, you can safely add <code>liboai</code>'s header and implementation files to your existing project and compile.</p>

<p>It's as easy as that!</p>

<h3>Integrate via a static/dynamic library</h3>
<p>Another means of integrating <code>liboai</code> into an existing C++17 project is as a static or dynamic library. This is slightly more complicated than simply including the source code of the library into your existing project, but can certainly be done in few steps.</p>

<p>Static and dynamic libraries take many forms:</p>

* <b>Windows</b>
  * Dynamic-Link Libraries (.dll)
  * Static Libraries (.lib)
* <b>Linux</b>
  * Shared Object (.so)
  * Static Library (.a)
* <b>MacOS</b>
  * Dynamic Library (.dylib)
  * Static Library (.a)

<p>However, their underlying concepts remain the same.</p>

<h3>Turning <code>liboai</code> into a library</h3>
<p>The process of compiling <code>liboai</code> into a static or dynamic library is not as hard as it may seem. Simply, using your IDE of choice, perform the following:

  1. Ensure cURL and nlohmann-json are installed.
  2. Create a new C++ project.
  3. Import the <code>liboai</code> source code (.cpp and .h files).
  4. *Link your project to the cURL library.
  5. *Make sure you are targetting C++17.
  6. *Compile as a static or dynamic library.
  
<p>Now, in the project you'd like to integrate <code>liboai</code> into:
  
  1. Include the <code>liboai</code> header files (.h files).
  2. *Link to the output static or dynamic library you compiled in the above steps.
  
*NOTE: how you do these steps depends on your choice of development environment. They can either be done in an IDE or a compiler on the command line.
