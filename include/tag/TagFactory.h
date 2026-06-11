#pragma once
#include <memory>
#include "tag/Tag.h"

enum class TagType {
    HANDY,
    ECONOMY,
    ORBITAL
};

class TagFactory {
public:
    static std::shared_ptr<Tag> CreateTag(TagType type);
    static std::shared_ptr<Tag> CreateRandomTag();
};
