# foo_notification

Foobar 2000 component that allows you to show a Windows 10 styled toast notification showing current song with album art.

# Overview

This component allows user to show Windows Toast notification when song, album or artist changes.

![](http://i.imgur.com/NgmGKKP.png)

It is also possible to show the notification by hand, or to temporarily override the settings and show the notification on every song change AKA shuffle mode.
Another feature is a possibility to toggle between default playback mode and random playback mode.

![](http://i.imgur.com/h3RQ5yM.png)
All these options are available through context menu, but it is suggested to set up the keybindings.

# Download
You can download this component from the [release](https://github.com/icepopo/foo_notification/releases) section 

# Building
To build the project you need the following direcory structure with the foobar sdk present. The portable instalation is used for development.
    .
    ├── foobar2000         <-- SDK
    ├── foo_notification   <-- this repository
    ├── pfc                <-- SDK
    └── test               <-- portable instalation of foobar2000