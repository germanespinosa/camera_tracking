This repository contains code for high-speed mice tracking and image stitching
High speed mice trackng is performed by using background subtraction algorithm.
Camera homographies are estimated by placing Charuco boards within the field
of view of the cameras. For a brief overview about the system, visit my portfolio
(https://senthilpalanisamy.github.io./) and visit the post under high speed 
image stitching and mice tracking (A backup for the same has been added under 
the folder web-backups). The post would alsp point you to a daily medium blog 
post on image stitching. The blog post has also been added as images 
within the web-back ups folder



Installation:-
1. Full: Run `sudo ./setup/install_all` 
2. Custom:  
```
> sudo ./setup/install_prerequisites #install prerequisites
> sudo ./setup/install_cuda #install cuda drivers
> sudo ./setup/install_opencv #install opencv
> sudo ./setup/install_xclib #install epixi libraries
> sudo ./setup/install_xcap #install epixi xcap
```
   

A description of all executables:-
1. image_stitching - Stitch images from all four cameras and show them. This
                     script relies on homography calibrations that were made
                     previously using calibrate_homographies. As long as the
                     cameras have not moved physically, the homographies don't
                     need to be recalibrated. If the cameras have moved,
                     consider running calibrate_homographies before running
                     image_stitching. This script also relies on lens distortion
                     parameters of the camera.
2. sample_capture -  An executable for storing raw and stitched image feed from
                     cameras. This is useful when you are running an experiment
                     but you don't have online tracking but just to record data
                     for future references
3. calibrate_homography - Estimate the homographies between the camera and the
                          stitched image view. The default location for storing
                          the estimated homographies is ./config/homographies
4. online_tracking -  Main execute for doing online tracking. This script does
                      all of the following
                      1. Tracks the position of the mice online
                      2. Tracks the position of the robot online
                      3. Writes the raw data from the camera views into the 
                      folder specified initially
                      4. Stitches images togther and writes the stitched image
                      into the specified folder