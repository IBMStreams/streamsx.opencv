Using the OpenCV Toolkit
========================

After [installing the OpenCV toolkit](InstallingToolkit.html) and its dependencies, you can verify that its operators work correctly by running some of the [sample applications](SampleApplications.html) that are included in the toolkit. The sample applications, which are compiled when you install the toolkit, illustrate how its [operators and types](OperatorsAndTypes.html) can be used in Streams applications.

If you have a graphical desktop (for example, the Gnome Desktop) on your machine, then you can use Streams Studio to view the source code for the sample applications and then launch them. Or if not, you can run some of the sample applications from a Linux command prompt, as shown in 'Verify the toolkit' below.

To use the OpenV toolkit in your own applications, you need to add an SPL ’use’ statement at the beginning of your source code, and specify the directory containing the toolkit when compiling your source code, as shown in 'Use the toolkit with Streams' below.

set environment variables
-------------------------

The OpenCV toolkit requires that you set Linux environment variables to identify the directory where the FFmpeg and OpenCV libraries will be installed. You can set them by copying these lines into your ’$HOME/.bashrc’ script:

        export OPENCV_INSTALL_PATH=/usr/local
        export LD_LIBRARY_PATH=$OPENCV_INSTALL_PATH/lib:$LD_LIBRARY_PATH 

After changing your ’$HOME/.bashrc’ script, you should log off Linux and log back in, to ensure that the new environment variables are set in all shells, including your window manager.

verify the toolkit
------------------

If you have access to a graphical desktop on your machine, then you can verify that the OpenCV toolkit is installed properly by running the sample application `file.spl`. This application will open a window on the desktop and replay a short video in it:

       cd .../streamsx.opencv
       ./samples/output/file/bin/standalone -t 3

**Please note:** If you access your machine’s desktop remotely, for example with X11 or VNC, network bandwidth and latency may limit the video refresh rate and introduce jitter.

If you have access to your machine via a web browser, then you can verfiy that the OpenCV toolkit is installed properly by running the sample application `video_stream.spl`. This application will start an embedded web server and replay a short video on HTTP port 9001:

       cd .../streamsx.opencv
       ./samples/output/video_stream/bin/standalone -t 3

To connect a web browser such as [Mozilla Firefox](https://www.mozilla.org/) or a video viewer such as the [VLC media player](http://www.videolan.org/) to the embedded web server, go here:

       http://your.machine.com:9001/

**Please note:** You may need to change your machine’s firewall settings to allow remote clients access to TCP port 9001.

If there is a camera attached to your machine (see ’Use the toolkit with cameras’ below), you can verify that the OpenCV toolkit can capture video from it by running the sample application `cam.spl`:

       cd .../streamsx.opencv
       ./samples/output/cam/bin/standalone -t 3 

You can verify that the OpenCV toolkit can capture video from remote web servers by running the sample application `url.spl`:

       cd .../streamsx.opencv
       ./samples/output/url/bin/standalone -t 3 

You can verify that operators can be composed and executed on separate threads by running the sample application `faces_edges_cam.spl` (if there is a camera attached to your machine) or `faces_edges_file.spl` (if you have access to HTTP port 9001 on your machine from a web browser or video viewer).

       cd .../streamsx.opencv
       ./samples/output/faces_edges_cam/bin/standalone -t 3 
       ./samples/output/faces_edges_file/bin/standalone -t 3 

There are many more sample applications included in the toolkit that illustrate how the operators can be used. Each sample application is described in [Sample Applications](SampleApplications.html).

use the toolkit with Streams
----------------------------

All of the operators and types in this toolkit are defined in the Streams namespace ’com.ibm.streamsx.opencv’. To use them in an SPL application, include this statement at the top of your SPL or SPLMM source file:

        use com.ibm.streamsx.opencv::*;

If you compile your SPL applications with the `sc` command, then you will need to specify the location of the toolkit with the `-t` option, like this:

        sc -M yournamespace::yourapplication -t .../streamsx.opencv/com.ibm.streamsx.opencv ... more options ...

If you build your SPL applications in Streams Studio, and you want to make the toolkit available to all Streams projects in your Eclipse workspace, then you can add the toolkit’s location to Studio like this:

- launch Streams Studio

- switch to the InfoSphere Streams perspective

- go to the ’Streams Explorer’ pane

- expand ’InfoSphere Streams 4.1.1.0’

- right-click on ’Toolkit Locations’ and select ’Add Toolkit Location ...’

- click the ’Directory...’ button

- navigate to the directory `.../streamsx.opencv/com.ibm.streamsx.opencv‘ and click ’OK’

Or, if you build your SPL applications in Streams Studio, and you want to import the operators’ code generation templates into your Eclipse workspace as a Streams project, then do this instead:

- launch Streams Studio

- switch to the InfoSphere Streams perspective

- go to the ’Project Explorer’ pane

- click ’New –> Import ...’ in the Eclipse menu bar

- expand ’General’

- select ’Existing projects into workspace ...’ and click ’Next’

- select ’Select root diretory’ and click the ’Browse...’ button

- navigate to the `.../streamsx.opencv‘ directory and click ’OK’

- select the project `com.ibm.streamsx.opencv‘ and click ’Finish’

use the toolkit with cameras
----------------------------

You can use cameras that are attached directly to your machine with the toolkit.

Linux represents cameras as devices named ‘/dev/videoX‘, where ’_X_’ is a positive integer. The camera number ’_X_’ is specified with the ’index’ parameter of the `CaptureFromCAM` operator.

You can list the cameras attached to your machine with these commands:

       lsusb | sort 
       ls -1 /dev/video*

Most video cameras can be configured to produce images in a variety of frame sizes, and at varying rates. You can display the current configuration of camera ’_X_’ with this command:

       v4l2-ctl -d/dev/videoX  --list-formats --all

The camera’s frame size and rate can be configured with the `width`, `height`, and `fps` parameters of the `CaptureFromCAM‘ operator. You can list the frame sizes and rates that camera ’_X_’ supports with this command:

       v4l2-ctl -d/dev/videoX  --list-formats-ext

If your camera supports both the ’YUYV’ and ’MJPG’ pixel formats, use the frame sizes and rates listed under the ’MJPG’ pixel formats with the `CaptureFromCAM` operator.

----

&copy; Copyright 2012, 2016, International Business Machines Corporation, All Rights Reserved 
