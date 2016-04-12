#!/usr/bin/perl

# Copyright (C) 2012, 2016, International Business Machines Corporation
# All Rights Reserved

use strict;
use Cwd 'realpath';
use File::Basename;

my $basetoolkitdir = realpath(dirname($0) . "/../../impl");

if ($ARGV[0] eq '-h')
{
	print "Usage: LibInfo.pl [-h] [includePath] [libPath] [lib]\n";
	exit;
}

my @includepaths = ("$basetoolkitdir/include","$ENV{'OPENCV_INSTALL_PATH'}/include");
my @libpaths = ("$basetoolkitdir/lib","$ENV{'OPENCV_INSTALL_PATH'}/lib");
#my @libs = ('dpsopencv', 'bz2', 'cv', 'highgui', 'cxcore', 'avcodec', 'avutil', 'microhttpd', 'curl'); # for OpenCV version 2.0
#my @libs = ('dpsopencv', 'bz2', 'opencv_core', 'opencv_imgproc', 'opencv_highgui', 'opencv_ml', 'opencv_features2d', 'opencv_video', 'opencv_objdetect', 'opencv_calib3d', 'opencv_flann', 'opencv_contrib', 'opencv_legacy', 'opencv_gpu', 'avcodec', 'avutil', 'microhttpd', 'curl'); # for OpenCV version 2.4
my @libs = qw( dpsopencv bz2 avcodec avutil microhttpd curl opencv_calib3d opencv_core opencv_features2d opencv_flann opencv_highgui opencv_imgcodecs opencv_imgproc opencv_ml opencv_objdetect opencv_photo opencv_shape opencv_stitching opencv_superres opencv_video opencv_videoio opencv_videostab ); # for OpenCV version 3.1

if ($ARGV[0] eq 'includePath') {
    foreach my $path (@includepaths) {
        print "$path\n";
    }
}
elsif ($ARGV[0] eq 'libPath') {
    foreach my $path (@libpaths) {
        print "$path\n";
    }
}
elsif ($ARGV[0] eq 'lib') {
    foreach my $lib (@libs) {
        print "$lib\n";
    }
}
else
{
    # Really can't get here
    exit 1;
}

exit 0;



