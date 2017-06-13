#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("usage: DisplayImage.out <Image_Path>\n");
    return EXIT_FAILURE;
  }
  Mat image;
  image = imread(argv[1], 1);
  if (!image.data) {
    printf("No image data \n");
    return EXIT_FAILURE;
  }
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", image);
  waitKey(0);
  return EXIT_SUCCESS;
}
