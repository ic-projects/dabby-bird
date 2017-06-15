
#include <stdio.h>
#define BLUR_RADIUS 2

void swap(unsigned char *array_list, int i, int j) {
  unsigned char temp = array_list[i];
  array_list[i] = array_list[j];
  array_list[j] = temp;
}

int partition(unsigned char *array_list, int left, int right) {
  int pivot = array_list[left];
  int i = left;
  int j = right;

  while (i < j) {
    for (; i <= right && array_list[i] <= pivot; i++);
    for (; array_list[j] > pivot; j--);
    if (i < j) {
      swap(array_list, i, j);
    }
  }
  swap(array_list, left, j);
  return j;
}

void quick_sort(unsigned char *array_list, int left, int right) {
  if (left < right) {
    int j = partition(array_list, left, right);
    quick_sort(array_list, left, j - 1);
    quick_sort(array_list, j+1, right);
  }
}

unsigned char lower_quartile(unsigned char *array_list, int num_elements) {
  quick_sort(array_list, 0, num_elements - 1);
  return array_list[(num_elements - 1) / 4];
}

unsigned char median(unsigned char *array_list, int num_elements) {
  quick_sort(array_list, 0, num_elements - 1);
  return array_list[(num_elements - 1) / 2];
}

unsigned char upper_quartile(unsigned char *array_list, int num_elements) {
  quick_sort(array_list, 0, num_elements - 1);
  return array_list[3 * (num_elements - 1) / 4];
}

CvSize get_blurred_size(CvSize oldsize) {
 return cvSize(oldsize.width / BLUR_RADIUS , oldsize.height / BLUR_RADIUS);
}

void median_blur(IplImage *frame , IplImage *result) {
  if (frame->nChannels != 1) {
    fprintf(stderr, "median_blur was called with %d channels, but only supports 1 channel.\n", frame->nChannels);
    exit(EXIT_FAILURE);
  }

  int num_offsets = 2 * BLUR_RADIUS + 1;
  int num_elements = num_offsets * num_offsets;
  unsigned char array_list[num_elements];

  for (int y = 0; y < result->height; y++) {
    for (int x = 0; x < result->width; x++) {
      for (int yo = 0; yo <= num_offsets; yo++) {
        for (int xo = 0; xo <= num_offsets; xo++) {
          array_list[num_offsets * yo + xo]
            = frame->imageData[(y * BLUR_RADIUS + yo - BLUR_RADIUS) * frame->widthStep +
                                x * BLUR_RADIUS + xo - BLUR_RADIUS];
        }
      }
      result->imageData[(y * result->widthStep + x)]
        = median(array_list, num_elements);
    }
  }
}
