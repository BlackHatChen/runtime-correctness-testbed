#include "rct/naive_gate.h"
#include "rct/deterministic_one_shot_gate.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

int main() {
    // ------------------------------------------------------
    // Parameters
    // ------------------------------------------------------
    const double kStart = 1.0;
    const double kEnd = 1.2;
    const int kExpectedEvents = 1;

    // Fixed delta time sequence: Produce a spike that crosses the entire window.
    const std::vector<double> kDt = {
        0.30,   // 0.00 -> 0.30
        0.30,   // 0.30 -> 0.60
        1.50,   // 0.60 -> 2.10 (Spike crossing [1.0, 1.2))
        0.10    // 2.10 -> 2.20
    };

    // ------------------------------------------------------
    // Gates
    // ------------------------------------------------------
    rct::NaiveGate naive(kStart, kEnd);
    rct::DeterministicOneShotGate det(kStart, kEnd);

    // ------------------------------------------------------
    // Execute codes
    // ------------------------------------------------------
    double progress = 0.0;
    int naive_fires = 0;
    int det_fires = 0;

    // Deterministic gate needs to record previous progress first.
    det.TryFire(progress);

    for (double dt : kDt) {
        progress += dt;
        if (naive.TryFire(progress)) ++naive_fires;
        if (det.TryFire(progress)) ++det_fires;
    }

    // ------------------------------------------------------
    // Print results
    // ------------------------------------------------------
    const int naive_miss = std::max(0, kExpectedEvents - naive_fires);
    const int det_miss = std::max(0, kExpectedEvents - det_fires);
    const int double_count = std::max(0, det_fires - kExpectedEvents);  // Check if fire count is exactly-once.
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[scenario_missed_trigger] window=[" << kStart << ", " << kEnd << ")\n";
    std::cout << "expected_events: " << kExpectedEvents << "\n";
    std::cout << "naive_fires: " << naive_fires << "\n";
    std::cout << "deterministic_fires: " << det_fires << "\n";
    std::cout << "naive_miss_count: " << naive_miss << "\n";
    std::cout << "deterministic_miss_count: " << det_miss << "\n";
    std::cout << "double_trigger_count: " << double_count << "\n";
    
    return 0; 
}