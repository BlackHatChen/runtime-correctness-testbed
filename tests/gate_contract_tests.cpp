#include "rct/naive_gate.h"
#include "rct/deterministic_one_shot_gate.h"

#include <gtest/gtest.h>

TEST(DeterministicGate, NoMissOnDtSpike) {
    const double start = 1.0;
    const double end = 1.2;

    rct::NaiveGate naive(start, end);
    rct::DeterministicOneShotGate det(start, end);

    double progress = 0.0;
    det.TryFire(progress);  // Initialize previous progress.

    progress = 2.1; // Make a spike: 0.0 -> 2.1 (Skip [1.0, 1.2)).

    EXPECT_FALSE(naive.TryFire(progress)) << "Naive should miss when spike jumps over the whole window.";
    EXPECT_TRUE(det.TryFire(progress)) << "Deterministic must detect crossing on spike.";
}

TEST(DeterministicGate, ExactlyOnceWithLatch) {
    const double start = 1.0;
    const double end = 1.2;

    rct::DeterministicOneShotGate det(start, end);

    double progress = 0.0;
    det.TryFire(progress);

    progress = 0.6;
    det.TryFire(progress);
    progress = 2.1;
    EXPECT_TRUE(det.TryFire(progress)); // 0.6 -> 2.1 should fire once.

    // Shouldn't fire again without `Reset()`.
    progress = 2.2;
    EXPECT_FALSE(det.TryFire(progress));
    progress = 3.0;
    EXPECT_FALSE(det.TryFire(progress));

    det.Reset();
    
    progress = 0.9;
    det.TryFire(progress);  // prev = 0.9
    progress = 1.0;
    EXPECT_TRUE(det.TryFire(progress));     // Fire again after `Reset()`.
    progress = 1.1;
    EXPECT_FALSE(det.TryFire(progress));    // The latch should block it.
}

TEST(DeterministicGate, BoundarySemanticsStartInclusiveEndExclusive) {
    const double start = 1.0;
    const double end = 1.2;

    rct::DeterministicOneShotGate det(start, end);

    double progress = 0.9;
    det.TryFire(progress);
    progress = 1.0;
    EXPECT_TRUE(det.TryFire(progress));     // Touch start: Counts for [start,end).

    det.Reset();

    progress = 1.19;
    det.TryFire(progress);
    progress = 1.2;
    EXPECT_TRUE(det.TryFire(progress));     // Intersect [start,end).

    det.Reset();

    progress = 1.2;
    det.TryFire(progress);
    progress = 2.0;
    EXPECT_FALSE(det.TryFire(progress));     // Start at the end: Not inside the window.
}