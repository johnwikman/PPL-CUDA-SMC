/**
 * Benchmark suite for reference implementations.
 */

#ifndef BENCHMARK_SUITE_HPP
#define BENCHMARK_SUITE_HPP

#include <algorithm>
#include <valarray>
#include <chrono>
#include <iostream>

#ifndef _WARMUPS_
#define _WARMUPS_ (4)
#endif

#ifndef _ITERS_
#define _ITERS_ (15)
#endif

#define BENCHMARK_RUNONCE(target, fn_prep, fn_run, fn_post) \
        do {                                                \
            fn_prep();                                      \
            auto start = std::chrono::system_clock::now();  \
            fn_run();                                       \
            auto end = std::chrono::system_clock::now();    \
            std::chrono::duration<double> d = end - start;  \
            target = d.count();                             \
            fn_post();                                      \
        } while (0)

#define BENCHMARK_PRINTRESULTS(name, a)                                                \
        do {                                                                           \
            std::cout << "== " << name << " Results ==" << std::endl;                  \
            std::sort(std::begin(a), std::end(a));                                     \
            double median = a[a.size() / 2];                                           \
            double avg = a.sum() / ((double) a.size());                                \
            double variance = std::max(a.max() - avg, avg - a.min());                  \
            std::cout << "No. of iterations: " << a.size() << std::endl;               \
            std::cout << "Median: " << (median * 1000.0) << " ms" << std::endl;        \
            std::cout << "Longest run: " << (a.max() * 1000.0) << " ms" << std::endl;  \
            std::cout << "Shortest run: " << (a.min() * 1000.0) << " ms" << std::endl; \
            std::cout << "Average: " << (avg * 1000.0) << " ms" << std::endl;          \
            std::cout << "Variance: +-" << (variance * 1000.0) << " ms" << std::endl;  \
        } while (0)

#define BENCHMARK(fn_prep, fn_run, fn_post)                                     \
        do {                                                                    \
            int i;                                                              \
            std::valarray<double> warmup_results{1.0};                          \
            std::valarray<double> iter_results{1.0};                            \
            warmup_results.resize(_WARMUPS_);                                   \
            iter_results.resize(_ITERS_);                                       \
            for (i = 0; i < _WARMUPS_; ++i) {                                   \
                BENCHMARK_RUNONCE(warmup_results[i], fn_prep, fn_run, fn_post); \
            }                                                                   \
            for (i = 0; i < _ITERS_; ++i) {                                     \
                BENCHMARK_RUNONCE(iter_results[i], fn_prep, fn_run, fn_post);   \
            }                                                                   \
            BENCHMARK_PRINTRESULTS("Iteration", iter_results);                  \
            std::cout << std::endl;                                             \
            BENCHMARK_PRINTRESULTS("Warmup", warmup_results);                   \
        } while (0)

#endif /* BENCHMARK_SUITE_HPP */
