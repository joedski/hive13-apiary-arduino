
#include "animation.h"



//////// Prototypes

int normalProgressIcrementForDelta( struct AnimationTimingModel model, unsigned long msDelta );
int progressIncrementForRate( int progressIncrement, int rate100 );



//////// Public Functions

struct AnimationTimingModel animation_incrementProgress( struct AnimationTimingModel animationModel, unsigned long timeDelta ) {
	int normalProgressIcrement = normalProgressIcrementForDelta( animationModel, timeDelta );

	animationModel.progress += progressIncrementForRate( normalProgressIcrement, animationModel.rate100 );
	animationModel.progress %= ANIMATION_PRORGESS_MAX;

	return animationModel;
}



//////// Utils

int normalProgressIcrementForDelta( struct AnimationTimingModel model, unsigned long msDelta ) {
	return (int)(((unsigned long) ANIMATION_PRORGESS_MAX) * msDelta / model.normalDuration);
}

// rate100 is the rate * 100.  Fixed point number for integer maths.
// As said before, it can run backwards if rate100 is negative.
int progressIncrementForRate( int progressIncrement, int rate100 ) {
	// all integer maths.
	return progressIncrement * rate100 / 100;
}
