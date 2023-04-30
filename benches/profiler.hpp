#include <chrono>
#include <functional>

class Profiler {
public:
    Profiler() {
        reset();
    }

    void reset() {
        m_duration = std::chrono::high_resolution_clock::duration::zero();
    }

    int64_t run(size_t iteration_count, std::function<void()> func) {
        begin();
        for (size_t i = 0; i < iteration_count; i++) {
            func();
        }
        end();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(m_duration).count();
    }

private:
    std::chrono::high_resolution_clock::time_point m_prev;
    std::chrono::high_resolution_clock::duration m_duration;


    void
    begin() {
        m_prev = std::chrono::high_resolution_clock::now();
    }

    void
    end() {
        m_duration += std::chrono::high_resolution_clock::now() - m_prev;
    }
};