#pragma once
#include <string>
#include <memory>

class RuntimeSession;

enum class TagTrigger {
    NEXT_BLIND,
    NEXT_ANTE,
    ENTER_SHOP
};

class Tag {
public:
    virtual ~Tag() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual TagTrigger getTrigger() const = 0;
    virtual void execute(RuntimeSession& session) = 0;
};
