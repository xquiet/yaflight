yaflight
========

![yaflight logo](https://bitbucket.org/_pmat_/yaflight/raw/be531923896f07ac28983e15cb93a781c1499b65/icons/yaflight-logo2013-128px.png)

yaflight is a flightgear launch control developed originally using the Gambas3 language
and now completely rewritten using C++ with the Qt Framework to make it multi-platform.

Current development status
--------------------------

yaflight series 0.99 (aka cyaflight) is stable enough for a basic usage even if it misses 
some feature already available in yaflight series 0.1.
By the end of the year I hope to completely sync the features provided by both, but I develop it in my free time
so I can't make any promises.

Why this total rewrite?
-----------------------

Gambas3 is a great programming language and software development environment allowing the creation of great applications 
for GNU/Linux systems. 
The problem is that Gambas3 apps cannot be executed by windows and macosx users, that's why I felt the need 
to rewrite the launch control using C++ and the Qt framework.
Consider that many users asked me how they could be able to use yaflight on Windows.

So, what happens to yaflight?
-----------------------------

The old YaFlight series (0.1.x) was renamed to fglc and it's still available here https://sourceforge.net/projects/fglc.
Consider please that maintaining two code bases is too much effort to me
consequently the gambas3 version will be abandoned, soon or later (the new yaflight implementation
can be executed even on GNU/Linux and BSD systems, so I'll focus on adding features and maintaining the new one).

Minimalist
----------

yaflight (like fglc) aims to be a minimalist, fast and easy to use launch control for FlightGear (http://www.flightgear.org).

Configuration file format
-------------------------

yaflight and fglc configuration files are now quite different and they are stored in different locations.

zlib
====

cyaflight uses zlib to work with compressed files.
http://www.zlib.net/

icons
=====

Some of the icons used by yaflight belongs to the kde theme (http://www.kde.org) and to the gnome theme (http://www.gnome.org).
