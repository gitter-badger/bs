// -*- mode: c++; -*-

#include <iostream>
using namespace std;

#include <opencv2/core.hpp>
using namespace cv;

#include <benchmark/benchmark.h>
using namespace benchmark;

#include <bs/detail/lbp.hpp>

////////////////////////////////////////////////////////////////////////

//
// A vanilla reference implementation:
//
static cv::Mat
f (const cv::Mat& src)
{
    static constexpr size_t U [] = {
        0, 1, 1, 2, 1, 9, 2, 3, 1, 9, 9, 9, 2, 9, 3, 4,
        1, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 3, 9, 4, 5,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        2, 9, 9, 9, 9, 9, 9, 9, 3, 9, 9, 9, 4, 9, 5, 6,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        3, 9, 9, 9, 9, 9, 9, 9, 4, 9, 9, 9, 5, 9, 6, 7,
        1, 2, 9, 3, 9, 9, 9, 4, 9, 9, 9, 9, 9, 9, 9, 5,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 6,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 7,
        2, 3, 9, 4, 9, 9, 9, 5, 9, 9, 9, 9, 9, 9, 9, 6,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 7,
        3, 4, 9, 5, 9, 9, 9, 6, 9, 9, 9, 9, 9, 9, 9, 7,
        4, 5, 9, 6, 9, 9, 9, 7, 5, 6, 9, 7, 6, 7, 7, 8
    };

    cv::Mat dst (src.size (), CV_8UC1);

    for (int i = 1; i < src.rows - 1; ++i) {
        for (int j = 1; j < src.cols - 1; ++j) {
            unsigned char c = src.at< unsigned char > (i, j);

            unsigned char value =
                ((src.at< unsigned char > (i - 1, j - 1) > c) << 7) |
                ((src.at< unsigned char > (i - 1, j    ) > c) << 6) |
                ((src.at< unsigned char > (i - 1, j + 1) > c) << 5) |
                ((src.at< unsigned char > (i,     j + 1) > c) << 4) |
                ((src.at< unsigned char > (i + 1, j + 1) > c) << 3) |
                ((src.at< unsigned char > (i + 1, j    ) > c) << 2) |
                ((src.at< unsigned char > (i + 1, j - 1) > c) << 1) |
                ((src.at< unsigned char > (i,     j - 1) > c) << 0);

            dst.at< unsigned char > (i, j) = U [value];
        }
    }

    return dst;
}

static void
BM_ref (benchmark::State& state) {
    Mat src (state.range (0), state.range (0), CV_8U);

    while (state.KeepRunning ()) {
        DoNotOptimize (f (src));
    }
};

////////////////////////////////////////////////////////////////////////

static void
BM_lbp (benchmark::State& state) {
    Mat src (state.range (0), state.range (0), CV_8U);

    while (state.KeepRunning ()) {
        DoNotOptimize (bs::detail::lbp (src));
    }
};

int main (int argc, char** argv) {
    RegisterBenchmark ("BM_lbp", &BM_lbp)->RangeMultiplier(2)->Range(128, 16384);
    RegisterBenchmark ("BM_ref", &BM_ref)->RangeMultiplier(2)->Range(128, 16384);

    Initialize (&argc, argv);
    RunSpecifiedBenchmarks ();
}
