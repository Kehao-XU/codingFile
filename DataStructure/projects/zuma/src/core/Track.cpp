#include "core/Track.h"

namespace zuma::core {

Track::iterator Track::PositionOf(std::size_t position) {
    if (position < 1) {
        return balls_.end();
    }
    iterator it = balls_.begin();
    for (std::size_t index = 1; index < position && it != balls_.end(); ++index) {
        ++it;
    }
    return it;
}

Track::const_iterator Track::PositionOf(std::size_t position) const {
    if (position < 1) {
        return balls_.end();
    }
    const_iterator it = balls_.begin();
    for (std::size_t index = 1; index < position && it != balls_.end(); ++index) {
        ++it;
    }
    return it;
}

Track::iterator Track::Insert(std::size_t position, Ball ball) {
    return balls_.insert(PositionOf(position), ball);
}

Track::iterator Track::Append(Ball ball) {
    return balls_.insert(balls_.end(), ball);
}

Track::iterator Track::Erase(iterator first, iterator last) {
    return balls_.erase(first, last);
}

bool Track::HasRunOfThreeOrMore() const {
    std::size_t runLength = 0;
    char previous = '\0';
    for (const Ball& ball : balls_) {
        if (runLength != 0 && ball.color == previous) {
            ++runLength;
        } else {
            runLength = 1;
            previous = ball.color;
        }
        if (runLength >= 3) {
            return true;
        }
    }
    return false;
}

}  // namespace zuma::core
