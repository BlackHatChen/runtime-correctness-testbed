#ifndef RCT_DETERMINISTIC_ONE_SHOT_GATE_H_

#define RCT_DETERMINISTIC_ONE_SHOT_GATE_H_
#include <optional>

namespace rct {
    /**
     * @brief Deterministic exactly-once gate that triggers if [`prev`, `curr`] cross [`start`, `end`).
     * 
     * - No-miss-on-skip: Even if `prev` < `start` and `curr` >= `end` (delta time spike), it still trigger.
     * 
     * - One-shot: Once fired, it won't fire again until `Reset()` is called.
     */
    class DeterministicOneShotGate {
    public:
        DeterministicOneShotGate(double start, double end) : start_(start), end_(end), fired_(false), prev_() {}

        void Reset() {
            fired_ = false;
            prev_.reset();
        }

        bool TryFire(double curr) {
            if (!prev_.has_value() || fired_) {
                prev_ = curr;
                return false;
            }

            bool crosses = (prev_.value() < end_) && (curr >= start_);

            prev_ = curr;   // Update previous progress by current's.

            if (crosses) {   // Fire condition.
                fired_ = true;
                return true;
            }
            return false;
        }
        
    private:
        double start_;                  // Inclusive window boundary.
        double end_;                    // Exclusive window boundary.
        bool fired_;                    // One-shot latch.
        std::optional<double> prev_;    // Previous progress. Unset before first `Step()`.
    };
}

#endif