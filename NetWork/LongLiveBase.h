#pragma once

#ifndef __LONGLIVEBASE_H___
#define __LONGLIVEBASE_H___

// self defined interface
class LongLiveComponent
{
public:
	virtual void update(float dt = 0.0f) = 0;
};

#endif