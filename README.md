yaflight
========

![yaflight logo](https://bitbucket.org/_pmat_/yaflight/raw/be531923896f07ac28983e15cb93a781c1499b65/icons/yaflight-logo2013-128px.png)

yaflight is a flightgear launch control developed originally using the Gambas3 language
and now completely rewritten using C++ with the Qt Framework to make it multi-platform.

Current development status
--------------------------

yaflight series 0.99 uses yalib, a shared library also used by yainstall to access FlightGear related informations.

Why this total rewrite?
-----------------------

Gambas3 is a great programming language and software development environment allowing the creation of great applications 
for GNU/Linux systems. 
The problem was that Gambas3 apps can't be executed by windows and macosx users, that's why I felt the need 
to rewrite the launch control using C++ and the Qt framework.

So, what happens to yaflight?
-----------------------------

The old YaFlight series (0.1.x) was renamed to fglc and it's still available here https://sourceforge.net/projects/fglc.
Consider please that maintaining two code bases is too much effort to me
consequently the gambas3 version will be abandoned, soon or later.
The new YaFlight implementation provides almost the same features and can be executed also on GNU/Linux and BSD systems:
that's why I'll focus on adding features and maintaining YaFlight only.

Minimalist
----------

yaflight (like fglc) aims to be a minimalist, fast and easy to use launch control for FlightGear (http://www.flightgear.org).

Configuration files - compatibility
-----------------------------------

There's no more compatibility between yaflight and fglc.
yaflight and fglc configuration files are now quite different and they are stored in different locations.

zlib
====

yaflight uses zlib to work with compressed files.
http://www.zlib.net/

icons
=====

Some of the icons used by yaflight belongs to the kde theme (http://www.kde.org) and to the gnome theme (http://www.gnome.org).
Custom icons built from scratch are work in progress.
