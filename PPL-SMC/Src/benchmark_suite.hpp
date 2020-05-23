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

#define BENCHMARK_PRINTRESULTS(name, a)                                           \
        do {                                                                      \
            int i;                                                                \
            std::cout << "[" << name << "_results]" << std::endl;                 \
            std::sort(std::begin(a), std::end(a));                                \
            double median = a[a.size() / 2] * 1000.0;                             \
            double avg = (a.sum() / ((double) a.size())) * 1000.0;                \
            double variance = (avg - (a[0] * 1000.0)) * (avg - (a[0] * 1000.0));  \
            std::cout << "no_of_iterations = " << a.size() << std::endl;          \
            std::cout << "median_ms = " << (median) << std::endl;                 \
            std::cout << "longest_run_ms = " << (a.max() * 1000.0) << std::endl;  \
            std::cout << "shortest_run_ms = " << (a.min() * 1000.0) << std::endl; \
            std::cout << "average_ms = " << (avg) << std::endl;                   \
            std::cout << "points = [" << (a[0] * 1000.0);                         \
            for (i = 1; i < (int) a.size(); ++i) {                                \
                std::cout << ", " << (a[i] * 1000.0);                             \
                variance += (avg - (a[i] * 1000.0)) * (avg - (a[i] * 1000.0));    \
            }                                                                     \
            std::cout << "]" << std::endl;                                        \
            variance /= (double) (a.size() - 1);                                  \
            std::cout << "variance_ms = " << variance << std::endl;               \
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
            BENCHMARK_PRINTRESULTS("iteration", iter_results);                  \
            std::cout << std::endl;                                             \
            BENCHMARK_PRINTRESULTS("warmup", warmup_results);                   \
        } while (0)

#endif /* BENCHMARK_SUITE_HPP */
