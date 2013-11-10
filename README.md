LazySimulator
=============

Simulator for MELOADES paper (http://www.sciencedirect.com/science/article/pii/S1383762113001008)

Paper abstract:

Abstract
In this work MELOADES [mel-uh-dees] is presented: a methodology for long-term online adaptation of 
embedded software that addresses the challenge of redeploying software and executing it within 
resource constraints. Instead of using fixed analytical models of resource consumption developed
offline or tuning model parameters, MELOADES automatically reconfigures hardware online without
any analytical model. MELOADES leverages long-term deployment by first selecting a set of hardware
configurations that can potentially execute software tasks while satisfying a range of resource
constraints and then storing these in a memoization table. The table is initialized using a Design
of Experiments (DoE) survey to generate these speculative configurations. During deployment, for
each new task assigned to the software, either a memoized configuration is found or a limited
search for a new configuration that satisfies the task constraints is performed. Search results
are added to the memoization table to reduce the time and energy required for future searches and
eventually MELOADES converges to a simple table look-up. The effectiveness of this technique was
demonstrated with an image capture and wireless transmission representative long-term application
deployed on a Nokia N80 smartphone. Using a genetic search algorithm for energy
efficiency/constrained image tasks, MELOADES satisfied 94% of all task constraints, evaluated only
1.6% of the configuration space, and used 98.5% less energy than an exhaustive search.
