#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#include <iostream>

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image &im) {

    // TODO: Normalize each channel
    float sum = 0;
    for(int c=0; c<im.c; c++){
        for(int j=0; j<im.h; j++){
            for(int i=0; i<im.w; i++){
                sum += im(i,j,c);
            }
        }
    }
    for(int c=0; c<im.c; c++){
        for(int j=0; j<im.h; j++){
            for(int i=0; i<im.w; i++){
                im(i,j,c) = im(i,j,c)/sum;
            }
        }
    }

}

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w) {
    assert(w % 2); // w needs to be odd

    // TODO: Implement the filter
    Image filter(w,w);

    float  gigi = 1.f /((float)(w*w));
    for(int i=0; i<w; i++){
        for(int j=0; j<w; j++){
            filter(i,j) = gigi;
        }
    }

    return filter;
}

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image(const Image &im, const Image &filter, bool preserve) {
    assert(filter.c == 1);
    Image ret;
    // This is the case when we need to use the function clamped_pixel(x,y,c).
    // Otherwise you'll have to manually check whether the filter goes out of bounds

    // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,h,c) to reset ret
    // TODO: Do the convolution operator

    ret.w = im.w;
    ret.h = im.h;
    ret.c = preserve ? im.c : 1;

    ret.data = (float*) calloc(ret.w*ret.h*ret.c, sizeof(float));
    memset(ret.data, 0, ret.w*ret.h*ret.c*sizeof(float));

    for(int c=0; c<im.c; c++){
        for(int j=0; j<im.h; j++){
            for(int i=0; i<im.w; i++){
                float nuovo=0;
                for(int fj=0; fj<filter.h; fj++){
                    for(int fi=0; fi<filter.w; fi++){
                        nuovo += filter.clamped_pixel(fi,fj)*im.clamped_pixel(i-(filter.w-1)/2 + fi, j-(filter.h-1)/2+fj,c);
                    }
                }
                if(!preserve){
                    ret(i,j) += nuovo;
                }
                if(preserve)
                    ret(i,j,c) = nuovo;
            }
        }
    }

    // Make sure to return ret and not im. This is just a placeholder
    return ret;
    
}

// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter() {
    // TODO: Implement the filter
    Image filter(3,3);

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if((i==0 || i==2) && (j==0 || j==2)) filter(i,j) = 0;
            if( i == 1 && j != 1 || j==1 && i!=1) filter(i,j) = -1;
            if( i==1 && j==1) filter(i,j) = 4;
       }
    }


    return filter;

}

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter() {
    // TODO: Implement the filter
    Image filter(3,3);

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if((i==0 || i==2) && (j==0 || j==2)) filter(i,j) = 0;
            if( i == 1 && j != 1 || j==1 && i!=1) filter(i,j) = -1;
            if( i==1 && j==1) filter(i,j) = 5;
       }
    }


    return filter;

}

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter() {
    // TODO: Implement the filter
    Image filter(3,3);

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if((i==0  && j == 2 || i==2 && j==0)) filter(i,j) = 0;
            else if(i==0  && j == 0) filter(i,j) = -2;
            else if(i==2 && j==2) filter(i,j) = 2;
            else if( i == 1 && j == 0|| j==1 && i==0) filter(i,j) = -1;
            else filter(i,j) = 1;
       }
    }


    return filter;

}

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma) {
    // TODO: Implement the filter
    int dim = ((int)ceil(6*sigma)%2 == 1) ? ceil(6*sigma) : ceil(6*sigma)+1;
    Image filter(dim,dim);

    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            int x = i-dim/2;
            int y = j-dim/2;
            filter(i,j) = exp( (-(x*x)-(y*y))/(2*sigma*sigma))/(2*M_PI*sigma*sigma) ;
       }
    }

    l1_normalize(filter);
    return filter;

}


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size

    // TODO: Implement addition
    Image ret(a.w, a.h, a.c);

    for(int c=0; c<a.c; c++){
        for(int i=0; i<a.w; i++){
            for(int j=0; j<a.h; j++){
                ret(i,j,c) = a.clamped_pixel(i,j,c)+b.clamped_pixel(i,j,c);
            }
        }
    }

    return ret;

}

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size

    // TODO: Implement subtraction
    Image ret(a.w, a.h, a.c);

    for(int c=0; c<a.c; c++){
        for(int i=0; i<a.w; i++){
            for(int j=0; j<a.h; j++){
                ret(i,j,c) = a.clamped_pixel(i,j,c)-b.clamped_pixel(i,j,c);
            }
        }
    }

    return ret;

}

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter() {
    // TODO: Implement the filter
    
    Image filter(3,3);

    for(int i=0; i<3; i++){
        int pippi = i==1?2:1;
        for(int j=0; j<3; j++){
            filter(i,j) =  (j-1)*pippi;
       }
    }

    return filter;
}

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter() {
    // TODO: Implement the filter
    Image filter(3,3);

    for(int j=0; j<3; j++){
        int pippi = j==1?2:1;
        for(int i=0; i<3; i++){
            filter(i,j) =  (i-1)*pippi;
       }
    }

    return filter;
}

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image &im) {
    assert(im.w * im.h); // assure we have non-empty image

    // TODO: Normalize the features for each channel
    for(int c=0; c<im.c; c++){
        float mini = im(0,0,c);
        float maxi = im(0,0,c);
        for(int i=0; i<im.w; i++){
            for(int j=0; j<im.h; j++){
                mini = min(im(i,j,c), mini);
                maxi = max(im(i,j,c), maxi);
            }
        }
        float C=maxi-mini;
        for(int i=0; i<im.w; i++){
            for(int j=0; j<im.h; j++){
                if(C==0)
                    im(i,j,c) = 0;
                else
                    im(i,j,c) = (im(i,j,c)-mini)/C;
            }
        }
    }

}


// Normalizes features across all channels
void feature_normalize_total(Image &im) {
    assert(im.w * im.h * im.c); // assure we have non-empty image

    int nc = im.c;
    im.c = 1;
    im.w *= nc;

    feature_normalize(im);

    im.w /= nc;
    im.c = nc;

}


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image, Image> sobel_image(const Image &im) {
    // TODO: Your code here
    Image Gx = convolve_image(im, make_gx_filter(), false);
    Image Gy = convolve_image(im, make_gy_filter(), false);
    
    Image modulo(im.w, im.h);
    Image fase(im.w, im.h);

    for (int i = 0; i < im.w; i++) {
    for (int j = 0; j < im.h; j++) {
        float gx = Gx(i, j, 0);
        float gy = Gy(i, j, 0);
        modulo(i, j, 0) = sqrtf(gx * gx + gy * gy);
        fase(i, j, 0) = atan2f(gy, gx)/ (2.f * M_PI) + 0.5;
    }
  }
  feature_normalize(modulo);
  feature_normalize(fase);

  return {modulo, fase};
}


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image &im) {

    // TODO: Your code here
    Image gaussed = convolve_image(im, make_gaussian_filter(4), true);

    pair<Image, Image> coppia = sobel_image(gaussed);
    
    Image colorized(im.w, im.h, 3);
    for(int c=0; c<3; c++){
        for(int i = 0; i<im.w; i++){
            for(int j=0; j<im.h; j++){
                if(c==0){
                    colorized(i,j,c) = coppia.second(i,j);
                }
                else{
                    colorized(i,j,c) = coppia.first(i,j);
                }
            }
        }
    }
    hsv_to_rgb(colorized);

    return colorized;
}


// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image &im, float sigma1, float sigma2) {
    Image bf = im;

    // TODO: Your bilateral code
    NOT_IMPLEMENTED();

    return bf;
}


// HM #5
//
float *compute_histogram(const Image &im, int ch, int num_bins) {
    float *hist = (float *) malloc(sizeof(float) * num_bins);
    for (int i = 0; i < num_bins; ++i) {
        hist[i] = 0;
    }

    // TODO: Your histogram code
    NOT_IMPLEMENTED();

    return hist;
}

float *compute_CDF(float *hist, int num_bins) {
    float *cdf = (float *) malloc(sizeof(float) * num_bins);

    cdf[0] = hist[0];

    // TODO: Your cdf code
    NOT_IMPLEMENTED();

    return cdf;
}

Image histogram_equalization_hsv(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // TODO: Your histogram equalization code
    NOT_IMPLEMENTED();
    // convert to hsv
    // compute histograms for the luminance channel
    // compute cdf
    // equalization
    // convert back to rgb

    // delete the allocated memory!
//    delete hist;
//    delete cdf;

    return new_im;
}

Image histogram_equalization_rgb(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // compute histograms for each color channel
    for (int c = 0; c < im.c; ++c) {

        // TODO: Your equalization code
        NOT_IMPLEMENTED();

        // delete the allocated memory!
//        delete hist;
//        delete cdf;
    }

    return new_im;
}


// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }

void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }

void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image &a, const Image &b) { return sub_image(a, b); }

Image operator+(const Image &a, const Image &b) { return add_image(a, b); }
