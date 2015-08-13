This is the CSUN Nothing But Net Repository.
It is currently in its infancy, meaning we have quite a lot of work to do.

Currently simple mapping is already integrated into the robotBase code,
we must also create PID movements for autonomous and work on the existing mapping code.

A plan is being created to use a arduino like board to communicate through i2c with the cortex so that
we may have more accurate sensors for angle and possibly position.

Current Issues:
-Making the mapping function more accurate and to eventually have it give position with cm accuracy.

-Implement PID functions for movement including turns and forward driving, S curve movements may be implemented
 when the mapping is accurate enough.
 
-Implement the base for an aim funtion which includes a turn funtion and finding the distance between two points,
 the final steps will be completing the kinematics equation.
 
Extra
-----
In the future we would like to use some vision system to check for objects or obstacles this will be a final phase
at which our robot's swagger will be so high that it may just leave the team and ride off into the sunset.

If you have any suggestions please email Kevin Joyce at kpjoyce1@gmail.com or call his cell phone (323) 377-6042