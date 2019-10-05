#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <complex>

#include "noize_map_generator/noize_map_generator.h"

void PrintHelp() {
  std::cout << "You're just not doing anything right!" << std::endl;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    PrintHelp();
    return -1;
  }
  std::string resources_dir = RESOURCE_DIR;
  std::string image_name(argv[1]);
  cv::Mat img =
      cv::imread(resources_dir + "/" + image_name, cv::IMREAD_GRAYSCALE);
  if (img.empty()) {
    std::cout << "Can't open image by path:" << resources_dir + "/" + image_name
              << std::endl;
    return -1;
  }

  cv::resize(img, img, cv::Size(500, 500));

  std::unique_ptr<NoizeMapGenerator> noize_generator(new NoizeMapVetrical(50));

  cv::Mat noize_img = noize_generator->Generate(img.size());

  if (noize_img.empty() || noize_img.size() != img.size() ||
      noize_img.type() != CV_8UC1) {
    std::cout << "Can't generate correct noize map, sorry." << std::endl;
    return -1;
  }

  img /= 2;
  noize_img /= 2;
  cv::add(img, noize_img, img);

  cv::imshow("img", img);
  cv::imshow("noize", noize_img);

  cv::Mat noize_float;
  noize_img.convertTo(noize_float, CV_32FC1);
  cv::normalize(noize_float, noize_float, 0, 1, cv::NORM_MINMAX);

  cv::Mat img_float;
  img.convertTo(img_float, CV_32FC1);
  cv::normalize(img_float, img_float, 0, 1, cv::NORM_MINMAX);

  cv::Mat draw_complex(500, 500, CV_8UC3, cv::Scalar::all(0));
  const double pi = std::acos(-1);

  std::vector<std::pair<double, std::complex<double>>> mean_weith;
  mean_weith.reserve(100);

  cv::Mat& explore_img = img_float;
  for (double frequency = 1; frequency < 25; frequency += 0.25) {
    std::cout << "explore " << frequency << " frequency.." << std::endl;
    std::vector<std::complex<double>> result_points;
    result_points.reserve(static_cast<size_t>(explore_img.cols));
    draw_complex.setTo(0);
    for (int i = 0; i < explore_img.cols; ++i) {
      double time_elapsed =
          static_cast<double>(i) / static_cast<double>(explore_img.cols);
      auto complex_result_exp =
          std::exp(std::complex<double>(0, -2 * pi * time_elapsed * frequency));
      auto result = complex_result_exp *
                    std::complex<double>(static_cast<double>(
                        explore_img.at<float>(explore_img.rows / 2, i)));
      result_points.emplace_back(result);
      cv::circle(draw_complex,
                 cv::Point(draw_complex.cols / 2 + result.real() * 100,
                           draw_complex.rows / 2 - result.imag() * 100),
                 2, cv::Scalar(0, 0, 250), -1);
      cv::imshow("d", draw_complex);
      cv::waitKey(1);
    }
    double all_real_summ = 0;
    double all_imag_summ = 0;
    for (const auto& i : result_points) {
      all_real_summ += i.real();
      all_imag_summ += i.imag();
    }
    mean_weith.emplace_back(frequency,
                            std::complex<double>(all_real_summ, all_imag_summ));
    std::cout << "\tresult is:" << mean_weith.back().second << std::endl;
  }

  cv::Mat draw_spectr(500, 1000, CV_8UC3, cv::Scalar::all(0));
  double x_coeff = 20;
  double y_coeff = 2;
  for (size_t i = 1; i < mean_weith.size(); ++i) {
    cv::line(
        draw_spectr,
        cv::Point(
            mean_weith[i - 1].first * x_coeff,
            draw_spectr.rows / 4 + mean_weith[i - 1].second.real() * y_coeff),
        cv::Point(mean_weith[i].first * x_coeff,
                  draw_spectr.rows / 4 + mean_weith[i].second.real() * y_coeff),
        cv::Scalar(0, 0, 250), 2);
    cv::line(draw_spectr,
             cv::Point(mean_weith[i - 1].first * x_coeff,
                       draw_spectr.rows / 4 * 3 +
                           mean_weith[i - 1].second.imag() * y_coeff),
             cv::Point(mean_weith[i].first * x_coeff,
                       draw_spectr.rows / 4 * 3 +
                           mean_weith[i].second.imag() * y_coeff),
             cv::Scalar(0, 250, 0), 2);
  }

  cv::imshow("spectr", draw_spectr);
  cv::waitKey(0);
  return 0;
}
