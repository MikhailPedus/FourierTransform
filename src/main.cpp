#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "iostream"

int main() {
  std::cout << "Helloworld" << std::endl;
  std::string resources_dir = RESOURCE_DIR;
  std::string image_name = "fox.jpg";
  cv::Mat img =
      cv::imread(resources_dir + "/" + image_name, cv::IMREAD_GRAYSCALE);
  if (img.empty()) {
    std::cout << "Can't open image by path:" << resources_dir + "/" + image_name
              << std::endl;
    return -1;
  }
  cv::Mat noize();
  cv::imshow("img", img);
  cv::waitKey(0);
  return 0;
}
