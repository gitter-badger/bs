#ifndef BS_STATIC_FRAME_HPP
#define BS_STATIC_FRAME_HPP

#include <bs/defs.hpp>
#include <opencv2/core/mat.hpp>

namespace bs {

struct static_frame {
    explicit static_frame (const cv::Mat&, size_t);

public:
    const cv::Mat&
    operator() (const cv::Mat&);

    const cv::Mat&
    mask () const {
        return mask_;
    }

private:
    cv::Mat background_, mask_;
    size_t threshold_;
};

}

#endif // BS_STATIC_FRAME_HPP
