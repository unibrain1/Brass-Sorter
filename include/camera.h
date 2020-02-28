#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


/*
   Functions to handle the JeVois camera
*/

// #define DEBUG // Debug messages to the Serial Port

void initCamera();
char * readCamera();
void recvWithStartEndMarkers();
void cameraFlush();

#endif