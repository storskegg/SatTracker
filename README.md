# SatTracker

SatTracker was written by @Tysonpower, and is freely available to download as a zip from [this blog article](https://tysonpower.de/blog/diy-az-el-antenna-rotator-for-under-150). 

As @Tysonpower states in the blog,

>The code i wrote is not bug free nor optimal but it works. You can download it if you want...

Since I couldn't fork his work on Github, I've created this initial repo to keep track of the changes I make.

I do not know how @Tysonpower wishes it to be licensed, so I licensed it Apache-2.0 to protect any of their own original work while making it open and available, and will change the license if I'm asked to.

## Communications

Serial communications with the controller are performed by an implementation of Yaesu's GS232B protocol, and therefore _should_ be easily controllable from any device or software capable of controlling Yaesu rotators.
