#include "tag/TagFactory.h"
#include "tag/ConcreteTags.h"
#include <cstdlib>

std::shared_ptr<Tag> TagFactory::CreateTag(TagType type) {
    switch (type) {
        case TagType::HANDY:
            return std::make_shared<HandyTag>();
        case TagType::ECONOMY:
            return std::make_shared<EconomyTag>();
        case TagType::ORBITAL:
            return std::make_shared<OrbitalTag>();
        default:
            return nullptr;
    }
}

std::shared_ptr<Tag> TagFactory::CreateRandomTag() {
    int r = std::rand() % 3;
    return CreateTag(static_cast<TagType>(r));
}
