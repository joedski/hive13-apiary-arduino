
/*
Write the top-level runloop procedure with this.

RUNLOOP_PROC( lights ) {
	updateLights();
	boopLights();
}

RUNLOOP_PROC_ARRAY( runloop ) = {
	RUNLOOP_PROC_PTR( lights ),
	RUNLOOP_PROC_PTR( sensor ),
	RUNLOOP_PROC_PTR( action ),
	NULL
};

loop() {
	RUNLOOP_PROC_PRT( proc ) = NULL;
	int i = 0;

	while( 1 ) {
		proc = runloop[ i ];

		if( proc == NULL ) {
			break;
		}

		proc();

		++i;
	}
}
 */

#ifndef __APIARY_MACROS__
#	define __APIARY_MACROS__ yay



// Base macros so we can make sure all pointers play nice with eachother.
#	define RUNLOOP_PROC_NAME(procName) procName##_loop
#	define RUNLOOP_PROC_ARGS ( struct ApiaryState state )
#	define RUNLOOP_PROC_RETURN_TYPE void

// Use these.
#	define RUNLOOP_PROC(procName) RUNLOOP_PROC_RETURN_TYPE RUNLOOP_PROC_NAME(procName) RUNLOOP_PROC_ARGS
#	define RUNLOOP_PROC_PTR(procName) RUNLOOP_PROC_RETURN_TYPE (* RUNLOOP_PROC_NAME(procName) ) RUNLOOP_PROC_ARGS
#	define RUNLOOP_PROC_ARRAY(arrayName) RUNLOOP_PROC_RETURN_TYPE (* arrayName []) RUNLOOP_PROC_ARGS



// Setup Macros.  Separate so that they can have different return and argument types from the runloop macros.
#	define SETUP_PROC_NAME(procName) procName##_setup
#	define SETUP_PROC_ARGS ()
#	define SETUP_PROC_RETURN_TYPE void

// Use these.
#	define SETUP_PROC(procName) SETUP_PROC_RETURN_TYPE SETUP_PROC_NAME(procName) SETUP_PROC_ARGS
#	define SETUP_PROC_PTR(procName) SETUP_PROC_RETURN_TYPE (* SETUP_PROC_NAME(procName) ) SETUP_PROC_ARGS
#	define SETUP_PROC_ARRAY(arrayName) SETUP_PROC_RETURN_TYPE (* arrayName []) SETUP_PROC_ARGS



#endif
