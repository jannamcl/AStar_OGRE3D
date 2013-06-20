This code DOES compile, however,
must install Ogre 3D SDK for this code to 
compile and run!

Keep in mind I have uploaded a video to YouTube so you
can see the coding sample running:

http://www.youtube.com/watch?v=WXr3IaX0Xao

The remainder of this file can be completely ignored if
you have decided not to compile/run the code.

---

About OGRE3D:

OGRE SDK main site:
http://www.ogre3d.org/

Download SDK:
http://www.ogre3d.org/download/

---

project settings:

I have setup the project settings using Windows 7 with 
Visual Studio 2010

these settings may not work on your machine

ogre has a "setting up an application" section

in case the project is not compiling/running properly,
try reconfiguring the project settings:

go to 
	http://www.ogre3d.org/tikiwiki/Setting+Up+An+Application

and select the operating system and environment you wish to compile
the code in on your machine

---

the project settings for setting up an application with windows 7 and
visual studio 2010 can be found here:

http://www.ogre3d.org/tikiwiki/Setting+Up+An+Application+-+Visual+Studio

PLEASE NOTE: these settings are not completely correct with a newly
installed OGRE SDK:

the ogre sdk installs the "bin" directory, but the website will capitalize
included settings to make ogre look in the "Bin" directory

for example
the website has you change the following "Debugging Configuration Properties" 
for Command to
	$(OGRE_HOME)\Bin\$(Configuration)\$(ProjectName).exe

that should actually be this:
	$(OGRE_HOME)\bin\$(Configuration)\$(ProjectName).exe

Keep in mind that there are a few other places that this needs to 
be changed where it should be "bin", not "Bin"
---

possible .dll issue when compiling:

	"d3dx9_42.dll missing"

Solution:

need the "DirectX End-User Runtime Web Installer"

this can be easily searched for on the microsoft/downloads
website or can be found here: 
http://www.microsoft.com/download/en/details.aspx?id=35

(if there is a Runtime error saying "d3dx9_42.dll missing",
this is what is needed. just install off of the microsoft website
and running the project again should solve the problem).

---

if there are any other missing .dll files or other linking/run time errors,
these issues can be easily searched for throughout the web by
searching "Ogre" + the error message or missing .dll file (etc.)

---

if using visual studio 2010 in windows and getting error

"MSVCRTD.lib(crtexe.obj) : error LNK2019: unresolved external symbol 
_main referenced in function ___tmainCRTStartup"

go to project properties, select "all configurations" from the
"Configuration" drop down menu at the top, then go to 
"Configuration Properties> Linker> System"
and under SubSystem, change to "Windows" or "console"
depending on which one is currently set. This usually seems to 
do the trick.