#include "noize_map_generator/noize_map_generator.h"

#include <iostream>

NoizeMapVetrical::NoizeMapVetrical(int frequency) : frequency_(frequency) {}

cv::Mat NoizeMapVetrical::Generate(const cv::Size& size) const {
  if (size.width < frequency_ || size.height == 0) {
    std::cout << "Can't generate noize map with cols count less then "
                 "<frequency_> or with zero rows"
              << std::endl;
    cv::Mat();
  }
  cv::Mat noize_map(size, CV_8UC1, cv::Scalar::all(0));

  for (auto row = 0; row < noize_map.rows; ++row) {
    auto* ptr = noize_map.ptr<uchar>(row);

    for (int cols = 0; cols < noize_map.cols;) {
      for (int local_cols = 0;
           local_cols < frequency_ && cols + local_cols < noize_map.cols;
           ++local_cols) {
        double coeff = static_cast<double>(local_cols % frequency_) /
                       static_cast<double>(frequency_);
        ptr[cols + local_cols] = static_cast<uchar>(255 * coeff);
      }
      cols += frequency_;
      for (int local_cols = 0;
           local_cols < frequency_ && cols + local_cols < noize_map.cols;
           ++local_cols) {
        double coeff = static_cast<double>(local_cols % frequency_) /
                       static_cast<double>(frequency_);
        ptr[cols + local_cols] = 255u - static_cast<uchar>(255 * coeff);
      }
      cols += frequency_;
    }
  }
  return noize_map;
}
