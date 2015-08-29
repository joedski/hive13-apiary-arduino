#ifndef __ANIMATION_H__
#define __ANIMATION_H__ yay



#define ANIMATION_PRORGESS_MAX 65535

struct AnimationTimingModel {
	// Current progress of the animation.
	unsigned int progress;
	// The duration in milliseconds of the animation when playing at 1x rate.
	unsigned long normalDuration;
	// The rate at which this animation is progressing.
	// This value is equal to the actual rate * 100, so that we can store fractional rates.
	int rate100;
};

// Increments the animation progress by a certain delta.  Takes into account the animation's current rate.
struct AnimationTimingModel animation_incrementProgress( struct AnimationTimingModel animationModel, unsigned long timeDelta );



#endif
