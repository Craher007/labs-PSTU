#pragma once

#include "Vector.h"

class Dialog : public Vector
{
public:
    Dialog();
    ~Dialog() override = default;

    void GetEvent(TEvent& event);
    int Execute();
    int Valid() const {  return EndState != 0; }
    void ClearEvent(TEvent& event) { event.what = evNothing; }
    void EndExec() { EndState = 1; }
    void HandleEvent(TEvent& event);

protected:
    int EndState;
};
