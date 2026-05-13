#pragma once

#include "Event.h"

class Object
{
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void Show() const = 0;
    virtual void Input() = 0;
    virtual void HandleEvent(const TEvent& event) = 0;
};
