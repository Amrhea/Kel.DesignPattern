#include "catch.hpp"
#include "session/RuntimeSession.h"
#include "tag/TagFactory.h"
#include "tag/ConcreteTags.h"

TEST_CASE("Tag and TagFactory Tests", "[tags]") {
    SECTION("TagFactory generates correct tag types") {
        auto handy = TagFactory::CreateTag(TagType::HANDY);
        REQUIRE(handy != nullptr);
        REQUIRE(handy->getName() == "Handy Tag");
        REQUIRE(handy->getTrigger() == TagTrigger::NEXT_BLIND);

        auto economy = TagFactory::CreateTag(TagType::ECONOMY);
        REQUIRE(economy != nullptr);
        REQUIRE(economy->getName() == "Economy Tag");
        REQUIRE(economy->getTrigger() == TagTrigger::ENTER_SHOP);

        auto orbital = TagFactory::CreateTag(TagType::ORBITAL);
        REQUIRE(orbital != nullptr);
        REQUIRE(orbital->getName() == "Orbital Tag");
        REQUIRE(orbital->getTrigger() == TagTrigger::NEXT_ANTE);
    }

    SECTION("Stacking similar tags and executing them all at once") {
        RuntimeSession session;
        session.gold = 10;
        
        // Add 3 Economy Tags to the stack (these trigger on ENTER_SHOP)
        session.tagStack.push_back(TagFactory::CreateTag(TagType::ECONOMY));
        session.tagStack.push_back(TagFactory::CreateTag(TagType::ECONOMY));
        session.tagStack.push_back(TagFactory::CreateTag(TagType::ECONOMY));

        REQUIRE(session.tagStack.size() == 3);

        // Trigger ENTER_SHOP tags
        auto logs = session.triggerTags(TagTrigger::ENTER_SHOP);
        
        // All 3 tags should trigger and execute at once
        REQUIRE(logs.size() == 3);
        REQUIRE(session.gold == 10 + (5 * 3)); // 10 + 15 = 25
        REQUIRE(session.tagStack.size() == 0); // Stack should be empty
    }

    SECTION("Stacking different tags and triggering only the matching ones") {
        RuntimeSession session;
        session.gold = 5;
        session.remainingPlays = 4;

        session.tagStack.push_back(TagFactory::CreateTag(TagType::ECONOMY));
        session.tagStack.push_back(TagFactory::CreateTag(TagType::HANDY));
        session.tagStack.push_back(TagFactory::CreateTag(TagType::ECONOMY));

        REQUIRE(session.tagStack.size() == 3);

        // Trigger NEXT_BLIND (only Handy Tag should trigger)
        auto blindLogs = session.triggerTags(TagTrigger::NEXT_BLIND);
        REQUIRE(blindLogs.size() == 1);
        REQUIRE(session.remainingPlays == 5);
        REQUIRE(session.gold == 5); // gold unchanged
        REQUIRE(session.tagStack.size() == 2); // 2 Economy Tags remain

        // Trigger ENTER_SHOP (both Economy Tags should trigger)
        auto shopLogs = session.triggerTags(TagTrigger::ENTER_SHOP);
        REQUIRE(shopLogs.size() == 2);
        REQUIRE(session.gold == 5 + 10); // 15
        REQUIRE(session.tagStack.size() == 0); // all tags cleared
    }
}
