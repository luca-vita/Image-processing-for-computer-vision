#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>

#include "image.h"

using namespace std;

// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  
  // TODO: calculate the pixels of 'gray'
  
  float value;
  for(int  j=0; j<im.h; j++){
    for(int i=0; i<im.w; i++){
      value = im(i,j,0)*0.299 + im(i,j,1)*0.587 +im(i,j,2)*0.114;
      gray(i,j,0)=value;
    }
  }
  
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }




// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: shift all the pixels at the specified channel
  
  for(int  j=0; j<im.h; j++){
    for(int i=0; i<im.w; i++){
      im(i,j,c) = im(i,j,c)+v;
    }
  }
  
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  
  for(int  j=0; j<im.h; j++){
    for(int i=0; i<im.w; i++){
      im(i,j,c) = im(i,j,c)*v;
    }
  }
  
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  
  for(int  j=0; j<im.h; j++){
    for(int i=0; i<im.w; i++){
      for(int n=0; n<im.c; n++){
        im(i,j,n) = min(max(0.f, im(i,j,n)), 1.f);
      }
    }
  }
  
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to HSV format
  
  float V,m,C=0, S, H;
  for(int  j=0; j<im.h; j++){
    for(int i=0; i<im.w; i++){

      V = max(im(i,j,0), im(i,j,1), im(i,j,2));

      if( V==0 ){
        S = 0;
      }
      else{
        m=min(im(i,j,0), im(i,j,1), im(i,j,2));
        C = V-m;
        S = C/V;
      }

      if(C==0){
        H = 0;
      }
      else if(V==im(i,j,0)){
        H = (im(i,j,1)-im(i,j,2))/C;
      }
      else if(V==im(i,j,1)){
        H = (im(i,j,2)-im(i,j,0))/C + 2;
      }
      else if(V==im(i,j,2)){
        H = (im(i,j,0)-im(i,j,1))/C + 4;
      }

      if( H < 0){
        H = H/6 +1;
      }
      else{
        H = H/6;
      }

      im(i,j,0) = H;
      im(i,j,1) = S;
      im(i,j,2) = V;
    }
  }
  
  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from HSV format to RGB format
  float C,X,m;
  for(int  j=0; j<im.h; j++){
    for(int i=0; i<im.w; i++){
      C = im(i,j,2) * im(i,j,1);
      X = C * (1 - abs(fmod(6*im(i,j,0), 2) - 1));
      m = im(i,j,2)-C;

      float H = im(i,j,0);
      if(H<1.f/6.f && H>=0){
        im(i,j,0) = C+m;
        im(i,j,1) = X+m;
        im(i,j,2) = m;
      }
      else if(H<2.f/6.f && H>=1.f/6.f){
        im(i,j,0) = X+m;
        im(i,j,1) = C+m;
        im(i,j,2) = m;
      }
      else if(H<3.f/6.f && H>=2.f/6.f){
        im(i,j,0) = m;
        im(i,j,1) = C+m;
        im(i,j,2) = X+m;
      }
      else if(H<4.f/6.f && H>=3.f/6.f){
        im(i,j,0) = m;
        im(i,j,1) = X+m;
        im(i,j,2) = C+m;
      }
      else if(H<5.f/6.f && H>=4.f/6.f){
        im(i,j,0) = X+m;
        im(i,j,1) = m;
        im(i,j,2) = C+m;
      }
      else if(H<=1.f && H>=5.f/6.f){
        im(i,j,0) = C+m;
        im(i,j,1) = m;
        im(i,j,2) = X+m;
      }
    }
  }
  
  }



// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
