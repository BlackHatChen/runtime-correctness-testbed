#ifndef RCT_NAIVE_GATE_H_

#define RCT_NAIVE_GATE_H_

namespace rct {
    /**
     * @brief Naive exactly-once gate that triggers if `curr` ∈ [`start`, `end`).
     * 
     * - Point-sampling: `curr` will MISS on delta time spikes that jump over the entire window.
     * 
     * - One-shot: Once fired, it won't fire again until `Reset()` is called.
     */
    class NaiveGate {
    public:
        NaiveGate(double start, double end) : start_(start), end_(end), fired_(false) {}

        void Reset() { fired_ = false; }

        bool TryFire(double curr) {
            bool inside = (curr >= start_) && (curr < end_);
            if (!fired_ && inside) {    // Fire condition.
                fired_ = true;
                return true;
            }
            return false;
        }

    private:
        double start_;  // Inclusive window boundary.
        double end_;    // Exclusive window boundary.
        bool fired_;    // One-shot latch.
    };
}

#endif