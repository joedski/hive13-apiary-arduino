Animation Analysis
==================

In the original, the delays were as such:

- NeoPixel:
	- 10 ms
- Servos:
	- 15 ms after servo 1
	- 15 ms after servo 2

Thus, in total, between each tick, the total delay was 40 ms, or about 25 tps, minus however long it takes the NeoPixel and Servo writes to take place.  This will be the basis of deriving the new animation timings from the old.

In this one, we're going to shoot for about 15 ms/tick, or about 60 tps.



Maintaining Timing
------------------

One thing I may implement is to try to keep things to 15 milliseconds as much as possible.  This means taking the time before the runloop and then again afterwards, subtracting the two, and waiting 15ms less that difference, or 0ms if it turns out the runloop took more than 15 ms.

The other option is that it could just run through the runloop as fast as possible and everything is updated according to the time.  However, at least 15 ms is needed to ensure the servos are properly positioned, so just trying to run it as fast as possible may not be feasible.  It could be made free wheeling, but position data would have to be actually written every so often, however the delay for he servos as originally written was noted to allow time for the servos to actually reach their position.

I could go on the assumption that that's a worst case scenario, and that just moving to the next step takes less than 15 ms. (15ms / 100steps = 0.15ms/step)  A possible addition is something that uses that maximum timing to calculate a theoretical position and to only write a new position once the theoretical position is small enough that the time to reach the next step is less than 1 ms, but that's a bit too complicated for now.

For now, just running the runloop then waiting out the remainder of 15ms will do.



Servos
------

The Servos moved 1 ~ 6 steps every 40ms, based on the light reading.  Uncovered meant 1 step, covered meant 6.  This means that to cover the range of 100 steps takes 4 seconds when the sensor is uncovered, and 2/3 seconds when covered.

Translating these to rates gives us:

- Uncovered: 100 steps / 4 seconds = 25 steps / second
- Covered: 100 steps / (2/3) seconds = 150 steps / second



NeoPixel
--------

I don't know how long it takes to write out the pixels, but it's at least several microseconds.  I don't know if it gets up to milliseconds or not, though.

In the original code, the way the pixels controlled timing was to just skip variable-changes until a certain number of ticks passed, with that number of ticks determined by the light sensor.  However, pixels were still written to the strip every time tick.

The mapping here of the light reading to var-update-skip timing is simpler compared the servos' mapping.  Here, when occluded, the target value to count to is 10.  When the sensor is unoccluded, then the value is 100.  Note, however, that the increment amount for the accumulator is also 10, so that when occluded, updates are only skipped for 1 tick; when unoccluded, 10 ticks.

This meant the pixels were updated every 2 ticks, or about every 80 ms; or every 11 tickes, or about 440 ms.

### New Animation

> Note: This can apply to the Servos, as well.  This same animation framework will be used there.

One thing that we know is going to stay the same: The input parameter is the Light Sensor Reading, and the Light Sensor Reading is mapped to the Animation Progress Rate.

> **Regarding Animation Progress**: Ints on the Uno are 16 bit (-32768 ~ 32767 or 0 ~ 65535).  Supposing we used an unsigned int, 65535 over 4000 ms is ~16 points per ms, giving us an effective resolution of 1/16 of a ms, which is more than is necessary.  At 15ms, this is ~240 (245.76) ticks.  Even a signed int provides ~8 points per ms, or ~120 per 15 ms.

Since the Progress is now determinable, we can parameterize the animation to that.  We'll start with the animation's longest time, and take that as 1x rate.  Faster will then be a higher rate multiplier.

The model will look something like this:

```c
#define ANIMATION_MAX_PROGRESS 65535

struct AnimationProgressModel {
	// The current progress of the animation.  Will wrap around to 0.
	unsigned int progress;
	// The duration in ms of the animation when playing at 1x rate.
	unsigned long normalDuration;
};

// Note that progress increment is a SIGNED int.  Animations can run backwards!
// Though, this function doesn't actually do that, it's the next one that can flip the sign.
int normalProgressIcrementForDelta( struct AnimationProgressModel model, unsigned long msDelta ) {
	return (int)(((unsigned long) ANIMATION_MAX_PROGRESS) * msDelta / model.normalDuration);
}

// rate100 is the rate * 100.  Fixed point number for integer maths.
// As said before, it can run backwards if rate100 is negative.
int progressIncrementForRate( int progressIncrement, int rate100 ) {
	// all integer maths.
	return progressIncrement * rate100 / 100;
}
```

### Theatre Chaser Animation

First, a technical note.  The chaser animation + colors is best done as two separate sub-animations, one which rotates through the color wheel, the other which determines which pixels are actually on and which aren't.

#### Color Wheeling

Determining the color to use at any point in the animation is trivial: just cycle across the rainbow as the animation progresses.  The duration of the animation can then be adjusted to taste independent of the chaser animation.

#### Chasing

Determining the marquee animation is trickier.  Originally, lights changed every time the variables updated, which meant every 80ms ~ 440ms.  The offset between pixels was 3, meaning every third pixel was on, and thus there were 2 pixels between that were off.  This sub-animation then can be taken to be just 1320ms long at 1x rate.

Shortening it to the smallest possible cycle duration means that it's easy to determine just when each pixel is on or off.  In fact, it could be done easier by just animating a parameter then combining the parameter with the color parameter to determine the pixel later.  Not that it matters too much since NeoPixel only writes to the LED strip when explicitly told.  Good practice, though, so doing that anyway.  This also means that it could be changed to 2 or 4 pixels later, but 3 is really about the best for chasers.  2 is more of a strobe.
