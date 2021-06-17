#pragma once
#include <opencv2/core/core.hpp>

namespace image_helper {
cv::Mat distortImageRadialAndTangential(const cv::Mat& undistortedImage, const float radialDistortionParams[4],
    const float tangentialDistortionParams[2], const float skew, const float scale = 1.0f);
cv::Mat distortImageAngular(
    const cv::Mat& undistortedImage, const float distortionParams[4], const float skew, const float scale = 1.0f);
cv::Mat distortImageOmniMeiRives(const cv::Mat& undistortedImage, const float radialDistortionParams[2],
    const float tangentialDistortionParams[2], const float xi, const float skew, const float scale = 1.0f);
}
