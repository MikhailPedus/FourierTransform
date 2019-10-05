#ifndef NOIZE_MAP_GENERATOR_H
#define NOIZE_MAP_GENERATOR_H

#include "macros.h"

#include <opencv2/core.hpp>

class NoizeMapGenerator {
public:
  NoizeMapGenerator() = default;
  virtual ~NoizeMapGenerator() = default;

  virtual cv::Mat Generate(const cv::Size& size) const  = 0;

private:
  DISALLOW_COPY_AND_ASSIGN(NoizeMapGenerator);
};

class NoizeMapVetrical : public NoizeMapGenerator {
public:
  NoizeMapVetrical(int frequency);
  virtual ~NoizeMapVetrical() override = default;

  virtual cv::Mat Generate(const cv::Size& size) const override;

private:
  const int frequency_;
  DISALLOW_COPY_AND_ASSIGN(NoizeMapVetrical);
};


#endif  // NOIZE_MAP_GENERATOR_H
