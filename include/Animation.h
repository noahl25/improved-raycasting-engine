#pragma once

class Animation {
public:

	static void UpdateCounter() { AnimationCounter++; }
	static int Counter() { return AnimationCounter; }

private:
	static inline int AnimationCounter = 0;
};