cyaflight - cross-platform yaflight
===================================

cyaflight is a complete rewrite of yaflight, a flightgear launcher developed by me using Gambas3 language.

Current development status
--------------------------

cyaflight is stable enough for a basic use even if it misses some feature already available in yaflight.
By the end of the year I hope to completely sync the features provided by both, but I develop it in my free time
so I can't make any promises.

Why?
----

Gambas3 is a great programming language and software development environment allowing the creation of great applications 
for GNU/Linux systems. 
The problem is that Gambas3 apps cannot be executed by windows and macosx users, that's why I felt the need 
to rewrite the launch control using C++ and the Qt framework.
Consider that many users asked me how they could be able to use yaflight on Windows.

So, what happens to yaflight?
-----------------------------

YaFlight will continue to exist but maintaining two code bases is to much effort to me
consequently it will be abandoned soon or later (consider please that cyaflight can be executed 
even on GNU/Linux and BSD systems, so I'll focus on adding features and maintaining the new one).

In the future cyaflight will replace yaflight entirely.

Minimalist
----------

cyaflight (like yaflight) aims to be a minimalist, fast and easy to use launch control for FlightGear (http://www.flightgear.org).

Configuration file format
-------------------------

yaflight and cyaflight configuration files use the same format except for the CUSTOMSCENERY keyword replaced by
 CUSTOMSCENERIES. This way migration will be easier.

zlib
====

cyaflight uses zlib to work with compressed files.
http://www.zlib.net/
