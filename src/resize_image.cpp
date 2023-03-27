#include <cmath>
#include "image.h"

using namespace std;

#include <iostream>

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neighbor to pixel (x,y,c)
float Image::pixel_nearest(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  int X,Y;

  if(fmod(x,1.f)>=0.5f) X = floor(x)+1;
  else  X=floor(x);

  if(fmod(y,1.f)>=0.5f) Y= floor(y)+1;
  else  Y=floor(y);
  // TODO: Your code here
  
  return clamped_pixel(X,Y,c);
  
  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  
  // TODO: Your code here
  float A4 = (x-floor(x))*(y-floor(y));
  float A3 = (ceil(x)-x)*(y-floor(y));
  float A2 = (x-floor(x))*(ceil(y)-y);
  float A1 = (ceil(x)-x)*(ceil(y)-y);

  return A1*clamped_pixel(floor(x),floor(y),c) + A2*clamped_pixel(ceil(x),floor(y),c) + A3*clamped_pixel(floor(x), ceil(y),c) + A4*clamped_pixel(ceil(x),ceil(y),c);
  
  }

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  
  // TODO: Your code here
  
  float W_FACTOR  = (float) w/(float) im.w;
  float H_FACTOR = (float) h/(float) im.h;

  for(int n=0; n<ret.c; n++){
    for(int j=0; j<h; j++){
      for(int i=0; i<w; i++){
        ret(i,j,n) = im.pixel_nearest(((i + 0.5) / W_FACTOR) - 0.5,((j + 0.5) / H_FACTOR) - 0.5, n);
      }
    }
  }
  
  return ret;
  }


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {
  
  // TODO: Your code here
  Image ret(w,h,im.c);
  
  float W_FACTOR  = (float) w/ (float)im.w;
  float H_FACTOR = (float) h/ (float) im.h;

  for(int n=0; n<im.c; n++){
    for(int j=0; j<h; j++){
      for(int i=0; i<w; i++){
        ret(i,j,n) = im.pixel_bilinear(((i + 0.5) / W_FACTOR) - 0.5,((j + 0.5) / H_FACTOR) - 0.5, n);
      }
    }
  }
  
  
  return ret;
  }


