set(SOURCES
	CFBackground.cpp
	CFCard.cpp
	CFPile.cpp
	CFPilable.cpp
	CFDeck.cpp
    CFAction.cpp
	CFGameMode.cpp
	CFGameState.cpp
	CFFlipAnimation.cpp)

target_include_directories(CardFramework
                           PUBLIC
                           include)

target_sources(CardFramework
		       PRIVATE
		       ${SOURCES})