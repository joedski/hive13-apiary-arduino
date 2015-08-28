
#include "animation.h"



//////// Public Functions

struct AnimationProgressModel animation_incrementProgress( struct AnimationProgressModel animationModel, unsigned long timeDelta ) {
	int normalProgressIcrement = normalProgressIcrementForDelta( animationModel, timeDelta );

	animationModel.progress += progressIncrementForRate( normalProgressIcrement, animationModel.rate100 );
	animationModel.progress %= ANIMATION_PRORGESS_MAX;

	return animationModel;
}



//////// Utils
// These are already exposed 

int normalProgressIcrementForDelta( struct AnimationProgressModel model, unsigned long msDelta ) {
	return (int)(((unsigned long) ANIMATION_MAX_PROGRESS) * msDelta / model.normalDuration);
}

// rate100 is the rate * 100.  Fixed point number for integer maths.
// As said before, it can run backwards if rate100 is negative.
int progressIncrementForRate( int progressIncrement, int rate100 ) {
	// all integer maths.
	return progressIncrement * rate100 / 100;
}
