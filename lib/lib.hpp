#pragma once

class Foo {
public:
    auto GetTheAnswer() const {
        return answer_;
    }

private:
    int answer_ = 42;
};